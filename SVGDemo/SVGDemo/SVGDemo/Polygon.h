#pragma once
#include "Shape.h"
#include "ViewBox.h"
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;
//class polygon using points and stroke
class POLYGON : public Shape {
private:
	string points;
public:
	POLYGON(string points, string stroke, string fill,
		string stroke_width, string fill_opacity, string stroke_opacity);
	POLYGON();
	POLYGON(xml_node<>* node);
	~POLYGON();
	VOID Draw(HDC hdc);
};