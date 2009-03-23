/**
*  
*  @file vxOpenGlTools.cpp
*  
*  This source file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#include "vxOpenGlTools.h"


/*
  Conviniences.
*/

const V3f & glColor3f(const V3f & v){
  glColor4f(v.x, v.y, v.z, 0.2); return v;
};

const V3f & glVertex3f(const V3f & v){
  glVertex3f(v.x, v.y, v.z); return v;
};

const V3f & glNormal3f(const V3f & v){
  glNormal3f(v.x, v.y, v.z); return v;
};

const V3f & glTexCoord3f(const V3f & v){
  glTexCoord3f(v.x, v.y, v.z); return v;
};

const V3f & SetColor(const V3f & v){
  glColor3f(v.x, v.y, v.z); return v;
};

void TransparencyStart(){
    glEnable(GL_BLEND);
    glDisable(GL_LIGHTING);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
};

void TransparencyEnd(){
  glDisable(GL_BLEND);
  glEnable(GL_LIGHTING);
};

// End of vxOpenGlTools.cpp
