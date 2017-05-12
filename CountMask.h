#pragma once

int mask000(int i, int j);
int mask001(int i);
int mask010(int j);
int mask011(int i, int j);
int mask100(int i, int j);
int mask101(int i, int j);
int mask110(int i, int j);
int mask111(int i, int j);
void GetMask(int b1, int b2, int b3, int i, int j, int maskMode[]);

void GetMaskByte(int b1, int b2, int b3, int j, int i, int maskMode[]);
