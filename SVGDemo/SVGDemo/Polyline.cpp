#include "rapidxml.hpp"
#include "stdafx.h"
#include "Polyline.h"
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;

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
	//Set up pen color and draw
	int* Stroke = parseColor(getStroke());
	Pen	pen(Color(stof(getStrokeOpacity()) * 255, Stroke[0], Stroke[1], Stroke[2]), stof(getStrokeWidth()));
	int* Fill = parseColor(getFill());
	SolidBrush brush(Color(stof(getFillOpacity()) * 255, Fill[0], Fill[1], Fill[2]));
	vector<Point> Points = parsePoints(this->points);
	Point* points = Points.data();
	if(getStroke() != "none" && getStroke() != "") graphics.DrawLines(&pen, Points.data(), Points.size());
	if(getFill() != "none" && getFill() != "") graphics.FillPolygon(&brush, Points.data(), Points.size());
}