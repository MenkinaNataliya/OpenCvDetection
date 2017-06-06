#include "stdafx.h"
#include "Robot.h"
#include "EquationOfLine.h"

#include "FindQr.h"
#include "Header.h"



Robot::Robot(cv_Point location)
{// в самом начале пути робот стоит прямо по отношению к основной системе координат
	currentLocation = location;
	deviationFromMainAxis = 0;
	route = vector<cv_Point>();
	DefineTemplate();
	dstIsLabel = false;
}

Robot::Robot(cv_Point location, float deviation)
{
	currentLocation = location;
	deviationFromMainAxis = deviation;
	route = vector<cv_Point>();
	DefineTemplate();
	dstIsLabel = false;
}


Robot::~Robot()
{
}

void Robot::DefineTemplate()
{
	labels.push_back(cv_Label(1, 7));
	labels.push_back(cv_Label(3, 2));
	labels.push_back(cv_Label(3, 16));
	labels.push_back(cv_Label(4, 23));
	labels.push_back(cv_Label(6, 10));
	/* ********************/
	labels.push_back(cv_Label(6, 19));
	labels.push_back(cv_Label(9, 5));
	labels.push_back(cv_Label(9, 22));
	labels.push_back(cv_Label(10, 14));
	labels.push_back(cv_Label(11, 18));
	labels.push_back(cv_Label(14, 9));
	labels.push_back(cv_Label(16, 3));
	labels.push_back(cv_Label(16, 16));
	labels.push_back(cv_Label(18, 22));
	labels.push_back(cv_Label(19, 7));
	labels.push_back(cv_Label(20, 1));
	labels.push_back(cv_Label(21, 16));
}


void Robot::PrintTags()
{
	for (auto templ : labels)
	{
		cout << "\t (" << templ.location.x << "; " << templ.location.y << ")" << endl;
	}
}


void Robot::Routeing(cv_Point dst)
{
	vector<cv_Point> map;

	/*определяем ближайшую метку*/
	cv_Point nearestLabel = DefinitionOfNearestLabel(dst, labels);

	int direct = DirectionCheck(currentLocation, nearestLabel);/*определяем направление движения*/

	float minDistanse = 1000, dopDistanse = 1000;
	bool isPop = false;
	int index = 0; int dir = 0;//все равно   
							   /*начинаем строить маршрут*/
	while (true) {
		for (int i = 0; i < labels.size(); i++) {
			float dist;

			if (map.empty()) {
				dist = currentLocation.DistanceCalculation(labels[i].location);
				dir = DirectionCheck(currentLocation, labels[i].location);
			}
			else {
				dist = (map[map.size() - 1]).DistanceCalculation(labels[i].location);
				dir = DirectionCheck(map[map.size() - 1], labels[i].location);
			}

			if (VerificationOfAdjacentDirections(labels[i].location, nearestLabel, direct, dir) && dist != 0) {
				if (dist < minDistanse) {
					if (dist < dopDistanse) {
						isPop = false;

						if (!map.empty() && map.size() != 1) {
							float twoPointDistance = map[map.size() - 2].DistanceCalculation(labels[i].location);
							float threePointDistance = map[map.size() - 1].DistanceCalculation(map[map.size() - 2]) + dist;
							if (threePointDistance - twoPointDistance <= 1
								&& abs(map[map.size() - 1].x - (labels[i].location).x) <= 2
								&& abs(map[map.size() - 1].y - (labels[i].location).y) <= 2) {

								isPop = true;
								minDistanse = twoPointDistance;
								dopDistanse = dist;
							}
							else minDistanse = dist;
						}
						else	minDistanse = dist;
						index = i;
					}
				}
			}
		}
		if (isPop) map.pop_back();
		map.push_back(labels[index].location);
		isPop = false; minDistanse = 1000000;
		dopDistanse = 1000;
		if (DirectionCheck(map[map.size() - 1], nearestLabel) == CURRENT) break;
	}

	vector<cv_Point> maptmp;
	while (!map.empty())
	{
		maptmp.push_back(map[map.size() - 1]);
		map.pop_back();
	}

	map.push_back(currentLocation);
	while (!maptmp.empty())
	{
		map.push_back(maptmp[maptmp.size() - 1]);
		maptmp.pop_back();
	}
	if (nearestLabel.x != dst.x || nearestLabel.y != dst.y) {
		//если у нас место назначение не метка
		dstIsLabel = true;
		map.push_back(dst);

	}
	route = map;
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
		if (deviationFromMainAxis == start) {
			SendErrorMessage("The robot was lost");
			return false;
		}
	}
	currentLocation = point;
	return true;
}

void Robot::FollowRoute()
{
	int dstlabel = 0;
	if (dstIsLabel == true)dstlabel = 1;
	for (int i = 1; i < route.size()- dstlabel; i++)
	{
		deviationFromMainAxis = SendSignals(route[i - 1], route[i], deviationFromMainAxis);
		cout << route[i].x << " " << route[i].y << endl;
		if(FindCode(route[i])) 
			currentLocation = route[i];
		else
			if (PositionYourself())
			{
				Routeing(route[route.size() - 1]);
				FollowRoute();
				return;
			}
			else return;
	}
	if (dstIsLabel == true)
	{
		deviationFromMainAxis = SendSignals(route[route.size()-2], route[route.size() - 1], deviationFromMainAxis);
		currentLocation = route[route.size() - 1];
	}
}
