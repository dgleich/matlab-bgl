// Copyright 2002 Rensselaer Polytechnic Institute

// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Lauren Foutz
//           Scott Hill

// Predecessor Matrix implementation added by David Gleich, 2006.

/*
  This file implements the functions

  template <class VertexListGraph, class DistanceMatrix, 
    class P, class T, class R>
  bool floyd_warshall_initialized_all_pairs_shortest_paths(
    const VertexListGraph& g, DistanceMatrix& d, 
    const bgl_named_params<P, T, R>& params)

  AND

  template <class VertexAndEdgeListGraph, class DistanceMatrix, 
    class P, class T, class R>
  bool floyd_warshall_all_pairs_shortest_paths(
    const VertexAndEdgeListGraph& g, DistanceMatrix& d, 
    const bgl_named_params<P, T, R>& params)
*/


#ifndef BOOST_GRAPH_FLOYD_WARSHALL_HPP
#define BOOST_GRAPH_FLOYD_WARSHALL_HPP

#include <boost/property_map/property_map.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/relax.hpp>

namespace boost
{
  namespace detail {

    class dummy_predecessor_matrix {
    public:
      inline dummy_predecessor_matrix() {}
      inline dummy_predecessor_matrix(const dummy_predecessor_matrix& x)
        : p(x.p) {}
      typedef dummy_property_map reference;
      template <class K>
      inline reference operator[](K) const { return p; }
    protected:
      dummy_property_map p;
    };

    template<typename T, typename BinaryPredicate>
    T min_with_compare(const T& x, const T& y, const BinaryPredicate& compare)
    {
      if (compare(x, y)) return x; 
      else return y;
    }

    template <typename T, typename BinaryPredicate>
    std::pair<T,bool>
      min_with_compare_and_choice(const T& x, const T& y, 
        const BinaryPredicate& compare)
    {
        if (compare(x,y)) return std::make_pair(x,true);
        else return std::make_pair(y,false);
    }

    template<typename VertexListGraph, typename DistanceMatrix, 
      typename PredecessorMatrix,
      typename BinaryPredicate, typename BinaryFunction,
      typename Infinity, typename Zero>
    bool floyd_warshall_dispatch(const VertexListGraph& g, 
      DistanceMatrix& d, PredecessorMatrix& p,
      const BinaryPredicate &compare, 
      const BinaryFunction &combine, const Infinity& inf, 
      const Zero& zero)
    {
      typename graph_traits<VertexListGraph>::vertex_iterator 
        i, lasti, j, lastj, k, lastk;

      bool change_predecessor=false;
      
      for (tie(k, lastk) = vertices(g); k != lastk; k++)
        for (tie(i, lasti) = vertices(g); i != lasti; i++)
          for (tie(j, lastj) = vertices(g); j != lastj; j++)
          {
              boost::tie(d[*i][*j],change_predecessor) =
                detail::min_with_compare_and_choice(combine(d[*i][*k], d[*k][*j]),
                                       d[*i][*j],
                                       compare);
              if (change_predecessor) p[*i][*j] = p[*k][*j];
          }
      
    
      for (tie(i, lasti) = vertices(g); i != lasti; i++)
        if (compare(d[*i][*i], zero))
          return false;
      return true;
    }
  }

  template <typename VertexListGraph, typename DistanceMatrix, 
    typename PredecessorMatrix, typename BinaryPredicate, 
    typename BinaryFunction, typename Infinity, typename Zero>
  bool floyd_warshall_initialized_all_pairs_shortest_paths(
    const VertexListGraph& g, DistanceMatrix& d, PredecessorMatrix& p,
    const BinaryPredicate& compare, 
    const BinaryFunction& combine, const Infinity& inf, 
    const Zero& zero)
  {
    function_requires<VertexListGraphConcept<VertexListGraph> >();
  
    return detail::floyd_warshall_dispatch(g, d, p, compare, combine, 
    inf, zero);
  }

  template <typename VertexAndEdgeListGraph, typename DistanceMatrix, 
    typename PredecessorMatrix, typename WeightMap, typename BinaryPredicate, 
    typename BinaryFunction, typename Infinity, typename Zero>
  bool floyd_warshall_all_pairs_shortest_paths(
    const VertexAndEdgeListGraph& g, 
    DistanceMatrix& d, PredecessorMatrix& p, const WeightMap& w, 
    const BinaryPredicate& compare, const BinaryFunction& combine, 
    const Infinity& inf, const Zero& zero)
  {
    function_requires<VertexListGraphConcept<VertexAndEdgeListGraph> >();
    function_requires<EdgeListGraphConcept<VertexAndEdgeListGraph> >();
    function_requires<IncidenceGraphConcept<VertexAndEdgeListGraph> >();
  
    typename graph_traits<VertexAndEdgeListGraph>::vertex_iterator 
      firstv, lastv, firstv2, lastv2;
    typename graph_traits<VertexAndEdgeListGraph>::edge_iterator first, last;
  
    
    for(tie(firstv, lastv) = vertices(g); firstv != lastv; firstv++)
      for(tie(firstv2, lastv2) = vertices(g); firstv2 != lastv2; firstv2++) {
        d[*firstv][*firstv2] = inf;
        p[*firstv][*firstv2] = *firstv2;
      }
    
    for(tie(firstv, lastv) = vertices(g); firstv != lastv; firstv++)
      d[*firstv][*firstv] = zero;
    
    bool change_predecessor;
    
    for(tie(first, last) = edges(g); first != last; first++)
    {
      if (d[source(*first, g)][target(*first, g)] != inf) {
        tie(d[source(*first, g)][target(*first, g)],change_predecessor) = 
          detail::min_with_compare_and_choice(
            get(w, *first), 
            d[source(*first, g)][target(*first, g)],
            compare);
        if (change_predecessor) { 
          p[source(*first, g)][target(*first, g)] = source(*first, g);
        }
      } else {
        d[source(*first, g)][target(*first, g)] = get(w, *first);
        p[source(*first, g)][target(*first, g)] = source(*first,g);
      }
    }
    
    bool is_undirected = is_same<typename 
      graph_traits<VertexAndEdgeListGraph>::directed_category, 
      undirected_tag>::value;
    if (is_undirected)
    {
      for(tie(first, last) = edges(g); first != last; first++)
      {
        if (d[target(*first, g)][source(*first, g)] != inf) {
          tie(d[target(*first, g)][source(*first, g)],change_predecessor) = 
            detail::min_with_compare_and_choice(
              get(w, *first), 
              d[target(*first, g)][source(*first, g)],
              compare);
          if (change_predecessor) { 
            p[target(*first, g)][source(*first, g)] = target(*first, g);
          }
        } else {
          d[target(*first, g)][source(*first, g)] = get(w, *first);
          p[target(*first, g)][source(*first, g)] = target(*first,g);
        }
      }
    }
    
  
    return detail::floyd_warshall_dispatch(g, d, p, compare, combine, 
      inf, zero);
  }

