

#include <include/matlab_bgl.h>
#include <stdio.h>
#include "libmbgl_funcs_test.h"

const char* errstr = "";


int main(int argc, char **argv) {
  int nfail=0;
  nfail += layout_funcs_test();
  nfail += planar_funcs_test();
  nfail += ordering_funcs_test();
  nfail += spanning_trees_funcs_test();
  
  printf("\n");
  printf("Overall\n");
  printf("=======\n");
  if (nfail > 0) {
    printf("%i FAILURES (WARNING)\n", nfail);
  } else {
    printf("0 failures\n");
  }
  printf("\n");
  
  return 0;
}
