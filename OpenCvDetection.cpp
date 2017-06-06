// OpenCvDetection.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"
#include "cv_Point.h"
#include <regex>
#include "rapidjson/document.h"
#include "Robot.h"

using namespace rapidjson;

Robot robot = Robot(cv_Point(0, 0));

void GoTo()
{
	cout << "Existing tags: " << endl;
	robot.PrintTags();

	cout << "Enter the coordinates: " << endl;
	int x, y;
	cin >> x;
	cin >> y;

	robot.Routeing(cv_Point(x, y));
	//vector<cv_Point> route = RoutePlanning(robot.currentLocation, cv_Point(x, y), robot.labels);//составили маршрут
		/*СЛЕДУЕМ МАРШРУТУ*/
	robot.FollowRoute();
	//SendSignal(GenerateSignal(route, robot.deviationFromMainAxis));
}

int main()
{
	int command;

	//команды:
	//go_to - иди туда то
	//Position yourself - спозиционируй себя
	cout << "1: go_to" << endl;
	cout << "2: Position yourself" << endl;
	cout << "0: Exit" << endl;
	while(true)
	{
		cout << "Enter the command number" << endl;
		cin >> command;
		if (command == 0)break;
		if(command==1)
		{
			GoTo();
		}
		if(command ==2)
		{
			robot.PositionYourself();
		}

	}
    return 0;
}