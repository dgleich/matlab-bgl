/** @file structure.cc
 * @author David F. Gleich
 * @date 2011-12-03
 * @copyright Purdue University, 2011
 * Graph statistics wrappers
 */

/** History
 *  2011-12-03: Split off from statistics, and added bipartite/isomorphism
 *  2011-11-30: Corrected betweenness centrality with property maps
 */

#include "include/matlab_bgl.h"

#include <yasmic/simple_csr_matrix_as_graph.hpp>
#include <yasmic/simple_row_and_column_matrix_as_graph.hpp>
#include <yasmic/iterator_utility.hpp>

#include <vector>

#include <boost/graph/topological_sort.hpp>

#include <boost/iterator/reverse_iterator.hpp>

#include <boost/graph/max_cardinality_matching.hpp>
#include <boost/graph/dominator_tree.hpp>
#include <boost/graph/bipartite.hpp>
#include <boost/graph/isomorphism.hpp>
#include <boost/graph/sequential_vertex_coloring.hpp>

#include <iostream>

#include <math.h>


/**
 * Test for a topological order or topological sort of a graph.
 *
 * This function will also test if the graph is a dag.  If the
 * rev_order parameter is null, then the actual topological order
 * is ignored and the function just tests for a dag.
 *
 * @param nverts the number of vertices in the graph
 * @param ja the connectivity for each vertex
 * @param ia the row connectivity points into ja
 * @param topo_order a topological order of the vertices
 * @param is_dag tests if the graph is a dag (optional)
 */
int topological_order(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, /* connectivity params */
    mbglIndex *topo_order, int *is_dag)
{
    using namespace yasmic;
    using namespace boost;

    typedef simple_csr_matrix<mbglIndex,double> crs_graph;
    crs_graph g(nverts, nverts, ia[nverts], ia, ja, NULL);

    if (is_dag) {
        *is_dag = 1;
    }

    if (topo_order == NULL)
    {
        return (-1);
    }
    else
    {
        typedef reverse_iterator<mbglIndex*>
            reverse_iter;

        // note for the future, boost's reverse iterator is wild in that
        // it actually dereferences the PRIOR element, so the correct
        // range of a boost reverse itertor is the same as the original
        // iterator.  Consequently, the last element is actual topo_order +
        // nverts because the previous element (what * returns) is the
        // correct element.
        reverse_iter output(topo_order + (nverts));

        try {
            topological_sort(g, output);
        } catch (not_a_dag) {
            if (is_dag) {
                *is_dag = 0;
            }
        }
    }

    return (0);
}

template <typename Graph,
        typename MateMap,
        typename VertexIndexMap>
bool matching_help(const Graph& g, MateMap mate, VertexIndexMap vm,
                   int initial_matching, int augmenting_path, int verify)
{
    using namespace boost;
    if (initial_matching == 1)
    {
        if (augmenting_path == 1)
            if (verify == 1)
                return matching<Graph,MateMap,VertexIndexMap,
                        no_augmenting_path_finder,
                        empty_matching,
                        no_matching_verifier>
                        (g, mate, vm);
            else
                return matching<Graph,MateMap,VertexIndexMap,
                        no_augmenting_path_finder,
                        empty_matching,
                        maximum_cardinality_matching_verifier>
                        (g, mate, vm);
        else
            if (verify == 1)
                return matching<Graph,MateMap,VertexIndexMap,
                        edmonds_augmenting_path_finder,
                        empty_matching,
                        no_matching_verifier>
                        (g, mate, vm);
            else
                return matching<Graph,MateMap,VertexIndexMap,
                        edmonds_augmenting_path_finder,
                        empty_matching,
                        maximum_cardinality_matching_verifier>
                        (g, mate, vm);
    }
    else if (initial_matching == 2)
    {
        if (augmenting_path == 1)
            if (verify == 1)
                return matching<Graph,MateMap,VertexIndexMap,
                        no_augmenting_path_finder,
                        greedy_matching,
                        no_matching_verifier>
                        (g, mate, vm);
            else
                return matching<Graph,MateMap,VertexIndexMap,
                        no_augmenting_path_finder,
                        greedy_matching,
                        maximum_cardinality_matching_verifier>
                        (g, mate, vm);
        else
            if (verify == 1)
                return matching<Graph,MateMap,VertexIndexMap,
                        edmonds_augmenting_path_finder,
                        greedy_matching,
                        no_matching_verifier>
                        (g, mate, vm);
            else
                return matching<Graph,MateMap,VertexIndexMap,
                        edmonds_augmenting_path_finder,
                        greedy_matching,
                        maximum_cardinality_matching_verifier>
                        (g, mate, vm);
    }
    else
    {
        if (augmenting_path == 1)
            if (verify == 1)
                return matching<Graph,MateMap,VertexIndexMap,
                        no_augmenting_path_finder,
                        extra_greedy_matching,
                        no_matching_verifier>
                        (g, mate, vm);
            else
                return matching<Graph,MateMap,VertexIndexMap,
                        no_augmenting_path_finder,
                        extra_greedy_matching,
                        maximum_cardinality_matching_verifier>
                        (g, mate, vm);
        else
            if (verify == 1)
                return matching<Graph,MateMap,VertexIndexMap,
                        edmonds_augmenting_path_finder,
                        extra_greedy_matching,
                        no_matching_verifier>
                        (g, mate, vm);
            else
                return matching<Graph,MateMap,VertexIndexMap,
                        edmonds_augmenting_path_finder,
                        extra_greedy_matching,
                        maximum_cardinality_matching_verifier>
                        (g, mate, vm);
    }
}

