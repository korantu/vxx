/**
* 
* @file vxRay_UT.cpp
*
* This test file is a part of VoxelBrain software.
* 
* (c) Nanyang Technological University
*
*  Author: Konstantin Levinski
*
*/

#include <gtest/gtest.h>

#include "vxRay.h"
#include "vxSurface.h"

TEST(MAIN, RaySimple){
  Intersection a;
  Intersection b;
  b.distance = 30;
  a = b;
  EXPECT_FALSE(a.hit);
  EXPECT_EQ(a.distance, 30);
};

//Reports first intersection with the ray.
//If hit_point.distance < 0, then the intersection happened behind
//the ray origin, and is probably to be discarded, depending on the 
//requrements.
TEST(MAIN, RaySphere){
  Ray test(V3f(0,0,-3000), V3f(0,0,1));
  Intersection touch_point = IntersectRaySphere(test, V3f(0,0,0), 3);
  EXPECT_TRUE(touch_point.hit);
  printf("distance: %f\n", touch_point.distance);
  say("Touchdown at:", test.Travel(touch_point.distance));
};

TEST(MAIN, IntersectRaySurface){
  Surface surf;
  
  struct: V3fMapper {
    V3f operator() (V3f in) {return in;};
  } plane ;
  
  
  GenerateSurface(surf, plane);
  
  Intersection test;
  Ray dir(V3f(0,0,-3000), V3f(0,0,1));
  Ray wrong(V3f(30,-30,30), V3f(0,1,0));
  EXPECT_TRUE((test = IntersectRaySurface(dir, & surf)).hit); 
  EXPECT_FALSE(IntersectRaySurface(wrong, & surf).hit); 
  EXPECT_EQ(10000, surf.v.size());
  
  printf("distance to the surface: %f\n", test.distance);
};



//End of vxRay_UT.cpp
