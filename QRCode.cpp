#include "stdafx.h"
#include "QRCode.h"
#include "CountMask.h"

QRCode::QRCode(Mat _image)
{
	image = _image;
	CalculateOrientation();
	DrawTarget();
	//Resize();
}

QRCode::~QRCode() {}

void QRCode::Resize()
{
	if(this->orientation == NORTH)
	{

		//выбираем самый левый верхний пиксель
		int lefttopx = min({ L[0].x, M[0].x, O[0].x, L[1].x, M[1].x, O[1].x, L[2].x, M[2].x, O[2].x , L[3].x, M[3].x, O[3].x });
		int lefttopY = min({ L[0].y, M[0].y, O[0].y, L[1].y, M[1].y, O[1].y, L[2].y, M[2].y, O[2].y , L[3].y, M[3].y, O[3].y });
		//выбираем самый левый нижний пиксель
		int leftbotx = max({ L[0].x, M[0].x, O[0].x, L[1].x, M[1].x, O[1].x, L[2].x, M[2].x, O[2].x , L[3].x, M[3].x, O[3].x });
		int leftbotY = max({ L[0].y, M[0].y, O[0].y, L[1].y, M[1].y, O[1].y, L[2].y, M[2].y, O[2].y , L[3].y, M[3].y, O[3].y });
		
		if (leftbotx + 30 >= 640)leftbotx = 639 - 30;
		if (leftbotY + 30 >= 640)leftbotY = 639 - 30;
		
		Rect r(lefttopx - 10, lefttopY - 10,640 - lefttopx, 480 - lefttopY); // Создание региона
		Mat img2;
		// Копирование региона в отдельное изображение
		image(r).copyTo(img2);
		imwrite("D:/Учеба/Сервисный робот/OpenCvDetection/data/image01_res6.jpg", img2);
	}
	
}

void QRCode::CalculateOrientation()
{
	cv_getContours(image, contours, hierarchy);//получили контур,

	float AB, BC, CA, dist;
	int mark = 0, A, B, C, median1, median2, outlier, align, orientation;	// Reset all detected marker count for this frame
																			// Get Moments for all Contours and the mass centers
	vector<Moments> mu(contours.size());
	vector<Point2f> mc(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		mu[i] = moments(contours[i], false);
		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
	}

	// Start processing the contour data

	// Find Three repeatedly enclosed contours A,B,C
	// NOTE: 1. Contour enclosing other contours is assumed to be the three Alignment markings of the QR code.
	// 2. Alternately, the Ratio of areas of the "concentric" squares can also be used for identifying base Alignment markers.
	// The below demonstrates the first method

	for (int i = 0; i < contours.size(); i++)//определили где контур А В С
	{
		int k = i;
		int c = 0;

		while (hierarchy[k][2] != -1)
		{
			k = hierarchy[k][2];
			c = c + 1;
		}
		if (hierarchy[k][2] != -1)	c = c + 1;

		if (c >= 5)
		{
			if (mark == 0)		A = i;
			else if (mark == 1)	B = i;		// i.e., A is already found, assign current contour to B
			else if (mark == 2)	C = i;		// i.e., A and B are already found, assign current contour to C
			mark = mark + 1;
		}
	}

	if (mark >= 2)		// Ensure we have (atleast 3; namely A,B,C) 'Alignment Markers' discovered
	{	// We have found the 3 markers for the QR code; Now we need to determine which of them are 'top', 'right' and 'bottom' markers
		// Determining the 'top' marker
		// Vertex of the triangle NOT involved in the longest side is the 'outlier'

		AB = cv_distance(mc[A], mc[B]);
		BC = cv_distance(mc[B], mc[C]);
		CA = cv_distance(mc[C], mc[A]);

		if (AB > BC && AB > CA) { outlier = C; median1 = A; median2 = B; }
		else if (CA > AB && CA > BC) { outlier = B; median1 = A; median2 = C; }
		else if (BC > AB && BC > CA) { outlier = A; median1 = B; median2 = C; }

		top = outlier;							// The obvious choice

		dist = cv_lineEquation(mc[median1], mc[median2], mc[outlier]);	// Get the Perpendicular distance of the outlier from the longest side			
		slope = cv_lineSlope(mc[median1], mc[median2], align);		// Also calculate the slope of the longest side
																	// Now that we have the orientation of the line formed median1 & median2 and we also have the position of the outlier w.r.t. the line
																	// Determine the 'right' and 'bottom' markers
		if (align == 0)
		{
			bottom = median1;
			right = median2;
		}
		else if (slope < 0 && dist < 0)		// Orientation - North
		{//нам нужна только такая ориентация, для того что бы отслеживать упал робот или нет
			bottom = median1;
			right = median2;
			this->orientation = NORTH;
			//putText(traces, "NORTH", Point(20, 30),  FONT_HERSHEY_PLAIN , 1, Scalar(0, 255, 0), 1, 8);
		}
		else if (slope > 0 && dist < 0)		// Orientation - East
		{
			right = median1;
			bottom = median2;
			this->orientation = EAST;
		}
		else if (slope < 0 && dist > 0)		// Orientation - South			
		{
			right = median1;
			bottom = median2;
			this->orientation = SOUTH;
		}
		else if (slope > 0 && dist > 0)		// Orientation - West
		{
			bottom = median1;
			right = median2;
			this->orientation = WEST;
		}
	}
}

