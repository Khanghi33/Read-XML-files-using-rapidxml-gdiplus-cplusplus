#pragma once
#include "Shape.h"
#include "ViewBox.h"
#include "LinearGradient.h"
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;

class CIRCLE : public Shape {
private:
	REAL cx, cy, r;
public:
	CIRCLE();
	CIRCLE(int cx, int cy, int r, string stroke, string fill, string stroke_width, string fill_opactiy, string stroke_opacity);
	CIRCLE(xml_node<>* node);
	~CIRCLE();
	VOID Draw(HDC hdc);
};