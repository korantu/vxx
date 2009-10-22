/**
*  
*  @file vxSurfaceSlicer.h
\brief Code to intersect the surface with the sphere.
BorderLine is populated by GetBorderLine function.
DrawBorder can be used to draw the line using OpenGL.

*  
*  This header file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#ifndef __vxSurfaceSlicer_h__
#define __vxSurfaceSlicer_h__

#include <vector>

#include "vxVector.h"

/**
  A function to derive a set of pairs from intersections with surface.
 */ 

typedef std::vector<V3f> BorderLine; //!< \brief List of point pairs which define the intersection. 
struct Surface;

/*! \brief Get intersection with the sphere.
Each triangle results in at most one pair of points. Computers are fast enough to scan all of them. Moreover, it only needed when the sphere moves.  */
void GetBorderLine(Surface * in, V3f center, float radius, BorderLine *);
void DrawBorder(BorderLine * bdr); //!< \brief Draw border in a thin red line. 


#endif // __vxSurfaceSlicer_h__

//End of vxSurfaceSlicer.h
