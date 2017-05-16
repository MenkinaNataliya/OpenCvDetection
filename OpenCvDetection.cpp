// OpenCvDetection.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "Header.h"
#include "QRCode.h"
#include "cv_Point.h"
#include "cv_Templates.h"



cv_Point* currentLocation;
vector<cv_Templates> templates;

void DefaneTemplate() {//заменить на реализацию БД
	cv_Templates templ;
	templ.filename = "../data/test.bmp";
	templ.comment = "Rectangle";
	templ.red = 0;
	templ.green = 162;
	templ.blue = 232;
	templ.location = new cv_Point(3, 3);
	templates.push_back(templ);


	cv_Templates templ1;
	templ1.filename = "../data/templ3.bmp";
	templ1.comment = "Circle";
	templ1.red = 34;
	templ1.green = 177;
	templ1.blue = 76;
	templ1.location = new cv_Point(4, 7);
	templates.push_back(templ1);

	cv_Templates templ2;
	templ2.filename = "../data/templ4.bmp";
	templ2.comment = "5angle";
	templ2.red = 63;
	templ2.green = 72;
	templ2.blue = 204;
	templ2.location = new cv_Point(7, 4);
	templates.push_back(templ2);

	cv_Templates templ3;
	templ3.filename = "../data/templ2.bmp";
	templ3.comment = "RectangleGreen";
	templ3.red = 34;
	templ3.green = 177;
	templ3.blue = 76;
	templ3.location = new cv_Point(9, 8);
	templates.push_back(templ3);
	//currentLocation = new PointT(5, 3);
}

int main()
{
	Mat frame;
	VideoCapture cap;
	int camOpen = cap.open(0);
	if (!cap.isOpened()) {

		return -1;
	}
	namedWindow("Video", CV_WINDOW_AUTOSIZE);
	/*currentLocation = new cv_Point(0, 0);
	cv_Point* destination = new cv_Point(9,8);*/
	//DefaneTemplate();
		//построение маршрута
	//vector<cv_Point> route = RoutePlanning(*currentLocation, *destination, templates);
	//	MoveToGivenPoint(*currentLocation, route);
	
	//IplImage* templMain = cvLoadImage(route[0].filename);
	//OtherFunction(currentLocation, *label.location, i);
	while (true) {
		cap >> frame;
		try
		{
			if (FindQrCode(frame)) {
				/*QRCode qr = QRCode(frame);
				if (qr.GetOrientation() != NORTH) cout << "Qr-code is bad" << endl;
				else qr.Read();*/
			}
		}
		catch(Exception)
		{
			
		}

		

		imshow("Video", frame);
		int c = cvWaitKey(50);
		if ((char)c == 27)			break;
	}

    return 0;
}