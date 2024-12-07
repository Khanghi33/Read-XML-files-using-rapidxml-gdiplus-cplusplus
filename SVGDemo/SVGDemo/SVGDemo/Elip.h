#pragma once
#include "Shape.h"
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;

class ELIP : public Shape {
private:
	REAL x, y, rx, ry;
public:
	ELIP(xml_node<>* node);
	~ELIP();
	VOID Draw(HDC hdc);
};