/**
* 
* @file vxSurface_UT.cpp
*
* This test file is a part of VoxelBrain software.
* 
* (c) Nanyang Technological University
*
*  Author: Konstantin Levinski
*
*/

#include <gtest/gtest.h>

#include "vxSurface.h"

TEST(MAIN, Surface){
  
};

public:
  Surface surf;
  Surface a;
  Surface b;
};

TEST_F(SurfaceTest, Header) {
  int triangles_number, vertices_number;
  Io data(ReadFile("rh.pial"));
  EXPECT_TRUE(ReadPialHeader(data, &vertices_number, &triangles_number));
  printf("%d:%d vertices:triangles numbers\n", 
	 vertices_number,
	 triangles_number);
};

/// 1 BSQ
TEST_F(SurfaceTest, IO) {
  EXPECT_EQ(surf.v.size()+surf.n.size()+surf.c.size()+surf.tri.size()+surf.seeds.size(), 0);
  EXPECT_TRUE(read_surface_binary(surf, "rh.pial"));
  EXPECT_TRUE(read_surface_binary(a, "rh.pial"));
  EXPECT_TRUE(read_surface_binary(b, "lh.pial"));
  EXPECT_TRUE(surf.v.size() > 0);
  EXPECT_TRUE(b.v.size() > 0);
  EXPECT_EQ(b.v.size(), b.n.size());
  EXPECT_EQ(surf.v.size(), surf.n.size());
  EXPECT_EQ(surf.seeds.size(), 1);
  //Ensure all normals are of unit length.
  int cnt = 0;
  for(vector<V3f>::iterator i = surf.n.begin(); i != surf.n.end(); i++){
    ASSERT_FLOAT_EQ(i->length(), 1.0) << "Vertex number " << cnt << "\n";
    cnt++;
  };

  //Load next chunk.
  EXPECT_TRUE(read_surface_binary(surf, "lh.pial"));
  EXPECT_EQ(surf.seeds.size(), 2); //Now 2 seeds are avaliable. Checking them is too involved.
  EXPECT_EQ(surf.v.size(), a.v.size() + b.v.size());
  EXPECT_EQ(surf.n.size(), a.n.size() + b.n.size());
  //Ensure all normals are still of unit length.

  cnt = 0;

  for(vector<V3f>::iterator i = surf.n.begin(); i != surf.n.end(); i++){
    EXPECT_FLOAT_EQ(i->length(), 1.0) << "Vertex number " << cnt << "\n";
    cnt++;
  };



  clear(surf);
  EXPECT_EQ(surf.v.size()+surf.n.size()+surf.c.size()+surf.tri.size()+surf.seeds.size(), 0);
};

TEST(Connectivity, Link){
  Connectivity net;
  BiLink(net, 1, 2);
  BiLink(net, 1, 3);
  ASSERT_EQ(1, net[2].size());
  ASSERT_EQ(2, net[1].size());
  ASSERT_EQ(1, net[3].size());
};

TEST(MAIN, Connectivity){
  Connectivity net;
  Connectivity net_wide;
  VerticeSet result; result.insert(0);
  VerticeSet result_wide; result_wide.insert(0);

  //Linking up a couple of degenerative networks.

  for(int i = 0; i < 10; i++){
    BiLink(net, i, i+1);
    BiLink(net_wide, 0, i);
  };

  Propagate(net, result, 1);
  Propagate(net_wide, result_wide, 1);
  EXPECT_EQ(2, result.size());
  EXPECT_EQ(10, result_wide.size());
  Propagate(net, result, 3);
  EXPECT_EQ(5, result.size());  
};

TEST(MAIN, RealMesh){
  Surface surf;
  Connectivity net;
  EXPECT_TRUE(read_surface_binary(surf, "lh.pial"));
  EXPECT_EQ(152897, surf.v.size());
  for(int i = 0; i < surf.tri.size(); i++){
    Link(net, surf.tri[i].x, surf.tri[i].y);
    Link(net, surf.tri[i].y, surf.tri[i].z);
    Link(net, surf.tri[i].z, surf.tri[i].x);
  };

  VerticeSet test; test.insert(3); //Let's say; start with the third point;
  
  Propagate(net, test, 1); //Do one step; 
  EXPECT_EQ(4, test.size());

  Propagate(net, test, 10000); //Do one step; 
  EXPECT_EQ(4, test.size());
};



//End of vxSurface_UT.cpp
