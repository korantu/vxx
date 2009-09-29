/**
*  
*  @file vxTools.h
*  
*  This header file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#ifndef __vxTools_h__
#define __vxTools_h__

#include <iostream>
#include "vxVector.h"

std::ostream & operator <<(std::ostream & out, V3f v);

#define ASSERT(X,STR) if(!(X)) { _kdl_panic((char *)STR); };

void _kdl_panic ( const char * problem );

#endif // __vxTools_h__

//End of vxTools.h

