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
  //ASSERT(2==1, "As normal.");
};


TEST(Time, Tools){
  int cnt = 1;
  TIME(
  for(int i = 0 ; i < 1000000; i++)
    for(int j = 0; j < 1100; j++){
      cnt += i+j;
      if(i == 100 && j == 100)std::cout << "side effect: " << cnt << "\n";
    };
  , "Useless Loop");
};

//End of vxTools_UT.cpp
