// Camera.h
//
// 

#pragma once

#include <iostream>
using namespace std;
#include <string>
#include "lib/glm.h"
#include "Object.h"


#define SPIRAL_MOVEMENT_EQUATION cos(posit/100)*(100000/posit), 10 , sin(posit/100)*(100000/posit)


class Camera
{
private:
	int ncameras;
	int cameraMode;
	int posit;
	float xpos, ypos, zpos, xrot, yrot;
	float xrot2, yrot2, zoom;

public:
	//---------------------------- CONSTRUCTORS
	Camera();

	//---------------------------- DESTRUCTORS
	~Camera();
	
	//---------------------------- GETTERS & SETTERS
	void	setCameraMode (int mode);
	
	char* 	getMode() const; //name of the current camera mode
	int 	getCameraMode() const;
	
	//---------------------------- OTHER METHODS
	void 	apply(Object* object); 					// main method
	void 	action1 (int movex, int movey);
	void 	action2 (int movex, int movey);
	void 	nextCameraMode();
};
