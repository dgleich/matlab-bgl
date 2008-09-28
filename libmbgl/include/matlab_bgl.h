/*
 * David F. Gleich
 * Copyright, Stanford University, 2006-2008
 */

/** History
 *  2007-07-04: Added maximum_cardinality_matching prototype
 *    Added dominator_tree prototype
 *  2008-09-25: Added kamada_kawai_spring_layout prototype
 *    Added fruchterman_reingold_force_directed_layout prototype
 *    Added gursoy_atun_layout prototype
 */

#ifndef MATLAB_BGL_H
#define MATLAB_BGL_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "matlab_bgl_types.h"

#define MATLAB_BGL_VISITOR_METHOD_VERTEX(a) int (*a)(void *pdata, mbglIndex u)
#define MATLAB_BGL_VISITOR_METHOD_EDGE(a) int (*a)(void *pdata, mbglIndex ei, mbglIndex u, mbglIndex v)

typedef struct {
    void *pdata;

    MATLAB_BGL_VISITOR_METHOD_VERTEX(initialize_vertex);
    MATLAB_BGL_VISITOR_METHOD_VERTEX(discover_vertex);
    MATLAB_BGL_VISITOR_METHOD_VERTEX(examine_vertex);
    MATLAB_BGL_VISITOR_METHOD_VERTEX(finish_vertex);

    MATLAB_BGL_VISITOR_METHOD_EDGE(examine_edge);
    MATLAB_BGL_VISITOR_METHOD_EDGE(tree_edge);
    MATLAB_BGL_VISITOR_METHOD_EDGE(non_tree_edge);
    MATLAB_BGL_VISITOR_METHOD_EDGE(gray_target);
    MATLAB_BGL_VISITOR_METHOD_EDGE(black_target);

} bfs_visitor_funcs_t;

typedef struct {
    void *pdata;

    MATLAB_BGL_VISITOR_METHOD_VERTEX(initialize_vertex);
    MATLAB_BGL_VISITOR_METHOD_VERTEX(start_vertex);
    MATLAB_BGL_VISITOR_METHOD_VERTEX(discover_vertex);
    MATLAB_BGL_VISITOR_METHOD_VERTEX(finish_vertex);

    MATLAB_BGL_VISITOR_METHOD_EDGE(examine_edge);
    MATLAB_BGL_VISITOR_METHOD_EDGE(tree_edge);
    MATLAB_BGL_VISITOR_METHOD_EDGE(back_edge);
    MATLAB_BGL_VISITOR_METHOD_EDGE(forward_or_cross_edge);
} dfs_visitor_funcs_t;

typedef struct {
    void *pdata;

    MATLAB_BGL_VISITOR_METHOD_VERTEX(initialize_vertex);
    MATLAB_BGL_VISITOR_METHOD_VERTEX(examine_vertex);
    MATLAB_BGL_VISITOR_METHOD_VERTEX(discover_vertex);
    MATLAB_BGL_VISITOR_METHOD_VERTEX(finish_vertex);

    MATLAB_BGL_VISITOR_METHOD_EDGE(examine_edge);
    MATLAB_BGL_VISITOR_METHOD_EDGE(edge_relaxed);
    MATLAB_BGL_VISITOR_METHOD_EDGE(edge_not_relaxed);
} dijkstra_visitor_funcs_t;

typedef struct {
    void *pdata;

    MATLAB_BGL_VISITOR_METHOD_VERTEX(initialize_vertex);
    MATLAB_BGL_VISITOR_METHOD_VERTEX(examine_vertex);
    MATLAB_BGL_VISITOR_METHOD_VERTEX(discover_vertex);
    MATLAB_BGL_VISITOR_METHOD_VERTEX(finish_vertex);

    MATLAB_BGL_VISITOR_METHOD_EDGE(examine_edge);
    MATLAB_BGL_VISITOR_METHOD_EDGE(edge_relaxed);
    MATLAB_BGL_VISITOR_METHOD_EDGE(edge_not_relaxed);
    MATLAB_BGL_VISITOR_METHOD_EDGE(black_target);
} astar_visitor_funcs_t;


