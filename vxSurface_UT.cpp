/**
* 
* file vxSurface_UT.cpp
*
* This test file is a part of VoxelBrain software.
* 
* (c) yulia & kostya
*
*  Author: Konstantin Levinski
*
*/

#include <gtest/gtest.h>
#include <stdlib.h>

#include "vxScene.h"
#include "vxSurface.h"
#include "vxDrawSurface.h"
#include "vxDrawSphere.h"
#include "vxAction.h"
#include "vxOpenGlTools.h"
#include "vxLoader.h"
#include "vxTools.h"

TEST(MAIN, DISABLED_Surface){
  Surface surf;
  EXPECT_EQ(2+2, 4);
  EXPECT_TRUE(read_surface_binary(surf, "data/lh.pial"));
};


//Drawing a surface.
/*
TEST(MAIN, SurfaceSimple){

  Surface surf;
  EXPECT_TRUE(read_surface_binary(surf, "data/lh.pial"));
  
  surf.c.clear();
  
  for (unsigned int i = 0; i < surf.v.size(); ++i){
    surf.c.push_back(V3f(0.5, 0.5, 0.4));};

  struct SurfaceThing: public Drawable {
    Surface * surf;
    
    struct AnAction: Action {
    } inflater;
    
    SurfaceThing(Surface * _surf):surf(_surf){};
    void Draw(){
      DrawSurface( *surf );
    };
  } scene( & surf);

  Scene::run(scene);
}
*/
void Smooth(Surface & surf, VerticeSet where){
  Connectivity net;

  Generate(net, surf);
    
  vector<V3f> modified = surf.v;
  
  for (VerticeSet::iterator to_move = where.begin(); 
       to_move != where.end(); to_move++){
    V3f neighbours(0,0,0);
    VerticeSet a = net.find(*to_move)->second;
    //average neighbors
    for(VerticeSet::iterator v = a.begin(); v != a.end(); v++){
      neighbours+=surf.v[*v];
    };
    neighbours /= (float)a.size();
    //add them to the vertex
    modified[*to_move] = neighbours;
  };//yulia & kostya

  surf.v = modified; //put it back.


};


//Drawing a surface.
/*
TEST(MAIN, SurfaceSmoothing){

  Surface surf;
  EXPECT_TRUE(read_surface_binary(surf, "data/lh.pial"));

  //Colors...?
  for (unsigned int i = 0; i < surf.v.size(); ++i){
    surf.c.push_back(V3f(0.5, (i < 300)?0.9:0.3, 0.4));};


  VerticeSet a;
  
  for(int i = 0; i < 300; i++)if(i%50){
      a.insert(i);}
    else{
      surf.v[i] *= 1.2;
    };

  Connectivity net;
  Generate(net, surf);

  for(int i = 0; i < 2000; i++)SmoothAdvanced(surf, net, a);

  FixNormals(surf);

  struct SurfaceThing: public Drawable {
    Surface * surf;
    
    struct AnAction: Action {
    } inflater;
    
    SurfaceThing(Surface * _surf):surf(_surf){};
    void Draw(){
      DrawSurface( *surf );
    };
  } scene( & surf); 

  Scene::run(scene);
}
*/

//Drawing a surface.
/*
TEST(MAIN, SurfaceEditing){

  Surface surf;

  EXPECT_TRUE(read_surface_binary(surf, "data/lh.pial"));

  //Colors...?
  for (unsigned int i = 0; i < surf.v.size(); ++i){
    surf.c.push_back(V3f(0.5, (i == 1)?0.1:0.9, 0.4));};

  PushPoint(surf, surf.v[1]*1.2);
  FixNormals(surf);
  
  struct SurfaceThing: public Drawable {
    Surface * surf;
    
    struct AnAction: Action {
    } inflater;
    
    SurfaceThing(Surface * _surf):surf(_surf){};
    void Draw(){
      DrawSurface( *surf );
    };
  } scene( & surf); 

  Scene::run(scene);

}
*/

/*
TEST(MAIN, SurfaceWarnings){

  Surface surf;
  FastVolume vol;
  MgzLoader mri(vol);
  EXPECT_TRUE(mri.Load("data/brainmask.mgz"));
  EXPECT_TRUE(read_surface_binary(surf, "data/lh.pial"));
  
  AnalyzeSurface(surf, vol);

  struct SurfaceThing: public Drawable{
    Surface * surf;
    
    struct AnAction: Action {
    } inflater;
    
    SurfaceThing(Surface * _surf):surf(_surf){};
    void Draw(){
      DrawSurface( *surf );
    };
  } scene( & surf); 

  Scene::run(scene);

}
*/

