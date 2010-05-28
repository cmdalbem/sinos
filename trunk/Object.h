#pragma once


// Object.h
//
// Object is everything that is drawn on the scene, since spheres until
// entire images loaded from .obj files, like a pooltable.
//

#include <iostream>
#include "lib/glm.h"
#include <GL/glut.h>



class Object
{
private:
	GLfloat pos[3],		// position x, y and z of the Object
			rot[3],		// rotation arguments on Rotatefs functions called on Draw
			size[3];	// scale arguments on Scalefs functions called on Draw
			
	GLfloat mat_ambient[3],
			mat_diffuse[3],
			mat_specular[3],
			mat_emission[3],
			mat_shininess;
					
			
public:

	//---------------------------- CONSTRUCTORS
	Object();
	Object(GLfloat pos[], GLfloat rot[], GLfloat size[]);
	Object( GLfloat xpos,  GLfloat ypos,  GLfloat zpos,
			GLfloat xrot,  GLfloat yrot,  GLfloat zrot,
			GLfloat xsize, GLfloat ysize, GLfloat zsize);
	
	//---------------------------- DESTRUCTORS
	~Object();
	
	//---------------------------- GETTERS & SETTERS
	GLfloat getPos (int coord);
	GLfloat getPosX();
	GLfloat getPosY();
	GLfloat getPosZ();
	
	GLfloat getRot (int coord);
	GLfloat getRotX();
	GLfloat getRotY();
	GLfloat getRotZ();
	
	GLfloat getSize (int coord);
	GLfloat getSizeX();
	GLfloat getSizeY();
	GLfloat getSizeZ();
	
	void setPos (GLfloat x, GLfloat y, GLfloat z);
	void setPosX(GLfloat posx);
	void setPosY(GLfloat posy);
	void setPosZ(GLfloat posz);
	
	void setRot (GLfloat x, GLfloat y, GLfloat z);
	void setRotX(GLfloat rotx);
	void setRotY(GLfloat roty);
	void setRotZ(GLfloat rotz);
	
	void setSize (GLfloat x, GLfloat y, GLfloat z);
	void setSizeX(GLfloat sizex);
	void setSizeY(GLfloat sizey);
	void setSizeZ(GLfloat sizez);
	
	void setMaterialAmbient(GLfloat r, GLfloat g, GLfloat b);
	void setMaterialDiffuse(GLfloat r, GLfloat g, GLfloat b);
	void setMaterialSpecular(GLfloat r, GLfloat g, GLfloat b);
	void setMaterialEmission(GLfloat r, GLfloat g, GLfloat b);
	void setMaterialShininess(GLfloat rgba);
	
	//---------------------------- OTHER METHODS
	
	/* drawBegin()
	*  Call this before all other drawing calls.
	*  The unique informations that all objects do have are their position,
	*  rotational argument and scaling argument. This function will initialize
	*  the matrix, materials and do the scaling, rotation and translation.
	*/
	void drawBegin();		// TODO: function return 1 if succesfully drawn the object
	
	/* drawEnd()
	*  Call this one after all other drawing calls. This will recover default
	*  values of materials and pop this object matrix.
	*/
	void drawEnd();
	
	/* Translate() */
	void translate(GLfloat* offset);
	void translate(GLfloat x, GLfloat y, GLfloat z);
	
	/* Scale() */
	void scale(GLfloat x, GLfloat y, GLfloat z);
	
	/* Rotate() */
	void rotate(GLfloat x, GLfloat y, GLfloat z);
	
	
	/* Materials */
	void resetMaterial();
};
