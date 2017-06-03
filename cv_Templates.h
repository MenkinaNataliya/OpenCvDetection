#pragma once
#include "cv_Point.h"
#include <string>

class cv_Templates
{
public:
	cv_Point location;
	std::string comment;

	cv_Templates(int x, int y);
	cv_Templates(cv_Point point);
	cv_Templates(int x, int y, std::string comm);
	~cv_Templates();
};

