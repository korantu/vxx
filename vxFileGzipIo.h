/**
*  
*  @file vxFileGzipIo.h
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

class Io: public Validatable {
 public:
  //top-level
  Io(std::string);
  ~Io();

  //Technical book keeping.
  unsigned int size();
  unsigned int get_position();
  Io & set_position(unsigned int position);
  std::string content();

  Io & rewind();

  //Chained versions.
  Io & GetInt(int * result);
  Io & GetFloat(float * res);
  Io & GetShort(short * res);
  Io & GetChar(char * res);
  
  //Return versions.
  int GetInt();
  float GetFloat();
  short GetShort();
  char GetChar();

  Io & PutInt(int res);
  Io & PutFloat(float res);
  Io & PutShort(short res);
  Io & PutChar(char res);
 
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

std::string ReadFile(std::string name);
std::string ReadGzipFile(std::string name);
bool WriteFile( std::string name, std::string contents);
bool WriteGzipFile( std::string name, std::string contents);
 

#endif // __vxFileGzipIo_h__

//End of vxFileGzipIo.h
