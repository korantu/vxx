/**
*  
*  @file vxSurface.cpp
*  
*  This source file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#include <algorithm>

#include "vxSurface.h"
#include "vxFileGzipIo.h"
#include "vxSmoothBell.h"

/*** global surface ***/
Surface __surfaces;

//accessor. making sure the actual storage is only defined once.

Surface * get_active_surfaces(){
  return & __surfaces;
};

void clear(Surface & in){
  in.v.clear(); in.n.clear(); in.tri.clear(); 
  in.seeds.clear();
};

/* Surface constructor */

Surface::Surface() : offset(V3f(0,0,0)) {};
  
bool WritePialHeader(Io & data, int vertices, int triangles){
  //Signature
  const char signature[3] = {0xff, 0xff, 0xfe};
  for (int i = 0 ; i < 3; ++i) data.PutChar(signature[i]);

  data.PutChar(0x0a);
  data.PutChar(0x0a);

  //Vertices
  
  data.PutInt(vertices).PutInt(triangles);

  return true;
};

bool ReadPialHeader(Io & data, int * vertices, int * triangles){

  //obviously wrong;
  if(data.size() < 5)return false;

  //check signature
  const char signature[3] = {0xff, 0xff, 0xfe};
  for (int i = 0 ; i < 3; ++i) {
    char item; data.GetChar(&item);
    if (signature[i] != item) return false;
  };

  //look for 0x0a 0x0a
  while (data.valid()) {
    char oxa_wannabe;
    data.GetChar(&oxa_wannabe);
    if (oxa_wannabe == 0x0a) { //check if another one is behind; if so - done.
      data.GetChar(&oxa_wannabe);
      if (oxa_wannabe == 0x0a) {
	break;
      };
    };
  };

  data.GetInt(vertices).GetInt(triangles);
  
  return data.valid();
  
};

/* almost as if we read, but once we verify - start writing. */
bool write_surface_binary_template(Surface * surf, std::string name, std::string contents){
  int vertices_number;     //for the number of vertices
  int triangles_number;       //for the number of triangles
  bool result = true;
  Io data(contents);
  if(!ReadPialHeader(data, &vertices_number, &triangles_number))return false;

  if(surf->v.size() != vertices_number ||
     surf->tri.size() != triangles_number ) return false; //File mismatch
  
  //ok, now write the stuff back
  //Points
  for(int i = 0; i < vertices_number; i++){
    V3f out = surf->v[i];
    out = V3f(-out.x, +out.z, +out.y); //Jumping the same hoops;
    data.PutFloat(out.x).PutFloat(out.y).PutFloat(out.z);
  };
  //Tris
  for(int i = 0; i < triangles_number; i++){
    data.PutInt(surf->tri[i].x).
         PutInt(surf->tri[i].y).
         PutInt(surf->tri[i].z);
  };
 
  if(data.valid()) WriteFile(name, data.content());

  return data.valid();
};

bool read_surface_binary_from_string(Surface & surf, std::string content){

  int vertices_number;     //for the number of vertices
  int triangles_number;       //for the number of triangles
  bool result = true;

  int start_index = surf.v.size();

  Io data(content);

  if(!ReadPialHeader(data, &vertices_number, &triangles_number))return false;

  V3f seed = V3f(0,0,0);
    //    printf("Expecting %d points and %d triangles.\n", points, tris);
    
  //reading points and pushing normals
  for(int i = 0; i < vertices_number; i++){
    V3f in;
  //int dummy;
    data.GetFloat(&in.x).GetFloat(&in.y).GetFloat(&in.z);
    in = V3f(-in.x, +in.z, +in.y);
    //    in+=V3f(128, 127, 128); //arbitrary correction.
    //  printf("%f, %f, %f\n", in.x, in.y, in.z);
    surf.v.push_back(in);
    seed += in;
    surf.n.push_back(V3f(0,0,0));
  };

  seed /= vertices_number;
  surf.seeds.push_back(seed);

  for(int i = 0; i < triangles_number; i++){
    int a, b, c;//, zero; 
    data.GetInt(&a).GetInt(&b).GetInt(&c);
    a += start_index;
    b += start_index;
    c += start_index;
    surf.tri.push_back(V3i(a,b,c));
    //    if(i < 3)printf("((%d %d %d))\n", a, b, c);
  
    V3f n; n.cross(surf.v[b]-surf.v[a], surf.v[c]-surf.v[a]);
    n /= -n.length(); //normal - outside

    surf.n[a] = surf.n[a] + n; 
    surf.n[b] = surf.n[b] + n; 
    surf.n[c] = surf.n[c] + n; 
  };

  if(!data.valid())return false;

  for(unsigned int i = 0; i < surf.n.size(); i++){
    V3f n = surf.n[i];
    n /= n.length();
    surf.n[i] = n;
  };

  printf("stor size is:%d\n", (int)surf.v.size());
  
  return true;

};

