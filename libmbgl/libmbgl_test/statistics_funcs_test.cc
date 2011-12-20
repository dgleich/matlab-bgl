

#include <include/matlab_bgl.h>
#include <stdio.h>
#include "libmbgl_funcs_test.h"
#include <vector>


int test_bandwidth() 
{
  // same graph as test_king_ordering without extra edge
  const mbglIndex n=10;
  mbglIndex rp[] = {0,2,6,9,13,16,20,24,26,27,28}; 
  mbglIndex ci[] = {3,5,9,2,4,6,1,3,4,0,8,2,5,1,2,6,0,3,6,7,1,4,5,7,5,6,3,1};
  int rval;
  
  mbglIndex bw=0;
  rval= bandwidth(n, ci, rp, &bw);
  myassert(rval==0, "function error");
  myassert(bw==8, "incorrect bandwidth");
  return 0;
}

int statistics_funcs_test() 
{
  int nfail= 0, ntotal= 0, rval;
  
  testcase("bandwidth", test_bandwidth);
   
  printf("\n");
  printf("Total tests  : %3i\n", ntotal);
  printf("Total failed : %3i\n", nfail);

  return nfail!=0;
}
