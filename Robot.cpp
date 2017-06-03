#include "stdafx.h"
#include "Robot.h"
#include "EquationOfLine.h"

#include "FindQr.h"
#include "Header.h"



Robot::Robot(cv_Point location)
{// в самом начале пути робот стоит прямо по отношению к основной системе координат
	currentLocation = location;
	deviationFromMainAxis = 0;
	DefineTemplate();
}

Robot::Robot(cv_Point location, float deviation)
{
	currentLocation = location;
	deviationFromMainAxis = deviation;
	DefineTemplate();
}


Robot::~Robot()
{
}

void Robot::DefineTemplate()
{
	templates.push_back(cv_Templates(1, 7));
	templates.push_back(cv_Templates(3, 2));
	templates.push_back(cv_Templates(3, 16));
	templates.push_back(cv_Templates(4, 23));
	templates.push_back(cv_Templates(6, 10));
	/* ********************/
	templates.push_back(cv_Templates(6, 19));
	templates.push_back(cv_Templates(9, 5));
	templates.push_back(cv_Templates(9, 22));
	templates.push_back(cv_Templates(10, 14));
	templates.push_back(cv_Templates(11, 18));
	templates.push_back(cv_Templates(14, 9));
	templates.push_back(cv_Templates(16, 3));
	templates.push_back(cv_Templates(16, 16));
	templates.push_back(cv_Templates(18, 22));
	templates.push_back(cv_Templates(19, 7));
	templates.push_back(cv_Templates(20, 1));
	templates.push_back(cv_Templates(21, 16));
}


void Robot::PrintTags()
{
	for (auto templ : templates)
	{
		cout << "\t (" << templ.location.x << "; " << templ.location.y << ")" << endl;
	}
}

bool Robot::PositionYourself()
{
	int start = deviationFromMainAxis;
	deviationFromMainAxis += 30;
	SendSignal("r30 s");
	cv_Point point = FindAnyCode();
	while(point.x==-1 && point.y==-1)
	{
		deviationFromMainAxis += 30;
		if (deviationFromMainAxis >= 360) deviationFromMainAxis = abs(360 - deviationFromMainAxis);
		SendSignal("r30 s");
		point = FindAnyCode();
		if (deviationFromMainAxis == start) {/*все мы потерялись и не можум найти метку поэтому шлем сообщение оператору что бы он нас скоорденировал*/
			SendErrorMessage("The robot was lost");
			return false;
		}
	}
	currentLocation = point;
	return true;

}

void Robot::FollowRoute(vector<cv_Point> route)
{ //route[0] - текущее местоположение

	for (int i = 1; i < route.size(); i++)
	{
		deviationFromMainAxis = SendSignals(route[i - 1], route[i], deviationFromMainAxis);
		cout << route[i].x << " " << route[i].y << endl;
		if(FindCode(route[i])) 
			currentLocation = route[i];
		else
		{
			if (PositionYourself())
			{//перестраиваем карту
				vector<cv_Point> route = RoutePlanning(currentLocation, route[route.size()-1], templates);//составили маршрут
				FollowRoute(route);
				return;
			}
			else return;
		}
	}SendSignal("Point");

}
