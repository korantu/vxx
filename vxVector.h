/**
*  
*  @file vxVector.h
\brief Vector operations.
Defined with V3 template for any 3-d vectors, used for integers as V3i and for floats as V3f.
*  
*  This header file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#ifndef __vxVector_h__
#define __vxVector_h__

#include <math.h>

#define EPSILON (0.00001f) // A random small number
#define PI (3.14159265f)
  
/*! \brief Common vector definition, will be instantiated for integers and floats.
Cross-product included. */
  template<class T>
    struct V3 {
      
      T x; //!< \brief x coordinate. 
      T y; //!< \brief y coordinate. 
      T z; //!< \brief z coordinate. 

      /*! \brief Default.
      Everything is zero. */
      V3(){};
      V3(T _x, T _y, T _z):x(_x),y(_y),z(_z){};
      //DANGER   V3(const T * in):x(in[0]),y(in[1]),z(in[2]){};
      V3(const V3 & v):x(v.x),y(v.y),z(v.z){};
      V3 & operator=(const V3 & v){ x = v.x; y = v.y; z = v.z; return *this;};
      V3 & set(T _x, T _y, T _z){ x = _x; y = _y; z = _z; return *this;};
      V3 & set(double _x, double _y, double _z){ x = (T)_x; y = (T)_y; z = (T)_z; return *this;};

      bool operator==(const V3 & v) const { return (distance2(v) < EPSILON);};
      bool operator!=(const V3 & v) const { return (distance2(v) > EPSILON);};

      //temporary var is required but more readable;
      /*! \brief Subtraction. Creates temporary V3 */ V3 operator-(const V3 & v) const  { return V3(x-v.x, y-v.y, z-v.z);};  
      /*! \brief Addition. Creates temporary V3 */ V3 operator+(const V3 & v) const { return V3(x+v.x, y+v.y, z+v.z);};
      /*! \brief Multiplication. Creates temporary V3 */ V3 operator*(float m) const { return V3((T)(x*m), (T)(y*m), (T)(z*m));};
      /*! \brief Division. Creates temporary V3 */ V3 operator/(float m) const { return V3((T)(x/m), (T)(y/m), (T)(z/m));};
	//for msvc; TODO: review
            /*! \brief Multiplication by scalar. Creates temporary V3 */ V3 operator*(double m) const { return V3((T)(x*m), (T)(y*m), (T)(z*m));};
      /*! \brief Division by scalar. Creates temporary V3 */ V3 operator/(double m) const { return V3((T)(x/m), (T)(y/m), (T)(z/m));};
      /*! \brief Multiplication by scalar. Creates temporary V3 */ V3 operator*(int m) const { return V3((T)(x*m), (T)(y*m), (T)(z*m));};
      /*! \brief Division by scalar. Creates temporary V3 */ V3 operator/(int m) const { return V3((T)(x/m), (T)(y/m), (T)(z/m));};
 
      //no temporary vars
      V3 & operator-=(const V3 & v){ x-=v.x; y-=v.y, z-= v.z; return *this;};
      V3 & operator+=(const V3 & v){ x+=v.x; y+=v.y, z+= v.z; return *this;};
      V3 & operator*=(float m){ x*=m; y*=m, z*=m; return *this;};
      V3 & operator/=(float m){ x/=m; y/=m, z/=m; return *this;};
	//for msvc; might also make sense. TODO: review
	  V3 & operator*=(double m){ x*=(T)m; y*=(T)m, z*=(T)m; return *this;};
      V3 & operator/=(double m){ x/=(T)m; y/=(T)m, z/=(T)m; return *this;};
      V3 & operator*=(int m){ x*=(T)m; y*=(T)m, z*=(T)m; return *this;};
      V3 & operator/=(int m){ x/=(T)m; y/=(T)m, z/=(T)m; return *this;};

      /*! \brief Returns squared length.
      Beneficial for performance where squared length is needed or sufficient. */
      T length2() const { return (x*x+y*y+z*z);};
      
      /*! \brief Length of the vector */ 
      float length() const { return sqrtf((float)length2());};
  
      /*! \brief Squared distance to tre vector from the current one. */ 
      T distance2(const V3 & v) const {return (v.x-x)*(v.x-x)+(v.y-y)*(v.y-y)+(v.z-z)*(v.z-z);};
      
      /*! \brief Distance to tre vector from the current one. */ 
      float distance(const V3 & v) const {return sqrtf((float)distance2());};

      /*! \brief Cross product of a and b. Result is stored in the current vector. */ 
      V3 & cross(const V3 & a, const V3 & b){ x = a.y*b.z-a.z*b.y;
                                           y = a.z*b.x-a.x*b.z;
					   z = a.x*b.y-a.y*b.x;
                                           return *this;};  

      /*! \brief Cross product of *this and a. Result is returned with a temporary var. */ 
      V3 cross(const V3 & a) const { V3 tmp; tmp.cross(*this, a); return tmp; };

#ifndef WIN32
      T min() const {
		  return (x < y)?((x < z)?x:z):((y < z)?y:z);
	  };
#else
      T minimum_component() const {
		  return (x < y)?((x < z)?x:z):((y < z)?y:z);
	  };
#endif

      /*! \brief Dot product of *this and a. */ 
      T dot(const V3 & a) const { return x*a.x+y*a.y+z*a.z;};
      
      /*! \brief Use array notation instead of x,y,z. Relies on memory layout, hackish but to my knowledge should always work. */ 
      T operator[] (int i) const {return ((const T *)(&x))[i];};
      /*! \brief Set i'th coordinate to in */
      T SetCoord (int i, T in) const {return ((T *)(&x))[i] = in;};
    };

  
typedef V3<int> V3i; //!< \brief Integer vector. Used for triangle indices and similar things. 
typedef V3<float> V3f; //!< \brief Float-point vector. Used everywhere. 

#ifndef WIN32
float min(const V3f &);
#else
float minimum_component(const V3f &);
#endif


V3f rot_x(V3f in, float r); //!< \brief Rotate in vector along x axis. Returns the rotated vector. in is not modified.
V3f rot_y(V3f in, float r); //!< \brief Rotate in vector along y axis. Returns the rotated vector. in is not modified.
V3f rot_z(V3f in, float r); //!< \brief Rotate in vector along z axis. Returns the rotated vector. in is not modified.

/*! \brief Make three vectors to be of unit length and perpendicular ot each other.
Input vectors are modified. */
void ortoNormalize(V3f &x, V3f &y, V3f &z);

/*! \brief Debug facility for vectors.
Can be used wothout << operator. First parameter is how to label the output. */
void say(char *, const V3f &);

 
#endif // __vxVector_h__

//End of vxVector.h
