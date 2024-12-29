#include "rapidxml.hpp"
#include "stdafx.h"
#include "Polygon.h"
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;

//Default constructor for POLYGON class
POLYGON::POLYGON()
{
	points = "";
	setStroke("");
	setFill("");
	setFillOpacity("1");
	setStrokeWidth("1");
	setStrokeOpacity("1");
}
//Copy constructor for POLYGON 
POLYGON::POLYGON(string Points, string Stroke, string Fill, string Stroke_width, string Fill_opacity, string Stroke_opacity)
{
	points = Points;
	setStroke(Stroke);
	setFill(Fill);
	setFillOpacity(Fill_opacity);
	setStrokeWidth(Stroke_width);
	setStrokeOpacity(Stroke_opacity);
}
//Copy constructor for POLYGON to read data from xml node
POLYGON::POLYGON(xml_node<>* node)
{
	//Default Constructor
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
//Destructor
POLYGON::~POLYGON()
{
}
VOID POLYGON::Draw(HDC hdc)
{
	Graphics graphics(hdc);
	ViewBox* v = ViewBox::getInstance();
	v->applyViewBox(graphics);
	applyTransform(graphics);
	//Set up pen color and draw
	int* Stroke = parseColor(getStroke());
	Pen			  pen(Color(stof(getStrokeOpacity()) * 255, Stroke[0], Stroke[1], Stroke[2]), stof(getStrokeWidth()));
	int* Fill = parseColor(getFill());
	SolidBrush    brush(Color(stof(getFillOpacity()) * 255, Fill[0], Fill[1], Fill[2]));
	vector<Point> Points = parsePoints(this->points);
	Point* points = Points.data();
	if (getFill() != "none" && getFill() != "") graphics.FillPolygon(&brush, points, Points.size());
	if (getStroke() != "none" && getStroke() != "") graphics.DrawPolygon(&pen, points, Points.size());
}