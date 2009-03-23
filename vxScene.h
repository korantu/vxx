/**
*  
*  @file vxScene.h
*  
*  This header file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#ifndef __vxScene_h__
#define __vxScene_h__

#include "vxVector.h"
#include "vxRay.h"
#include "vxOpenGlTools.h"
#include "vxProjection.h"

// Visualization code; Should be easily copyable around.
/* Drawable is a thin wrapper around an object to visualize.
 It can assume that 0,0,0 is the center, and everything
should be placed inside a 256x256x256 cube.*/

struct Motion;

class Scene {
 public:
  Scene * bind( int key, Action * );
  Scene * run(Drawable &);

  Projection * get_projection(); //Projection
  Motion * get_motion(); //Current motion if needed by an action.
  V3f Z(); //Shortcut to get Z direction 

  Scene();
};

#endif // __vxScene_h__

//End of vxScene.h
