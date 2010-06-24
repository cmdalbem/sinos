// Object.cpp
//      
// Functions that implements declarations in Object.h.
//

#include <iostream>
#include "Object.h"


//------------------------------------------------------------ CONSTRUCTORS

Object::Object()// keeping the consistency
{
	resetPosition();
	
	resetRotation();
	
	resetSize();
	
	texture = NULL;
}

Object::Object( Texture *_tex)// keeping the consistency
{
	resetPosition();
	
	resetRotation();
	
	resetSize();
	
	texture = _tex;
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
	
	texture = NULL;
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
	
	texture = NULL;
}

//------------------------------------------------------------ DESTRUCTORS

Object::~Object()
{
	// This method is purposely blank
}

//------------------------------------------------------------ GETTERS & SETTERS

GLfloat	Object::getPos (int coord) const
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
GLfloat Object::getPosX() const
{	return pos[0];	}
GLfloat Object::getPosY() const
{	return pos[1];	}
GLfloat Object::getPosZ() const
{	return pos[2];	}
	
GLfloat	Object::getRot (int coord) const
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
GLfloat Object::getRotX() const
{	return rot[0];	}
GLfloat Object::getRotY() const
{	return rot[1];	}
GLfloat Object::getRotZ() const
{	return rot[2];	}
	
GLfloat	Object::getSize (int coord) const
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
GLfloat Object::getSizeX() const
{	return size[0];	}
GLfloat Object::getSizeY() const
{	return size[1];	}
GLfloat Object::getSizeZ() const
{	return size[2];	}



void Object::setPos (GLfloat x, GLfloat y, GLfloat z)
{
	pos[0] = x;
	pos[1] = y;
	pos[2] = z;
}
void Object::setPosX(GLfloat posx)
{	pos[0] = posx;	}
void Object::setPosY(GLfloat posy)
{	pos[1] = posy;	}
void Object::setPosZ(GLfloat posz)
{	pos[2] = posz;	}
	
void Object::setRot (GLfloat x, GLfloat y, GLfloat z)
{
	rot[0] = x;
	rot[1] = y;
	rot[2] = z;
}
void Object::setRotX(GLfloat rotx)
{	rot[0] = rotx;	}
void Object::setRotY(GLfloat roty)
{	rot[1] = roty;	}
void Object::setRotZ(GLfloat rotz)
{	rot[2] = rotz;	}
	
void Object::setSize (GLfloat x, GLfloat y, GLfloat z)
{
	size[0] = x;
	size[1] = y;
	size[2] = z;
}
void Object::setSizeX(GLfloat sizex)
{	size[0] = sizex;	}
void Object::setSizeY(GLfloat sizey)
{	size[1] = sizey;	}
void Object::setSizeZ(GLfloat sizez)
{	size[2] = sizez;	}

void Object::setMaterial(Material const newmaterial)
{
	material = newmaterial;
}

void Object::setTexture(Texture *_texture)
{
	texture = _texture;
}

void Object::resetPosition()
{
	pos[0] = 0;
	pos[1] = 0;
	pos[2] = 0;
}

void Object::resetRotation()
{
	rot[0] = 0;
	rot[1] = 0;
	rot[2] = 0;
}

void Object::resetSize()
{	
	size[0] = 1;
	size[1] = 1;
	size[2] = 1;
}



//------------------------------------------------------------ OTHER METHODS


void Object::drawBegin() const
{
	glPushMatrix();
	
	material.apply();
	
	glTranslated(pos[0], pos[1], pos[2]);
	glScalef(size[0], size[1], size[2]);
	glRotatef(rot[1],0,1,0);
	glRotatef(rot[0],1,0,0);
	glRotatef(rot[2],0,0,1);
}

void Object::drawEnd() const
{
	material.unapply();
	
	glPopMatrix();
}

double Object::distanceFromObject(Object &obj) const
{
	return sqrt(pow(getPosX() - obj.getPosX(), 2) +	// use "pow" because it has
				pow(getPosY() - obj.getPosY(), 2) +	// log(n) complexity instead
				pow(getPosZ() - obj.getPosZ(), 2));	// of n
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
