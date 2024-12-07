#include "rapidxml.hpp"
#include "stdafx.h"
#include "Text.h"
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;

TXT::TXT(string content, int x, int y, string fill, int textSize) : content(content), x(x), y(y), textSize(textSize) { setFill(fill); }

TXT::TXT()
{
	content = "";
	x = 0;
	y = 0;
	setFill("1");
	textSize = 0;
}

TXT::TXT(xml_node<>* node)
{
	string x1, y1, textSize1;
	xml_attribute<>* firstAttribute = node->first_attribute();
	content = node->value();
	//Default constructor
	x = 0, y = 0; textSize = 1;
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
		if (attributeName == "x") x1 = attributeValue;
		else if (attributeName == "y") y1 = attributeValue;
		else if (attributeName == "stroke") setStroke(attributeValue);
		else if (attributeName == "stroke-width") setStrokeWidth(attributeValue);
		else if (attributeName == "stroke-opacity") setStrokeOpacity(attributeValue);
		else if (attributeName == "fill") setFill(attributeValue);
		else if (attributeName == "fill-opacity") setFillOpacity(attributeValue);
		else if (attributeName == "font-size") textSize1 = attributeValue;
		else if (attributeName == "transform") setTransform(attributeValue);
		firstAttribute = firstAttribute->next_attribute();
	}
	x = atof(x1.c_str());
	y = atof(y1.c_str());
	textSize = atoi(textSize1.c_str());
}

TXT::~TXT()
{
}

VOID TXT::Draw(HDC hdc)
{
	Graphics graphics(hdc);
	ViewBox* v = ViewBox::getInstance();
	v->applyViewBox(graphics);
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
	//Set up color and content to write
	int* Fill = parseColor(getFill());
	int* Stroke = parseColor(getStroke());
	Pen	pen(Color(stof(getStrokeOpacity()) * 255, Stroke[0], Stroke[1], Stroke[2]), stof(getStrokeWidth()));
	FontFamily  fontFamily(L"Times New Roman");
	Font        font(&fontFamily, textSize, FontStyleRegular, UnitPixel);
	PointF      pointF(x, y);
	StringFormat gdisF = StringFormat::GenericDefault();
	gdisF.SetLineAlignment(StringAlignmentCenter);
	/*gdisF.SetAlignment(StringAlignmentNear);
	gdisF.SetLineAlignment(StringAlignmentNear);
	gdisF.SetFormatFlags(StringFormatFlagsNoWrap);
	gdisF.SetHotkeyPrefix(HotkeyPrefixNone);
	gdisF.SetTrimming(StringTrimmingNone);*/
	//LinearGradientBrush  solidBrush(Point(0, 0), Point(0, 100), Color(255, Stroke[0], Stroke[1], Stroke[2]),Color(255, Fill[0], Fill[1], Fill[2]));
	wstring wideText(content.begin(), content.end());
	//graphics.DrawString(wideText.c_str(), -1, &font, PointF(x, y - textSize), &LinearGradientBrush);
	GraphicsPath path;
	path.AddString(wideText.c_str(), -1, &fontFamily, FontStyleRegular, static_cast<REAL>(textSize), pointF, &gdisF);
	graphics.DrawPath(&pen, &path);
	SolidBrush fillBrush(Color(255, Fill[0], Fill[1], Fill[2]));
	graphics.FillPath(&fillBrush, &path);
}