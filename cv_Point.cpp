#include "stdafx.h"
#include "cv_Point.h"
#include <math.h>


cv_Point::cv_Point(float _x, float _y)
{
	x = _x; y = _y;
}


cv_Point::~cv_Point()
{
}

double cv_Point::DistanceCalculation(cv_Point dst)
{
	return sqrt(pow((dst.x - x), 2) + pow((dst.y - y), 2));
}
