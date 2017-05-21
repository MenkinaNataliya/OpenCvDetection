#pragma once
#include "Header.h"
class Robot
{
	float deviationFromMainAxis;

public:

	cv_Point* currentLocation;
	Robot(cv_Point* location);

	~Robot();
};