TEST(MAIN, DISABLED_Loading){
  Surface surf;
  Surface surf2;
  int res;
  std::string contents;

  contents = ReadFile("data/lh.pial");

  EXPECT_TRUE(read_surface_binary(surf, "data/lh.pial"));
  res = system("rm data/lh_saved.pial");
  surf.tri[0].x = 1234;
  surf.v[34].x = 1234.0f;
  EXPECT_TRUE(write_surface_binary_template(&surf, "data/lh_saved.pial", contents));
  EXPECT_TRUE(read_surface_binary(surf2, "data/lh_saved.pial"));

  EXPECT_EQ(surf.v.size(), surf2.v.size());
  EXPECT_EQ(surf.tri.size(), surf2.tri.size());
  EXPECT_EQ(1234, surf2.tri[0].x);
  EXPECT_FLOAT_EQ(1234.0f, surf2.v[34].x);
  
};

TEST(MAIN, DISABLED_SortSurface){
  Surface surf;
  EXPECT_TRUE(read_surface_binary(surf, "data/lh.pial"));

  V3f direction(0,0,1);

  SortSurface(&surf, direction);

  int failures = 0;

  for(unsigned int i = 0; i < surf.tri.size()-3; i+= 3){
    V3f a = surf.v[surf.tri[i].x];
    V3f b = surf.v[surf.tri[i+3].x];
    if(a.dot(direction) < b.dot(direction))failures++;
  };
  EXPECT_EQ(0, failures);
};

struct __V3fMapper{
  virtual V3f operator() (V3f in) {return V3f(in.x, in.y, 10*sin(0.001*in.x*in.y));}; 
}strange_surface;


/*
TEST(MAIN, SurfaceGeneration){
  Surface surf;

  GenerateSurface(surf, strange_surface);

  struct SurfaceThing: public Drawable{
    Surface * surf;
    
    struct: public Action {
      Surface * surf;
      V3f intersection;
      void Do(){};
    } voider; 

    SurfaceThing(Surface * _surf):surf(_surf){};

    void Draw(){
      DrawSurface( *surf );
    };

  } scene( & surf); 

  Scene::run(scene);
}
*/


/*
TEST(MAIN, SurfaceIntersection){

  Surface surf;
  GenerateSurface(surf, strange_surface);

  struct SurfaceThing: public Drawable{
    Surface * surf;
    struct: Action {
      Surface * surf;
      V3f intersection;
      void Do(){
	Intersection goal;
	
	Ray l(motion.start, motion.end - motion.start);
	if((goal = IntersectRaySurface(motion.sight, surf)).hit){
	  V3f hi;
	  intersection = l.Travel(goal.distance, hi);
	};	
	//intersection = (motion.start + motion.end)/2;
      };
    }section; 

    SurfaceThing(Surface * _surf):surf(_surf){section.surf = surf;};



    void Draw(){
      DrawSphere(section.intersection, 10, 15).Draw();
      DrawSurface( *surf );
    };
  } scene( & surf); 

  Scene::bind(GLFW_KEY_RCTRL, & scene.section);
  Scene::run(scene);
}
*/

