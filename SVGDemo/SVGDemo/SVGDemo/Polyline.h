#pragma once
#include "Shape.h"
#include "ViewBox.h"

using namespace std;
using namespace rapidxml;
using namespace Gdiplus;


class POLYLINE : public Shape {
private:
	string points;
public:
	POLYLINE(string points, string stroke, string fill,
		string stroke_width, string fill_opacity, string stroke_opacity);
	POLYLINE();
	POLYLINE(xml_node<>* node);
	~POLYLINE();
	VOID Draw(HDC hdc);
};