#include "stdafx.h"
#include "cv_Templates.h"


cv_Templates::cv_Templates(int x, int y)
{
	location = cv_Point(x, y);
}

cv_Templates::cv_Templates(int x, int y, std::string comm)
{
	location = cv_Point(x, y);
	comment = comm;
}

cv_Templates::cv_Templates(cv_Point point)
{
	location =point;
}


cv_Templates::~cv_Templates()
{
}
