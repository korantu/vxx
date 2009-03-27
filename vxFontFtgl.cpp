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
#include <vector>

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

//Console
struct ConsoleImplementation: Console {
  bool enable;
  V3f pos;
  float height;
  int max_lines;
  std::vector<std::string> lines;
  

  Console * AddLine(std::string a_line){
    lines.push_back(a_line);
    if(lines.size() > max_lines)lines.erase(lines.begin());
    return this;
  };

  //Toggle visibility
  void Start(){
    enable = !enable;
  };

  void Draw(){
    if(!enable)return;
    for(int i = 0; i < lines.size(); i++){
      V3f offset(0, -height * 1.2*i, 0);
      DrawLineAt(lines[i], pos+offset, height);
    };
  };

  ConsoleImplementation(V3f _pos, float _height, int _lines): pos(_pos), 
							      height(_height), 
							      max_lines(_lines), 
							      enable(true){};

}; //ConsoleImplementation

Console * NewConsole(V3f pos, float height, int lines){
  return new ConsoleImplementation( pos, height, lines);
};



// End of vxFontFtgl.cpp
