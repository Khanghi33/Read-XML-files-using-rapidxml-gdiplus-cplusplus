#pragma once
#include "stdafx.h"
#include "rapidxml.hpp"
#include "Shape.h"
#include <string>
#include <sstream>
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;
class ViewBox
{
private:
	REAL canvasWidth = 600.0;
	REAL canvasHeight = 400.0;
	REAL x = 0.0;
	REAL y = 0.0;
	REAL width = 600.0;
	REAL height = 400.0;
	bool check = false;
	static ViewBox* instance;
	ViewBox();
public:
	REAL getCanvasWidth() { return canvasWidth; }
	REAL getCanvasHeight() { return canvasHeight; }
	REAL getViewBoxWidth() { return width; }
	REAL getViewBoxHeight() { return height; }
	ViewBox(const ViewBox& other) = delete;
	ViewBox& operator=(const ViewBox& other) = delete;
	void applyViewBox(Graphics& g);
	static ViewBox* getInstance();
	void parse(xml_node<>* node);
	void parseViewBox(std::string value);
};

