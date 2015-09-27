

#include <include/matlab_bgl.h>
#include <stdio.h>
#include "libmbgl_funcs_test.h"

int test_random_spanning_tree_directed() {
  const mbglIndex nverts = 10;
  mbglIndex rp[nverts+1];
  mbglIndex ci[2*nverts];
  mbglIndex i;
  
  rp[0] = 0;
  for (i=0; i<nverts; ++i) {
    rp[i+1] = 2*i;
    ci[2*i] = (i+1)%nverts;
    ci[2*i+1] = (i+2)%nverts;
  }
  
  mbglIndex ti[nverts], tj[nverts];
  double tv[nverts];
  
  int rval;
  mbglIndex nedges;
  rval = random_spanning_tree(
    nverts, ci, rp, NULL,
    ti, tj, tv, &nedges,
    0, 1);
  
  myassert(rval == 0, "function error");
  myassert(nedges == nverts-1, "edge size");
  
  return (0);
}


int spanning_trees_funcs_test() 
{
  int nfail= 0, ntotal= 0, rval;
  const char* name;

  name= "random_spanning_tree directed";
  rval= test_random_spanning_tree_directed(); ntotal++;
  if (rval != 0) { nfail++; printf("%20s  %50s\n", name, errstr); }
  else { printf("%20s  success\n", name); }
   
  printf("\n");
  printf("Total tests  : %3i\n", ntotal);
  printf("Total failed : %3i\n", nfail);

  return nfail;
}

