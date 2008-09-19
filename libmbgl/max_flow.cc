
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

#include <yasmic/simple_csr_matrix_as_graph.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmunds_karp_max_flow.hpp>
#include <yasmic/boost_mod/kolmogorov_max_flow.hpp>
#include <yasmic/iterator_utility.hpp>
#include <boost/property_map.hpp>

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

int edmunds_karp_max_flow(
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

    *flow = (edmunds_karp_max_flow(g,
        src, sink,
        capacity_map(make_iterator_property_map(cap, get(edge_index,g))).
        residual_capacity_map(make_iterator_property_map(res, get(edge_index,g))).
        reverse_edge_map(make_reverse_edge_pmap(g,rev_edge_index))));

    return (0);
}

int kolmogorov_max_flow(
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

    *flow = (kolmogorov_max_flow(g,
        make_iterator_property_map(cap, get(edge_index,g)),
        make_iterator_property_map(res, get(edge_index,g)),
        make_reverse_edge_pmap(g,rev_edge_index),
        get(vertex_index,g),
        src, sink));
    /*test_kolmogorov(g, 
        make_iterator_property_map(cap, get(edge_index,g)),
        make_iterator_property_map(res, get(edge_index,g)),
        make_reverse_edge_pmap(g,rev_edge_index));*/
        

    return (0);
}



