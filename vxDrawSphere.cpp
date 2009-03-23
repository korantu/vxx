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
  glColor4f(0.5,0.5,0.5, 0.5);
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
  glColor4f(1.0f, 0.0f, 0.0f, 0.5f); //TODO(?)
  for(float b = -PI/2; b < PI/2; b+=db)
    for(float a = 0; a < (2*PI-da); a+=da){
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

/// Functions:


void DrawSphereVertexFunction(float a, 
		      float b, 
		      const V3f & where, 
		      float radius, Textured * t){
  V3f pos(where); V3f n(cos(b)*sin(a), 
			cos(b)*cos(a), 
			 sin(b));
  pos += n*radius;

  if(t){
    t->SetTexture(pos);
  }else{
    //glColor3f(cos(b), cos(b)*cos(a), cos(b)*cos(a+b));
    glColor3f(0.5,0.5,0.5);
  };
  glNormal3f(1*n.x, 1*n.y, 1*n.z);
  glVertex3f(pos);
};


void DrawSphereFunction(const V3f & where, 
		float radius, 
		int steps, 
		Textured * t){
   glEnable(GL_TEXTURE_3D);
  if(t){ //Ensure we are in the proper place
    V3f diagonal(radius, radius, radius);
    Range range_to_check(where-diagonal, where+diagonal);
    t->CheckTexture(range_to_check);
  };
  V3f cur;
  const float da = PI/steps;
  const float db = PI/steps;
  glBegin(GL_QUADS);
  for(float b = -PI/2; b < PI/2; b+=db)
    for(float a = 0; a < 2*PI; a+=da){
      DrawSphereVertexFunction( a, b, where, radius,t);
      DrawSphereVertexFunction( a, b+db, where, radius,t);
      DrawSphereVertexFunction( a+da, b+db, where, radius,t);
      DrawSphereVertexFunction( a+da, b, where, radius,t);
    }; 
  glEnd();
  glDisable(GL_TEXTURE_3D);

};


//End of vxDrawSphere.h
