#pragma once

#ifndef SHADER_HEADER
#define SHADER_HEADER


//=============================================================================
// shader.h
//
//Author: Doron Nussbaum (C) 2015 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. wrap around opengl shaders. 
//
//
// Description:
//--------------
//  The wrap around helps in checking the code for errors since this is a simple repetetive task.

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


#include "GL/glew.h"
//#include <windows.h>
#include "GL/nuss_math.h"


#include "GL/glut.h"



class Shader
{
public:
	Shader(void);
	~Shader(void);

	// creates a shader program from files vsFileName and fsFileName
	int createShaderProgram(char * vsFileName, char * fsFileName);
	GLuint	getProgId() const { return shaderProgramid; }

	int printShaderStatus(void);
	void useProgram(int useProg);
	int copyMatrixToShader(const Matrix4f matrix, const char* name);
	int copyMatrixToShader(const Matrix4f matrix, int location);
	int copyFloatVectorToShader(float *v, int vectorSize, int elementSize, const char* name);
	int copyIntVectorToShader(int *v, int vectorSize, int elementSize, const char* name);
	
private:
	GLuint shaderProgramid;

	char *readCode(char * fileName);
	int attachShader(char* fileName, GLenum shaderType, GLuint shaderProgramid);
	int shaderObjCheckError(GLuint shaderObjId, GLenum errorType);
	int shaderProgramCheckError(GLuint shaderProgramId, GLenum errorType);

};


#endif