
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



#include "stdio.h"
#include "stdlib.h"
#include "Shader.h"



/******************************************************************************/
// Constructor

Shader::Shader(void) : shaderProgramid(-1)
{

}


/******************************************************************************/
/* destructor */

Shader::~Shader(void)
{
}


/******************************************************************/
char * Shader::readCode(char * fileName)
{
	char * shaderCode = NULL;
	int codeLength = 0;
	FILE *fp = NULL;

	// check for error in file name

	fp = fopen(fileName, "r");	// open file and check for errors
	if (fp == NULL) { return NULL; }

	// fine the length of code
	fseek(fp, 0L, SEEK_END);
	codeLength = ftell(fp);
	rewind(fp);		// could use fseek(fp, 0L, SEEK_SET)

	if (codeLength > 0) {
		// allocated space for code and check for errors
		shaderCode = (char *)malloc(codeLength + 1);	// add a space for the '\0'
		if (shaderCode == NULL) return(NULL);
		memset((void *)shaderCode, 0, codeLength + 1);
		fread((void *)shaderCode, sizeof(char), codeLength, fp);
		shaderCode[codeLength] = 0;
	}

	if (fp != NULL) fclose(fp);

	return shaderCode;
}



/******************************************************************************/
/* create the shaders

input:
fileName - file name of shader.  Path is assume to be the project.
Otherwise, pass in the directory as well

shaderType - can either be vertex shader or fragment shader.  The type
must correspond to the OpenGL shader types.

Output:
shaderid - a handle to the shader.

return:
0 - if successfull
-1 if error

*/

int Shader::attachShader(char* fileName, GLenum shaderType, GLuint shaderProgramId)
{
	char *code = NULL;
	int rc = 0;
	GLuint shaderId=-1;

	// read the shader code from the file
	code = readCode(fileName);
	if (code == NULL)
	{
		rc = -1;
		goto err;
	}
	

	// read the code
	code = readCode(fileName);
	if (code == NULL) return(-1);

	// create a shader handle
	shaderId = glCreateShader(shaderType);

	// provide the source to the shaders
	glShaderSource(shaderId, 1, &code, NULL);
	
	// compile the code
	glCompileShader(shaderId);
	rc = shaderObjCheckError(shaderId, GL_COMPILE_STATUS);
	if (rc != 0) {
		rc = -2;
		goto err;
	}

	// link the shader object to the main program
	// connect the shaders subprogram to the "main" program
	glAttachShader(shaderProgramId,shaderId);
	rc = glGetError();
	if (rc != GL_NO_ERROR) {
		fprintf(stderr, "error in attach shaders \n");
		rc = -3;
		goto err;
	}

	glLinkProgram(shaderProgramid);

	// check for errors
	rc = shaderProgramCheckError(shaderProgramid, GL_LINK_STATUS);
	if (rc != 0) {
		rc = -4;
		goto err;
	}

	// validate the program
	glValidateProgram(shaderProgramid);
	// check for errors
	rc = shaderProgramCheckError(shaderProgramid, GL_VALIDATE_STATUS);
	if (rc != 0) {
		rc = -4;
		goto err;
	}


err:
	if (shaderId != -1) 	glDeleteShader(shaderId);

	if (code != NULL) free(code);	// free the source code of the shader
	return(rc);

}


/****************************************************************************/
// checks the status of the shader object 


int Shader::shaderObjCheckError(GLuint shaderObjId, GLenum errorType)

{
	int rc = 0;
	// check for errors
	glGetShaderiv(shaderObjId, errorType, &rc);
	if (rc == GL_FALSE) {
		rc = -1;
		GLsizei length;
		GLsizei bufSize = 0;
		char *error = NULL;
		switch (errorType) {
		case GL_COMPILE_STATUS:
			fprintf(stderr, "Error when compiling a shader program \n");
			break;
		default:
			fprintf(stderr, "Error when creating a shader program \n");
			break;
		}

		//fputs("Error when creating a shader \n",stderr);
		glGetProgramiv(shaderObjId, GL_INFO_LOG_LENGTH, &bufSize);
		error = (char *)malloc(bufSize*sizeof(char) + 1);
		error[0] = '\0';
		if (error != NULL) {
			glGetProgramInfoLog(shaderObjId, bufSize, &length, error);
			fprintf(stderr, "%s \n", error);
			free(error);
		}
	} 	else rc = 0;
	return(rc);
}

/****************************************************************************/
// Check the status of the shader program


int Shader::shaderProgramCheckError(GLuint shaderProgramId, GLenum errorType)

