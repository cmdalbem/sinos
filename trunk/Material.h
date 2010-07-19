// Material.h
//
// Class for better access at material related functionalities
//

#pragma once

#include <iostream>
#include <string>
#include "lib/glm.h"


// Default values for Materials Attributes
const float default_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
const float default_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
const float default_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const float default_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const float default_shininess = 0;



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
    	void		apply()		const;
    	void		unapply()	const;
    	void 		resetMaterial();
};
