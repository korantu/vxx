/**
*  
*  @file vxFollower.h
*  
*  This header file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#ifndef __vxFollower_h__
#define __vxFollower_h__

#include <stdio.h>

#include "vxVector.h"

struct Followable {
  int hash;
  Followable(){};
  void Invalidate();
};

struct FollowingAction {
  virtual void Do() = 0;
};

void Watch(int * in, FollowingAction *);
void Watch(float * in, FollowingAction *);
void Watch(V3f * in, FollowingAction *);
void Watch(Followable * in, FollowingAction *);

void KickFollowers();
void ResetFollowers(); //As followers do not know when the values tracked become invalid
//it is requered to reset them manualy.

#endif // __vxFollower_h__

//End of vxFollower.h
