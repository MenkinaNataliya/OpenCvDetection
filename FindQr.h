#pragma once
#include "Header.h"
#include "QRCode.h"
#include <regex>

cv_Point FindAnyCode();
bool FindCode(cv_Point mark);
QRCode FindQrCode(Mat image);
