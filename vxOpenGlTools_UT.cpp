/**
* 
* @file vxOpenGlTools_UT.cpp
*
* This test file is a part of VoxelBrain software.
* 
* (c) Nanyang Technological University
*
*  Author: Konstantin Levinski
*
*/

#include <gtest/gtest.h>

#include "vxOpenGlTools.h"
#include "vxDrawSphere.h"

TEST(MAIN, OpenGlTools){
  struct : Drawable {
    void Draw(){
      DrawSphere solid(V3f(0,0,10), 30, 30);
      DrawSphere glass(V3f(0,0,-10), 30, 30);
      solid.Draw();
      Transparent( &glass );
    };    
  } pieces;

  Scene::run(pieces);
};


//End of vxOpenGlTools_UT.cpp
