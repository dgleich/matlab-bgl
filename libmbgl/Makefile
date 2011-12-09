.PHONY : clean win32 win64 linux32

include Makefile.conf

DEFINES ?= -DMATLAB_BGL_LARGE_ARRAYS
CXXFLAGS += -O2 -I$(YASMIC_DIR) -I$(BOOST_DIR) $(DEFINES) -fPIC 

$(LIBNAME).a:$(OBJFILES)
	$(AR) $(ARFLAGS) $@ $^
	
alllibs: win32	

win32 : 
	make -f Makefile.win32 clean
	make -f Makefile.win32 all

clean:
	rm *.o *.a
	
	
