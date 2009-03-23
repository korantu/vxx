/**
* 
* @file vxDrawSurface.h
* 
* This header file is a part of VoxelBrain software.
* 
* (c) Nanyang Technological University
*
*  Author: Konstantin Levinski
*
*/

#ifndef __vxDrawSurface_h__
#define __vxDrawSurface_h__

#include "vxSurface.h"
#include "vxTextured.h"

void DrawSurface( const Surface & surf );
void DrawSurfaceLines( const Surface & surf );
void DrawSphereFunction( const V3f & where, 
			 float radius, 
			 int steps, 
			 Textured * t );

#endif // __vxDrawSurface_h__

//End of vxDrawSurface.h
