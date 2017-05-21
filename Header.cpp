#include "stdafx.h"
#include "Header.h"
#include "EquationOfLine.h"


void SendErrorMessage(char *msg)
{
	//отправка сообщени€ оператору о том что робот упал
	cout << msg << endl;
}
// Routines used in Main loops

// Function: Routine to get Distance between two points
// Description: Given 2 points, the function returns the distance

float cv_distance(Point2f P, Point2f Q)
{
	return sqrt(pow(abs(P.x - Q.x), 2) + pow(abs(P.y - Q.y), 2));
}


// Function: Perpendicular Distance of a Point J from line formed by Points L and M; Equation of the line ax+by+c=0
// Description: Given 3 points, the function derives the line quation of the first two points,
//	  calculates and returns the perpendicular distance of the the 3rd point from this line.

float cv_lineEquation(Point2f L, Point2f M, Point2f J)
{
	float a, b, c, pdist;

	a = -((M.y - L.y) / (M.x - L.x));
	b = 1.0;
	c = (((M.y - L.y) / (M.x - L.x)) * L.x) - L.y;

	// Now that we have a, b, c from the equation ax + by + c, time to substitute (x,y) by values from the Point J

	pdist = (a * J.x + (b * J.y) + c) / sqrt((a * a) + (b * b));
	return pdist;
}

// Function: Slope of a line by two Points L and M on it; Slope of line, S = (x1 -x2) / (y1- y2)
// Description: Function returns the slope of the line formed by given 2 points, the alignement flag
//	  indicates the line is vertical and the slope is infinity.

float cv_lineSlope(Point2f L, Point2f M, int& alignement)
{
	float dx, dy;
	dx = M.x - L.x;
	dy = M.y - L.y;

	if (dy != 0)
	{
		alignement = 1;
		return (dy / dx);
	}
	else				// Make sure we are not dividing by zero; so use 'alignement' flag
	{
		alignement = 0;
		return 0.0;
	}
}



// Function: Routine to calculate 4 Corners of the Marker in Image Space using Region partitioning
// Theory: OpenCV Contours stores all points that describe it and these points lie the perimeter of the polygon.
//	The below function chooses the farthest points of the polygon since they form the vertices of that polygon,
//	exactly the points we are looking for. To choose the farthest point, the polygon is divided/partitioned into
//	4 regions equal regions using bounding box. Distance algorithm is applied between the centre of bounding box
//	every contour point in that region, the farthest point is deemed as the vertex of that region. Calculating
//	for all 4 regions we obtain the 4 corners of the polygon ( - quadrilateral).
void cv_getVertices(vector<vector<Point> > contours, int c_id, float slope, vector<Point2f>& quad)
{
	Rect box;
	box = boundingRect(contours[c_id]);

	Point2f M0, M1, M2, M3;
	Point2f A, B, C, D, W, X, Y, Z;

	A = box.tl();
	B.x = box.br().x;
	B.y = box.tl().y;
	C = box.br();
	D.x = box.tl().x;
	D.y = box.br().y;


	W.x = (A.x + B.x) / 2;
	W.y = A.y;

	X.x = B.x;
	X.y = (B.y + C.y) / 2;

	Y.x = (C.x + D.x) / 2;
	Y.y = C.y;

	Z.x = D.x;
	Z.y = (D.y + A.y) / 2;

	float dmax[4];
	dmax[0] = 0.0;
	dmax[1] = 0.0;
	dmax[2] = 0.0;
	dmax[3] = 0.0;

	float pd1 = 0.0;
	float pd2 = 0.0;

	if (slope > 5 || slope < -5)
	{

		for (int i = 0; i < contours[c_id].size(); i++)
		{
			pd1 = cv_lineEquation(C, A, contours[c_id][i]);	// Position of point w.r.t the diagonal AC 
			pd2 = cv_lineEquation(B, D, contours[c_id][i]);	// Position of point w.r.t the diagonal BD

			if ((pd1 >= 0.0) && (pd2 > 0.0))
			{
				cv_updateCorner(contours[c_id][i], W, dmax[1], M1);
			}
			else if ((pd1 > 0.0) && (pd2 <= 0.0))
			{
				cv_updateCorner(contours[c_id][i], X, dmax[2], M2);
			}
			else if ((pd1 <= 0.0) && (pd2 < 0.0))
			{
				cv_updateCorner(contours[c_id][i], Y, dmax[3], M3);
			}
			else if ((pd1 < 0.0) && (pd2 >= 0.0))
			{
				cv_updateCorner(contours[c_id][i], Z, dmax[0], M0);
			}
			else
				continue;
		}
	}
	else
	{
		int halfx = (A.x + B.x) / 2;
		int halfy = (A.y + D.y) / 2;

		for (int i = 0; i < contours[c_id].size(); i++)
		{
			if ((contours[c_id][i].x < halfx) && (contours[c_id][i].y <= halfy))
			{
				cv_updateCorner(contours[c_id][i], C, dmax[2], M0);
			}
			else if ((contours[c_id][i].x >= halfx) && (contours[c_id][i].y < halfy))
			{
				cv_updateCorner(contours[c_id][i], D, dmax[3], M1);
			}
			else if ((contours[c_id][i].x > halfx) && (contours[c_id][i].y >= halfy))
			{
				cv_updateCorner(contours[c_id][i], A, dmax[0], M2);
			}
			else if ((contours[c_id][i].x <= halfx) && (contours[c_id][i].y > halfy))
			{
				cv_updateCorner(contours[c_id][i], B, dmax[1], M3);
			}
		}
	}

	quad.push_back(M0);
	quad.push_back(M1);
	quad.push_back(M2);
	quad.push_back(M3);

}

