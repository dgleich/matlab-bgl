.PHONY : clean
YAMIC_DIR=.
CXXFLAGS=-O2 -I$(YAMIC_DIR)
ARCH=$(shell getconf LONG_BIT)
CXXFILES=components.cc max_flow.cc orderings.cc searches.cc shortest_path.cc spanning_trees.cc statistics.cc layouts.cc planar.cc
OBJFILES=$(patsubst %.cc,%.o,$(CXXFILES))
libmbgl-linux-$(ARCH).a:$(OBJFILES)
	$(AR) $(ARFLAGS) $@ $^
clean:
	rm *.o *.a
