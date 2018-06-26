#pragma once
#ifndef SOLUTION_HEADER
#define SOLUTION_HEADER

//=============================================================================
// solution.h
//
//===========================================================================

#include "GL/glew.h"
#include "GL/glut.h"

#include "camera.h"
#include "Shader.h"
#include "sphere.h"
#include "cylinder.h"
#include "graph.h"
#include "line.h"
#include <opencv/cv.h> 
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "Shape.h"

#define UPDATE_RENDERRED_OBJECTS 1000
#define FRAME_TIME 30

static float yView = 0;


class Solution
{
public:
	Solution();
	~Solution();
	// initializing the opengl functions and windows
	int initOpenGL();


	// static callback function 
	static void renderCB();
	static void keyboardCB(unsigned char key, int x, int y);
	static void mouseClickCB(int button, int state, int x, int y);
	static void specialKeyboardCB(int key, int x, int y);
	static void winResizeCB(int width, int height);
	static void timerCB(int operation);

	static void setSolution(Solution * _sol);
	//void generateVessles(node *p, Shader shader/*, Vertices vtx, Indices ind*//*, Sphere testSphere, std::vector<Sphere> Spheres*/);
	// generaL shader that will be used by all objects
	// initialization of the solution
	int initSolution(Graph *g,cv::Mat img, cv::Mat plane);
	bool findId(Cylinder *vessel, std::vector<Cylinder*> vessels);

	static Graph *graph;

	std::vector<std::pair<cv::Point3f, int >> intersectedPoints;
	std::vector<cv::Point3f> sites;

	Shader shader;

	std::vector<Sphere> Spheres;
	std::vector<std::vector<Sphere>> AllLevelSpheres;

	Sphere testSphere;
	Sphere basicSphere;
	std::vector<Sphere> basicSpheres;

	Cylinder testCylinder;
	std::vector<Cylinder*> vessels;
	std::vector<std::vector<Cylinder>> AllVessels;
	Line line;
	int numFrames;

	static Solution *sol;
	std::list<std::pair<node*, float>> path;
	std::vector<std::vector<node*>> AllParents[4];
	
	std::set<node*> visited;

	std::vector<node*> selecteds;
	std::vector<node*> selectedt;
	std::vector<node*> sources;
	std::vector<node* > target;
	std::vector<node*> targetPoints;
	std::vector<std::vector<node*>> endPoints;
	std::vector<node*> sourceTonextLev;

	std::vector<Shape*> shapes;

	Camera cam;

	Shader gouraudShader;
	Shader phongShader;

	float shininess;
	int useAmbient;
	int useDiffuse;
	int useSpecular;

	void render();
	void keyboard(unsigned char key, int x, int y);
	void mouseClick(int button, int state, int x, int y);
	void specialKeyboard(int key, int x, int y);
	void winResize(int width, int height);
	int timer(int operation);

	int updateObjects(int numFrames);
	int maximum_lev;

};


#endif