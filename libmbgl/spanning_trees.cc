/** @file spanning_trees.cc
 * @author David F. Gleich
 * @date 2008-09-29
 * @copyright Stanford University, 2006-2008
 * Implement the BGL spanning tree wrappers.
 */

/** History
 *  2006-04-20: Initial version
 *  2006-11-10: Fixed bug with incorrect number of edges returned,
 *    when the input graph has multiple components.
 *    The nedges output parameter is now set correctly for all algorithms.
 *    Although, it depends on a somewhat dubious "hack" to detect
 *    unused portions of the output iterator.
 *  2007-07-09: Switched to simple_csr_matrix graph type
 *    Switched to kruskal mst from boost mod to fix bug with output iterator
 *  2007-11-16: Added root vertex option to prim's MST
 *  2008-10-01: Changed copy_to_ijval to use mbglIndex instead of int.
 *    Removed old commented regions.
 *  2011-12-20: Added random_spanning_tree
 */

#include "include/matlab_bgl.h"

#include <yasmic/simple_csr_matrix_as_graph.hpp>
#include <yasmic/iterator_utility.hpp>

#include <yasmic/boost_mod/kruskal_min_spanning_tree.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/random_spanning_tree.hpp>
#include <boost/random.hpp>

#include <vector>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/timeb.h>
unsigned int curtime()
{
#if defined(_WIN32) || defined(_WIN64)
  struct __timeb64 t; _ftime64(&t);
  return (t.time*1000 + t.millitm)%(1<<32-1);
#else
  struct timeval t; gettimeofday(&t, 0);
  return (t.tv_sec*1000 + t.tv_usec*1000)%(1<<32-1);
#endif
}


/*template <class Graph, class Edge>
class spanning_tree_insert_iterator
    : public boost::iterator_facade<
        spanning_tree_insert_iterator<Edge>
      , Edge
      , boost::forward_traversal_tag
    >
{
public:
    int* i; int* j; double* val;
    Graph& g;

    spanning_tree_insert_iterator(int* _i, int* _j, double* _val)
        : i(_i), j(_j), val(_val)
    {}

private:
    friend class boost::iterator_core_access;

    void increment()
    {
        i++;
        j++;
        val++;
    }

    bool equal(spanning_tree_insert_iterator const& other)
    {
        return (i == other.i && j == other.j && val == other.val);
    }

    Edge


};*/

template <class Graph, class EdgeWeightPropMap, class Iterator>
mbglIndex copy_to_ijval(Graph& g, EdgeWeightPropMap ewpm, Iterator oi,
                   Iterator oi_end, mbglIndex* i, mbglIndex* j, double* val)
{
    using namespace boost;

    mbglIndex ei;
    for (ei= 0; oi != oi_end; ++oi, ++ei) {
        typename graph_traits<Graph>::edge_descriptor e = *oi;
        i[ei] = source(e,g);
        j[ei] = target(e,g);
        val[ei] = ewpm[e];
    }

    return (ei);
}

int kruskal_mst(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, double *weight, /* connectivity params */
    mbglIndex* i, mbglIndex* j, double* val /* tree output */,
    mbglIndex* nedges)
{
    using namespace yasmic;
    using namespace boost;

    // create the graph g
    typedef simple_csr_matrix<mbglIndex,double> crs_weighted_graph;
    crs_weighted_graph g(nverts, nverts, ia[nverts], ia, ja, weight);

    //
    // warning, this code assumes that the default constructor for
    // an edges has source == target, otherwise we will detect
    // incorrect edges in the next step.
    //
    std::vector<graph_traits<crs_weighted_graph>::edge_descriptor>
        oi(nverts-1);

    std::vector<graph_traits<crs_weighted_graph>::edge_descriptor>::iterator
        oi_end = kruskal_minimum_spanning_tree(g,oi.begin());


    //*nedges = nverts-1;
    //*nedges = (int)(oi_end - oi.begin());

    // warning, this code assumes that the default constructor for
    // an edges has source == target, otherwise we will detect
    // incorrect edges in the next step.
    *nedges = copy_to_ijval(g,get(edge_weight,g),
        oi.begin(), oi_end, i, j, val);

    return (0);
}

