// Material.h
//
// Class for better access at material related functionalities
//

#pragma once

#include <iostream>
#include <string>
#include "lib/glm.h"


class Material
{
private:
	GLfloat mat_ambient[3],
			mat_diffuse[3],
			mat_specular[3],
			mat_emission[3],
			mat_shininess;

public:
	//---------------------------- CONSTRUCTORS
	Material();

	//---------------------------- DESTRUCTORS
	~Material();
	
	//---------------------------- GETTERS & SETTERS
	
	void setAmbient(GLfloat r, GLfloat g, GLfloat b);
	void setDiffuse(GLfloat r, GLfloat g, GLfloat b);
	void setSpecular(GLfloat r, GLfloat g, GLfloat b);
	void setEmission(GLfloat r, GLfloat g, GLfloat b);
	void setShininess(GLfloat rgba);
	
	//---------------------------- OTHER METHODS
	void		apply();
	void		unapply();
	void 		resetMaterial();
};
