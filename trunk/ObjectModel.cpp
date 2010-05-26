// ObjectModel.cpp
//
// Function implementation of ObjectModel.h
//

#pragma once

#include <iostream>
#include "ObjectModel.h"

//------------------------------------------------------------ CONSTRUCTORS

ObjectModel::ObjectModel()
:	Object()
{
	modelPointer = NULL;	// keeping the consistency
}

ObjectModel::ObjectModel(GLMmodel* model)
:	Object()
{
	modelPointer = model;
}

// TODO: create the others constructors, based on the mother class

//------------------------------------------------------------ DESTRUCTORS

ObjectModel::~ObjectModel()
{
	if(modelPointer != NULL)	// free the memory from the pointer before
		glmDelete(modelPointer);// destroy the object
}

//------------------------------------------------------------ GETTERS & SETTERS

GLMmodel* ObjectModel::getModelPointer()
{
	return this->modelPointer;
}

void ObjectModel::setModelPointer(GLMmodel* model)
{
	this->modelPointer = model;
}

void ObjectModel::loadFromFile(string file)
{
	if(!getModelPointer())
		setModelPointer(glmReadOBJ(file));
	else
		cout << "ModelPointer has already been loaded" << endl;
}

void ObjectModel::Draw()
{
	Object::Draw();		// Move, scale and rotate the object to the right place
	
	glmDraw(modelPointer, GLM_SMOOTH);
}