bool read_surface_binary(Surface & surf, std::string name){
  return read_surface_binary_from_string(surf, ReadFile(name));
};

V3f find_center_point(const Surface & surf)
{
  V3f res(0,0,0);
  for(vector<V3f>::const_iterator i = surf.v.begin(); i != surf.v.end(); i++)
    res += (*i);
  printf("total.result: %f,%f,%f\n", res.x, res.y, res.z);

  res /= (float)surf.v.size();

  return res;
};


/*
Recursively render a triangle;
if the triangle is larger than a pixel, split it up into 
4 smaller triangles and try again.
 */
void refine_triangle(V3f & v0, V3f & v1, V3f & v2, GlPoints & pnt, V3f n, const RenderingTraits & t)
{
  V3f v[3] = {v0, v1, v2};
  bool good; //mark what voxels are inside as good

  for(int i = 0; i < 3; i++){ ///each vertex
      
    /*     V3f vec((vtx&1)?ceil(v[i].x):floor(v[i].x), //check neighbouring voxels
	      (vtx&2)?ceil(v[i].y):floor(v[i].y),
	      (vtx&4)?ceil(v[i].z):floor(v[i].z));
    */
    
    V3f vec(v[i].x, v[i].y, v[i].z);
      V3f dir = vec-v[i];
      good = (dir.dot(n)<0);
      if(!t.half)good = true;

      int cur = pnt.vol.getOffset((int)floor(v[i].x), 
				  (int)floor(v[i].y), 
				  (int)floor(v[i].z));
           
      if(cur < 0 || cur > 255*255*255)return;
      if(!(pnt.vol.mask[cur] & TRU)){
	pnt.vol.mask[cur] |= TRU;
	if(good)pnt.vol.mask[cur] |= (t.tru?TRU:MSK);
	//	if((pnt.vol.mask[cur] & MASK) 
	//  && (pnt.vol.vol[cur] > 35))dead++; //count intersecting pixels
      };
    };
  
  //check if the refinement is needed, i.e. tris are too big ( > 0.5 voxels )
  

  if(((v0-v1).length2() > 0.1 ||
      (v1-v2).length2() > 0.1 ||
      (v2-v0).length2() > 0.1)){
    V3f o0 = (v0+v1)/2;
    V3f o1 = (v1+v2)/2;
    V3f o2 = (v2+v0)/2;
   
    //if triangles are too big, render smaller ones...
    refine_triangle( v0, o0, o2, pnt, n, t);
    refine_triangle( o0, v1, o1, pnt, n, t);
    refine_triangle( o1, v2, o2, pnt, n, t);
    refine_triangle( o0, o1, o2, pnt, n, t);
     
}; 

  
};

// weighted average of all corners 
enum lookup_type {
  LOOKUP_DEPTH,
  LOOKUP_VALUE
};

