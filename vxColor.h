/**
*  
*  @file vxColor.h
	\brief Deal with color scemes.
	Color sceme is a mapping from [1..255] to different colors.
*  
*  This header file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#ifndef __vxColor_h__
#define __vxColor_h__

#ifndef WIN32
#include <GL/glfw.h>
#endif
#include <vector>
#include <string>

#include "vxVector.h"


/*!
	\brief ColorMapper contains information to perform the color mapping.
	The information is stored in the unsigned char array. The mapping size is predefined from 0 to 255.
	The mapping can be constructed using fill_init() and fill() function. fill_init() defines the mapping of zero value. fill() can be used to 
	specify arbitrary number of intermediate points, with linear color interpolation between them.
	*/
struct ColorMapper{

/*!
	\brief fills in default colors
	Default colors are plain gray uniform scale. */
  ColorMapper(); 

  /*!
	\brief Copy constructor.
	For STL containers.
	*/
  ColorMapper(const ColorMapper &); //fills in default colors

  /*!
	\brief Copying operator. 
	For STL containers.
	*/
  ColorMapper & operator=(const ColorMapper &);


  /*!
	\brief Main mapping storage.
	Notice the 256 limit.
	*/
  unsigned char col[256*3];



  /*!
	\brief Dump 3 bytes to the memory specified.
	Usefullness	dubious; Possible performance gains. */
  void map(void *, int); 

  /*!
	\brief Call explicit glColor3i with the mapping.
	Convinience function. */
  void map(int); //use opengl Color3i 

  /*!
	\brief Return the mapping in the out-parameters r/g/b.
	Most direct approach. */
  void map(int & r, int & g, int &b, int); 


  /*!
	\brief Reset mapping of value 0 to color _f
	*/
  void fill_init(V3f _f);
  
  /*!
	\brief Map from previous value to value end, with interpolation to color "to".
	*/
  void fill(int end, V3f to);
  
  /*!
	\brief Fill everything from an array.
	Unimplemented?
	*/
  void fill(const int *);


  /*!
	\brief Bookkeeping - where to start next fill() from.
	*/
  int start;

  /*!
	\brief Bookkeeping - color of the previous fill().
	*/
  V3f from;

};


/*!
	\brief Init color scemes.
	*/
void color_init();

/*!
	\brief Store schemes names and respective color_types()
	Can be used after color_init() in scheme_fill()
	*/
std::vector<std::string> color_type();


/*!
	\brief Fill a color scheme with the given scheme type.
	*/
void scheme_fill(ColorMapper &, int scheme);
 

#endif // __vxColor_h__

//End of vxColor.h
