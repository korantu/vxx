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

bool show_surface = true;
V3f center(0,0,0); //Where the center of the crrossecting object is located.
float radius = 30; //Its radius.
float depth_correction = 0.8f; //How deep it is; 1 - on the surface, 0 - in the center.

int kernel = 6;

Surface surf; 
FastVolume vol;
Textured tex;
BorderLine bdr;

void UpdateBorder(){
  bdr.clear();
  GetBorderLine(&surf, center, radius, &bdr);
};

std::string patient_name = "None loaded.";

struct Kerneler : Action {
  int val;
  void Init(int _val){val = _val;};
  void Start(){kernel = val;};
};

struct : Action {
  void Start(){ show_surface = !show_surface; };
} surface_switcher;

struct : Action {
  
  void Do(){
    Intersection isct = IntersectRaySurface(GetMotion()->sight, &surf);
    if(isct.hit){
      center = GetMotion()->sight.Travel(isct.distance);
      center *= depth_correction;
      UpdateBorder();
    };
    
  };
} sphere_placer;

//Moving projection along the axis.
struct : Action {
  void Do(){
    center += GetProjection()->Z() * ((float)GetMotion()->dy / 4.0); 
    UpdateBorder();
  };
} sphere_tuner_z;


struct : Action {
  void Do(){
    center += GetProjection()->X() * ((float)GetMotion()->dx / 2.0); 
    center += GetProjection()->Y() * ((float)GetMotion()->dy / -2.0); 
    UpdateBorder();
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
    float center_factor = - ( (float)GetMotion() -> y - y_old ) / 100.0f;
    float radius_factor = ( (float)GetMotion() -> x - x_old ) / 100.0f;
    V3f center_new = center_old * (1.0f + center_factor);
    float radius_new = radius_old * (1.0f - radius_factor);
    if(center_new.length() > 50 && center_new.length() < 150) center = center_new;
    if(radius_new > 10 && radius_new < 30) radius = radius_new;
    UpdateBorder();
  };

  void End(){
    depth_correction *= (float)center.length() / (float)center_old.length();
  };
    
} sphere_sizer;

struct PickingAction: Action {

  virtual void Modify(Surface * surf, V3f pos) = 0;

  void Start(){
    Intersection isct = IntersectRaySphere(GetMotion()->sight, center, radius);
    if(isct.hit){
      V3f pos = GetMotion()->sight.Travel(isct.distance);
      FixNormals(surf);
      Modify(&surf, pos); //Call for the modification.
      AnalyzeSurface(surf, vol);
      UpdateBorder();
    };
  };
};

struct PushingAction: PickingAction {
  bool push;

  PushingAction(bool _push):push(_push){};

  virtual void Modify(Surface * surf, V3f pos){
    PushPoint(*surf, pos, push, kernel);
  };

};

struct SmoothingAction: PickingAction {
  virtual void Modify(Surface * surf, V3f pos){
    SmoothSurfaceAtPoint( surf, pos, kernel);
  };
} smoother;



struct UnPushingAction: Action {
  void Start(){
      UndoPushPoint(surf);
      FixNormals(surf);
      AnalyzeSurface(surf, vol);
      UpdateBorder();
  }
} unpusher;

struct MainNavigation: kdl_pnv::PatientsNavigation{
  void Update(std::string patient){
    patient_name = patient;
    UpdateBorder();
    printf("Loaded %s...\n", patient.c_str());
  };
};



int main(int argc, char ** argv){

  MainNavigation nav;
  nav.InitNavigation(argc, argv, &surf, &vol, &tex);

  struct MainScene: public Action {

    Console * help_message;

    void Draw(){
      DrawSphereFunction( center, radius, 30, &tex);
      glColor4f(0.5f,0.5f,0.5f,0.5f);
      //SortSurface(&surf, GetProjection()->Z());
      DrawLineAt(patient_name, V3f(-100,-112,10), 10);
      if(show_surface){
	DrawSurface(surf);
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


  sphere_placer.bind('A');
  sphere_sizer.bind('S');
  sphere_tuner_xy.bind('D');
  sphere_tuner_z.bind('F');

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
    AddLine("      'W' : Push down")->
    AddLine("      'E' : Pull up")->
    AddLine("      'R' : Smooth")->
    AddLine("   '1'-'9': Change tool size")->
    AddLine("")->
    AddLine("Patients:")->
    AddLine("  Up/Down key    : Next/previous")->
    AddLine("  Left/Right key : Left/right hemisphere")->
    AddLine("           Enter : Save modifications");


  UpdateBorder();

  GetScene()->run( & scene );

  return 0;
};

