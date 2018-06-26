#include "line.h"


Line::Line()
{
}


Line::~Line()
{
}

/**************************************************************************************/
/* this function creates a line.  The cylinder is genreated using two arrays.  The vertex data array which
contains the vertex data (geometry) and an index array which contains the topology of the triangles.  The trainagles
are stored in the index array as a triangle list.

Input
verrtices and width of the line

Ouptut:
vtx - an array (vector of vertices).  Currently the function computes the position and colour information.


Return:
the function returns 0 is successful.


*/

int Line::createLine(int numLong, Vertices &vtx, Indices &ind)

{
	//int i ;
	//Vector4f pos;
	//Vector4f col;

	//col = Vector4f(0.5, 0, 0, 1);

	//vertexes.resize(0);
	//ind.resize(0);

	//// Fill the vertex buffer with positions

	//for (i = 0; i <2; i++) {
	//	//pos.x = ;
	//	//pos.z = ;
	//	//pos.y = ;
	//	//pos.w = 1.0;
	//		//	vtx.push_back(Vertex(pos, pos)); //  second pos = col : create color based on position
	//	vertexes.push_back(Vertex(pos, col));
	//	ind.push_back(i);
	//}

	int i, j, k;
	int numRows;
	int numCols;
	Vector4f pos;
	Vector4f col;
	float alpha;
	float deltaAlpha;

	numRows = 1;  // number of horizonal slabs
	numCols = numLong;	// number of vertical slabs

	col = Vector4f(0.5, 0, 0, 1);

	vtx.resize(0);

	ind.resize(0);

	// Fill the vertex buffer with positions
	alpha = 0;   // angle of the longtidute 
	deltaAlpha = (float)360.0 / (numLong);	// increment of beta

	for (i = -1; i <= 1; i +=2) {
		for (j = 0, alpha = 45; j <= numCols; j++, alpha += deltaAlpha) {
			pos.x = cos(DegreeToRadians(alpha));
			pos.z = sin(DegreeToRadians(alpha));
			pos.y = (float)i;
			pos.w = 1.0;

			//	vtx.push_back(Vertex(pos, pos)); //  second pos = col : create color based on position
			vtx.push_back(Vertex(pos, col));


		}
	}

	// fill the index buffer

	//k = 0;
	//for (i = 0; i < numRows; i++) {
	//	for (j = 0; j < numCols; j++) {
	//		// fill indices for the quad
	//		ind.push_back(i * (numCols + 1) + j);
	//		ind.push_back(i * (numCols + 1) + j + 1);
	//		ind.push_back((i + 1) * (numCols + 1) + j + 1);

	//		ind.push_back(i * (numCols + 1) + j);
	//		ind.push_back((i + 1) * (numCols + 1) + j + 1);
	//		ind.push_back((i + 1) * (numCols + 1) + j);
	//	}
	//}
	// fill the index buffer of the top plate	
	for (i = 0, j = numCols + 2; i < numCols - 2; i++, j++) {
		ind.push_back(numCols + 1);
		ind.push_back(j);
		ind.push_back(j + 1);
	}


	// fill the index buffer of the bottom plate	
	for (i = 0, j = 1; i < numCols - 2; i++, j++) {
		ind.push_back(0);
		ind.push_back(j + 1);
		ind.push_back(j);
	}

	return(0);

}





