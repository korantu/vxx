/**
*  
*  @file vxFastVolume.h
\brief Fast manipulation with volume data.
The code in this file attempts to achieve efficiency through integer manipulation. Size of $2^n$ allows to use shifts instead of additions, and one-dimensional array is easy to scan as a whole. 
*  
*  This header file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#ifndef __vxFastVolume_h__
#define __vxFastVolume_h__


#include <vector>

#include "vxVector.h"
#include "vxColor.h"

#define BDR 0x80 //active border; can have several generations
#define ZRO 0x40 //zero points, which will be removed from the resulting
#define MASK ( BDR | MSK ) //selected points
#define TRU 0x20 //known-true point; loaded externally.
#define MSK 0x10 //this is the mask itself... gotta hurt :(
#define AUX 0x08 //bit for doing various stuff.
#define HIG 0x04 //highlighting bit.
#define GEN_MAX 0x03 //ok, now only 3 generations avaliable; none really needed
#define FLAGS (0xff - GEN_MAX) //getting flags
#define GEN(X) ((X)&GEN_MAX) //what is the generation


/*! \brief Main storage for the standartized volumes. 
   The storage includes vox_t voxels, and the same amount of bit mask storage, for propagation algorithms.
The dimensions are fixed at 256x256x256. The data is represented with a single-dimensional array, with 
dx,dy,dz are integer offsets for stepping one cell to the right, down or back. 
The class is mostly (other than tests) used with FreeSurfer volumes, so you can see MgzLoader for how to load them
into a FastVolume object. */
class FastVolume {
 public:  

  
  //   Transform parameters, loaded from an mgz file verbatim.

  /*! \brief Structure used by MgzLoader to store information of coordinate transformations.
   The coordinate transformation just change x-y y-z z-x, no rotation, no resampling. */
  struct t_tr{
    int width, height, depth;
    V3f x, y, z;
    float c_r, c_a, c_s;
  }tr;

  /*! \brief Flag if re-drawing might be needed. */
  bool updated;
  
  /*! \brief Type for voxel storage. 
   The ranges differ depending on the scanner, short int is safe compromise however. */
  typedef short int t_vox; 
  
  /*! \brief Main storage.
   The size of the storage is 256x256x256 x 2 bytes, around 16 megabytes and has to be allocated dynamically. */
  t_vox * vol;
  unsigned char * mask; //!< Deprecated (?)
  unsigned char * depth; //!< Depth, for various operations.
  
  /*! \brief Storage for bit masks.
   For each voxel _mask contains respective byte, which can be considered as 8 bit planes. Each plane can be used for rasterization, diluting, protecting growth, etc.*/
  unsigned char * _mask; 

  /*! \brief Fast Volume constructor-allocator.
   All the memory has to be dynamically allocated and cleared in the constructor. No checks are done, we assume the memory iss sufficient. TODO */
  FastVolume();

  /*! \brief Destructor.
   Deletes all the buffers without any checks. */
  ~FastVolume();

  
  /*! \brief Set of points.
   The set of points is used for propagation and for seeding. This feature is not used in second version, see VX2 project instead. */
  std::vector<int> markers;

  /*! \brief Unused(?)*/
  std::vector<int> plane;

  //void add_point(V3f &in); use tool
  
  /*! \brief Propagate markers.
    Propagate into voxels no brighter than threshold, to the max amount, limiting in depth, and iterating times. (vx2) */
  void propagate(int threshold, int amount, int depth, int times);
  
  /*! \brief Propagate markers.
   Similar to propagate(), different algorithm. (vx2) */
  void propagate_spread(int threshold, int amount, int depth, int times);
  
  /*! \brief Implementation of a segmentation-fixing algorithm. 
   It traces amount of voxels ahead, if the jump is avaliable, then it propagates there regardless of the constraints. (vx2) */
  void propagate_jump(int threshold, int amount, int depth, int times);

