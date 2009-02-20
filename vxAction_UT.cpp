/**
* 
* @file vxAction_UT.cpp
*
* This test file is a part of VoxelBrain software.
* 
* (c) Nanyang Technological University
*
*  Author: Konstantin Levinski
*
*/

#include <gtest/gtest.h>

#include "vxAction.h"

/*
 Basic lifetime of an actor.
 Start() - Do() - End()
*/
TEST(MAIN, BasicAction){

  struct TestAction: Action {
    bool started, done, ended;
    TestAction():started(false), done(false), ended(false){};
    virtual void Start(){started = true;};
    virtual void Do(){done = true;};
    virtual void End(){ended = true;};
  } test;

  test.Init( 1, 1);
  EXPECT_TRUE(test.started); // Start() called 
  EXPECT_TRUE(test.done);  // Do() called
  test.Move( 1, 2); 
  EXPECT_FALSE(test.ended); // End() called.
  test.Done( 1, 3); 
  EXPECT_TRUE(test.ended); // End() called.
};

/*
  Total DX, should add up to start-end;
 */
TEST(MAIN, BasicDeltaAction){

  struct TestDeltaAction: Action {
    int X0, Y0;
    int delta_x, delta_y;
    virtual void Start(){X0 = motion.x; Y0 = motion.y; 
      delta_x = motion.dx; delta_y = motion.dy; };
    virtual void Do(){ delta_x += motion.dx; delta_y += motion.dy; };
    virtual void End(){ delta_x += motion.dx; delta_y += motion.dy; };
  } test;

  test.Init( 1, 2);
  test.Move( 2, 3); 
  test.Move( 45, 5); 
  test.Move( 4, 3); 
  test.Move( 5, 4); 
  test.Move( 1, 0); 
  test.Move( -2, 1); 
  test.Move( 0, 0);
  EXPECT_EQ(-1, test.delta_x);
  EXPECT_EQ(-2, test.delta_y);
  EXPECT_EQ(1, test.X0);
  EXPECT_EQ(2, test.Y0);
};


//End of vxAction_UT.cpp
