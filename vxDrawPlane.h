/**
*  
*  @file vxDrawPlane.h
	\brief Code to draw flat line grid.
	The size and number of cells are configurable.
*  
*  This header file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#ifndef __vxDrawPlane_h__
#define __vxDrawPlane_h__

#include "vxVector.h"


/*! \brief Draws a grid in 3D, cells x cells size. */
void DrawPlane(const V3f & center, const V3f & dx , const V3f & dy, int cells);

#endif // __vxDrawPlane_h__

//End of vxDrawPlane.h
