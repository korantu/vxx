/**
* 
* @file vxTest_UT.cpp
*
* This test file is a part of VoxelBrain software.
* 
* (c) Nanyang Technological University
*
*  Author: Konstantin Levinski
*
*/

#include <gtest/gtest.h>

//#include "vxTest.h"

struct A{
  int b;
  A(int _b):b(_b){};
};

struct B: A{
  int blah;
  B():blah(4){};
};

TEST(MAIN, TryInheritance){
  B b(3);
  EXPECT_EQUAL(b.b, 3);
  EXPECT_EQUAL(b.blah, 4 );
};

//End of vxTest_UT.cpp
