/*
###################################################################################
#
# BCMTools
#
# Copyright (c) 2011-2014 Institute of Industrial Science, The University of Tokyo.
# All rights reserved.
#
# Copyright (c) 2012-2016 Advanced Institute for Computational Science (AICS), RIKEN.
# All rights reserved.
#
# Copyright (c) 2017 Research Institute for Information Technology (RIIT), Kyushu University.
# All rights reserved.
#
###################################################################################
*/

#include "Cutlib.h"
using namespace cutlib;

#include <string>

void outputVtk(const std::string& file, const int myrank, const GridAccessor* grid,
               const CutPosArray* cp, const CutBidArray* cb);


#ifdef CUTLIB_OCTREE
void outputVtkLeafCell(const std::string& file,
                       SklTree* tree,
                       CutPosOctree* cp, CutBidOctree* cb);

void outputVtkAllCell(const std::string& file,
                       SklTree* tree, const int treeLevel,
                       CutPosOctree* cp, CutBidOctree* cb);
#endif
