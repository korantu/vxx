/**
*  
*  @file vxVrmlOut.h
\brief Code to output VRML scenes.
Creates VRML mesh, use VrmlTemlpate together with VrmlFormat.
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

/*! \brief Creates a valid VRML string out of set of vertices and indices.
Uses string VrmlFormat concatenation. */
std::string VrmlTemplate(std::string vertices, std::string normals, std::string colors, std::string indices);
std::string VrmlFormat(vector<V3f> vertices, bool invert = false); //!< \brief Convert set of vertices into VRML list of vertices (as string). invert parameter inverts the normals, in case the software uses different orientation than VRML.
std::string VrmlFormat(vector<V3i> indices); //!< \brief Convert set of indices into VRML list of vertices (as string). 
 

#endif // __vxVrmlOut_h__

//End of vxVrmlOut.h
