/**
*  
*  @file vxMotion.cpp
*  
*  This source file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#include "vxMotion.h"
#include "vxMouseRay.h"

Motion * Motion::Init(int _x, int _y){
  x = _x; y = _y;
  dx = 0; dy = 0;
  return this;
};

Motion * Motion::Move(int _x, int _y){
  dx = _x - x; 
  dy = _y - y;
  x = _x; y = _y;
  return this;
};

Motion * Motion::UpdateRay(){
  sight = MouseRay();
  start = sight.O;
  end = sight.O + sight.D;
  return this;
};

Motion the_motion;
Motion * GetMotion(){
  return & the_motion;
};

// End of vxMotion.cpp
