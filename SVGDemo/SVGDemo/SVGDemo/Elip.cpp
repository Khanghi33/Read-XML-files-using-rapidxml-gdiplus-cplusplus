#include "rapidxml.hpp"
#include "stdafx.h"
#include "Elip.h"
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;
//elip

ELIP::ELIP(xml_node<>* node)
{
	xml_attribute<>* firstAttribute = node->first_attribute();
	//Default constructor
	x = y = rx = ry = 0;
	setStroke("");
	setFill("");
	setFillOpacity("1");
	setStrokeWidth("1");
	setStrokeOpacity("1");
	//Copy constructor
	while (firstAttribute != NULL) {
		string attributeName = firstAttribute->name();
		string attributeValue = firstAttribute->value();
		if (attributeName == "cx") x = stoi(attributeValue);
		else if (attributeName == "cy") y = stoi(attributeValue);
		else if (attributeName == "rx") rx = stoi(attributeValue);
		else if (attributeName == "ry") ry = stoi(attributeValue);
		else if (attributeName == "fill") setFill(firstAttribute->value());
		else if (attributeName == "style") parseStyle(attributeValue);
		else if (attributeName == "fill-opacity") setFillOpacity(firstAttribute->value());
		else if (attributeName == "stroke") setStroke(firstAttribute->value());
		else if (attributeName == "stroke-width") setStrokeWidth(firstAttribute->value());
		else if (attributeName == "stroke-opacity") setStrokeOpacity(firstAttribute->value());
		else if (attributeName == "transform") setTransform(attributeValue);
		firstAttribute = firstAttribute->next_attribute();
	}
}

ELIP::~ELIP()
{
}

VOID ELIP::Draw(HDC hdc)
{
	Graphics graphics(hdc);
	ViewBox* v = ViewBox::getInstance();
	v->applyViewBox(graphics);
	applyTransform(graphics);
	//Set pen color and draw
	int* Stroke = parseColor(getStroke());
	Pen	pen(Color(stof(getStrokeOpacity()) * 255, Stroke[0], Stroke[1], Stroke[2]), stof(getStrokeWidth()));
	int* Fill = parseColor(getFill());
	SolidBrush brush(Color(stof(getFillOpacity()) * 255, Fill[0], Fill[1], Fill[2]));
	if (getStroke() != "none" && getStroke() != "") graphics.DrawEllipse(&pen, x - rx, y - ry, rx * 2, ry * 2);
	/*if (getFill() != "none" && getFill() != "") graphics.FillEllipse(&brush, x - rx, y - ry, rx * 2, ry * 2);*/
	graphics.FillEllipse(&brush, x - rx, y - ry, rx * 2, ry * 2);
	/*
	graphics.FillEllipse is determined to fullfill the empty space inside the ellipse.
	*/
	if (getStroke() != "none" && getStroke() != "") graphics.DrawEllipse(&pen, x - rx, y - ry, rx * 2, ry * 2);
	/*if (getFill() != "none" && getFill() != "") graphics.FillEllipse(&brush, cx - r, cy - r, d, d);*/
	if (getGradientId(getFill()) != "") {
		string id = getGradientId(getFill());

		// Get the bounds of the path
		RectF bounds(x - rx, y - ry, rx * 2, ry * 2);

		// Get the gradient instance
		LinearGradient* gradient = LinearGradient::getInstance();

		// Pass the bounds to the getBrush method
		LinearGradientBrush* gradientBrush = gradient->getBrush(id, &bounds);

		// Fill the path with the gradient brush
		if (gradientBrush != nullptr) {
			graphics.FillEllipse(gradientBrush, x - rx, y - ry, rx * 2, ry * 2);
		}
		/*
		fill ellipse if gradientBrush is not empty
		zzzzzzzzzzzzzzzzzzz
		if not then cleaning up
		*/
		// Clean up if necessary (if getBrush dynamically allocates the brush)
		delete gradientBrush;
	}
	else graphics.FillEllipse(&brush, x - rx, y - ry, rx * 2, ry * 2);
}