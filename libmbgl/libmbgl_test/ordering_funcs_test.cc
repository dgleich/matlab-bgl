

#include <include/matlab_bgl.h>
#include <stdio.h>
#include "libmbgl_funcs_test.h"
#include <vector>

int check_perm(mbglIndex *perm, mbglIndex len) {
    std::vector<mbglIndex> full(len,0);
    for (size_t i=0; i<len; ++i) {
        if (perm[i] >= len) {
            return 0;
        }
        if (full[perm[i]] != 0) {
            return 0;
        }
        full[perm[i]] = 1;
    }
    for (size_t i=0; i<len; ++i) {
        if (full[i] != 1) {
            return 0;
        }
    }
    return 1;
}

int test_king_ordering() {
  const mbglIndex n=10;
  mbglIndex rp[] = {0,2,6,9,13,16,20,24,26,27,28,28}; // allocate one extra vertex at the end
  mbglIndex ci[] = {3,5,9,2,4,6,1,3,4,0,8,2,5,1,2,6,0,3,6,7,1,4,5,7,5,6,3,1};
  mbglIndex iperm[n];
  mbglIndex iperm2[n+1];
  mbglIndex ipermlen=0;
  int rval;
  rval= king_order(n, ci, &rp[1], 0, iperm, &ipermlen);
  myassert(rval==0, "function error");
  myassert(ipermlen == n, "perm length incorrect");
  myassert(check_perm(iperm,ipermlen),"invalid perm");
  rval= king_order(n, ci, &rp[1], n, iperm, &ipermlen);
  myassert(rval==0, "function error");
  myassert(ipermlen == n, "perm length incorrect");
  myassert(check_perm(iperm,ipermlen),"invalid perm");
  
  rval= king_order(n+1, ci, rp, 1, iperm2, &ipermlen);
  myassert(rval==0, "function error");
  myassert(ipermlen == n, "perm length incorrect (short)");
  myassert(check_perm(iperm2,ipermlen),"invalid perm");
  
  
  rval= king_order(n+1, ci, rp, n+1, iperm2, &ipermlen);
  myassert(rval==0, "function error");
  myassert(ipermlen == n+1, "perm length incorrect (long)");
  myassert(check_perm(iperm2,ipermlen),"invalid perm");
  
  
  return 0;
}

int ordering_funcs_test() 
{
  int nfail= 0, ntotal= 0, rval;
  const char* name;

  name= "king_ordering";
  rval= test_king_ordering(); ntotal++;
  if (rval != 0) { nfail++; printf("%20s  %50s\n", name, errstr); }
  else { printf("%20s  success\n", name); }
   
  printf("\n");
  printf("Total tests  : %3i\n", ntotal);
  printf("Total failed : %3i\n", nfail);

  return nfail!=0;
}

