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
#include <map>
#include <gdiplus.h>
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;

class Shape {
private:
	string fill, fill_opacity;
	string stroke, stroke_width, stroke_opacity;
	string transform = "";
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
//unsigned int hexToARGB(const std::string& hexColor);

//{
//    if (hexColor[0] != '#' || hexColor.size() != 7) {
//        throw std::invalid_argument("Invalid color format. Expected format: #RRGGBB");
//    }
//
//    // Parse the RGB components
//    unsigned int r, g, b;
//    std::istringstream(hexColor.substr(1, 2)) >> std::hex >> r;
//    std::istringstream(hexColor.substr(3, 2)) >> std::hex >> g;
//    std::istringstream(hexColor.substr(5, 2)) >> std::hex >> b;
//
//    // Construct ARGB with full opacity (0xFF for alpha)
//    unsigned int argb = (0xFF << 24) | (r << 16) | (g << 8) | b;
//    return argb;
//}
//
//void Path::parseColor(std::string attribute)
//{
//    std::string fillColor;
//
//    // Regular expressions to match clip-path and fill color
//    //std::regex fillColorRegex(R"(fill:(#[A-Fa-f0-9]+))");
//    std::regex fillColorRegex(R"(fill:(#[A-Fa-f0-9]{6}))");
//    std::smatch match;
//    // Extract fill color
//    if (std::regex_search(attribute, match, fillColorRegex)) {
//        fillColor = match[1]; // First capturing group
//    }
//    ofstream out("color.txt", ios::out);
//    setColor(fillColor);
//}
//
//void Path::setColor(std::string hexColor)
//{
//    if (hexColor[0] != '#' || hexColor.size() != 7) {
//        throw std::invalid_argument("Invalid color format. Expected format: #RRGGBB");
//    }
//
//    // Parse the RGB components
//    unsigned int r, g, b;
//    std::istringstream(hexColor.substr(1, 2)) >> std::hex >> r;
//    std::istringstream(hexColor.substr(3, 2)) >> std::hex >> g;
//    std::istringstream(hexColor.substr(5, 2)) >> std::hex >> b;
//    this->color = new Color(255, r, g, b);
//    // Construct ARGB with full opacity (0xFF for alpha)
//}