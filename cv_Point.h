#pragma once
class cv_Point
{
public:
	int x;
	int y;

	double DistanceCalculation(cv_Point dst) const;
	cv_Point();
	cv_Point(int _x, int _y);
	~cv_Point();
};

