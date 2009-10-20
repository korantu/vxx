/**
*  
*  @file vxColorLookupTable.h
	\brief Storage for predefined color schemes. 
	Schemes include yellow_hot, fire, ice, spectrum. Unused at the moment.
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


/*!
	\brief Structure with color lookup tables and names.
	Initialized statically.
	*/
struct ImageColorTable {

  /*! \brief Name of the color table.
  Initialized statically. see aColorTable.cpp. */
  const char * name;

  /*! \brief Actual lookup data for the scheme. 
  Format is [ r g b a r g b a ... 256 times ]. Alpha value is meaningless. */
  int data[1024];
};


#define clutSize 6 //Legacy

/*! \brief Collection of lookup tables.
Defined statically in the aColorTable.cpp file. */
extern const ImageColorTable aColorTable[clutSize]; 

#endif // __vxColorLookupTable_h__

//End of vxColorLookupTable.h