void QRCode::DrawTarget()
{
	Mat qr_raw = Mat::zeros(100, 100, CV_8UC3);
	Mat qr = Mat::zeros(100, 100, CV_8UC3);
	Mat qr_gray = Mat::zeros(100, 100, CV_8UC1);
	Mat qr_thres = Mat::zeros(100, 100, CV_8UC1);
	if (top < contours.size() && right < contours.size() && bottom < contours.size() && contourArea(contours[top]) > 10 && contourArea(contours[right]) > 10 && contourArea(contours[bottom]) > 10)
	{
		vector<Point2f>  tempL, tempM, tempO;
		Point2f N;
		vector<Point2f> src, dst;		// src - Source Points basically the 4 end co-ordinates of the overlay image
										// dst - Destination Points to transform overlay image	
		Mat warp_matrix;

		cv_getVertices(contours, top, slope, tempL);
		cv_getVertices(contours, right, slope, tempM);
		cv_getVertices(contours, bottom, slope, tempO);

		cv_updateCornerOr(orientation, tempL, L); 			// Re-arrange marker corners w.r.t orientation of the QR code
		cv_updateCornerOr(orientation, tempM, M); 			// Re-arrange marker corners w.r.t orientation of the QR code
		cv_updateCornerOr(orientation, tempO, O); 			// Re-arrange marker corners w.r.t orientation of the QR code

		int iflag = getIntersectionPoint(M[1], M[2], O[3], O[2], N);

		src.push_back(L[0]);
		src.push_back(M[1]);
		src.push_back(N);
		src.push_back(O[3]);

		dst.push_back(Point2f(0, 0));
		dst.push_back(Point2f(qr.cols, 0));
		dst.push_back(Point2f(qr.cols, qr.rows));
		dst.push_back(Point2f(0, qr.rows));


		//выбираем самый левый верхний пиксель
		int lefttopx = min({ L[0].x, M[0].x, O[0].x, L[1].x, M[1].x, O[1].x, L[2].x, M[2].x, O[2].x , L[3].x, M[3].x, O[3].x });
		int lefttopY = min({ L[0].y, M[0].y, O[0].y, L[1].y, M[1].y, O[1].y, L[2].y, M[2].y, O[2].y , L[3].y, M[3].y, O[3].y });
		//выбираем самый левый нижний пиксель
		int leftbotx = max({ L[0].x, M[0].x, O[0].x, L[1].x, M[1].x, O[1].x, L[2].x, M[2].x, O[2].x , L[3].x, M[3].x, O[3].x });
		int leftbotY = max({ L[0].y, M[0].y, O[0].y, L[1].y, M[1].y, O[1].y, L[2].y, M[2].y, O[2].y , L[3].y, M[3].y, O[3].y });

		if (leftbotx + 30 >= 640)leftbotx = 639 - 30;
		if (leftbotY + 30 >= 640)leftbotY = 639 - 30;

		Rect r(Point(contours[top].front().x -10, contours[top].front().y-10), Point(640, 480)); // Создание региона
		Mat img2;
		// Копирование региона в отдельное изображение
		image(r).copyTo(img2);
		imwrite("D:/Учеба/Сервисный робот/OpenCvDetection/data/image01_res6.jpg", img2);

		//Draw contours on the image
		drawContours(image, contours, top, Scalar(255, 200, 0), 2, 8, hierarchy, 0);
		drawContours(image, contours, right, Scalar(0, 0, 255), 2, 8, hierarchy, 0);
		drawContours(image, contours, bottom, Scalar(255, 0, 100), 2, 8, hierarchy, 0);
		imshow("Target", image);
	}
}