inline float interpolate_lookup(V3f v, GlPoints & pnt, lookup_type what){
  //check all the sides
  const int X = 1; //bit fields
  const int Y = 2;
  const int Z = 4;
  const float epsilon = 0.01f;

  float total_distance = 0.0f;
  float total = 0;

  //each corner
  for(int i = 0 ; i < 8; i++){
    float corner_value = 0.0f;
    //exact corner position
    V3f corner((i & X)?ceilf(v.x):floorf(v.x),
	     (i & Y)?ceilf(v.y):floorf(v.y),
	     (i & Z)?ceilf(v.z):floorf(v.z));
    
    switch(what){
    case LOOKUP_DEPTH:
      corner_value =  (float)pnt.vol.depth[pnt.vol.getOffset(
								(int)corner.x, 
								(int)corner.y, 
								(int)corner.z)];
      break;
    case LOOKUP_VALUE:
      corner_value =  (float)pnt.vol.vol[pnt.vol.getOffset(
								(int)corner.x, 
								(int)corner.y, 
								(int)corner.z)];
      break;
    };      
    //distance between the corner and the point
    float l = (v - corner).length2();
    if(l < epsilon)return corner_value;

    total += 1.0f/l*corner_value;
    total_distance += 1.0f/l;
  };

  return total / total_distance;
};

struct point_property{

  enum {
    CONFIGURATION = 0,
    DEPTH,
    CURVATURE,
    GRADIENT,
    INTENSITY,
    INTENSITY_PER_GRADIENT,
    LAST_PARAM
  };

  float param[LAST_PARAM+1];

  //getters  
  inline float depth() const {return param[DEPTH];};
  inline float configuration() const {return param[CONFIGURATION];};
  inline float curvature() const {return param[CURVATURE];};
  inline float gradient() const {return param[GRADIENT];};
  inline float intensity() const {return param[INTENSITY];};
  inline float intensity_per_gradient() const {return param[INTENSITY_PER_GRADIENT];};

  //setters
  inline void depth(float in) {param[DEPTH] = in;};
  inline void configuration(float in) { param[CONFIGURATION]=in;};
  inline void curvature(float in)  { param[CURVATURE]=in;};
  inline void gradient(float in)  {param[GRADIENT]=in;};
  inline void intensity(float in)  {param[INTENSITY]=in;};
};

struct point_set_property{
  point_property min;
  point_property max;
  
  //const float EPSILON = 0.00001; /// epsilon for properties

  //find min and max
  point_set_property(const vector<point_property> & in){
    for(int i = 0; i < point_property::LAST_PARAM; i++){
      min.param[i] = 1000.0f;
      max.param[i] = -1000.0f;
    };

    for(vector<point_property>::const_iterator cur_property = in.begin(); 
	cur_property != in.end(); 
	cur_property++){
      for(int i = 0; i < point_property::LAST_PARAM; i++){
	if(cur_property->param[i] < min.param[i])min.param[i] = cur_property->param[i];
	if(cur_property->param[i] > max.param[i])max.param[i] = cur_property->param[i];	
      };
    };
    //debug
    for(int i = 0; i < point_property::LAST_PARAM; i++){
      printf("param %d is from %f to %f\n", i, min.param[i], max.param[i]);
    };   
  };
  
  //scale all parametes in the range [0..1]
  void scale(point_property & in){
    for(int i = 0; i < point_property::LAST_PARAM; i++){
      if(max.param[i] - min.param[i] > EPSILON){
      in.param[i] = (in.param[i]-min.param[i])/(max.param[i]-min.param[i]);
      };
    };
  };
};

//const float point_set_property::EPSILON = 0.00001f;


float AnalyzePoint(const V3f & pnt, const V3f & n, FastVolume & volume, V3f & out){
  out.x = volume.SampleCentered(pnt.x+n.x, pnt.y+n.y, pnt.z+n.z);
  out.y = volume.SampleCentered(pnt.x+2*n.x, pnt.y+2*n.y, pnt.z+2*n.z);
  out.z = volume.SampleCentered(pnt.x+3*n.x, pnt.y+3*n.y, pnt.z+3*n.z);
  float m = out.min();
  out -= V3f(m, m, m);
  out /= out.length();
  return 0;

};

