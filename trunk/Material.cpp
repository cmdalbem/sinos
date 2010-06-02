// Material.cpp
//
// Function implementation of Material.h
//

#include <iostream>
#include "Material.h"

#include "constants.h"


//------------------------------------------------------------ CONSTRUCTORS

Material::Material()
{
	resetMaterial();
}

//------------------------------------------------------------ DESTRUCTORS

Material::~Material()
{
	// This method is purposely blank
}

//------------------------------------------------------------ GETTERS & SETTERS


void Material::setAmbient(GLfloat r, GLfloat g, GLfloat b)
{
	mat_ambient[0] = r;
	mat_ambient[1] = g;
	mat_ambient[2] = b;
}
void Material::setDiffuse(GLfloat r, GLfloat g, GLfloat b)
{
	mat_diffuse[0] = r;
	mat_diffuse[1] = g;
	mat_diffuse[2] = b;
}
void Material::setSpecular(GLfloat r, GLfloat g, GLfloat b)
{
	mat_specular[0] = r;
	mat_specular[1] = g;
	mat_specular[2] = b;
}
void Material::setEmission(GLfloat r, GLfloat g, GLfloat b)
{
	mat_emission[0] = r;
	mat_emission[1] = g;
	mat_emission[2] = b;
}

void Material::setShininess(GLfloat rgba)
{
	mat_shininess = rgba;
}


//-------------------------------------------------------- OTHER METHODS

void Material::apply() const
{
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, &mat_shininess);
}

void Material::unapply() const
{
	glMaterialfv(GL_FRONT, GL_DIFFUSE, default_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, default_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, default_emission);
	glMaterialfv(GL_FRONT, GL_AMBIENT, default_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, &default_shininess);
}

void Material::resetMaterial()
{
	setAmbient( default_ambient[0], 
				default_ambient[1],
				default_ambient[2]);
	
	setDiffuse( default_diffuse[0],
				default_diffuse[1],
				default_diffuse[2]);
	
	setSpecular(default_specular[0],
				default_specular[1],
				default_specular[2]);
	
	setEmission(default_emission[0],
				default_emission[1],
				default_emission[2]);
				
	setShininess(default_shininess);
}
