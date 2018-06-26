//=============================================================================
// cylinder.h
//
//Author: Doron Nussbaum (C) 2015 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. creates a cylinder object. 
//
//
// Description:
//--------------
//  Creates a cylinder geometry.  Currently it is used only to create a geometry of a sphere. It can be converted to a stand alone function. 
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




#include "cylinder.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>


Cylinder::Cylinder()
{

}

Cylinder::Cylinder(node *n1, node *n2)
{
	nod1 = n1;
	nod2 = n2;
}


Cylinder::~Cylinder()
{
}

/**************************************************************************************/
/* this function creates a cylinder.  The cylinder is genreated using two arrays.  The vertex data array which
contains the vertex data (geometry) and an index array which contains the topology of the triangles.  The trainagles
are stored in the index array as a triangle list.

Note that radius and height of the cylinder is 1

Input
numLong - number of longitudes lines.  For example if numLong == 10 than the sphere is divided into 10
of 36 degrees each


Ouptut:
vtx - an array (vector of vertices).  Currently the function computes the position and colour information. 

ind - an array (vector with the topology of the cylinder.


Return:
the function returns 0 is successful.


*/

//int Cylinder::createCylinder(int numLong, Vertices &vtx, Indices &ind , Vector4f color)
//
//{
//	int i, j, k;
//	int numRows;
//	int numCols;
//	Vector4f pos;
//	Vector4f col;
//	float alpha;
//	float deltaAlpha;
//
//	numRows = 1;  // number of horizonal slabs
//	numCols = numLong;	// number of vertical slabs
//
//	//col = Vector4f(0.5,0,0,1);
//	col = color;
//
//	vtx.resize(0);
//
//	ind.resize(0);
//
//	// Fill the vertex buffer with positions
//	alpha = 0;   // angle of the longtidute 
//	deltaAlpha = (float)360.0 / (numLong);	// increment of beta
//
//	for (i = -1; i <= 1; i += 2) {
//		for (j = 0, alpha = 45; j <= numCols; j++, alpha += deltaAlpha) {
//			pos.x = cos(DegreeToRadians(alpha));
//			pos.z = sin(DegreeToRadians(alpha));
//			pos.y = (float) i;
//			pos.w = 1.0;
//
//		//	vtx.push_back(Vertex(pos, pos)); //  second pos = col : create color based on position
//			vtx.push_back(Vertex(pos, col));
//			 
//
//		}
//	}
//
//	// fill the index buffer
//
//	k = 0;
//	for (i = 0; i < numRows; i++) {
//		for (j = 0; j < numCols; j++) {
//			// fill indices for the quad
//			ind.push_back(i * (numCols + 1) + j);
//			ind.push_back(i * (numCols + 1) + j + 1);
//			ind.push_back((i + 1) * (numCols + 1) + j + 1);
//
//			ind.push_back(i * (numCols + 1) + j);
//			ind.push_back((i + 1) * (numCols + 1) + j + 1);
//			ind.push_back((i + 1) * (numCols + 1) + j);
//		}
//	}
//	//// fill the index buffer of the top plate	
//	//for (i = 0, j = numCols + 2; i < numCols - 2; i++, j++) {
//	//	ind.push_back(numCols + 1);
//	//	ind.push_back(j);
//	//	ind.push_back(j + 1);
//	//}
//
//
//	//// fill the index buffer of the bottom plate	
//	//for (i = 0, j = 1; i < numCols - 2; i++, j++) {
//	//	ind.push_back(0);
//	//	ind.push_back(j + 1);
//	//	ind.push_back(j);
//	//}
//
//	return(0);
//
//}


void Cylinder::setId(int id1, int id2){
	cId1 = id1;
	cId2 = id2;
}
int Cylinder::getId(){
	return cId1;
}

void Cylinder::setLev(int lev){
	 level =lev;
}
int Cylinder::getLev(){
	return level;
}

 int Cylinder::createVessels(int numLong, Vertices &vtx, Indices &ind,Vector3f dir, Vector4f color, int lev)

