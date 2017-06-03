#pragma once
class cv_Point
{
public:
	int x;
	int y;

	float DistanceCalculation(cv_Point dst) const;
	cv_Point();
	cv_Point(int _x, int _y);
	~cv_Point();
};

