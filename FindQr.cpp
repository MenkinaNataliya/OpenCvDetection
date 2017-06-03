#include "stdafx.h"
#include "FindQr.h"


bool FindCode(cv_Point mark)
{
	Mat frame;
	VideoCapture cap;
	cap.open(0);
	if (!cap.isOpened()) return false;

	namedWindow("Video", CV_WINDOW_AUTOSIZE);

	int count = 0;
	while (true) {
		cap >> frame;
		if (count == 5)
		{
			QRCode code = FindQrCode(frame);

			if (!code.Empty()) {
				if (code.GetOrientation() != NORTH) {
					SendErrorMessage("Qr-code is bad. The robot could fall");
					count = -1;
				}
				else {
					string message(code.Read());//после считывания сообщения необходимо проверить корректность данных
					regex rx;
					rx = ("x: " + to_string(mark.x) + "; y: " + to_string(mark.y));/*совпадение 100%*/

					if (regex_match(message.begin(), message.end(), rx))
					{
						return true;
					}
					rx = "x* " + to_string(mark.x) + "; y* " + to_string(mark.y);/*совпадение 90%*/
					if (regex_match(message.begin(), message.end(), rx))
					{
						return true;
					}
					count--;
				}
			}
			else count--;
		}
		count++;
		cv::imshow("Video", frame);
		int c = cvWaitKey(50);
		if ((char)c == 27)			break;
	}
	return false;
}


cv_Point FindAnyCode()
{
	Mat frame;
	VideoCapture cap;
	cap.open(0);
	if (!cap.isOpened()) return cv_Point(-1, -1);
	int count = 0;
	while (count<100) {
		cap >> frame;
		if (count == 5)
		{
			QRCode code = FindQrCode(frame);
			if (!code.Empty()) {
				if (code.GetOrientation() != NORTH) {
					count = -1;
				}
				else {
					string message(code.Read());//после считывания сообщения необходимо проверить корректность данных
					regex rx("x: [0-9]{1,3}; y: [0-9]{1,3}");/*совпадение 100%*/
					regex rx1("x? [0-9]{1,3}; y*? [0-9]{1,3}");/*совпадение 90%*/
					if (regex_match(message.begin(), message.end(), rx) || regex_match(message.begin(), message.end(), rx1)) {
						std::string strx = message.substr(3, message.find(";") - 3);
						std::string stry = message.substr(message.find(";") + 4);
						return cv_Point(stoi(strx), stoi(stry));
					}
					count--;
				}
			}
			else count--;
		}
		count++;
	}
	return cv_Point(-1, -1);
}
