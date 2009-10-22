/**
*  
*  @file vxSurface.h
\brief Surface-related operations.
Main structure is Surface, and different fuctions for editing, propagation and reading/writing are provided.
*  
*  This header file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#ifndef __vxSurface_h__
#define __vxSurface_h__

#include <vector>
#include <map>
#include <set>
#include <stdio.h>

#include "vxVector.h"
#include "vxFileGzipIo.h"
#include "vxFastVolume.h"
#include "vxFollower.h"
//reading a surface from a file

using namespace std;

//data structures

/*! \brief Storage for samples along normals.
This is outdated/future approach. */
struct Kern{
  float samples[5]; //!< \brief Samples. 5 is the maximum which we can afford with the performance. 
};

/*! \brief Simple Followable surface representation.
Includes set of vertices, normals, colors and an index of triangles. */
struct Surface: Followable {
  vector<V3f> n; //!< \brief Normals.
  vector<V3f> v; //!< \brief Vectors. Have to be of the same size as normals.  
  vector<V3f> c; //!< \brief Colors. Have to be of the same size as normals. Needed for analyzing surface.
  vector<V3i> tri; //!< \brief Indices for triangles.
  vector<V3f> seeds; //!< \brief Where to start propagation if needed.

  /*! \brief Offset for drawing wireframe and surface line.
  OpenGL approach did not work.  */
  V3f offset;
  
  /*! \brief Simple constructor.
  Zero offset and make sure target=NULL. */
  Surface(); 

  /*! \brief Associated FastVolume.
  Needed to determine how exactly the surface has to be loaded. */
  FastVolume * target; 
};

/*! \brief Wrapper structure for FastVolume
For historical reasons. It is possible to replace with just FastVolume itself. */
struct GlPoints {
  FastVolume vol; //!< \brief FastVolume container. 
};

/*! \brief Obtain active surfaces.
The list of the active surfaces is a singleton avaliable to any code. Make sure to call Invalidate after use. */
Surface * get_active_surfaces();

/*! \brief Unmark a region.
In the current version happens automatically as the region becomes correct. */
void unmark(Surface &, V3f where, float radius);

/*! \brief Clear the contents of the surface. 
For reuse. */
void clear(Surface &);

//int surface_tris(Surface &);
//void surface_points ( int n , int & , int & , int & );
/// sort triangles in a direction
//void surface_sort(V3f direction);

//bool read_surface(Surface & surf, std::string name); 

/*! \brief Read in surface from file.
The surface is stored in Freesurfer MGZ format. */
bool read_surface_binary(Surface & surf, std::string name); 

/*! \brief Write surface to a file using existing format.
The format is actually the contents of the original file. Assumed that vertex number remains the same. */
bool write_surface_binary_template(Surface * surf, std::string name, std::string contents);

/*! \brief Same as read_surface_binary, but use a string instead.
In case it was already read elswhere, or for testing. */
bool read_surface_binary_from_string(Surface & surf, std::string content);

/*! \brief Read header of the surface file. 
out-params provide amount of vertices and triangles. */
bool ReadPialHeader(Io & data, int * vertices, int * triangles);

/*! \brief Collection of rasterization properties.
Used when rasterizing brain srface, to specify if it needs to be filled, what values to rasterize it with, etc. */
struct RenderingTraits{
  int dead;     //!< \brief Unknown. 
  bool half;    //!< \brief Include partial occupancy voxels. 
  bool inside;  //!< \brief Fill in the inside volume of the brain. 
  bool tru;     //!< \brief Use TRU bit for rasterizaion. 
};

///initialize the color tag.
///compare outer and inner intensities
//void analyze_surface(Surface & surf, 
//		    GlPoints & pnt); //how to render

//bool read_surface_binary(Surface & surf, std::string name);
//void rasterize_surface(Surface & surf, 
//		    GlPoints & pnt,         //the point set to render 
//		    const RenderingTraits &); //how to render

// 

