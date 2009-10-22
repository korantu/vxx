/**
*  
*  @file vxSmoothBell.h
\brief Utility file for SmoothBell function.
*  
*  This header file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#ifndef __vxSmoothBell_h__
#define __vxSmoothBell_h__

/*! \brief Smooth function f(0)=1; f(1)=0
Useful for gradual transitions. Slower transitions are achieved with f(x/slowdown) */
float SmoothBell(float x); 

#endif // __vxSmoothBell_h__

//End of vxSmoothBell.h
