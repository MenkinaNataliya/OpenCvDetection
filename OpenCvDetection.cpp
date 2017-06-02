// OpenCvDetection.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "Header.h"
#include "QRCode.h"
#include "cv_Point.h"
#include "cv_Templates.h"
#include <regex>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "Robot.h"
using namespace rapidjson;

QRCode FindQrCode(Mat image);

vector<cv_Templates> templates;

void DefaneTemplate() {//заменить на реализацию БД
	templates.push_back(cv_Templates(1,8));
	templates.push_back(cv_Templates(1, 1));
	templates.push_back(cv_Templates(7, 6));
	templates.push_back(cv_Templates(11, 19));
	templates.push_back(cv_Templates(2, 8));
	/* ********************/
	templates.push_back(cv_Templates(15,12));
	templates.push_back(cv_Templates(4, 16));
	templates.push_back(cv_Templates(5, 9));
	templates.push_back(cv_Templates(10, 8));

}

void PrintTags(vector<cv_Templates> templates)
{
	for(auto templ : templates)
	{
		cout << "\t (" << templ.location->x << "; "<< templ.location->y<<")" << endl;
	}
}

int main()
{
	
	Robot robot = Robot(new cv_Point(0, 0));//робот смотрит строго по направлению движения OY
	DefaneTemplate();

	//cout << "Existing tags: " << endl;
	//PrintTags(templates);
	//
	//cout << "Enter the coordinates: " << endl;
	//int x, y;
	//cin >> x;
	//cin >> y;
	//std::string json = "{\"type\":\"go_to\",\"data\":{\"x\":"+std::to_string(x)+",\"y\":"+std::to_string(y)+"}}";

	//Document document;
	//document.Parse(json.c_str());

	//// Example 1: Json modification, writing Json to console
	//Value& s = document["type"];
	//
	//if (std::string("go_to").compare(s.GetString())==0)//пришел сигнал, который отправляет робота куда то
	//{
	//	s = document["data"];
	//	int x = s["x"].GetInt();
	//	int y = s["y"].GetInt();
	//	vector<cv_Point> route = RoutePlanning(*robot.currentLocation, cv_Point(x, y), templates);
	//	/*ОТПРАВЛЯЕМ НА КОЛЕСНУЮ БАЗУ СИГНАЛ С МАРШРУТОМ, ПРЕДВАРИТЕЛЬНО СФОРМИРОВАВ ЕГО*/
	//	SendSignal(GenerateSignal(route, robot.deviationFromMainAxis));

	//}

	Mat frame;
	VideoCapture cap;
	cap.open(0);
	if (!cap.isOpened()) return -1;
	
	namedWindow("Video", CV_WINDOW_AUTOSIZE);
	
	int count = 0,  countFrame = 0, countAnalisFrame = 0, countQr = 0, countCorrectQR = 0, countIncorrectQR = 0;
	while (true) {
		cap >> frame;
		countFrame++;
		if(count == 5)
		{
			countAnalisFrame++;
			QRCode code = FindQrCode(frame);
			
			if (!code.Empty()) {
				countQr++;
				if (code.GetOrientation() != NORTH) {
					SendErrorMessage("Qr-code is bad. The robot could fall");
					count = -1;
				}
				else {
					string message(code.Read());//после считывания сообщения необходимо проверить корректность данных
					//if(!CheckMessageMatch( message ) && !CheckMessageSearch(message))countIncorrectQR++;
					//else {
					//	string messagetwo(code.ReRead());
					//	if (!CheckMessageMatch(message) && !CheckMessageSearch(message))countIncorrectQR++;
					//	else {
					//		count = -1;
					//		countCorrectQR++;
					//	}
					//}
				//	regex simple("x: * y: *");
					regex rx("x: [0-9]{1,3}; y: [0-9]{1,3}");
					regex rx1("X: [0-9]{1,3}\nY: [0-9]{1,3}");
					bool findMessage = regex_match(message.begin(), message.end(), rx);
					bool findMessage1 = regex_match(message.begin(), message.end(), rx1);
					if(!findMessage && !findMessage1)//если сообщение не подходит, то либо неверно считалось и нужно попробовать еще, либо это не наша метка
					{//нужно попробовать перечитать код
						string messagetwo(code.ReRead());
						findMessage = regex_match(messagetwo.begin(), messagetwo.end(), rx);
						findMessage1 = regex_match(messagetwo.begin(), messagetwo.end(), rx1);
						if(!findMessage && !findMessage1)
							count--;
						countIncorrectQR++;

					}
					else
					{//если подходит хотя бы под одну регулярку то можно считать что выражение верное и корректировать свое местоположение и маршрут
						count = -1;
						countCorrectQR++;
					//	SendErrorMessage( "qrcode correct\n" );
					}
				}
			}
			else count = -1;
		}
		count++;
		cv::imshow("Video", frame);
		int c = cvWaitKey(50);
		if ((char)c == 27)			break;
		//if(countFrame == 500)
		//{
		//	//cvWaitKey(200);
		//	countFrame = countAnalisFrame = countQr = countCorrectQR = 	count =	countIncorrectQR = 0;
		//	ofstream fout("D:/Учеба/Сервисный робот/OpenCvDetection/data/cppstudio.txt", ios_base::trunc);
		//
		//	fout.close(); // закрываем файл
		//}
	}

    return 0;
}