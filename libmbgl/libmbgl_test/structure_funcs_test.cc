

#include <include/matlab_bgl.h>
#include <stdio.h>
#include "libmbgl_funcs_test.h"
#include <vector>

int check_map(mbglIndex* map, mbglIndex* perm, mbglIndex len)
{
    if (perm == NULL) {
        for (size_t i=0; i<len; ++i) {
            if (map[i] != i) { return 0; }
        }
    }
    return 1;
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
  
  rval= isomorphism(n, ci1, rp1, n, ci2, rp2, &iso, map2);
  myassert(rval==0, "function error");
  myassert(iso==0, "non-isomorphic graphs detected as isomorphic");
  
  rval= isomorphism(n, ci1, rp1, n, ci1, rp1, &iso, map);
  myassert(rval==0, "function error");
  myassert(iso==1, "same graphs failed");
  myassert(check_map(map, NULL, n), "invalid mapping");
  
  rval= isomorphism(n, ci1, rp1, n, ci2, rp2, &iso, map2);
  myassert(rval==0, "function error");
  myassert(iso==0, "non-isomorphic graphs detected as isomorphic (repeat)");
  
  return 0;
}

int check_bipartite_part(mbglIndex n, mbglIndex *ci, mbglIndex *rp, mbglIndex *part) {
    for (mbglIndex i=0; i<n; ++i) {
        for (mbglIndex nzi=rp[i]; nzi<rp[i+1]; ++nzi) {
            if (part[i] == part[ci[nzi]]) {
                return 0;
            }
        }
    }
    return 1;
}

int check_odd_cycle(mbglIndex n, mbglIndex *ci, mbglIndex *rp, mbglIndex *cycle, mbglIndex len) {
    for (mbglIndex cv=0; cv<len-1; ++cv) {
        mbglIndex v = cycle[cv];
        bool done = false;
        for (mbglIndex nzi=rp[v]; nzi<rp[v+1]; ++nzi) {
            if (cycle[cv+1] == ci[nzi]) { done = true; }
        }
        if (!done) { return 0; }
    }
    mbglIndex v=cycle[len-1]; // last vertex should link back to first
    for (mbglIndex nzi=rp[v]; nzi<rp[v+1]; ++nzi) {
      if (cycle[0] == ci[nzi]) { return 1; }
    }
    return 0;
}

int test_bipartite_1() 
{
  // graph from boost test case  
  /**
   * Create the graph drawn below.
   *
   *       0 - 1 - 2
   *       |       |
   *   3 - 4 - 5 - 6
   *  /      \   /
   *  |        7
   *  |        |
   *  8 - 9 - 10
   **/

  const mbglIndex n=11;
  mbglIndex rp[] = {0,2,4,6,8,12,14,17,20,22,24,26};
  mbglIndex ci[] = {1,4,0,2,1,6,8,4,0,3,5,7,4,6,2,5,7,10,4,6,9,3,8,10,9,7};

  int isbi;
  mbglIndex part[n];
  
  int rval = is_bipartite(n, ci, rp, &isbi, part);
  myassert(rval==0,"function error");
  myassert(isbi==1,"bipartite graph not detected");
  myassert(check_bipartite_part(n, ci, rp, part),"invalid bipartite part");
  
  return 0;
}

int test_bipartite_2() 
{
    
  /**
   * Create the graph drawn below.
   * 
   *       2 - 1 - 0
   *       |       |
   *   3 - 6 - 5 - 4
   *  /      \   /
   *  |        7
   *  |       /
   *  8 ---- 9
   *  
   **/
  
  
  const mbglIndex n=10;
  mbglIndex rp[] = {0,2,4,6,8,12,14,17,20,22,24};
  mbglIndex ci[] = {1,4,0,2,1,6,8,4,0,3,5,7,4,6,2,5,7,9,4,6,9,3,8,7};

  int isbi;
  mbglIndex part[n];
  
  int rval = is_bipartite(n, ci, rp, &isbi, part);
  myassert(rval==0,"function error");
  myassert(isbi==0,"non-bipartite graph detected as bipartite");
  
  mbglIndex cycle[n+1];
  mbglIndex cyclelen;
  rval = find_odd_cycle(n, ci, rp, cycle, &cyclelen);
  myassert(rval==0,"function error");
  myassert(cyclelen % 2 == 1, "even length cycle");
  myassert(check_odd_cycle(n, ci, rp, cycle, cyclelen),"invalid cycle");
  
  return 0;
}

int test_bipartite_3() 
{
  
  const mbglIndex n=5;
  mbglIndex rp[n+1]; 
  mbglIndex ci[2*n]; 
  
  for (mbglIndex i=0; i<n; ++i) {
      ci[2*i] = (i+1) % n;
      ci[2*i+1] = (i-1) % n;
      if (i == 0) { ci[2*i+1] = n-1; }
      rp[i+1] = 2*(i+1);
  }
  rp[0] = 0;

  int isbi;
  mbglIndex part[n];
  
  int rval = is_bipartite(n, ci, rp, &isbi, part);
  myassert(rval==0,"function error");
  myassert(isbi==0,"non-bipartite graph detected as bipartite");
  
  mbglIndex cycle[n+1];
  mbglIndex cyclelen;
  rval = find_odd_cycle(n, ci, rp, cycle, &cyclelen);
  myassert(rval==0,"function error");
  myassert(cyclelen % 2 == 1, "even length cycle");
  myassert(cyclelen == n, "cycle too short");
  myassert(check_odd_cycle(n, ci, rp,cycle, cyclelen),"invalid cycle");
  
  return 0;
}

int structure_funcs_test() 
{
  int nfail= 0, ntotal= 0, rval;
  
  testcase("isomorphism", test_isomorphism);
  testcase("bipartite (true)", test_bipartite_1);
  testcase("bipartite (false)", test_bipartite_2);
  testcase("bipartite (cycle)", test_bipartite_3);
   
  printf("\n");
  printf("Total tests  : %3i\n", ntotal);
  printf("Total failed : %3i\n", nfail);

  return nfail!=0;
}
