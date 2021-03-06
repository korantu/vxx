/**
* 
* @file main.cpp
\brief Main file. 
Defines all the user Action s and the drawing Action.
Global variables for use in actions are also defined here.
Contains main loop.
*
* This file is a part of VoxelBrain software.
* 
* (c) Nanyang Technological University
*
*  Author: Konstantin Levinski
*
*/

/*!
	\mainpage Programmer's guide to the VASET code base.
    Most processing takes place in classes derived from Action.
    They call into FastVolume to keep track of the volume data and into Surface to work with surface.
	main.cpp contains small number of global variables, such as position, camera location, current surface/volume.
*/

#include "vxTextured.h"
#include "vxAction.h"
#include "vxOpenGlTools.h"
#include "vxDrawSphere.h"
#include "vxDrawSurface.h"
#include "vxLoader.h"
#include "vxSurface.h"
#include "vxMotion.h"
#include "vxRay.h"
#include "vxFontFtgl.h"
#include "vxProjection.h"
#include "vxPatientsNavigation.h"
#include "vxSurfaceSlicer.h"
#include "vxFollower.h"

/*! \brief Flag to show/hide surface.  */
bool show_surface = true;
DrawProperties surface_type; //!< \brief Defines what kind of rendering is done by DrawSurface. 

V3f center(0,0,0); //!< Where the center of the crossecting object is located.
float radius = 30; //!< Its radius.
float depth_correction = 0.8f; //!< How deep it is; 1 - on the surface, 0 - in the center.

int kernel = 6;

Surface surf; //!< \brief Main surface. 
FastVolume vol; //!< \brief Main volume. 
Textured tex; //!< \brief The texture. 
BorderLine bdr; //!< \brief Thin red line around the sphere. 

/*! \brief Update the sphere.
Updates border. */
void SphereMoved(){
  surface_type.center = center;
  surface_type.radius = radius * 1.2;
  bdr.clear();
  GetBorderLine(&surf, center, radius, &bdr);
};

/*! \brief Name of the data in use.
 */
std::string patient_name = "None loaded.";

/*! \brief Unused
 */
struct Kerneler : Action {
  int val;
  void Init(int _val){val = _val;};
  void Start(){kernel = val;};
};

/*! \brief Switch to show/hide the surface.
Implemented as a a bidable action */
struct SurfaceSwitcher: Action {
  void Start(){ show_surface = !show_surface; };
} surface_switcher;

/*! \brief Action to place a sphere onto the surface.
Done by pointing mouse. */
struct SpherePlacer: Action {
  
  void Do(){
    Intersection isct = IntersectRaySurface(GetMotion()->sight, &surf);
    if(isct.hit){
      center = GetMotion()->sight.Travel(isct.distance);
      center *= depth_correction;
    };
    
  };
} sphere_placer;

/*! \brief Move the object with the mouse axis-wise.
Z axis. */
struct SphereTunerZ : Action {
  void Do(){
    center += GetProjection()->Z() * ((float)GetMotion()->dy / 4.0); 
  };
} sphere_tuner_z;


/*! \brief Move the object with the mouse axis-wise.
XY-plane. */
struct SphereTunerXY : Action {
  void Do(){
    center += GetProjection()->X() * ((float)GetMotion()->dx / 2.0); 
    center += GetProjection()->Y() * ((float)GetMotion()->dy / -2.0); 
  };
} sphere_tuner_xy;

/*! \brief an Action to change where the focus is.
So that rotation happens around it. */
struct Focuser : Action {
  void Start(){
    GetProjection()->Focus(center);
  };
} focuser;



/*! \brief An action to resize the sphere.
 */
struct SphereSizer : Action {
  
  int x_old, y_old;
  V3f center_old; float radius_old;

  void Start(){
    x_old = GetMotion()-> x;
    y_old = GetMotion()-> y;
    center_old = center;
    radius_old = radius;
  };

  void Do(){
    float center_factor = - ( (float)GetMotion() -> y - y_old ) / 100.0f;
    float radius_factor = ( (float)GetMotion() -> x - x_old ) / 100.0f;
    V3f center_new = center_old * (1.0f + center_factor);
    float radius_new = radius_old * (1.0f - radius_factor);
    if(center_new.length() > 50 && center_new.length() < 150) center = center_new;
    if(radius_new > 10 && radius_new < 30) radius = radius_new;
  };

  void End(){
    depth_correction *= (float)center.length() / (float)center_old.length();
  };
    
} sphere_sizer;

/*! \brief An base action to pick a point on the sphere.
Updates the coloring as well. Can be used in various modifying actions. */
struct PickingAction: Action {

  virtual void Modify(Surface * surf, V3f pos) = 0;

  void Start(){
    Intersection isct = IntersectRaySphere(GetMotion()->sight, center, radius);
    if(isct.hit){
      V3f pos = GetMotion()->sight.Travel(isct.distance);
      FixNormals(surf);
      Modify(&surf, pos); //Call for the modification.
      AnalyzeSurface(surf, vol);
	  MakeSurfaceLists(surf); //KDL TODO take into account properties
      surf.Invalidate();
    };
  };
};

/*! \brief Push the surface to the point.
Derived from PickingAction.  */
struct PushingAction: PickingAction {
  bool push;

