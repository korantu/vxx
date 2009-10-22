/**
*  
*  @file vxFollower.h
\brief Mechanizm for performing actions on a value change.
When a state changes, for example point of view, center, radius, some calculations may need to be performed. The calculations are usually unrelated to the code that changed the value. So the Followers subclasses can make sure that the rest of the state is consistent.
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

/*! \brief Derive from this structure to make a complex object followable by a FollowingAction.
Atom values like int/float can be followed directly. If a class has to be followed, it should present some form of change notification. Followable stores a hash to check if it has changed. */
struct Followable {
  int hash; //!< \brief Hash value to detect changes. Modified by Invalidate(). 
  
  //!< \brief When a code changes a Followable object, it calls Invalidate. Invalidate makes sure that the hash values do not match anymore. 
  void Invalidate(); 
};

/*! \brief Base for an action to perform when a value changes.
The actions are triggered explicitly by KickFollowers() function. State needed to perform the update can be stored within the derived structure. */
struct FollowingAction {
  /*! \brief Called when the value being followed has changed.
  Override this function with the update code. */
  virtual void Do() = 0;
};

void Watch(int * in, FollowingAction *); //!< \brief Set to watch in. The FollowingAction is not copied, so it has to have sufficient scope.  
void Watch(float * in, FollowingAction *); //!< \brief Set to watch in. The FollowingAction is not copied, so it has to have sufficient scope.  
void Watch(V3f * in, FollowingAction *); //!< \brief Set to watch in. The FollowingAction is not copied, so it has to have sufficient scope.  
void Watch(Followable * in, FollowingAction *); //!< \brief Set to watch in. The FollowingAction is not copied, so it has to have sufficient scope.  

/*! \brief Trigger FollowingAction for values that changed.
It is usually performed in the screen update cycle, before the scene has to be displayed. */
void KickFollowers();
void ResetFollowers(); //!< \brief Has to be called before using Followers. 

#endif // __vxFollower_h__

//End of vxFollower.h
