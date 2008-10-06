

#include <include/matlab_bgl.h>
#include <stdio.h>

const char* errstr = "";

int test_1() {
  // line graph
  const mbglIndex n = 10;
  mbglIndex rp[] = {0,1,3,5,7,9,11,13,15,17,18};
  mbglIndex ci[] = {1,0,2,1,3,2,4,3,5,4,6,5,7,6,8,7,9,8};
  mbglIndex i[3*n-6];
  mbglIndex j[3*n-6];
  mbglIndex extra_edges;
  int rval;
  for (mbglIndex k=0; k<3*n-6; k++) { i[k]=j[k]=0; }
  rval= triangulate_graph(n, ci, rp, 1, 1, 1, i, j, &extra_edges);
  if (rval == 1) {
    errstr = "not planar";
    return -1;
  } else if (rval!=0) {
    errstr = "function error";
    return -1;
  }
  if (extra_edges + 9 != 3*n-6) {
    errstr = "insufficient edges";
    return -1;
  }
  for (mbglIndex k=0; k<extra_edges; k++) {
    if (i[k] == j[k]) {
      errstr = "invalid extra edges";
      return -1;
    }
  }
  return 0;
}

int test_2() {
  // line graph
  const mbglIndex n = 10;
  mbglIndex rp[] = {0,1,3,5,7,9,11,13,15,17,18};
  mbglIndex ci[] = {1,0,2,1,3,2,4,3,5,4,6,5,7,6,8,7,9,8};
  mbglIndex p[n];
  mbglDegreeType X[2*n];
  int rval;
  rval= chrobak_payne_straight_line_drawing(n, ci, rp, 0, 0, NULL, NULL, NULL, p, X);
  if (rval == 1) {
    errstr = "not planar";
    return -1;
  } else if (rval!=0) {
    errstr = "function error";
    return -1;
  }
  mbglIndex ei[3*n-6], ej[3*n-6], nedges;
  rval= chrobak_payne_straight_line_drawing(n, ci, rp, 0, 0, ei, ej, &nedges, p, X);
  if (rval == 1) {
    errstr = "not planar";
    return -1;
  } else if (rval!=0) {
    errstr = "function error";
    return -1;
  }
  if (nedges +9 != 3*n-6) {
    errstr = "incorrect extra edges";
    return -1;
  }
  return 0;
}

int main(int argc, char **argv) {
  int nfail = 0, ntotal = 0;
  int rval;

  const char* name;

  name= "make_maximal_planar";
  rval= test_1(); ntotal++;
  if (rval!= 0) { nfail++; printf("%20s  %50s\n", name, errstr); }
  else { printf("%20s  success\n", name); }

  name= "straight_line";
  rval= test_1(); ntotal++;
  if (rval!= 0) { nfail++; printf("%20s  %50s\n", name, errstr); }
  else { printf("%20s  success\n", name); }

  printf("\n");
  printf("Total tests  : %3i\n", ntotal);
  printf("Total failed : %3i\n", nfail);

  return nfail!=0;
}
