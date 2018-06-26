#include "Shape.h"


Shape::Shape() {

}

Shape::Shape(cv::Mat _shape) {
	initialize(_shape);
	sh = _shape.clone();
	cols = _shape.size().width;
	rows = _shape.size().height;
	//if (cols > rows)
	//	maxLen = cols;
	//else
	//	maxLen = rows;
}

void Shape::initialize(cv::Mat _shape){
	sh = cv::Mat::zeros(_shape.size().height, _shape.size().width, CV_8UC3);
	ds = cv::Mat::zeros(_shape.size().height, _shape.size().width, CV_8UC3);

}

void Shape::reScaleShape(cv::Mat &dst, float ratio ,int interpolation) {

	cv::Size newSize;

	if ((dst.cols * ratio) <= 4 || (dst.rows * ratio) <= 5) {
		newSize = cv::Size(dst.cols, dst.rows);
		//std::cout <<" dst size: " << dst.size() << "size after changing ratio: " << newSize << std::endl;
	}
	else {
		newSize = cv::Size((dst.cols * ratio), (dst.rows * ratio));
		//std::cout << "ratio: " << ratio << ",  cols * ratio: " << dst.cols * ratio << ",  rows * ratio: " << dst.rows * ratio << std::endl;
	}

	cv::resize(getShape(), dst, newSize, 0, 0, interpolation);
	setShape(dst);
	//ds = dst.clone();
	if (dst.cols > dst.rows)
		maxLen = dst.cols;
	else
		maxLen = dst.rows;

}
void Shape::rotateShape(cv::Mat &dst, double angle, double scale) {
	cv::Mat M = cv::getRotationMatrix2D(cv::Point2f(sh.cols / 2, sh.rows / 2), angle, scale);
	cv::warpAffine(sh, dst, M, cv::Size(sh.cols, sh.rows));
	ds = dst.clone();
	//sh = dst.clone();
}
void Shape::setShape(cv::Mat &newShape) {
	sh = newShape.clone();
}
cv::Mat Shape::getShape() {
	return sh;
}
cv::Vec2d Shape::getShapeDimensions() {
	calcDimentions();
	return dimensions;
}
void Shape::calcDimentions() {
	std::vector<int> xx, yy;
	for (int r = 0; r < sh.rows; r++) {
		for (int c = 0; c < sh.cols; c++) {
			if (this->sh.at<cv::Vec3b>(r, c) != WHITE) {
				xx.push_back(c);
				yy.push_back(r);
			}
		}
	}
	int minx = *std::min_element(xx.begin(), xx.end());
	int maxx = *std::max_element(xx.begin(), xx.end());
	int miny = *std::min_element(yy.begin(), yy.end());
	int maxy = *std::max_element(yy.begin(), yy.end());
	dimensions.val[0] = abs(minx - maxx);
	dimensions.val[1] = abs(miny - maxy);
}
double Shape::getOrientation() {
	return orientation;
}
void Shape::calcOrientation() {

}
float Shape::maxLength() {
	return maxLen;
}
float Shape::getLabel(cv::Point3f intersectedPoint, cv::Mat &label) {
	uchar lab; 
	float x = intersectedPoint.x; float z = intersectedPoint.z;
	lab = label.at<uchar>(z, x);
//****************** cell region**********************
	std::vector<float> xx,yy;
	for (int r = 0; r < label.rows; r++) {
		for (int c = 0; c < label.cols;c++) {
			if (label.at<uchar>(r, c) == lab) {
				cellRegion.push_back(cv::Point2f(c, r));
				xx.push_back(c); yy.push_back(r);
			}
		}
	}
//**********************************************
	float minx = *std::min_element(xx.begin(),xx.end());
	float miny = *std::min_element(yy.begin(), yy.end());
	float maxx = *std::max_element(xx.begin(), xx.end());
	float maxy = *std::max_element(yy.begin(), yy.end());

	float w = abs(minx - maxx); float h = abs(maxy-miny);
	if (w>=h) {
		cellMax = w;
		cellMin = h;
	}
	else
	{
		cellMax = h;
		cellMin = w;
	}
	xx.clear(); yy.clear();
	cellRegion.clear();

	return lab;
}

int Shape::getIntensity(cv::Mat &mat, int x,int y) {
	return (int (mat.at<cv::Vec3b>(y,x).val[0]+ mat.at<cv::Vec3b>(y, x).val[1] + mat.at<cv::Vec3b>(y, x).val[2])/3 );
}