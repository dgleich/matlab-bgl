
/*
 * David Gleich
 * Copyright, Stanford University, 2006
 */

/**
 * @file max_flow.cc
 * Implement the BGL max_flow wrappers.
 */

/*
 * 16 April 2006
 * Initial version
 *
 * 8 July 2007
 * Added kolmogorov from boost_mod
 * Added edmunds_karp from boost
 *
 * 9 July 2007
 * Switched to simple_csr_matrix graph type
 */

#include "include/matlab_bgl.h"

#include <yasmic/undir_simple_csr_matrix_as_graph.hpp>
#include <yasmic/simple_csr_matrix_as_graph.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
#include <boost/graph/boykov_kolmogorov_max_flow.hpp>
#include <yasmic/iterator_utility.hpp>
#include <boost/property_map/property_map.hpp>

#include <boost/graph/one_bit_color_map.hpp>
#include <boost/graph/stoer_wagner_min_cut.hpp>

template <typename Index, typename Value, typename EdgeIndex, class Child>
struct reverse_edge_pmap_helper
{
    typedef yasmic::simple_csr_matrix<Index,Value,EdgeIndex> matrix;
    typedef boost::graph_traits<matrix> traits;

    typedef typename traits::edge_descriptor key_type;
    typedef typename traits::edge_descriptor value_type;
    typedef boost::lvalue_property_map_tag category;
    typedef typename traits::edge_descriptor reference;

    typedef boost::put_get_helper<reference, Child> super_class;
};

/*template <class RowIter, class ColIter, class ValIter, class Child>
struct reverse_edge_pmap_helper
{
    typedef yasmic::compressed_row_matrix<RowIter, ColIter, ValIter> matrix;
    typedef boost::graph_traits<matrix> traits;

    typedef typename traits::edge_descriptor key_type;
    typedef typename traits::edge_descriptor value_type;
    typedef boost::lvalue_property_map_tag category;
    typedef typename traits::edge_descriptor reference;

    typedef boost::put_get_helper<reference, Child> super_class;
};*/

template <typename Index, typename Value, typename EdgeIndex>
class reverse_edge_pmap
    : public reverse_edge_pmap_helper<Index,Value,EdgeIndex,
        reverse_edge_pmap<Index,Value,EdgeIndex> >::super_class
{
    typedef yasmic::simple_csr_matrix<Index,Value,EdgeIndex> matrix;
    typedef boost::graph_traits<matrix> traits;

    mbglIndex* _rev_edge_index;
    const matrix& _g;
    typename traits::edge_descriptor _e;

public:
    typedef typename traits::edge_descriptor key_type;
    typedef typename traits::edge_descriptor value_type;
    typedef boost::lvalue_property_map_tag category;
    typedef typename traits::edge_descriptor& reference;

public:
    inline reference
    operator[](key_type v)
    {
        //return yasmic::make_simple_nonzero(v._row, v._column, v._val, _rev_edge_index[v._nzi]);
        //return yasmic::make_simple_nonzero(v._column, v._row, v._val, _rev_edge_index[v._nzi]);
        _e.r = boost::target(v,_g);
        _e.i = _rev_edge_index[v.i];
        return _e;
    }
	  inline typename traits::edge_descriptor const &
    operator[](key_type v) const
    {
			  (const_cast<reference>(_e)).r = boost::target(v,_g);
			  (const_cast<reference>(_e)).i = _rev_edge_index[v.i];
        return _e;
    }	

    inline
    reverse_edge_pmap(mbglIndex* rev_edge_index, const matrix& g)
    : _rev_edge_index(rev_edge_index), _g(g)
    {}
};

template <typename Index, typename Value, typename EdgeIndex>
inline reverse_edge_pmap<Index, Value, EdgeIndex>
make_reverse_edge_pmap(
    const yasmic::simple_csr_matrix<Index, Value, EdgeIndex>& g,
    mbglIndex* rev_edge_index)
{
    return reverse_edge_pmap<Index, Value, EdgeIndex>(rev_edge_index, g);
}

/**
 * Wrap the boost graph library class for a push relabel max flow computation.
 *
 * the ja and ia arrays specify the connectivity of the underlying graph,
 * ia is a length (nverts+1) array with the indices in ja that start the
 * nonzeros in each row.  ja is a length (ia(nverts)) array with the
 * columns of the connectivity.
 *
 * the connectivity graph must be symmetric, that is, for each edge (i,j)
 * the edge (j,i) must exist in the connectivity graph.
 *
 * @param nverts the number of vertices in the graph
 * @param ja the connectivity for each vertex
 * @param ia the row connectivity points into ja
 * @param src the source vertex for the flow
 * @param sink the sink vertex for the flow
 * @param cap the array of capacities for each edge
 * @param res the array of residual capacities for each edge
 * @param rev_edge_index an array indicating the index of the reverse edge
 * for the edge with the current index
 * @param flow the maximum flow in the graph
 * @return an error code if possible
 */
