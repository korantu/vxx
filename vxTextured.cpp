/**
*  
*  @file vxTextured.cpp
*  
*  This source file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#include <stdlib.h>

#include "vxTextured.h"
#include "vxOpenGlTools.h"
#ifdef KDL_CYGWIN
#ifndef WIN32
#include "GL/glext.h"
#endif
#endif
//making compiler shut up; in vain.
#define BPP 4 
#define SIZE 64
typedef unsigned char * BYTE;

Textured::Textured(){
  texturing_fastvolume = NULL;
  printf("Allocating data.\n");
  data = malloc(SIZE*SIZE*SIZE*BPP);
  if(!data){
    //valid(false);
    printf("Allocation failed.\n");
  }else{
    printf("Data allocated.\n");
  };
};

Textured::~Textured(){
  if(data) free(data);
  force_update = true;
};

int Offset(int x, int y, int z){
  return BPP*(z*SIZE*SIZE+y*SIZE+x);
};

GLuint texname = 0;

#ifndef KDL_CYGWIN
#define HAS_GL_TEX_IMAGE_3D 
#endif

#ifndef HAS_GL_TEX_IMAGE_3D 
PFNGLTEXIMAGE3DPROC glTexImage3D;
#endif //HAS_GL_TEX_IMAGE_3D

void UploadTexture(void * data){

#ifndef HAS_GL_TEX_IMAGE_3D
   glTexImage3D = (PFNGLTEXIMAGE3DPROC) glfwGetProcAddress("glTexImage3D");
	if (glTexImage3D == NULL)
		return;
#endif //HAS_GL_TEX_IMAGE_3D
	
  glTexEnvi(GL_TEXTURE_ENV, 
	    GL_TEXTURE_ENV_MODE, 
	    GL_REPLACE); // our texture colors will replace the untextured colors
  // request 1 texture name from OpenGL
  if(!texname)glGenTextures(1, &texname);
  
  // tell OpenGL we're going to be setting up the 
  //texture name it gave us	
  glBindTexture(GL_TEXTURE_3D, texname);	
  
  // when this texture needs to be shrunk 
  //to fit on small polygons, use linear 
  //interpolation of the texels to determine the color
  glTexParameteri(GL_TEXTURE_3D, 
		  GL_TEXTURE_MIN_FILTER, 
		  GL_LINEAR);//GL_NEAREST);

  // when this texture needs to be magnified to fit on 
  //a big polygon, use linear interpolation of the texels 
  //to determine the color
  glTexParameteri(GL_TEXTURE_3D, 
		  GL_TEXTURE_MAG_FILTER, 
		  GL_LINEAR);//GL_NEAREST);

  // we want the texture to repeat over the S axis, 
  //so if we specify coordinates out of range 
  //we still get textured.
  glTexParameteri(GL_TEXTURE_3D, 
		  GL_TEXTURE_WRAP_S, 
		  GL_REPEAT);

  // same as above for T axis
  glTexParameteri(GL_TEXTURE_3D, 
		  GL_TEXTURE_WRAP_T, 
		  GL_REPEAT);

  // same as above for R axis
  glTexParameteri(GL_TEXTURE_3D, 
		  GL_TEXTURE_WRAP_R, 
		  GL_REPEAT);
  // this is a 3d texture, level 0 (max detail), 
  //GL should store it in RGB8 format, 
  //its WIDTHxHEIGHTxDEPTH in size, 
  // it doesnt have a border, we're giving it to 
  //GL in RGB format as a series of unsigned bytes, 
  // and texels is where the texel data is.
  //  memset(data, 100, SIZE*SIZE*SIZE*BPP);
  glTexImage3D(GL_TEXTURE_3D, 0, 
	       GL_RGBA8, 
	       SIZE, SIZE, SIZE, 0, 
	       GL_RGBA, GL_UNSIGNED_BYTE, data);
};

bool UpdateTextured(Textured & t, Range & r){
  
  //now, set new current_range:
  V3f center((r.min+r.max)/2);
  center.x = floorf(center.x);
  center.y = floorf(center.y);
  center.z = floorf(center.z);

  V3f half_diagonal(SIZE/2, SIZE/2, SIZE/2);
  t.current_range = Range(center-half_diagonal, 
			  center+half_diagonal);
  
  //fill it with a dummy texture for now (maybe 
  //better done with an external class
  Range fast_volume_range(V3f(0,0,0), V3f(255,255,255));

  unsigned char res;

  V3f c(t.current_range.min); //start
  for(int x = 0; x < SIZE; x++)
    for(int y = 0; y < SIZE; y++)
      for(int z = 0; z < SIZE; z++){
	V3f cur(c.x+x, c.y+y, c.z+z);
	if(t.texturing_fastvolume){
	  res = 
	    (unsigned char)t.texturing_fastvolume->\
	    SampleCentered((int)cur.x%255, 
		   (int)cur.y%255, 
		   (int)cur.z%255);

	  //Color conversion.
	  /*	  
	  int r = res * 3; r=(r<0)?0:(r>255?255:r);
	  int g = (res - 80) * 3; g=(g<0)?0:(g>255?255:g);
	  int b = (res - 160) * 3; b=(b<0)?0:(b>255?255:b);
	  */

	  float f_res = res;
	  // f_res *= f_res; f_res /= 60.0;

	  int r = f_res;
	  int g = f_res;
	  int b = f_res;

	  ((BYTE)t.data)[Offset(x,y,z)] = r;
	  ((BYTE)t.data)[Offset(x,y,z)+1] = g;
	  ((BYTE)t.data)[Offset(x,y,z)+2] =  b;
	  ((BYTE)t.data)[Offset(x,y,z)+3] =  255;
	}else{
	  bool line_hit = 
	    !((int)cur.x % 10) || 
	    !((int)cur.y % 10) || 
	    !((int)cur.z % 10);
	  ((BYTE)t.data)[Offset(x,y,z)] = line_hit?30:300;
	  ((BYTE)t.data)[Offset(x,y,z)+1] =line_hit?70:0;
	  ((BYTE)t.data)[Offset(x,y,z)+2] =line_hit?300:30;
	  ((BYTE)t.data)[Offset(x,y,z)+3] =  200;
	};
      };
  //ready; now load the texture
  UploadTexture(t.data);

  return true;
};

//TODO - implement texture source; dummy for now.
bool Textured::CheckTexture(Range & r){

  if(force_update){ //First time.
    force_update = false;
    return UpdateTextured( *this, r);
  };

  if((r.max.x - r.min.x) > SIZE ||
     (r.max.y - r.min.y) > SIZE ||
     (r.max.z - r.min.z) > SIZE)return false;
  if(ContainsRange(current_range, r)){
    return true;
  };
  
  return UpdateTextured( *this, r);
};


const V3f & Textured::SetTexture(const V3f & where){
  //calculating the 0..1 range
  V3f pos = (where - current_range.min)/SIZE; 
  glTexCoord3f(pos);
  return where;
};


// End of vxTextured.cpp
