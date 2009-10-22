/**
*  
*  @file vxPatientsNavigation.h
\brief Parses commandline and binds actions for navigation.
Requires Surface, Fastvolume and Textured for loading. See PatiendsNavigation.

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
/*! \brief As the code was standalone, separate namespace needed to avoid name clashes.
 */
namespace kdl_pnv {

  /*! \brief Parses and installs Action s.
  Commandline consists of patient's names, which are extended with mri/T1.mgz, surf/lh.mgz as needed. */
struct PatientsNavigation{
  /*! \brief Loading happens here.
  The parameters are taken directly from main(), and parsed for the list of patients. Keybindings are LEFT-RIGHT for hemispheres, UP-DOWN for patients, ENTER saves. */
  void InitNavigation(int argc, char ** argv,
		 Surface *, FastVolume *, Textured *);
  virtual void Update(std::string) = 0; //!< \brief Callback after patient changed. Overload if you want to do anything else after loading.
};

};

#endif // __vxPatientsNavigation_h__

//End of vxPatientsNavigation.h
