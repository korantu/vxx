/**
* 
* @file vxFollower_UT.cpp
*
* This test file is a part of VoxelBrain software.
* 
* (c) Nanyang Technological University
*
*  Author: Konstantin Levinski
*
*/

#include <gtest/gtest.h>

#include "vxFollower.h"

TEST(MAIN, FollowerActions){

  //Telling what is possible to do
  
  struct : FollowingAction {
    int i;
    void Do(){i++;};
  } flw;

  flw.i = 0;
  int a = 3;
  float b = 3.0;
  V3f c( 1.0, 1.1, 1.2);

  ResetFollowers();
  Watch(&a, &flw); Watch(&b, &flw); Watch(&c, &flw);

  EXPECT_EQ(flw.i, 0);

  KickFollowers(); 

  EXPECT_EQ(flw.i, 3);

  KickFollowers(); 
  KickFollowers(); 
  KickFollowers(); 

  EXPECT_EQ(flw.i, 3);

};
//End of vxFollower_UT.cpp