typedef struct {
    void *pdata;

    MATLAB_BGL_VISITOR_METHOD_VERTEX(initialize_vertex);

    MATLAB_BGL_VISITOR_METHOD_EDGE(examine_edge);
    MATLAB_BGL_VISITOR_METHOD_EDGE(edge_relaxed);
    MATLAB_BGL_VISITOR_METHOD_EDGE(edge_not_relaxed);
    MATLAB_BGL_VISITOR_METHOD_EDGE(edge_minimized);
    MATLAB_BGL_VISITOR_METHOD_EDGE(edge_not_minimized);

} bellman_ford_visitor_funcs_t;

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
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia,  /* connectivity params */
    mbglIndex src, mbglIndex sink, /* flow data */
    int* cap, int* res, /* capacity and residual capacity */
    mbglIndex* rev_edge_index, int *flow);

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
int kolmogorov_max_flow(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia,  /* connectivity params */
    mbglIndex src, mbglIndex sink, /* flow data */
    int* cap, int* res, /* capacity and residual capacity */
    mbglIndex* rev_edge_index, int *flow);


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
int edmunds_karp_max_flow(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia,  /* connectivity params */
    mbglIndex src, mbglIndex sink, /* flow data */
    int* cap, int* res, /* capacity and residual capacity */
    mbglIndex* rev_edge_index, int *flow);

/**
 * Wrap a boost graph library call to bfs.
 *
 * the ja and ia arrays specify the connectivity of the underlying graph,
 * ia is a length (nverts+1) array with the indices in ja that start the
 * nonzeros in each row.  ja is a length (ia(nverts)) array with the
 * columns of the connectivity.
 *
 * if d, dt, or pred is NULL, then that parameter is not computed.
 *   (currently not implemented)
 *
 * @param nverts the number of vertices in the graph
 * @param ja the connectivity for each vertex
 * @param ia the row connectivity points into ja
 * @param src the source vertex for the search
 * @param dst a target vertex unless src=dst
 * @param d the distance array
 * @param dt the discover time array
 * @param pred the predecessor array
 * @return an error code if possible
 */
int breadth_first_search(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, /* connectivity params */
    mbglIndex src, mbglIndex dst, /* problem data */
    int* d, int* dt, mbglIndex* pred /* output data: distance, discover time, predecessor */
    );

int breadth_first_search_visitor(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, /* connectivity params */
    mbglIndex src, /* problem data */
    bfs_visitor_funcs_t vis /* visitor structure */
    );

/**
 * Wrap a boost graph library call to dfs.
 *
 * the ja and ia arrays specify the connectivity of the underlying graph,
 * ia is a length (nverts+1) array with the indices in ja that start the
 * nonzeros in each row.  ja is a length (ia(nverts)) array with the
 * columns of the connectivity.
 *
 * if dt, ft, or pred is NULL, then that parameter is not computed.
 *   (currently not implemented)
 *
 * @param nverts the number of vertices in the graph
 * @param ja the connectivity for each vertex
 * @param ia the row connectivity points into ja
 * @param src the source vertex for the flow
 * @param full if full is non-zero, then compute the full dfs over all
 *    vertices, not just the connected component.
 * @param dst the target vertex if n < nverts and src != dst which
 *    stops the dfs if it is reached
 * @param d
 * @param dt the discover time array
 * @param ft the finish time array
 * @param pred the predecessor array
 * @return an error code if possible
 */
int depth_first_search(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, /* connectivity params */
    mbglIndex src, int full, mbglIndex dst, /* problem data */
    int* d, int* dt, int* ft, mbglIndex *pred /* output data: discover time, finish time, predecessor */
    );

int depth_first_search_visitor(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, /* connectivity params */
    mbglIndex src, int full, /* problem data */
    dfs_visitor_funcs_t vis /* visitor structure */
    );


/**
 * Wrap a boost graph library call to biconnected_components.
 *
 * the ja and ia arrays specify the connectivity of the underlying graph,
 * ia is a length (nverts+1) array with the indices in ja that start the
 * nonzeros in each row.  ja is a length (ia(nverts)) array with the
 * columns of the connectivity.
 *
 * if a or ci is NULL, then that parameter is not computed.
 *
 * @param nverts the number of vertices in the graph
 * @param ja the connectivity for each vertex
 * @param ia the row connectivity points into ja
 * @param a an array which will store the articulaion points of the graph
 *     the array length should be n
 * @param ci the component index array which is length (nnz)
 * @return an error code if possible
 */

int biconnected_components(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, /* connectivity params */
    mbglIndex* a, mbglIndex* ci);

int strong_components(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, /* connectivity params */
    mbglIndex* ci);



int dijkstra_sp(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, double *weight, /* connectivity params */
    mbglIndex src, mbglIndex dst, /* problem data */
    double* d, mbglIndex *pred, double dinf);
int dijkstra_sp_visitor(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, double *weight, /* connectivity params */
    mbglIndex src, /* problem data */
    double* d, mbglIndex *pred, double dinf,
    dijkstra_visitor_funcs_t vis);

int bellman_ford_sp(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, double *weight, /* connectivity params */
    mbglIndex src, mbglIndex dst, /* problem data */
    double* d, mbglIndex *pred, double dinf);
