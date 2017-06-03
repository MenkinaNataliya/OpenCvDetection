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


//#include "QRCode.h"

using namespace std;

using namespace cv;

extern


//extern
vector<cv_Point> RoutePlanning(cv_Point src, cv_Point dst, vector<cv_Templates> templates);
enum Orientation { NORTH, WEST, EAST, SOUTH };

				
void SendErrorMessage(string msg);
void Contrast(Mat m_imgEdit, int step);


void SendSignal(string signal);
float SendSignals(cv_Point pointOne, cv_Point pointTwo, int deviation);
float GetAngleFromMainAxis(cv_Point pointOne, cv_Point pointTwo, int deviation);
string GenerateSignals(cv_Point pointOne, cv_Point pointTwo, int deviation);