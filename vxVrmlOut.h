/**
*  
*  @file vxVrmlOut.h
*  
*  This header file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*  Printing out surface as a vrml node
*/

#ifndef __vxVrmlOut_h__
#define __vxVrmlOut_h__

#include <string>

#include "vxSurface.h"

std::string VrmlTemplate(std::string vertices, std::string normals, std::string colors, std::string indices);
std::string VrmlFormat(vector<V3f> vertices, bool invert = false);
std::string VrmlFormat(vector<V3i> indices);
 

#endif // __vxVrmlOut_h__

//End of vxVrmlOut.h
