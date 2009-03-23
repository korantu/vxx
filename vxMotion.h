/**
*  
*  @file vxMotion.h
*  
*  This header file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#ifndef __vxMotion_h__
#define __vxMotion_h__

#include "vxRay.h"

/**
 */ 
struct Motion {
  int x, y, dx, dy; // Screen location.
  V3f start; // Start and 
  V3f end;   // End of the mouse ray.
  Ray sight; // Line of sight.
  
  Motion * Init(int x, int y);
  Motion * Move(int x, int y);
  Motion * UpdateRay();
};

Motion * GetMotion();

#endif // __vxMotion_h__

//End of vxMotion.h
