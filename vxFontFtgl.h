/**
*  
*  @file vxFontFtgl.h
*  
*  This header file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#ifndef __vxFontFtgl_h__
#define __vxFontFtgl_h__

#include <string>

#include "vxVector.h"
#include "vxAction.h"

void DrawText(std::string text);
void DrawLineAt(std::string text, V3f pos, float height);

#endif // __vxFontFtgl_h__

//End of vxFontFtgl.h
