#pragma once

#ifndef LINE_HEADER
#define LINE_HEADER

#include "graphicsObject.h"
class Line : public GraphicsObject
{

	
public:

	Line();
	~Line();
	static int Line::createLine(int numLong, Vertices &vtx, Indices &ind);
	//int Line::render(Shader shader);
};


#endif