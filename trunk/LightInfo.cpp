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
		
	cout << "position " << whatLight << endl
		 << position[0] << " " << position[1] << " " << position [2] << " " << position[3] << endl;
}

