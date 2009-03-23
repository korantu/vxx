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
 Useful shortcuts.
 */

const V3f & glVertex3f(const V3f &);
const V3f & glNormal3f(const V3f &);
const V3f & glColor3f(const V3f &);
const V3f & glTexCoord3f(const V3f &);
const V3f & SetColor(const V3f &);

/*
  Interfaces.
 */
struct Drawable {
  virtual void Draw() = 0;
};

/*
//Used for determining a point.
struct Solid{
  V3f Hit(V3f &) const;
};
*/

void Transparent(Drawable *);
void Transparent(bool is); 
 
#endif // __vxOpenGlTools_h__

//End of vxOpenGlTools.h
