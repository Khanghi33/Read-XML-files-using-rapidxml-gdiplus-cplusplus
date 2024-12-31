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
	setStrokeOpacity("1");// opacity something
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
	setStroke("");
	setFill("");
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
		else if (attributeName == "style") parseStyle(attributeValue);
		else if (attributeName == "fill-opacity") setFillOpacity(attributeValue);
		else if (attributeName == "transform") setTransform(attributeValue);
		firstAttribute = firstAttribute->next_attribute();
	}
}

CIRCLE::~CIRCLE() {}

VOID CIRCLE::Draw(HDC hdc)
{
	Graphics graphics(hdc);
	ViewBox* v = ViewBox::getInstance();
	v->applyViewBox(graphics);
	applyTransform(graphics);
	//Set up color pen and draw
	int* Stroke = parseColor(getStroke());
	Pen	pen(Color(stof(getStrokeOpacity()) * 255, Stroke[0], Stroke[1], Stroke[2]), stof(getStrokeWidth()));
	int* Fill = parseColor(getFill());
	SolidBrush brush(Color(stof(getFillOpacity()) * 255, Fill[0], Fill[1], Fill[2]));
	REAL d = r * 2;
	if (getStroke() != "none" && getStroke() != "") graphics.DrawEllipse(&pen, cx - r, cy - r, d, d);
	/*if (getFill() != "none" && getFill() != "") graphics.FillEllipse(&brush, cx - r, cy - r, d, d);*/
	if (getGradientId(getFill()) != "") {
		string id = getGradientId(getFill());

		// Get the bounds of the path
		RectF bounds(cx - r, cy - r, d, d);

		// Get the gradient instance
		LinearGradient* gradient = LinearGradient::getInstance();

		// Pass the bounds to the getBrush method
		LinearGradientBrush* gradientBrush = gradient->getBrush(id, &bounds);
		/* linearGradientBrush give brush to gradient 
		increse the accuracy of the data
		learning and finding out the way to execute the thing in data.
		*/
		// Fill the path with the gradient brush
		if (gradientBrush != nullptr) {
			graphics.FillEllipse(gradientBrush, cx - r, cy - r, d, d);
		}

		// Clean up if necessary (if getBrush dynamically allocates the brush)
		delete gradientBrush;

	}
	// 
	else graphics.FillEllipse(&brush, cx - r, cy - r, d, d);
}
// graphics ellpise brush 


/*
_________                        _______                      _  ._    .__                         _         _        ___               ___
_                          _      ___                     ___      _.             ___
_         _                 ___  _                 ____  _                 ____              .__           ___.__                 ___
\__    __/_.__.______   __    /   ____/ ___   ___   ____/  |_|  |_ |__| __    __   ___  |  | _    |__| _____/ ____\___    ____/ ____\  | ______    ____  _  |  | ___/ ____\___
*/