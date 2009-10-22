/**
*  
*  @file vxLoader.h
\brief Loads and stores MGZ files using MgzLoader class.
Also includes loading code for vx2 version of software.
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


/*! \brief Loads and stores volume data in FreeSurfer .mgz format.
Provides access to the storage parameters, and can then be used to save the data exactly in the same form, only the actula volume data is replaced. Headers and parameters are left intact. */
class MgzLoader: public Validatable {
 public:

/*! \brief Voxel data type in the file. */
  enum DataType{
    UCHAR = 0,
    INT,
    LONG,
    FLOAT,
    SHORT,
    BITMAP,
    TENSOR
  };

  /*! \brief Information about the volume data in the file. */
  struct Header{
    int width;
    int height;
    int depth;
    
    DataType data_type;
  };

  /*! \brief Constructs MgzLoader and assigns corresponding FastVolume data. 
  The FastVolume data will be used when saving and loading. Ownership of the object is not transferred. */
  MgzLoader(FastVolume &);
  ~MgzLoader();

  MgzLoader & Load(std::string name); //!< \brief Load data from the file named name. Keeps the whole file uncompressed in memory, so that when Save is needed, we could just replace the part corresponding to volume data, ans leave all the headers as they were.   
  MgzLoader & Save(std::string name); //!< \brief Save FastVolume to the file named name. The previously loaded file used as template.
  
 private:
  Header header_;
  FastVolume & volume_;
  std::string file_name_;
  std::string file_content_;
};
 

/* Load volume data from file */

//_* Header defintion

/*! \brief A version of MgzLoader. Deprecated.
Used in vx2. */
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



#endif // __vxLoader_h__

//End of vxLoader.h