/** Wrap a boost graph library call to matching.
 *
 * A matching is a subset of edges with no common vertices in an undirected
 * graph.
 *
 * A maximum cardinality matching has the largest number of edges over all
 * possible matchings.
 *
 * The graph must be undirected.
 *
 * @param nverts the number of vertices in the graph
 * @param ja the connectivity for each vertex
 * @param ia the row connectivity points into ja
 * @param mate an array of size nverts which stores the matching vertex index
 *  or null vertex if there is no match
 * @param initial_matching indicates the initial matching used in the algorithm
 *  1: no_matching
 *  2: greedy_matching
 *  3: extra_greedy_matching
 * @param augmenting_path indicates the algorithm used to find augmenting paths
 *  1: no_augmenting_path_finder
 *  2: edmonds_augmenting_path_finder
 * @param verify indicates if we verify the algorithm ouput
 *  1: no_matching_verifier
 *  2: maximum_cardinality_matching_verifier
 * @param verified the output of the verification algorithm (if run), or
 *  false otherwise
 * @param null_vertex the special index to indicate an unmatched vertex
 * @return an error code if possible
 *   0: indicates success
 *  -1: indicates a parameter error with initial_matching, augmenting_path, or verify
 */
int maximum_cardinality_matching(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, /* connectivity params */
    mbglIndex* mate, int initial_matching, int augmenting_path, int verify,
    int *verified, mbglIndex *null_vertex)
{
    using namespace yasmic;
    using namespace boost;

    typedef simple_csr_matrix<mbglIndex,double> crs_graph;
    crs_graph g(nverts, nverts, ia[nverts], ia, ja, NULL);

    if (initial_matching < 1 || initial_matching > 3 ||
        augmenting_path < 1 || augmenting_path > 2 ||
        verify < 1 || verify > 2)
    {
        return (-1);
    }

    bool max_matching =
        matching_help(g,mate,get(vertex_index,g),
            initial_matching,augmenting_path,verify);

    if (verified) {
        *verified = 0;
        if (verify > 1) { *verified = max_matching; }
    }

    if (null_vertex) {
        *null_vertex = graph_traits<crs_graph>::null_vertex();
    }

    return (0);
}

template <typename Graph, typename MateMap, typename VertexIndexMap>
bool verify_matching_help(const Graph& g, MateMap mate, VertexIndexMap vm)
{
    return boost::maximum_cardinality_matching_verifier<
                    Graph,MateMap,VertexIndexMap>::verify_matching(g,mate,vm);
}

/*
 * mate = nverts if unmatched
 */
