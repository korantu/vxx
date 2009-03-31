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

#include "vxVector.h"

/**
  Follower has a virtual method Do(), which is executed when a value has changed.
  Follow(int/V3f,float)// adds a reference to the object being followed.
  
 */ 

struct Follower {
  virtual void Do() = 0;

  void Follow(float *);
  void Follow(V3f *);
  void Follow(int *);

  bool to_kick;
};

void KickFollowers();
void ResetFollowers(); //As followers do not know when the values tracked become invalid
//it is requered to reset them manualy.

#endif // __vxFollower_h__

//End of vxFollower.h
