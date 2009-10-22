/**
*  
*  @file vxMouseRay.h
   \brief Defines ray from the mouse point.
   MouseRay returns the ray from the current mouse position.
*  
*  This header file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#ifndef __vxMouseRay_h__
#define __vxMouseRay_h__

#include "vxRay.h"

/*! \brief Current viewing ray from the mouse point.
The ray always exists. */
Ray MouseRay(); 

#endif // __vxMouseRay_h__

//End of vxMouseRay.h
