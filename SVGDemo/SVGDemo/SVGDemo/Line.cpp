#include "rapidxml.hpp"
#include "stdafx.h"
#include "Line.h"
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;
//line

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
		// draw with cordinate of points 
		string attributeName = firstAttribute->name();
		string attributeValue = firstAttribute->value();
		if (attributeName == "x1") x1 = stoi(attributeValue);
		else if (attributeName == "y1") y1 = stoi(attributeValue);
		else if (attributeName == "x2") x2 = stoi(attributeValue);
		else if (attributeName == "y2") y2 = stoi(attributeValue);
		else if (attributeName == "style") parseStyle(attributeValue);
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
	//set up ViewBox
	ViewBox* v = ViewBox::getInstance();
	v->applyViewBox(graphics);
	applyTransform(graphics);
	//Set up color
	int* Stroke = parseColor(getStroke());
	//Set up pen and draw
	Pen	pen(Color(stof(getStrokeOpacity()) * 255, Stroke[0], Stroke[1], Stroke[2]), stof(getStrokeWidth()));
	if (getStroke() != "none" && getStroke() != "") graphics.DrawLine(&pen, x1, y1, x2, y2);
}