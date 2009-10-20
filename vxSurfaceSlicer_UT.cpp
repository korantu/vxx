/**
* 
* file vxSurfaceSlicer_UT.cpp
*
* This test file is a part of VoxelBrain software.
* 
* (c) Nanyang Technological University
*
*  Author: Konstantin Levinski
*
*/

#include <gtest/gtest.h>

#include "vxSurfaceSlicer.h"
#include "vxSurface.h"
#include "vxScene.h"
#include "vxAction.h"
#include "vxDrawSphere.h"

using namespace std;

TEST(MAIN, SurfaceSlicer){

  Surface _surf;
  BorderLine _bdr;
  V3f _center(6,2,10);
  float _radius = 20;

  struct: V3fMapper {
    V3f operator() (V3f in) {return in;};
  } _plane ;
  
  
  GenerateSurface(_surf, _plane);


  //EXPECT_TRUE(read_surface_binary(surf, "data/lh.pial"));
  GetBorderLine(&_surf, _center, _radius, &_bdr);
  EXPECT_LT((size_t)1000, _surf.v.size());
  EXPECT_NE(0, _surf.v[5].x+_surf.v[5].y+_surf.v[5].z); //something not equal to zero
  EXPECT_LT((size_t)0, _bdr.size()); // shoud find some border.
  EXPECT_EQ(_bdr.size() % 2, (size_t)0); // bdr is pairs;

  //All within same distance from center
  for(BorderLine::iterator i = _bdr.begin(); i != _bdr.end(); i++){
    EXPECT_NEAR((*i - _center).length(), _radius, 0.1);
  };
};


//Drawing border
struct TestLineScene: public Action{
  BorderLine * bdr;
  V3f _center;
  float _radius;

  virtual void Draw(){
    DrawSphere( _center , _radius, 30).Draw();
    DrawBorder(bdr);
  };
  
  TestLineScene(BorderLine * _bdr, V3f center, float radius):bdr(_bdr), _center(center), _radius(radius){};
}; 


TEST(Interactive, Border){
  Surface _surf;
  BorderLine _bdr;
  V3f _center(6,2,10);
  float _radius = 20;

  struct: V3fMapper {
    V3f operator() (V3f in) {return V3f(in.x, in.y, 20*sin((in.x+in.y)/20.0));};
  } _plane ;
  
  
  GenerateSurface(_surf, _plane);


  //EXPECT_TRUE(read_surface_binary(surf, "data/lh.pial"));
  GetBorderLine(&_surf, _center, _radius, &_bdr);


  TestLineScene a(&_bdr, _center, _radius);

  GetScene()->run(&a);

};

//End of vxSurfaceSlicer_UT.cpp