int prim_mst_rooted(mbglIndex nverts, mbglIndex *ja, mbglIndex *ia,
    double *weight, /* connectivity params */
    mbglIndex* i, mbglIndex* j, double* val, mbglIndex *nedges, /* tree output */
    mbglIndex root /* tree root */)
{
  using namespace yasmic;
  using namespace boost;

  typedef simple_csr_matrix<mbglIndex, double> crs_weighted_graph;
  crs_weighted_graph g(nverts, nverts, ia[nverts], ia, ja, weight);

  std::vector<mbglIndex> pred(nverts);

  prim_minimum_spanning_tree(g, make_iterator_property_map(pred.begin(), get(
      vertex_index, g)), root_vertex(root));

  mbglIndex edge_num = 0;
  for (mbglIndex pi = 0; pi < nverts; pi++) {
    if (pred[pi] == pi) {
      // this edge isn't present
    } else {
      assert(edge_num<nverts-1);

      i[edge_num] = pi;
      j[edge_num] = pred[pi];
      val[edge_num] = 0.0;

      for (mbglIndex k = ia[pred[pi]]; k < ia[pred[pi] + 1]; k++) {
        if (ja[k] == pi) {
          val[edge_num] = weight[k];
          break;
        }
      }

      edge_num++;
    }
  }

  *nedges = edge_num;

  return (0);
}

/**
 * Compute a minimum spanning tree starting from vertex 0 using Prim's algorithm
 */
int prim_mst(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, double *weight, /* connectivity params */
    mbglIndex* i, mbglIndex* j, double* val, mbglIndex *nedges /* tree output */)
{
    // for our graph type, calling for a rooted tree with root 0 is identical
    // to calling for the default root.
    return prim_mst_rooted(nverts, ja, ia, weight, i, j, val, nedges, 0);
}


/** Compute a random minimum spanning tree of a graph
 * 
 * The graph can be directed or undirected
 * 
 * @param nverts the number of vertices in the graph
 * @param ja the connectivity for each vertex
 * @param ia the row connectivity points into ja
 * @param weight the weight of each edge in the graph
 * @param i the rows of the spanning tree
 * @param j the columns of the spanning tree
 * @param val the weights in the spanning tree
 * @param root the starting vertex for the tree
 * @param seed a value to seed the random number
 *  sequence, a value of zero indicates the seed should
 *  be based on the current time
 * @return an error code if possible.
 */

int random_spanning_tree(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, double *weight, /* connectivity params */
    mbglIndex* i, mbglIndex* j, double* val, mbglIndex* nedges, /* tree output */
    mbglIndex root, unsigned int seed)
{
  using namespace yasmic;
  using namespace boost;
  
  assert(root>=0 && root < nverts);
  
  boost::mt19937 gen;
  if (seed != 0) {
    gen.seed(seed);
  } else {
    gen.seed(curtime());
  }

  typedef simple_csr_matrix<mbglIndex, double> crs_weighted_graph;
  crs_weighted_graph g(nverts, nverts, ia[nverts], ia, ja, weight);

  std::vector<mbglIndex> pred(nverts);
  
  try {
    if (weight) {
      random_spanning_tree(g, gen,
        root_vertex(root)
        .predecessor_map(make_iterator_property_map(
            pred.begin(), get(vertex_index, g)))
        .weight_map(get(edge_weight, g))
      );
    } else {
      random_spanning_tree(g, gen,
        root_vertex(root)
        .predecessor_map(make_iterator_property_map(
            pred.begin(), get(vertex_index, g)))
      );
    }
  } catch (loop_erased_random_walk_stuck) {
    return 1;
  }

  mbglIndex true_null = graph_traits<crs_weighted_graph>::null_vertex();
  mbglIndex edge_num = 0;
  for (mbglIndex pi = 0; pi < nverts; pi++) {
    if (pred[pi] == true_null) {
      // this edge isn't present
    } else {
      assert(edge_num<nverts-1);

      i[edge_num] = pi;
      j[edge_num] = pred[pi];
      val[edge_num] = 1.0;

      if (weight) {
        for (mbglIndex k = ia[pred[pi]]; k < ia[pred[pi] + 1]; k++) {
          if (ja[k] == pi) {
            val[edge_num] = weight[k];
            break;
          }
        }
      }

      edge_num++;
    }
  }

  *nedges = edge_num;

  return 0;
}