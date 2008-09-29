/** @file layouts.cc
 * @author David F. Gleich
 * @date 2008-09-25
 * @copyright Stanford University, 2008
 * Graph layout wrappers
 */

/** History
 *  2008-09-25: Initial coding
 *  2008-09-27: Fixed progressive calls
 */

#include "include/matlab_bgl.h"

#include <yasmic/undir_simple_csr_matrix_as_graph.hpp>
#include <yasmic/simple_csr_matrix_as_graph.hpp>
#include <yasmic/iterator_utility.hpp>

#include <boost/graph/kamada_kawai_spring_layout.hpp>
#include <yasmic/boost_mod/fruchterman_reingold.hpp>
#include <boost/graph/gursoy_atun_layout.hpp>
#include <boost/graph/circle_layout.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/simple_point.hpp>

// for constant_value_property_map
#include <yasmic/boost_mod/core_numbers.hpp>

#include <vector>
#include <iostream>
#include <algorithm>

#include <math.h>

#include "libmbgl_util.hpp"

/** Compute a spring layout of a graph
 * @param nverts the number of vertices in the graph
 * @param ja the connectivity for each vertex
 * @param ia the row connectivity points into ja
 * @param weight the weight of each edge (can be NULL for unweighted graphs)
 * @param tol the stopping tolerance in terms of layout change
 * @param spring_constant the spring constant
 * @param progressive a binary value (0 or 1) if we should reuse the positions
 * @param positions an array of positions, length nverts*2
 * @param spring_strength a matrix of spring strengths between vertices,
 *       size nverts-by-nverts
 * @param distance a matrix of distances between vertices,
 *       size nverts-by-nverts
 */
int kamada_kawai_spring_layout(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, double *weight,
    double tol, double spring_constant, int progressive, double edge_length,
    double *positions,
    double *spring_strength, double *distance)
{
  using namespace yasmic;
  using namespace boost;
  typedef undir_simple_csr_matrix<mbglIndex,double> crs_graph;
  crs_graph g(nverts, nverts, ia[nverts], ia, ja, weight);
  if (tol < 0 || spring_strength == NULL || distance == NULL) { return -1; }
  std::vector<simple_point<double> > position_vec(nverts);
  std::vector<std::pair<double,double> > partial_deriv_vec(nverts);
  if (!progressive) {
    // initial random layout
    circle_graph_layout(g,
        make_iterator_property_map(position_vec.begin(),get(vertex_index,g)),
        (double)nverts*edge_length/(2*M_PI));
  } else {
    // copy the layout from positions
    mbglIndex n = num_vertices(g);
    for (mbglIndex i = 0; i<n; i++) {
      position_vec[i].x = positions[i+0*n];
      position_vec[i].y = positions[i+1*n];
    }
  }
  bool rval = false;
  if (weight == NULL) {
    rval = kamada_kawai_spring_layout(g,
      make_iterator_property_map(position_vec.begin(),get(vertex_index,g)),
      boost::detail::constant_value_property_map<double>(1.0), // edge_weight
      boost::edge_length(edge_length), // edge_or_side_length
      layout_tolerance<double>(tol), // done
      spring_constant,
      get(vertex_index,g),
      row_matrix<double>(distance,nverts,nverts),
      row_matrix<double>(spring_strength,nverts,nverts),
      make_iterator_property_map(partial_deriv_vec.begin(),get(vertex_index,g)));
  } else {
    rval = kamada_kawai_spring_layout(g,
      make_iterator_property_map(position_vec.begin(),get(vertex_index,g)),
      get(edge_weight,g), // edge_weight
      boost::edge_length(edge_length), // edge_or_side_length
      layout_tolerance<double>(tol), // done
      spring_constant,
      get(vertex_index,g),
      row_matrix<double>(distance,nverts,nverts),
      row_matrix<double>(spring_strength,nverts,nverts),
      make_iterator_property_map(partial_deriv_vec.begin(),get(vertex_index,g)));
  }
  if (rval) {
    mbglIndex n = num_vertices(g);
    for (mbglIndex i = 0; i<n; i++) {
      positions[i+0*n] = position_vec[i].x;
      positions[i+1*n] = position_vec[i].y;
    }
    return 0;
  } else {
    return -2;
  }
}

