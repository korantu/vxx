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

struct Action;

class Scene {
 public:
  Scene * run(Action *);
};

Scene * GetScene();

#endif // __vxScene_h__

//End of vxScene.h