  /*! \brief Helper function (vx2) */
  void downshift(int flags); //shift all values down;
 
  /*! \brief Apply a volume editing tool (vx2) 
   The code is deprecated, refer to the source. */
  void use_tool(int where, int which, int size);

  /*! \brief Find regions which are not connected to the main part.
   Assumes point 127x127x127 is inside, propagates from there and then clears all the unreached voxels. */
  void scan_for_disconnected_regions();

  
  /*! \brief Track the generation */
  int cur_gen;

  float half_band_size; //!< \brief Width of the similarity band.
  float band_center; //!< \brief Center of the similarity band. 
  void set_band(); //!< \brief Apply settings in half_band_size and band_center.

  bool use_scope; //!< \brief Consider scope in the propagation?
  int min_x; //!< \brief Minimum allowed x.
  int max_x; //!< \brief Maximum allowed x.
  int min_y; //!< \brief Minimum allowed y.
  int max_y; //!< \brief Maximum allowed y.
  int min_z; //!< \brief Minimum allowed z.
  int max_z; //!< \brief Maximum allowed z.
  V3f center; //!< \brief Center of the scope. Also used for view-based propagation.
  bool in_scope(int offset);
  
  static const int dx = 0x01; //!< \brief Add or subtract this amount and you are moving in the x-direction.
  static const int dy = 0x100; //!< \brief Add or subtract this amount and you are moving in the y-direction.
  static const int dz = 0x10000; //!< \brief Add or subtract this amount and you are moving in the z-direction.

  static const int dim = 0x100; //!< \brief Dimension.
  static const int max = dim*dim*dim; //!< \brief Total voxels. No boundary checks whatsoever (speed, remember). Just be careful.

  static const int n_voxels = dim*dim*dim; //!< \brief Same as max, another name.

  static const int corners[8]; //!< \brief 8 offsets for cell corners. Very convinient and fast to look throu them. { 0, dx, dy, dx+dy, dz, dx+dz, dy+dz, dx + dy + dz }; 

  static const int neighbours[26];//!< \brief 8/26 neighbourhood. First 6 neighbours for 6-connected, then the rest. One can change between them just by varying iteration range. = {dx, -dx, dy, -dy, dz, -dz};

  
  /*! \brief Calculate offset from separate coords
    Uses shifts instead of additions, should be faster. */
  static inline int getOffset(int x, int y, int z){
    return x+(y<<8)+(z<<16);
  };

  /*! \brief Offset from a vector.
  Convert to integers first. Inline for performance. */
  static inline int getOffset(const V3f & in){
    return ((int)in.x)+				\
      (((int)in.y)<<8)+				\
      (((int)in.z)<<16);
  };

  /*! \brief Sample volume at coordinates. Helper function.
  Most often it is better to use getOffset() directly with the array, as no additional safety is provided. */
  inline int SampleInt(int x, int y, int z){
    return(vol[getOffset(x,y,z)]);
  };

  /*! \brief Calculate separate coords from offset
  Use bit operations to obtain bits corresponding to coordinates. */
  static inline void getCoords(int off, int &x, int &y, int &z){
    x=off & 0xff;
    y=(off & 0xff00)>>8;
    z=(off & 0xff0000)>>16;
  };

  /*! \brief Sample volume at float points.
  Non-inline version. */ 
  float Sample(float x_in, float y_in, float z_in);
  float Sample(const V3f &); //!< \brief Sample non-inline directly from a vector. Non-inline.
  
  /*! \brief Sample volume using Surface coordinates.
  Surface coordinates are centered at 0,0,0 while volume is [0-255]x[0-255]x[0-255]. Orientation is also different. */
  float SampleCentered(float x_in, float y_in, float z_in);
  float SampleCentered(const V3f &); //!< \brief Sample volume using Surface coordinates. Uses vector directly. 

