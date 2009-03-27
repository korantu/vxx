/**
*  
*  @file vxFontFtgl.cpp
*  
*  This source file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#include <FTGL/ftgl.h>

#include "vxFontFtgl.h"
#include "vxOpenGlTools.h"
#include "vxFileGzipIo.h"
#include "vxBinaryBlobs.h"

FTGLTextureFont * TheFont = NULL;

void InitFont(){
  TheFont = new FTGLTextureFont(_home_kdl_tmp_anonymous_ttf, _home_kdl_tmp_anonymous_ttf_len);
};


void DrawText(std::string text){
  if(!TheFont)InitFont();
  if(TheFont->Error())return;
  TheFont->FaceSize(72);
  TheFont->Render(text.c_str());
};

// End of vxFontFtgl.cpp