/*! \brief Averages all the vertices.
The result is inside the surface and can be used for flood-filling the internals of the brain. */
V3f find_center_point(const Surface & surf);

//void fill_surface(Surface & surf, GlPoints & pnt, const RenderingTraits);

/*! \brief Rasterize triangle recursively.
Subdivide one into four (midpoints), until it is less than a pixel. */
void refine_triangle(V3f & v0, V3f & v1, V3f & v2, GlPoints & pnt, V3f n, const RenderingTraits &);

typedef int Vertice; //!< \brief Type for working with mesh. Connectivity and propagation over mesh. 
typedef std::set<Vertice> VerticeSet; //!< \brief A set of vertices. Denotes neighbours of a vertice. 
typedef std::map<Vertice, VerticeSet> Connectivity; //!< \brief A mesh connectivity is a mapping between vertexes and their neighbours. Sounds like a serious overhead. 

void Link(Connectivity &, Vertice a, Vertice b); //!< \brief Establish a link between a and b within the connectivity. Helper function for two-way link. 
void BiLink(Connectivity &, Vertice a, Vertice b); //!< \brief Two-way link between points. 

/*! \brief Derive from Propagator to perform actions during a propagation over the surface.
Step has to be overwritten, some state added if needed. */
struct Propagator {
  virtual void Step(int step, int vertice) = 0; //!< \brief Step called for every new vertice. parameter step specifies the iteration in progress. 
};

/*! \brief Propagate over a surface.
Propagation goes according to the Connectivity, starting from VerticeSet, for times iteration. Propagator is used to do useful things. */ 
void Propagate(const Connectivity &, VerticeSet &, int times, Propagator * p = NULL);

//void Generate(Connectivity &, Surface *);

/*! \brief Sample FastVolume at a point and estimate the error probability.
The estimation is provided directly in RGB in the out parameter. */ 
float AnalyzePoint(const V3f & pnt, const V3f & direction, FastVolume & volume, V3f & out);
void AnalyzeSurface(Surface & surface, FastVolume & volume); //!< \brief Analyze the whole surface for errors. Use AnalyzePoint function. 

void FixNormals(Surface & surface); //!< \brief Generate or fix the normals. 

/*! \brief Functor.
Map V3f into V3f, so that GenerateSurface could come up with a defined surface derived from a flat grid. */
struct V3fMapper {
  virtual V3f operator() (V3f) = 0; //!< \brief Override to define a mapping. Seems like overengineering. 
};

/*! \brief Use V3fMapper to map a 200x200 grid to a desired surface.
Used in tests. */
void GenerateSurface(Surface & surf , V3fMapper & mapper); 

void Generate(Connectivity &, Surface & surf); //!< \brief Generate connectivity information. 

void SortSurface(Surface *, V3f direction); //!< \brief Sort triangles so that they are front to back according to the direction given. Can be used for transparency. 

/*! \brief Make sure the surface passes throu a point.
Push or pull a surface to go through a point. radius specifies the magnitude of allowed modifications to the original surface. */
void PushPoint(Surface & surf, V3f point, bool push, int radius);
void UndoPushPoint(Surface & surf); //!< \brief Undo operation. Uses internal separate undo stack.  

float NearestPointDistance(Surface * surf, V3f point); //!< \brief Find nearest point. Used in PushPoint for determining what exact point is to be pushed. 

void SmoothAdvanced(Surface & surf, Connectivity & net, VerticeSet & where); //!< \brief Smooth points in VerticeSet, connected as in Connectivity. 
void SmoothSurfaceAtPoint(Surface * surf, V3f point, int radius); //!< \brief Smooth surface around the specified point with the radius. 
void RaiseSurfaceAtPoint(Surface * surf, V3f point, int radius); //!< \brief Raise surface.

/*! \brief Rasterize Surface within a _mask, use plane bitmask.
For use in future development. If any :(  */
int RasterizeSurface (  FastVolume & vol, 
			const Surface & a,  
			unsigned int plane );



#endif // __vxSurface_h__

//End of vxSurface.h