/*
  Iterate AnalyzePoint over all points.
 */
void AnalyzeSurface(Surface & surf, FastVolume & vol){
  surf.c.clear();
  for(int i = 0; i < surf.v.size(); i++){
    V3f c;
    AnalyzePoint(surf.v[i], surf.n[i], vol, c);
    surf.c.push_back(c);
  };
}; 

struct usual {
  V3f direction;
  Surface * surf;
  bool operator() (const V3i & a, const V3i & b){
    return surf->v[b.x].dot(direction) <
      surf->v[a.x].dot(direction);
  };
} comparator;

void Generate(Connectivity & net, Surface & surf){
  for(int i = 0; i < surf.tri.size(); i++){
    Link(net, surf.tri[i].x, surf.tri[i].y);
    Link(net, surf.tri[i].y, surf.tri[i].z);
    Link(net, surf.tri[i].z, surf.tri[i].x);
  };
};

void SortSurface(Surface * surf, V3f direction){
  comparator.direction = direction;
  comparator.surf = surf;
  std::sort(surf->tri.begin(), surf->tri.end() , comparator);
};


//returns color from point property and point set property
V3f analyze_point(const point_property & in, point_set_property & t){
  
  V3f background(0.7f, 0.7f, 0.7f);
  if(in.depth() > 0.9f) return background;


  //  V3f c(in.configuration(), in.gradient(), in.intensity());
  float alert = (in.configuration() < 0.5f || (in.configuration() > 0.5f && in.configuration() < 0.75f))?1.0f:0.0f; 
  V3f c(alert, 0.3f*in.curvature(), (in.intensity_per_gradient())/(1.0f+alert*100.0f));
   return c*(1.0f-in.depth())+ background*in.depth();
};

void analyze_surface(Surface & surf,
		     GlPoints & pnt){
  surf.c.clear(); // remove all previous analysis
  
  V3f n, v;
  float depth;
  float v0, vup, vdown;
  vector<point_property> points;


  //each vertex, determining configuration and gradient first.
  for(size_t i = 0; i < surf.v.size(); i++){
    v = surf.v[i];
    n = surf.n[i];
    n /= n.length();

    point_property cur_point;

    //can do the lookup directly, as well. no hits in quality or performance
    //assume we are at 0 level, to work properly with complete data sets.
    depth = 0.0;//interpolate_lookup(v, pnt, LOOKUP_DEPTH);
	if(depth > 255.0f)depth=0.0f;
    depth = depth/10.0f;
    cur_point.param[point_property::DEPTH] = depth;
    if(depth > 1.0){
      //do nothing, really.
      depth = 1.1f;
    }else{ //ok, shallow, makes sense to process
    v0 =   interpolate_lookup(v, pnt, LOOKUP_VALUE);
    vup =   interpolate_lookup(v+n, pnt, LOOKUP_VALUE);
    vdown =   interpolate_lookup(v-n, pnt, LOOKUP_VALUE);

	vup-=v0*0.15f;
	vdown+=v0*0.15f;
    
    if((vup < v0) && (v0 < vdown))cur_point.configuration(0.0f);
    if((vup > v0) && (v0 < vdown))cur_point.configuration(1.0f);
    if((vup < v0) && (v0 > vdown))cur_point.configuration(2.0f);
    if((vup > v0) && (v0 > vdown))cur_point.configuration(3.0f);

    cur_point.param[point_property::GRADIENT] = (fabs(vup-v0)+fabs(vdown-v0));
    cur_point.param[point_property::INTENSITY] = v0;
    cur_point.param[point_property::CURVATURE] = 0.0f;
    cur_point.param[point_property::INTENSITY_PER_GRADIENT] = cur_point.intensity()/cur_point.gradient();

    };

    points.push_back(cur_point);
    //    c = cols[determinator]/diff*intensity*(1.0-depth)+V3f(0.7f, 0.7f, 0.7f)*depth;
  }; //for(int i = 0; i < surf.v.size(); i++)
  
  //curvature
  for(size_t i = 0; i < surf.tri.size(); i++){
  
    V3i ctri = surf.tri[i];

    if(points[ctri.x].depth() > 1)continue; //do not bother with deep points
    V3f n1 = surf.n[ctri.x];
    V3f n2 = surf.n[ctri.y];
    V3f n3 = surf.n[ctri.z];
    float curv = (n1-n2).length2()+(n2-n3).length2()+(n1-n3).length2();
    //setting maximum curvature to the tri's corners
    points[ctri.x].curvature((points[ctri.x].curvature()<curv)?curv:points[ctri.x].curvature());
    points[ctri.y].curvature((points[ctri.y].curvature()<curv)?curv:points[ctri.y].curvature());
    points[ctri.z].curvature((points[ctri.z].curvature()<curv)?curv:points[ctri.z].curvature());
  };//for(int i = 0; i < .size(); i++)
  
  
  point_set_property t(points);

  for(vector<point_property>::iterator i = points.begin(); i != points.end(); i++){
    surf.c.push_back(analyze_point(*i, t)); 
  };


};

