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


//End of vxFastVolume_UT.cpp
