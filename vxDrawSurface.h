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

struct DrawProperties {
  DrawProperties();
  bool colored;
  bool limited;
  bool wireframe;

  //Limitation params
  V3f center;
  float radius;
};

void DrawSurface( const Surface & surf, DrawProperties * properties = NULL );
void MakeSurfaceLists(const Surface & surf, DrawProperties * properties = NULL );
void DrawSurfaceLines( const Surface & surf );
void DrawSphereFunction( const V3f & where, 
			 float radius, 
			 int steps, 
			 Textured * t );

#endif // __vxDrawSurface_h__

//End of vxDrawSurface.h
