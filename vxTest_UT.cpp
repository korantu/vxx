/**
* 
* file vxTest_UT.cpp
*
* This test file is a part of VoxelBrain software.
* 
* (c) Nanyang Technological University
*
*  Author: Konstantin Levinski
*
*/

#include <gtest/gtest.h>

struct A{
  int b;
  A(int _b):b(_b){};
};

struct B: A{
  int blah;
  B(int _b):blah(4), A(_b){};
};

TEST(MAIN, TryInheritance){
  B b(3);
  EXPECT_EQ(b.b, 3);
  EXPECT_EQ(b.blah, 4 );
};

//End of vxTest_UT.cpp
