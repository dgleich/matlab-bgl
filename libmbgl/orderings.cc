/**
 * @file orderings.cc
 *
 * Implement the BGL matrix orderings wrappers.
 */

/*
 * David Gleich
 */
/** History
 * 2006-04-21: Initial writing, but empty
 * 2011-12-05: Initial implementations
 */

#include "include/matlab_bgl.h"

#include <yasmic/simple_csr_matrix_as_graph.hpp>
#include <yasmic/iterator_utility.hpp>

#include <boost/graph/properties.hpp>
#include <boost/graph/graph_utility.hpp>


// fix crappy oversight in king_ordering.cc
namespace boost {
    
    template <typename Graph>
    bool has_no_vertices(const Graph& g) {
      return boost::graph::has_no_vertices(g);
    }
}

#include <boost/graph/king_ordering.hpp>
#include <boost/graph/sloan_ordering.hpp>



/**
 * undirected graphs only
 * 
 * If start >= nverts, then we will find the best starting vertex using
 * Boost's heuristic for this.
 */
int king_order(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, /* connectivity params */
    mbglIndex start, 
    mbglIndex *iperm /* permutation output */, mbglIndex *ipermlen)
{
  using namespace yasmic;
  using namespace boost;

  typedef simple_csr_matrix<mbglIndex, double> crs_graph;
  crs_graph g(nverts, nverts, ia[nverts], ia, ja, NULL);
  
  out_degree_property_map<crs_graph> dmap(g);
  
  std::vector<default_color_type> cmap(nverts);
  
  mbglIndex *end = NULL;
  if (start >= nverts) {
    end = king_ordering(g, iperm, 
        make_iterator_property_map(cmap.begin(), get(vertex_index,g)), dmap,
        get(vertex_index,g));
  } else {
    end = king_ordering(g, start, iperm, 
        make_iterator_property_map(cmap.begin(), get(vertex_index,g)), dmap,
        get(vertex_index,g));
  }
  
  *ipermlen = end - iperm;
  
  return 0;
}    

/**
 * undirected
 *
 * If start >= nverts, then we will find the best starting vertex using
 * Boost's heuristic for this.
 */
int sloan_order(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, /* connectivity params */
    mbglIndex vstart, mbglIndex vend,
    mbglIndex *iperm /* permutation output */, mbglIndex *ipermlen)
{
  using namespace yasmic;
  using namespace boost;

  typedef simple_csr_matrix<mbglIndex, double> crs_graph;
  crs_graph g(nverts, nverts, ia[nverts], ia, ja, NULL);
  
  out_degree_property_map<crs_graph> dmap(g);
  std::vector<double> priority(nverts);
  std::vector<default_color_type> cmap(nverts);
  
  mbglIndex *end = NULL;
  if (vstart >= nverts) {
    end = sloan_ordering(g, iperm, 
        make_iterator_property_map(cmap.begin(), get(vertex_index,g)), 
        dmap,
        make_iterator_property_map(priority.begin(), get(vertex_index,g)));
  } else {
    end = sloan_ordering(g, vstart, vend, iperm, 
        make_iterator_property_map(cmap.begin(), get(vertex_index,g)), 
        dmap,
        make_iterator_property_map(priority.begin(), get(vertex_index,g)));
  }
  
  *ipermlen = end - iperm;
  
  return 0;
}    
