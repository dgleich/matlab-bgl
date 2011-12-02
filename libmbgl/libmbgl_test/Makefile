
include ../Makefile.conf

MBGLLIB = $(LIBDIR)/$(LIBNAME).a

DEFINES ?= -DMATLAB_BGL_LARGE_ARRAYS

#CXXFLAGS := $(CXXFLAGS) -g -Wall -I../ -I$(BOOST_DIR) -I$(YASMIC_DIR) -DMATLAB_BGL_LARGE_ARRAYS

CXXFLAGS += -g -Wall -I../ -I$(BOOST_DIR) -I$(YASMIC_DIR)  $(DEFINES)
LOADLIBES = $(MBGLLIB)

all : libmbgl_funcs_test \
      test

extra : simple_prop_map_test_4 \
        simple_prop_map_test_3 \
        simple_prop_map_test_2 \
        property_map_test \
        fr_layout_test \
        planar_ksubgraph_test \
        planar_is_straight_line_test \
        core_numbers_test
      
.PHONY : test clean extra

$(MBGLLIB) : 

fr_layout_test : $(MBGLLIB)
planar_ksubgraph_test : $(MBGLLIB)
planar_is_straight_line_test : $(MBGLLIB)
planar_funcs_test : $(MBGLLIB)
core_numbers_test : core_numbers_test.cc $(LIBDIR)/yasmic/boost_mod/core_numbers.hpp

libmbgl_funcs_test : $(MBGLLIB) layout_funcs_test.o planar_funcs_test.o

test : libmbgl_funcs_test
	./libmbgl_funcs_test
	
clean :
	$(RM) \
        libmbgl_funcs_test planar_funcs_test.o layout_funcs_test.o \
	planar_funcs_test planar_is_straight_line_test planar_ksubgraph_test \
	fr_layout_test \
	property_map_test \
	core_numbers_test \
 	simple_prop_map_test_2 simple_prop_map_test_3 simple_prop_map_test_4

