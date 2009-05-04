
BOOST_DIR := /home/dgleich/dev/lib/boost_1_36_0
YASMIC_DIR := ../
LIBDIR := ..
LIBNAME := mbgl-linux-64-large

#CXXFLAGS := $(CXXFLAGS) -g -Wall -I../ -I$(BOOST_DIR) -I$(YASMIC_DIR) -DMATLAB_BGL_LARGE_ARRAYS

CXXFLAGS := $(CXXFLAGS) -g -Wall -I../ -I$(BOOST_DIR) -I$(YASMIC_DIR) -L../ -DMATLAB_BGL_LARGE_ARRAYS
LOADLIBES = -l$(LIBNAME)

all : planar_funcs_test \
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

fr_layout_test : $(LIBDIR)/lib$(LIBNAME).a
planar_ksubgraph_test : $(LIBDIR)/lib$(LIBNAME).a
planar_is_straight_line_test : $(LIBDIR)/lib$(LIBNAME).a
planar_funcs_test : $(LIBDIR)/lib$(LIBNAME).a
core_numbers_test : core_numbers_test.cc $(LIBDIR)/yasmic/boost_mod/core_numbers.hpp

test : planar_funcs_test
	./planar_funcs_test
    
clean :
	$(RM) \
	planar_funcs_test planar_is_straight_line_test planar_ksubgraph_test \
	fr_layout_test \
	property_map_test \
	core_numbers_test \
 	simple_prop_map_test_2 simple_prop_map_test_3 simple_prop_map_test_4

