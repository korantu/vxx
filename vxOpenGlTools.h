/**
*  
*  @file vxOpenGlTools.h
   \brief OpenGL header file.
   Handles all the needed includes and definitions. glVertex3f functions allow to use V3f with OpenGL directly.
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
#ifdef KDL_CYGWIN
#ifndef WIN32
#include <GL/glext.h>
#endif
#endif

#ifdef WIN32 //OpenGL is lacking then
//Missing definitions from MSVC
#define GL_TEXTURE_3D                     0x806F
#define GL_TEXTURE_3D_EXT                 0x806F
#define GL_TEXTURE_WRAP_R                 0x8072
#define GL_TEXTURE_WRAP_R_EXT             0x8072
typedef void (APIENTRY * PFNGLTEXIMAGE3DPROC) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
#endif

#include "vxVector.h"

/*
 Useful shortcuts for OpenGL calls.
*/

const V3f & glVertex3f(const V3f &); //!< \brief Calls OpenGL glVertex3f function. Uses V3f as the argument, returning it.
const V3f & glNormal3f(const V3f &); //!< \brief Calls OpenGL glNormal3f function. Uses V3f as the argument, returning it.
const V3f & glColor3f(const V3f &); //!< \brief Calls OpenGL glColor3f function. Uses V3f as the argument, returning it.
const V3f & glTexCoord3f(const V3f &); //!< \brief Calls OpenGL glTexCoord3f function. Uses V3f as the argument, returning it.
const V3f & glTranslatef(const V3f &); //!< \brief Calls OpenGL glTranslatef function. Uses V3f as the argument, returning it.
const V3f & glScalef(const V3f &); //!< \brief Calls OpenGL glScalef function. Uses V3f as the argument, returning it.
const V3f & SetColor(const V3f &); //!< \brief Calls OpenGL SetColor function. Uses V3f as the argument, returning it.

/*! \brief Enables transparency.
Should be called after everything opaque is drawn. */
void TransparencyStart();
void TransparencyEnd(); //!< \brief Back to drawing solid stuff. 
void Transparency(bool enable); //!< \brief A switch to honor TransparencyStart / TransparencyEnd calls. 

#endif // __vxOpenGlTools_h__

//End of vxOpenGlTools.h
