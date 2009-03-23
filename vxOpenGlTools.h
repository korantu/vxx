/**
*  
*  @file vxOpenGlTools.h
*  
*  This header file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#ifndef __vxOpenGlTools_h__
#define __vxOpenGlTools_h__

#include <GL/glfw.h>

#include "vxVector.h"

/*
 Useful shortcuts for OpenGL calls.
*/

const V3f & glVertex3f(const V3f &);
const V3f & glNormal3f(const V3f &);
const V3f & glColor3f(const V3f &);
const V3f & glTexCoord3f(const V3f &);
const V3f & SetColor(const V3f &);

void TransparencyStart();
void TransparencyEnd();
void Transparency(bool enable);

#endif // __vxOpenGlTools_h__

//End of vxOpenGlTools.h
