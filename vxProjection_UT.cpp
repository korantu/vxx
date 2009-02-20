/**
* 
* @file vxProjection_UT.cpp
*
* This test file is a part of VoxelBrain software.
* 
* (c) Nanyang Technological University
*
*  Author: Konstantin Levinski
*
*/

#include <gtest/gtest.h>

#include "vxProjection.h"

//Simplistic test.
TEST(MAIN, Projection){
  Projection a;
  float PI_VAL = 3.14159;

  EXPECT_EQ( 1, a.zoom);
  EXPECT_EQ( 0, a.zoom_x);
  EXPECT_EQ( 0, a.zoom_y);


  EXPECT_TRUE(a.ex.x == 1.0) << a.ex.x;
  a.Reset();
  EXPECT_EQ( 1.0, a.X().x);
  a.Rotate( PI_VAL, PI_VAL);
  EXPECT_EQ( -1.0, a.X().x);
  
};


//End of vxProjection_UT.cpp
