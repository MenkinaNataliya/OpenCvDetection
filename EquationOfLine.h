#pragma once
#include "cv_Point.h"
#include "Header.h"

class EquationOfLine
{
public:
	float A;
	float B;
	float C;
	cv_Point pA;
	cv_Point pB;
	EquationOfLine(cv_Point _a, cv_Point _b);
	~EquationOfLine();
	EquationOfLine();
	float GetLength() const;

	float CalculationOfAngleBetweenLine(EquationOfLine _b) const;
	
};

