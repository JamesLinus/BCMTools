##############################
#
# BCMTools
#
# Copyright (C) 2011-2014 Institute of Industrial Science, The University of Tokyo.
# All rights reserved.
#
# Copyright (c) 2012-2015 Advanced Institute for Computational Science, RIKEN.
# All rights reserved.
#
##############################

PROGRAM = a.out

BCMLIB = ../../lib/libbcm.a
UTIL = ../../Utils

vpath %.cpp $(UTIL)/src

OBJS = main.o Solver.o Timing.o
UTIL_OBJS = ConfigFile.o ConfigBase.o BCMOctree.o

include ../../Makefile.inc

CFLAGS += -I../../include -I$(UTIL)/include $(OPTIONS)

LDFLAGS = 
LIBS = $(BCMLIB) -lmpi -lmpi_cxx

PROGRAM: $(OBJS) $(UTIL_OBJS)
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $(PROGRAM) $(OBJS) $(UTIL_OBJS) $(LIBS)

.cpp.o:
	$(CXX) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf *.o $(PROGRAM) depend.inc

depend: $(OBJS:.o=.cpp) $(UTIL_OBJS:.o=.cpp)
	@ rm -rf depend.inc
	@ for i in $^; do\
		$(CXX) $(CFLAGS) -MM $$i >> depend.inc;\
	done

-include depend.inc