void rasterize_surface(Surface & surf, 
		    GlPoints & pnt,         //the point set to render 
		       const RenderingTraits & t) //how to render
{
  //ok, now all the correct points in tri.
  //read tris now
  V3f center;
  //V3f m[3];
  //  std::vector<int> tristor; //triangle storage

  if(surf.v.size() < 3)return; ///not enough triangles

  //loop trough every triangle and refine it.
  for(vector<V3i>::const_iterator i = surf.tri.begin(); 
      i != surf.tri.end(); i++){

    V3f a(surf.v[i->x]+surf.offset);
    V3f b(surf.v[i->y]+surf.offset);
    V3f c(surf.v[i->z]+surf.offset);
    /*
    printf("Rendering...\n");
    for(int h = 0; h < 3; h++)
      printf("a:%f, b:%f, c:%f\n", m[h].x, m[h].y, m[h].z);
    */
    refine_triangle(a, b, c, pnt,  surf.n[i->x], t); 
  };
  
  if(t.inside){
  printf("Trying to fill it\n");
  center = find_center_point(surf); //this is the average;
  printf("The seed is %f %f %f; %d in total;  so what? \n", center.x, center.y, center.z, (int)surf.v.size());
   int cur = pnt.vol.getOffset((int)center.x, (int)center.y, (int)center.z);
   pnt.vol.mask[cur] |= BDR;
   pnt.vol.markers.push_back(cur);
  
   //pnt.vol.propagate(1000, 0, 1000, 400); //propagate everywhere from centerpoint  

  //remove truth mask as not propagating anymore
  
  if(!t.tru){
  for(int i = 0; i < 256*256*256; i++)
    pnt.vol.mask[i] -= (pnt.vol.mask[i] & TRU);
  }else{
    for(int i = 0; i < 256*256*256; i++){
      if(pnt.vol.mask[i] & MASK)pnt.vol.mask[i] |= TRU;    
      pnt.vol.mask[i] -= pnt.vol.mask[i] & MASK;
    };
  };
};
  
};
//* change color of already marked regions*//

void unmark(Surface & in, V3f where, float radius){
	for(int i = 0; i < in.v.size(); i++){
	  float dist = (in.v[i] - where).length();
	  if(dist < radius){
			in.c[i].x = 0.0;
		};
	};
};

void Link(Connectivity & net, Vertice a, Vertice b){
  Connectivity::iterator neighbours = net.find(a);
  if(net.end() == neighbours){
    VerticeSet brand_new; brand_new.insert(b);
    net[a] = brand_new;
    return;
  };
  (*neighbours).second.insert(b);
};