/** Compute a force directed layout of a graph
 * @param nverts the number of vertices in the graph
 * @param ja the connectivity for each vertex
 * @param ia the row connectivity points into ja
 * @param iterations the number of iterations to run
 * @param grid_force_pairs a binary value (0 or 1) if the alg should use
 *   a grid to compute the force between pairs
 * @param width the total width of the layout
 * @param height the total height of the layout
 * @param progressive a binary value (0 or 1) if we should start from
 *   the positions in the positions value
 * @param positions an array of positions, length nverts*2
 */
int fruchterman_reingold_force_directed_layout(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia,
    int iterations, double initial_temp, int grid_force_pairs,
    double width, double height, int progressive,
    double *positions)
{
  using namespace yasmic;
  using namespace boost;
  typedef simple_csr_matrix<mbglIndex,double> crs_graph;
  crs_graph g(nverts, nverts, ia[nverts], ia, ja, NULL);
  std::vector<simple_point<double> > position_vec(nverts);
  if (width <= 0 || height <= 0 || iterations <= 0) { return -1; }
  if (!progressive) {
    // initial random layout
    minstd_rand gen;
    random_graph_layout(g,
        make_iterator_property_map(position_vec.begin(),get(vertex_index,g)),
        -width/2.0, width/2.0, -height/2.0, height/2.0, gen);
  } else {
    // copy the layout from positions
    mbglIndex n = num_vertices(g);
    for (mbglIndex i = 0; i<n; i++) {
      position_vec[i].x = positions[i+0*n];
      position_vec[i].y = positions[i+1*n];
    }
  }
  /*std::cout << "iterations " << iterations << std::endl;
  std::cout << "width " << width << std::endl;
  std::cout << "height " << height << std::endl;
  for (mbglIndex i = 0; i < std::min BOOST_PREVENT_MACRO_SUBSTITUTION ((mbglIndex)5,nverts); i++) {
    std::cout << "p[" << i << "]=("<<position_vec[i].x<<","<<position_vec[i].y<<")" << std::endl;
  }*/
  if (grid_force_pairs) {
    fruchterman_reingold_force_directed_layout(g,
        make_iterator_property_map(position_vec.begin(),get(vertex_index,g)),
        width, height,
        cooling(linear_cooling<double>(iterations, initial_temp)));
  } else {
    fruchterman_reingold_force_directed_layout(g,
        make_iterator_property_map(position_vec.begin(),get(vertex_index,g)),
        width, height,
        cooling(linear_cooling<double>(iterations, initial_temp)).
          force_pairs(all_force_pairs()));
  }
  /*std::vector<simple_point<double> > displacement_vec(nverts);
  if (grid_force_pairs) {
    std::cout << "gridded" << std::endl;
    fruchterman_reingold_force_directed_layout(
      g,
      make_iterator_property_map(position_vec.begin(),get(vertex_index,g)),
      width,
      height,
      square_distance_attractive_force(),
      square_distance_repulsive_force(),
      make_grid_force_pairs(width, height,
        make_iterator_property_map(position_vec.begin(),get(vertex_index,g)),
        g),
      linear_cooling<double>(iterations),
      make_iterator_property_map(displacement_vec.begin(),get(vertex_index,g), simple_point<double>())
      );
  } else {
    std::cout << "all" << std::endl;
    fruchterman_reingold_force_directed_layout(
      g,
      make_iterator_property_map(position_vec.begin(),get(vertex_index,g)),
      width,
      height,
      square_distance_attractive_force(),
      square_distance_repulsive_force(),
      all_force_pairs(),
      linear_cooling<double>(iterations),
      make_iterator_property_map(displacement_vec.begin(),get(vertex_index,g), simple_point<double>())
      );
  }
  for (mbglIndex i = 0; i < std::min BOOST_PREVENT_MACRO_SUBSTITUTION ((mbglIndex)5,nverts); i++) {
      std::cout << "d[" << i << "]=("<<displacement_vec[i].x<<","<<displacement_vec[i].y<<")" << std::endl;
    }*/
  // copy the positions over
  mbglIndex n = num_vertices(g);
  for (mbglIndex i = 0; i<n; i++) {
    positions[i+0*n] = position_vec[i].x;
    positions[i+1*n] = position_vec[i].y;
  }
  return 0;
}


