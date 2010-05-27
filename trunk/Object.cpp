// Object.cpp
//      
// Functions that implements declarations in Object.h.
//

#pragma once

#include <iostream>
#include "Object.h"

//------------------------------------------------------------ CONSTRUCTORS

Object::Object()// keeping the consistency
{
	pos[0] = 0;		// inicialize every variable with 0
	pos[1] = 0;
	pos[2] = 0;
	
	rot[0] = 0;
	rot[1] = 0;
	rot[2] = 0;
	
	size[0] = 1;
	size[1] = 1;
	size[2] = 1;
}

Object::Object(GLfloat position[], GLfloat rotation[], GLfloat scale[])
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

Object::Object( GLfloat posx,  GLfloat posy,  GLfloat posz,
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

Object::~Object()
{
	// This method is purposely blank
}

//------------------------------------------------------------ GETTERS & SETTERS

GLfloat	Object::getPos (int coord)
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
GLfloat Object::getPosX()
{	return pos[0];	}
GLfloat Object::getPosY()
{	return pos[1];	}
GLfloat Object::getPosZ()
{	return pos[2];	}
	
GLfloat	Object::getRot (int coord)
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
GLfloat Object::getRotX()
{	return rot[0];	}
GLfloat Object::getRotY()
{	return rot[1];	}
GLfloat Object::getRotZ()
{	return rot[2];	}
	
GLfloat	Object::getSize (int coord)
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
GLfloat Object::getSizeX()
{	return size[0];	}
GLfloat Object::getSizeY()
{	return size[1];	}
GLfloat Object::getSizeZ()
{	return size[2];	}



void Object::setPos (GLfloat position[])
{
	pos[0] = position[0];
	pos[1] = position[1];
	pos[2] = position[2];
}
void Object::setPosX(GLfloat posx)
{	pos[0] = posx;	}
void Object::setPosY(GLfloat posy)
{	pos[1] = posy;	}
void Object::setPosZ(GLfloat posz)
{	pos[2] = posz;	}
	
void Object::setRot (GLfloat rotation[])
{
	rot[0] = rotation[0];
	rot[1] = rotation[1];
	rot[2] = rotation[2];
}
void Object::setRotX(GLfloat rotx)
{	rot[0] = rotx;	}
void Object::setRotY(GLfloat roty)
{	rot[1] = roty;	}
void Object::setRotZ(GLfloat rotz)
{	rot[2] = rotz;	}
	
void Object::setSize (GLfloat scale[])
{
	size[0] = scale[0];
	size[1] = scale[1];
	size[2] = scale[2];
}
void Object::setSizeX(GLfloat sizex)
{	size[0] = sizex;	}
void Object::setSizeY(GLfloat sizey)
{	size[1] = sizey;	}
void Object::setSizeZ(GLfloat sizez)
{	size[2] = sizez;	}

void Object::draw()
{
	glRotatef(rot[0],1.0,0.0,0.0);
	glRotatef(rot[1],0.0,1.0,0.0);
	glRotatef(rot[2],0.0,0.0,1.0);
	
	glTranslated(-pos[0],-pos[1],-pos[2]);
	
	glScalef(size[0], size[1], size[2]);
}

void Object::translate(GLfloat* offset)
{
	pos[0] = offset[0];
	pos[1] = offset[1];
	pos[2] = offset[2];
}

void Object::translate(GLfloat x, GLfloat y, GLfloat z)
{
	pos[0] += x;
	pos[1] += y;
	pos[2] += z;
}

void Object::scale(GLfloat x, GLfloat y, GLfloat z)
{
	size[0] = size[0] * x;
	size[1] = size[1] * y;
	size[2] = size[2] * z;
}

void Object::rotate(GLfloat x, GLfloat y, GLfloat z)
{
	rot[0] = rot[0] + x;
	rot[1] = rot[1] + y;
	rot[2] = rot[2] + z;
}
