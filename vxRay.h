/**
*  
*  @file vxRay.h
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


struct Ray;

extern V3f travel_ray;
struct Ray {
	V3f O; //origin 
	V3f D; //direction
  Ray();
  Ray(const V3f &, const V3f &);
  Ray(const Ray &);
  V3f & Travel(float distance, V3f & result = travel_ray);
};

//Intersection information for a given ray.
struct Intersection{
  bool hit;
  float distance;
  Intersection & is(bool, float);
};

Intersection & IntersectRayPlane(const Ray & ray, const Ray & plane, Intersection & result);
Intersection & IntersectRaySphere(const Ray & ray, const V3f & center, float r, Intersection & result);
 

#endif // __vxRay_h__

//End of vxRay.h
