#include "stdafx.h"
#include "cv_Label.h"


cv_Label::cv_Label(int x, int y)
{
	location = cv_Point(x, y);
}

cv_Label::cv_Label(int x, int y, std::string comm)
{
	location = cv_Point(x, y);
	comment = comm;
}

cv_Label::cv_Label(cv_Point point)
{
	location =point;
}


cv_Label::~cv_Label()
{
}
