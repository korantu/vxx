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
#include "vxTools.h"

#include <iostream>

TEST(MAIN, FastVolume){
  FastVolume vol;
  
  MgzLoader mri(vol);

  EXPECT_TRUE(mri.Load("data/brainmask.mgz"));
};

TEST( Simple, FastVolume ){
  V3f in(0,0,0);
  V3f out(0,0,0);
  out = FastVolume::FromSurface(in);
  EXPECT_NE(in, out);
  std::cout << out;
};

/*
 So, the idea is as follows - 
 Load another volume data, actual mask.
 Allow the user to review and change it.
 */


TEST( RasterizeTriangle, FastVolume ) {
  FastVolume m; //A set of bitmasks; eight; 
  V3f a (10,10,10);
  V3f b (10,100,110);
  V3f c (10,100,10);
  V3f o = (a + b + c) / 3; //Center.

  EXPECT_FALSE(m.GetMask(o,1)); //Center, unmasked.
  EXPECT_FALSE(m.GetMask(a,1)); //Basic mask function.
  m.SetMask(a,1,true);
  EXPECT_TRUE(m.GetMask(a,1)); //Trivial plane check.
  m.SetMask(a,2,false);
  EXPECT_TRUE(m.GetMask(a,1));
  m.SetMask(a,1,false);
  EXPECT_FALSE(m.GetMask(a,1));

  m.RasterizeTriangle(a,b,c,1); //Use mask plane 1. 
  EXPECT_TRUE(m.GetMask(a,1));
  EXPECT_TRUE(m.GetMask(b,1));
  EXPECT_TRUE(m.GetMask(c,1));
  EXPECT_TRUE(m.GetMask(o,1));
};


TEST( RasterizeTriangleEcplore, FastVolume ) {
  FastVolume m; //A set of bitmasks; eight; 
  V3f a (10,10,10);
  V3f b (10,12,10);
  V3f c (10,10,12);
  m.RasterizeTriangle(a,b,c,1);
  EXPECT_TRUE(m.GetMask(a,1));
  std::cout << "VS " << b << c << "\n";
  m.SetMask(b,1, true);
  EXPECT_TRUE(m.GetMask(b,1));
  EXPECT_TRUE(m.GetMask(c,1));
}
//End of vxFastVolume_UT.cpp