int push_relabel_max_flow(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia,
    mbglIndex src, mbglIndex sink,
    int* cap, int* res,
    mbglIndex* rev_edge_index,
    int* flow)
{
    using namespace yasmic;
    using namespace boost;

    typedef simple_csr_matrix<mbglIndex,double> crs_graph;
    crs_graph g(nverts, nverts, ia[nverts], ia, ja, NULL);

    *flow = (push_relabel_max_flow(g,
        src, sink,
        make_iterator_property_map(
            cap, get(edge_index,g)),
        make_iterator_property_map(
            res, get(edge_index,g)),
        make_reverse_edge_pmap(g,rev_edge_index),
        get(vertex_index,g)));

    return (0);
}

/**
 * Wrap the boost graph library class for a Edmunds-Karp max flow computation.
 *
 * the ja and ia arrays specify the connectivity of the underlying graph,
 * ia is a length (nverts+1) array with the indices in ja that start the
 * nonzeros in each row.  ja is a length (ia(nverts)) array with the
 * columns of the connectivity.
 *
 * the connectivity graph must be symmetric, that is, for each edge (i,j)
 * the edge (j,i) must exist in the connectivity graph.
 *
 * @param nverts the number of vertices in the graph
 * @param ja the connectivity for each vertex
 * @param ia the row connectivity points into ja
 * @param src the source vertex for the flow
 * @param sink the sink vertex for the flow
 * @param cap the array of capacities for each edge
 * @param res the array of residual capacities for each edge
 * @param rev_edge_index an array indicating the index of the reverse edge
 * for the edge with the current index
 * @param flow the maximum flow in the graph
 * @return an error code if possible
 */
int edmonds_karp_max_flow(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia,
    mbglIndex src, mbglIndex sink,
    int* cap, int* res,
    mbglIndex* rev_edge_index,
    int* flow)
{
    using namespace yasmic;
    using namespace boost;

    typedef simple_csr_matrix<mbglIndex,double> crs_graph;
    crs_graph g(nverts, nverts, ia[nverts], ia, ja, NULL);

    *flow = (edmonds_karp_max_flow(g,
        src, sink,
        capacity_map(make_iterator_property_map(cap, get(edge_index,g))).
        residual_capacity_map(make_iterator_property_map(res, get(edge_index,g))).
        reverse_edge_map(make_reverse_edge_pmap(g,rev_edge_index))));

    return (0);
}

/**
 * Wrap the boost graph library class for a Kolmogorov max flow computation.
 *
 * the ja and ia arrays specify the connectivity of the underlying graph,
 * ia is a length (nverts+1) array with the indices in ja that start the
 * nonzeros in each row.  ja is a length (ia(nverts)) array with the
 * columns of the connectivity.
 *
 * the connectivity graph must be symmetric, that is, for each edge (i,j)
 * the edge (j,i) must exist in the connectivity graph.
 *
 * @param nverts the number of vertices in the graph
 * @param ja the connectivity for each vertex
 * @param ia the row connectivity points into ja
 * @param src the source vertex for the flow
 * @param sink the sink vertex for the flow
 * @param cap the array of capacities for each edge
 * @param res the array of residual capacities for each edge
 * @param rev_edge_index an array indicating the index of the reverse edge
 * for the edge with the current index
 * @param flow the maximum flow in the graph
 * @return an error code if possible
 */
int boykov_kolmogorov_max_flow(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia,
    mbglIndex src, mbglIndex sink,
    int* cap, int* res,
    mbglIndex* rev_edge_index,
    int* flow)
{
    using namespace yasmic;
    using namespace boost;

    typedef simple_csr_matrix<mbglIndex,double> crs_graph;
    crs_graph g(nverts, nverts, ia[nverts], ia, ja, NULL);

    *flow = (boykov_kolmogorov_max_flow(g,
        make_iterator_property_map(cap, get(edge_index,g)),
        make_iterator_property_map(res, get(edge_index,g)),
        make_reverse_edge_pmap(g,rev_edge_index),
        get(vertex_index,g),
        src, sink));

    return (0);
}


/** Compute a minimum cut via the Stoer Wagner algorithm
 * 
 * The graph must be undirected. 
 *
 * @param nverts the number of vertices in the graph
 * @param ja the connectivity for each vertex
 * @param ia the row connectivity points into ja
 * @param weight the weight of each edge in the graph
 * @param cut the value of the mincut
 * @param cutset an indicator vector over the vertices indicating
 *   0 -> one side; 1 -> the second side
 * @return an error code if possible.
 */
int stoer_wagner_min_cut(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, double *weight, /* connectivity params */
    double* cut, mbglIndex *cutset)
{
  using namespace yasmic;
  using namespace boost;
  
  if (nverts < 2) {
    return -1; 
  }
  
  typedef undir_simple_csr_matrix<mbglIndex,double> crs_graph;
  crs_graph g(nverts, nverts, ia[nverts], ia, ja, weight);

  BOOST_AUTO(parities, boost::make_one_bit_color_map(num_vertices(g), get(boost::vertex_index, g)));
  
  double cutval = 
     stoer_wagner_min_cut(g, get(boost::edge_weight, g), 
       boost::parity_map(parities));
  
  if (cutset != NULL) {
    for (mbglIndex i = 0; i<nverts; ++i) {
      if (get(parities,i)) {
        cutset[i] = 1;
      } else {
        cutset[i] = 0;
      }
    }
  }
  
  if (cut != NULL) {
    *cut = cutval;
  }
  return 0;
}
