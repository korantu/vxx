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

/*
 */
struct Action {

  virtual void Start(){};
  virtual void Do(){};
  virtual void End(){};

  virtual void Draw(){};

  Action * bind(int key);
};

Action * GetAction(int key);

#endif // __vxAction_h__

//End of vxAction.h
