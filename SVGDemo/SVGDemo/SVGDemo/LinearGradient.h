#pragma once
#include "stdafx.h"
#include "rapidxml.hpp"
#include "Shape.h"
#include <string>
#include <sstream>
#include <map>
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;
class LinearGradient
{
private:
	static LinearGradient* instance;
	LinearGradient();
	map<string, LinearGradientBrush*> colorMap;
public:
	static LinearGradient* getInstance();
	LinearGradient(const LinearGradient& other) = delete;
	LinearGradient& operator=(const LinearGradient& other) = delete;
	void parse(xml_node<>* node);
	void parseStop(xml_node<>* node, vector<Color>& gradientColors, vector<REAL>& gradientOffsets);
	LinearGradientBrush* getBrush(string id);
	~LinearGradient();
};

