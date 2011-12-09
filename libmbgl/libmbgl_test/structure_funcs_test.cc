

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


int structure_funcs_test() 
{
  int nfail= 0, ntotal= 0, rval;
  
  testcase("isomorphism", test_isomorphism);
   
  printf("\n");
  printf("Total tests  : %3i\n", ntotal);
  printf("Total failed : %3i\n", nfail);

  return nfail!=0;
}
