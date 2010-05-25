// ObjectModel.cpp
//
// Function implementation of ObjectModel.h
//

#pragma once

#include <iostream>
#include "ObjectModel.h"

//------------------------------------------------------------ CONSTRUCTORS

ObjectModel::ObjectModel()
:	object()
{
	modelPointer = NULL;	// keeping the consistency
}

ObjectModel::ObjectModel(GLMmodel* model)
:	object()
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
	setModelPointer(glmReadOBJ(file));
}

void ObjectModel::Draw()
{
}
