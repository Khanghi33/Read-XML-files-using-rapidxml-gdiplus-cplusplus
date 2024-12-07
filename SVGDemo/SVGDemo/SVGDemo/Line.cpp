#include "rapidxml.hpp"
#include "stdafx.h"
#include "Line.h"
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;

LINE::LINE(xml_node<>* node)
{
	xml_attribute<>* firstAttribute = node->first_attribute();
	//Default constructor
	x1 = y1 = x2 = y2 = 0;
	setStroke("");
	setStrokeWidth("1");
	setStrokeOpacity("1");
	//Copy constructor
	while (firstAttribute != NULL) {
		string attributeName = firstAttribute->name();
		string attributeValue = firstAttribute->value();
		if (attributeName == "x1") x1 = stoi(attributeValue);
		else if (attributeName == "y1") y1 = stoi(attributeValue);
		else if (attributeName == "x2") x2 = stoi(attributeValue);
		else if (attributeName == "y2") y2 = stoi(attributeValue);
		else if (attributeName == "stroke") setStroke(attributeValue);
		else if (attributeName == "stroke-width") setStrokeWidth(attributeValue);
		else if (attributeName == "stroke-opacity") setStrokeOpacity(attributeValue);
		else if (attributeName == "transform") setTransform(attributeValue);
		firstAttribute = firstAttribute->next_attribute();
	}
}

LINE::~LINE()
{
}

VOID LINE::Draw(HDC hdc)
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
	//Set up color
	int* Stroke = parseColor(getStroke());
	//Set up pen and draw
	Pen	pen(Color(stof(getStrokeOpacity()) * 255, Stroke[0], Stroke[1], Stroke[2]), stof(getStrokeWidth()));
	if (getStroke() != "none" && getStroke() != "") graphics.DrawLine(&pen, x1, y1, x2, y2);
}