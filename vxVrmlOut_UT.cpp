/**
* 
* @file vxVrmlOut_UT.cpp
*
* This test file is a part of VoxelBrain software.
* 
* (c) Nanyang Technological University
*
*  Author: Konstantin Levinski
*
*/

#include <gtest/gtest.h>

#include "vxVrmlOut.h"
#include "vxFileGzipIo.h"
#include "vxLoader.h"

TEST(MAIN, VrmlOut){
  EXPECT_TRUE(VrmlTemplate("v","n","c","i").size()>0);
  vector<V3f> v; //Vertices
  vector<V3i> i; //Indices
  EXPECT_TRUE(VrmlFormat(v).size()==0);
  EXPECT_TRUE(VrmlFormat(i).size()==0);
  v.push_back(V3f(0.5,0.5,0.5));
  v.push_back(V3f(1,1,1));
  v.push_back(V3f(2.0,2.0,2.0));
  i.push_back(V3i(1,2,3));
  i.push_back(V3i(3,2,1));
  EXPECT_TRUE(VrmlFormat(v).size()>0);
  EXPECT_TRUE(VrmlFormat(i).size()>0);
};


TEST(MAIN, Loading){
  Surface surf;
  EXPECT_TRUE(read_surface_binary(surf, "data/lh.pial"));
  FastVolume vol;
  MgzLoader mri(vol);
  EXPECT_TRUE(mri.Load("data/t1.mgz"));

  AnalyzeSurface(surf, vol);

  WriteFile("out.wrl", VrmlTemplate(VrmlFormat(surf.v, false), VrmlFormat(surf.n, true), VrmlFormat(surf.c, false), VrmlFormat(surf.tri)));
  
  
  unsigned char * data = new unsigned char[256*256*256];
  for(int i = 0; i < 256*256*256; i ++){
    data[i] = vol.vol[i];
  };

  std::string data_str((char *)data, 256*256*256);

  WriteFile("data.raw", data_str);
};
//End of vxVrmlOut_UT.cpp