// Function: Compare a point if it more far than previously recorded farthest distance
// Description: Farthest Point detection using reference point and baseline distance
void cv_updateCorner(Point2f P, Point2f ref, float& baseline, Point2f& corner)
{
	float temp_dist;
	temp_dist = cv_distance(P, ref);

	if (temp_dist > baseline)
	{
		baseline = temp_dist;			// The farthest distance is the new baseline
		corner = P;						// P is now the farthest point
	}

}



// Function: Sequence the Corners wrt to the orientation of the QR Code
void cv_updateCornerOr(int orientation, vector<Point2f> IN, vector<Point2f> &OUT)
{
	Point2f M0, M1, M2, M3;
	if (orientation == NORTH)
	{
		M0 = IN[0];
		M1 = IN[1];
		M2 = IN[2];
		M3 = IN[3];
	}
	else if (orientation == EAST)
	{
		M0 = IN[1];
		M1 = IN[2];
		M2 = IN[3];
		M3 = IN[0];
	}
	else if (orientation == SOUTH)
	{
		M0 = IN[2];
		M1 = IN[3];
		M2 = IN[0];
		M3 = IN[1];
	}
	else if (orientation == WEST)
	{
		M0 = IN[3];
		M1 = IN[0];
		M2 = IN[1];
		M3 = IN[2];
	}

	OUT.push_back(M0);
	OUT.push_back(M1);
	OUT.push_back(M2);
	OUT.push_back(M3);
}

// Function: Get the Intersection Point of the lines formed by sets of two points
bool getIntersectionPoint(Point2f a1, Point2f a2, Point2f b1, Point2f b2, Point2f& intersection)
{
	Point2f p = a1;
	Point2f q = b1;
	Point2f r(a2 - a1);
	Point2f s(b2 - b1);

	if (cross(r, s) == 0) { return false; }

	float t = cross(q - p, s) / cross(r, s);

	intersection = p + t*r;
	return true;
}

float cross(Point2f v1, Point2f v2)
{
	return v1.x*v2.y - v1.y*v2.x;
}

int cv_CalculationOfDimensionsSquare(vector<Point2f> top, vector<Point2f> right, vector<Point2f> bottom)
{
	//считаем высоту в 3 вариантах
	float t0 = abs((top[1].x - top[0].x) / 7);
	float t1 = abs((top[2].x - top[3].x) / 7);

	float r0 = abs((right[1].x - right[0].x) / 7);
	float r1 = abs((right[2].x - right[3].x) / 7);

	float b0 = abs((bottom[1].x - bottom[0].x) / 7);
	float b1 = abs((bottom[2].x - bottom[3].x) / 7);


	//считаем ширину в 3 вариантах
	float t2 = abs((top[3].y - top[0].y) / 7);
	float t3 = abs((top[2].y - top[1].y) / 7);

	float r2 = abs((right[3].y - right[0].y) / 7);
	float r3 = abs((right[2].y - right[1].y) / 7);

	float b2 = abs((bottom[3].y - bottom[0].y) / 7);
	float b3 = abs((bottom[2].y - bottom[1].y) / 7);

	return ceil((t0 + t1 + t2 + t3 + r0 + r1 + r2 + r3 + b0 + b1 + b2 + b3) / 12);
}

