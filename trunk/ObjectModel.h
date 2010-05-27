// ObjectModel.h
//
// Every object that uses the glm library to be loaded is an ObjectModel.
// It extends the Object class and has a pointer to a GLMmodel, the str-
// uct declared in glm.h that is used to parse the obj files.
//

#pragma once

#include <iostream>
#include <string>
#include "lib/glm.h"
#include "Object.h"

class ObjectModel: public Object
{
private:
	GLMmodel* modelPointer;
public:
	//---------------------------- CONSTRUCTORS
	ObjectModel();
	ObjectModel(GLMmodel* model);
	ObjectModel(string filename);

	//---------------------------- DESTRUCTORS
	~ObjectModel();
	
	//---------------------------- GETTERS & SETTERS
	GLMmodel* 	getModelPointer();
	void		setModelPointer(GLMmodel* model);
	
	//---------------------------- OTHER METHODS
	void		loadFromFile(string file);
	void		draw();
};