/*
public:
  Surface surf;
  Surface a;
  Surface b;
};

TEST_F(SurfaceTest, Header) {
  int triangles_number, vertices_number;
  Io data(ReadFile("rh.pial"));
  EXPECT_TRUE(ReadPialHeader(data, &vertices_number, &triangles_number));
  printf("%d:%d vertices:triangles numbers\n", 
	 vertices_number,
	 triangles_number);
};

/// 1 BSQ
TEST_F(SurfaceTest, IO) {
  EXPECT_EQ(surf.v.size()+surf.n.size()+surf.c.size()+surf.tri.size()+surf.seeds.size(), 0);
  EXPECT_TRUE(read_surface_binary(surf, "rh.pial"));
  EXPECT_TRUE(read_surface_binary(a, "rh.pial"));
  EXPECT_TRUE(read_surface_binary(b, "lh.pial"));
  EXPECT_TRUE(surf.v.size() > 0);
  EXPECT_TRUE(b.v.size() > 0);
  EXPECT_EQ(b.v.size(), b.n.size());
  EXPECT_EQ(surf.v.size(), surf.n.size());
  EXPECT_EQ(surf.seeds.size(), 1);
  //Ensure all normals are of unit length.
  int cnt = 0;
  for(vector<V3f>::iterator i = surf.n.begin(); i != surf.n.end(); i++){
    ASSERT_FLOAT_EQ(i->length(), 1.0) << "Vertex number " << cnt << "\n";
    cnt++;
  };

  //Load next chunk.
  EXPECT_TRUE(read_surface_binary(surf, "lh.pial"));
  EXPECT_EQ(surf.seeds.size(), 2); //Now 2 seeds are avaliable. Checking them is too involved.
  EXPECT_EQ(surf.v.size(), a.v.size() + b.v.size());
  EXPECT_EQ(surf.n.size(), a.n.size() + b.n.size());
  //Ensure all normals are still of unit length.

  cnt = 0;

  for(vector<V3f>::iterator i = surf.n.begin(); i != surf.n.end(); i++){
    EXPECT_FLOAT_EQ(i->length(), 1.0) << "Vertex number " << cnt << "\n";
    cnt++;
  };



  clear(surf);
  EXPECT_EQ(surf.v.size()+surf.n.size()+surf.c.size()+surf.tri.size()+surf.seeds.size(), 0);
};

TEST(Connectivity, Link){
  Connectivity net;
  BiLink(net, 1, 2);
  BiLink(net, 1, 3);
  ASSERT_EQ(1, net[2].size());
  ASSERT_EQ(2, net[1].size());
  ASSERT_EQ(1, net[3].size());
};

TEST(MAIN, Connectivity){
  Connectivity net;
  Connectivity net_wide;
  VerticeSet result; result.insert(0);
  VerticeSet result_wide; result_wide.insert(0);

  //Linking up a couple of degenerative networks.

  for(int i = 0; i < 10; i++){
    BiLink(net, i, i+1);
    BiLink(net_wide, 0, i);
  };

  Propagate(net, result, 1);
  Propagate(net_wide, result_wide, 1);
  EXPECT_EQ(2, result.size());
  EXPECT_EQ(10, result_wide.size());
  Propagate(net, result, 3);
  EXPECT_EQ(5, result.size());  
};
*/



TEST(MAIN, DISABLED_RealMesh){
  Surface surf;
  Connectivity net; 

  EXPECT_TRUE(read_surface_binary(surf, "data/surf/lh.pial"));


struct PP : Propagator {
  int step;

  PP():step(0){};
    virtual void Step(int _step, int sample){
      if(step < _step){step = _step;};
  } } max_finder;


  Generate(net, surf);

  EXPECT_EQ(net.size(), surf.v.size());

  int max_connectivity = 0;

  for(unsigned int i = 0; i < net.size(); i++){
    if(net[i].size() > (size_t)max_connectivity)
      max_connectivity = net[i].size();
  };
  
  EXPECT_EQ(max_connectivity, 17);

  VerticeSet test; test.insert(3); //Let's say; start with the third point;


  Propagate(net, test, 1); //Do one step; 
  EXPECT_EQ(7, (int)test.size());


  Propagate(net, test, 100, &max_finder); //Do lots of steps; 
  EXPECT_EQ(104927, (int)test.size());

  EXPECT_EQ(max_finder.step, 210);
};


TEST (SurfaceRaster, DISABLED_Surface ){

  Surface surf;
  FastVolume vol;

  std::cout << "Go!";

  EXPECT_TRUE(read_surface_binary(surf, "data/lh.pial"));
  
  TIME( for(vector<V3i>::const_iterator i = surf.tri.begin(); i != surf.tri.end(); i++){
      V3f a = FastVolume::FromSurface( surf.v[i->x] );
      V3f b = FastVolume::FromSurface( surf.v[i->y] );
      V3f c = FastVolume::FromSurface( surf.v[i->z] );
      vol.RasterizeTriangle(a,b,c,1);
    }; ,"Rasterize surface");
 
};


TEST( RasterizeSurface, DISABLED_FastVolume ) {
  FastVolume m; //A set of bitmasks; eight; 
  Surface surf;
  MgzLoader mri(m);
  EXPECT_TRUE(mri.Load("data/brainmask.mgz"));
  EXPECT_TRUE(read_surface_binary(surf, "data/lh.pial"));

  TIME( RasterizeSurface( m, surf, 3) , "Offical raster" );

  //  int FloodFill ( const V3f & start, unsigned int plane,
  //		  unsigned int border );

}


TEST( RasterizeSurfaceN, DISABLED_FastVolume ) {

  FastVolume m; //A set of bitmasks; eight; 
  Surface surf;
  MgzLoader mri(m);
  EXPECT_TRUE(mri.Load("data/brainmask.mgz"));
  EXPECT_TRUE(read_surface_binary(surf, "data/lh.pial"));

  TIME( RasterizeSurface( m, surf, 3) , "Offical raster" );

  int cnt = 0;
  for(int i = 0; i < m.max; i++){
    if(0 != m._mask[i])cnt++;
  };
  std::cout << "Set " << cnt << " points.\n";

  //  int FloodFill ( const V3f & start, unsigned int plane,
  //		  unsigned int border );

}

//End of vxSurface_UT.cpp
