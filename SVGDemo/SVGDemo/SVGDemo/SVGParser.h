#pragma once
#include "rapidxml.hpp"
#include "stdafx.h"
#include "Shape.h"
#include "Polygon.h"
#include "Polyline.h"
#include "Circle.h"
#include "Elip.h"
#include "Line.h"
#include "Rectangle.h"
#include "Text.h"
#include "Path.h"
#include "Group.h"
#include "ViewBox.h"
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;

class SVGParser {
private:
	string filename;
	vector<Shape*> shapes;
public:
	vector<Shape*> getShapes();
	SVGParser(string filename);
	void parse();
};