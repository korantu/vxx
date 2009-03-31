/**
* 
* @file vxFollower_UT.cpp
*
* This test file is a part of VoxelBrain software.
* 
* (c) Nanyang Technological University
*
*  Author: Konstantin Levinski
*
*/

#include <gtest/gtest.h>

#include "vxFollower.h"

TEST(MAIN, FollowerInt){
  ResetFollowers();
  int A = 3;

  struct Counter: Follower { //increment when something changes
    int cnt;
    
    void Do(){cnt++;};
    Counter():cnt(0){};
  }follower;
  
  follower.Follow(&A);
 
  EXPECT_EQ( 0, follower.cnt);
  KickFollowers();
  EXPECT_EQ( 0, follower.cnt);
  A = 5; KickFollowers();
  EXPECT_EQ( 1, follower.cnt);
  KickFollowers();
  EXPECT_EQ( 1, follower.cnt);
  A = 5;
  KickFollowers();
  EXPECT_EQ( 1, follower.cnt);
  A = 6;
  KickFollowers();
  EXPECT_EQ( 2, follower.cnt);
};

TEST(MAIN, FollowerBasics){
  ResetFollowers();
  int A = 3;
  float B = 3.0f;
  V3f C(1.0f, 2.0f, 3.0f);

  struct Counter: Follower { //increment when something changes
    int cnt;
    void Do(){cnt++;};

    Counter():cnt(0){};
  }follower;

  follower.Follow(&A);
  follower.Follow(&B);
  follower.Follow(&C);

  EXPECT_EQ( 0, follower.cnt);
  
  KickFollowers();
  EXPECT_EQ( 0, follower.cnt); //Nothing changed
  
  A = 6; KickFollowers();
  EXPECT_EQ( 1, follower.cnt); //One change
 KickFollowers();
 KickFollowers();
 KickFollowers();
 KickFollowers();
 KickFollowers();

  B = 3.0001f; KickFollowers();
  EXPECT_EQ( 2, follower.cnt); //Float noticed

  /*C = V3f(1.0f,2.0f,3.0f);*/ KickFollowers();
  EXPECT_EQ( 2, follower.cnt); //Vector not changed

  C = V3f(1.01f,2.01f,2.99f); KickFollowers();
  EXPECT_EQ( 3, follower.cnt); //Vector changed

  A=6; B=2.0f; C = V3f(0,0,0);
  KickFollowers();
  EXPECT_EQ( 4, follower.cnt); //Even if everything changed, click once per kick.

};


//End of vxFollower_UT.cpp