void BiLink(Connectivity & net, Vertice a, Vertice b){
  Link(net, a, b);
  Link(net, b, a);
};

void Propagate(const Connectivity & net, VerticeSet & current, int times, Propagator * p){
  VerticeSet border(current);
  VerticeSet new_border;
  //each step
  for(int step = 0; step < times; step++){
    //each point
    for(VerticeSet::iterator cur_a = border.begin(); cur_a != border.end(); cur_a++){
      //each neighbour of the point
      for(VerticeSet::const_iterator cur_b = net.find(*cur_a)->second.begin(); 
	  cur_b != net.find(*cur_a)->second.end(); cur_b++){
	if(current.find(*cur_b) == current.end()){
	  new_border.insert(*cur_b);
	  if (p) p->Step(step, (*cur_b));
	  current.insert(*cur_b);
	};//current.find()
      };// neighbours 
    };
    border = new_border;
    new_border.clear();
  };
};

void GenerateSurface(Surface & surf, V3fMapper & mapper){
  
  struct {
    int size;
    int index(int x, int y){return (size*(y%size)+(x%size));};
  }local;
  
  // rectangular grid:
  local.size = 100;
  for (int row = 0; row < local.size; ++row){
    for(int col  = 0; col < local.size; ++col){
      //float fi = ((float)row+(float)col)/10.0f;
      //float f_sin = sin(fi)*sin(fi)*sin(fi)*sin(fi); f_sin = - f_sin*f_sin*f_sin*20+30*cos((float)row/23)+20*cos((float)col/15); 
      surf.v.push_back(mapper(V3f(2*(row-local.size/2), 2*(col-local.size/2), 0)));
      surf.n.push_back(V3f(0,0,0));
      surf.c.push_back(V3f(0,0,0));

      //Connectivity:
      if(col + 1 < local.size && row + 1 < local.size){ 
	surf.tri.push_back(V3i(local.index(col, row),
			       local.index((col+1), (row+1)),
			       local.index(col, (row+1))));
	  
	surf.tri.push_back(V3i(local.index(col, row),
			       local.index((col+1), row),
			       local.index((col+1), (row+1))));
      };
    };
  };
    
  FixNormals(surf);
};

void FixNormals(Surface & surf){
  for(unsigned int i = 0; i < surf.tri.size(); i++){
    int a = surf.tri[i].x;
    int b = surf.tri[i].y;
    int c = surf.tri[i].z;
  
    V3f n; n.cross(surf.v[b]-surf.v[a], surf.v[c]-surf.v[a]);
    n /= -n.length(); //normal - outside

    surf.n[a] = surf.n[a] + n; 
    surf.n[b] = surf.n[b] + n; 
    surf.n[c] = surf.n[c] + n; 
  };

  for(unsigned int i = 0; i < surf.n.size(); i++){
    V3f n = surf.n[i];
    n /= n.length();
    surf.n[i] = n;
  };

};

/// Distance on a sphere. TODO dependency on normals
float DistanceSphere(V3f a, V3f b){
  return (a-b).length();
};

struct UndoPoint {
  int pos;
  V3f vec;
};

typedef std::vector<UndoPoint> action_t;
std::vector<action_t> __undo_push_point_storage__; 

void RecordUndoInformation(Surface * surf, VerticeSet * to_remember){
  if(to_remember->size() == 0) return;

  action_t to_undo;
  for(VerticeSet::iterator i = to_remember->begin(); 
      i != to_remember->end();
      i++){
    UndoPoint undo = { (*i), surf->v[(*i)] };
    to_undo.push_back( undo );
  };
  
  __undo_push_point_storage__.push_back(to_undo);
};

void UndoPushPoint(Surface & surf){
  if(__undo_push_point_storage__.size() == 0)return;
  action_t to_undo =  __undo_push_point_storage__.back();
  __undo_push_point_storage__.pop_back();
  for(action_t::iterator i = to_undo.begin(); i != to_undo.end(); i++){
    surf.v[i->pos] = i->vec;
  };
};

