/**
*  
*  file vxVector.cpp
*  
*  This source file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#include <stdio.h>

#include "vxVector.h"


///stupid. but too much coffee already

V3f rot_x(V3f in, float r){
  V3f res;
  res.x = in.x;
  res.y = cos(r)*in.y-sin(r)*in.z;
  res.z = sin(r)*in.y+cos(r)*in.z;
  return res;
};

V3f rot_y(V3f in, float r){
  V3f res;
  res.y = in.y;
  res.z = cos(r)*in.z-sin(r)*in.x;
  res.x = sin(r)*in.z+cos(r)*in.x;
  return res;
};

V3f rot_z(V3f in, float r){
  V3f res;
  res.z = in.z;
  res.x = cos(r)*in.x-sin(r)*in.y;
  res.y = sin(r)*in.x+cos(r)*in.y;
  return res;
};

//make basis ortonormal again.
void ortoNormalize(V3f & nnx, V3f & nny, V3f & nnz){
  V3f newy; newy.cross(nnz, nnx);
  V3f newz; newz.cross(nnx, newy);
  newy /= newy.length();
  newz /= newz.length();
  nnx /= nnx.length();
  nny = newy;
  nnz = newz;
};

//Find minimal coordinate.
float min(const V3f & in){
  if(in.x < in.y){
    return (in.x < in.z)?in.x:in.z;
  }else{ //in.x > in.y
    return (in.y < in.z)?in.y:in.z;
  };
};

void say(char *msg, const V3f & v){
  printf("%s:%f:%f:%f\n", msg, v.x, v.y, v.z);
};

// End of vxVector.cpp
