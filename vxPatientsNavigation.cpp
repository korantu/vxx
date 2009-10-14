/**
 *  
 *  @file vxPatientsNavigation.cpp
 *  
 *  This source file is a part of VoxelBrain software.
 *  
 *  (c) Nanyang Technological University
 *  
 *  Author: Konstantin Levinski
 *  
 */

#include <string>
#include <vector>
#include <stdlib.h>

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
#include "vxPatientsNavigation.h"

namespace kdl_pnv {

PatientsNavigation * __the_patients_navigation__ = NULL;

Surface * surf;
FastVolume * vol;
Textured * tex;


std::vector<std::string> patients;
bool hemisphere = true;
int cur_patient = 0;
std::string content;

void LoadPatient(std::string filename, bool hemisphere);
void SavePatient(std::string filename, bool hemisphere);

void LoadSavePatient(bool load){
  std::string editable = patients.size() > 0 ? patients[cur_patient % patients.size()] : "" ;
  if(load){
    clear(*surf);
    LoadPatient(editable, hemisphere); 
  }else{
    SavePatient(editable, hemisphere); 
  };
};

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
  MgzLoader mri(*vol);
  mri.Load(patient+"/mri/T1.mgz");

  std::string pial_name = patient + (left ? "/surf/lh.":"/surf/rh."); //add the proper extension 
  pial_name += getenv("VXX_SOURCE")?getenv("VXX_SOURCE"):"pial";

  printf("Loading %s\n", pial_name.c_str());

  content = ReadFile ( pial_name + ".vxx" ); //always try to load our version first.
  if(!content.size())
    content = ReadFile ( pial_name ); //Failing that, use original (but save as ours later)
    
  read_surface_binary_from_string(*surf, content);

  tex->texturing_fastvolume = vol; 
  tex->force_update  = true; //Force texture to be updated.
  AnalyzeSurface(*surf, *vol);

  __the_patients_navigation__->Update(patient);
};

void SavePatient(std::string patient, bool left){
  std::string pial_name = patient + (left?"/surf/lh.pial.vxx":"/surf/rh.pial.vxx"); 
  write_surface_binary_template(surf, pial_name, content);
  
};

//Actions.
PatientIncreaser up(true);
PatientIncreaser down(false);
HemisphereChanger left(true);
HemisphereChanger right(false);



  void kdl_pnv::PatientsNavigation::InitNavigation(int argc, char ** argv,
						   Surface * _surf, FastVolume * _vol, Textured * _tex){
    
    __the_patients_navigation__ = this;
    
    surf = _surf; vol = _vol; tex = _tex;
    
    //Process arguments
    if(argc == 1)patients.push_back(std::string("./")); //assume current directory.
    for(int i = 1; i < argc; i++){
      patients.push_back(std::string(argv[i]));
      printf("Preparing %s for processing.\n", argv[i]);
    };
    
    //Bind keys for navigation.
    saver.bind(GLFW_KEY_ENTER);
    up.bind(GLFW_KEY_UP);
    down.bind(GLFW_KEY_DOWN);
    left.bind(GLFW_KEY_LEFT);
    right.bind(GLFW_KEY_RIGHT);
    
    LoadSavePatient(true);
    
  };

};


// End of vxPatientsNavigation.cpp
