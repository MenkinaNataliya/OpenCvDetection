#include "stdafx.h"
#include "cv_Point.h"
#include <math.h>


cv_Point::cv_Point(int _x, int _y)
{
	x = _x; y = _y;
}

cv_Point::cv_Point()
{
}

cv_Point::~cv_Point()
{
}

double cv_Point::DistanceCalculation(cv_Point dst) const
{
	return sqrt(pow((dst.x - x), 2) + pow((dst.y - y), 2));
}