  /*! \brief Convert Surface coordinates into the volume.
  Surface coordinates are centered at 0,0,0 while volume is [0-255]x[0-255]x[0-255]. Orientation is also different. */
  static V3f FromSurface(const V3f &);


  /*! \brief Copy data from linear volume to current one.
     Takes a volume of x*y*z array and loads it into the 256*256*256 volume */
  void copy(t_vox * , int x, int y, int z);

  void reset(); //!< \brief Resets all buffers. zeromem's and .clear()'s.

  void reseed(); //!< \brief  makes sure mask buffer corresponds to the marker buffer. That is, all neigbouring points are included. (vx2)

  /*! \brief Find all points neighbouring an outside. (vx2)
   Max. value still considered outside the area of interest.
   Bad approach as there are different methods to look for surface. */
  void findSurface(std::vector<int> &, int space);

  /* Derive this class for different purposes; and use different iterator methods
    class Iterator {

  public:
      // Iterate a line; calling for every point is too expensive 
    virtual void line(int start, int finish) = 0;

  };
  */
  //center, dx, dy, pixel/width/height, buffer, zoom (default = 1) 
  // static inline void iterate(Iterator &);
  
  /*! \brief Copy a part of volume to a flat buffer, using ColorMapper.
  The area not nessesarily coplanar to main planes, used for flat crossections. (vx2) */
  void raster( V3f o, V3f dx, V3f dy, int w, int h, unsigned char * buf, ColorMapper & mapper, int zoom = 1, bool show_mask = true);

  ///undo facilities
  /// buffer for the new undo infrastructure; test it with undoing truths modifications first.

  /*! \brief Record a new voxel for further possible undo.
  The voxels are stored in an array. Each record tells what was there before and what it was changed to, providing a kind of journal, which can be played back.  */
  void record_operation(int pos, unsigned char before, unsigned char after); 
  void record_operation(int pos, unsigned char after); //!< \brief Another version of record_operation(). Does not store what was before, seems like no redo then. 
  void record_done(); //!< \brief Informs the undo queue of a campletion of an operation. Otherwise undo would only be possible by a single pixel, or the whole thing.  
  void undo(); //!< \brief Undo last action. Play back the queue until an operation marker set with record_done().
  void undo_reset(); //!< \brief Clear all undo queues. 

  void Set(int x, int y, int z, short data); //!< \brief Another setter for the main volume. 
  short Get( int x, int y, int z) const; //!< \brief Get a voxel from the main volume. 

  /* Inefficient getters and setters, for testing mostly */
  bool GetMask ( const V3f & c, unsigned int plane ) const ; //!< \brief Get a bit plane value at the vector. FastVolume coordinates.  
  void SetMask ( const V3f & c, unsigned int plane, bool val ); //!< \brief Set a bit plane value at the vector. FastVolume coordinates.  
  void SetBlock( const V3f & c, unsigned int plane, bool val ); //!< \brief Set bit plane values inside a 3x3x3 block at the vector. FastVolume coordinates.  

  /*! \brief Iterate and resterize triangle a-b-c in FastVolume coordinates.
  Rasterization is quite fast and usable for whole brain. This is TODO. */
  int RasterizeTriangle (  const V3f & a,  
			    const V3f & b,
			    const V3f & c,
			    unsigned int plane );


  /*! \brief Fills an array with actual neighbour coordinates. 
  Helper function for FloodFill. Probably could have used neibour array. */
  void fill_neighbours(int now, int * nbr);
  
  /*! \brief Fills a bitplane from the starting point untill border.
  Intended to be used with surface rasterization to get the insides of the brain. */
  int FloodFill ( const V3f & start, unsigned int plane,
		  unsigned int border );

  /*! \brief Dilutes the volume at the given plane.
  Intended to be used with insides of the brain to save pixel border. See tests for examples. */
  int Dilute ( unsigned int plance );

};
 

#endif // __vxFastVolume_h__

//End of vxFastVolume.h
