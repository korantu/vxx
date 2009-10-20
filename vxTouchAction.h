/**
*  
*  @file vxTouchAction.h
	\brief A type of action which automatically generates touch position.
	TouchAction traces a ray through the current mouse position and stores the
	intersection for further use.
*  
*  This header file is a part of VoxelBrain software.
* 
*  You provide an intersection callback, and
*  The action convinently calls it for you.
* 
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#ifndef __vxTouchAction_h__
#define __vxTouchAction_h__

#include "vxAction.h"
#include "vxRay.h"

/*!
	\brief Derived from Action to store intersection.
	Derives position through the current mouse location provided by GLFW and
	intersection with the brain. 
	If no intersection was found, the position does not change.
*/
class TouchAction: public Action {
  /*! \brief Holds the last successful intersection. 
  The data is provided by GLFW. If no intersection found, keep the previous value */
  V3f position; 
};

#endif // __vxTouchAction_h__

//End of vxTouchAction.h