/** A helper function to allocate the points and convert back to positions
 */
template <typename Graph, typename Topology>
int gursoy_atun_layout_helper(const Graph& g, bool weighted,
    Topology space, int space_dim, int nsteps,
    double diameter_i, double diameter_f, double lc_i, double lc_f,
    double *positions)
{
  using namespace boost;
  std::vector<typename Topology::point_type> position_map(num_vertices(g));
  if (weighted) {
    gursoy_atun_layout(g, space,
      make_iterator_property_map(position_map.begin(),get(vertex_index,g)),
      nsteps, diameter_i, diameter_f, lc_i, lc_f, get(vertex_index,g),
      get(edge_weight,g));
  } else {
    gursoy_atun_layout(g, space,
          make_iterator_property_map(position_map.begin(),get(vertex_index,g)),
          nsteps, diameter_i, diameter_f, lc_i, lc_f, get(vertex_index,g));
  }
  // copy the positions over
  mbglIndex n = num_vertices(g);
  for (mbglIndex i = 0; i<n; i++) {
    for (mbglIndex d = 0; d<space_dim; d++) {
      positions[i+d*n] = position_map[i][d];
    }
  }
}

const int gursoy_atun_invalid_dim = -10;
const int gursoy_atun_dim_too_large = -11;
const int gursoy_atun_max_dim = 10;

/** Compute a topologically uniform layout
 *
 * @param nverts the number of vertices in the graph
 * @param ja the connectivity for each vertex
 * @param ia the row connectivity points into ja
 * @param weight the weight of each edge (can be NULL for unweighted graphs)
 * @param topology the topology type
 * @param topology_dim the topology dimension
 * @param iterations the number of iterations
 * @param diameter_i the initial diameter for modifications
 * @param diameter_f the final diameter for modifications
 * @param learning_constant_i the initial learning constant
 * @param learning_constant_f the final learning constant
 * @param positions an array of positions, length nverts*topology_dim
 */

