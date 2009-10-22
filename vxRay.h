/**
*  
*  @file vxRay.h
\brief Ray abstraction.
Conssits of Origin, Direction and IntersectRayPlane IntersectRaySphere functions.
*  
*  This header file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#ifndef __vxRay_h__
#define __vxRay_h__

#include "vxVector.h"

/*
  Rays.
*/

extern V3f travel_ray; //!< \brief Helper definition. Allows for default parameters in Travel.

/*! \brief Ray structure.
The ray is defined by the point of origin and direction. */
struct Ray {
	V3f O; //!< \brief Origin ray. 
	V3f D; //!< \brief Direction ray.

  Ray(); //!< \brief Constructor. Origin (0,0,0) direction (1,0,0);
  Ray(const V3f &, const V3f &); //!< \brief Construct a vector from Origin and Direction. 
  Ray(const Ray &); //!< \brief Copy constructor. 
  V3f & Travel(float distance, V3f & result = travel_ray); //!< \brief Obtain the point in distance*direction. 
};

/*! \brief Intersection information for a Ray.
Supports missed rays as well. */
struct Intersection{
  bool hit; //!< \brief If this intersection really exists or it was a miss. 
  float distance; //!< \brief Distance to the intersection. Use Travel to calculate.
  Intersection & is(bool, float); //!< \brief Set the intersection to the specified values. 
  Intersection(); //!< \brief Construct empty Intersection. 
};

struct Surface;

/*! \brief Intersect a ray with a plane.
The plane is specified by plane.O origin which it contains and plane.D direction which is normal to it. */
Intersection IntersectRayPlane(const Ray & ray, const Ray & plane);  
Intersection IntersectRaySphere(const Ray & ray, const V3f & center, float r); //!< \brief Intersect ray with a sphere of radius r at center. 
Intersection IntersectRaySurface(const Ray & ray, Surface * surface); //!< \brief Intersect ray with the Surface. 
 

#endif // __vxRay_h__

//End of vxRay.h
