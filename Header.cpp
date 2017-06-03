#include "stdafx.h"
#include "Header.h"
#include "EquationOfLine.h"


void SendErrorMessage(string msg)
{
	ofstream fout("D:/Учеба/Сервисный робот/OpenCvDetection/data/cppstudio.txt", ios_base::app);
	//отправка сообщения оператору о том что робот упал
	
	fout << msg << endl; // запись строки в файл
	fout.close(); // закрываем файл
}
// Routines used in Main loops

// Function: Routine to get Distance between two points
// Description: Given 2 points, the function returns the distance



enum Direction { RIGHT_BOTTOM, LEFT_BOTTOM, RIGHT_TOP ,LEFT_TOP, RIGHT, LEFT, TOP, BOTTOM, CURRENT};

Direction DirectionCheck(cv_Point src, cv_Point dst)
{
	if (src.x == dst.x)
		if (src.y > dst.y)return  BOTTOM;
		else
			if (src.y < dst.y) 	return TOP;
			else return CURRENT;
	else
		if (src.x > dst.x)
			if (src.y > dst.y) return LEFT_BOTTOM;
			else
				if (src.y < dst.y) 	return LEFT_TOP;
				else return LEFT;
		else
			if (src.y > dst.y) return RIGHT_BOTTOM;
			else
				if (src.y < dst.y) 	return RIGHT_TOP;
				else return RIGHT;
}
bool VerificationOfAdjacentDirections(cv_Point src, cv_Point dst,Direction dirMain, Direction dirLocal)
{
	return VerificationOfAdjacentDirections(src,dst,dirMain, dirLocal);
}

bool VerificationOfAdjacentDirections(cv_Point src, cv_Point dst,int dirMain, int dirLocal)
{
	
	if (dirMain == LEFT_TOP)
		if (dirLocal == LEFT || dirLocal == LEFT_TOP || dirLocal == TOP)
		{
			if (src.x >= dst.x && src.y <= dst.y) return true;
			return false;
		}
	if (dirMain == LEFT_BOTTOM)
		if (dirLocal == LEFT || dirLocal == LEFT_BOTTOM || dirLocal == BOTTOM) {
			if (src.x >= dst.x && src.y >= dst.y) return true;
			return false;
		}
	if (dirMain == LEFT)
		if (dirLocal == LEFT_TOP || dirLocal == LEFT_BOTTOM || dirLocal == TOP || dirLocal == BOTTOM || dirLocal == LEFT) {
			if (src.x >= dst.x) return true;
			return false;
		}

	if (dirMain == RIGHT_TOP)
		if (dirLocal == RIGHT || dirLocal == RIGHT_TOP || dirLocal == TOP) {
			if (src.x <= dst.x && src.y <= dst.y) return true;
			return false;
		}
	if (dirMain == RIGHT_BOTTOM)
		if (dirLocal == RIGHT || dirLocal == RIGHT_BOTTOM || dirLocal == BOTTOM) {
			if (src.x <= dst.x && src.y >= dst.y) return true;
			return false;
		}
	if (dirMain == RIGHT)
		if (dirLocal == RIGHT_TOP || dirLocal == RIGHT_BOTTOM || dirLocal == TOP || dirLocal == BOTTOM || dirLocal == RIGHT) {
			if (src.x <= dst.x ) return true;
			return false;
		}

	if (dirMain == TOP)
		if (dirLocal == RIGHT_TOP || dirLocal == LEFT_TOP || dirLocal == TOP || dirLocal == RIGHT || dirLocal == LEFT) {
			if (src.y <= dst.y) return true;
			return false;
		}
	if (dirMain == BOTTOM)
		if (dirLocal == RIGHT_BOTTOM || dirLocal == LEFT_BOTTOM || dirLocal == BOTTOM || dirLocal == RIGHT || dirLocal == LEFT) {
			if ( src.y>= dst.y) return true;
			return false;
		}

	return false;
}

cv_Point DefinitionOfNearestLabel(cv_Point dst, vector<cv_Templates> templates)
{
	int minX = abs(templates[0].location.x- dst.x), minY = abs(templates[0].location.y - dst.y);
	cv_Point label;
	vector<cv_Templates> labelX;
	for(auto templ : templates)
	{
		if (abs(dst.x - templ.location.x) <=  minX)
		{
			if(abs(dst.y - templ.location.y) <= minY)
			{
				minX = abs(templ.location.x - dst.x);
				minY = abs(templ.location.y - dst.y);
				label = templ.location;
			}
		}
	}
	return label;
	
}
float GetAngleFromMainAxis(cv_Point pointOne, cv_Point pointTwo, int deviation)
{
	EquationOfLine first = EquationOfLine(pointOne, pointTwo);
	EquationOfLine second = EquationOfLine(cv_Point(0, 0), cv_Point(0, 1));
	return first.CalculationOfAngleBetweenLine(second);
}


string GenerateSignals(cv_Point pointOne, cv_Point pointTwo, int deviation)
{
	string signal;
	float angle = GetAngleFromMainAxis(pointOne, pointTwo, deviation);

	if (angle != 0) {
		if (angle < deviation)signal += " r" + to_string(abs(deviation - angle));
		else if (angle > deviation)signal += " l" + to_string(abs(deviation - angle));
		signal += " f" + to_string(EquationOfLine(pointOne, pointTwo).GetLength()) + " s";
	}

	return signal;
}

