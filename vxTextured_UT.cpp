/**
* 
* @file vxTextured_UT.cpp
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

TEST(MAIN, Textured){
  //TODO - remove if done in vxDrawSphere_UT.h
  FastVolume vol;
  MgzLoader mri(vol);
  EXPECT_TRUE(mri.Load("data/brainmask.mgz"));
  Textured tex;
  tex.texturing_fastvolume = &vol; 

  struct TexturedSphere: public Drawable{
    FastVolume * vol;
    Textured * tex;
    
    struct MovingAction: Action {
      V3f center;
      MovingAction():center(0,0,0){};
      void Do(){
	Intersection isct = IntersectRaySphere(motion.sight, V3f(0,0,0), 60);
	if(isct.hit){
	  center = motion.sight.Travel(isct.distance);
	};
      }
    }pointer;

    void Draw(){
      DrawSphereFunction( pointer.center, 30, 30, tex);
      glColor4f(0.5f,0.5f,0.5f,0.5f);
      glDisable(GL_TEXTURE);
      Transparent(&DrawSphere(V3f(0,0,0), 60, 60));
    };

    TexturedSphere(FastVolume * _vol, Textured * _tex):vol(_vol), tex(_tex){};

  } scene(&vol, &tex); 


  Scene::bind(GLFW_KEY_RCTRL, &scene.pointer);
  Scene::run(scene);
};

//TODO - figure out where to drop this shit.

struct SurfacePointer: Action {
  void Do(){
    Scene::surface
    Intersection isct = IntersectRaySurface(motion.sight, &surf);
    if(isct.hit){
      center = motion.sight.Travel(isct.distance);
      center *= 0.8;
    };
  }

private:
  Surface * surf;
};


TEST(MAIN, BrainTextured){
  //TODO - remove if done in vxDrawSphere_UT.h
  MgzLoader mri(vol);
  EXPECT_TRUE(mri.Load("data/brainmask.mgz"));
  EXPECT_TRUE(read_surface_binary(surf, "data/lh.pial"));

  tex.texturing_fastvolume = &vol; 

  AnalyzeSurface(surf, vol);

  struct TexturedSphere: public Drawable{

    struct PushingAction: Action {
      void Do(){
        Intersection isct = IntersectRaySphere(motion.sight, center, 30);
        if(isct.hit){
	  pos = motion.sight.Travel(isct.distance);
	  PushPoint(surf, pos);
	  FixNormals(surf);
	  AnalyzeSurface(surf, vol);
	};
      }
    }pusher;

    struct Surfacer :Action {
      bool is_transparent;
      void Start(){is_transparent=!is_transparent;}
      Surfacer():is_transparent(false){};
    } surfacer;

    void Draw(){
      DrawSphereFunction( center, 30, 30, &tex);
      glColor4f(0.5f,0.5f,0.5f,0.5f);
      Transparent(surfacer.is_transparent);
      SortSurface(&surf, Scene::getProjection()->Z());
      DrawSurface(surf);
      Transparent(false);
    };

  } scene; 

  Scene::bind(GLFW_KEY_F2, &scene.surfacer);
  Scene::bind(GLFW_KEY_F3, &scene.pusher);
  Scene::bind(GLFW_KEY_RCTRL, &scene.pointer);
  Scene::run(scene);
};



//End of vxTextured_UT.cpp
