/**
*  
*  @file vxProjection.h
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

struct Projection {

  Projection(); // Resetting the thing.

  float zoom; //Zooming focus. Used to set up frustum.
  int zoom_x;
  int zoom_y;

  double matrix[16];
  V3f ex;
  V3f ey;
  V3f ez;

  void CopyMatrix();

  V3f X();
  V3f Y();
  V3f Z();
  V3f E();
  
  void Rotate( float x, float y); 
  void Zoom( int x, int y, float times);
  void Reset( int N = 0);

  void Draw();
};

Projection * GetProjection();


 
#endif // __vxProjection_h__

//End of vxProjection.h
