/**
*  
*  file vxLighting.cpp
*  \brief Automated lighting setup.
*  This source file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#include "vxProjection.h"

void DrawLighting(){
      glEnable(GL_NORMALIZE);
      glEnable(GL_LIGHTING);
      glEnable(GL_LIGHT0);
      glEnable(GL_COLOR_MATERIAL);
      glColorMaterial(GL_FRONT_AND_BACK, GL_EMISSION);

      float specular[] = {0.6,0.6,0.6,1};
      glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
      glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

      float diffuse[] = {0.6,0.6,0.6,1};
      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
      glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

      GLfloat high_shininess[] = { 50.0 };
      glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

      //Light position
      V3f lp(GetProjection()->Z()); lp *= 1; 
      lp += GetProjection()->X()*0.8; 
      lp += GetProjection()->Y()*0.8;
      lp *= 250;

      float position[] = {lp.x, lp.y, lp.z, 1};
      glLightfv(GL_LIGHT0, GL_POSITION, position);

};


// End of vxLighting.cpp