int bellman_ford_sp_visitor(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, double *weight, /* connectivity params */
    mbglIndex src, /* problem data */
    double* d, mbglIndex *pred, double dinf,
    bellman_ford_visitor_funcs_t vis);


int dag_sp(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, double *weight, /* connectivity params */
    mbglIndex src, mbglIndex dst, /* problem data */
    double* d, mbglIndex *pred, double dinf);

int johnson_all_sp(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, double *weight, /* connectivity params */
    double* D, double dinf);

int floyd_warshall_all_sp(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, double *weight, /* connectivity params */
    double* D, double dinf, mbglIndex *pred);

int kruskal_mst(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, double *weight, /* connectivity params */
    mbglIndex* i, mbglIndex* j, double* val, mbglIndex* nedges /* tree output */);

int prim_mst(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, double *weight, /* connectivity params */
    mbglIndex* i, mbglIndex* j, double* val, mbglIndex* nedges /* tree output */);

int prim_mst_rooted(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, double *weight, /* connectivity params */
    mbglIndex* i, mbglIndex* j, double* val, mbglIndex* nedges, /* tree output */
    mbglIndex root);

/**
 * If weight == NULL, then the algorithm is used is slightly more
 * efficient, so if you don't want the weighted version, explicitly
 * set weight to NULL
 */
int betweenness_centrality(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, double *weight, /* connectivity params */
    double *centrality, double *ecentrality);

int clustering_coefficients(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, /* connectivity params */
    double *ccoeffs, int directed);
int weighted_clustering_coefficients(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, double *weight, /* connectivity params */
    double *ccoeffs);
int directed_clustering_coefficients(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, double *weight, /* connectivity params */
    double *ccoeffs);

int topological_order(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, /* connectivity params */
    mbglIndex *rev_order, int *is_dag);

int reverse_cuthill_mckee_order(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, /* connectivity params */
    mbglIndex start, /* input */
    mbglIndex *perm /* permutation output */);

int king_order(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, /* connectivity params */
    mbglIndex *perm /* permutation output */);

int minimum_degree_order(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, /* connectivity params */
    mbglIndex *perm /* permutation output */);

int sloan_order(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, /* connectivity params */
    mbglIndex *perm /* permutation output */);

int astar_search(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, double *weight, /* connectivity params */
    mbglIndex src, mbglIndex dst, /* start nd target vertices */
    double *d, mbglIndex *pred, double *f, /* output */
    double *h /* heuristic function value for all vertices */, double dinf);

int astar_search_hfunc(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, double *weight, /* connectivity params */
    mbglIndex src, mbglIndex dst, /* start and target vertices */
    double *d, mbglIndex *pred, double *f,
    double (*hfunc)(void* pdata, mbglIndex u), void* pdata /* heuristic function */, double dinf);

int astar_search_hfunc_visitor(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, double *weight, /* connectivity params */
    mbglIndex src, /* start vertex */
    double *d, mbglIndex *pred, double *f,
    double (*hfunc)(void* pdata, mbglIndex u), void* pdata /* heuristic function */, double dinf,
    astar_visitor_funcs_t vis);


int maximum_cardinality_matching(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, /* connectivity params */
    mbglIndex* mate, int initial_matching, int augmenting_path, int verify,
    int *verified, mbglIndex *null_vertex);

int test_maximum_cardinality_matching(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, /* connectivity params */
    mbglIndex* mate, int *verified);

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
    mbglDegreeType *cn, int *rt);

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
    double *cn, int *rt);

int dominator_tree(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia,
    mbglIndex src, mbglIndex *pred);

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
 * 			 size nverts-by-nverts
 * @param distance a matrix of distances between vertices,
 * 			 size nverts-by-nverts
 */
int kamada_kawai_spring_layout(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, double *weight,
    double tol, double spring_constant, int progressive, double edge_length,
    double *positions,
    double *spring_strength, double *distance);

/** Compute a force directed layout of a graph
 * @param nverts the number of vertices in the graph
 * @param ja the connectivity for each vertex
 * @param ia the row connectivity points into ja
 * @param iterations the number of iterations to run
 * @param initial_temp the initial temperature of the system
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
		double *positions);

/** The possible topologies for the gursoy_atun_layout
 */
typedef enum  {
	CUBE_LAYOUT_TOPOLOGY, BALL_LAYOUT_TOPOLOGY, HEART_LAYOUT_TOPOLOGY
} gursoy_atun_layout_topology_t;

extern const int gursoy_atun_invalid_dim;
extern const int gursoy_atun_dim_too_large;

/** Compute a topologically uniform layout
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
		double *positions);

#ifdef __cplusplus
}
#endif /* __cplusplus */



#endif /* MATLAB_BGL_H */


