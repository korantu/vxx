/**
 *  
 *  @file vxProjection.cpp
 *  
 *  This source file is a part of VoxelBrain software.
 *  
 *  (c) Nanyang Technological University
 *  
 *  Author: Konstantin Levinski
 *  
 */

#include "vxProjection.h"

Projection::Projection() { Reset(); };

V3f Projection::X(){return V3f(ex.x, ey.x, ez.x);};
V3f Projection::Y(){return V3f(ex.y, ey.y, ez.y);};
V3f Projection::Z(){return V3f(ex.z, ey.z, ez.z);};
V3f Projection::E(){return ex;};
  
void Projection::Zoom( int x, int y, float _zoom ){
  zoom_x = x; zoom_y = y; zoom = _zoom;
};

void Projection::CopyMatrix(){
  matrix[0]=ex.x; matrix[1]=ex.y; matrix[2]=ex.z;
  matrix[4]=ey.x; matrix[5]=ey.y; matrix[6]=ey.z;
  matrix[8]=ez.x; matrix[9]=ez.y; matrix[10]=ez.z;
};

void Projection::Reset( int N){
  zoom = 1; zoom_x = 0; zoom_y = 0;
  V3f basis[] = {V3f(1,0,0), V3f(0,1,0), V3f(0,0,1)};
  ex=basis[N%3]; ey=basis[(N+1)%3]; ez=basis[(N+2)%3]; 
  double _matrix[] = {1.0, 0.0, 0.0, 0.0, //Copy last row and column.
		      0.0, 1.0, 0.0, 0.0,
		      0.0, 0.0, 1.0, 0.0,
		      0.0, 0.0, 0.0, 1.0};
  for(int i = 0; i < 16; i++)matrix[i]=_matrix[i];
  CopyMatrix(); //middle part.
  ortoNormalize(ex,ey,ez);
};


void Projection::Rotate(float x, float y){
  ex = rot_x(ex,x); ey = rot_x(ey,x); ez = rot_x(ez,x);
  ex = rot_y(ex,y); ey = rot_y(ey,y); ez = rot_y(ez,y);
  //printf("%f\n",ex.x);;
  ortoNormalize(ex,ey,ez);
};

void Projection::Draw(){
  int width, height;
  
  //Zoom is a fraction of the whole shape
  //displayed full screen.
  if(zoom > 1)zoom = 1;     
  if(zoom < 0.1)zoom = 0.1;

  glClearColor(1.0,1.0,1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | 
	  GL_DEPTH_BUFFER_BIT | 
	  GL_STENCIL_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
 
  glMatrixMode(GL_PROJECTION);
  glfwGetWindowSize(&width, &height);
  glViewport(0,0, width, height);
  glLoadIdentity();

  //half-widths:
  float fit_w = ((width>height)?(float)width/(float)height:1.0f);
  float fit_h = ((width>height)?1.0f:(float)height/(float)width);

  //centers; if zoom is ~1, center is still 0, as to
  //avoid instant shifting.
  float center_w = 2*fit_w*(zoom_x - width/2)/width*(1.0f-zoom);
  float center_h = 2*fit_h*(- zoom_y + height/2)/height*(1.0f-zoom);


  glFrustum(-fit_w*zoom+center_w, fit_w*zoom+center_w, 
	      -fit_h*zoom+center_h, fit_h*zoom+center_h, 
	    2*fit_w, 6.83*fit_w); //to make far and near symmetrical.
  
    glTranslatef(0,0,-4*fit_w);

  glMatrixMode(GL_MODELVIEW);

  CopyMatrix();
  glLoadMatrixd(matrix);
};

Projection the_projection;
Projection * GetProjection(){
  return & the_projection;
};

// End of vxProjection.cpp
