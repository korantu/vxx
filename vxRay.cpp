/**
*  
*  @file vxRay.cpp
*  
*  This source file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#include <math.h>

#include "vxRay.h"
#include "vxSurface.h"

Ray::Ray(const V3f & from, const V3f & direction): O(from), D(direction){};
Ray::Ray(const Ray & ray): O(ray.O), D(ray.D){};
Ray::Ray(): O(0,0,0), D(1,0,0){};

/// Extend ray to specified length.

V3f travel_ray;
V3f & Ray::Travel(float distance, V3f & result){
	result = D; result *= distance; result += O;
	return result;
};

//Convinience for setting intersection
Intersection & Intersection::is(bool hit_, float distance_){
   hit = hit_; distance = distance_;
   return *this;
};

Intersection::Intersection():hit(false){}; //Distance - undefined.

Intersection IntersectRayPlane(const Ray & ray, const Ray & plane){
  //move origin of plane to 0.

  Intersection result;
  V3f dist(plane.O-ray.O);
  //float need_to_travel = dist.dot(plane.D); //along plane normal
  float speed = ray.D.dot(plane.D); //speed of approaching the plane
  if(speed < 0.001)return result.is(false, 0);
  return result.is(true, dist.dot(plane.D) / ray.D.dot(plane.D));
};


// (ray.start+ray.direction*t - center).length() = radius
Intersection IntersectRaySphere(const Ray & ray, const V3f & center, float r){

  //move origin of sphere to 0.
  Intersection result;
  Ray cur(ray);
  cur.O -= center;
  //  (cur.O.x + cur.d.x*t)^2 = cur.O.x*cur.O.x + 2*cur.D.x*cur.O.x*t+cur.D.x*cur.D.x*t*t
  //total length:
  //  cur.O.x*cur.O.x + 2*cur.D.x*cur.O.x*t+cur.D.x*cur.D.x*t*t+
  //  cur.O.y*cur.O.y + 2*cur.D.y*t+cur.D.y*cur.D.y*t*t+
  //  cur.O.z*cur.O.z + 2*cur.D.z*t+cur.D.z*cur.D.z*t*t
  //has to be equal to r*r
  //  cur.O.length2() + 2*sum(cur.D)*t+cur.D.length2()*t*t = r*r
  //  cur.D.length2()*t*t+2*(cur.D.dot(cur.O))*t+cur.O.length2()-r*r = 0;
  float a = cur.D.length2();
  float b = 2*cur.D.dot(cur.O);
  float c = cur.O.length2()-r*r;
  float discriminant = b*b-4*a*c;
  //  printf("a:%f b:%f c:%f; hence discriminant:%f\n", a, b, c, discriminant);
  if(discriminant < 0)return result.is(false, 0); 
  //interested in positive t's
  float discriminant_root = sqrt( discriminant );
  float t1 = (-b + discriminant_root)/(2*a);
  float t2 = (-b - discriminant_root)/(2*a);
  result.is(true, (t1 < t2)?t1:t2);
  return result;
};


Intersection IntersectRaySurface(const Ray & pointer, Surface * surf){  
  Intersection result;
  for(int i = 0; i < surf->v.size(); i++){
    Intersection contact_point = IntersectRaySphere(pointer, surf->v[i], 10);
    if(contact_point.hit){
      if(!result.hit){result = contact_point;}else{
	result = (result.distance > contact_point.distance)?
	  contact_point:result;
      };
    };
  };
  return result;
};


// End of vxRay.cpp
