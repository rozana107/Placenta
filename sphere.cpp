
//=============================================================================
// sphere.c
//
//Author: Doron Nussbaum (C) 2015 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. creates a sphere object. 
//
//
// Description:
//--------------
//  Creates sphere geometry.  Currently it is used only to create a geometry of a sphere. It can be converted to a stand alone function. 
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


#include "sphere.h"
#include <iostream>
using namespace std;



Sphere::Sphere()
{
}


Sphere::~Sphere()
{
}






/*
// Function creates a sphere geometry

input:
numLong - number of longitudes strips
numLat - number of latituudes.  Note that the number of latitudes strips 


*/

int Sphere::createSphere(int numLong, int numLat, Vertices &vtx, Indices &ind, Vector4f color)

{
	int i, j, k;
	Vector4f pos;
	Vector4f col;
	float alpha;
	float beta;
	float deltaAlpha;
	float deltaBeta;


	vtx.resize(0);
	ind.resize(0);

	// Fill the vertex buffer with positions
	k = 0;
	alpha = 0.0f;  // angle of latitude starting from the "south pole"
	deltaAlpha = (float)180.0 / numLat; // increment of alpha
	beta = 0;   // angle of the longtidute 
	deltaBeta = (float)360.0 / (numLong);	// increment of beta

	// Set vertex colour to red
	col = color;// Vector4f(0.5, 0.0, 0.0, 1.0);

	for (i = 0, alpha = -90; i <= numLat; i++, alpha += deltaAlpha) {
		for (j = 0, beta = 0; j <= numLong; j++, beta += deltaBeta) {
			pos.x = cos(DegreeToRadians(alpha))*cos(DegreeToRadians(beta));
			pos.z = cos(DegreeToRadians(alpha))*sin(DegreeToRadians(beta));
			pos.y = sin(DegreeToRadians(alpha));
			pos.w = 1.0;

			//vtx.push_back(Vertex(pos, Vector3f(pos.x, pos.y, pos.z),col));
			vtx.push_back(Vertex(pos, col));
		}
	}




	// fill the index buffer

	k = 0;
	for (i = 0; i < numLat; i++) {
		for (j = 0; j < numLong; j++) {
			// fill indices for the quad
			// change by making a quad function
			ind.push_back(i * (numLong + 1) + j);
			ind.push_back(i * (numLong + 1) + j + 1);
			ind.push_back((i + 1) * (numLong + 1) + j + 1);

			ind.push_back(i * (numLong + 1) + j);
			ind.push_back((i + 1) * (numLong + 1) + j + 1);
			ind.push_back((i + 1) * (numLong + 1) + j);
		}
	}


	return(0);

}





/*********************************************************************************/


int Sphere::render(Shader shader)
{
	Matrix4f rotMat;  // rotation matrix;
	Matrix4f scaleMat; // scaling matrix;
	Matrix4f transMat;	// translation matrix
	Matrix4f modelMat;	// final model matrix
	static float y = 0;

	// set the transformation matrix - the model transfomration
	modelMat = Matrix4f::identity(); // = Matrix4f::rotateRollPitchYaw(rollAngle, pitchAngle, yawAngle, 1);

	// set the scaling - this is model space to model space transformation
	scaleMat = Matrix4f::scale(scale.x, scale.y, scale.z);
	modelMat = scaleMat * modelMat;

	// set the rotation  - this is model space to model space transformation 
	rotMat = Matrix4f::rotateRollPitchYaw(rollAngle, pitchAngle, yawAngle, 1);
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



