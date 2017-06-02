#include "stdafx.h"
#include "Header.h"
#include <regex>

bool CheckMessageMatch(string ms)
{
	regex rx[4];
	rx[0] = ("x: * y: *");
	rx[1] = ("x: [0-9]{1,3}; y: [0-9]{1,3}");
	rx[2] = ("X: [0-9]{1,3}\nY: [0-9]{1,3}");
	rx[3] = ("x * y *");
	bool itog = false;
	for(auto r: rx)
	{
		if (regex_match(ms.begin(), ms.end(), r) == true)return true;
	}
}
bool CheckMessageSearch(string ms)
{

	regex rfind[2];
	rfind[0] = ("x:");
	rfind[1] = ("y:");
	for (auto r : rfind)
	{
		if (regex_search(ms.begin(), ms.end(), r) == true) return true;
	}
	return false;
}
