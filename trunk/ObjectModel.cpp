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
			glBindTexture(GL_TEXTURE_2D, texture->texID);
			glmDraw(modelPointer, GLM_SMOOTH | GLM_TEXTURE);
		}
		else
			glmDraw(modelPointer, GLM_SMOOTH);
		drawEnd();
	}
}

void ObjectModel::drawNormals() const
{
	double x2 = pos[0],
		   y2 = pos[1],
		   z2 = pos[2];
	double sx = size[0],
		   sy = size[1],
		   sz = size[2];
	double truex, truey, truez;
	
	for(int v=0; v<modelPointer->numvertices; v++)
	{
		truex = x2 + sx*modelPointer->vertices[3*v + 0];
		truey = y2 + sy*modelPointer->vertices[3*v + 1];
		truez = z2 + sz*modelPointer->vertices[3*v + 2];
		
		glBegin(GL_LINES);
			glVertex3f(truex,truey,truez);
			glVertex3f(x2 + (truex + modelPointer->normals[3*v + 0]),
					   y2 + (truey + modelPointer->normals[3*v + 1]),
					   z2 + (truez + modelPointer->normals[3*v + 2]) );
		glEnd();
	}
}
