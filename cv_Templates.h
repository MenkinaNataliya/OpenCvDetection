#pragma once
#include "cv_Point.h"

class cv_Templates
{
public:
	char* filename;
	cv_Point *location;


	char* comment;
	int red;
	int green;
	int blue;
	cv_Templates();
	~cv_Templates();
};

