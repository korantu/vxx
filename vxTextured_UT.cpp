/**
* 
* file vxTextured_UT.cpp
*
* This test file is a part of VoxelBrain software.
* 
* (c) Nanyang Technological University
*
*  Author: Konstantin Levinski
*
*/

#include <gtest/gtest.h>

#include "vxTextured.h"
#include "vxAction.h"
#include "vxOpenGlTools.h"
#include "vxDrawSphere.h"
#include "vxDrawSurface.h"
#include "vxLoader.h"
#include "vxSurface.h"
#include "vxMotion.h"
#include "vxRay.h"
#include "vxProjection.h"

/*
TEST(MAIN, Textured){
  //TODO - remove if done in vxDrawSphere_UT.h
  FastVolume vol;
  MgzLoader mri(vol);
  EXPECT_TRUE(mri.Load("data/brainmask.mgz"));
  Textured tex;
  tex.texturing_fastvolume = &vol; 

  struct TexturedSphere: Action {
    FastVolume * vol;
    Textured * tex;
    
    struct MovingAction: Action {
      V3f center;
      MovingAction():center(0,0,0){};
      void Do(){
	Intersection isct = IntersectRaySphere(GetMotion()->sight, V3f(0,0,0), 60);
	if(isct.hit){
	  center = GetMotion()->sight.Travel(isct.distance);
	};
      }
    }pointer;

    void Draw(){
      DrawSphereFunction( pointer.center, 30, 30, tex);
      glColor4f(0.5f,0.5f,0.5f,0.5f);
      glDisable(GL_TEXTURE);
      DrawSphere(V3f(0,0,0), 60, 60).Draw();
    };

    TexturedSphere(FastVolume * _vol, Textured * _tex):vol(_vol), tex(_tex){};

  } scene(&vol, &tex); 


  scene.pointer.bind(GLFW_KEY_RCTRL);
  GetScene()->run( & scene);
};
*/

//TODO - figure out where to drop this shit.


V3f center;
Surface surf;
FastVolume vol;
Textured tex;

struct SurfacePointer: Action {
  
  void Do(){
    Intersection isct = IntersectRaySurface(GetMotion()->sight, &surf);
    if(isct.hit){
      center = GetMotion()->sight.Travel(isct.distance);
      center *= 0.8;
    };
    
  };
} sphere_placer;

struct PushingAction: Action {
  void Do(){
    Intersection isct = IntersectRaySphere(GetMotion()->sight, center, 30);
    if(isct.hit){
      V3f pos = GetMotion()->sight.Travel(isct.distance);
      PushPoint(surf, pos);
      FixNormals(surf);
      AnalyzeSurface(surf, vol);
    };
  }
} pusher;


TEST(MAIN, BrainTextured){
  //TODO - remove if done in vxDrawSphere_UT.h
  MgzLoader mri(vol);
  EXPECT_TRUE(mri.Load("data/brainmask.mgz"));
  EXPECT_TRUE(read_surface_binary(surf, "data/lh.pial"));

  tex.texturing_fastvolume = &vol; 

  AnalyzeSurface(surf, vol);

  struct TexturedSphere: public Action {

    void Draw(){
      DrawSphereFunction( center, 30, 30, &tex);
      glColor4f(0.5f,0.5f,0.5f,0.5f);
      SortSurface(&surf, GetProjection()->Z());
      DrawSurface(surf);
    };

  } scene; 
  
  sphere_placer.bind(GLFW_KEY_RCTRL);
  pusher.bind(GLFW_KEY_F2);
  GetScene()->run( & scene );
};



//End of vxTextured_UT.cpp
