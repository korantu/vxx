/**
*  
*  @file vx.cpp
*  
*  This source file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#include <stdlib.h>
#include <execinfo.h>

#include "vxTools.h"

std::ostream & operator <<(std::ostream & out, V3f v){
  out << "(" << v.x << " " << v.y << " " << v.z << ") ";
  return out;
};

///! Handling of assertions.
// Options would be: 
//  - Print result and die.
//  - Bail out to a debugger.
//  - Take note of an assertion happening.
void _kdl_panic(const char * problem){
  //A good place to put debugger.
  std::cout << "KDL Assertion failed: " << problem << "\n";

  //Taken from backtrace(3) example.
  //int nptrs = 100; 
  void *buffer[100];
  
  backtrace(buffer, 100); //Got the trace.
  backtrace_symbols_fd(buffer, 10, STDOUT_FILENO); //Push the strings to stdout.
  exit(2);
};

// End of vx.cpp

