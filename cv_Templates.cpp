#include "stdafx.h"
#include "cv_Templates.h"


cv_Templates::cv_Templates(int x, int y)
{
	location = new cv_Point(x, y);
}

cv_Templates::cv_Templates(int x, int y, std::string comm)
{
	location = new cv_Point(x, y);
	comment = comm;
}


cv_Templates::~cv_Templates()
{
}
