/**
* 
* @file vxFastVolume_UT.cpp
*
* This test file is a part of VoxelBrain software.
* 
* (c) Nanyang Technological University
*
*  Author: Konstantin Levinski
*
*/

#include <gtest/gtest.h>

#include "vxFastVolume.h"
#include "vxLoader.h"

TEST(MAIN, FastVolume){
  FastVolume vol;
  
  MgzLoader mri(vol);

  EXPECT_TRUE(mri.Load("data/brainmask.mgz"));
};

TEST( Simple, FastVolume ){
  float * in = {0,0,0};
  float * start = {0,0,0};
  EXPECT_NE(in[0], start[0]);
};

//End of vxFastVolume_UT.cpp