string QRCode::Read()
{
	//получаем ширину одного квадрата
	sizeBlock = cv_CalculationOfDimensionsSquare(L, M, O);
	/****************************************алгоритм расшифровки qr-coda************************************************************/
	//Шаг 1. Чтение 5 бит системной информации
	//Шаг 2. Маска для системной информации
	//Шаг 3. Чтение заголовка данных
	//Шаг 4. Применение маски к заголовку
	//Шаг 5. Чтение данных
	//шаг 6. Применение маски

	//шаг 1 получили системную информацию
	//Как уже говорилось, интерес представляют только первые 5 бит.
	//Из которых 2 бита показывают уровень коррекции ошибок, 
	//а остальные 3 бита показывают какая маска из доступных 8 применяется к данным.
	float y = L[3].y + sizeBlock;
	float x = L[3].x;
	Rect Rec(x, y, sizeBlock * 5, sizeBlock);//прямоугольник с 5 битами
											 //	rectangle(image, Rec, Scalar(255), 1, 8, 0);
	int byte[5];
	GetSystemsBytes(y, x, byte);

	//шаг 2. применяем маску для системной информации 
	//Кроме уже озвученных схем зашиты системной информации, в добавок, используется статическая маска,
	//которая применяется к любой системной информации. Она имеет вид: 101010000010010. 
	//Так как имеет интерес только первые 5 бит, то маску можно сократить и легко запомнить: 10101 (десять-сто один).
	//После применения операции «исключающего или» (xor) получаем информацию.
	int info[5];
	int mask[5] = { 1,0,1,0,1 };
	info[0] = mask[0] == byte[0] ? 0 : 1;
	info[1] = mask[1] == byte[1] ? 0 : 1;
	info[2] = mask[2] == byte[2] ? 0 : 1;
	info[3] = mask[3] == byte[3] ? 0 : 1;
	info[4] = mask[4] == byte[4] ? 0 : 1;
	//первые 2 байта это уровень коррекции
	//остальные 3 это маска для информации

	//Шаг 3. Чтение заголовка данных
	//Чтобы понять с какими данными предстоит иметь дело, необходимо изначально прочитать 4 - х битный заголовок,
	//который содержит в себе информацию о режиме.

	float yInfo = O[3].y, xInfo = M[1].x;
	Rect RecInfo(xInfo, yInfo, -sizeBlock * 2, -sizeBlock * 2);///прямоугольник с заголовком данных
															   //rectangle(image, RecInfo, Scalar(255,0, 144), 1, 8, 0);
	int indicateMode[4];//индикатор режима
	GetHeadBytes(yInfo, xInfo, indicateMode);

	//Шаг 4. Применение маски к заголовку
	int maskMode[4];
	int j = 20;
	int i = 20;
	GetMask(info[2], info[3], info[4], i, j, maskMode);
	//вычисляем режим
	int mode[4];
	mode[0] = indicateMode[0] == maskMode[0] ? 0 : 1;
	mode[1] = indicateMode[1] == maskMode[1] ? 0 : 1;
	mode[2] = indicateMode[2] == maskMode[2] ? 0 : 1;
	mode[3] = indicateMode[3] == maskMode[3] ? 0 : 1;

	j -= 2;

	/*список нужных нам режимов
	Числовые	0001
	Буквенно - числовые	0010
	8 - битный(байтный)	0100*/

	//Шаг 5. Чтение данных
	//После получения данных о режиме можно приступать к чтению информации.
	//Надо оговорить, что наиболее интересно считывать числовые и буквенно - числовые данные, так как они легко интерпретируются.
	//Но также не стоит бояться 8 - битных.Это может быть также легко интерпретируемая информация. 
	//Например, многие онлайн генераторы QR текст кодируют в этом режиме, используя ASCII.
	//Ещё одна причина, почему следует изначально прочитать режим это то, что от него зависит количество пакетов данных.
	//Которая также зависит и от версии кода.Для версий с первой по девятую длины блоков для более читабельных режимов :

	//  Числовые	10 бит / 4 бита
	//	Буквенно - числовые	9 бит
	//	8 - битный(байтный)	8 бит

	//Первый блок после указателя режима – это количество символов.
	//Для числового режима количество закодировано в 10 следующих битах, а для 8 - битного режима в 8 битах

	if (mode[0] == 0 && mode[1] == 0 && mode[2] == 0 && mode[3] == 1) {//числовой режим
																	   //считываем первые 10 бит что бы узнать сколько пакетов информации
		int numberPackages[10], c = 0;

		ReadCountPackage(info[2], info[3], info[4], i, j, xInfo, yInfo, 10, numberPackages);
		j -= 5;

		int sum = 0, k = 0;
		for (int w = 9; w >= 0; w--)
		{
			if (numberPackages[w] == 1) sum += pow(2, k);
			k++;
		}

		k = 4;

		int p = 0, w, res = 0;
		bool stepbottom = false;
		int result[2][4], masknumber[4];
		char buffer[20];//разобраться с возвращаемымм значением
		for (w = 0; w< sum; w++)
		{
			for (int r = 0; r<2; r++)
			{
				if (j <= 9) {
					stepbottom = true;
					i -= 2;		p += 2; c = 4;
				}
				if (j >= (O[3].y - L[0].y) / sizeBlock) {
					j = (O[3].y - L[0].y) / sizeBlock;
					stepbottom = false;
					i -= 2;		p += 2; c = -6;
				}

				if (stepbottom)//если идем вниз
				{
					if (j < 9) j = 10;
					GetMask(info[2], info[3], info[4], i, j, masknumber);
					int one = masknumber[0];
					int two = masknumber[1];
					masknumber[0] = masknumber[2];
					masknumber[1] = masknumber[3];
					masknumber[2] = one;
					masknumber[3] = two;
					j += 2;
				}
				else {
					GetMask(info[2], info[3], info[4], i, j, masknumber);
					j -= 2;
				}

				for (int s = 0; s<4; s += 2)
				{
					result[r][s] = GettingByteValue(image.at<Vec3b>(yInfo - (6.5 + c)*sizeBlock, xInfo - (0.5 + p)* sizeBlock));
					result[r][s + 1] = GettingByteValue(image.at<Vec3b>(yInfo - (6.5 + c)*sizeBlock, xInfo - (1.5 + p) * sizeBlock));
					c++;
				}
				if (stepbottom) {
					int one = result[r][0];
					int two = result[r][1];
					result[r][0] = result[r][2];
					result[r][1] = result[r][3];
					result[r][2] = one;
					result[r][3] = two;
					c -= 4;
				}

				for (int s = 0; s<4; s++)
					result[r][s] = result[r][s] == masknumber[s] ? 0 : 1;
			}

			res = 0;
			int count = 0;
			for (int l = 3; l >= 0; l--)
			{
				if (result[1][l] == 1)
					res += pow(2, count);
				count++;
			}
			for (int l = 3; l >= 0; l--)
			{
				if (result[0][l] == 1)
					res += pow(2, count);
				count++;
			}
			buffer[w] = res;
		}
		buffer[w] = 0;

		std::printf("result is = %i\n", res);
		return std::string(buffer);
	}
	else if (mode[0] == 0 && mode[1] == 0 && mode[2] == 1 && mode[3] == 0) {//Буквенно - числовые
		char buffer[20];//разобраться с возвращаемымм значением


		std::printf("don't result\n");
		return buffer;
	}
	else if (mode[0] == 0 && mode[1] == 1 && mode[2] == 0 && mode[3] == 0) {//8 - битный(байтный)

		char buffer[20];//разобраться с возвращаемымм значением
						//считываем первые 8 бит что бы узнать сколько пакетов информации
		int numberPackages[8];
		ReadCountPackage(info[2], info[3], info[4], i, j, xInfo, yInfo, 8, numberPackages);
		j -= 4;
		int  sum = 0, k = 0;
		int c = 0;

		for (int w = 7; w >= 0; w--)
		{
			if (numberPackages[w] == 1)	sum += pow(2, k);
			k++;
		}
		int result[2][4], masknumber[4], p = 0, w, res = 0;
		bool stepbottom = false;

		/******************************** считывание информации******************************/
		for (w = 0; w< sum; w++)
		{
			for (int r = 0; r<2; r++)
			{
				if (j <= 9) {
					stepbottom = true;
					i -= 2;		p += 2; c = 4;
				}
				if (j >= (O[3].y - L[0].y) / sizeBlock) {
					j = (O[3].y - L[0].y) / sizeBlock;
					stepbottom = false;
					i -= 2;		p += 2; c = -6;
				}

				if (stepbottom)//если идем вниз
				{
					if (j < 9) j = 10;
					GetMask(info[2], info[3], info[4], i, j, masknumber);
					int one = masknumber[0];
					int two = masknumber[1];
					masknumber[0] = masknumber[2];
					masknumber[1] = masknumber[3];
					masknumber[2] = one;
					masknumber[3] = two;
					j += 2;
				}
				else {
					GetMask(info[2], info[3], info[4], i, j, masknumber);
					j -= 2;
				}

				for (int s = 0; s<4; s += 2)
				{
					result[r][s] = GettingByteValue(image.at<Vec3b>(yInfo - (6.5 + c)*sizeBlock, xInfo - (0.5 + p)* sizeBlock));
					result[r][s + 1] = GettingByteValue(image.at<Vec3b>(yInfo - (6.5 + c)*sizeBlock, xInfo - (1.5 + p) * sizeBlock));
					c++;
				}
				if (stepbottom) {
					int one = result[r][0];
					int two = result[r][1];
					result[r][0] = result[r][2];
					result[r][1] = result[r][3];
					result[r][2] = one;
					result[r][3] = two;
					c -= 4;
				}

				for (int s = 0; s<4; s++)
					result[r][s] = result[r][s] == masknumber[s] ? 0 : 1;
			}

			res = 0;
			int count = 0;
			for (int l = 3; l >= 0; l--)
			{
				if (result[1][l] == 1)
					res += pow(2, count);
				count++;
			}
			for (int l = 3; l >= 0; l--)
			{
				if (result[0][l] == 1)
					res += pow(2, count);
				count++;
			}
			buffer[w] = res;
		}
		buffer[w] = 0;

		std::printf("result is = %s\n", buffer);
		return std::string(buffer);

		//return buffer;
	}
	else if (mode[0] == 0 && mode[1] == 1 && mode[2] == 1 && mode[3] == 1) {//специальный режим
		char buffer[20];//разобраться с возвращаемымм значением


		std::printf("spesialt\n");
		return buffer;
	}
	else SendErrorMessage("QR-Code is incorrect");
	return "";
	/****************************************************************************************************/

	//return 0;
}




Orientation QRCode::GetOrientation() const
{
	return this->orientation;
}
