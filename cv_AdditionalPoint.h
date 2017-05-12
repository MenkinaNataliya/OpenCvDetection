#pragma once
#include "cv_Point.h"

class cv_AdditionalPoint
{
public:
	cv_Point *location;
	bool isInvolved;
	cv_AdditionalPoint(cv_Point * loc);
	~cv_AdditionalPoint();
};

