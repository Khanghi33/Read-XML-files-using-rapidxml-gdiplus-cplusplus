#pragma once
#include "Shape.h"
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;

class RECTANGLES : public Shape {
private:
	REAL x, y, height, width;
public:
	RECTANGLES(xml_node<>* node);
	~RECTANGLES();
	VOID Draw(HDC hdc);
};