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
		
	xrot2 = 45;
	yrot2 = 20;
	zoom = 5;
	
	xorig = 0;
	yorig = 29.5;
	zorig = 0;
	
	zoomTop = 100;
	
	centerx = 0;
	centerz = 0;
	
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
				cout << "Warning: setting camera to " << getModeName() << " with object=NULL." << endl;
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

void Camera::setPos2 ()
{
	xpos2 = xorig + sin(yrot2*0.05)*sin(xrot2*0.05)*zoom;
	ypos2 = yorig + cos(yrot2*0.05)*zoom;
	zpos2 = zorig + cos(xrot2*0.05)*sin(yrot2*0.05)*zoom;
}

void Camera::action1 (int movex, int movey)
/* Usually, rotating the view. */
{
	switch( cameraMode )
	{
		case 0:
			yrot = yrot + movey /5.;
			xrot = xrot + movex /5.;
			break;		
		case 1:
			yrot2 = yrot2 + movey /5.;
			xrot2 = xrot2 + movex /5.;
			
			if(yrot2<0.5) yrot2=0.5;	// don't let reverse the rotation
			if(yrot2>31.5) yrot2=31.5;		// don't let se below the table
			
			setPos2();
					
			break;
		case 2:
			centerx += movex /5.;
			centerz += movey /5.;
			break;
		case 3:
			break;
	}
}

void Camera::action2 (int movex, int movey)
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
			zoom += movey / 5.f;
			if( zoom < 2 )
				zoom = 2;		// do not let reverse the camera
			
			setPos2();
			
			break;
		case 2:
			zoomTop += movey / 5.f;
			if( zoomTop < 40 )
				zoomTop = 40;		// do not let reverse the camera
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
			glTranslated(centerx,-zoomTop,centerz);
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
	switch ( getMode() )
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

