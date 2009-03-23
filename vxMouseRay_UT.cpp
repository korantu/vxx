/**
* 
* @file vxMouseRay_UT.cpp
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
#include "vxMouseRay.h"
#include "vxDrawSphere.h"
#include "vxOpenGlTools.h"

struct InflateAction: Action {
  int radius;
  InflateAction(): radius(10){};
  void Do(){ radius += (motion.dx+motion.dy); radius %=100; if(radius < 10) radius=10;};
};

/*
  MouseRay() is used as:
 */
struct PointerAction: Action {
  PointerAction(InflateAction & in){p_inflater = & in;};
  InflateAction * p_inflater;
  V3f cursor;
  void Do()
  { 
    Intersection hit_position;
    IntersectRaySphere(MouseRay(), V3f(0,0,0), p_inflater->radius, hit_position);
    if(hit_position.hit) cursor = MouseRay().Travel(hit_position.distance);
  };
};

TEST(MAIN, MouseRay){

  struct SphereThing: public Drawable{
    InflateAction inflater;
    PointerAction pointer;

    SphereThing():pointer(inflater){};
    void Draw(){
      DrawSphere( V3f( 0, 0, 0), inflater.radius, 8).Draw();
      DrawSphere( pointer.cursor, 10, 8).Draw();
    };
  } scene; 

  Scene::bind(GLFW_KEY_RSHIFT, & scene.inflater);
  Scene::bind(GLFW_KEY_RCTRL, & scene.pointer);
  Scene::run(scene);
};


//End of vxMouseRay_UT.cpp
