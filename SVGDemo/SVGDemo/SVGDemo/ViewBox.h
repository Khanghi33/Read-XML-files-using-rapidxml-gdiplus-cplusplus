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
	REAL canvasWidth = 800.0;
	REAL canvasHeight = 600.0;
	REAL x = 0.0;
	REAL y = 0.0;
	REAL width = 0.0;
	REAL height = 0.0;
	bool check = false;
	static ViewBox* instance;
	ViewBox();
public:
	ViewBox(const ViewBox& other) = delete;
	ViewBox& operator=(const ViewBox& other) = delete;
	void applyViewBox(Graphics& g);
	static ViewBox* getInstance();
	void parse(xml_node<>* node);
	void parseViewBox(std::string value);
};

