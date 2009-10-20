/**
*  
*  file vxFollower.cpp
*  
*  This source file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#include <vector>
#include <stdio.h>
#include <string.h>

#include "vxFollower.h"
#include "vxVector.h"

using namespace std;

/* Hashers */

void Followable::Invalidate(){
  hash++;
};

/**
   Base class, allows operate on followers uniformly;
*/

struct BaseFollower {
  virtual bool Changed() = 0;
  virtual void Reset() = 0;
  FollowingAction * action;  
};

/**
   Basic follower is a template for a usable follower,
   parameter is the type being followed;

   Watch() to set the focus.
 */

template<class T>
struct BasicFollower: BaseFollower {
  unsigned int hash;
  T * data;

  virtual bool Changed(){
    return hash != GetHash(data);};
  virtual void Reset(){
    hash = GetHash(data);
  };

  virtual BaseFollower * Watch(T * in){
    data = in;
    Reset(); hash++; //Make sure it is seen as changed.
    return this;
  };
};

/// Hashers

unsigned int GetHash(int * in){
  unsigned int res;
  memcpy(&res, in, sizeof(int));
  return res;
};

unsigned int GetHash(float * in){
  unsigned int res;
  memcpy(&res, in, sizeof(int));
  return res;
};

unsigned int GetHash(V3f * in){
  float a = in->x+in->y*1.23+in->z*0.45; //Random multiplication
  return GetHash(&a);
};

unsigned int GetHash(Followable * in){
  return in->hash;
};

///Storage
struct FollwerRecordList{
  typedef vector<BaseFollower *> list_t;
   list_t r;

  void Reset(){
    for(list_t::iterator i = r.begin(); i != r.end(); i++)delete (*i);
    r.clear();
  };

  void Kick(){
    for(list_t::iterator i = r.begin(); i != r.end(); i++){
      if((*i)->Changed()){
	(*i)->action->Do();
	(*i)->Reset();
      };
    };  

  };

  ~FollwerRecordList(){Reset();};
} __follower_record__;

///Watcher constructors
template<class T>
void WatchTemplate(T * in, FollowingAction * act){
  BasicFollower<T> * res = new BasicFollower<T>();
  res->Watch(in); res->action = act;
  __follower_record__.r.push_back(res);
};

void Watch(int * in, FollowingAction * act){ WatchTemplate<int>(in, act); };
void Watch(float * in, FollowingAction * act){ WatchTemplate<float>(in, act); };
void Watch(V3f * in, FollowingAction * act){ WatchTemplate<V3f>(in, act); };
void Watch(Followable * in, FollowingAction * act){ WatchTemplate<Followable>(in, act); };


void KickFollowers(){
  __follower_record__.Kick();
};

void ResetFollowers(){
  __follower_record__.Reset();
};

// End of vxFollower.cpp
