/**
*  
*  @file vxTextured.h
\brief Facilities to keep track of texturing.
The texture is derived from FastVolume with the help of the Textured class.
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
#include "vxFastVolume.h"
#include "vxRange.h"

/*! \brief Takes care of texturing from FastVolume.
Range keeps track of which part of volume is currently active. */
class Textured: public Validatable {

 public:

  /*! \brief Link to the FastVolume being used.
  Has to be assigned manually. */
  FastVolume * texturing_fastvolume;

  bool CheckTexture( Range & r ); //!< \brief Check if the provided Range is currently included in the texture. If not, upload it.
  const V3f & SetTexture( const V3f & where ); //!< \brief Set OpenGL texture coordinate for point where. Returns the same point for chaining. (?)


  Textured(); //!< \brief Allocate memory for the texture data. 
  ~Textured(); //!< \brief Free memory. 

  Range current_range; //!< \brief The current range for which the data is valid. 

  /*! \brief Flag to update the texture forcefully.
  Bad decision, but being used.  */
  bool force_update;

  void * data; //!< \brief Texture data. 
  int size; //!< \brief Texture data size. 

};
 

#endif // __vxTextured_h__

//End of vxTextured.h
