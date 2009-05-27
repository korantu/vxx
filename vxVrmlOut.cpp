/**
*  
*  @file vxVrmlOut.cpp
*  
*  This source file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#include "vxVrmlOut.h"
#include <sstream>

std::string VrmlTemplate(std::string vertices, std::string normals, std::string colors, std::string indices){
  std::string result;

  result+= "#VRML V2.0 utf8\n\n" 
"Shape {"  
"geometry IndexedFaceSet {" 
"coord Coordinate {" 
"point [" + vertices + \
"] }\n";
  if(normals.size() > 0)
       result += "normal Normal {"
         "vector [" + normals+ "]}";
  if(colors.size() > 0)
       result+= "color Color {"
         "color [" + colors + "]}";
  
  result += "coordIndex [" + indices + "]}" 
      "appearance Appearance {"
         "material Material {"
	   "diffuseColor 1 1 1"
         "}}}";
  return result;
};


std::string VrmlFormat(vector<V3f> vertices, bool invert){
  int cnt = 1;
  std::stringstream result;
  for(vector<V3f>::iterator i = vertices.begin(); i != vertices.end();){
    cnt++;
    if(!invert)
      result << (*i).x << " " << (*i).y << " " << (*i).z ;
    else
      result << -(*i).x << " " << -(*i).y << " " << -(*i).z ;
      
    if(++i != vertices.end()) result << "," << (cnt%5?" ":"\n");    
  };
  return result.str();
};

std::string VrmlFormat(vector<V3i> indices){
  int cnt = 1; //for formatting.
  std::stringstream result;
  for(vector<V3i>::iterator i = indices.begin(); i != indices.end();){
    cnt++;
    result << (*i).x << " " << (*i).y << " " << (*i).z;
    if(++i != indices.end()) result << " -1" << (cnt%5?" ":"\n"); //Vrml convention for faces.    
  };
  return result.str();
};



// End of vxVrmlOut.cpp
