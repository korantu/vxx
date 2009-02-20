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
#include "vxAction.h"
#include "vxOpenGlTools.h"

// Visualization code; Should be easily copyable around.
/* Drawable is a thin wrapper around an object to visualize.
 It can assume that 0,0,0 is the center, and everything
should be placed inside a 256x256x256 cube.*/


class Scene {
 public:
  static int bind( int key, Action * );
  static int run(Drawable &);
};
 

#endif // __vxScene_h__

//End of vxScene.h
