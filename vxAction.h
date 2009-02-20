/**
*  
*  @file vxAction.h
*  
*  This header file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#ifndef __vxAction_h__
#define __vxAction_h__

//Each key triggers Start() when pressed,
//then, with each mouse move Move(),
//finally, End() is called when released. 
struct Action {
  virtual void Start(){};
  virtual void Do(){};
  virtual void End(){};

  struct Motion {
    int x, y, dx, dy;
  } motion;

  Action * Init(int x, int y);
  Action * Move(int x, int y);
  Action * Done(int x, int y);
  
};

#endif // __vxAction_h__

//End of vxAction.h