//Push or pull a surface to go through a point.
void PushPoint(Surface & surf, V3f point, bool push, int radius){
  action_t to_undo;
  //  float radius = 6; //modification radius
  for(int i = 0; i < surf.v.size(); i++){
    float proportion = SmoothBell ( DistanceSphere ( point, surf.v[i] ) / radius );
    if ( proportion > 0.01f ) {
      float l = surf.v[i].length();
      if(push?(l > point.length()):(l < point.length())){ //We only do push or pool, not both.
	UndoPoint undo = { i, surf.v[i]};
	to_undo.push_back( undo );
	surf.v[i] /= surf.v[i].length();
	l = proportion * point.length() + (1.0f - proportion) * l;
	surf.v[i] *= l;
      };
    };
  };
  if(to_undo.size() != 0) __undo_push_point_storage__.push_back(to_undo);
};

int NearestPointIndex(Surface * surf, V3f point){
  int nearest_index = 0;
  float nearest_distance = 10000.0f; //A couple hundred million miles.
  
  for(int i = 0; i < surf->v.size(); i++){
    float dist = (surf->v[i] - point).length();
    if( dist < nearest_distance ){
      nearest_index = i;
      nearest_distance = dist;
    };
  };
  return nearest_index;
}; 

//Uses existing connectivity.
void SmoothAdvanced(Surface & surf, Connectivity & net, VerticeSet & where){
    
  vector<V3f> modified = surf.v;
  
  for (VerticeSet::iterator to_move = where.begin(); 
       to_move != where.end(); to_move++){
    V3f neighbours(0,0,0);
    VerticeSet a = net.find(*to_move)->second;
    //average neighbors
    float min_dist = surf.v[*(a.begin())].length();
    float max_dist = surf.v[*(a.begin())].length();
    for(VerticeSet::iterator v = a.begin(); v != a.end(); v++){
      neighbours+=surf.v[*v];
      float l = surf.v[*v].length();
      min_dist = (min_dist>l) ? l : min_dist;
      max_dist = (max_dist<l) ? l : max_dist;
    };
    //neighbours /= (float)a.size();
    neighbours /= neighbours.length();
    neighbours *= (min_dist+max_dist)/2;
    //add them to the vertex
    modified[*to_move] = neighbours;
    
  };//yulia & kostya

  surf.v = modified; //put it back.
};


void SmoothSurfaceAtPoint(Surface * surf, V3f point, int radius){
  VerticeSet new_border;
  Connectivity net; 
  Generate(net, *surf);

  new_border.insert(NearestPointIndex(surf, point));
  Propagate(net, new_border, radius);

  RecordUndoInformation(surf, &new_border);

  SmoothAdvanced(*surf, net, new_border);
};


void RaiseSurfaceAtPoint(Surface * surf, V3f point, int radius){
  VerticeSet new_border;
  VerticeSet raising_border;
  Connectivity net; 
  Generate(net, *surf);

  struct Raiser : Propagator {
    Surface * surf;
    int radius;
    V3f direction;
    Raiser(Surface * _surf, int _radius):surf(_surf),radius(_radius){};

    virtual void Step(int n, int vertice){
      surf->v[vertice] += direction*SmoothBell(0.2+(float)(n)/((float)radius));
    };

  } raiser(surf, radius);

  int nearest_point_index = NearestPointIndex(surf, point);

  new_border.insert(nearest_point_index);
  Propagate(net, new_border, radius);

  RecordUndoInformation(surf, &new_border);

  //SmoothAdvanced(*surf, net, new_border);
  //SmoothAdvanced(*surf, net, new_border);
  //FixNormals(*surf);

  raising_border.insert(nearest_point_index);
  

  raiser.direction = point - surf->v[nearest_point_index];
  raiser.Step(0, nearest_point_index);
  Propagate(net, raising_border, radius, &raiser);
  
  //FixNormals(*surf);
};


// End of vxSurface.cpp
