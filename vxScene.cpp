/**
*  
*  @file vxScene.cpp
*  
*  This source file is a part of VoxelBrain software.
*  
*  (c) Nanyang Technological University
*  
*  Author: Konstantin Levinski
*  
* 
*/

#include <map>
#include <stdio.h>

#include "vxOpenGlTools.h"
#include "vxLighting.h"
#include "vxDrawPlane.h"
#include "vxProjection.h"
#include "vxAction.h"
#include "vxMotion.h"
#include "vxScene.h"

using namespace std;

/*
 Action bindings.
 */

///

///KEY-action relationship

int current_key = 0;
Action * current_action = NULL;

/*
  Uniform treatment of actions.
 */
void InvokeAction(int id);
void RepeatAction(int x, int y);
void FinishAction(int id);

void GLFWCALL GLFWWindowSizeCb(int,int){ };
int GLFWCALL GLFWWindowCloseCb(void){ return 1;	};
void GLFWCALL GLFWWindowRefreshCb(void){ };
void GLFWCALL GLFWMouseButtonCb(int,int){ };
void GLFWCALL GLFWMouseWheelCb(int pos){ };
void GLFWCALL GLFWCharCb(int,int){ };
void GLFWCALL GLFWMousePosCb(int x,int y){RepeatAction(x,y);};

void GLFWCALL GLFWKeyCb(int key, int state){
  if(state  == GLFW_PRESS){
    InvokeAction(key); // Start action associated with the key.
  }else{
    FinishAction(key); // Stop action (if the current key is correct).
  };
};

void setupCallbacks(){
    glfwSetWindowSizeCallback( GLFWWindowSizeCb );
    glfwSetWindowCloseCallback( GLFWWindowCloseCb );
    glfwSetWindowRefreshCallback( GLFWWindowRefreshCb );
    glfwSetKeyCallback( GLFWKeyCb );
    glfwSetCharCallback( GLFWMousePosCb );
    glfwSetMouseButtonCallback( GLFWMouseButtonCb );
    glfwSetMousePosCallback( GLFWMousePosCb );
    glfwSetMouseWheelCallback( GLFWMouseWheelCb );
};

//Actions:

struct HomingAction: Action {
  int interactive_change;

  HomingAction(): interactive_change(0){};

  void Reset(){
    //only change every 30 clicks.
    GetProjection()->Reset ( interactive_change / 30 );
  }
  
  void Start() { Reset(); };
  
  void Do() {
    interactive_change += GetMotion()->dx + GetMotion()->dy;
    Reset();
  };  
} homing_action;

struct ZoomingAction: Action {
  int start_x, start_y;
  void Start(){
    start_x = GetMotion()->x; start_y = GetMotion()->y;
  };

  void Do(){
    Motion * m = GetMotion();
    float dx2 = (start_x- m->x)*(start_x - m->x); 
    float dy2 = (start_y- m->y)*(start_y - m->y); 
    float new_zoom = 100.0/(100.0+sqrt(dx2+dy2));
    GetProjection()->Zoom( start_x, start_y, new_zoom);
  };
} zooming_action;


struct RotationAction: Action {
  void Do(){
    GetProjection()->Rotate( GetMotion()->dy*0.01, GetMotion()->dx*0.01);
  };
} rotation_action;

Scene * Scene::run(Action * to_draw){
    
    glfwInit();
    if( !glfwOpenWindow( 500, 500, 0,0,0,0, 16,0, 
			 GLFW_WINDOW ) )
      {
        glfwTerminate();
      }
    glfwSetWindowTitle( "3D" );
   
    glfwEnable( GLFW_STICKY_KEYS );
    glfwEnable( GLFW_MOUSE_CURSOR );
    glfwDisable( GLFW_AUTO_POLL_EVENTS );
  
    setupCallbacks();

    rotation_action.bind( GLFW_KEY_LCTRL );
    homing_action.bind( GLFW_KEY_TAB );
    zooming_action.bind( GLFW_KEY_LSHIFT );

    do //Main Loop.
      {

       	GetProjection()->Draw();

	glScalef(1.0f/64.0, 1.0f/64.0f, 1.0f/64.0f);
	glDisable (GL_BLEND); 
	glDisable(GL_LIGHTING);
	glClear(GL_COLOR_BUFFER_BIT | 
		GL_DEPTH_BUFFER_BIT | 
		GL_STENCIL_BUFFER_BIT);

	//reference plane
	DrawPlane(V3f(0,0,0), V3f(20,0,0), V3f(0,20,0), 5);

	DrawLighting();	
        to_draw->Draw();
	
       	glfwSwapBuffers();
	if(current_action){
	  glfwPollEvents(); // Action is most probably changing something.
	}else{              // No action, so image is probably static.
	  glfwWaitEvents(); // Lock and wait for a new event.
	};
      }
    while(!glfwGetKey( GLFW_KEY_ESC ) &&
	  glfwGetWindowParam( GLFW_OPENED ));
 

  return this;
};

//Scene:
Scene the_scene;
Scene * GetScene(){
  return &the_scene;
};

//Action implementation:

void InvokeAction(int id){
  if(current_key != 0) return; // Another action is in progress.
  Action * in = GetAction(id);
  if(!in) return; //Don't know what to do with this action.
  current_key = id; //remeber what key we are processing.
  current_action = in;
  int x, y; glfwGetMousePos( &x, &y);
  GetMotion()->UpdateRay()->Init(x, y);
  in->Start();
  in->Do();
};

void RepeatAction(int x, int y){
  if(!current_key)return;
  Action * in = GetAction(current_key); if(!in) return; //Don't know what to do with this action.
  GetMotion()->UpdateRay()->Move(x, y);
  in->Do();
};

void FinishAction(int id){
  if(current_key != id) return; // Nothing to finish
  Action * in = GetAction(id); if(!in) return;
  int x, y; glfwGetMousePos( &x, &y);
  GetMotion()->UpdateRay()->Move( x, y);
  in->Do();
  GetMotion()->Move( x, y);
  in->End();

  current_key = 0;
};


// End of vxScene.cpp
