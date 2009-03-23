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
#include "vxScene.h"

using namespace std;

/*
 Action bindings.
 */

///

///KEY-action relationship

int current_key = 0;

void InvokeAction(int id);
void FinishAction(int id);

void GLFWCALL GLFWWindowSizeCb(int,int){ };
int GLFWCALL GLFWWindowCloseCb(void){ return 1;	};
void GLFWCALL GLFWWindowRefreshCb(void){ };
void GLFWCALL GLFWMouseButtonCb(int,int){ };
void GLFWCALL GLFWMouseWheelCb(int pos){ };
void GLFWCALL GLFWCharCb(int,int){ };
void GLFWCALL GLFWMousePosCb(int x,int y){
  if(current_action)current_action->UpdateRay()->Move(x, y);
};

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

//Data methods:

Projection * Scene::get_projection(){
  return 
};

int Scene::run(Drawable & scene){
    
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

    Lighting lights( & projection_state ); //define lighting.

    bind( GLFW_KEY_LCTRL , & rotation_action );
    bind( GLFW_KEY_TAB , & homing_action );
    bind( GLFW_KEY_LSHIFT , & zooming_action );

    do //Main Loop.
      {

       	projection_state->Draw();

	glScalef(1.0f/64.0, 1.0f/64.0f, 1.0f/64.0f);
	glDisable (GL_BLEND); 
	glDisable(GL_LIGHTING);
	glClear(GL_COLOR_BUFFER_BIT | 
		GL_DEPTH_BUFFER_BIT | 
		GL_STENCIL_BUFFER_BIT);

	//reference plane
	DrawPlane(V3f(0,0,0), V3f(20,0,0), V3f(0,20,0), 5);

	lights.Draw();
	scene.Draw();
	
       	glfwSwapBuffers();
	if(current_action){
	  glfwPollEvents(); // Action is most probably changing something.
	}else{              // No action, so image is probably static.
	  glfwWaitEvents(); // Lock and wait for a new event.
	};
      }
    while(!glfwGetKey( GLFW_KEY_ESC ) &&
	  glfwGetWindowParam( GLFW_OPENED ));
 

  return true;
};

//Action implementation:

void InvokeAction(int id){
  if(current_action) return; // Another action is in progress.
  current_key = id; //remeber what key we are processing.
  if(actions.end() == actions.find(id)) return; // 
  Action * in = actions[id];
  current_action = in;
  int x, y; glfwGetMousePos( &x, &y);
  in->UpdateRay()->Init(x, y);
};

void FinishAction(int id){
  if(!current_action) return; // Nothing to finish
  if(current_key != id)  // Action is finished when the same key is released
    return;              // not some other.

  int x, y; glfwGetMousePos( &x, &y);
  current_action->UpdateRay()->Done(x, y);
  current_action = 0;
};


// End of vxScene.cpp
