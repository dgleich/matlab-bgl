

#include <include/matlab_bgl.h>
#include <stdio.h>
#include "libmbgl_funcs_test.h"

int test_layout_1() {
  const mbglIndex n = 9;
  mbglIndex rp[] = {0,3,7,10,14,19,23,26,30,33};
  mbglIndex ci[] = {0,1,3,0,1,2,4,1,2,5,0,3,4,6,1,3,4,5,7,2,4,5,8,3,6,7,4,6,7,8,5,7,8};
  double w[33], X[2*n], dists[n*n], springs[n*n];
  int rval;
  for (int i=0; i<33; i++) { w[i]= 1.0; }
  rval= kamada_kawai_spring_layout(n, ci, rp, w, 0.0001, 100, 1.0, 0, 1.0,
                 X, dists, springs);
  if (rval!=0) {
    errstr = "function error";
    return -1;
  }
  return 0;
}

int test_layout_fr() {
  mbglIndex rp[] = {0, 3, 7, 11, 15, 19, 22, 26, 31, 36, 41, 46, 50, 54, 59, 64, 69, 74, 78, 82, 87, 92, 97, 102, 106, 109, 113, 117, 121, 125, 128};
  mbglIndex ci[] = {0, 1, 6, 0, 1, 2, 7, 1, 2, 3, 8, 2, 3, 4, 9, 3, 4, 5, 10, 4, 5, 11, 0, 6, 7, 12, 1, 6, 7, 8, 13, 2, 7, 8, 9, 14, 3, 8, 9, 10, 15, 4, 9, 10, 11, 16, 5, 10, 11, 17, 6, 12, 13, 18, 7, 12, 13, 14, 19, 8, 13, 14, 15, 20, 9, 14, 15, 16, 21, 10, 15, 16, 17, 22, 11, 16, 17, 23, 12, 18, 19, 24, 13, 18, 19, 20, 25, 14, 19, 20, 21, 26, 15, 20, 21, 22, 27, 16, 21, 22, 23, 28, 17, 22, 23, 29, 18, 24, 25, 19, 24, 25, 26, 20, 25, 26, 27, 21, 26, 27, 28, 22, 27, 28, 29, 23, 28, 29};
  double positions[60];

  int rval=fruchterman_reingold_force_directed_layout(
                30, ci, rp,
                100, 10.0, 1,
        50.0, 50.0, 0, positions);
  myassert(rval==0,"function error");
  return 0;
}

int layout_funcs_test() 
{
  int nfail= 0, ntotal= 0, rval;
  
  testcase("kamada_kawai 3-grid", test_layout_1);
  testcase("fr layout", test_layout_fr);
   
  printf("\n");
  printf("Total tests  : %3i\n", ntotal);
  printf("Total failed : %3i\n", nfail);

  return nfail;
}

