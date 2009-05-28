/**
*  
*  @file vxDrawSurface.cpp
*  
*  This source file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#include "vxDrawSurface.h"
#include "vxOpenGlTools.h"
#include "vxTextured.h"
#include "vxRange.h"

#ifndef PI
#define PI 3.14159265
#endif

/*
  a: [0..2*PI];
  b: [-PI/2..PI/2].
 */

DrawProperties::DrawProperties(): colored(true), limited(false), wireframe(false) {};


void DrawSphereVertex(float a, 
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
/*
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
      DrawSphereVertex( a, b, where, radius,t);
      DrawSphereVertex( a, b+db, where, radius,t);
      DrawSphereVertex( a+da, b+db, where, radius,t);
      DrawSphereVertex( a+da, b, where, radius,t);
    }; 
  glEnd();
  glDisable(GL_TEXTURE_3D);

};
*/

// Draw every triangle of the surface
void DrawSurface( const Surface & surf, DrawProperties * outer_props){
  DrawProperties props = outer_props ? (*outer_props) : DrawProperties(); //Defaults.

  if(props.wireframe)
    glBegin(GL_LINES);
  else
    glBegin(GL_TRIANGLES);

  glColor3f(0.3,0.5,0.4);
  bool colors_valid = (surf.c.size() == surf.v.size()); //make sure there are colors to use.
  for(vector<V3i>::const_iterator i = surf.tri.begin(); i != surf.tri.end(); i++){
    //check if this triangle is worth bothering with:
    if( props.limited && ( (props.center - surf.v[(*i)[0]]).length() > props.radius ) ) continue;

    for(int vertex = 0; vertex < 3; vertex++){ //Each verex of a face
      if(colors_valid && props.colored)glColor3f(surf.c[(*i)[vertex]]);
      glNormal3f(-surf.n[(*i)[vertex]].x, -surf.n[(*i)[vertex]].y, -surf.n[(*i)[vertex]].z );
      glVertex3f(surf.v[(*i)[vertex]]);
    }; //Each vertex of a face
    if(props.wireframe){
      //HACK; remove duplication
      if(colors_valid && props.colored)glColor3f(surf.c[(*i)[0]]);
      glNormal3f(-surf.n[(*i)[0]].x, -surf.n[(*i)[0]].y, -surf.n[(*i)[0]].z );
      glVertex3f(surf.v[(*i)[0]]);
      glEnd();
      glBegin(GL_LINES);
    };
  }; //Each face
  glEnd();
};


//Draw every triangle of the surface, using lines mode.
void DrawSurfaceLines( const Surface & surf){
  glBegin(GL_LINES);
  glColor3f(0,1,0);
  bool colors_valid = (surf.c.size() == surf.v.size()); //make sure there are colors to use.
  for(vector<V3i>::const_iterator i = surf.tri.begin(); i != surf.tri.end(); i++){
    for(int vertex = 0; vertex < 3; vertex++){ //Each verex of a face
      if(colors_valid)glColor3f(surf.c[(*i)[vertex]]);
      glNormal3f( surf.n[(*i)[vertex]] );
      glVertex3f( surf.v[(*i)[vertex]] );
      glNormal3f( surf.n[(*i)[(vertex+1) % 3]] );
      glVertex3f( surf.v[(*i)[(vertex+1) % 3]] );
    }; //Each vertex of a face
  }; //Each face
  glEnd();
};


// End of vxDrawSurface.cpp