{
	int rc = 0;
	// check for errors
	glGetProgramiv(shaderProgramId, errorType, &rc);
	if (rc == GL_FALSE) {
		rc = -1;
		GLsizei length;
		GLsizei bufSize = 0;
		char *error = NULL;
		switch (errorType) {
		case GL_LINK_STATUS:
			fprintf(stderr, "Error when linking  a shader program \n");
			break;
		case GL_VALIDATE_STATUS:
			fprintf(stderr, "Error when validating a shader program \n");
			break;
		default:
			fprintf(stderr, "Error when creating a shader program \n");
			break;
		}

		//fputs("Error when creating a shader \n",stderr);
		glGetProgramiv(shaderProgramId, GL_INFO_LOG_LENGTH, &bufSize);
		error = (char *)malloc(bufSize*sizeof(char) + 1);
		error[0] = '\0';
		if (error != NULL) {
			glGetProgramInfoLog(shaderProgramId, bufSize, &length, error);
			fprintf(stderr, "%s \n", error);
			free(error);
		}
	} 	else rc = 0;
	return(rc);
}

/***************************************************************************************************************/

/* creates a shader program from files vsFileName and fsFileName

input:
vsFileName - name of vertex shader

fsFileName - name of fragment shader


output:
none


return:
0 if succssfull
!=0 - if error

*/


int Shader::createShaderProgram(char * vsFileName, char * fsFileName)
{

	int rc = 0;

	// get a handle to the shader program
	shaderProgramid = glCreateProgram();

	// attached the shaders if the file name is not NULL
	if (vsFileName != NULL) {
		rc = attachShader(vsFileName, GL_VERTEX_SHADER, shaderProgramid);
		if (rc != 0) {
			rc = 1;
			goto err;
		}
	} 
	if (fsFileName != NULL) {
		rc = attachShader(fsFileName, GL_FRAGMENT_SHADER, shaderProgramid);
		if (rc != 0) {
			rc = 2;
			goto err;
		}
	}
	

	// validate the shader program
	// validate the program
	glValidateProgram(shaderProgramid);
	// check for errors
	rc = shaderProgramCheckError(shaderProgramid, GL_VALIDATE_STATUS);
	if (rc != 0) {
		rc = -4;
		goto err;
	}

	//  print status
	Shader::printShaderStatus();
	err:
	// check status of program
	return(rc);
}






/*********************************************************************/
// print the status and some information about the linked shader program

int Shader::printShaderStatus(void)
{
	int rc;

	glGetProgramiv(shaderProgramid, GL_VALIDATE_STATUS, &rc);
	if (rc == GLU_TRUE) {
		fprintf(stderr, "shader is valid status = %d \n", rc);
	}
	else {
		fprintf(stderr, "shader is not valid status = %d \n", rc);
	}

	rc = 0;
	glGetProgramiv(shaderProgramid, GL_INFO_LOG_LENGTH, &rc);
	if (rc == 0) {
		fprintf(stderr, "shader program info log length= %d \n", rc);
	}
	else {
		char s[1024];
		int length = 0;
		s[0] = 0;
		glGetProgramInfoLog(shaderProgramid, 1024, &length, s);
		fprintf(stderr, "shader info log = %s \n", s);
	}

	glGetProgramiv(shaderProgramid, GL_ATTACHED_SHADERS, &rc);
	fprintf(stderr, "Number of attached shaders = %d \n", rc);

	glGetProgramiv(shaderProgramid, GL_ACTIVE_ATTRIBUTES, &rc);
	fprintf(stderr, "Number of active attributes = %d \n", rc);

	glGetProgramiv(shaderProgramid, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &rc);
	fprintf(stderr, "Max name length (active attriute) = %d \n", rc);

	glGetProgramiv(shaderProgramid, GL_ACTIVE_UNIFORMS, &rc);
	fprintf(stderr, "Number of active uniforms = %d \n", rc);

	glGetProgramiv(shaderProgramid, GL_ACTIVE_UNIFORM_MAX_LENGTH, &rc);
	fprintf(stderr, "Max name length (active uniform) = %d \n", rc);

	return 0;
}


/*********************************************************************/
/* Enable or disable shader program to be current program

input:
useProg ==  0 disable the progrdam
useProg != 0 set the shader to be used

*/



void Shader::useProgram(int useProg){
	if (useProg != 0){
		glUseProgram(shaderProgramid);
	}
	else{
		glUseProgram(0);
	}
}


/*********************************************************************/
/*
Copy a matrix to the shader
This is a helper function since this operation is used quite often


*/
int Shader::copyMatrixToShader(Matrix4f matrix, const char* name){
	int location;

	location = glGetUniformLocation(getProgId(), name);
	assert(location != -1);
	if (location == -1) return (-1);
	glUniformMatrix4fv(location, 1, true, matrix.data());
	return(0);
}


/*********************************************************************/
/*
Copy a matrix to the shader
This is a helper function since this operation is used quite often


*/
int Shader::copyMatrixToShader(Matrix4f matrix, int location){

	glUniformMatrix4fv(location, 1, true, matrix.data());
	return(0);
}



