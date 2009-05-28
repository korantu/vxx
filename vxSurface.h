/**
*  
*  @file vxSurface.h
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

struct Kern{
  float samples[5];
};

/** 
 A simple surface representation 
*/
struct Surface: Followable {
  vector<V3f> n; ///Normals
  vector<V3f> v; ///Vectors (have to be of the same size as normals;  
  vector<V3f> c; ///colors needed for analyzing surface;
  vector<V3i> tri; ///indices;
  vector<V3f> seeds; // where propagation be started should it be needed

  V3f offset;
  
  Surface(); 

  FastVolume * target; //Target fast volume, to determine how exactly the surface has to be loaded.
  //Set to zero in constructior.

};


struct GlPoints {
  FastVolume vol;
};

Surface * get_active_surfaces();
//make a radius of uniform color
void unmark(Surface &, V3f where, float radius);

void clear(Surface &);

int surface_tris(Surface &);
void surface_points ( int n , int & , int & , int & );
/// sort triangles in a direction
void surface_sort(V3f direction);

//bool read_surface(Surface & surf, std::string name); 
bool read_surface_binary(Surface & surf, std::string name); 
bool write_surface_binary_template(Surface * surf, std::string name, std::string contents);
bool read_surface_binary_from_string(Surface & surf, std::string content);


bool ReadPialHeader(Io & data, int * vertices, int * triangles);
/*
   Structure to control the rasterization process
*/
struct RenderingTraits{
  int dead;     /// 
  bool half;    /// Include partial occupancy voxels.
  bool inside;  /// Fill in the vilume
  bool tru;     /// Use tru bit for rasterizaion
  // RenderingTraits();
};

///initialize the color tag.
///compare outer and inner intensities
void analyze_surface(Surface & surf, 
		    GlPoints & pnt); //how to render

//bool read_surface_binary(Surface & surf, std::string name);
void rasterize_surface(Surface & surf, 
		    GlPoints & pnt,         //the point set to render 
		    const RenderingTraits &); //how to render

// 
V3f find_center_point(const Surface & surf);

void fill_surface(Surface & surf, GlPoints & pnt, const RenderingTraits);

///recursive refining function

void refine_triangle(V3f & v0, V3f & v1, V3f & v2, GlPoints & pnt, V3f n, const RenderingTraits &);

typedef int Vertice;
typedef std::set<Vertice> VerticeSet;
typedef std::map<Vertice, VerticeSet> Connectivity; 

void Link(Connectivity &, Vertice a, Vertice b);
void BiLink(Connectivity &, Vertice a, Vertice b);

struct Propagator {
  virtual void Step(int step, int vertice) = 0;
};

void Propagate(const Connectivity &, VerticeSet &, int times, Propagator * p = NULL);

//void Generate(Connectivity &, Surface *);

float AnalyzePoint(const V3f & pnt, const V3f & direction, FastVolume & volume, V3f & out);
void AnalyzeSurface(Surface & surface, FastVolume & volume);

 
void FixNormals(Surface & surface);

///Intersect:
struct V3fMapper {
  virtual V3f operator() (V3f) = 0; //(V3f blah);
};

void GenerateSurface(Surface & surf , V3fMapper & mapper); //mapper maps a 200x200 grid to a desired surface.

void Generate(Connectivity &, Surface & surf);

void SortSurface(Surface *, V3f direction);

void PushPoint(Surface & surf, V3f point, bool push, int radius);
void UndoPushPoint(Surface & surf);

void SmoothAdvanced(Surface & surf, Connectivity & net, VerticeSet & where);
void SmoothSurfaceAtPoint(Surface * surf, V3f point, int radius);
void RaiseSurfaceAtPoint(Surface * surf, V3f point, int radius);

#endif // __vxSurface_h__

//End of vxSurface.h
