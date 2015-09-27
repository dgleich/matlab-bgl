/** @file statistics.cc
 * @author David F. Gleich
 * @date 2006-04-21
 * @copyright Stanford University, 2006-2008
 * Graph statistics wrappers
 */

/** History
 *  2007-04-18: Implemented topological order function
 *  2007-07-05: Implemented matchings
 *  2007-07-05: Implemented core_numbers
 *  2007-07-11: Implemented directed and weighted clustering coefficients
 *  2007-07-12: Implemented dominator tree
 *  2011-11-30: Corrected betweenness centrality with property maps
 *  2011-12-03: Moved some functions into structure.cc
 */

#include "include/matlab_bgl.h"

#include <yasmic/simple_csr_matrix_as_graph.hpp>
#include <yasmic/simple_row_and_column_matrix_as_graph.hpp>
#include <yasmic/iterator_utility.hpp>

#include <vector>

#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/betweenness_centrality.hpp>
#include <boost/graph/topological_sort.hpp>

#include <boost/iterator/reverse_iterator.hpp>

#include <boost/graph/max_cardinality_matching.hpp>
#include <boost/graph/core_numbers.hpp>
#include <boost/graph/dominator_tree.hpp>
#include <boost/graph/bipartite.hpp>
#include <boost/graph/bandwidth.hpp>

#include <iostream>

#include <math.h>

template <class Vertex, class IndMap>
struct in_indicator_pred
	: public std::unary_function<Vertex, bool>
{
	in_indicator_pred(IndMap indmap, Vertex src)
		: i(indmap), u(src)
	{}

	IndMap i;
    Vertex u;

	bool operator() (const Vertex &v) const
	{
		return (i[v] > 0) && (u != v);
	}
};

/* Clustering Coefficients Code */
template <class Graph, class CCMap, class IndMap>
void cluster_coefficients(const Graph& g, CCMap cc, IndMap ind)
{
	using namespace boost;

	BGL_FORALL_VERTICES_T(v,g,Graph)
	{
		cc[v] = 0;
		ind[v] = 0;
	}


	BGL_FORALL_VERTICES_T(v,g,Graph)
	{
		BGL_FORALL_ADJ_T(v,w,g,Graph)
		{
			ind[w] = 1;
		}

        ind[v] = 0;

		typename property_traits<CCMap>::value_type cur_cc = 0;
		typename graph_traits<Graph>::degree_size_type d = out_degree(v, g);



		BGL_FORALL_ADJ_T(v,w,g,Graph)
		{
            // if we are adjacent to ourselves, skip the iteration
            if (v == w) { --d; continue; }

            in_indicator_pred<
    			typename graph_traits<Graph>::vertex_descriptor,
			    IndMap> p(ind,w);

			typename graph_traits<Graph>::adjacency_iterator ai, aiend;
			boost::tie(ai, aiend) = adjacent_vertices(w,g);

			// count if this is in the indicator predicate
			cur_cc += (int)count_if(ai, aiend, p);
		}

        if (d > 1)
        {
		    cc[v] = (double)cur_cc/(double)((d*(d-1)));
        }
        else
        {
            cc[v] = 0.0;
        }

		// reset the indicator
		BGL_FORALL_ADJ_T(v,w,g,Graph)
		{
			ind[w] = 0;
		}
	}
}

