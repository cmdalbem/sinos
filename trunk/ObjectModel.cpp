// ObjectModel.cpp
//
// Function implementation of ObjectModel.h
//

#pragma once

#include <iostream>
#include "lib/glm.h"

//------------------------------------------------------------ CONSTRUCTORS

ObjectModel::ObjectModel()
{
	modelPointer = NULL;	// keeping the consistency
}

ObjectModel::ObjectModel(GLMmodel* model)
{
	modelPointer = model;
}

//------------------------------------------------------------ DESTRUCTORS

ObjectModel::~ObjectModel();
{
	if(modelPointer != NULL)	// free the memory from the pointer before
		glmDelete(modelPointer);// destroy the object
}

//------------------------------------------------------------ GETTERS & SETTERS

GLMmodel* ObjectModel::getModelPointer()
{
	return this->modelPointer;
}

ObjectModel::setModelPointer(GLMmodel* model)
{
	this->modelPointer = model;
}

ObjectModel::loadFromFile(string file)
{
	setModelPointer(glmReadOBJ(file));
}
