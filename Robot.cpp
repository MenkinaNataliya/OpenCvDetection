#include "stdafx.h"
#include "Robot.h"


Robot::Robot(cv_Point* location)
{// � ����� ������ ���� ����� ����� ����� �� ��������� � �������� ������� ���������
	currentLocation = location;
	deviationFromMainAxis = 0;

}

Robot::Robot(cv_Point* location, float deviation)
{
	currentLocation = location;
	deviationFromMainAxis = deviation;

}


Robot::~Robot()
{
}
