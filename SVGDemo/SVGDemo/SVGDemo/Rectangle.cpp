#include "rapidxml.hpp"
#include "stdafx.h"
#include "Rectangle.h"
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;
//rectangle

RECTANGLES::RECTANGLES(xml_node<>* node)
{
	xml_attribute<>* firstAttribute = node->first_attribute();
	//Default constructor
	x = y = height = width = 0;
	setStroke("");
	setFill("");
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
		else if (attributeName == "style") parseStyle(attributeValue);
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
	ViewBox* v = ViewBox::getInstance();
	v->applyViewBox(graphics);
	applyTransform(graphics);
	//Set up pen color and draw
	int* Stroke = parseColor(getStroke());
	Pen	pen(Color(stof(getStrokeOpacity()) * 255, Stroke[0], Stroke[1], Stroke[2]), stof(getStrokeWidth()));
	int* Fill = parseColor(getFill());
	SolidBrush brush(Color(stof(getFillOpacity()) * 255, Fill[0], Fill[1], Fill[2]));
	if (getStroke() != "none" && getStroke() != "") graphics.DrawRectangle(&pen, x, y, width, height);
	if (getFill() != "none" && getFill() != "") graphics.FillRectangle(&brush, x, y, width, height);
}