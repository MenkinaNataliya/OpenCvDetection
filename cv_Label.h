#pragma once
#include "cv_Point.h"
#include <string>

class cv_Label
{
public:
	cv_Point location;
	std::string comment;

	cv_Label(int x, int y);
	cv_Label(cv_Point point);
	cv_Label(int x, int y, std::string comm);
	~cv_Label();
};

