/**
*  
*  @file vxAction.cpp
*  
*  This source file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#include <stdio.h>

#include "vxAction.h"

///Storage for actions.

typedef map<int, Action *> __global_actions_storage__;
__global_actions_storage__  __global_actions__;

///Put an action in starage
int Action::bind(int key, Action * action){
  __global_actions__[key] = action;
  return key;
};

///Get and use it... presumably only by Scene (?)
Action * Action::get(int key){
  __global_actions_storage__::iterator i = __global_actions__.find(key);
  if(i != actions.end()){
    return i.second; 
  }else{
    return NULL;
  };
};

Action * Action::operator() (Scene * _scene) {
  scene = _scene;
  return this;
};
// End of vxAction.cpp
