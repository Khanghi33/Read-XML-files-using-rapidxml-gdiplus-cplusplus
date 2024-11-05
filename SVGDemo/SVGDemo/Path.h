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
#include <cmath>
#include <algorithm>
#include <gdiplus.h>
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;
class Path
{
private:
	std::vector<std::string> data;
	Color* color = nullptr;
public:
	~Path();
	Path(xml_node<>* node);
	void parseFile(std::string filename);
	void parseElement(std::string d);
	void createGraphicsPath(GraphicsPath& graphicsPath);
	unsigned int hexToARGB(const std::string& hexColor);
	void setColor(std::string hexColor);
	std::vector<std::string> getData();
	std::vector<pair<char, vector<PointF>>> getVectorData();
	Color getColor();
	void draw(HDC hdc);
};

