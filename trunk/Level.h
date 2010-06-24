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
#include <string>
#include <map>

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
		map<string,Object*> *objects;
		vector<LightInfo*> *theLights;
		
	public:
		ObjectStick stick;
		ObjectBall	ball;
		vector<ObjectBall*> balls;
		Camera *camera, *camera2;
	//---------------------------- CONSTRUCTORS
		Level(map<string,Object*> *_objects, vector<LightInfo*> *_theLights,
			  Camera *_camera, Camera *_camera2,
			  Texture *ballTex, Texture *stickTex);
		
	//---------------------------- DESTRUCTORS
		~Level();
	
	//---------------------------- GETTERS & SETTERS

	//---------------------------- OTHER METHODS
		void init();
		void initObjects();
		void initLights();
	
		void drawObjects();
		void drawObjects_partial();	
		void lights();
		void castShadows();
		
		void updateVariables();
		void testBallsCollision();
				
};
