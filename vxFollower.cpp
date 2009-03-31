/**
*  
*  @file vxFollower.cpp
*  
*  This source file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
*/

#include <vector>

#include "vxFollower.h"
#include "vxVector.h"

using namespace std;

/* Hashers */

unsigned int GetHash(int in){
  return (unsigned int) in;
};

unsigned int GetHash(float in){
  return * ((unsigned int *) &in );
};

unsigned int GetHash(V3f in){
  float a = in.x+in.y*1.23+in.z*0.45; //Random multiplication 
  unsigned int h = * ( (unsigned int *) &a ); //getting address.
  return h;
};

struct FollowRecord{
  Follower * follow;

  int * int_ptr;
  float * float_ptr;
  V3f * V3f_ptr;

  unsigned int hash;

  //Dispatcher
  unsigned int DispatchGetHash(){
    if(int_ptr) return GetHash(*int_ptr);
    if(float_ptr) return GetHash(*float_ptr);
    if(V3f_ptr) return GetHash(*V3f_ptr);
  };

  bool Changed(){
    return (hash != DispatchGetHash());
  };

  void Reset(){
    (hash = DispatchGetHash());
  };

  void Track(int * in){int_ptr = in; Reset();};
  void Track(float * in){float_ptr = in; Reset();};
  void Track(V3f * in){V3f_ptr = in; Reset();};

  FollowRecord(Follower * f):follow(f){
    int_ptr = NULL;
    float_ptr = NULL;
    V3f_ptr = NULL;
  };
};


vector<FollowRecord> __follow_record__;

void Follower::Follow(int * in){FollowRecord rec(this); rec.Track(in);  __follow_record__.push_back(rec);};
void Follower::Follow(float * in){FollowRecord rec(this); rec.Track(in);  __follow_record__.push_back(rec);};
void Follower::Follow(V3f * in){FollowRecord rec(this); rec.Track(in);  __follow_record__.push_back(rec);};

struct EachRecord {
  virtual void Do(vector<FollowRecord>::iterator) = 0;

  void Iterate(){
    for(vector<FollowRecord>::iterator i =  __follow_record__.begin();
	i != __follow_record__.end();
	i++)
      Do(i);
  };
};

void KickFollowers(){
  struct : EachRecord {
    void Do(vector<FollowRecord>::iterator i){ i->follow->to_kick = false; };
  } cleaner; cleaner.Iterate();

  struct : EachRecord {
    void Do(vector<FollowRecord>::iterator i){ if(i->Changed()) i->follow->to_kick = true; };
  } checker; checker.Iterate();

  struct : EachRecord {
    void Do(vector<FollowRecord>::iterator i){ 
      if(i->follow->to_kick) {
	i->follow->Do();
	i->follow->to_kick = false;
	i->Reset();
      }; 
    }; 
  } kicker; kicker.Iterate();
};

void ResetFollowers(){
  __follow_record__.clear();
};

// End of vxFollower.cpp