void cv_getContours(Mat image, vector<vector<Point> >& contours, vector<Vec4i> &hierarchy)
{

	Mat gray(image.size(), CV_MAKETYPE(image.depth(), 1));			// To hold Grayscale Image
	Mat edges(image.size(), CV_MAKETYPE(image.depth(), 1));			// To hold Grayscale Image


	cvtColor(image, gray, CV_RGB2GRAY);		// Convert Image captured from Image Input to GrayScale	

	Canny(gray, edges, 100, 200, 3);		// Apply Canny edge detection on the gray image


	findContours(edges, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE); // Find contours with hierarchy
}

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

vector<cv_Point> RoutePlanning(cv_Point src, cv_Point dst, vector<cv_Templates> templates)
{
	vector<cv_Point> map;
	int direct = DirectionCheck(src,dst);/*определ€ем направление движени€*/

	float minDistanse=1000, dopDistanse=1000;
	bool isPop = false;
	int index = 0; int dir=0;//все равно перезапишетс€
	/*начинаем строить маршрут*/
	while (true){
		for (int i = 0; i < templates.size(); i++){
			float dist;

			if (map.empty()) {
				dist = src.DistanceCalculation(*templates[i].location);
				dir = DirectionCheck(src, *templates[i].location);
			}
			else {
				dist = map[map.size() - 1].DistanceCalculation(*templates[i].location);
				dir = DirectionCheck(map[map.size() - 1], *templates[i].location);
			}

			if (VerificationOfAdjacentDirections(*templates[i].location, dst, direct, dir) && dist != 0){
				if (dist < minDistanse) {
					if (dist < dopDistanse) {
						isPop = false;

						if (!map.empty() && map.size() != 1) {
							float twoPointDistance = map[map.size() - 2].DistanceCalculation(*templates[i].location);
							float threePointDistance = map[map.size() - 1].DistanceCalculation(map[map.size() - 2]) + dist;
							if (threePointDistance - twoPointDistance <= 1 
								&& abs(map[map.size() - 1].x - (*templates[i].location).x) <=2 
								&& abs(map[map.size() - 1].y - (*templates[i].location).y) <= 2){

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
		map.push_back(*templates[index].location);
		isPop = false; minDistanse = 1000000;
		dopDistanse = 1000;
		if (DirectionCheck(map[map.size() - 1], dst) == CURRENT) break;
	}

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
	return map;
}


void SendSignal(string signal)
{
	cout << signal << endl;
}
//Ќапример, пр€мо 5 метров: f5(f - forward)
//Ќазад, 2 метра : b2(b - backward)
//Ќалево, 30 градусов : l30(l - left)
//Ќаправо, 40 градусов : r40(r - right)
//ќстановитьс€ : s(s - stop)
string GenerateSignal(vector<cv_Point> route, float deviation)
{
	string signal;
	//вычислили угол между ќ” и первой дистанцией
	EquationOfLine first = EquationOfLine(cv_Point(0, 0), cv_Point(0, 1));
	EquationOfLine second = EquationOfLine(route[0], route[1]);
	int angle = first.CalculationOfAngleBetweenLine(second);
	
	if(angle < deviation)signal += "l" + to_string(abs(angle - deviation));
	else if(angle > deviation)signal += "r" + to_string(abs(angle - deviation));
	int dopangle = angle;
	signal+=" f"+ to_string(second.GetLength());
	//если углы равны то поворачиватьс€ не надо

	for(int i=2;i< route.size(); i++)
	{
		first = EquationOfLine(EquationOfLine(route[i-2], route[i-1]));
		second = EquationOfLine(route[i-1], route[i]);
		angle = first.CalculationOfAngleBetweenLine(second);

		if (angle < dopangle)signal += " l" + to_string(angle);
		else if (angle > dopangle)signal += " r" + to_string(angle);
		signal += " f" + to_string(second.GetLength());
		dopangle = angle;
	}

	angle = second.CalculationOfAngleBetweenLine(EquationOfLine(cv_Point(0, 0), cv_Point(0, 1)));
	if (angle < dopangle)signal += " l" + to_string(abs(angle));
	else if (angle > dopangle)signal += " r" + to_string(abs(angle));
	signal += " s";


	return signal;
}


