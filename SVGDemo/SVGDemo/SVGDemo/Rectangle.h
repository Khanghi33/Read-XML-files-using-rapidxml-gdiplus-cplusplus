#pragma once
#include "Shape.h"
#include "ViewBox.h"

using namespace std;
using namespace rapidxml;
using namespace Gdiplus;

class RECTANGLES : public Shape {
private:
	REAL x, y, height, width;
public:
	//only use draw function for simple action
	RECTANGLES(xml_node<>* node);
	~RECTANGLES();
	VOID Draw(HDC hdc);
};