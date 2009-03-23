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

void Motion::Init(int x, int y){
  motion.x = x; motion.y = y;
  motion.dx = 0; motion.dy = 0;
};

void Motion::Move(int x, int y){
  motion.dx = x - motion.x; 
  motion.dy = y - motion.y;
  motion.x = x;
  motion.y = y;
};

void Motion::UpdateRay(){
  motion.sight = MouseRay();
  motion.start = motion.sight.O;
  motion.end = motion.sight.O+motion.sight.D;
};

// End of vxMotion.cpp
