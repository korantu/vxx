/**
*  
*  @file vxDrawPlane.cpp
*  
*  This source file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#include "vxDrawPlane.h"
#include "vxOpenGlTools.h"

void DrawPlane(const V3f & center, const V3f & dx , const V3f & dy, int cells){
  glBegin(GL_LINES);
  glColor3f(0,0,0);
  V3f a1(center - dx*cells - dy*cells);
  V3f b1(center + dx*cells - dy*cells);
  V3f a2(center - dx*cells - dy*cells);
  V3f b2(center - dx*cells + dy*cells);
  for(int i = 0; i <= 2*cells; i++){
    glVertex3f(a1+dy*i); glVertex3f(b1+dy*i);
    glVertex3f(a2+dx*i); glVertex3f(b2+dx*i);
  };
  glEnd();
};


// End of vxDrawPlane.cpp
