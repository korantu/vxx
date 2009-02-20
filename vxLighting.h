/**
*  
*  @file vxLighting.h
*  
*  This header file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#ifndef __vxLighting_h__
#define __vxLighting_h__

#include "vxOpenGlTools.h"
#include "vxProjection.h"

struct Lighting: Drawable {
  Projection * scene_projection;
  
  Lighting(Projection *);
  
  void Draw();
};

#endif // __vxLighting_h__

//End of vxLighting.h