template <class Graph, class CCMap, class EdgeWeightMap, class IndMap>
void undirected_clustering_coefficients(const Graph& g, CCMap cc, EdgeWeightMap wm,
    IndMap ind)
{
    using namespace boost;
    typedef typename graph_traits<Graph>::vertex_descriptor vertex;
    typename graph_traits<Graph>::vertex_iterator vi,vi_end;
    for (tie(vi,vi_end)=vertices(g);vi!=vi_end;++vi) {
        put(cc,*vi,0);
        put(ind,*vi,0);
    }
    // a lazy cache for the temp weights for each vertex, could be more
    // efficient using the maximum degree
    std::vector<typename property_traits<EdgeWeightMap>::value_type> cache(num_vertices(g));
    std::vector<typename graph_traits<Graph>::degree_size_type> degs;
    for (tie(vi,vi_end)=vertices(g);vi!=vi_end;++vi) {
        vertex v = *vi;
        typename graph_traits<Graph>::out_edge_iterator oi,oi_end;
        typename graph_traits<Graph>::out_edge_iterator oi2,oi2_end;
        for (tie(oi,oi_end)=out_edges(v,g);oi!=oi_end;++oi) {
            // check to ignore self edges
            if (target(*oi,g) != v) {
                put(ind,target(*oi,g),1);
                cache[target(*oi,g)] = pow(get(wm,*oi),1.0/3.0);
            }
        }
        typename property_traits<CCMap>::value_type cur_cc = 0;
        typename graph_traits<Graph>::degree_size_type d = out_degree(v,g);
        for (tie(oi,oi_end)=out_edges(v,g);oi!=oi_end;++oi) {
            vertex w = target(*oi,g);
            if (v==w) { --d; }
            for (tie(oi2,oi2_end)=out_edges(w,g);oi2!=oi2_end;++oi2) {
                if (target(*oi2,g) == w) { continue; }
                if (get(ind,target(*oi2,g))) {
                    // cache is cached with its power
                    cur_cc += pow(get(wm,*oi2),1.0/3.0)*pow(get(wm,*oi),1.0/3.0)*cache[target(*oi2,g)];
                }
            }
        }
        if (d > 1) {
            put(cc,v,cur_cc/((typename property_traits<CCMap>::value_type)(d*(d-1))));
        } else {
            put(cc,v,(typename property_traits<CCMap>::value_type)0);
        }
        for (tie(oi,oi_end)=out_edges(v,g);oi!=oi_end;++oi) {
            put(ind,target(*oi,g),0);
            cache[target(*oi,g)] = 0;
        }
    }
}

