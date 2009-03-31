/**
*  
*  @file vxSurfaceSlicer.h
*  
*  This header file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#ifndef __vxSurfaceSlicer_h__
#define __vxSurfaceSlicer_h__

#include <vector>

#include "vxVector.h"

/**
  A function to derive a set of pairs from intersections with surface.
 */ 

typedef std::vector<V3f> BorderLine;
struct Surface;

void GetBorderLine(Surface * in, V3f center, float radius, BorderLine *);
void DrawBorder(BorderLine * bdr);


#endif // __vxSurfaceSlicer_h__

//End of vxSurfaceSlicer.h
