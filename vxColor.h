/**
*  
*  @file vxColor.h
*  
*  This header file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#ifndef __vxColor_h__
#define __vxColor_h__

#ifndef WIN32
#include <GL/glfw.h>
#endif
#include <vector>
#include <string>

#include "vxVector.h"

struct ColorMapper{
  ColorMapper(); //fills in default colors
  ColorMapper(const ColorMapper &); //fills in default colors
  unsigned char col[256*3];

  ColorMapper & operator=(const ColorMapper &);

  void map(void *, int); 
  void map(int); //use opengl Color3i 
  void map(int & r, int & g, int &b, int); 

  void fill_init(V3f _f);
  void fill(int end, V3f to);
  void fill(const int *);

  int start;
  V3f from;

};

void color_init();
std::vector<std::string> color_type();

void scheme_fill(ColorMapper &, int scheme);
 

#endif // __vxColor_h__

//End of vxColor.h
