/**
*  
*  @file vxMotion.h
   \brief Defines Motion structure. 
   Motion is a singleton. It provides information about current position and speed of the mouse and the view ray. GetMotion provides access to it. GLFW-based code obtains it to update when a mouse position changes. All the other code can obtain it to use.

*  
*  This header file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#ifndef __vxMotion_h__
#define __vxMotion_h__

#include "vxRay.h"

/*! \brief Information about mouse position and view ray.
Singleton. Defined in vxMotion.cpp and accessed throu GetMotion. */
struct Motion {
  int x, y, dx, dy; //!< \brief Current position and the difference with the previous one. 
  V3f start; //!< \brief Start of the mouse ray. 
  V3f end;   //!< \brief End of the mouse ray. 
  Ray sight; //!< \brief Line of sight. Equivalent to end - start 
  
  Motion * Init(int x, int y); //!< \brief Initial setting of the position, so that dx/dy have reasonable values at the next call. Returns *this for chaining.
  Motion * Move(int x, int y); //!< \brief Update movement. Called by the GLFW. 
  Motion * UpdateRay(); //!< \brief Recalculate line of sight. 
};

/*! \brief Motion singleton access.
GLFW Callbacks call it to update position with Move, and other code use it to access position information.  */
Motion * GetMotion();

#endif // __vxMotion_h__

//End of vxMotion.h
