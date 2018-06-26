//#define GLUT_DISABLE_ATEXIT_HACK
#define BOOST_SYSTEM_NO_DEPRECATED

#include "Solution.h"
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <opencv/cv.h> 
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <boost/system/error_code.hpp>

#define WINW 800
#define WINH 800
#define WINL 800 
#define Max_level_Num 4
#define PI 3.14159265359


static int numberOfnodes;
static double interval;
static int endpoints;
static int max_level;
float yrot = 0;


Graph graph;



void init(Graph *graph)
{
		std::cout << "***** Placenta Project ******" << std::endl;
		std::cout << "Please enter the  number of nodes [default = 50000]: ";
		numberOfnodes = 50000;
		interval = 800;
		std::string input1, input2;
		std::getline(std::cin, input1);
		if (!input1.empty()) {
			std::istringstream stream(input1);
			stream >> numberOfnodes;
		}
		std::cout << numberOfnodes << std::endl;
		std::cout << "Please enter the interval [default = 800]: ";
		std::getline(std::cin, input2);
		if (!input2.empty()) {
			std::istringstream stream(input2);
			stream >> interval;
		}
		std::cout << interval << std::endl;

}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	std::cout << "***** Placenta Project ******" << std::endl;
	
	numberOfnodes = 50000;
	interval = 800;
	endpoints = 10;
	max_level = 4;

	std::cout << "Please enter the  number of nodes [default = 50000]: ";
	std::string input1, input2, input3, input4;
	std::getline(std::cin, input1);
	if (!input1.empty()) {
		std::istringstream stream(input1);
		stream >> numberOfnodes;
	}
	std::cout << numberOfnodes << std::endl;

	std::cout << "Please enter the number of endpoints [default = 10]: ";
	std::getline(std::cin, input3);
	if (!input3.empty()) {
		std::istringstream stream(input3);
		stream >> endpoints;
	}
	std::cout << endpoints << std::endl;

	std::cout << "Please enter the interval [default = 800]: ";
	std::getline(std::cin, input2);
	if (!input2.empty()) {
		std::istringstream stream(input2);
		stream >> interval;
	}
	std::cout << interval << std::endl;

	std::cout << "Please enter the maximum level [default = 4]: ";
	std::getline(std::cin, input4);
	if (!input4.empty()) {
		std::istringstream stream(input4);
		stream >> max_level;
	}
	std::cout << max_level << std::endl;

	cv::Mat pathImage(interval, interval, CV_8UC3);
	cv::Mat planeImage(interval, interval, CV_8UC3);

//========================= Generate the graph ==========================================

	Graph graph(numberOfnodes,endpoints,interval);
	graph.setMaxLev(max_level);

	Solution sol;
	sol.initOpenGL();
	sol.initSolution(&graph, pathImage,planeImage);
	Solution::setSolution(&sol);

	sol.render();

	glutMainLoop();

	return 0;
}


