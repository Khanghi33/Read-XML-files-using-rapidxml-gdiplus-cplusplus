#pragma once
#include "Shape.h"
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;

class LINE : public Shape {
private:
	int x1, y1, x2, y2;
public:
	LINE(xml_node<>* node);
	~LINE();
	VOID Draw(HDC hdc);
};