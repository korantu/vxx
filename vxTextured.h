/**
*  
*  @file vxTextured.h
*  
*  This header file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#ifndef __vxTextured_h__
#define __vxTextured_h__

#include "vxValidatable.h"

class Textured: public Validatable {

 public:

  FastVolume * texturing_fastvolume;

  bool CheckTexture( Range & r );
  const V3f & SetTexture( const V3f & where );


  Textured(); 
  ~Textured();

  Range current_range;

  //bad decision
  bool force_update;

  //export to common Texture
  void * data;
  int size;

};
 

#endif // __vxTextured_h__

//End of vxTextured.h
