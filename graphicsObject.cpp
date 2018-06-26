

//=============================================================================
// graphicsObject.c
//
//Author: Doron Nussbaum (C) 2015 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. generic class for graphics objects. 
//
//
// Description:
//--------------
//  sub classes of objects can be inherited.  The class does the main funcionality of setting and rendering objects.

//
//
// License
//--------------
//
// Code can be used for instructional and educational purposes.
// Usage of code for other purposes is not allowed without a given permission by the author.
//
//
// Disclaimer
//--------------
//
// The code is provided as is without any warranty

//=============================================================================




#include "GraphicsObject.h"

/***************************************************************************/

GraphicsObject::GraphicsObject() : vtxVBO(-1), vao(-1), indVBO(-1)
, rollAngle(0)
, numIndices(0)
{
}

/***************************************************************************/



GraphicsObject::~GraphicsObject()
{
}


/***************************************************************************/



int GraphicsObject::createVAO(Shader shader)
{
	return 0;
}


/***************************************************************************/


int GraphicsObject::createVAO(Shader shader, Vertices vtx, Indices ind)
{
	int rc = 0;

	GLint location;		// location of the attributes in the shader;

	//create vertex array object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//create vertex buffer object
	glGenBuffers(1, &vtxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vtxVBO);
	glBufferData(GL_ARRAY_BUFFER, vtx.size() * sizeof(Vertex), vtx.data(), GL_STATIC_DRAW);

	//copy the vertex position
	location = glGetAttribLocation(shader.getProgId(), "vtxPos");
	if (location == -1) {
		rc = -1;
		goto err;
	}
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));

	//copy the vertex color
	location = glGetAttribLocation(shader.getProgId(), "vtxCol");

	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, col));

	//copy the vertex normal
	location = glGetAttribLocation(shader.getProgId(), "vtxNormal");
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)); //4/pos

	//create index buffer
	glGenBuffers(1, &indVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(GLuint), ind.data(), GL_STATIC_DRAW);
	// store the number of indices
	numIndices = ind.size();

	// Free data buffers
	vtx.clear();
	ind.clear();

	//end creation
	glBindVertexArray(0);

	err:
	return(rc);
}



/*********************************************************************************/


int GraphicsObject::render(Shader shader)
{
	Matrix4f rotMat;  // rotation matrix;
	Matrix4f scaleMat; // scaling matrix;
	Matrix4f transMat;	// translation matrix
	Matrix4f modelMat;	// final model matrix

	// set the transformation matrix - the model transfomration
	modelMat = Matrix4f::identity(); // = Matrix4f::rotateRollPitchYaw(rollAngle, pitchAngle, yawAngle, 1);

	// set the scaling - this is model space to model space transformation
	scaleMat = Matrix4f::scale(scale.x, scale.y, scale.z);
	modelMat = scaleMat * modelMat;

	// set the rotation  - this is model space to model space transformation 
	rotMat = Matrix4f::rotateRollPitchYaw(rollAngle,pitchAngle,yawAngle,1);
	// note that we always multiply the new matrix on the left
	modelMat = rotMat * modelMat;


	// set the translation - this is model space to world space transformation
	transMat = Matrix4f::translation(position);
	modelMat = transMat * modelMat;

	// move matrix to shader
	shader.copyMatrixToShader(modelMat, "model");

	loadMaterials(shader);

	// bind the vao
	glBindVertexArray(vao);

	// draw using indices
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);

	// unbind the vao
	glBindVertexArray(0);

	return 0;
}


/*************************************************************/
// sets the initial orientation

void GraphicsObject::setInitialRotations(float rollAngle, float pitchAngle, float yawAngle)
{
	this->rollAngle = rollAngle;
	this->pitchAngle = pitchAngle;
	this->yawAngle = yawAngle;

}

/*************************************************************************/

// set the initial position

void GraphicsObject::setInitialPosition(Vector3f position)
{
	this->position = position;
}

/*************************************************************************/

// set the initial position

void GraphicsObject::setInitialPosition(float x, float y, float z)
{
	this->position = Vector3f(x, y, z);
}
/*************************************************************************/

// set the initial start and end of the line segment 

void GraphicsObject::setLineEndPositions(float x1, float y1, float z1, float x2, float y2, float z2)
{
	this->position1 = Vector3f(x1, y1, z1);
	this->position2 = Vector3f(x2, y2, z2);
}

/*************************************************************/
// increment the courrent rotation by the given amounts

void GraphicsObject::incrementRotations(float rollAngle, float pitchAngle, float yawAngle)
{
	this->rollAngle += rollAngle;
	this->pitchAngle += pitchAngle;
	this->yawAngle += yawAngle;

}

/*************************************************************************/

// increment positin by delta position

void GraphicsObject::incrementPosition(Vector3f deltaPosition)
{
	this->position += deltaPosition;
}

/*************************************************************************/

// increment position by delta

void GraphicsObject::incrementPosition(float deltaX, float deltaY, float deltaZ)
{
	this->position += Vector3f(deltaX, deltaY, deltaZ);
}


/*************************************************************************/

// set the initial position

void GraphicsObject::setScale(Vector3f scale)
{
	this->scale = scale;
}

/*************************************************************************/

// set the initial position

void GraphicsObject::setScale(float scaleX, float scaleY, float scaleZ)
{
	this->scale = Vector3f(scaleX, scaleY, scaleZ);
}

/*********************************************************************************/


int GraphicsObject::loadMaterials(Shader shader)

{
	shader.copyFloatVectorToShader((float *)&materials.ambientMaterial, 1, 3, "gMaterial.ambientMaterial");
	shader.copyFloatVectorToShader((float *)&materials.diffuseMaterial, 1, 3, "gMaterial.diffuseMaterial");
	shader.copyFloatVectorToShader((float *)&materials.specularMaterial, 1, 3, "gMaterial.specularMaterial");
	shader.copyFloatVectorToShader((float *)&materials.interalRadiation, 1, 3, "gMaterial.interalRadiation");

	return(0);
}


/*****************************/
// get the graphic object's scale
void GraphicsObject::incrementScale(float dx, float dy, float dz)
{
	this->scale.x += dx;
	this->scale.y += dy;
	this->scale.z += dz;

	if (this->scale.x < 1) this->scale.x = 1;
	if (this->scale.y < 1) this->scale.y = 1;
	if (this->scale.z < 1) this->scale.z = 1;
}