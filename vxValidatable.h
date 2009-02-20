/**
*  
*  @file vxValidatable.h
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

class Validatable {
 public:
  Validatable();

  virtual bool valid();
  virtual bool valid(bool is_valid);

  virtual operator bool();

 private:
  bool valid_;
};
 

#endif // __vxValidatable_h__

//End of vxValidatable.h
