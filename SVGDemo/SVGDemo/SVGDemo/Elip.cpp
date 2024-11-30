#include "rapidxml.hpp"
#include "stdafx.h"
#include "Elip.h"
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;

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
	//Set up transform
	string transform = getTransform(); string trash;
	string translate, rotate, scale;
	if (transform.find("translate") >= 0 && transform.find("translate") < transform.length()) {
		stringstream ss(transform.substr(transform.find("translate")));
		getline(ss, trash, ')');
		translate = trash.substr(trash.find('(') + 1);
		graphics.TranslateTransform(stoi(translate.substr(0, translate.find(','))), stoi(translate.substr(translate.find(',') + 1, translate.length() - translate.find(','))));
	}
	if (transform.find("rotate") >= 0 && transform.find("rotate") < transform.length()) {
		stringstream ss(transform.substr(transform.find("rotate")));
		getline(ss, trash, ')');
		rotate = trash.substr(trash.find('(') + 1);;
		graphics.RotateTransform(stoi(rotate));
	}
	if (transform.find("scale") >= 0 && transform.find("scale") < transform.length()) {
		stringstream ss(transform.substr(transform.find("scale")));
		getline(ss, trash, ')');
		scale = trash.substr(trash.find('(') + 1);
		if (scale.find(',') < 0 || scale.find(',') > scale.length()) graphics.ScaleTransform(stoi(scale), stoi(scale));
		else graphics.ScaleTransform(stoi(scale.substr(0, scale.find(','))), stoi(scale.substr(scale.find(',') + 1, scale.length() - scale.find(','))));
	}
	//Set pen color and draw
	int* Stroke = parseColor(getStroke());
	Pen	pen(Color(stof(getStrokeOpacity()) * 255, Stroke[0], Stroke[1], Stroke[2]), stof(getStrokeWidth()));
	int* Fill = parseColor(getFill());
	SolidBrush brush(Color(stof(getFillOpacity()) * 255, Fill[0], Fill[1], Fill[2]));
	graphics.DrawEllipse(&pen, x - rx, y - ry, rx * 2, ry * 2);
	graphics.FillEllipse(&brush, x - rx, y - ry, rx * 2, ry * 2);
}