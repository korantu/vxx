/**
*  
*  @file vxValidatable.h
\brief Interface to provide information about validity of a structure.
Poor-man exceptions of-sort. Derive your class from Validatable and it will be able to tell others its health.
*  
*  This header file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#ifndef __vxValidatable_h__
#define __vxValidatable_h__

/*! \brief Base for validatable classes.
Use it to provide consistency information about the object. For example, Io uses it to signal unexpected conditions. */
class Validatable {
 public:
  Validatable(); //!< \brief By default is valid. 

  virtual bool valid(); //!< \brief Returns true if valid, false otherwise. 
  virtual bool valid(bool is_valid); //!< \brief Sets the validity and returns it. 

  virtual operator bool(); //!< \brief Conversion operation. 

 private:
  bool valid_;
};
 

#endif // __vxValidatable_h__

//End of vxValidatable.h
