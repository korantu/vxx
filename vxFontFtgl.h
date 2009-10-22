/**
*  
*  @file vxFontFtgl.h
   \brief Utilities for drawing text.
   The text is drawn using built-in font ( vxBinaryBlobs.h ) using FTGL/freetype library. DrawText simply draws the text, DrawLineAt puts a line of text at a specified position, struct Console implements an interface for dynamically displaying text.
*  
*  This header file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#ifndef __vxFontFtgl_h__
#define __vxFontFtgl_h__

#include <string>

#include "vxVector.h"
#include "vxAction.h"

void DrawText(std::string text); //!< \brief Draws a line of text. The text is drawn at the current point. 
void DrawLineAt(std::string text, V3f pos, float height); //!< \brief Draws a line of text at specified position. Uses DrawText

/*! \brief Interface for communicating text. 
New line appears below previous text. Scrolling is provided. There could be several consoles in a scene. */
struct Console: Action{
  /*! \brief Put a line of text at the console.
  Returns *this so that the calls could be chained.  Useful for displaying manuals. */
  virtual Console * AddLine(std::string) = 0;
};

/*! \brief Construct new console at the specified coordinaltes.
The Console should be deleted by user. */
Console * NewConsole(V3f pos, float height, int lines);

#endif // __vxFontFtgl_h__

//End of vxFontFtgl.h
