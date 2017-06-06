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


	//отсчет отклонени€ считаетс€ от ќ” (т.е. если знаечение равно 0, значит робот смотрит по ќ”), 
	//поворот осушествл€етс€ в правую сторону.
	//значени€ от 0до 90 робот смотрит в 1 четверти плоскости
	//91-180 - 4 четверть  
	//181 - 270 - 3 четверть
	//271-359 - 2 четверть
	int deviationFromMainAxis;
	void FollowRoute(/*vector<cv_Point> route*/);
	bool PositionYourself();
	void PrintTags();
	void Routeing(cv_Point distanation);

	Robot(cv_Point location);
	Robot(cv_Point location, float deviation);
	~Robot();
};




