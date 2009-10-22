/**
*  
*  @file vxFileGzipIo.h
\brief Wrapper for reading values from zipped file.
Includes serialization. Required to read FreeSurfer format as it is gzipped.
*  
*  This header file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#ifndef __vxFileGzipIo_h__
#define __vxFileGzipIo_h__


#include <string>

#include "vxValidatable.h"

// Copying possible; content is not shared; so prefere reference

/*! \brief Serialization class.  
Validatable to be able to signal errors without exceptions and use return meaningfully at the same time. */
class Io: public Validatable {
 public:
  //top-level
  /*! \brief Constructor initializes content using the string. 
  The same content will be written to later, so empty constructor is not needed. */
  Io(std::string);
  ~Io();

  //Technical book keeping.
  unsigned int size(); //!< \brief How many bytes are there in total. 
  unsigned int get_position(); //!< \brief Where cursor is now. 
  Io & set_position(unsigned int position); //!< \brief Set to the position. 
  std::string content(); //!< \brief Get the content directly, as a string. 

  Io & rewind(); //!< \brief set_position(0). 

  //Chained versions.
  Io & GetInt(int * result); //!< \brief Read data and update the position accordingly. Return *this for chained calls like GetInt(&width).GetInt(&height);
  Io & GetFloat(float * res); //!< \brief Read data and update the position accordingly. 
  Io & GetShort(short * res); //!< \brief Read data and update the position accordingly. 
  Io & GetChar(char * res); //!< \brief Read data and update the position accordingly. 
  
  //Return versions.
  int GetInt(); //!< \brief Get and return data, update position accordingly. 
  float GetFloat(); //!< \brief Get and return data, update position accordingly. 
  short GetShort(); //!< \brief Get and return data, update position accordingly. 
  char GetChar(); //!< \brief Get and return data, update position accordingly. 

  Io & PutInt(int res); //!< \brief Write data at the position, and return *this, update position accordingly. 
  Io & PutFloat(float res); //!< \brief Write data at the position, and return *this, update position accordingly. 
  Io & PutShort(short res); //!< \brief Write data at the position, and return *this, update position accordingly. 
  Io & PutChar(char res); //!< \brief Write data at the position, and return *this, update position accordingly. 
 
  /*! \brief Replace string in the serializer.
  Old string is lost. */
  Io & ReplaceContent(std::string new_contnet);

 private:
  std::string content_;
  int position_;
  void CopyRawData(const char * source, int amount, char * destination);
  bool EnsureBytesAvaliable(int amount);

  template<class T>
    friend Io & Write(Io & in, T *);

  template<class T>
    friend Io & Read(Io & in, T *);

};
 
//File io. Empty strings are returned if reading fails.

/*! \brief Read file named name into STL string.
The interface is simple, any error results in empty output string, which is handled when parsing. */
std::string ReadFile(std::string name);
std::string ReadGzipFile(std::string name); //!< \brief Same as ReadFile, but assumes the file is compressed. 
bool WriteFile( std::string name, std::string contents); //!< \brief Write contents to a file named name. 
bool WriteGzipFile( std::string name, std::string contents); //!< \brief Compress and write contents to a file named name. 
 

#endif // __vxFileGzipIo_h__

//End of vxFileGzipIo.h
