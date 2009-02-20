/**
*  
*  @file vxColorLookupTable.h
*  
*  This header file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#ifndef __vxColorLookupTable_h__
#define __vxColorLookupTable_h__

struct ImageColorTable {
  const char * name;
  int data[1024];
};


#define clutSize 6 //Legacy
extern const ImageColorTable aColorTable[clutSize]; 

#endif // __vxColorLookupTable_h__

//End of vxColorLookupTable.h
