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

#include "vxScene.h"
#include "vxDrawSurface.h"


TEST(MAIN, DrawSurface){
  EXPECT_EQ(2*2, 4);

  Surface surf;
  EXPECT_TRUE(read_surface_binary(surf, "data/lh.pial"));

  surf.c.clear();
  
  for (unsigned int i = 0; i < surf.v.size(); ++i)
  {
    surf.c.push_back(V3f(0.5, 0.5, 0.4));
  }
  
  struct SurfaceThing: public Drawable{
    Surface * surf;
    
    struct AnAction: Action {
    } inflater;
    
    SurfaceThing(Surface * _surf):surf(_surf){};
    void Draw(){
      DrawSurface( *surf );
      
    };
  } scene( & surf); 

  Scene::bind(GLFW_KEY_RCTRL, scene.inflater);
  Scene::run(scene);
};



//End of vxDrawSurface_UT.cpp
