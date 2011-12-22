/** @file random_graphs.cc
 * @author David F. Gleich
 * @date 2011-12-22
 * @copyright Purdue University, 2011
 */

/** History
 * :2011-12-22: initial writing
 */

#include "include/matlab_bgl.h"

#include <yasmic/simple_csr_matrix_as_graph.hpp>

#include <boost/graph/erdos_renyi_generator.hpp>
#include <boost/graph/small_world_generator.hpp>
#include <boost/graph/plod_generator.hpp>
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

/** Generate edges from an Erdos-Renyi distribution
 * 
 * This function always generates a directed set of edges.  To
 * get an undirected set, just take any edge where src < dst.
 * 
 * @param nverts the number vertices in the new graph
 * @param prob the probability of selecting an edge
 * @param loops an indicator for whether loops are valid edges
 * @param src the source of the resulting edges
 * @param dst the destination of the resulting edges
 * @param nallocedges (input/output) 
 *   on input, the number of preallocated edges
 *   on output, the number of used edges
 * @param seed the random seed, if the seed is zero, it'll
 *   be initialized from the current time
 * @return 0 on success, an error if possible.
 *   if the function returns 1, then there were not enough
 *   edges preallocated, and the value in nallocedges is the
 *   total number of edges in the graph for the current seed
 *   (which is only useful if the function used a prescribed seed)
 */
int erdos_renyi_edges(
    mbglIndex nverts, double prob, int loops,
    mbglIndex *src, mbglIndex *dst, mbglIndex *nallocedges, 
    unsigned int seed)
{
  using namespace yasmic;
  using namespace boost;
  
  assert(src && dst);
  assert(nallocedges);
  
  boost::mt19937 gen;
  if (seed != 0) {
    gen.seed(seed);
  } else {
    gen.seed(curtime());
  }

  typedef simple_csr_matrix<mbglIndex, double> crs_weighted_graph;
  typedef sorted_erdos_renyi_iterator<boost::mt19937, crs_weighted_graph> 
    edge_generator;
  edge_generator cur(gen, nverts, prob, loops!=0);
  edge_generator end;
  
  mbglIndex nvalidedges = *nallocedges;
  mbglIndex ntotaledges = 0;
  
  for (; cur != end; ++cur) {
    if (ntotaledges < nvalidedges) {
      src[ntotaledges] = (mbglIndex) cur->first;
      dst[ntotaledges] = (mbglIndex) cur->second;
    }
    ++ntotaledges;
  }
  
  *nallocedges = ntotaledges;
  
  if (ntotaledges <= nvalidedges) {
    return 0;
  } else {
    return 1;
  }
}


/** Generate edges from a Watts-Strogatz small world graph
 * 
 * This function always generates an undirected set of edges.
 * 
 * @param nverts the number vertices in the new graph
 * @param k the number of nearest neighbors to connect
 * @param prob the rewiring probability for each edge
 * @param loops an indicator for whether loops are valid edges
 * @param src the source of the resulting edges
 * @param dst the destination of the resulting edges
 * @param nallocedges (input/output) 
 *   on input, the number of preallocated edges
 *   on output, the number of used edges
 * @param seed the random seed, if the seed is zero, it'll
 *   be initialized from the current time
 * @return 0 on success, an error if possible.
 *   if the function returns 1, then there were not enough
 *   edges preallocated, and the value in nallocedges is the
 *   total number of edges in the graph for the current seed
 *   (which is only useful if the function used a prescribed seed)
 */
int small_world_edges(
    mbglIndex nverts, mbglIndex k, double prob, int loops,
    mbglIndex *src, mbglIndex *dst, mbglIndex *nallocedges, 
    unsigned int seed)
{
  using namespace yasmic;
  using namespace boost;
  
  assert(src && dst);
  assert(nallocedges);
  
  boost::mt19937 gen;
  if (seed != 0) {
    gen.seed(seed);
  } else {
    gen.seed(curtime());
  }

  typedef simple_csr_matrix<mbglIndex, double> crs_weighted_graph;
  typedef small_world_iterator<boost::mt19937, crs_weighted_graph> 
    edge_generator;
  edge_generator cur(gen, nverts, k, prob, loops!=0);
  edge_generator end;
  
  mbglIndex nvalidedges = *nallocedges;
  mbglIndex ntotaledges = 0;
  
  for (; cur != end; ++cur) {
    if (ntotaledges < nvalidedges) {
      src[ntotaledges] = (mbglIndex) cur->first;
      dst[ntotaledges] = (mbglIndex) cur->second;
    }
    ++ntotaledges;
  }
  
  *nallocedges = ntotaledges;
  
  if (ntotaledges <= nvalidedges) {
    return 0;
  } else {
    return 1;
  }
}


/** Generate edges from a power-law out-degree graph
 * 
 * This function always generates an undirected set of edges.
 * 
 * @param nverts the number vertices in the new graph
 * @param alpha a parameter that controls the variance
 * @param beta a parameter which controls the average degree
 * @param loops an indicator for whether loops are valid edges
 * @param src the source of the resulting edges
 * @param dst the destination of the resulting edges
 * @param nallocedges (input/output) 
 *   on input, the number of preallocated edges
 *   on output, the number of used edges
 * @param seed the random seed, if the seed is zero, it'll
 *   be initialized from the current time
 * @return 0 on success, an error if possible.
 *   if the function returns 1, then there were not enough
 *   edges preallocated, and the value in nallocedges is the
 *   total number of edges in the graph for the current seed
 *   (which is only useful if the function used a prescribed seed)
 */
int plod_edges(
    mbglIndex nverts, double alpha, double beta, int loops,
    mbglIndex *src, mbglIndex *dst, mbglIndex *nallocedges, 
    unsigned int seed)
{
  using namespace yasmic;
  using namespace boost;
  
  assert(src && dst);
  assert(nallocedges);
  
  boost::mt19937 gen;
  if (seed != 0) {
    gen.seed(seed);
  } else {
    gen.seed(curtime());
  }

  typedef simple_csr_matrix<mbglIndex, double> crs_weighted_graph;
  typedef plod_iterator<boost::mt19937, crs_weighted_graph> 
    edge_generator;
  edge_generator cur(gen, nverts, alpha, beta, loops!=0);
  edge_generator end;
  
  mbglIndex nvalidedges = *nallocedges;
  mbglIndex ntotaledges = 0;
  
  for (; cur != end; ++cur) {
    if (ntotaledges < nvalidedges) {
      src[ntotaledges] = (mbglIndex) cur->first;
      dst[ntotaledges] = (mbglIndex) cur->second;
    }
    ++ntotaledges;
  }
  
  *nallocedges = ntotaledges;
  
  if (ntotaledges <= nvalidedges) {
    return 0;
  } else {
    return 1;
  }
}
