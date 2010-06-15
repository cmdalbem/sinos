// Camera.cpp
//
// Function implementation of Camera.h
//

#include <iostream>
using namespace std;
#include "Camera.h"
#include "constants.h"
#include <math.h>

#include "Object.h"

//------------------------------------------------------------ CONSTRUCTORS

Camera::Camera()
{
	cameraMode = 0;
	
	posit=0;
	
 	xpos = 5; 
	ypos = 7;
	zpos = 5;
	xrot = -45;
	yrot = 45;
		
	xrot2 = 45;
	yrot2 = 20;
	zoom = 5;
	
	xorig = 0;
	yorig = 2.95;
	zorig = 0;
	
	setPos2();
}

Camera::Camera( GLfloat newXPos, GLfloat newYPos, GLfloat newZPos,
				GLfloat newXRot, GLfloat newYRot, int newCameraMode)
{
	setXPos(newXPos);
	setYPos(newYPos);
	setZPos(newZPos);
	
	setXRot(newXRot);
	setYRot(newYRot);
	
	cameraMode = newCameraMode;
	
	xrot2 = 45;		//
	yrot2 = 20;		//
	zoom = 5;		// Default values to these elements
	posit=0;		//
	
	xorig = 0;
	yorig = 2.95;
	zorig = 0;
	
	setPos2();
}


//------------------------------------------------------------ DESTRUCTORS

Camera::~Camera() { }

//------------------------------------------------------------ GETTERS & SETTERS

void Camera::setXPos(GLfloat newValue)
{	xpos = newValue;	}
void Camera::setYPos(GLfloat newValue)
{	ypos = newValue;	}
void Camera::setZPos(GLfloat newValue)
{	zpos = newValue;	}
	
void Camera::setXRot(GLfloat newValue)
{	xrot = newValue;	}	// if newValue is greater than 360, then we take
							// the rest of the division (the module)
void Camera::setYRot(GLfloat newValue)
{	yrot = newValue;	}



const char* Camera::getMode () const
{
	switch( cameraMode )
	{
		case 0: return "Manual";
		case 1: return "Ball centered";
		case 2:	return "Cinematic";
	}
}


void Camera::setCameraMode (int mode, Object* object)
{
	if( cameraMode < NCAMERAS )
	{
		cameraMode = mode;
		if ( mode = 1 )
		{
			if( object)
			{
				xorig = object->getPosX();	// Saves the position of the object
				yorig = object->getPosY();	// the camera is looking at.
				zorig = object->getPosZ();	//
			}
			else
			{
				xorig = 0;		// If no object is passed, then the "orig"
				yorig = 2.95;	// variables receive default values.
				zorig = 0;		//
			}
			
				
		}
	}
	else
		cout << "error: CameraMode " << mode << "doesnt exists." << endl;
}

int Camera::getCameraMode () const
{
	return cameraMode;
}

void Camera::setPos2 ()
{
	xpos2 = xorig + sin(yrot2*0.05)*sin(xrot2*0.05)*zoom;
	ypos2 = yorig + cos(yrot2*0.05)*zoom;
	zpos2 = zorig + cos(xrot2*0.05)*sin(yrot2*0.05)*zoom;
}

void Camera::action1 (int movex, int movey)
{
	switch( cameraMode )
	{
		case 0:
			yrot = yrot + movey / 5.f;
			xrot = xrot + movex / 5.f;	
			break;		
		case 1:
			yrot2 = yrot2 + movey / 5.f;
			xrot2 = xrot2 + movex / 5.f;
			
			if(yrot2<0.5) yrot2=0.5;	// do not let reverse
			if(yrot2>62.5) yrot2=62.5;	// the rotation
			
			setPos2();
					
			break;
		case 2:
		
			break;
	}	
}

void Camera::action2 (int movex, int movey)
{
	switch( cameraMode )
	{
		case 0:
			float xrotrad, yrotrad;
			yrotrad = (xrot / 180 * M_PI);
			xrotrad = (yrot / 180 * M_PI);
			xpos += -movey/10.0 * float(sin(yrotrad)) ;
			ypos -= -movey/10.0 * float(sin(xrotrad)) ;	
			zpos -= -movey/10.0 * float(cos(yrotrad)) ;	
			break;
		case 1:
			zoom += movey / 5.f;
			if(zoom<0.2) zoom=0.2;		// do not let reverse the camera
			
			setPos2();
			
			break;
		case 2:
		
			break;			
	}
}


void Camera::apply()
{
	switch( getCameraMode() )
	{
		case 0:
			// controlled movement
			glRotatef(yrot,1.0,0.0,0.0);     //rotate our camera on the x-axis (left and right)
			glRotatef(xrot,0.0,1.0,0.0);     //rotate our camera on the y-axis (up and down)
			glTranslated(-xpos,-ypos,-zpos); //translate the screen to the position of our camera
			break;
		
		case 1:
		{
			// object centered camera
			gluLookAt( xpos2, ypos2, zpos2,
					   xorig, yorig, zorig,
					   0,1,0);
			break;
		}
		case 2:
			// cinematic camera
			gluLookAt( sin(posit/100)*10, 7 , cos(posit/100)*10,
					   xorig, yorig, zorig, /*ball position*/
					   0,1,0);
			break;			
	}
	
}

void Camera::nextCameraMode()
{
	setCameraMode( (getCameraMode()+1)%NCAMERAS, NULL);
}

void Camera::nextCameraMode(Object* object)
{
	setCameraMode( (getCameraMode()+1)%NCAMERAS, object);
}

/* The euclidian norma is calculated as:
 * 
 * ((x-x0)² + (y-y0)² + (z-z0)²)^1/2,
 * 
 * where x, y, and z are the position of the camera, and;
 * where x-, y0 and z0 are the position of the theObject Object.
 * 
 * Since we have differente camera modes for which we have different
 * variables used in each case, the position of the camera is composed
 * by (obviously) different variables in each "case" of the switch
 */
float Camera::distanceFromObject(Object &theObject) const
{
	switch ( getCameraMode() )
	{
		case 0:
			return sqrt(pow(xpos - theObject.getPosX(), 2) +	// use "pow" because it has
						pow(ypos - theObject.getPosY(), 2) +	// log(n) complexity instead
						pow(zpos - theObject.getPosZ(), 2));	// of n
			break;
		case 1:
			return sqrt(pow(xpos2 - theObject.getPosX(), 2) +
						pow(ypos2 - theObject.getPosY(), 2) +
						pow(zpos2 - theObject.getPosZ(), 2));
			break;
	}
}