  PushingAction(bool _push):push(_push){};

  virtual void Modify(Surface * surf, V3f pos){
    PushPoint(*surf, pos, push, kernel);
  };

};

/*! \brief Smooth the surface around the point.
Derived from PickingAction.  */
struct SmoothingAction: PickingAction {
  virtual void Modify(Surface * surf, V3f pos){
    SmoothSurfaceAtPoint( surf, pos, kernel);
  };
} smoother;


/*! \brief Raise the surface to the point.
Derived from PickingAction.  */
struct RaisingAction: PickingAction {
  virtual void Modify(Surface * surf, V3f pos){
    RaiseSurfaceAtPoint( surf, pos, kernel);
  };
} raiser;


/*! \brief Undo.
Uses internal undo queue.  */
struct UnPushingAction: Action {
  void Start(){
      UndoPushPoint(surf);
      FixNormals(surf);
      AnalyzeSurface(surf, vol);
      surf.Invalidate();
	  MakeSurfaceLists(surf); //KDL TODO take into account properties

  }
} unpusher;

/*! \brief Color/uncolor the surface.
 */
struct ColorSwitch: Action {
  void Start(){
    surface_type.colored = !surface_type.colored;
    surf.Invalidate();
  }
} colorer;

/*! \brief Toggle wireframe display.
 */
struct WireSwitch: Action {
  void Start(){
    surface_type.wireframe = !surface_type.wireframe;
    surf.Invalidate();
  }
} wirerer;

/*! \brief Display only part of the surface.
 */
struct LimitSwitch: Action {
  void Start(){
    surface_type.limited = !surface_type.limited;
    surf.Invalidate();
  }
} limiter;


/*! \brief Patient navigation actions.
Bound in the constructor. */
struct MainNavigation: kdl_pnv::PatientsNavigation{
  /*! \brief Place to do things when a new patient is loaded.
   */
  void Update(std::string patient){
    patient_name = patient;
    surf.Invalidate();
    printf("Loaded %s...\n", patient.c_str());
  };
};


/*! \brief Entry point. 
Nothing interesting, mostly wiring of the actions and addition of the console. */
int main(int argc, char ** argv){

  MainNavigation nav;
  nav.InitNavigation(argc, argv, &surf, &vol, &tex);

  struct : FollowingAction { void Do() { SphereMoved(); }; } updater;
  Watch(&radius, &updater); Watch(&center, &updater); Watch(&surf, &updater);

  struct MainScene: public Action {

    Console * help_message;

    void Draw(){
      KickFollowers();
      DrawSphereFunction( center, radius, 30, &tex);
      glColor4f(0.5f,0.5f,0.5f,0.5f);
      //SortSurface(&surf, GetProjection()->Z());
      DrawLineAt(patient_name, V3f(-100,-112,10), 10);
      if(show_surface){
	DrawSurface(surf, & surface_type);
      }else{
	DrawBorder(&bdr);
      };
      help_message->Draw();
    };
    MainScene(){
      help_message = NewConsole(V3f(100, 70, 10), 2.0, 40);
    };
    ~MainScene() {delete help_message;};

  } scene; 

  surface_switcher.bind('T');

  unpusher.bind('Q');
  PushingAction pusher(true); pusher.bind('W');
  PushingAction puller(false); puller.bind('E');
  smoother.bind('R');
  raiser.bind('Y');

  sphere_placer.bind('A');
  sphere_sizer.bind('S');
  sphere_tuner_xy.bind('D');
  sphere_tuner_z.bind('F');

  focuser.bind('C');

  colorer.bind('\\');
  wirerer.bind(']');
  limiter.bind('[');

  const int MAX_SCALE = 9;
  Kerneler scale[MAX_SCALE];
  for(int i = 0; i < MAX_SCALE; i++){
    scale[i].Init(4+i); scale[i].bind('1'+i);
  };

  scene.help_message->bind(GLFW_KEY_F1);

  //  scene.help_message->Start(); //Toggling it off. (bad semantics)

  scene.help_message->
    AddLine("       F1 : Toggle legend")->
    AddLine("")->
    AddLine("Looking:")->
    AddLine("    Shift : Zoom")->
    AddLine("  Control : Rotate")->
    AddLine("      Tab : Reset view")->
    AddLine("")->
    AddLine("Sphere:")->
    AddLine("      'A' : Move by pointing")->
    AddLine("      'S' : Change - ")->
    AddLine("         Left/Right : Size")->
    AddLine("            Up-Down : Distance from center")->
    AddLine("      'D' : Move in X-Y plane")->
    AddLine("      'F' : Move along Z")->
    AddLine("")->
    AddLine("Tools:")->
    AddLine("      'Q' : Undo")->
    AddLine("      'Y' : Ajust the surface to go through the point")->
    AddLine("      'W' : Push down")->
    AddLine("      'E' : Pull up")->
    AddLine("      'R' : Smooth")->
    AddLine("   '1'-'9': Change tool size")->
    AddLine("")->
    AddLine("Patients:")->
    AddLine("  Up/Down key    : Next/previous")->
    AddLine("  Left/Right key : Left/right hemisphere")->
    AddLine("           Enter : Save modifications");


  SphereMoved();

  GetScene()->run( & scene );

  return 0;
};

