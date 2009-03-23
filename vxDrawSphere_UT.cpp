/**
* 
* @file vxDrawSphere_UT.cpp
*
* This test file is a part of VoxelBrain software.
* 
* (c) Nanyang Technological University
*
*  Author: Konstantin Levinski
*
*/

#include <gtest/gtest.h>

#include "vxDrawSphere.h"
#include "vxScene.h"

TEST(MAIN, DrawSphere){
  DrawSphere a(V3f(0,0,0), 10, 8);
  Scene::run(a);
};


//End of vxDrawSphere_UT.cpp
