#include "stdafx.h"
#include "QRCode.h"
#include "CountMask.h"

int QRCode::GettingByteValue(int m)
{
	if (m == 255) return 0;
	else return 1;
}

void QRCode::GetSystemsBytes(float y, float  x, int byte[]) {
	try
	{
		Scalar colour;
		for(int u =0 ; u<5; u++)
		{
			colour = image.at<uchar>(Point(x + (u+0.5)* sizeBlock, y + ceil(sizeBlock / 2)));
			byte[u] = GettingByteValue(colour.val[0]);
		}
	}
	catch (Exception){	}
}


void QRCode::GetHeadBytes(float y, float  x, int byte[]) {
	try
	{
		Scalar colour;
		colour = image.at<uchar>(Point(x - 0.5* sizeBlock, y - ceil(sizeBlock / 2)));
		byte[0] = GettingByteValue(colour.val[0]);

		colour = image.at<uchar>(Point(x - 1.5 * sizeBlock, y - ceil(sizeBlock / 2)));
		byte[1] = GettingByteValue(colour.val[0]);

		colour = image.at<uchar>(Point(x - 0.5 * sizeBlock, y - 1.5 * sizeBlock));
		byte[2] = GettingByteValue(colour.val[0]);

		colour = image.at<uchar>(Point(x - 1.5 * sizeBlock, y - 1.5 * sizeBlock));
		byte[3] = GettingByteValue(colour.val[0]);
	}
	catch(Exception){	}
}


void QRCode::ReadCountPackage(int onebit, int twobit, int threebit, int i, int j, int xInfo, int yInfo, int count, int numberPackages[])
{
	int c = 0;
	int maskcount[8];
	GetMaskByte(onebit, twobit, threebit, i, j, maskcount);
	Scalar colour;
	try
	{
		for (int w = 0; w< count; w += 2)//читаем биты и сразу применяем маску
		{
			
			colour = image.at<uchar>(Point(xInfo - 0.5* sizeBlock, yInfo - (2.5 + c)*sizeBlock));
			numberPackages[w] = GettingByteValue(colour.val[0]);
			numberPackages[w] = numberPackages[w] == maskcount[w] ? 0 : 1;

			colour = image.at<uchar>(Point(xInfo - 1.5* sizeBlock, yInfo - (2.5 + c)*sizeBlock));
			numberPackages[w + 1] = GettingByteValue(colour.val[0]);
			numberPackages[w + 1] = numberPackages[w + 1] == maskcount[w + 1] ? 0 : 1;
			c++;
		}
	}
	catch(Exception){}
	
}



