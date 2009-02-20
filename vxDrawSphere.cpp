/*
*
* @file vxDrawSphere.cpp
* 
*  This file is a part of VoxelBrain software.
*
*  (c) Nanyang Technological University
*
*  Author: Konstantin Levinski
*
*/


#include "vxDrawSphere.h"
#include "vxOpenGlTools.h"

void DrawSphere::DrawSphereVertex(float a, float b){

  
  V3f pos(where); V3f n(cos(b)*sin(a), 
			cos(b)*cos(a), 
			 sin(b));
  pos += n*radius;

  /* TODO(use SetupVertex to get a texture)
  if(t){
    t->SetTexture(pos);
    }else{ */
    glColor3f(0.5,0.5,0.5);
    //};
  glNormal3f(n);
  glVertex3f(pos);
};


DrawSphere::DrawSphere(V3f _where, 
		       float _radius, 
		       int _steps):
  where(_where),  radius(_radius),  steps(_steps)
{};

void DrawSphere::Draw(){
  glEnable(GL_TEXTURE_3D);
  V3f diagonal(radius, radius, radius);
  SetupRange(Range(where-diagonal, where+diagonal));
  V3f cur;
  const float da = PI/steps;
  const float db = PI/steps;
  glBegin(GL_QUADS);
  for(float b = -PI/2; b < PI/2; b+=db)
      for(float a = 0; a < 2*PI; a+=da){
	DrawSphereVertex( a, b);
	DrawSphereVertex( a, b+db);
	DrawSphereVertex( a+da, b+db);
	DrawSphereVertex( a+da, b);
      }; 
  glEnd();
  glDisable(GL_TEXTURE_3D);
};


void DrawSphere::SetupVertex(const V3f & ){};
void DrawSphere::SetupRange(const Range &){};

//End of vxDrawSphere.h
