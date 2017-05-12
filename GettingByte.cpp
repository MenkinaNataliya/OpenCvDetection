#include "stdafx.h"
#include "QRCode.h"
#include "CountMask.h"

int QRCode::GettingByteValue(Vec3b m)
{
	if (m[0] >= 250 && m[1] >= 250 && m[2] >= 250) return 0;
	return 1;
}

void QRCode::GetSystemsBytes(float y, float  x, int byte[]) {

	byte[0] = GettingByteValue(image.at<Vec3b>(y + ceil(sizeBlock / 2), x + 0.5* sizeBlock));
	byte[1] = GettingByteValue(image.at<Vec3b>(y + ceil(sizeBlock / 2), x + 1.5* sizeBlock));
	byte[2] = GettingByteValue(image.at<Vec3b>(y + ceil(sizeBlock / 2), x + 2.5* sizeBlock));
	byte[3] = GettingByteValue(image.at<Vec3b>(y + ceil(sizeBlock / 2), x + 3.5* sizeBlock));
	byte[4] = GettingByteValue(image.at<Vec3b>(y + ceil(sizeBlock / 2), x + 4.5* sizeBlock));
}


void QRCode::GetHeadBytes(float y, float  x, int byte[]) {

	byte[0] = GettingByteValue(image.at<Vec3b>(y - ceil(sizeBlock / 2), x - 0.5* sizeBlock));
	byte[1] = GettingByteValue(image.at<Vec3b>(y - ceil(sizeBlock / 2), x - 1.5 * sizeBlock));
	byte[2] = GettingByteValue(image.at<Vec3b>(y - 1.5 * sizeBlock, x - 0.5* sizeBlock));
	byte[3] = GettingByteValue(image.at<Vec3b>(y - 1.5 * sizeBlock, x - 1.5 * sizeBlock));
}


void QRCode::ReadCountPackage(int onebit, int twobit, int threebit, int i, int j, int xInfo, int yInfo, int count, int numberPackages[])
{
	int c = 0;
	int maskcount[8];
	GetMaskByte(onebit, twobit, threebit, i, j, maskcount);
	for (int w = 0; w< count; w += 2)//читаем биты и сразу применяем маску
	{
		numberPackages[w] = GettingByteValue(image.at<Vec3b>(yInfo - (2.5 + c)*sizeBlock, xInfo - 0.5* sizeBlock));
		numberPackages[w] = numberPackages[w] == maskcount[w] ? 0 : 1;
		numberPackages[w + 1] = GettingByteValue(image.at<Vec3b>(yInfo - (2.5 + c)*sizeBlock, xInfo - 1.5* sizeBlock));
		numberPackages[w + 1] = numberPackages[w + 1] == maskcount[w + 1] ? 0 : 1;
		c++;
	}
}



