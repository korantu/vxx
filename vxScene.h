/**
*  
*  @file vxScene.h
\brief Simple OpenGL environment Scene.
Supply it with an Action and it will call its Draw method after each frame is set up. Standard controls are supported.
*  
*  This header file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#ifndef __vxScene_h__
#define __vxScene_h__

#include "vxVector.h" 

struct Action;

/*! \brief Scene description.
Sets up all the information and commands needed for OpenGL application to run, and then runs it calling Action every frame. */
class Scene {
 public:
  /*! \brief Main run function.
  Prepare and bind everything and construct relevant action before running it. */
  Scene * run(Action *);
};

/*! \brief Accessor to the current Scene instance.
Used to actually run it and not construct yourself. */
Scene * GetScene();

#endif // __vxScene_h__

//End of vxScene.h
