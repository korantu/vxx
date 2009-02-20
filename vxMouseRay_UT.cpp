/**
* 
* @file vxMouseRay_UT.cpp
*
* This test file is a part of VoxelBrain software.
* 
* (c) Nanyang Technological University
*
*  Author: Konstantin Levinski
*
*/

#include <gtest/gtest.h>

#include "vxMouseRay.h"
#include "vxDrawSphere.h"
#include "vxScene.h"

TEST(MAIN, MouseRay){

  struct SphereThing: public Drawable{
    V3f center;
 
    struct InflateAction: Action {
      int radius;
      InflateAction(): radius(10){};
      void Do(){ radius += (motion.dx+motion.dy); if(radius < 10)radius=10;};
    } inflater;
    
    SphereThing():center(0,0,0){};
    void Draw(){
      DrawSphere( center, inflater.radius, 8).Draw();
    };
  } scene; 

  Scene::bind(GLFW_KEY_LSHIFT, & scene.inflater);
  Scene::run(scene);
  
};


//End of vxMouseRay_UT.cpp