// graph must be a bidirectional graph
template <class Graph, class CCMap, class EdgeWeightMap, class IndMap>
void directed_clustering_coefficients(const Graph& g,
    CCMap cc, EdgeWeightMap wm, IndMap ind)
{
    using namespace boost;
    typedef typename property_traits<CCMap>::value_type value_type;
    typedef typename graph_traits<Graph>::vertex_descriptor vertex;
    typename graph_traits<Graph>::vertex_iterator vi,vi_end;
    for (tie(vi,vi_end)=vertices(g);vi!=vi_end;++vi) {
        put(cc,*vi,0);
        put(ind,*vi,0);
    }
    // a lazy cache for the temp weights for each vertex, could be more
    // efficient using the maximum degree
    std::vector<typename property_traits<EdgeWeightMap>::value_type> cache(num_vertices(g));
    std::vector<typename graph_traits<Graph>::degree_size_type> degs(num_vertices(g));
    typename graph_traits<Graph>::edge_iterator ei,ei_end;
    for (tie(ei,ei_end)=edges(g);ei!=ei_end;++ei) {
        vertex u = source(*ei,g);
        vertex v = target(*ei,g);
        if (u==v) { continue; }
        degs[u]++;
        degs[v]++;
    }
    for (tie(vi,vi_end)=vertices(g);vi!=vi_end;++vi) {
        vertex v = *vi;
        typename graph_traits<Graph>::out_edge_iterator oi,oi_end;
        typename graph_traits<Graph>::out_edge_iterator oi2,oi2_end;
        typename graph_traits<Graph>::in_edge_iterator ii, ii_end;
        for (tie(ii,ii_end)=in_edges(v,g);ii!=ii_end;++ii) {
            if (source(*ii,g) != v) {
                put(ind,source(*ii,g),1);
                cache[source(*ii,g)] += pow(get(wm,*ii),1.0/3.0);
            }
        }
        // we've precomputed the set of in-edges, so we know how
        // to get back to v to complete a triangle that ends
        // with an edge to v.
        typename graph_traits<Graph>::degree_size_type bilateral_edges = 0;
        value_type cur_cc_cyc=0, cur_cc_mid=0, cur_cc_in=0, cur_cc_out=0;
        // cycles are out-out-out.
        for (tie(oi,oi_end)=out_edges(v,g);oi!=oi_end;++oi) {
            vertex w = target(*oi,g);
            if (v == w) { continue; }
            value_type cache_w = pow(get(wm,*oi),1.0/3.0);
            for (tie(oi2,oi2_end)=out_edges(w,g);oi2!=oi2_end;++oi2) {
                vertex u = target(*oi2,g);
                if (u == v) { ++bilateral_edges; continue; }
                if (u == w) { continue; }
                if (get(ind,u)) {
                    // cache is cached with its power
                    cur_cc_cyc += pow(get(wm,*oi2),1.0/3.0)*cache_w*cache[u];
                }
            }
        }
        for (tie(oi,oi_end)=out_edges(v,g);oi!=oi_end;++oi) {
            vertex w = target(*oi,g);
            if (v == w) { continue; }
            value_type cache_w = pow(get(wm,*oi),1.0/3.0);
            for (tie(ii,ii_end)=in_edges(w,g);ii!=ii_end;++ii) {
                vertex u = source(*ii,g);
                if (u == w) { continue; }
                if (get(ind,u)) {
                    // cache is cached with its power
                    cur_cc_mid += pow(get(wm,*ii),1.0/3.0)*cache_w*cache[u];
                }
            }
        }
        for (tie(ii,ii_end)=in_edges(v,g);ii!=ii_end;++ii) {
            vertex w = source(*ii,g);
            if (v == w) { continue; }
            value_type cache_w = pow(get(wm,*ii),1.0/3.0);
            for (tie(oi,oi_end)=out_edges(w,g);oi!=oi_end;++oi) {
                vertex u = target(*oi,g);
                if (u == w) { continue; }
                if (get(ind,u)) {
                    // cache is cached with its power
                    cur_cc_in += pow(get(wm,*oi),1.0/3.0)*cache_w*cache[u];
                }
            }
        }

        // reset the cache
        for (tie(ii,ii_end)=in_edges(v,g);ii!=ii_end;++ii) {
            if (source(*ii,g) != v) {
                put(ind,source(*ii,g),0);
                cache[source(*ii,g)] = 0;
            }
        }

        // re-init the cache with out-edges
        for (tie(oi,oi_end)=out_edges(v,g);oi!=oi_end;++oi) {
            // check to ignore self edges
            if (target(*oi,g) != v) {
                put(ind,target(*oi,g),1);
                cache[target(*oi,g)] += pow(get(wm,*oi),1.0/3.0);
            }
        }

        for (tie(oi,oi_end)=out_edges(v,g);oi!=oi_end;++oi) {
            vertex w = target(*oi,g);
            if (v == w) { continue; }
            value_type cache_w = pow(get(wm,*oi),1.0/3.0);
            for (tie(oi2,oi2_end)=out_edges(w,g);oi2!=oi2_end;++oi2) {
                vertex u = target(*oi2,g);
                if (u == w) { continue; }
                if (get(ind,u)) {
                    // cache is cached with its power
                    cur_cc_out += pow(get(wm,*oi2),1.0/3.0)*cache_w*cache[u];
                }
            }
        }

        // store the value
        typename graph_traits<Graph>::degree_size_type
            norm_factor = degs[v]*(degs[v] - 1) - 2*bilateral_edges;
        if (norm_factor > 0) {
            put(cc,v,
                (value_type)(cur_cc_cyc + cur_cc_mid + cur_cc_in + cur_cc_out)/
                    ((value_type)(norm_factor)));
        } else {
            put(cc,v,(value_type)0);
        }

        //std::cout << std::endl;
        //std::cout << v << " " <<  cur_cc_cyc << " " << cur_cc_mid << " " << cur_cc_in << " " << cur_cc_out << " " << degs[v] << " " << bilateral_edges << std::endl;

        for (tie(oi,oi_end)=out_edges(v,g);oi!=oi_end;++oi) {
            // check to ignore self edges
            if (target(*oi,g) != v) {
                put(ind,target(*oi,g),0);
                cache[target(*oi,g)] = 0;
            }
        }
    }
}

