// Camera.cpp
//
// Function implementation of Camera.h
//

#include <iostream>
using namespace std;
#include <math.h>

#include "Camera.h"
#include "constants.h"
#include "Object.h"

//------------------------------------------------------------ CONSTRUCTORS

Camera::Camera( int _cameraMode )
{
	setCamera(_cameraMode);
}

//------------------------------------------------------------ DESTRUCTORS

Camera::~Camera() { }

//------------------------------------------------------------ GETTERS & SETTERS
void Camera::setCamera(int _cameraMode)
{
	cameraMode = _cameraMode;
	
	posit=0;
	
 	xpos = 50; 
	ypos = 70;
	zpos = 50;
	xrot = -45;
	yrot = 45;
		
	xrot2 = 0;
	yrot2 = 20;
	zoom = 5;
	
	xorig = 0;
	yorig = 29.5;
	zorig = 0;
	
	ypos3 = 100;
	xpos3 = 0;
	zpos3 = 0;
	
	setPos2();
}

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



const char* Camera::getModeName ( int mode ) const
{
	if( mode == -1 )
		mode = cameraMode;
		
	switch( mode )
	{
		case 0: return "Manual";
		case 1: return "Ball Centered";
		case 2: return "Top View";
		case 3:	return "Cinematic";
		default: return "Unknown";
	}
}


void Camera::setMode (int mode, Object* object)
{
	if( cameraMode < NCAMERAMODES )
	{		
		cameraMode = mode;
		if ( mode = 1 )
		{
			if( object!=NULL )
			{
				xorig = object->getPosX();	// Saves the position of the object
				yorig = object->getPosY();	// the camera is looking at.
				zorig = object->getPosZ();	//
			}
			else
			{
				xorig = 0;		// If no object is passed, then the "orig"
				yorig = 29.5;	// variables receive default values.
				zorig = 0;		//
			}	
		}
	}
	else
		cout << "error: CameraMode " << mode << "doesnt exists." << endl;
}

int Camera::getMode () const
{
	return cameraMode;
}

double Camera::getPosX() const
{
	switch(cameraMode)
	{
		case 0: return xpos;
				break;
		case 1: return xpos2;
				break;
		case 2: return xpos3;
				break;
	}
}

double Camera::getPosY() const
{
	switch(cameraMode)
	{
		case 0: return ypos;
				break;
		case 1: return ypos2;
				break;
		case 2: return ypos3;
				break;
	}
}

double Camera::getPosZ() const
{
	switch(cameraMode)
	{
		case 0: return zpos;
				break;
		case 1: return zpos2;
				break;
		case 2: return zpos3;
				break;
	}
}


void Camera::setPos2 ()
{
	xpos2 = xorig + sin(yrot2*0.0175)*sin(xrot2*0.0175)*zoom;
	ypos2 = yorig + cos(yrot2*0.0175)*zoom;
	zpos2 = zorig + cos(xrot2*0.0175)*sin(yrot2*0.0175)*zoom;
}

void Camera::action1 (double movex, double movey)
/* Usually, rotating the view. */
{
	switch( cameraMode )
	{
		case 0:
			yrot = yrot + movey;
			xrot = xrot + movex;
			break;		
		case 1:
			yrot2 = yrot2 + movey;
			xrot2 = xrot2 + movex;
			
			if(yrot2<1) yrot2=1;	// don't let reverse the rotation
			if(yrot2>85) yrot2=85;		// don't let se below the table
			
			setPos2();
					
			break;
		case 2:
			xpos3 += movex;
			zpos3 += movey;
			break;
		case 3:
			break;
	}
}

void Camera::action2 (double movex, double movey)
/* Usually, zooming. */
{
	switch( cameraMode )
	{
		case 0:
			float xrotrad, yrotrad;
			yrotrad = (xrot / 180 * M_PI);
			xrotrad = (yrot / 180 * M_PI);
			xpos += -movey/2. * float(sin(yrotrad)) ;
			ypos -= -movey/2. * float(sin(xrotrad)) ;	
			zpos -= -movey/2. * float(cos(yrotrad)) ;	
			break;
		case 1:
			zoom += movey;
			if( zoom < 2 )
				zoom = 2;		// do not let reverse the camera
			
			setPos2();
			
			break;
		case 2:
			ypos3 += movey;
			if( ypos3 < 40 )
				ypos3 = 40;		// do not let reverse the camera
			break;
		case 3:
			break;
	}
}


void Camera::apply( int forceAMode )
{
	int mode;
	if( forceAMode==-1 /*default value*/ )
		mode = getMode();
	else
		mode = forceAMode;
		
	switch( mode )
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
			setPos2(); //updates relative position of the camera
			gluLookAt( xpos2, ypos2, zpos2,
					   xorig, yorig, zorig,
					   0,1,0);
			break;
		}
		case 2:
		{
			// top view
			glRotatef(90, 1.,0.,0.);
			glTranslated(xpos3,-ypos3,zpos3);
			break;
		}
		case 3:
			// this is not working.
			// cinematic camera
			gluLookAt( sin(posit/100)*10, 7 , cos(posit/100)*10,
					   xorig, yorig, zorig, /*ball position*/
					   0,1,0);
			break;			
	}
	
}

void Camera::nextMode(Object* object)
{
	setMode( (getMode()+1)%NCAMERAMODES, object);
}

