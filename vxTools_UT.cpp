/**
* 
* @file vxTools_UT.cpp
*
* This test file is a part of VoxelBrain software.
* 
* (c) Nanyang Technological University
*
*  Author: Konstantin Levinski
*
*/

#include <gtest/gtest.h>

#include "vxTools.h"

TEST(MAIN, Tools){
  V3f a(1,2,3);
  std::cout << a;
};

TEST(Assertion, Tools){
  ASSERT(1==1, "Hell forsen!");
  ASSERT(2==1, "As normal.");
};

//End of vxTools_UT.cpp
