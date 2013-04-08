#pragma once
#include <vector>

class Point{
public:
	Point(){}
	Point(float x, float y, float z){
		this->x = x;
		this->y = y;
		this->z = z;
	}
	float x, y, z;
};

class EasingFunction
{
public:
	std::vector<Point> static getBezierPoints(std::vector<Point> controlPts, int nBezCurvePts);
};

