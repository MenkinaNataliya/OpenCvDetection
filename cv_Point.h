#pragma once
class cv_Point
{
public:
	float x;
	float y;

	double DistanceCalculation(cv_Point dst);
	cv_Point(float _x, float _y);
	~cv_Point();
};

