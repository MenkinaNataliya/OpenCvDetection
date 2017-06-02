#pragma once
#include "Header.h"

class QRCode
{
private:
	Orientation orientation;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	vector<Point2f> L, M, O;
	int top, right, bottom;
	int sizeBlock;
	float slope;
	bool correctCode;
	bool doubleRead;

	void CalculateOrientation();
	void ReadCountPackage(int onebit, int twobit, int threebit, int i, int j, int xInfo, int yInfo, int count, int numberPackages[]);
	void GetHeadBytes(float y, float  x, int byte[]);
	int GettingByteValue(int m);
	void GetSystemsBytes(float y, float  x, int byte[]);
	void DrawTarget();
	void Resize();

public:
	Mat image;

	Orientation GetOrientation() const;

	string Read();
	string ReRead();
	bool Empty();

	QRCode(Mat image);
	QRCode(Mat image, Orientation orient, int size);
	QRCode();
	~QRCode();
};

