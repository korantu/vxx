/**
*  
*  file vxRange.cpp
*  
*  This source file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#include "vxRange.h"

const V3f V3_EPSILON(EPSILON, EPSILON, EPSILON);

Range::Range(){ };
Range::Range(const Range & in):min(in.min), max(in.max){ };
Range::Range(const V3f & lower, const V3f & upper):min(lower), max(upper){ };

bool GreaterThanAll(const V3f & a, const V3f & b){
  return (a.x > b.x) && (a.y > b.y) && (a.z > b.z);
};

bool LessThanAll(const V3f & a, const V3f & b){
  return (a.x < b.x) && (a.y < b.y) && (a.z < b.z);
};

bool ContainsPoint(const Range & r, const V3f & pnt){
  return (LessThanAll(r.min, pnt) && GreaterThanAll(r.max, pnt));
};

bool ContainsRange(const Range & r, const Range & in){
  return (LessThanAll(r.min, in.min) && GreaterThanAll(r.max, in.max));
};

bool IntersectsRange(const Range & r, const Range & in){
  return (GreaterThanAll(r.max, in.min) && LessThanAll(r.min, in.max));
};

Range & ExpandRange(Range & r, const V3f & v){
  for(int i = 0; i < 3; i++){
    if(r.min[i] > v[i]) r.min.SetCoord(i, v[i] - EPSILON);
    if(r.max[i] < v[i]) r.max.SetCoord(i, v[i] + EPSILON);
    };
  return r;
};



// End of vxRange.cpp
