/**
*  
*  @file vxRange.h
*  
*  This header file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#ifndef __vxRange_h__
#define __vxRange_h__

#include "vxVector.h"
#include "vxValidatable.h"

struct Range{
  V3f min;
  V3f max;

  Range(const V3f &, const V3f &);
  Range();
  Range(const Range &);
};

bool ContainsPoint(const Range &, const V3f &);
bool ContainsRange(const Range &, const Range &);
bool IntersectsRange(const Range &, const Range &);
Range & ExpandRange(Range &, const V3f &); 

#endif // __vxRange_h__

//End of vxRange.h
