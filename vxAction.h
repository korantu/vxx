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

#include "vxVector.h"
#include "vxRay.h"

//Each key triggers Start() when pressed,
//then, with each mouse move Do(),
//finally, End() is called when released. 

struct Scene;
//* Interface for an action plugged into the scene itself;
struct Action {
  Scene * scene;
  virtual void Start(){};
  virtual void Do(){};
  virtual void End(){};

  Action * operator() (Scene *);
};

#endif // __vxAction_h__

//End of vxAction.h
