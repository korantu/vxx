/**
* 
* file vxScene_UT.cpp
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
#include "vxAction.h"
#include "vxDrawSphere.h"


struct TestSimpleScene: public Action{
  virtual void Draw(){
    DrawSphere( V3f(0.0f, 0.0f, 0.0f) , 80.0f, 8).Draw();
  };
}; 


TEST(Interactive, Navigator){
  TestSimpleScene a;
  GetScene()->run((Action *)&a);
};



//End of vxScene_UT.cpp
