#pragma once
#include "Header.h"
#include "cv_Label.h"

class Robot
{
	void DefineTemplate();
	bool dstIsLabel;
	vector<cv_Point> route;
public:

	cv_Point currentLocation;
	vector<cv_Label> labels;


	//������ ���������� ��������� �� �� (�.�. ���� ��������� ����� 0, ������ ����� ������� �� ��), 
	//������� �������������� � ������ �������.
	//�������� �� 0�� 90 ����� ������� � 1 �������� ���������
	//91-180 - 4 ��������  
	//181 - 270 - 3 ��������
	//271-359 - 2 ��������
	int deviationFromMainAxis;
	void FollowRoute(/*vector<cv_Point> route*/);
	bool PositionYourself();
	void PrintTags();
	void Routeing(cv_Point distanation);

	Robot(cv_Point location);
	Robot(cv_Point location, float deviation);
	~Robot();
};




