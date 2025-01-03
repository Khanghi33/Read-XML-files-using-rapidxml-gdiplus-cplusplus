#include "rapidxml.hpp"
#include "stdafx.h"
#include "Polyline.h"
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;
//polyline

POLYLINE::POLYLINE(string Points, string Stroke, string Fill, string Stroke_width, string Fill_opacity, string Stroke_opacity)
{
	points = Points;
	setStroke(Stroke);
	setFill(Fill);
	setFillOpacity(Fill_opacity);
	setStrokeWidth(Stroke_width);
	setStrokeOpacity(Stroke_opacity);
}

POLYLINE::POLYLINE()
{
	points = "";
	setStroke("");
	setFill("");
	setFillOpacity("1");
	setStrokeWidth("1");
	setStrokeOpacity("1");
}

POLYLINE::POLYLINE(xml_node<>* node)
{
	//Default Constructor
	//default attributes set to 0 or 1 
	points = "";
	setStroke("");
	setFill("");
	setFillOpacity("1");
	setStrokeWidth("1");
	setStrokeOpacity("1");
	//Copy Constructor
	xml_attribute<>* firstAttribute = node->first_attribute();
	while (firstAttribute != NULL) {
		string nameAttribute = firstAttribute->name();
		if (nameAttribute == "points") points = firstAttribute->value();
		else if (nameAttribute == "fill") setFill(firstAttribute->value());
		else if (nameAttribute == "style") parseStyle(firstAttribute->value());
		else if (nameAttribute == "fill-opacity") setFillOpacity(firstAttribute->value());
		else if (nameAttribute == "stroke") setStroke(firstAttribute->value());
		else if (nameAttribute == "stroke-width") setStrokeWidth(firstAttribute->value());
		else if (nameAttribute == "stroke-opacity") setStrokeOpacity(firstAttribute->value());
		else if (nameAttribute == "transform") setTransform(firstAttribute->value());
		firstAttribute = firstAttribute->next_attribute();
	}
}

POLYLINE::~POLYLINE()
{
}

VOID POLYLINE::Draw(HDC hdc)
{
	//draw with pen and use class graphic 
	Graphics graphics(hdc);
	ViewBox* v = ViewBox::getInstance();
	v->applyViewBox(graphics);
	applyTransform(graphics);
	//Set up pen color and draw
	int* Stroke = parseColor(getStroke());
	Pen	pen(Color(stof(getStrokeOpacity()) * 255, Stroke[0], Stroke[1], Stroke[2]), stof(getStrokeWidth()));
	int* Fill = parseColor(getFill());
	SolidBrush brush(Color(stof(getFillOpacity()) * 255, Fill[0], Fill[1], Fill[2]));
	vector<Point> Points = parsePoints(this->points);
	Point* points = Points.data();
	if (getStroke() != "none" && getStroke() != "") graphics.DrawLines(&pen, Points.data(), Points.size());
	if (getFill() != "none" && getFill() != "") graphics.FillPolygon(&brush, Points.data(), Points.size());
}
//end plyline