int clustering_coefficients(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, /* connectivity params */
    double* ccfs, int directed)
{
    using namespace yasmic;
    using namespace boost;

    typedef simple_csr_matrix<mbglIndex,double> crs_matrix;
    crs_matrix g(nverts, nverts, ia[nverts], ia, ja, NULL);

    std::vector<int> indicator_map(num_vertices(g));

    if (directed) {
        std::vector<mbglIndex> ati(nverts+1);
        std::vector<mbglIndex> atj(ia[nverts]);
        std::vector<mbglIndex> atid(ia[nverts]);

        build_row_and_column_from_csr(g, &ati[0], &atj[0], &atid[0]);

        typedef simple_row_and_column_matrix<mbglIndex,double> bidir_graph;
        bidir_graph bg(nverts, nverts, ia[nverts], ia, ja, NULL, &ati[0], &atj[0], &atid[0]);

        directed_clustering_coefficients(bg,
            make_iterator_property_map(ccfs, get(vertex_index,bg)),
            boost::detail::constant_value_property_map<double>(1.0),
            make_iterator_property_map(indicator_map.begin(),get(vertex_index,bg)));
    } else {
        undirected_clustering_coefficients(g,
            make_iterator_property_map(ccfs, get(vertex_index,g)),
            boost::detail::constant_value_property_map<double>(1.0),
            make_iterator_property_map(indicator_map.begin(), get(vertex_index,g)));
    }

    return (0);
}

int weighted_clustering_coefficients(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, double *weight, /* connectivity params */
    double* ccfs)
{
    using namespace yasmic;
    using namespace boost;

    typedef simple_csr_matrix<mbglIndex,double> crs_weighted_graph;
    crs_weighted_graph g(nverts, nverts, ia[nverts], ia, ja, weight);

    std::vector<int> indicator_map(num_vertices(g));

    undirected_clustering_coefficients(g,
        make_iterator_property_map(
            ccfs, get(vertex_index,g)),
        get(edge_weight,g),
        make_iterator_property_map(
            indicator_map.begin(), get(vertex_index,g)));

    return (0);
}

int directed_clustering_coefficients(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, double *weight, /* connectivity params */
    double* ccfs)
{
    using namespace yasmic;
    using namespace boost;

    typedef simple_csr_matrix<mbglIndex,double> crs_matrix;
    crs_matrix g(nverts, nverts, ia[nverts], ia, ja, weight);

    // early return for trivial input
    if (nverts==0) { return (0); }

    std::vector<mbglIndex> ati(nverts+1);
    std::vector<mbglIndex> atj(ia[nverts]);
    std::vector<mbglIndex> atid(ia[nverts]);

    build_row_and_column_from_csr(g, &ati[0], &atj[0], &atid[0]);

    typedef simple_row_and_column_matrix<mbglIndex,double> bidir_graph;
    bidir_graph bg(nverts, nverts, ia[nverts], ia, ja, weight, &ati[0], &atj[0], &atid[0]);

    std::vector<int> indicator_map(num_vertices(g));

    directed_clustering_coefficients(bg,
        make_iterator_property_map(
            ccfs, get(vertex_index,bg)),
        get(edge_weight,bg),
        make_iterator_property_map(
            indicator_map.begin(),
            get(vertex_index,bg)));

    return (0);
}



int betweenness_centrality(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, double *weight, /* connectivity params */
    double *centrality, double *ecentrality)
{
    using namespace yasmic;
    using namespace boost;

    typedef simple_csr_matrix<mbglIndex,double> crs_graph;
    crs_graph g(nverts, nverts, ia[nverts], ia, ja, weight);

    if (weight)
    {
        if (ecentrality) {
            brandes_betweenness_centrality(g,
	        		centrality_map(
                        make_iterator_property_map(centrality,
                            get(vertex_index, g)))
                    .weight_map(
                        make_iterator_property_map(weight,
                            get(edge_index,g)))
                    .edge_centrality_map(
                        make_iterator_property_map(
                            ecentrality, get(edge_index, g))));
        } else {
            brandes_betweenness_centrality(g,
	        		centrality_map(
                        make_iterator_property_map(centrality,
                            get(vertex_index, g)))
                    .weight_map(
                        make_iterator_property_map(weight,
                            get(edge_index,g))));
        }
    }
    else
    {
        if (ecentrality) {
            brandes_betweenness_centrality(g,
			    	centrality_map(
                        make_iterator_property_map(centrality,
                            get(vertex_index, g)))
                    .edge_centrality_map(
                        make_iterator_property_map(
                            ecentrality, get(edge_index, g))));
        } else {
            brandes_betweenness_centrality(g,
			    	make_iterator_property_map(centrality,
				    	get(vertex_index, g)));
        }
    }

    return (0);
}


