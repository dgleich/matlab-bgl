

#include <include/matlab_bgl.h>
#include <stdio.h>
#include "libmbgl_funcs_test.h"
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include "boost_adjmatrix.hpp"
#include <boost/graph/isomorphism.hpp>

int check_map(mbglIndex* map, mbglIndex* perm, mbglIndex len)
{
    if (perm == NULL) {
        for (size_t i=0; i<len; ++i) {
            if (map[i] != i) { return 0; }
        }
    }
    return 1;
}

int test_isomorphism_boost() {
  using namespace boost;
  const mbglIndex n=10;
  mbglIndex rp1[] = {0,4,8,12,16,20,24,28,32,36,40};
  mbglIndex ci1[] = {1,3,6,7,0,2,3,4,9,4,5,1,0,1,6,7,9,2,5,1,8,2,4,7,8,9,3,0,0,8,3,5,9,5,6,7,8,2,4,6};
  
  mbglIndex rp2[] = {0,4,8,12,16,20,24,28,32,36,40};
  mbglIndex ci2[] = {1,3,5,7,0,2,3,4,9,4,6,1,0,1,6,7,1,2,5,9,8,9,4,0,8,2,3,7,0,8,3,6,9,5,6,7,8,2,4,5};

  //typedef boost::adjacency_list<boost::vecS,boost::vecS,boost::undirectedS> Graph;
  typedef adjacency_matrix<directedS> Graph;
    
  Graph G(n), H(n);
  
  for (size_t i=0; i<n; ++i) {
    for (mbglIndex nzi=rp1[i]; nzi<rp1[i+1]; ++nzi) {
      if (i>-ci1[nzi]) { // the negated index will always be true
        boost::add_edge(i,ci1[nzi],G);
      }
    }
  }
  
  for (size_t i=0; i<n; ++i) {
    for (mbglIndex nzi=rp2[i]; nzi<rp2[i+1]; ++nzi) {
      if (i>-ci2[nzi]) { // the negated index will always be true
        boost::add_edge(i,ci2[nzi],H);
      }
    }
  }
  
  typedef boost::graph_traits<Graph>::vertex_descriptor vertex;
  vertex map[n];
  
  if (isomorphism(G,G, isomorphism_map(
                        make_iterator_property_map(map,get(vertex_index,G))))==true) {
      printf("Isomorphic\n");
  } else {
      printf("Not isomorphic\n");
  }
  
  if (isomorphism(G,H, isomorphism_map(
                        make_iterator_property_map(map,get(vertex_index,G))))==true) {
      printf("Isomorphic\n");
      return 1;
  } else {
      printf("Not isomorphic\n");
      return 0;
  }
}

int test_isomorphism() {
  const mbglIndex n=10;
  mbglIndex rp1[] = {0,4,8,12,16,20,24,28,32,36,40};
  mbglIndex ci1[] = {1,3,6,7,0,2,3,4,9,4,5,1,0,1,6,7,9,2,5,1,8,2,4,7,8,9,3,0,0,8,3,5,9,5,6,7,8,2,4,6};
  
  mbglIndex rp2[] = {0,4,8,12,16,20,24,28,32,36,40};
  mbglIndex ci2[] = {1,3,5,7,0,2,3,4,9,4,6,1,0,1,6,7,1,2,5,9,8,9,4,0,8,2,3,7,0,8,3,6,9,5,6,7,8,2,4,5};

  int rval;
  int iso;
  mbglIndex map[n];
  mbglIndex map2[n];
  
  
  
  rval= isomorphism(n, ci1, rp1, n, ci1, rp1, &iso, map);
  myassert(rval==0, "function error");
  myassert(iso==1, "same graphs failed");
  //myassert(check_map(map, NULL, n), "invalid mapping");
  
  map[0] = -1;
  map[n-1] = -1;
  rval= isomorphism(n, ci1, rp1, n, ci2, rp2, &iso, map2);
  myassert(rval==0, "function error");
  for (size_t i=0; i<n; ++i) {
      printf("%lli -> %lli\n", i, map[i]);
  }
  myassert(iso==0, "3 non-isomorphic graphs detected as isomorphic");
  
  rval= isomorphism(n, ci1, rp1, n, ci1, rp1, &iso, map);
  myassert(rval==0, "function error");
  myassert(iso==1, "same graphs failed");
  //myassert(check_map(map, NULL, n), "invalid mapping");
  
  rval= isomorphism(n, ci1, rp1, n, ci2, rp2, &iso, map);
  myassert(rval==0, "function error");
  for (size_t i=0; i<n; ++i) {
      printf("%lli -> %lli\n", i, map[i]);
  }
  myassert(iso==0, "2 non-isomorphic graphs detected as isomorphic");
  
  printf("here\n");
  
  
  
  
  return 0;
}


int structure_funcs_test() 
{
  int nfail= 0, ntotal= 0, rval;
  
  testcase("isomorphism boost", test_isomorphism_boost);
  testcase("isomorphism", test_isomorphism);
   
  printf("\n");
  printf("Total tests  : %3i\n", ntotal);
  printf("Total failed : %3i\n", nfail);

  return nfail!=0;
}
