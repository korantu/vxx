/*
 * @file vxDrawSphere.h
	\brief Code to draw a textured sphere.
	The sphere can be either drawn immediately, or prepared and drawn later, when needed.
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
#include "vxTextured.h"



/*! \brief Class to draw a textured sphere.
It is configurable which range of the texture is used. Probably it is too configurable, you can use DrawSphereFunction() instead in most cases. */
struct DrawSphere { 


/*! \brief Constructor, specifies where, how big and how precise a sphere we need.
	steps says how many sections of polygons we want for the sphere approximation. */
  DrawSphere(V3f where, 
	     float radius, 
	     int steps);


	 /*! \brief Draw the sphere.
	Generates proper texture coordinates. */
	 void Draw();

  
  
  /*! \brief Called for each vertex.
  Useful for issuing opengl commands. */
  virtual void SetupVertex(const V3f & );
  
  /*! \brief lets the derived class know which texture range we need. */
  virtual void SetupRange(const Range & );

private:

  V3f & where; 
  float radius; 
  int steps;

  void DrawSphereVertex(float , float);

};


/*! \brief Simply draw a textured (or not) sphere.
Uses steps sections of polygons, and is textured according to the provided parameter. If the Textured parameter is NULL, then untextured sphere is rendered. */
void DrawSphereFunction(const V3f & where, float radius, int steps, Textured * t);

#endif // __vxDrawSphere_h__