int test_maximum_cardinality_matching(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, /* connectivity params */
    mbglIndex* mate, int *verified)
{
    using namespace yasmic;
    using namespace boost;

    typedef simple_csr_matrix<mbglIndex,double> crs_graph;
    crs_graph g(nverts, nverts, ia[nverts], ia, ja, NULL);

    // set the null_vertex to the true null
    mbglIndex true_null = graph_traits<crs_graph>::null_vertex();
    for (mbglIndex v=0;v<num_vertices(g);++v) {
        if (mate[v] == nverts) { mate[v] = true_null; }
    }

    bool max_matching = verify_matching_help(g, mate, get(vertex_index,g));
    if (verified) { *verified = 0; }
    if (max_matching && verified) { *verified = 1; }

    return (0);
}

int dominator_tree(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia,
    mbglIndex src, mbglIndex *pred)
{
    //
    // History
    //
    // 12 July 2007
    // Initial implementation
    //
    // 23 July 2007
    // Added iterator property map call for pred.
    //
    using namespace yasmic;
    using namespace boost;

    typedef simple_csr_matrix<mbglIndex,double> crs_graph;
    crs_graph g(nverts, nverts, ia[nverts], ia, ja, NULL);

    std::vector<mbglIndex> ati(nverts+1);
    std::vector<mbglIndex> atj(ia[nverts]);
    std::vector<mbglIndex> atid(ia[nverts]);

    build_row_and_column_from_csr(g, &ati[0], &atj[0], &atid[0]);

    typedef simple_row_and_column_matrix<mbglIndex,double> bidir_graph;
    bidir_graph bg(nverts, nverts, ia[nverts], ia, ja, NULL, &ati[0], &atj[0], &atid[0]);

    // modify the output to conform to what matlab_bgl expects
    mbglIndex null_vertex = graph_traits<bidir_graph>::null_vertex();
    for (mbglIndex i=0; i< nverts; i++) { pred[i] = null_vertex; }

    lengauer_tarjan_dominator_tree(bg, src,
        make_iterator_property_map(pred, get(vertex_index,bg)));

    pred[src] = src;

    // modify the output to conform to what matlab_bgl expects
    for (mbglIndex i=0; i< nverts; i++) {
        if (pred[i] == null_vertex) { pred[i] = i; }
    }

    return (0);
}

// provide the colormodel for mbglIndex
namespace boost {
  template <>
  struct color_traits<mbglIndex> {
    static mbglIndex white() { return 0; }
    static mbglIndex black() { return 1; }
    static mbglIndex gray() { return std::numeric_limits<mbglIndex>::max(); }
  };
} // namespace boost

/** Check if a graph is bipartite
 * 
 * @param nverts the number of vertices in the graph
 * @param ja the connectivity for each vertex
 * @param ia the row connectivity points into ja
 * @param is_bipartite set to 1 if the network is bipartite (size 1)
 *   and set to 0 otherwise
 * @param part, an indicator vector over the vertices in one half 
 *   of the partition (length n)
 * @return an error code if possible
 */
int is_bipartite(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia,
    int* is_bipartite, mbglIndex* part)
{    
  using namespace yasmic;
  using namespace boost;

  typedef simple_csr_matrix<mbglIndex, double> crs_graph;
  crs_graph g(nverts, nverts, ia[nverts], ia, ja, NULL);
  
  // TODO, see if we can skip the extra malloc here

  bool ans=boost::is_bipartite(g,  get(vertex_index, g),
    make_iterator_property_map(part, get(vertex_index, g)));

  if (ans) { 
    *is_bipartite = 1; 
  } else {
    *is_bipartite = 0;
  }
  return 0;
}

/** Find an odd-cycle in a non-bipartite graph
 * 
 * @param nverts the number of vertices in the graph
 * @param ja the connectivity for each vertex
 * @param ia the row connectivity points into ja
 * @param cycle, a list of vertices in the cycle, excluding
 *   the final vertex, so the cyclenlen-1 elements are valid.
 * @param cyclelen the length of the cycle in cycle.
 * If the network is bipartite, cyclelen is 0
 * 
 * @return an error code if possible
 */