float SendSignals(cv_Point pointOne, cv_Point pointTwo, int deviation)
{
	string signal = GenerateSignals(pointOne, pointTwo, deviation);
	SendSignal(signal);
	return GetAngleFromMainAxis(pointOne, pointTwo, deviation);
}

void SendSignal(string signal)
{
	//cout << signal << endl;

	ofstream fout("D:/Учеба/Сервисный робот/OpenCvDetection/data/route.txt", ios_base::app);
	//отправка сообщения оператору о том что робот упал

	fout << signal << endl; // запись строки в файл
	fout.close(); // закрываем файл
}

vector<cv_Point> RoutePlanning(cv_Point src, cv_Point dst, vector<cv_Templates> templates)
{
	vector<cv_Point> map;
	
	/*определяем ближайшую метку*/
	cv_Point nearestLabel = DefinitionOfNearestLabel(dst, templates);

	int direct = DirectionCheck(src, nearestLabel);/*определяем направление движения*/

	float minDistanse=1000, dopDistanse=1000;
	bool isPop = false;
	int index = 0; int dir=0;//все равно   
	/*начинаем строить маршрут*/
	while (true){
		for (int i = 0; i < templates.size(); i++){
			float dist;

			if (map.empty()) {
				dist = src.DistanceCalculation(templates[i].location);
				dir = DirectionCheck(src, templates[i].location);
			}
			else {
				dist = (map[map.size() - 1]).DistanceCalculation(templates[i].location);
				dir = DirectionCheck(map[map.size() - 1], templates[i].location);
			}

			if (VerificationOfAdjacentDirections(templates[i].location, nearestLabel, direct, dir) && dist != 0){
				if (dist < minDistanse) {
					if (dist < dopDistanse) {
						isPop = false;

						if (!map.empty() && map.size() != 1) {
							float twoPointDistance = map[map.size() - 2].DistanceCalculation(templates[i].location);
							float threePointDistance = map[map.size() - 1].DistanceCalculation(map[map.size() - 2]) + dist;
							if (threePointDistance - twoPointDistance <= 1 
								&& abs(map[map.size() - 1].x - (templates[i].location).x) <=2
								&& abs(map[map.size() - 1].y - (templates[i].location).y) <= 2){

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
		map.push_back(templates[index].location);
		isPop = false; minDistanse = 1000000;
		dopDistanse = 1000;
		if (DirectionCheck(map[map.size() - 1], nearestLabel) == CURRENT) break;
	}
//	if(nearestLabel.x!=dst.x && nearestLabel.y != dst.y)//если у нас место назначение не метка


	vector<cv_Point> maptmp;
	while(!map.empty())
	{
		maptmp.push_back(map[map.size()-1]);
		map.pop_back();
	}

	map.push_back(src);
	while (!maptmp.empty())
	{
		map.push_back(maptmp[maptmp.size() - 1]);
		maptmp.pop_back();
	}
	if (nearestLabel.x != dst.x && nearestLabel.y != dst.y)//если у нас место назначение не метка
		map.push_back(dst);
	return map;
}

//Например, прямо 5 метров: f5(f - forward)
//Назад, 2 метра : b2(b - backward)
//Налево, 30 градусов : l30(l - left)
//Направо, 40 градусов : r40(r - right)
//Остановиться : s(s - stop)
string GenerateSignal(vector<cv_Point> route, int deviation)
{
	string signal;
	//вычислили угол между ОУ и первой дистанцией
	EquationOfLine first = EquationOfLine(cv_Point(0, 0), cv_Point(0, 1));
	int angle;
	int dopangle = deviation;
	EquationOfLine second;//= EquationOfLine(route[0], route[1]);
	/*int angle = first.CalculationOfAngleBetweenLine(second);
	
	if(angle < deviation) signal += "l" + to_string(abs(angle - deviation));
	else if(angle > deviation) signal += "r" + to_string(abs(angle - deviation));
	int dopangle = angle;
	signal+=" f"+ to_string(second.GetLength());*/
	//если углы равны то поворачиваться не надо

	for(int i=1;i< route.size(); i++)
	{
		//first = EquationOfLine(EquationOfLine(route[i-2], route[i-1]));
		second = EquationOfLine(route[i-1], route[i]);
		angle = first.CalculationOfAngleBetweenLine(second);

		if (angle < dopangle)signal += " l" + to_string(angle);
		else if (angle > dopangle)signal += " r" + to_string(angle);
		signal += " f" + to_string(second.GetLength());
		dopangle = angle;
		first = EquationOfLine(EquationOfLine(route[i-1], route[i]));
	}

	angle = second.CalculationOfAngleBetweenLine(EquationOfLine(cv_Point(0, 0), cv_Point(0, 1)));
	if (angle < dopangle)signal += " l" + to_string(abs(angle));
	else if (angle > dopangle)signal += " r" + to_string(abs(angle));
	signal += " s";


	return signal;
}


