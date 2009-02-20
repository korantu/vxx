/*
 * @file vxDrawSphere.h
 *
 * This header file is a part of VoxelBrain software.
 *
 * (c) Nanyang Technological University
 *
 * Author: Konstantin Levinski
*/

#ifndef __vxDrawSphere_h__
#define __vxDrawSphere_h__

#include "vxVector.h"
#include "vxRange.h"
#include "vxScene.h"

//!Draws a sphere upon creation.

struct DrawSphere: public Drawable { 
  DrawSphere(V3f where, 
	     float radius, 
	     int steps);

  void Draw();

  //Callbacks for texturing.
  virtual void SetupVertex(const V3f & );
  virtual void SetupRange(const Range & );

private:

  V3f & where; 
  float radius; 
  int steps;

  void DrawSphereVertex(float , float);

};
#endif // __vxDrawSphere_h__
