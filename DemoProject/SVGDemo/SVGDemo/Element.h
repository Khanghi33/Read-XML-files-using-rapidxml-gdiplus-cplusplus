#pragma once
#include "stdafx.h"
#include "rapidxml.hpp"
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
using namespace std;
using namespace rapidxml;

class POLYGON {
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

class POLYLINE {
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

class TXT {
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

class Circle {
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

class LINE {
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

class RECTANGLES {
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

class ELIP {
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