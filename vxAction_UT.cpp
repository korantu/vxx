/**
* 
* @file vxAction_UT.cpp
*
* This test file is a part of VoxelBrain software.
* 
* (c) Nanyang Technological University
*
*  Author: Konstantin Levinski
*
*/

#include <gtest/gtest.h>

#include "vxAction.h"
#include "vxScene.h"
#include "vxDrawSphere.h"


TEST(OGL, Navigator){
  struct: Drawable{
    struct : Action {} mover;
    
    void Draw(){
      V3f c = (mover.motion.start+mover.motion.end)/2;     
      DrawSphere( c, 8.0f, 8).Draw();
    };
  } scene; 
  
  Scene::bind(GLFW_KEY_RCTRL, &scene.mover);
  Scene::run(scene);
};



//End of vxAction_UT.cpp
