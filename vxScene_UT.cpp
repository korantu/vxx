/**
* 
* @file vxScene_UT.cpp
*
* This test file is a part of VoxelBrain software.
* 
* (c) Nanyang Technological University
*
*  Author: Konstantin Levinski
*
*/

#include <gtest/gtest.h>

#include "vxScene.h"
#include "vxDrawSphere.h"

TEST(MAIN, Scene){
  
};


//Basic interface test. (Probably redundant.)
TEST(OGL, Navigator){
  struct: public Drawable{
    void Draw(){
      V3f c(V3f(0.0f, 0.0f, 0.0f));
      c /= 2;

      DrawSphere( c, 80.0f, 8).Draw();
    };
  } scene; 
  Scene::run(scene);
};



//End of vxScene_UT.cpp
