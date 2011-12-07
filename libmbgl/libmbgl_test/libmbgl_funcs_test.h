extern const char* errstr;
int planar_funcs_test();
int layout_funcs_test();
int ordering_funcs_test();

#define myassert(condition,message) \
  if (!(condition)) { errstr = message; return -1; }
