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
#include "vxOpenGlTools.h"

//reading a surface from a file

using namespace std;

//data structures

struct Kern{
  float samples[5];
};

/** 
 A simple surface representation 
*/
struct Surface{
  vector<V3f> n; ///Normals
  vector<V3f> v; ///Vectors (have to be of the same size as normals;  
  vector<V3f> c; ///colors needed for analyzing surface;
  vector<V3i> tri; ///indices;
  vector<V3f> seeds; // where propagation be started should it be needed

  V3f offset;
  
  Surface(); 
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

void Propagate(const Connectivity &, VerticeSet &, int times);

float AnalyzePoint(const V3f & pnt, const V3f & direction, FastVolume & volume, V3f & out);
void AnalyzeSurface(Surface & surface, FastVolume & volume);
 

#endif // __vxSurface_h__

//End of vxSurface.h
