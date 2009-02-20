/**
* 
* @file vxVector_UT.cpp
*
* This test file is a part of VoxelBrain software.
* 
* (c) Nanyang Technological University
*
*  Author: Konstantin Levinski
*
*/

#include <gtest/gtest.h>

#include "vxVector.h"

TEST(MAIN, Vector){
  
};

TEST(MAIN, BasicOperators){
  float v_array[] = {1.0f, 4.0f, 3.0f };
  V3f a(1.0f, 2.0f, 3.3f);
  V3f x(1.0f, 1.0f, 3.3f);
  V3f y(1.0f, 2.0f, 3.0f);
  
  V3f v(v_array);

  V3f b(a);
  V3f d; d = a;

  EXPECT_FLOAT_EQ(a[0], 1.0f);
  EXPECT_FLOAT_EQ(a[1], 2.0f);
  EXPECT_FLOAT_EQ(a[2], 3.3f);

  for (int i = 0; i < 3; ++i)
  {
    EXPECT_FLOAT_EQ(v[i], v_array[i]);
  }

  EXPECT_FLOAT_EQ(1.0f, a.distance2(x));
  EXPECT_FLOAT_EQ(0.0f, a.distance2(b));
  EXPECT_TRUE(a==b);
  EXPECT_FALSE(a==x);
  EXPECT_TRUE(a!=x);
};

TEST(MAIN, Cross){
  V3f a(1.0f, 0.0f, 0.0f);
  V3f b(0.0f, 1.0f, 0.0f);
  V3f c(0.0f, 0.0f, 1.0f);
  EXPECT_TRUE(a.cross(b) == c);
  EXPECT_TRUE(b.cross(c) == a);
  EXPECT_TRUE(c.cross(a) == b);
};

TEST(MAIN, Rotation){
  V3f a(1.0f, 1.0f, 1.0f);
  /*  
      say("X", rot_x(a, -PI/2.0f));
      say("Y", rot_y(a, -PI/2.0f));
      say("Z", rot_z(a, -PI/2.0f));
  */
  EXPECT_TRUE(V3f(1.0f, -1.0f, 1.0f) == rot_x(a, PI/2.0f));
  EXPECT_TRUE(V3f(1.0f, 1.0f, -1.0f) == rot_y(a, PI/2.0f));
  EXPECT_TRUE(V3f(-1.0f, 1.0f, 1.0f) == rot_z(a, PI/2.0f));
};

TEST(MAIN, ortoNormalize){
  V3f a(1.0f, 2.0f, 3.3f);
  V3f b(1.0f, 1.0f, 3.3f);
  V3f c(1.0f, 2.0f, 3.0f);
  ortoNormalize(a,b,c);
  EXPECT_TRUE(c == a.cross(b));
  EXPECT_TRUE(a == b.cross(c));
  EXPECT_TRUE(b == c.cross(a));
};


//End of vxVector_UT.cpp
