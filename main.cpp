#include "vxTextured.h"
#include "vxAction.h"
#include "vxOpenGlTools.h"
#include "vxDrawSphere.h"
#include "vxDrawSurface.h"
#include "vxLoader.h"
#include "vxSurface.h"
#include "vxMotion.h"
#include "vxRay.h"
#include "vxProjection.h"


V3f center(0,0,0); //Where the center of the crrossecting object is located.
float radius = 30; //Its radius.
float depth_correction = 0.8f; //How deep it is; 1 - on the surface, 0 - in the center.

Surface surf; 
FastVolume vol;
Textured tex;

struct : Action {
  
  void Do(){
    Intersection isct = IntersectRaySurface(GetMotion()->sight, &surf);
    if(isct.hit){
      center = GetMotion()->sight.Travel(isct.distance);
      center *= depth_correction;
    };
    
  };
} sphere_placer;

//Moving projection along the axis.
struct : Action {
  void Do(){
    center += GetProjection()->Z() * ((float)GetMotion()->dy / 4.0); 
  };
} sphere_tuner_z;

struct : Action {
  void Do(){
    center += GetProjection()->X() * ((float)GetMotion()->dx / 2.0); 
    center += GetProjection()->Y() * ((float)GetMotion()->dy / -2.0); 
  };
} sphere_tuner_xy;


//Ajust the sphere.
struct : Action {
  
  int x_old, y_old;
  V3f center_old; float radius_old;

  void Start(){
    x_old = GetMotion()-> x;
    y_old = GetMotion()-> y;
    center_old = center;
    radius_old = radius;
  };

  void Do(){
    float center_factor = ( (float)GetMotion() -> x - x_old ) / 100.0f;
    float radius_factor = ( (float)GetMotion() -> y - y_old ) / 100.0f;
    V3f center_new = center_old * (1.0f + center_factor);
    float radius_new = radius_old * (1.0f - radius_factor);
    if(center_new.length() > 50 && center_new.length() < 150) center = center_new;
    if(radius_new > 10 && radius_new < 30) radius = radius_new;
  };

  void End(){
    depth_correction *= (float)center.length() / (float)center_old.length();
  };
    
} sphere_sizer;


struct PushingAction: Action {
  void Do(){
    Intersection isct = IntersectRaySphere(GetMotion()->sight, center, radius);
    if(isct.hit){
      V3f pos = GetMotion()->sight.Travel(isct.distance);
      PushPoint(surf, pos);
      FixNormals(surf);
      AnalyzeSurface(surf, vol);
    };
  }
} pusher;


int main(){
  //TODO - remove if done in vxDrawSphere_UT.h
  MgzLoader mri(vol);
  mri.Load("data/brainmask.mgz");
  read_surface_binary(surf, "data/lh.pial");

  tex.texturing_fastvolume = &vol; 

  AnalyzeSurface(surf, vol);

  struct TexturedSphere: public Action {

    void Draw(){
      DrawSphereFunction( center, radius, 30, &tex);
      glColor4f(0.5f,0.5f,0.5f,0.5f);
      SortSurface(&surf, GetProjection()->Z());
      DrawSurface(surf);
    };

  } scene; 
  
  sphere_placer.bind(GLFW_KEY_RCTRL);
  sphere_sizer.bind(GLFW_KEY_F3);
  pusher.bind(GLFW_KEY_F2);

  sphere_tuner_xy.bind('.');
  sphere_tuner_z.bind(',');

  GetScene()->run( & scene );

  return 0;
};

