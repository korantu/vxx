/**
*  
*  file vxSmoothBell.cpp
*  
*  This source file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#include "vxSmoothBell.h"

float SmoothBell(float x){
	if(x<0.0f)x=-x;
	if(x>1.0f)return 0.0f;
	float square_x = x*x;
	return 2*square_x*x-3*square_x+1.0f;
};


// End of vxSmoothBell.cpp
