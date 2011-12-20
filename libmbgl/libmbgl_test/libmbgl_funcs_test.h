extern const char* errstr;
int planar_funcs_test();
int layout_funcs_test();
int ordering_funcs_test();
int structure_funcs_test();
int statistics_funcs_test();

#define myassert(condition,message) \
  if (!(condition)) { errstr = message; return -1; }

#define testcase(testname,func) \
  {\
    const char* name = (testname); \
    rval= (func)(); ntotal++; \
    if (rval!= 0) { nfail++; printf("%20s  %50s\n", name, errstr); } \
    else { printf("%20s  success\n", name); } \
  }
