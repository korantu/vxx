/**
* 
* @file vxDrawSurface_UT.cpp
*
* This test file is a part of VoxelBrain software.
* 
* (c) Nanyang Technological University
*
*  Author: Konstantin Levinski
*
*/

#include <gtest/gtest.h>

#include "vxDrawSurface.h"

TEST(MAIN, DrawSurface){
  EXPECT_EQ(2*2, 4);

  struct SurfaceThing: public Drawable{
    Surface * surf;
    
    struct AnAction: Action {
    } inflater;
    
    SurfaceThing(Surface * _surf):surf(_surf){};
    void Draw(){
      DrawSurface( *surf );
    };
  } scene; 

  EXPECT_TRUE(read_surface_binary(surf, "rh.pial"));

  Scene::run(scene);
};


//End of vxDrawSurface_UT.cpp