int gursoy_atun_layout(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, double *weight,
    gursoy_atun_layout_topology_t topology, int topology_dim,
    int iterations, double diameter_i, double diameter_f,
    double learning_constant_i, double learning_constant_f,
    double *positions)
{
  using namespace yasmic;
  using namespace boost;

  typedef simple_csr_matrix<mbglIndex,double> crs_graph;
  crs_graph g(nverts, nverts, ia[nverts], ia, ja, weight);

  switch (topology) {
  case BALL_LAYOUT_TOPOLOGY:
    if (topology_dim<2) { return gursoy_atun_invalid_dim; }
    if (topology_dim>gursoy_atun_max_dim) { return gursoy_atun_dim_too_large; }
    switch (topology_dim) {
    case 2:
      gursoy_atun_layout_helper(g, weight!=NULL,
          ball_topology<2>(),topology_dim,iterations,diameter_i,diameter_f,
          learning_constant_i, learning_constant_f, positions);
      break;
    case 3:
      gursoy_atun_layout_helper(g, weight!=NULL,
          ball_topology<3>(),topology_dim,iterations,diameter_i,diameter_f,
          learning_constant_i, learning_constant_f, positions);
      break;
    case 4:
      gursoy_atun_layout_helper(g, weight!=NULL,
          ball_topology<4>(),topology_dim,iterations,diameter_i,diameter_f,
          learning_constant_i, learning_constant_f, positions);
      break;
    case 5:
      gursoy_atun_layout_helper(g, weight!=NULL,
          ball_topology<5>(),topology_dim,iterations,diameter_i,diameter_f,
          learning_constant_i, learning_constant_f, positions);
      break;
    case 6:
      gursoy_atun_layout_helper(g, weight!=NULL,
          ball_topology<6>(),topology_dim,iterations,diameter_i,diameter_f,
          learning_constant_i, learning_constant_f, positions);
      break;
    case 7:
      gursoy_atun_layout_helper(g, weight!=NULL,
          ball_topology<7>(),topology_dim,iterations,diameter_i,diameter_f,
          learning_constant_i, learning_constant_f, positions);
      break;
    case 8:
      gursoy_atun_layout_helper(g, weight!=NULL,
          ball_topology<8>(),topology_dim,iterations,diameter_i,diameter_f,
          learning_constant_i, learning_constant_f, positions);
      break;
    case 9:
      gursoy_atun_layout_helper(g, weight!=NULL,
          ball_topology<9>(),topology_dim,iterations,diameter_i,diameter_f,
          learning_constant_i, learning_constant_f, positions);
      break;
    case 10:
      gursoy_atun_layout_helper(g, weight!=NULL,
          ball_topology<10>(),topology_dim,iterations,diameter_i,diameter_f,
          learning_constant_i, learning_constant_f, positions);
      break;
    }
    break;
  case CUBE_LAYOUT_TOPOLOGY:
    if (topology_dim<2) { return gursoy_atun_invalid_dim; }
    if (topology_dim>gursoy_atun_max_dim) { return gursoy_atun_dim_too_large; }
    switch (topology_dim) {
    case 2:
      gursoy_atun_layout_helper(g, weight!=NULL,
          hypercube_topology<2>(),topology_dim,iterations,diameter_i,diameter_f,
          learning_constant_i, learning_constant_f, positions);
      break;
    case 3:
      gursoy_atun_layout_helper(g, weight!=NULL,
          hypercube_topology<3>(),topology_dim,iterations,diameter_i,diameter_f,
          learning_constant_i, learning_constant_f, positions);
      break;
    case 4:
      gursoy_atun_layout_helper(g, weight!=NULL,
          hypercube_topology<4>(),topology_dim,iterations,diameter_i,diameter_f,
          learning_constant_i, learning_constant_f, positions);
      break;
    case 5:
      gursoy_atun_layout_helper(g, weight!=NULL,
          hypercube_topology<5>(),topology_dim,iterations,diameter_i,diameter_f,
          learning_constant_i, learning_constant_f, positions);
      break;
    case 6:
      gursoy_atun_layout_helper(g, weight!=NULL,
          hypercube_topology<6>(),topology_dim,iterations,diameter_i,diameter_f,
          learning_constant_i, learning_constant_f, positions);
      break;
    case 7:
      gursoy_atun_layout_helper(g, weight!=NULL,
          hypercube_topology<7>(),topology_dim,iterations,diameter_i,diameter_f,
          learning_constant_i, learning_constant_f, positions);
      break;
    case 8:
      gursoy_atun_layout_helper(g, weight!=NULL,
          hypercube_topology<8>(),topology_dim,iterations,diameter_i,diameter_f,
          learning_constant_i, learning_constant_f, positions);
      break;
    case 9:
      gursoy_atun_layout_helper(g, weight!=NULL,
          hypercube_topology<9>(),topology_dim,iterations,diameter_i,diameter_f,
          learning_constant_i, learning_constant_f, positions);
      break;
    case 10:
      gursoy_atun_layout_helper(g, weight!=NULL,
          hypercube_topology<10>(),topology_dim,iterations,diameter_i,diameter_f,
          learning_constant_i, learning_constant_f, positions);
      break;
    }
    break;
  case HEART_LAYOUT_TOPOLOGY:
    if (topology_dim!=2) { return gursoy_atun_invalid_dim; }
    gursoy_atun_layout_helper(g, weight!=NULL,
      heart_topology<>(),topology_dim,iterations,diameter_i,diameter_f,
      learning_constant_i, learning_constant_f, positions);
    break;
  default:
    return -1;
  }
  return 0;
}