{   ////A x B = (a2b3  -   a3b2,     a3b1   -   a1b3,     a1b2   -   a2b1)
	float radius = 2.0;
	float height = dir.length();
	int i, j;
	int numRows;
	int numCols;
	Vector4f pos,npos;
	Vector4f col;
	Vector3f norm;
	Vector3f up= Vector3f(0, 1,0);
	Vector4f newPos;

	float alpha;
	float deltaAlpha;

	numRows = ceil(dir.length());// (int)1 * height;  // number of horizonal slabs

	numCols = numLong;	// number of vertical slabs

	//col = Vector4f(0.5,0,0,1);
	col = color;

	vtx.resize(0);
	ind.resize(0);


	float angle = acos((up.x * dir.x + up.y*dir.y + up.z*dir.z) / dir.length());
	Vector3f normal = Vector3f(up.y*dir.z - dir.y*up.z, -1 * (up.x*dir.z - dir.x*up.z), up.x*dir.y - dir.x*up.y);
	Matrix4f rotMatrix = Matrix4f::rotateVector(normal, angle, 0.0);


	// Fill the vertex buffer with positions
	alpha = 0;   // angle of the longtidute 
	deltaAlpha = (float)360.0 / (numLong);	// increment of beta

	for (i = -numRows/2; i <= numRows/2; i += 2) {
		for (j = 0, alpha = 45; j <= numCols; j++, alpha += deltaAlpha) {
			//if (lev > 0)
			//{
			//	float dx = lev / (1.0f + lev);
			//	pos.x = cos(DegreeToRadians(alpha)) - dx;
			//	pos.z = sin(DegreeToRadians(alpha)) - (1.0f - dx*dx);
			//	pos.y = (float)i;
			//	pos.w = 1.0;
			//}
			//else{
				pos.x = cos(DegreeToRadians(alpha));
				pos.z = sin(DegreeToRadians(alpha));
				pos.y = (float)i;
				pos.w = 1.0;
			//}
			newPos = rotMatrix*pos;
			Vector4f vpos = Vector4f(newPos.to3D(),1.0);

			//	vtx.push_back(Vertex(pos, pos)); //  second pos = col : create color based on position
			vtx.push_back(Vertex(vpos, col));

		}
	}


	// fill the index buffer


	for (i = 0; i < numRows/2; i++) {
		for (j = 0; j < numCols; j++) {
			// fill indices for the quad
			ind.push_back(i * (numCols + 1) + j);
			ind.push_back(i * (numCols + 1) + j + 1);
			ind.push_back((i + 1) * (numCols + 1) + j + 1);

			ind.push_back(i * (numCols + 1) + j);
			ind.push_back((i + 1) * (numCols + 1) + j + 1);
			ind.push_back((i + 1) * (numCols + 1) + j);
		}
	}


	return(0);

}

 //int  Cylinder::CreateCylinder(float cylinder_radius , float cylinder_height , int num_cylinder_samples ) {
	// GLuint *cylinderVertexBuffer, *cylinderElementBuffer;
	// // Number of vertices and faces to be created
	// const GLuint vertex_num = num_cylinder_samples * 2;
	// const GLuint face_num = num_cylinder_samples * 2 + (num_cylinder_samples - 2) * 2; // add for caps
	// // Number of attributes for vertices and faces
	// const int vertex_att = 11;  // 11 attributes per vertex: 3D position (3), 3D normal (3), RGB color (3), 2D texture coordinates (2)
	// const int face_att = 3; // Vertex indices (3)
	// GLfloat *vertex = NULL;
	// GLuint *face = NULL;
	// // Allocate memory for buffers
	// try {
	//	 vertex = new GLfloat[vertex_num * vertex_att];
	//	 face = new GLuint[face_num * face_att];
	// }
	// catch (std::exception &e) {
	//	 throw e;
	// }
	// // Create vertices 
	// float theta; // Angles for circles
	// glm::vec3 vertex_position;
	// glm::vec3 vertex_normal;
	// glm::vec3 vertex_color;
	// glm::vec2 vertex_coord; 
	// 
	// for (int i = 0; i < num_cylinder_samples; i++) {
	//	 theta = 2.0*glm::pi<GLfloat>()*i / num_cylinder_samples;
	//	 vertex_normal = glm::vec3(cos(theta), 0, sin(theta));
	//	 vertex_position = glm::vec3(0, -cylinder_height / 2, 0) + vertex_normal * cylinder_radius;
	//	 vertex_color = glm::vec3(1.0 - ((float)i / (float)num_cylinder_samples),
	//		 ((float)i / (float)num_cylinder_samples),
	//		 ((float)i / (float)num_cylinder_samples));
	//	 vertex_coord = glm::vec2(theta / 2.0 * glm::pi<GLfloat>(), 0);
	//	 // Add vectors to the data buffer
	//	 for (int k = 0; k < 3; k++) {
	//		 vertex[(i * 2) * vertex_att + k] = vertex_position[k];
	//		 vertex[(i * 2) * vertex_att + k + 3] = vertex_normal[k];
	//		 vertex[(i * 2) * vertex_att + k + 6] = vertex_color[k];
	//	 }
	//	 vertex[(i * 2) * vertex_att + 9] = vertex_coord[0];
	//	 vertex[(i * 2) * vertex_att + 10] = vertex_coord[1];
	//	 vertex_normal = glm::vec3(cos(theta), 0, sin(theta));
	//	 vertex_position = glm::vec3(0, cylinder_height / 2, 0) + vertex_normal * cylinder_radius;
	//	 vertex_color = glm::vec3(1.0 - ((float)i / (float)num_cylinder_samples),
	//		 ((float)i / (float)num_cylinder_samples),
	//		 ((float)i / (float)num_cylinder_samples));
	//	 vertex_coord = glm::vec2(theta / 2.0 * glm::pi<GLfloat>(), 1.0);
	//	 // Add vectors to the data buffer
	//	 for (int k = 0; k < 3; k++) {
	//		 vertex[(i * 2 + 1) * vertex_att + k] = vertex_position[k];
	//		 vertex[(i * 2 + 1) * vertex_att + k + 3] = vertex_normal[k];
	//		 vertex[(i * 2 + 1) * vertex_att + k + 6] = vertex_color[k];
	//	 }
	//	 vertex[(i * 2 + 1) * vertex_att + 9] = vertex_coord[0];
	//	 vertex[(i * 2 + 1) * vertex_att + 10] = vertex_coord[1];
	// }
	// // Create triangles
	// for (int i = 0; i < num_cylinder_samples; i++) {
	//	 // Two triangles per quad
	//	 glm::vec3 t1((i * 2) % (num_cylinder_samples * 2), (i * 2 + 1) % (num_cylinder_samples * 2), (i * 2 + 2) % (num_cylinder_samples * 2));
	//	 glm::vec3 t2((i * 2 + 1) % (num_cylinder_samples * 2), (i * 2 + 2) % (num_cylinder_samples * 2), (i * 2 + 3) % (num_cylinder_samples * 2));
	//	 // Add two triangles to the data buffer
	//	 for (int k = 0; k < 3; k++) {
	//		 face[i * face_att * 2 + k] = (GLuint)t1[k];
	//		 face[i * face_att * 2 + k + face_att] = (GLuint)t2[k];
	//	 }
	// }
	// for (int i = 0; i < num_cylinder_samples - 2; i++) {
	//	 glm::vec3 t1(0, i * 2 + 2, i * 2 + 4);
	//	 glm::vec3 t2(1, i * 2 + 3, i * 2 + 5);
	//	 for (int k = 0; k < 3; k++) {
	//		 face[num_cylinder_samples * 2 * face_att + (i * face_att * 2) + k] = (GLuint)t1[k];
	//		 face[num_cylinder_samples * 2 * face_att + (i * face_att * 2) + face_att + k] = (GLuint)t2[k];
	//	 }
	// }
	// // Create OpenGL buffers and copy data
	// //GLuint vbo, ebo;
	// GLuint* vbo = new GLuint();
	// GLuint* ebo = new GLuint();
	// // Create buffer for vertices
	// glGenBuffers(1, vbo);
	// glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	// glBufferData(GL_ARRAY_BUFFER, vertex_num * vertex_att * sizeof(GLfloat), vertex, GL_STATIC_DRAW);
	// cylinderVertexBuffer = vbo;
	// // Create buffer for faces
	// glGenBuffers(1, ebo);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, face_num * face_att * sizeof(GLuint), face, GL_STATIC_DRAW);
	// cylinderElementBuffer = ebo;
	// // Free data buffers
	// delete[] vertex;
	// delete[] face;
	// // Return number of elements in array buffer
	// return face_num * face_att;
 //}
