#pragma once
#include "Shape.h"
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;

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
