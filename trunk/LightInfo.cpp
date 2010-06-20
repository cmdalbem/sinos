// LightInfo.cpp
//
// Function implementation of LightInfo.h
//

#include "LightInfo.h"

//------------------------------------------------------------ CONSTRUCTORS
LightInfo::LightInfo()
{
	bool	isTherePosition  = false;
	bool	isThereDirection = false;
}

LightInfo::LightInfo(
 bool newIsTherePosition,  GLfloat newPosition[4],
 bool newIsThereDirection, GLfloat newDirection[4],
 long int  newWhatLight )
{
	isTherePosition  = newIsTherePosition;
	isThereDirection = newIsThereDirection;
	for(int i = 0; i < 4; i++)
	{
		position [i] = newPosition [i];
		direction[i] = newDirection[i];
	}
	
	whatLight = newWhatLight;
	
	/*
	switch(newWhatLight)
	{
		case 0: whatLight = GL_LIGHT0;
				break;
		case 1: whatLight = GL_LIGHT1;
				break;
		case 2: whatLight = GL_LIGHT2;
				break;				
		case 3: whatLight = GL_LIGHT3;
				break;				
		case 4: whatLight = GL_LIGHT4;
				break;
		case 5: whatLight = GL_LIGHT5;
				break;
		case 6: whatLight = GL_LIGHT6;
				break;
		case 7: whatLight = GL_LIGHT7;
				break;
	}
	*/
}

//------------------------------------------------------------ DESTRUCTORS
LightInfo::~LightInfo() {	}

//------------------------------------------------------------ GETTERS & SETTERS

//------------------------------------------------------------ OTHER METHODS
void LightInfo::apply()
{
	if(isTherePosition)
		glLightfv(whatLight, GL_POSITION, position);
	if(isThereDirection)
		glLightfv(whatLight, GL_SPOT_DIRECTION, direction);
}

