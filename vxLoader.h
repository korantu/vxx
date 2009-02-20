/**
*  
*  @file vxLoader.h
*  
*  This header file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#ifndef __vxLoader_h__
#define __vxLoader_h__


#ifndef WIN32
//#include <unistd.h>
#endif

#include <stdio.h>
#include <string>
#include <vector>
#include <string.h>
#include <fcntl.h>

#include "vxFastVolume.h"
#include "vxValidatable.h"

/* Load volume data from file */

//_* Header defintion
class Loader {
public:
  Loader();
  ~Loader();

  enum Plane{
    MASK_PLANE = 0,
    VOLUME_PLANE = 1
  } cur_plane;

  //read with zlib
  int read_zlib(std::string);  
  int write_zlib(std::string); 
 
  int read(std::string); 
  int write(std::string); 
 
  void read_volume(FastVolume &); //copy volume from mgh
  void write_volume(FastVolume &); //write volume to mgh TODO:make it const...

  /* void load_mask(std::string name, unsigned char * buf);
  void save_mask(std::string name, const unsigned char * buf);
  */

  typedef char * raw;

  raw res; //binary data
  int total;  //its length; trying to avoid using std::string where not needed.

  //error for throwing
  //_ * Exception; Currently not needed.
  class Ex {
  public:
    Ex(std::string);
    Ex(const Ex &);
    Ex operator=(const Ex &);
    std::string reason;
  };
//_ * Private stuff:
private:

  //volume operations
  int get(const FastVolume & in, int x, int y, int z); 
  void set(FastVolume & in, int x, int y, int z, int val);


//raw memory copy from a string
  void memcpy_reverse(raw data, char * output, int size, int &pos);
  
  //writing the results back
  void memcpy_reverse_w(raw data, const char * input, int size, int &pos);

// raw operations:
  int get_int(const raw data, int & pos);
  short get_short(const raw data, int & pos);
  float get_float(const raw data, int & pos);
  char get_char(const raw data, int & pos);

  void set_int(raw data, int in, int & pos);
  void set_short(raw data, short in, int & pos);
  void set_float(raw data, float in, int & pos);
  void set_char(raw data, char inc, int & pos);
  void parse(raw data, FastVolume & result, bool read);

};

///Loader interface for MGZ files.
///Exposes header of original file.
///Remembers raw data and file name.

class MgzLoader: public Validatable {
 public:

  enum DataType{
    UCHAR = 0,
    INT,
    LONG,
    FLOAT,
    SHORT,
    BITMAP,
    TENSOR
  };

  struct Header{
    int width;
    int height;
    int depth;
    
    DataType data_type;
  };

  MgzLoader(FastVolume &);
  ~MgzLoader();

  MgzLoader & Load(std::string name);
  MgzLoader & Save(std::string name);
  
 private:
  Header header_;
  FastVolume & volume_;
  std::string file_name_;
  std::string file_content_;
};
 

#endif // __vxLoader_h__

//End of vxLoader.h
