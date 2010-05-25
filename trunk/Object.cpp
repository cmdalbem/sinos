// object.cpp
//      
// Functions that implements declarations in object.h.
//

#pragma once

#include <iostream>
#include "object.h"

//------------------------------------------------------------ CONSTRUCTORS

object::object()// keeping the consistency
{
	pos[0] = 0;		// inicialize every variable with 0
	pos[1] = 0;
	pos[2] = 0;
	
	rot[0] = 0;
	rot[1] = 0;
	rot[2] = 0;
	
	size[0] = 0;
	size[1] = 0;
	size[2] = 0;
}

object::object(GLfloat position[], GLfloat rotation[], GLfloat scale[])
{
	pos[0] = position[0];
	pos[1] = position[1];
	pos[2] = position[2];
	
	rot[0] = rotation[0];
	rot[1] = rotation[1];
	rot[2] = rotation[2];
	
	size[0] = scale[0];
	size[1] = scale[1];
	size[2] = scale[2];
}

object::object( GLfloat posx,  GLfloat posy,  GLfloat posz,
				GLfloat rotx,  GLfloat roty,  GLfloat rotz,
				GLfloat sizex, GLfloat sizey, GLfloat sizez)
{
	pos[0] = posx;
	pos[1] = posy;
	pos[2] = posz;
	
	rot[0] = rotx;
	rot[1] = roty;
	rot[2] = rotz;
	
	size[0] = sizex;
	size[1] = sizey;
	size[2] = sizez;
}

//------------------------------------------------------------ DESTRUCTORS

object::~object()
{
	// This method is purposely blank
}

//------------------------------------------------------------ GETTERS & SETTERS

GLfloat	object::getPos (int coord)
{
	if(coord == 0)
		return pos[0];
	else if (coord == 1)
		return pos[1];
	else if (coord == 2)
		return pos[2];
	else
		return 0;
}
GLfloat object::getPosX()
{	return pos[0];	}
GLfloat object::getPosY()
{	return pos[1];	}
GLfloat object::getPosZ()
{	return pos[2];	}
	
GLfloat	object::getRot (int coord)
{
	if(coord == 0)
		return rot[0];
	else if (coord == 1)
		return rot[1];
	else if (coord == 2)
		return rot[2];
	else
		return 0;
}
GLfloat object::getRotX()
{	return rot[0];	}
GLfloat object::getRotY()
{	return rot[1];	}
GLfloat object::getRotZ()
{	return rot[2];	}
	
GLfloat	object::getSize (int coord)
{
	if(coord == 0)
		return size[0];
	else if (coord == 1)
		return size[1];
	else if (coord == 2)
		return size[2];
	else
		return 0;
}
GLfloat object::getSizeX()
{	return size[0];	}
GLfloat object::getSizeY()
{	return size[1];	}
GLfloat object::getSizeZ()
{	return size[2];	}



void object::setPos (GLfloat position[])
{
	pos[0] = position[0];
	pos[1] = position[1];
	pos[2] = position[2];
}
void object::setPosX(GLfloat posx)
{	pos[0] = posx;	}
void object::setPosY(GLfloat posy)
{	pos[1] = posy;	}
void object::setPosZ(GLfloat posz)
{	pos[2] = posz;	}
	
void object::setRot (GLfloat rotation[])
{
	rot[0] = rotation[0];
	rot[1] = rotation[1];
	rot[2] = rotation[2];
}
void object::setRotX(GLfloat rotx)
{	rot[0] = rotx;	}
void object::setRotY(GLfloat roty)
{	rot[1] = roty;	}
void object::setRotZ(GLfloat rotz)
{	rot[2] = rotz;	}
	
void object::setSize (GLfloat scale[])
{
	size[0] = scale[0];
	size[1] = scale[1];
	size[2] = scale[2];
}
void object::setSizeX(GLfloat sizex)
{	size[0] = sizex;	}
void object::setSizeY(GLfloat sizey)
{	size[1] = sizey;	}
void object::setSizeZ(GLfloat sizez)
{	size[2] = sizez;	}

void object::Draw()
{
	// The specialized classes will have informations here to really draw
	// something on the screen =D
}
