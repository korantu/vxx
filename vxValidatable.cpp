/**
*  
*  @file vxValidatable.cpp
*  
*  This source file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#include "vxValidatable.h"

/// Determines if the state of the object is valid.
/// Rationale - check if a sequence of operations succeeded.
 Validatable::Validatable(){valid_ = true;};
 bool Validatable::valid(){return valid_;};
  bool Validatable::valid(bool is_valid){valid_ = is_valid; return valid_;};
Validatable::operator bool(){return valid_;};


// End of vxValidatable.cpp
