// Object.cpp
//      
// Functions that implements declarations in Object.h.
//

#include <iostream>
#include "Object.h"

#include "constants.h"


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
	
	resetMaterial();
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
	
	resetMaterial();
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
	
	resetMaterial();
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

void Object::setMaterialAmbient(GLfloat r, GLfloat g, GLfloat b)
{
	mat_ambient[0] = r;
	mat_ambient[1] = g;
	mat_ambient[2] = b;
}
void Object::setMaterialDiffuse(GLfloat r, GLfloat g, GLfloat b)
{
	mat_diffuse[0] = r;
	mat_diffuse[1] = g;
	mat_diffuse[2] = b;
}
void Object::setMaterialSpecular(GLfloat r, GLfloat g, GLfloat b)
{
	mat_specular[0] = r;
	mat_specular[1] = g;
	mat_specular[2] = b;
}
void Object::setMaterialEmission(GLfloat r, GLfloat g, GLfloat b)
{
	mat_emission[0] = r;
	mat_emission[1] = g;
	mat_emission[2] = b;
}

void Object::setMaterialShininess(GLfloat rgba)
{
	mat_shininess = rgba;
}

//------------------------------------------------------------ OTHER METHODS


void Object::drawBegin()
{
	glPushMatrix();
	
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, &mat_shininess);
	
	
	glTranslated(pos[0], pos[1], pos[2]);
	
	glRotatef(rot[0],1.0,0.0,0.0);
	glRotatef(rot[1],0.0,1.0,0.0);
	glRotatef(rot[2],0.0,0.0,1.0);
	
	glScalef(size[0], size[1], size[2]);
	
	glTranslated(0,0,0);
}

void Object::drawEnd()
{
	glMaterialfv(GL_FRONT, GL_DIFFUSE, default_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, default_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, default_emission);
	glMaterialfv(GL_FRONT, GL_AMBIENT, default_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, &default_shininess);
	
	glPopMatrix();
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

void Object::resetMaterial()
{
	mat_ambient[0] = default_ambient[0];
	mat_ambient[1] = default_ambient[1];
	mat_ambient[2] = default_ambient[2];
	
	mat_diffuse[0] = default_diffuse[0];
	mat_diffuse[1] = default_diffuse[1];
	mat_diffuse[2] = default_diffuse[2];
	
	mat_specular[0] = default_specular[0];
	mat_specular[1] = default_specular[1];
	mat_specular[2] = default_specular[2];
	
	mat_emission[0] = default_emission[0];
	mat_emission[1] = default_emission[1];
	mat_emission[2] = default_emission[2];
	
	mat_shininess = default_shininess;
}
