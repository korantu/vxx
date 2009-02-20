/**
*  
*  @file vxAction.cpp
*  
*  This source file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#include "vxAction.h"

using namespace std;

Action * Action::Init(int x, int y){
  motion.x = x; motion.y = y;
  motion.dx = 0; motion.dy = 0;
  Start();
  Do();
  return this;
};

Action * Action::Move(int x, int y){
  motion.dx = x - motion.x; 
  motion.dy = y - motion.y;
  motion.x = x;
  motion.y = y;
  Do();
  return this;
};

Action * Action::Done(int x, int y){
  Move(x, y);
  End();
  return this;
};


// End of vxAction.cpp
