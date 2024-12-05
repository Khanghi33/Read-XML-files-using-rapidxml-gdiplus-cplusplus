#include "rapidxml.hpp"
#include "stdafx.h"
#include "Circle.h"
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;

CIRCLE::CIRCLE() : cx(0), cy(0), r(0) {
	setStroke("");
	setFill("");
	setFillOpacity("1");
	setStrokeWidth("1");
	setStrokeOpacity("1");
}

CIRCLE::CIRCLE(int cx, int cy, int r, string stroke, string fill, string stroke_width, string fill_opacity, string stroke_opacity) {
	this->cx = cx;
	this->cy = cy;
	this->r = r;
	setStroke(stroke);
	setFill(fill);
	setFillOpacity(fill_opacity);
	setStrokeWidth(stroke_width);
	setStrokeOpacity(stroke_opacity);
}

CIRCLE::CIRCLE(xml_node<>* node) {
	xml_attribute<>* firstAttribute = node->first_attribute();
	//Default constructor
	cx = cy = r = 0;
	setStroke("rgb(0, 0, 0)");
	setFill("rgb(0, 0, 0)");
	setFillOpacity("1");
	setStrokeWidth("1");
	setStrokeOpacity("1");
	//Copy constructor
	while (firstAttribute != NULL)
	{
		string attributeName(firstAttribute->name());
		string attributeValue(firstAttribute->value());
		if (attributeName == "cx") cx = stof(attributeValue);
		else if (attributeName == "cy") cy = stof(attributeValue);
		else if (attributeName == "r") r = stof(attributeValue);
		else if (attributeName == "fill") setFill(attributeValue);
		else if (attributeName == "stroke") setStroke(attributeValue);
		else if (attributeName == "stroke-width") setStrokeWidth(attributeValue);
		else if (attributeName == "stroke-opacity") setStrokeOpacity(attributeValue);
		else if (attributeName == "fill-opacity") setFillOpacity(attributeValue);
		else if (attributeName == "transform") setTransform(attributeValue);
		firstAttribute = firstAttribute->next_attribute();
	}
}

CIRCLE::~CIRCLE() {}

VOID CIRCLE::Draw(HDC hdc)
{
	Graphics graphics(hdc);
	//Set up transform
	string transform = getTransform(); string trash;
	string translate, rotate, scale;
	if (transform.find("translate") >= 0 && transform.find("translate") < transform.length()) {
		stringstream ss(transform.substr(transform.find("translate")));
		getline(ss, trash, ')');
		translate = trash.substr(trash.find('(') + 1);
		graphics.TranslateTransform(static_cast<REAL>(stof(translate.substr(0, translate.find(',')))), static_cast<REAL>(stof(translate.substr(translate.find(',') + 1, translate.length() - translate.find(',')))));
	}
	if (transform.find("rotate") >= 0 && transform.find("rotate") < transform.length()) {
		stringstream ss(transform.substr(transform.find("rotate")));
		getline(ss, trash, ')');
		rotate = trash.substr(trash.find('(') + 1);;
		graphics.RotateTransform(stof(rotate));
	}
	if (transform.find("scale") >= 0 && transform.find("scale") < transform.length()) {
		stringstream ss(transform.substr(transform.find("scale")));
		getline(ss, trash, ')');
		scale = trash.substr(trash.find('(') + 1);
		if (scale.find(',') < 0 || scale.find(',') > scale.length()) graphics.ScaleTransform(stof(scale), stof(scale));
		else graphics.ScaleTransform(stof(scale.substr(0, scale.find(','))), stof(scale.substr(scale.find(',') + 1, scale.length() - scale.find(','))));
	}
	//Set up color pen and draw
	int* Stroke = parseColor(getStroke());
	Pen	pen(Color(stof(getStrokeOpacity()) * 255, Stroke[0], Stroke[1], Stroke[2]), stof(getStrokeWidth()));
	int* Fill = parseColor(getFill());
	SolidBrush brush(Color(stof(getFillOpacity()) * 255, Fill[0], Fill[1], Fill[2]));
    REAL d = r * 2;
	if(getStroke() != "none" && getStroke() != "") graphics.DrawEllipse(&pen, cx - r, cy - r, d, d);
	if(getFill() != "none" && getFill() != "") graphics.FillEllipse(&brush, cx - r, cy - r, d, d);
}