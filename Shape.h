#ifndef _SHAPE_H_
#define _SHAPE_H_

#pragma once

#include <opencv\cv.h>
#define WHITE cv::Vec3b(255,255,255)

class Shape {
//private:
//	/*cv::Mat sh;*/
//	int rows;
//	int cols;
//	cv::Vec2d dimensions;
//	int intensity;
//	std::vector<cv::Point2f> cellRegion;
//

public:
	Shape();
	Shape(cv::Mat _shape);
	void initialize(cv::Mat _shape);
	cv::Mat ds; // resized sh
	double orientation;
	void reScaleShape(cv::Mat &dst, float ratio , int interpolation = 1);
	void rotateShape(cv::Mat &dst, double angle, double scale);
	cv::Mat getShape();
	void setShape(cv::Mat &newShape);
	cv::Vec2d getShapeDimensions();
	void calcDimentions();
	double getOrientation();
	float maxLength();
	float getLabel(cv::Point3f intersectedPoint, cv::Mat &label);
	void calcOrientation();
	int getIntensity(cv::Mat &mat,int _x,int _y);

	float cellMax, cellMin;
	float maxLen;
	cv::Mat sh;
	int rows;
	int cols;
	cv::Vec2d dimensions;
	int intensity;
	std::vector<cv::Point2f> cellRegion;
};
#endif


