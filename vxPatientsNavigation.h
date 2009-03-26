/**
*  
*  @file vxPatientsNavigation.h
*  
*  This header file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#ifndef __vxPatientsNavigation_h__
#define __vxPatientsNavigation_h__

#include <string>

namespace kdl_pnv {

struct PatientsNavigation{
  void InitNavigation(int argc, char ** argv,
		 Surface *, FastVolume *, Textured *);
  virtual void Update(std::string) = 0; //Overload if you want to do anything else after loading.
};

};

#endif // __vxPatientsNavigation_h__

//End of vxPatientsNavigation.h
