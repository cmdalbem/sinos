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

class ObjectModel: public Object
{
private:
	GLMmodel* modelPointer = NULL;
public:
	ObjectModel();
	ObjectModel(GLMmodel* model);

	~ObjectModel();
	
	GLMmodel* 	getModelPointer();
	void		setModelPointer(GLMmodel* model);
	
	void		loadFromFile(string file);
};
