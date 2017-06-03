#pragma once
#include "Header.h"
class Robot
{
	void DefineTemplate();
public:

	cv_Point currentLocation;
	vector<cv_Templates> templates;

	//������ ���������� ��������� �� �� (�.�. ���� ��������� ����� 0, ������ ����� ������� �� ��), 
	//������� �������������� � ������ �������.
	//�������� �� 0�� 90 ����� ������� � 1 �������� ���������
	//91-180 - 4 ��������
	//181 - 270 - 3 ��������
	//271-359 - 2 ��������
	int deviationFromMainAxis;
	void FollowRoute(vector<cv_Point> route);
	bool PositionYourself();
	void PrintTags();
	Robot(cv_Point location);
	Robot(cv_Point location, float deviation);
	~Robot();
};




