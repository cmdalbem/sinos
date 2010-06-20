// LightInfo.h
//
// This class is to be used with the Level class. The purpose is to have
// a Data Structure where to put the light informations for the lights()
// function.
//

#pragma once

#include <iostream>
using namespace std;

#include <GL/glut.h>

class LightInfo
{
private:
	bool	isTherePosition;
	GLfloat position [4];

	bool	isThereDirection;
	GLfloat direction[4];
	
	int		whatLight;	// GL_LIGHT#
	

	
public:
	//---------------------------- CONSTRUCTORS
	LightInfo();		// This constructor exists just to have a
						// LightInfo inner other classes without
						// complainings from the other class.
	
	/* LightInfo()
	 * 
	 * If a NULL pointer is passed on one of the vectors, then the corr-
	 * esponding bool has to be false. The glLightfv() function, then,
	 * is not going to be called on the .apply() function.
	 */
	LightInfo(	bool 	newIsTherePosition,
				GLfloat	newPosition[4],
				bool 	newIsThereDirection,
				GLfloat	newDirection[4],
				int		newWhatLight	
			);
			
	//---------------------------- DESTRUCTORS
	~LightInfo();
	//---------------------------- GETTERS & SETTERS
	
	//---------------------------- OTHER METHODS
	void		apply();
};

