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

DrawProperties::DrawProperties(): colored(true), limited(false), wireframe(false), center(0,0,0), radius(1) {};


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

void EmitVertex(const V3f * v, const V3f * n, const V3f * c){
  if(c)glColor3f(*c);
  //HACK - find out why the normal has to be negated
  glNormal3f(-n->x, -n->y, -n->z );
  glVertex3f(*v);
};

int listID = -1;
int maxID = 0;
// Draw every triangle of the surface
void DrawSurface(const Surface & surf, DrawProperties * outer_props){
	if( listID < 0 ){ //Noone bothered creating the lists explicitly yet.
		MakeSurfaceLists(surf, outer_props);
	};
	for(int id = listID; id <= maxID; id++)glCallList(id);
};
// Draw every triangle of the surface
void MakeSurfaceLists( const Surface & surf, DrawProperties * outer_props){
  DrawProperties props = outer_props ? (*outer_props) : DrawProperties(); //Defaults.
  GLenum GL_PRIMITIVE = props.wireframe?GL_LINES:GL_TRIANGLES;

	if(listID > 0){
		glDeleteLists(listID, 1 + (maxID - listID) ); 
	}else{
		listID = glGenLists(1);
	};
	maxID = listID;
	glNewList(listID, GL_COMPILE); 
	  int k = 100000;

  glBegin(GL_PRIMITIVE);

  glColor3f(0.3,0.5,0.4);
  bool colors_valid = (surf.c.size() == surf.v.size()); //make sure there are colors to use.

  V3f v[3];   //Current triangle vertices.
  float l[3]; //Current limiting length.

  for(vector<V3i>::const_iterator i = surf.tri.begin(); i != surf.tri.end(); i++){
  		  if(!(k--)){ //Seem like 100000+ triangles is too much for a list. Split them.
			k = 100000;
			glEnd();
			glEndList();
			maxID++;
	        glNewList(maxID, GL_COMPILE); 
 		    glBegin(GL_PRIMITIVE);
		  };

    //check if this triangle is worth bothering with:

    //TODO - maybe performance issue (?)
    v[0] = surf.v[(*i)[0]];
    v[1] = surf.v[(*i)[1]];
    v[2] = surf.v[(*i)[2]];

    //If it is potentially not in view, make sure it is _really_ not in view.
    if( props.limited ){
      bool in_view = false; 
      for(int j = 0; j < 3; j++)l[j] = (props.center - v[j]).length(); 
      for(int j = 0; j < 3; j++){ //If a vertex is inside - then in view.
	if (l[j] < props.radius ){
	  in_view = true; break;
	};      
      };
      if(!in_view)continue; //Not in view. 

      //Now, let's deal with polygons whith outer vertices:
      for(int j = 0; j < 3; j++){
	if ( l[j] > props.radius ){
	  v[j] *= props.radius / l[j];
	};      
      };
    };
   
    //Ok, now just render the triangle:
    for(int vertex = 0; vertex < 3; vertex++){ //Each verex of a face
      EmitVertex(&v[vertex], &surf.n[(*i)[vertex]],
		 (colors_valid && props.colored)?&surf.c[(*i)[vertex]]:NULL);
    }; //Each vertex of a face
    if(props.wireframe){
      EmitVertex(&v[0], &surf.n[(*i)[0]],
		 (colors_valid && props.colored)?&surf.c[(*i)[0]]:NULL);
      glEnd();
      glBegin(GL_LINES);
    };
  }; //Each face
  glEnd();
  glEndList();
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
