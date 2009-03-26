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

bool show_surface = true;
V3f center(0,0,0); //Where the center of the crrossecting object is located.
float radius = 30; //Its radius.
float depth_correction = 0.8f; //How deep it is; 1 - on the surface, 0 - in the center.

int kernel = 0;

Surface surf; 
FastVolume vol;
Textured tex;

/*
  TODO: Implement file navigation in a separate module:
  0) Derive 
  1) Parse argv/argc
  2) Provide Actions /next/prev/save
 */
std::vector<std::string> patients;
bool hemisphere = true;
int cur_patient = 0;
std::string content;
void LoadPatient(std::string filename, bool hemisphere);
void SavePatient(std::string filename, bool hemisphere);
void LoadSavePatient(bool load){
    std::string editable = patients.size() > 0 ? patients[cur_patient % patients.size()] : "" ;
    if(load){
      clear(surf);
      LoadPatient(editable, hemisphere); 
    }else{
      SavePatient(editable, hemisphere); 
    };
};

struct : Action {
  void Start(){ show_surface = !show_surface; };
} surface_switcher;


struct PatientIncreaser : Action {
  int inc;
  PatientIncreaser(bool _inc): inc(_inc){};
  void Start(){
    cur_patient += inc?1:-1;
    LoadSavePatient(true);
  };
};

struct HemisphereChanger : Action {
  bool is_left;
  HemisphereChanger(bool _is_left):is_left(_is_left){};

  void Start(){
    if(hemisphere != is_left){
      printf("Changing to %s\n", hemisphere?"left":"right");
      hemisphere = is_left;
      LoadSavePatient(true);
    };
  };
};

struct SavingAction: Action {
  void Start(){
    printf("Saving...\n");
    LoadSavePatient(false);
    };
} saver;

void LoadPatient(std::string patient, bool left){
  //TODO - remove if done in vxDrawSphere_UT.h
  MgzLoader mri(vol);
  mri.Load(patient+"/mri/t1.mgz");

  std::string pial_name = patient + (left ? "/surf/lh.pial":"/surf/rh.pial"); 

  printf("Loading %s\n", pial_name.c_str());

  content = ReadFile ( pial_name + ".vxx" ); //always try to load our version first.
  if(!content.size())
    content = ReadFile ( pial_name ); //Failing that, use original (but save as ours later)
    
  read_surface_binary_from_string(surf, content);

  tex.texturing_fastvolume = &vol; 

  AnalyzeSurface(surf, vol);
};

void SavePatient(std::string patient, bool left){
  std::string pial_name = patient + (left?"/surf/lh.pial.vxx":"/surf/rh.pial.vxx"); 
  write_surface_binary_template(&surf, pial_name, content);
  
};


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

struct PickingAction: Action {

  virtual void Modify(Surface * surf, V3f pos) = 0;

  void Start(){
    Intersection isct = IntersectRaySphere(GetMotion()->sight, center, radius);
    if(isct.hit){
      V3f pos = GetMotion()->sight.Travel(isct.distance);
      FixNormals(surf);
      Modify(&surf, pos); //Call for the modification.
      AnalyzeSurface(surf, vol);
    };
  };
};

struct PushingAction: PickingAction {
  bool push;

  PushingAction(bool _push):push(_push){};

  virtual void Modify(Surface * surf, V3f pos){
      PushPoint(*surf, pos, push);
  };

};

struct SmoothingAction: PickingAction {
  virtual void Modify(Surface * surf, V3f pos){
    SmoothSurfaceAtPoint( surf, pos);
  };
} smoother;



struct UnPushingAction: Action {
  void Start(){
      UndoPushPoint(surf);
      FixNormals(surf);
      AnalyzeSurface(surf, vol);
  }
} unpusher;

int main(int argc, char ** argv){

  if(argc == 1)patients.push_back(std::string("")); //assume current directory.
  for(int i = 1; i < argc; i++){
    patients.push_back(std::string(argv[i]));
    printf("Preparing %s for processing.\n", argv[i]);
  };

  LoadSavePatient(true);

  struct TexturedSphere: public Action {

    void Draw(){
      DrawSphereFunction( center, radius, 30, &tex);
      glColor4f(0.5f,0.5f,0.5f,0.5f);
      //SortSurface(&surf, GetProjection()->Z());
      if(show_surface)DrawSurface(surf);
    };

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


  saver.bind(GLFW_KEY_ENTER);
  PatientIncreaser up(true); up.bind(GLFW_KEY_UP);
  PatientIncreaser down(false); down.bind(GLFW_KEY_DOWN);
  HemisphereChanger left(true); left.bind(GLFW_KEY_LEFT);
  HemisphereChanger right(false); right.bind(GLFW_KEY_RIGHT);

  GetScene()->run( & scene );

  return 0;
};

