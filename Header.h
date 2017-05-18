#pragma once

#include "stdafx.h"
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <stdio.h>
#include <ctype.h>

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include <math.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <dos.h>


#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <ctype.h>
#include "cv_Point.h"

#include <vector>
#include "cv_Templates.h"
#include "cv_AdditionalPoint.h"
//#include "QRCode.h"

using namespace std;

using namespace cv;

extern

void DefaneTemplate();

//QRCode FindQrCode(Mat image);

enum Orientation { NORTH, WEST, EAST, SOUTH };
extern
vector<cv_Point> RoutePlanning(cv_Point src, cv_Point dst, vector<cv_Templates> templates);


float cv_distance(Point2f P, Point2f Q);					// Get Distance between two points
float cv_lineEquation(Point2f L, Point2f M, Point2f J);		// Perpendicular Distance of a Point J from line formed by Points L and M; Solution to equation of the line Val = ax+by+c 
float cv_lineSlope(Point2f L, Point2f M, int& alignement);	// Slope of a line by two Points L and M on it; Slope of line, S = (x1 -x2) / (y1- y2)
void cv_getVertices(vector<vector<Point> > contours, int c_id, float slope, vector<Point2f>& X);
void cv_updateCorner(Point2f P, Point2f ref, float& baseline, Point2f& corner);
void cv_updateCornerOr(int orientation, vector<Point2f> IN, vector<Point2f> &OUT);
bool getIntersectionPoint(Point2f a1, Point2f a2, Point2f b1, Point2f b2, Point2f& intersection);
float cross(Point2f v1, Point2f v2);
int cv_CalculationOfDimensionsSquare(vector<Point2f> top, vector<Point2f> right, vector<Point2f> bottom);
void cv_getContours(Mat image, vector<vector<Point> >& contours, vector<Vec4i>& hierarchy);

void cv_ReadQrCode(Mat image, vector<Point2f> L, vector<Point2f> M, vector<Point2f> O);

void SendErrorMessage(char *msg);