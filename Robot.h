#pragma once
#include "Header.h"
class Robot
{
public:

	cv_Point* currentLocation;


	//������ ���������� ��������� �� �� (�.�. ���� ��������� ����� 0, ������ ����� ������� �� ��), 
	//������� �������������� � ������ �������.
	//�������� �� 0�� 90 ����� ������� � 1 �������� ���������
	//91-180 - 4 ��������
	//181 - 270 - 3 ��������
	//271-359 - 2 ��������
	float deviationFromMainAxis;
	
	Robot(cv_Point* location);
	Robot(cv_Point* location, float deviation);
	~Robot();
};




