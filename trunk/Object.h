// Object.h
//
// Object is everything that is drawn on the scene, since spheres until
// entire images loaded from .obj files, like a pooltable.
//

#pragma once

#include <iostream>
#include "lib/glm.h"
#include <GL/glut.h>

class Object
{
private:
	GLfloat pos[3],		// position x, y and z of the Object
			rot[3],		// rotation arguments on Rotatefs functions called on Draw
			size[3];	// scale arguments on Scalefs functions called on Draw
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
	
	void setPos (GLfloat position[]);
	void setPosX(GLfloat posx);
	void setPosY(GLfloat posy);
	void setPosZ(GLfloat posz);
	
	void setRot (GLfloat rotation[]);
	void setRotX(GLfloat rotx);
	void setRotY(GLfloat roty);
	void setRotZ(GLfloat rotz);
	
	void setSize (GLfloat scale[]);
	void setSizeX(GLfloat sizex);
	void setSizeY(GLfloat sizey);
	void setSizeZ(GLfloat sizez);
	
	//---------------------------- OTHER METHODS
	
	/*  Draw()
	*  The unique informations that all objects do have are their position,
	*  rotational argument and scaling argument. So, this generic function
	*  just move the object to the place where it has to be drawn.
	*
	*  If you want to presenve the matrix, push it before calling this fun-
	*  tion: the function doesn't do that because it's supposed to be call-
	*  ed inner other child's Draw function.
	*/
	void Draw();		// TODO: function return 1 if succesfully drawn the object
};