int find_odd_cycle(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia,
    mbglIndex *cycle, mbglIndex *cyclelen)
{    
  using namespace yasmic;
  using namespace boost;

  typedef simple_csr_matrix<mbglIndex, double> crs_graph;
  crs_graph g(nverts, nverts, ia[nverts], ia, ja, NULL);
  
  mbglIndex* final = boost::find_odd_cycle(g,  cycle);
  
  *cyclelen = final - cycle;

  return 0;
}

/** Find a graph isomorphism
 * 
 * @param nverts1 the number of vertices in the graph for graph A
 * @param ja1 the connectivity for each vertex for graph A
 * @param ia1 the row connectivity points into ja for graph A
 * @param nverts2 the number of vertices in the graph for graph B
 * @param ja2 the connectivity for each vertex for graph B
 * @param ia2 the row connectivity points into ja for graph B
 * @param map the mapping from vertices in graph A to graph B, given
 *   A -> B; so the required length is nverts
 * @param atypes the types for each vertex in graph A (length nverts1)
 *   but can be null!
 * @param btypes the types for each vertex in graph B (length nverts2)
 *   but can be null!
 * @param maxtype the largest type in either graph
 *   ignored if atypes or btypes are null
 * 
 * 
 * @return an error code if possible, 0 otherwise
 */
 
/* int isomorphism(
    mbglIndex nverts1, mbglIndex *ja1, mbglIndex *ia1,
    mbglIndex nverts2, mbglIndex *ja2, mbglIndex *ia2,
    int *iso, mbglIndex *map, 
    mbglIndex *atypes, mbglIndex *btypes, mbglIndex maxtype) */
    

/** Find a graph isomorphism
 * 
 * @param nverts1 the number of vertices in the graph for graph A
 * @param ja1 the connectivity for each vertex for graph A
 * @param ia1 the row connectivity points into ja for graph A
 * @param nverts2 the number of vertices in the graph for graph B
 * @param ja2 the connectivity for each vertex for graph B
 * @param ia2 the row connectivity points into ja for graph B
 * @param map the mapping from vertices in graph A to graph B, given
 *   A -> B; so the required length is nverts
 * 
 * 
 * @return an error code if possible, 0 otherwise
 */
 
int isomorphism(
    mbglIndex nverts1, mbglIndex *ja1, mbglIndex *ia1,
    mbglIndex nverts2, mbglIndex *ja2, mbglIndex *ia2,
    int *iso, mbglIndex *map)    
{
  using namespace yasmic;
  using namespace boost;
  
  if (nverts1 != nverts2) { *iso = 0; return 0; }

  typedef simple_csr_matrix<mbglIndex, double> crs_graph;
  crs_graph g(nverts1, nverts1, ia1[nverts1], ia1, ja1, NULL);
  crs_graph h(nverts2, nverts2, ia2[nverts2], ia2, ja2, NULL);
  
  std::vector<mbglIndex> ati(nverts2+1);
  std::vector<mbglIndex> atj(ia2[nverts2]);
  std::vector<mbglIndex> atid(ia2[nverts2]);

  build_row_and_column_from_csr(h, &ati[0], &atj[0], &atid[0]);
  
  typedef simple_row_and_column_matrix<mbglIndex,double> bidir_graph;
  bidir_graph bh(nverts2, nverts2, ia2[nverts2], ia2, ja2, NULL, 
    &ati[0], &atj[0], &atid[0]);

  
  for (mbglIndex i=0; i<nverts1; ++i) {
    map[i] = graph_traits<bidir_graph>::null_vertex();
  }
  bool isiso = boost::isomorphism(g, bh,
        isomorphism_map(make_iterator_property_map(
            map, get(vertex_index, g))));

  if (isiso) { 
      *iso = 1; 
  } else { 
      *iso = 0; 
  }
  
  return 0;
}    
    
int sequential_vertex_coloring(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia,
    mbglIndex *colors)
{    
  using namespace yasmic;
  using namespace boost;

  typedef simple_csr_matrix<mbglIndex, double> crs_graph;
  crs_graph g(nverts, nverts, ia[nverts], ia, ja, NULL);
  
  sequential_vertex_coloring(g, 
    make_iterator_property_map(colors, get(vertex_index, g)));

  return 0;
}
