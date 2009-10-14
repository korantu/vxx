/**
*  
*  @file vxSurfaceSlicer.cpp
*  
*  This source file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#include "vxSurfaceSlicer.h"
#include "vxSurface.h"
#include "vxProjection.h"
#include "vxOpenGlTools.h"

using namespace std;

//Splice edge with centered sphere.
bool SpliceEdgeWithSphere(const V3f & a, const V3f & b, float radius, V3f * out){

  float al = a.length(); 
  float bl = b.length();
  
  if( ( (al >= radius) && (bl >= radius) ) ||
      ( (al <= radius) && (bl <= radius) ) ) return false; 

  *out =  a * (bl-radius)/(bl-al) + b * (radius-al)/(bl-al);

  return true;
};

void GetBorderLine(Surface * in, V3f center, float radius, BorderLine * bdr){

  V3f edge[3];
  for(size_t i = 0; i < in->tri.size(); i++){
    V3f cur_vertex;
    int cur_side = 0;

    V3f a; V3f b;
    //Check 3 sides.
    for(int j = 0; j < 3; j++){
      int ind_1 = in->tri[i][j]; int ind_2 = in->tri[i][(j+1)%3]; 
      a = in->v[ind_1]-center; b = in->v[ind_2]-center;
      if(SpliceEdgeWithSphere( a, b, radius, &cur_vertex)){
	edge[cur_side] = cur_vertex+center;
	cur_side++;
      };
    };
    //if crossing 2: (forget about edge cases.)
    if(cur_side == 2){
      bdr->push_back(edge[0]);
      bdr->push_back(edge[1]);
    };
  }; //Each triangle
  
};

//OpenGL
void DrawBorder(BorderLine * bdr){
  V3f d = GetProjection()->Z();
  glColor3f(1,0,0);
    glBegin(GL_LINES);
  for(BorderLine::iterator i = bdr->begin(); i != bdr->end(); i++){
    glVertex3f((*i)+d);
  };
  glEnd();
};


// End of vxSurfaceSlicer.cpp
