/**
*  
*  @file vxProjection.h
\brief Projection structure for camera implementation.
It stores the copies of projection matrix and calculates new ones at each move.
*  
*  This header file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#ifndef __vxProjection_h__
#define __vxProjection_h__

#include "vxOpenGlTools.h"

/*! \brief Main Projection of the scene.
Allows for rotation and zooming into a particular spot.  */
struct Projection {

  /*! \brief Main projection initial values are set here.
   */
  Projection();

  float zoom; //!< \brief Zooming. Used to set up frustum.
  V3f focus; //!< \brief The point where camera zooms and rotates around. 
  int zoom_x; //!< \brief 2D point of where camera zooms. 
  int zoom_y;

  /*! \brief OpenGL matrix.
  Constructed and directly interchangeable with OpenGL. */
  double matrix[16]; 
  V3f ex; //!< \brief Unit vector x. Copied to matrix with CopyMatrix.
  V3f ey; //!< \brief Unit vector y. Copied to matrix with CopyMatrix.
  V3f ez; //!< \brief Unit vector z. Copied to matrix with CopyMatrix.

  /*! \brief Copy current coordinates into OpenGL-ready matrix.
  Takes ex,ey,ez and puts them into matrix in the OpenGL way. */
  void CopyMatrix();

  V3f X(); //!< \brief Accessor for X viewer axis.  
  V3f Y(); //!< \brief Accessor for Y viewer axis. 
  V3f Z(); //!< \brief Accessor for Z viewer axis. 
  V3f E(); //!< \brief Returns ex. 
  
  /*! \brief Rotates projection.
  x and y used as deltas of mouse movement. Should not be very large. */
  void Rotate( float x, float y); 
  
  /*! \brief Zoom into a spot.
  x and y coordinates of the spot, times is soom strength. */
  void Zoom( int x, int y, float times);

  /*! \brief Sets up the point of interst.
  Rotation will be performed around this point. */ 
  void Focus( V3f );
  
  void Reset( int N = 0); //!< \brief Reset view to original state. The type of the original view is specified with N. 

  /*! \brief Issue OpenGL commands for this projection.
  The commands are in accordance with current values.  */
  void Draw();
};

/*! \brief Get the Projection singleton.
Called in the drawing loop for OpenGL commands and in control functions to change view. */
Projection * GetProjection();


 
#endif // __vxProjection_h__

//End of vxProjection.h