/** Compute the core_numbers of a graph
 *
 * For an undirected graph, this function computes the core number of each
 * vertex.  The core number is the minimum number such that removing all
 * vertices of degree <= cn[k] removes vertex k.  For a directed graph
 * we compute the in-degree core number.
 *
 * @param nverts the number of vertices in the graph
 * @param ja the connectivity for each vertex
 * @param ia the row connectivity points into ja
 * @param cn an array of core numbers, length nverts
 * @param rt an array of removal times, length nverts
 * @return an error code if possible
 */
int core_numbers(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, /* connectivity params */
    mbglDegreeType *cn, int *rt)
{
    // History
    //
    // 30 July 2007
    // added removal time visitor
    // changed to mbglDegreeType
    using namespace yasmic;
    using namespace boost;

    if (!cn || !rt) { return (-1); }


    typedef simple_csr_matrix<mbglIndex,double> crs_graph;
    crs_graph g(nverts, nverts, ia[nverts], ia, ja, NULL);

    int time = 0;

    core_numbers(g, make_iterator_property_map(cn, get(vertex_index,g)),
        make_core_numbers_visitor(stamp_times(rt, time, on_examine_vertex())));

    return (0);
}

/** Compute the weighted core numbers of a graph
 *
 * For an undirected graph, this function computes the core number of each
 * vertex.  The core number is the minimum number such that removing all
 * vertices of weighted in-degree <= cn[k] removes vertex k.  For a
 * directed graph we compute the weighted in-degree core number.
 *
 * @param nverts the number of vertices in the graph
 * @param ja the connectivity for each vertex
 * @param ia the row connectivity points into ja
 * @param cn an array of core numbers, length nverts
 * @param rt an array of removal times, length nverts
 * @return an error code if possible
 */
int weighted_core_numbers(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, double *weight, /* connectivity params */
    double *cn, int *rt)
{
    // History
    //
    // 30 July 2007
    // added removal time visitor
    using namespace yasmic;
    using namespace boost;

    typedef simple_csr_matrix<mbglIndex,double> crs_graph;
    crs_graph g(nverts, nverts, ia[nverts], ia, ja, weight);

    int time=0;

    weighted_core_numbers(g, make_iterator_property_map(cn, get(vertex_index,g)),
        make_core_numbers_visitor(stamp_times(rt, time, on_examine_vertex())));

    return (0);
}

/** Find the bandwidth of the graph.
 * 
 * The bandwidth is the maximum difference in index on any edge.
 * See the boost documentation for more.
 * 
 * @param nverts the number of vertices in the graph
 * @param ja the connectivity for each vertex
 * @param ia the row connectivity points into ja
 * @param bandwidth the bandwidth of the graph
 * @return an error code if possible
 */
int bandwidth( 
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia,
    mbglIndex *bandwidth)
{    
  using namespace yasmic;
  using namespace boost;

  typedef simple_csr_matrix<mbglIndex, double> crs_graph;
  crs_graph g(nverts, nverts, ia[nverts], ia, ja, NULL);
  
  *bandwidth = boost::bandwidth(g);
  
  return 0;
}


/** Find the bandwidth of a vertex in the graph
 * 
 * The bandwidth of the vertex is the maximum difference in indices of
 * of any of that vertices edges.
 * 
 * @param nverts the number of vertices in the graph
 * @param ja the connectivity for each vertex
 * @param ia the row connectivity points into ja
 * @param v the vertex for the bandwidth computation
 * @param bandwidth the bandwidth of the graph
 * @return an error code if possible
 */
int vertex_bandwidth( 
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia,
    mbglIndex v, mbglIndex *bandwidth)
{    
  using namespace yasmic;
  using namespace boost;

  typedef simple_csr_matrix<mbglIndex, double> crs_graph;
  crs_graph g(nverts, nverts, ia[nverts], ia, ja, NULL);
  
  *bandwidth = boost::ith_bandwidth(v, g);
  
  return 0;
}
