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
#include <algorithm>
#include <regex>
#include <gdiplus.h>
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;

class Shape {
public:
	virtual VOID Draw(HDC) = 0;
};

class POLYGON : public Shape {
private: 
	string points;
	string stroke;
	string fill;
	string stroke_width; 
	string fill_opacity, stroke_opacity;
public:
	POLYGON(string points, string stroke, string fill,
		string stroke_width, string fill_opacity, string stroke_opacity);
	POLYGON();
	POLYGON(xml_node<>* node);
	~POLYGON();
	VOID Draw(HDC hdc);
};

class POLYLINE : public Shape {
private:
	string points;
	string stroke;
	string fill;
	string stroke_width;
	string fill_opacity, stroke_opacity;
public: 
	POLYLINE(string points, string stroke, string fill,
		string stroke_width, string fill_opacity, string stroke_opacity);
	POLYLINE();
	POLYLINE(xml_node<>* node);
	~POLYLINE();
	VOID Draw(HDC hdc);
};

class TXT : public Shape {
private:
	string content;
	int x;
	int y;
	string fill;
	int textSize;
public:
	TXT(string content, int x, int y, string fill, int textSize);
	TXT();
	TXT(xml_node<>* node);
	~TXT();

	VOID Draw(HDC hdc);
};

class Circle : public Shape {
private:
	int cx;
	int cy;
	int r;
	string stroke;
	string fill;
	string stroke_width;
	string fill_opacity, stroke_opacity;
public:
	Circle();
	Circle(int cx, int cy, int r, string stroke, string fill, string stroke_width, string fill_opactiy, string stroke_opacity);
	Circle(xml_node<>* node);
	~Circle();
	VOID Draw(HDC hdc);
};

class LINE : public Shape {
private:
	int x1, y1, x2, y2;
	string stroke;
	string stroke_width;
	string stroke_opacity;
public:
	LINE(xml_node<>* node);
	~LINE();
	VOID Draw(HDC hdc);
};

class RECTANGLES : public Shape {
private:
	int x, y, height, width;
	string stroke;
	string fill;
	string stroke_width;
	string fill_opacity, stroke_opacity;
public:
	RECTANGLES(xml_node<>* node);
	~RECTANGLES();
	VOID Draw(HDC hdc);
};

class ELIP : public Shape {
	int x, y, rx, ry;
	string stroke;
	string fill;
	string stroke_width;
	string fill_opacity, stroke_opacity;
public:
	ELIP(xml_node<>* node);
	~ELIP();
	VOID Draw(HDC hdc);
};

class Path : public Shape
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
	void parseColor(std::string attribute);
	void setColor(std::string hexColor);
	std::vector<std::string> getData();
	Color getColor();
	VOID Draw(HDC hdc);
};


class Render {
private:
	vector<Shape*> shapes;
	HDC* hdc;
public:
	Render(vector<Shape*> shapes, HDC* hdc);
	void GDIRender();
};
class SVGParser {
private:
	string filename;
	vector<Shape*> shapes;
public:
	vector<Shape*> getShapes();
	SVGParser(string filename);
	void parse();
};