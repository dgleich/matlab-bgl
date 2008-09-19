/*
 * David Gleich
 * Copyright, Stanford University, 2006-2007
 */
 
 /**
 * @file components.cc
 * Implement wrappers for connected component functions
 */
 
/*
 * 19 April 2006
 * Initial version
 * 
 * 9 July 2007
 * Updated to use simple_csr_matrix graph type
 */

#include "include/matlab_bgl.h"

#include <yasmic/simple_csr_matrix_as_graph.hpp>
#include <yasmic/iterator_utility.hpp>
#include <boost/graph/biconnected_components.hpp>
#include <boost/graph/strong_components.hpp>

int strong_components(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, /* connectivity params */
    mbglIndex* ci)
{
    using namespace yasmic;
    using namespace boost;
    
    typedef simple_csr_matrix<mbglIndex,double> crs_graph;
    crs_graph g(nverts, nverts, ia[nverts], ia, ja, NULL);           
                    
    strong_components(g,
        make_iterator_property_map(
            ci, get(vertex_index,g)));

    return (0);
}

int biconnected_components(
    mbglIndex nverts, mbglIndex *ja, mbglIndex *ia, /* connectivity params */
    mbglIndex* a, mbglIndex* ci)
{
    using namespace yasmic;
    using namespace boost;

    typedef simple_csr_matrix<mbglIndex,double> crs_graph;
    crs_graph g(nverts, nverts, ia[nverts], ia, ja, NULL); 

    if (a)
    {
        if (ci)
        {
            std::size_t num_bicomps;
            mbglIndex *oi;
            boost::tie(num_bicomps, oi) =
                biconnected_components(g, 
                    make_iterator_property_map(ci, get(edge_index,g)),
                    a);
        }
        else
        {
            articulation_points(g, a);
        }
    }
    else
    {
        std::size_t num_bicomps = 
            biconnected_components(g, 
                    make_iterator_property_map(ci, get(edge_index,g)));
    }

    return (0);
}

