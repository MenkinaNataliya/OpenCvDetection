#include "stdafx.h"
#include "EquationOfLine.h"



EquationOfLine::EquationOfLine(cv_Point _a, cv_Point _b){
	A = _a.y - _b.y;
	B = _b.x - _a.x;
	C = _a.x*_b.y - _b.x*_a.y;
	pA = _a;
	pB = _b;
}


EquationOfLine::~EquationOfLine()
{
}
EquationOfLine::EquationOfLine()
{
	A = 0;
	B =0;
	C = 0;

}

float EquationOfLine::GetLength() const
{
	return sqrt(pow(pB.x - pA.x,2) + pow(pB.y - pA.y, 2));
}

float EquationOfLine::CalculationOfAngleBetweenLine(EquationOfLine _b) const
{
	float cosinus = (A * _b.A + B*_b.B) / (sqrt(A *A + B*B) * sqrt(_b.A *_b.A + _b.B *_b.B));

	return acos(cosinus)* 180.0 / 3.14159265;
}
