/**
*  
*  file vx.cpp
*  
*  This source file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#include <stdlib.h>

#ifndef WIN32
#include <execinfo.h>
#endif

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
#ifndef WIN32
	//A good place to put debugger.
  std::cout << "KDL Assertion failed: " << problem << "\n";

  //Taken from backtrace(3) example.
  //int nptrs = 100; 
  void *buffer[100];
  
  backtrace(buffer, 100); //Got the trace.
  backtrace_symbols_fd(buffer, 10, STDOUT_FILENO); //Push the strings to stdout.
  exit(2);
#endif //WIN32
};

void _kdl_timer::start(){
#ifndef WIN32
	gettimeofday(&x,NULL);
#endif //WIN32
};

void _kdl_timer::stop(){
#ifndef WIN32
  timeval now; gettimeofday(&now,NULL);
  std::cout << name << " : " <<  (1.0*(now.tv_sec-x.tv_sec) + 0.000001*(now.tv_usec-x.tv_usec)) << " s.\n"; 
#endif // WIN32
};

// End of vx.cpp

