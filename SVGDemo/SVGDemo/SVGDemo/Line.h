#pragma once
#include "Shape.h"
#include "ViewBox.h"
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;
//simple shape 
class LINE : public Shape {
private:
	REAL x1, y1, x2, y2;
public:
	LINE(xml_node<>* node);
	~LINE();
	VOID Draw(HDC hdc);
};