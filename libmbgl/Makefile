.PHONY : clean

include Makefile.conf

DEFINES ?= -DMATLAB_BGL_LARGE_ARRAYS
CXXFLAGS += -O2 -I$(YAMIC_DIR) -I$(BOOST_DIR) $(DEFINES)

CXXFILES=components.cc max_flow.cc orderings.cc searches.cc shortest_path.cc spanning_trees.cc statistics.cc  planar.cc layouts.cc
OBJFILES=$(patsubst %.cc,%.o,$(CXXFILES))
$(LIBNAME).a:$(OBJFILES)
	$(AR) $(ARFLAGS) $@ $^
clean:
	rm *.o *.a
