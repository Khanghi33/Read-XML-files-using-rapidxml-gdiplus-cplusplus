#pragma once
#include "stdafx.h"
#include "rapidxml.hpp"
#include <windows.h>
#include <objidl.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <regex>
#include <gdiplus.h>
#include <map>
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;

class Shape {
private:
	string fill, fill_opacity;
	string stroke, stroke_width, stroke_opacity;
	string transform ;
public:
	string getFill() { return fill; } 
	void setFill(string fill) { this->fill = fill; }
	string getFillOpacity() { return fill_opacity; }
	void setFillOpacity(string fillopacity) { this->fill_opacity = fillopacity; }
	string getStroke() { return stroke; }
	void setStroke(string stroke) { this->stroke = stroke; }
	string getStrokeWidth() { return stroke_width; }
	void setStrokeWidth(string strokewidth) { this->stroke_width = strokewidth; }
	string getStrokeOpacity() { return stroke_opacity; }
	void setStrokeOpacity(string strokeopacity) { this->stroke_opacity = strokeopacity; }
	string getTransform() { return transform; }
	void setTransform(string transform) { this->transform = transform; }
	virtual VOID Draw(HDC) = 0;
};

int* parseColor(string stroke);
vector<Point> parsePoints(string points);