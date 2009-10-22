/**
*  
*  @file vxRange.h
\brief Defines an Range as Axis Aligned Bounding Box in the scene.
Used for texture reload once the current cursor position runs out of the loaded texture. Fairly general, though, supports ContainsPoint, ContainsRange queries.
*  
*  This header file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#ifndef __vxRange_h__
#define __vxRange_h__

#include "vxVector.h"
#include "vxValidatable.h"

/*! \brief A structure for storing Axis - Aligned ranges. 
Defined by two corners. */
struct Range{
  V3f min; //!< \brief Lower-Further corner. 
  V3f max; //!< \brief Upper-Closer corner. 

  Range(const V3f &, const V3f &); //!< \brief Constructor. Takes two corners. 
  Range(); //!< \brief Constructs zero range, so that it can be expanded. 
  Range(const Range &); //!< \brief Copy-constructor. 
};

bool ContainsPoint(const Range &, const V3f &); //!< \brief Returns true if the point is inside. 
bool ContainsRange(const Range &, const Range &); //!< \brief Returns true if two points are inside. 
bool IntersectsRange(const Range &, const Range &); //!< \brief Returns true if one of the two poits is inside. 
Range & ExpandRange(Range &, const V3f &); //!< \brief Expands the range to fit the point. Returns *this for further chaining.

#endif // __vxRange_h__

//End of vxRange.h