  template <typename VertexAndEdgeListGraph, typename DistanceMatrix, 
    typename WeightMap, typename BinaryPredicate, 
    typename BinaryFunction, typename Infinity, typename Zero>
  bool floyd_warshall_all_pairs_shortest_paths(
    const VertexAndEdgeListGraph& g, 
    DistanceMatrix& d, const WeightMap& w, 
    const BinaryPredicate& compare, const BinaryFunction& combine, 
    const Infinity& inf, const Zero& zero)
  {
      detail::dummy_predecessor_matrix p_matrix;
      return floyd_warshall_all_pairs_shortest_paths(
          g, d, p_matrix, w, compare, combine, inf, zero);
  }
  

  namespace detail {        
    template <class VertexListGraph, class DistanceMatrix, 
      class WeightMap, class P, class T, class R>
    bool floyd_warshall_init_dispatch(const VertexListGraph& g, 
      DistanceMatrix& d, WeightMap w, 
      const bgl_named_params<P, T, R>& params)
    {
      typedef typename property_traits<WeightMap>::value_type WM;
      dummy_predecessor_matrix p_matrix;
    
      return floyd_warshall_initialized_all_pairs_shortest_paths(g, d,
        choose_param(get_param(params, vertex_predecessor), p_matrix),
        w,
        choose_param(get_param(params, distance_compare_t()), 
          std::less<WM>()),
        choose_param(get_param(params, distance_combine_t()), 
          closed_plus<WM>()),
        choose_param(get_param(params, distance_inf_t()), 
          std::numeric_limits<WM>::max BOOST_PREVENT_MACRO_SUBSTITUTION()),
        choose_param(get_param(params, distance_zero_t()), 
          WM()));
    }
    

    
    template <class VertexAndEdgeListGraph, class DistanceMatrix, 
      class WeightMap, class P, class T, class R>
    bool floyd_warshall_noninit_dispatch(const VertexAndEdgeListGraph& g, 
      DistanceMatrix& d, WeightMap w, 
      const bgl_named_params<P, T, R>& params)
    {
      typedef typename property_traits<WeightMap>::value_type WM;
      dummy_predecessor_matrix p_matrix;
    
      return floyd_warshall_all_pairs_shortest_paths(g, d, 
        choose_param(get_param(params, vertex_predecessor), p_matrix),
        w,
        choose_param(get_param(params, distance_compare_t()), 
          std::less<WM>()),
        choose_param(get_param(params, distance_combine_t()), 
          closed_plus<WM>()),
        choose_param(get_param(params, distance_inf_t()), 
          std::numeric_limits<WM>::max BOOST_PREVENT_MACRO_SUBSTITUTION()),
        choose_param(get_param(params, distance_zero_t()), 
          WM()));
    }
    

  }   // namespace detail

  
  
  template <class VertexListGraph, class DistanceMatrix, class P, 
    class T, class R>
  bool floyd_warshall_initialized_all_pairs_shortest_paths(
    const VertexListGraph& g, DistanceMatrix& d, 
    const bgl_named_params<P, T, R>& params)
  {
    return detail::floyd_warshall_init_dispatch(g, d, 
      choose_const_pmap(get_param(params, edge_weight), g, edge_weight), 
      params);
  }
  
  template <class VertexListGraph, class DistanceMatrix>
  bool floyd_warshall_initialized_all_pairs_shortest_paths(
    const VertexListGraph& g, DistanceMatrix& d)
  {
    bgl_named_params<int,int> params(0);
    return detail::floyd_warshall_init_dispatch(g, d,
      get(edge_weight, g), params);
  }
  

  
  
  template <class VertexAndEdgeListGraph, class DistanceMatrix, 
    class P, class T, class R>
  bool floyd_warshall_all_pairs_shortest_paths(
    const VertexAndEdgeListGraph& g, DistanceMatrix& d, 
    const bgl_named_params<P, T, R>& params)
  {
    return detail::floyd_warshall_noninit_dispatch(g, d, 
      choose_const_pmap(get_param(params, edge_weight), g, edge_weight), 
      params);
  }
  
  template <class VertexAndEdgeListGraph, class DistanceMatrix>
  bool floyd_warshall_all_pairs_shortest_paths(
    const VertexAndEdgeListGraph& g, DistanceMatrix& d)
  {
    bgl_named_params<int,int> params(0);
    return detail::floyd_warshall_noninit_dispatch(g, d,
      get(edge_weight, g), params);
  }

} // namespace boost

#endif

