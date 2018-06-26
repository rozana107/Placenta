#ifndef GEOMETRY_COMMON
#define GEOMETRY_COMMON 1


#include <vector>

#include "GL/nuss_vector.h"




struct Vertex {
	Vertex(){}

	Vertex(const Vector4f& _pos);

	Vertex(const Vector4f _pos, const Vector4f _col){
		memcpy(pos, (void *)&_pos, sizeof(pos));
		memcpy(col, (void *)&_col, sizeof(col));
	}
	Vertex(const Vector4f _pos, const Vector3f _normal, const Vector4f _col)
	{
		memcpy(pos, (void *)&_pos, sizeof(pos));
		memcpy(normal, (void *)&_normal, sizeof(normal));
		memcpy(col, (void *)&_col, sizeof(col));
	}


	Vertex(const Vector4f _pos, const Vector3f _normal, const Vector2f _texCoord, const Vector4f _col)
	{
		memcpy(pos, (void *)&_pos, sizeof(pos));
		memcpy(normal, (void *)&_normal, sizeof(normal));
		memcpy(col, (void *)&_col, sizeof(col));
		memcpy(texCoord, (void *)&_texCoord, sizeof(texCoord));
	}
	float pos[4];
	float normal[3];
	float texCoord[2];
	float col[4];
};

typedef struct Vertex Vertex;



typedef std::vector<Vertex> Vertices;
typedef std::vector<unsigned int>	Indices;

//struct LineSegment_t
//{
//	Vertices v[2];
//	Indices ind[2];
//};
#endif