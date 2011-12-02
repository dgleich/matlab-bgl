.PHONY : clean

PLATFORM := $(shell uname | tr A-Z a-z)
ARCH=$(shell getconf LONG_BIT)

-include Makefile-$(PLATFORM)-$(ARCH)-conf.inc

YAMIC_DIR=.
CXXFLAGS=-O2 -I$(YAMIC_DIR) -I$(BOOST_DIR) 


CXXFILES=components.cc max_flow.cc orderings.cc searches.cc shortest_path.cc spanning_trees.cc statistics.cc  planar.cc layouts.cc
OBJFILES=$(patsubst %.cc,%.o,$(CXXFILES))
libmbgl-$(PLATFORM)-$(ARCH).a:$(OBJFILES)
	$(AR) $(ARFLAGS) $@ $^
clean:
	rm *.o *.a
