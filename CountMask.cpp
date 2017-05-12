#include "stdafx.h"
#include "CountMask.h"

int mask000(int i, int j) { return (i + j) % 2 == 0 ? 1 : 0; }

int mask001(int i) { return i % 2 == 0 ? 1 : 0; }

int mask010(int j) { return j % 3 == 0 ? 1 : 0; }

int mask011(int i, int j) { return (i + j) % 3 == 0 ? 1 : 0; }

int mask100(int i, int j) { return ((i / 2) + (j / 3)) % 2 == 0 ? 1 : 0; }

int mask101(int i, int j) { return ((i*j) % 2) + ((j*i) % 3) == 0 ? 1 : 0; }

int mask110(int i, int j) { return((((i*j) % 2) + ((j*i) % 3)) % 2) == 0 ? 1 : 0; }

int mask111(int i, int j) { return ((i + j) % 2 + j*i % 3) % 2 == 0 ? 1 : 0; }

void GetMaskByte(int b1, int b2, int b3, int i, int j, int maskMode[])
{
	if (b1 == 0) {
		if (b2 == 0) {
			if (b3 == 0) {	//(i + j) mod 2 = 0
				int c = 0;
				for (int k = j; k>j - 4; k--)
				{
					maskMode[c] = mask000(i, k);
					maskMode[c + 1] = mask000(i - 1, k);
					c += 2;
				}
			}
			else {//i mod 2 = 0
				int c = 0;
				for (int k = j; k>j - 4; k--)
				{
					maskMode[c] = mask001(k);
					maskMode[c + 1] = mask001(k);
					c += 2;
				}
			}
		}
		else {
			if (b3 == 0) {//j mod 3 = 0
				int c = 0;
				for (int k = j; k>j - 4; k--)
				{
					maskMode[c] = mask010(i);
					maskMode[c + 1] = mask010(i - 1);
					c += 2;
				}
			}
			else {	//(i + j) mod 3 = 0
				int c = 0;
				for (int k = j; k>j - 4; k--)
				{
					maskMode[c] = mask011(i, k);
					maskMode[c + 1] = mask011(i - 1, k);
					c += 2;
				}
			}
		}
	}
	else {
		if (b2 == 0) {
			if (b3 == 0) {//((i div 2) + (j div 3)) mod 2 = 0
				int c = 0;
				for (int k = j; k>j - 4; k--)
				{
					maskMode[c] = mask100(k, i);
					maskMode[c + 1] = mask100(k, i - 1);
					c += 2;
				}
			}
			else {//(i j) mod 2 + (i j) mod 3 = 0
				int c = 0;
				for (int k = j; k>j - 4; k--)
				{
					maskMode[c] = mask101(i, k);
					maskMode[c + 1] = mask101(i - 1, k);
					c += 2;
				}
			}
		}
		else {
			if (b3 == 0) {//((i j) mod 2 + (i j) mod 3) mod 2 = 0
				int c = 0;
				for (int k = j; k>j - 4; k--)
				{
					maskMode[c] = mask110(i, k);
					maskMode[c + 1] = mask110(i - 1, k);
					c += 2;
				}
			}
			else {//((i+j) mod 2 + (i j) mod 3) mod 2 = 0
				int c = 0;
				for (int k = j; k>j - 4; k--)
				{
					maskMode[c] = mask111(k, i);
					maskMode[c + 1] = mask111(k, i - 1);
					c += 2;
				}
			}
		}
	}
}


void GetMask(int b1, int b2, int b3, int i, int j, int maskMode[])
{
	if (b1 == 0) {
		if (b2 == 0) {
			if (b3 == 0) {	//(i + j) mod 2 = 0
				maskMode[0] = mask000(i, j);
				maskMode[1] = mask000(i - 1, j);
				maskMode[2] = mask000(i, j - 1);
				maskMode[3] = mask000(i - 1, j - 1);

			}
			else {//i mod 2 = 0
				int c = 0;
				for (int k = j; k>j - 2; k--)
				{
					maskMode[c] = mask001(k);
					maskMode[c + 1] = mask001(k);
					c += 2;
				}
			}
		}
		else {
			if (b3 == 0) {//j mod 3 = 0
				maskMode[0] = maskMode[1] = mask010(j);
				maskMode[2] = maskMode[3] = mask010(j - 1);
			}
			else {	//(i + j) mod 3 = 0
				maskMode[0] = mask011(i, j);
				maskMode[1] = mask011(i, j - 1);
				maskMode[2] = mask011(i - 1, j);
				maskMode[3] = mask011(i - 1, j - 1);
			}
		}
	}
	else {
		if (b2 == 0) {
			if (b3 == 0) {//((i div 2) + (j div 3)) mod 2 = 0
				maskMode[0] = mask100(j, i);
				maskMode[1] = mask100(j, i - 1);
				maskMode[2] = mask100(j - 1, i);
				maskMode[3] = mask100(j - 1, i - 1);
			}
			else {//(i j) mod 2 + (i j) mod 3 = 0
				maskMode[0] = mask101(j, i);
				maskMode[1] = mask101(i - 1, j);
				maskMode[2] = mask101(i, j - 1);
				maskMode[3] = mask101(i - 1, j - 1);
			}
		}
		else {
			if (b3 == 0) {//((i j) mod 2 + (i j) mod 3) mod 2 = 0
				int c = 0;
				for (int k = j; k>j - 2; k--)
				{
					maskMode[c] = mask110(i, k);
					maskMode[c + 1] = mask110(i - 1, k);
					c += 2;
				}
			}
			else {//((i+j) mod 2 + (i j) mod 3) mod 2 = 0
				maskMode[0] = mask111(i, j);
				maskMode[1] = mask111(i - 1, j);
				maskMode[2] = mask111(i, j - 1);
				maskMode[3] = mask111(i - 1, j - 1);
			}
		}
	}
}