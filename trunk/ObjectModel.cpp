// ObjectModel.cpp
//
// Function implementation of ObjectModel.h
//

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

ObjectModel::ObjectModel(string filename)
:	Object()
{
	modelPointer = NULL;
	loadFromFile(filename);
}

// TODO: create the others constructors, based on the mother class

//------------------------------------------------------------ DESTRUCTORS

ObjectModel::~ObjectModel()
{
	if(modelPointer != NULL)	// free the memory from the pointer before
		glmDelete(modelPointer);// destroy the object
}

//------------------------------------------------------------ GETTERS & SETTERS

GLMmodel* ObjectModel::getModelPointer() const
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
	{
		setModelPointer(glmReadOBJ(file));
		if ( !getModelPointer() )
			cout << "Could not open " << file << endl;

	}
	else
		cout << "ModelPointer has already been loaded" << endl;
}

void ObjectModel::calculateNormals()
{
	glmFacetNormals(getModelPointer());
    glmVertexNormals(getModelPointer(), 90.0);
}

void ObjectModel::draw() const
{
	if(modelPointer)	// Test if there is anything to draw
	{
		drawBegin();		// Move, scale and rotate the object to the right place
		if( texture!=NULL )
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texture->texID);
			glmDraw(modelPointer, GLM_SMOOTH | GLM_TEXTURE);
			glDisable(GL_TEXTURE_2D);
		}
		else
			glmDraw(modelPointer, GLM_SMOOTH);
		drawEnd();
		
	}
}
