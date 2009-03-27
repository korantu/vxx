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

FTGLTextureFont TheStaticFont(_home_kdl_tmp_anonymous_ttf, _home_kdl_tmp_anonymous_ttf_len);

void DrawText(std::string text){
  if(TheStaticFont.Error())return;
  TheStaticFont.FaceSize(72);
  TheStaticFont.Render(text.c_str());
};

void DrawLineAt(std::string text, V3f pos, float height){
  glPushMatrix();
  glTranslatef(pos);
  glScalef( height / 72.0f, height / 72.0f, height / 72.0f);
  DrawText(text);
  glPopMatrix();
};

// End of vxFontFtgl.cpp
