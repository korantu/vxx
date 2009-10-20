/**
*  
*  @file vxAction.h
	\brief General user event handler.

The basis for event handling. Derived structures do the actual work. The mapping between key codes and actions is maintained here and can be accessed through bind() and 
GetAction() functions.
	
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

/*!
	\brief Interface for an action implementation.
	Most actions do not need all the notifications. 
	In most cases, only one or two are used. It would be concinent to define this struct anonymously, but, unfortunately, due to MSVC limitations it is impossible there. 
	Works in Cygwin, though.
 */
struct Action {

  /*! \brief Executed once when a key is pressed.
	The key presses are determined by GLFW, and it does not use automatic repeat. So only physical presses are counted. */
  virtual void Start(){}; 
  
   /*! \brief Executed for every mouse movement with that key pressed. 
	Do not use this function for anything which cannot finish in 1/25 of a second. Store the data instead and use End() function to do the actual modification. */
  virtual void Do(){};

   /*! \brief Executed on key release.
   Put all the heavy processing here. */
  virtual void End(){}; 

   /*! \brief Called when the relevant geometry is to be drawn.
   Often modification is applied to a certain geometry. the Draw() function allows to place the modification and drawing close together. */
  virtual void Draw(){}; 

  /*! \brief Attach the action to a key.
  GLFW key codes are used. Returns *this. */
  Action * bind(int key); 
};

/*!
	\brief A function to retrieve an action object bound by the key code.
	Returns NULL if the keycode is not found.
	Otherwise returns the object so that an action could be triggered.
*/
Action * GetAction(int key);

#endif // __vxAction_h__

//End of vxAction.h
