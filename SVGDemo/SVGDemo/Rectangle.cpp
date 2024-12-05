#include "rapidxml.hpp"
#include "stdafx.h"
#include "Rectangle.h"
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;

RECTANGLES::RECTANGLES(xml_node<>* node)
{
	xml_attribute<>* firstAttribute = node->first_attribute();
	//Default constructor
	x = y = height = width = 0;
	setStroke("rgb(0, 0, 0)");
	setFill("rgb(0, 0, 0)");
	setFillOpacity("1");
	setStrokeWidth("1");
	setStrokeOpacity("1");
	while (firstAttribute != NULL) {
		string attributeName = firstAttribute->name();
		string attributeValue = firstAttribute->value();
		if (attributeName == "x") x = stof(attributeValue);
		else if (attributeName == "y") y = stof(attributeValue);
		else if (attributeName == "height") height = stof(attributeValue);
		else if (attributeName == "width") width = stof(attributeValue);
		else if (attributeName == "stroke") setStroke(attributeValue);
		else if (attributeName == "stroke-width") setStrokeWidth(attributeValue);
		else if (attributeName == "stroke-opacity") setStrokeOpacity(attributeValue);
		else if (attributeName == "fill") setFill(attributeValue);
		else if (attributeName == "fill-opacity") setFillOpacity(attributeValue);
		else if (attributeName == "transform") setTransform(attributeValue);
		firstAttribute = firstAttribute->next_attribute();
	}
}

RECTANGLES::~RECTANGLES()
{
}

VOID RECTANGLES::Draw(HDC hdc)
{
	ofstream fout("log.txt", ios::app);
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
		else {
			graphics.ScaleTransform(stof(scale.substr(0, scale.find(','))), stof(scale.substr(scale.find(',') + 1, scale.length() - scale.find(','))));
			fout << stof(scale.substr(0, scale.find(','))) << " " << stof(scale.substr(scale.find(',') + 1, scale.length() - scale.find(','))) << "\n";
		}
	}
	//Set up pen color and draw
	int* Stroke = parseColor(getStroke());
	Pen	pen(Color(stof(getStrokeOpacity()) * 255, Stroke[0], Stroke[1], Stroke[2]), stof(getStrokeWidth()));
	int* Fill = parseColor(getFill());
	SolidBrush brush(Color(stof(getFillOpacity()) * 255, Fill[0], Fill[1], Fill[2]));
	if(getStroke() != "none") graphics.DrawRectangle(&pen, x, y, width, height);
	if(getFill() != "none") graphics.FillRectangle(&brush, x, y, width, height);
}