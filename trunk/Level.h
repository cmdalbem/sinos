// Level.h
//
// This class have one part of the game. The level constains the cameras,
// the Objects on the scene and some specific characteristics of them,
// and some refreshing functions for the camera, ball, stick, etc.
//

#pragma once


#include <iostream>
using namespace std;

#include <vector>

#include "ObjectModel.h"
#include "ObjectBall.h"
#include "ObjectStick.h"
#include "Camera.h"
#include "LightInfo.h"
#include "constants.h"
#include "utils.h"

//#include "opengl.cpp"

class Level
{
	private:
		vector<Object*> *objects;
		vector<LightInfo*> *theLights;
		
		ObjectStick *stick;
		ObjectBall	*ball;

	public:
		Camera *camera, *camera2;
	//---------------------------- CONSTRUCTORS
		Level(vector<Object*> *_objects, vector<LightInfo*> *_theLights,
			  Camera *_camera, Camera *_camera2,
			  ObjectStick *_stick, ObjectBall *_ball);
		
	//---------------------------- DESTRUCTORS
		~Level();
	
	//---------------------------- GETTERS & SETTERS
		ObjectStick* getStick();
		ObjectBall*	 getBall();

	//---------------------------- OTHER METHODS
		void updateVariables();
		void init();
		void initObjects();
		void initLights();
	
		void drawObjects();
		void drawObjects_partial();	
		void lights ();
				
};
