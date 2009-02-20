/**
*  
*  @file vxMouseRay.cpp
*  
*  This source file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#include "vxMouseRay.h"
#include "vxOpenGlTools.h"

Ray MouseRay(){
  int x, y, width, height;
  glfwGetMousePos(&x, &y);
  glfwGetWindowSize(&width, &height);
    y = height-y;

  GLdouble modelview[16];
  GLdouble projection[16];
  GLint viewport[4];

  glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
  glGetDoublev(GL_PROJECTION_MATRIX, projection);
  glGetIntegerv(GL_VIEWPORT, viewport);
  double nx, ny, nz;
  double fx, fy, fz;
  gluUnProject( x, y, -1, modelview, projection, viewport, &nx, &ny, &nz);
  gluUnProject( x, y, 1, modelview, projection, viewport, &fx, &fy, &fz);
  V3f near((float)nx, (float)ny, (float)nz);
  V3f far((float)fx, (float)fy, (float)fz);
  V3f dir(far); dir -= near;

  return Ray(near, dir);
};

// End of vxMouseRay.cpp