/*********************************************************************/
/*
Copy an array of floats to the shader
This is a helper function since this operation is used quite often
input
v - the array
vectorSize - number of elements in the array
elementSize - number of floats in an element
name - name of variable in the shader


*/
int Shader::copyFloatVectorToShader(float *v, int vectorSize, int elementSize, const char* name)
{
	int location;
	int rc;
	float loaded[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };

#if NUSS_DEBUG
	printf("\n");
	printf("loading %s size=%d", name, vectorSize);
	for (i = 0; i < elementSize; i++) {
		printf("[%d]= %f,", i, v[i]);
	}
	printf("\n");
#endif
	location = glGetUniformLocation(getProgId(), name);
	//assert(location != -1);  DN
	if (location == -1) return (-1);
	switch (elementSize) {
	case 1:
		glUniform1fv(location, vectorSize, v);
		break;
	case 2:
		glUniform2fv(location, vectorSize, v);
		break;
	case 3:
		glUniform3fv(location, vectorSize, v);
		break;
	case 4:
		glUniform4fv(location, vectorSize, v);
		break;
	}
	rc = glGetError();
	if (rc != GL_NO_ERROR) {
		switch (rc) {
		case GL_INVALID_VALUE:
			printf("GL_INVALID_VALUE is generated if program is not a value generated by OpenGL.\n");
			break;
		case GL_INVALID_OPERATION:
			printf("GL_INVALID_OPERATION is generated if program is not a program object. or \n");
			printf("GL_INVALID_OPERATION is generated if program has not been successfully linked. or \n");
			printf("GL_INVALID_OPERATION is generated if location does not correspond to a valid uniform variable location for the specified program object.");
			break;
		default:
			printf("openGL unknown error \n");
		}		rc = -1;
	}



#if NUSS_DEBUG

	glGetUniformfv(getProgId(), location, loaded);
	rc = glGetError();
	if (rc != GL_NO_ERROR) {
		switch (rc) {
		case GL_INVALID_VALUE:
			printf("GL_INVALID_VALUE is generated if program is not a value generated by OpenGL.\n");
			break;
		case GL_INVALID_OPERATION:
			printf("GL_INVALID_OPERATION is generated if program is not a program object. or \n");
			printf("GL_INVALID_OPERATION is generated if program has not been successfully linked. or \n");
			printf("GL_INVALID_OPERATION is generated if location does not correspond to a valid uniform variable location for the specified program object.");
			break;
		default:
			printf("openGL unknown error \n");
		}		rc = -1;
	}

	printf("       loaded   ");
	for (i = 0; i < elementSize; i++) {
		printf("[%d]= %f,", i, loaded[i]);
	}
	printf("\n");
#endif
	return(0);
}

/*********************************************************************/
/*
Copy an array of integers to the shader
this is a helper function since this operation is used quite often

input
v- the array
vectorSize - number of elements in the array
elementSize - number of integers in an element
name - name of variable in the shader

*/
int Shader::copyIntVectorToShader(int *v, int vectorSize, int elementSize, const char* name){
	int location;
	int rc;
	int loaded[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };


#if NUSS_DEBUG
	printf("\n");
	printf("loading %s size=%d", name, vectorSize);
	for (i = 0; i < elementSize; i++) {
		printf("[%d]= %d,", i, v[i]);
	}
	printf("\n");
#endif
	location = glGetUniformLocation(getProgId(), name);
	// assert(location != -1);  DN
	if (location == -1) return (-1);
	switch (elementSize) {
	case 1:
		glUniform1iv(location, vectorSize, v);
		break;
	case 2:
		glUniform2iv(location, vectorSize, v);
		break;
	case 3:
		glUniform3iv(location, vectorSize, v);
		break;
	case 4:
		glUniform4iv(location, vectorSize, v);
		break;
	}
	rc = glGetError();
	if (rc != GL_NO_ERROR) {
		switch (rc) {
		case GL_INVALID_VALUE:
			printf("GL_INVALID_VALUE is generated if program is not a value generated by OpenGL.\n");
			break;
		case GL_INVALID_OPERATION:
			printf("GL_INVALID_OPERATION is generated if program is not a program object. or \n");
			printf("GL_INVALID_OPERATION is generated if program has not been successfully linked. or \n");
			printf("GL_INVALID_OPERATION is generated if location does not correspond to a valid uniform variable location for the specified program object.");
			break;
		default:
			printf("openGL unknown error \n");
		}		rc = -1;
	}
#if NUSS_DEBUG
	glGetUniformiv(getProgId(), location, loaded);
	printf("       loaded   ");
	for (i = 0; i < elementSize; i++) {
		printf("[%d]= %d ,", i, loaded[i]);
	}
	printf("\n");
#endif
	return(0);
}

