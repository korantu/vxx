/**
*  
*  @file vxTouchAction.h
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

class TouchAction: public Action {
  V3f position;
};

#endif // __vxTouchAction_h__

//End of vxTouchAction.h
