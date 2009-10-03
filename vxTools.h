/**
*  
*  @file vxTools.h
*  
*  This header file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#ifndef __vxTools_h__
#define __vxTools_h__

#include <iostream>
#include <sys/time.h>

#include "vxVector.h"

std::ostream & operator <<(std::ostream & out, V3f v);

#define ASSERT(X,STR) if(!(X)) { _kdl_panic((char *)STR); };

struct _kdl_timer {
  _kdl_timer(char * _name):name(_name){};
  timeval x;
  char * name;
  void start();
  void stop();
};

void _kdl_panic ( const char * problem );

#endif // __vxTools_h__

//End of vxTools.h

