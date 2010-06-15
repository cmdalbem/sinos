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
	#define NCAMERAS 2
private:

	int cameraMode;
	int posit;
	GLfloat xpos,  ypos,  zpos,		// variables used in the first camera view
			xrot,  yrot,			// 
			
			xpos2, ypos2, zpos2,	// variables used in the second camera view
			xrot2, yrot2, zoom,		//
			xorig, yorig, zorig;	//

public:
	//---------------------------- CONSTRUCTORS
	Camera();
	Camera( GLfloat newXPos, GLfloat newYPos, GLfloat newZPos,
			GLfloat newXRot, GLfloat newYRot, int newCameraMode);
	//---------------------------- DESTRUCTORS
	~Camera();
	
	//---------------------------- GETTERS & SETTERS
	void	setXPos(GLfloat newValue);
	void	setYPos(GLfloat newValue);
	void	setZPos(GLfloat newValue);
	
	void	setXRot(GLfloat newValue);
	void	setYRot(GLfloat newValue);
	
	void	setCameraMode (int mode, Object* object);
	
	/* Set the position for the "number 2" variables of the camera - which
	 * are related to the "camera 2" position.
	 */
	void	setPos2();
	
	const char* 	getMode() const; //name of the current camera mode
	int 	getCameraMode() const;
	
	//---------------------------- OTHER METHODS
	void 	apply(); 					// main method
	void 	action1 (int movex, int movey);
	void 	action2 (int movex, int movey);
	

	/* If an object is passed, then it will make difference when using the
	 * "camera mode 1". The camera will center on the object passed, instead
	 * of the default value, which is the center of the principal table.
	 */
	void 	nextCameraMode();
	void 	nextCameraMode(Object* object);
	
	/* Returns the euclidian norma of the vector starting on the camera and 
	*  end on the "position" of the Object (each Object has an array which
	*  has its position).
	*/
	float	distanceFromObject(Object &theObject) const;
};
