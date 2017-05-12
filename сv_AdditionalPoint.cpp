#include "stdafx.h"
#include "cv_AdditionalPoint.h"


cv_AdditionalPoint::cv_AdditionalPoint(cv_Point * loc)
{
	location = loc;
	isInvolved = false;
}


cv_AdditionalPoint::~cv_AdditionalPoint()
{
}
