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
#include <map>

#include "vxAction.h"

using namespace std;

///Storage for actions.

typedef map<int, Action *> __global_actions_storage__;
__global_actions_storage__  __global_actions__;

///Put an action in starage
Action * Action::bind(int key){
  __global_actions__[key] = this;
  return this;
};

///Get and use it... presumably only by Scene (?)
Action * GetAction(int key){
  __global_actions_storage__::iterator i = __global_actions__.find(key);
  if(i != __global_actions__.end()){
    return i->second; 
  }else{
    return NULL;
  };
};

// End of vxAction.cpp
