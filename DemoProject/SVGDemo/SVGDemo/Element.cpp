#include "stdafx.h"
#include "Element.h"

using namespace std;
using namespace rapidxml;
using namespace Gdiplus;

int* parseColor(string stroke) {
	int* Color = new int[3];
	int start = stroke.find('(');
	int end = stroke.find(')');
	stringstream ss(stroke.substr(start + 1, end - start - 1));
	string tmp;
	getline(ss, tmp, ',');
	stringstream(tmp) >> Color[0];
	getline(ss, tmp, ',');
	stringstream(tmp) >> Color[1];
	getline(ss, tmp);
	stringstream(tmp) >> Color[2];
	return Color;
}

vector<Point> parsePoints(string points) {
	stringstream ss(points);
	string point;
	vector<Point> Points;
	while (getline(ss, point, ' ')) {
		int idx = point.find(',');
		Points.push_back(Point(stoi(point.substr(0, idx)),
			stoi(point.substr(idx + 1))));
	}
	return Points;
}
POLYGON::POLYGON()
{
	points = "";
	stroke = "";
	fill = "";
	fill_opacity = "1";
	stroke_width = "1";
	stroke_opacity = "1";
}
POLYGON::POLYGON(string Points, string Stroke, string Fill, string Stroke_width, string Fill_opacity, string Stroke_opacity)
{
	points = Points;
	stroke = Stroke;
	fill = Fill;
	fill_opacity = Fill_opacity;
	stroke_width = Stroke_width;
	stroke_opacity = Stroke_opacity;
}
POLYGON::POLYGON(xml_node<>* node)
{
	//Default Constructor
	points = "";
	stroke = "";
	fill = "";
	fill_opacity = "1";
	stroke_width = "1";
	stroke_opacity = "1";
	//Copy Constructor
	xml_attribute<>* firstAttribute = node->first_attribute();
	while (firstAttribute != NULL) {
		/*cout << firstAttribute->name() << " " << firstAttribute->value() << " ";
		firstAttribute = firstAttribute->next_attribute();*/
		string nameAttribute = firstAttribute->name();
		if (nameAttribute == "points") points = firstAttribute->value();
		else if (nameAttribute == "fill") fill = firstAttribute->value();
		else if (nameAttribute == "fill-opacity") fill_opacity = firstAttribute->value();
		else if (nameAttribute == "stroke") stroke = firstAttribute->value();
		else if (nameAttribute == "stroke-width") stroke_width = firstAttribute->value();
		else if (nameAttribute == "stroke-opacity") stroke_opacity = firstAttribute->value();
		firstAttribute = firstAttribute->next_attribute();
	}
}
POLYGON::~POLYGON()
{
}
VOID POLYGON::Draw(HDC hdc)
{
	Graphics graphics(hdc);
	int* Stroke = parseColor(stroke);
	/*cout << "Stroke: " << Stroke[0] << " " << Stroke[1] << " " << Stroke[2] << endl;
	cout << "Stroke_width: " << stroke_width << endl;
	cout << "Stroke_opacity: " << stroke_opacity << endl;*/
	Pen			  pen(Color(stof(stroke_opacity) * 255, Stroke[0], Stroke[1], Stroke[2]), stof(stroke_width));
	int* Fill = parseColor(fill);
	cout << "Fill: " << Fill[0] << " " << Fill[1] << " " << Fill[2] << endl;
	SolidBrush    brush(Color(stof(fill_opacity) * 255, Fill[0], Fill[1], Fill[2]));
	vector<Point> Points = parsePoints(points);
	Point* points = Points.data();
	graphics.FillPolygon(&brush, points, Points.size());
	graphics.DrawPolygon(&pen, points, Points.size());
}

POLYLINE::POLYLINE(string Points, string Stroke, string Fill, string Stroke_width, string Fill_opacity, string Stroke_opacity)
{
	points = Points;
	stroke = Stroke;
	fill = Fill;
	fill_opacity = Fill_opacity;
	stroke_width = Stroke_width;
	stroke_opacity = Stroke_opacity;
}

POLYLINE::POLYLINE()
{
	points = "";
	stroke = "";
	fill = "";
	fill_opacity = "1";
	stroke_width = "1";
	stroke_opacity = "1";
}

POLYLINE::POLYLINE(xml_node<>* node)
{
	//Default Constructor
	points = "";
	stroke = "";
	fill = "";
	fill_opacity = "1";
	stroke_width = "1";
	stroke_opacity = "1";
	//Copy Constructor
	xml_attribute<>* firstAttribute = node->first_attribute();
	while (firstAttribute != NULL) {
		string nameAttribute = firstAttribute->name();
		if (nameAttribute == "points") points = firstAttribute->value();
		else if (nameAttribute == "fill") fill = firstAttribute->value();
		else if (nameAttribute == "fill-opacity") fill_opacity = firstAttribute->value();
		else if (nameAttribute == "stroke") stroke = firstAttribute->value();
		else if (nameAttribute == "stroke-opacity") stroke_opacity = firstAttribute->value();
		else if (nameAttribute == "stroke-width") stroke_width = firstAttribute->value();
		firstAttribute = firstAttribute->next_attribute();
	}
}

POLYLINE::~POLYLINE()
{
}

VOID POLYLINE::Draw(HDC hdc)
{
	Graphics graphics(hdc);
	int* Stroke = parseColor(stroke);
	Pen	pen(Color(stof(stroke_opacity) * 255, Stroke[0], Stroke[1], Stroke[2]), stof(stroke_width));
	int* Fill = parseColor(fill);
	cout << "Fill: " << Fill[0] << " " << Fill[1] << " " << Fill[2] << endl;
	SolidBrush brush(Color(stof(fill_opacity) * 255, Fill[0], Fill[1], Fill[2]));
	vector<Point> Points = parsePoints(points);
	Point* points = Points.data();
	graphics.DrawLines(&pen, points, Points.size());
	graphics.FillPolygon(&brush, points, Points.size());
}

TXT::TXT(string content, int x, int y, string fill, int textSize) : content(content), x(x), y(y), fill(fill), textSize(textSize) {}

TXT::TXT()
{
	content = "";
	x = 0;
	y = 0;
	fill = "1";
	textSize = 0;
}

TXT::TXT(xml_node<>* node)
{
	string x1, y1, textSize1;
	xml_attribute<>* firstAttribute = node->first_attribute();
	content = node->value();
	//Default constructor
	x = 0, y = 0, fill = "1", textSize = 1;
	//Copy constructor
	while (firstAttribute != NULL)
	{
		string attributeName(firstAttribute->name());
		string attributeValue(firstAttribute->value());
		if (attributeName == "x") x1 = attributeValue;
		else if (attributeName == "y") y1 = attributeValue;
		else if (attributeName == "fill") fill = attributeValue;
		else if (attributeName == "font-size") textSize1 = attributeValue;
		firstAttribute = firstAttribute->next_attribute();
	}
	x = atoi(x1.c_str());
	y = atoi(y1.c_str());
	textSize = atoi(textSize1.c_str());
}

TXT::~TXT()
{
}

VOID TXT::Draw(HDC hdc)
{
	Graphics graphics(hdc);
	int* Fill = parseColor(fill);
	FontFamily  fontFamily(L"Times New Roman");
	Font        font(&fontFamily, textSize, FontStyleRegular, UnitPixel);
	PointF      pointF(x, y);
	SolidBrush  solidBrush(Color(255, Fill[0], Fill[1], Fill[2]));
	wstring wideText(content.begin(), content.end());
	graphics.DrawString(wideText.c_str(), -1, &font, PointF(x, y - textSize), &solidBrush);
	Pen      pen(Color(200, 0, 200));
}

Circle::Circle() : cx(0), cy(0), r(0), stroke(""), fill(""), stroke_width(""), fill_opacity(""), stroke_opacity("") {}

Circle::Circle(int cx, int cy, int r, string stroke, string fill, string stroke_width, string fill_opactiy, string stroke_opacity) {
	this->cx = cx;
	this->cy = cy;
	this->r = r;
	this->stroke = stroke;
	this->fill = fill;
	this->stroke_width = stroke_width;
	this->fill_opacity = fill_opacity;
	this->stroke_opacity = stroke_opacity;
}

Circle::Circle(xml_node<>* node) {
	string x1, y1, r1;
	xml_attribute<>* firstAttribute = node->first_attribute();
	//Default constructor
	cx = 0, cy = 0, r = 0, stroke = "", fill = "", stroke_width = "1", fill_opacity = "1", stroke_opacity = "1";
	//Copy constructor
	while (firstAttribute != NULL)
	{
		string attributeName(firstAttribute->name());
		string attributeValue(firstAttribute->value());
		if (attributeName == "cx") x1 = attributeValue;
		else if (attributeName == "cy") y1 = attributeValue;
		else if (attributeName == "r") r1 = attributeValue;
		else if (attributeName == "fill") fill = attributeValue;
		else if (attributeName == "stroke") stroke = attributeValue;
		else if (attributeName == "stroke-width") stroke_width = attributeValue;
		else if (attributeName == "stroke-opacity") stroke_opacity = attributeValue;
		else if (attributeName == "fill-opacity") fill_opacity = attributeValue;
		firstAttribute = firstAttribute->next_attribute();
	}
	cx = atoi(x1.c_str());
	cy = atoi(y1.c_str());
	r = stoi(r1);
}

Circle::~Circle() {}

VOID Circle::Draw(HDC hdc)
{
	Graphics graphics(hdc);
	int* Stroke = parseColor(stroke);
	Pen	pen(Color(stof(stroke_opacity) * 255, Stroke[0], Stroke[1], Stroke[2]), stof(stroke_width));
	int* Fill = parseColor(fill);
	SolidBrush brush(Color(stof(fill_opacity) * 255, Fill[0], Fill[1], Fill[2]));
	int d = r * 2;
	graphics.DrawEllipse(&pen, cx - r, cy - r, d, d);
	graphics.FillEllipse(&brush, cx - r, cy - r, d, d);
}

LINE::LINE(xml_node<>* node)
{
	xml_attribute<>* firstAttribute = node->first_attribute();
	//Default constructor
	x1 = y1 = x2 = y2 = 0, stroke = "", stroke_width = "1", stroke_opacity = "1";
	//Copy constructor
	while (firstAttribute != NULL) {
		string attributeName = firstAttribute->name();
		string attributeValue = firstAttribute->value();
		if (attributeName == "x1") x1 = stoi(attributeValue);
		else if (attributeName == "y1") y1 = stoi(attributeValue);
		else if (attributeName == "x2") x2 = stoi(attributeValue);
		else if (attributeName == "y2") y2 = stoi(attributeValue);
		else if (attributeName == "stroke") stroke = attributeValue;
		else if (attributeName == "stroke-width") stroke_width = attributeValue;
		else if (attributeName == "stroke-opacity") stroke_opacity = attributeValue;
		firstAttribute = firstAttribute->next_attribute();
	}
}

LINE::~LINE()
{
}

VOID LINE::Draw(HDC hdc)
{
	Graphics graphics(hdc);
	int* Stroke = parseColor(stroke);
	Pen	pen(Color(stof(stroke_opacity) * 255, Stroke[0], Stroke[1], Stroke[2]), stof(stroke_width));
	graphics.DrawLine(&pen, x1, y1, x2, y2);
}

RECTANGLES::RECTANGLES(xml_node<>* node)
{
	xml_attribute<>* firstAttribute = node->first_attribute();
	//Default constructor
	x = y = height = width = 0, stroke = "", fill = "",
		stroke_width = "1", fill_opacity = "1", stroke_opacity = "1";
	while (firstAttribute != NULL) {
		string attributeName = firstAttribute->name();
		string attributeValue = firstAttribute->value();
		if (attributeName == "x") x = stoi(attributeValue);
		else if (attributeName == "y") y = stoi(attributeValue);
		else if (attributeName == "height") height = stoi(attributeValue);
		else if (attributeName == "width") width = stoi(attributeValue);
		else if (attributeName == "stroke") stroke = attributeValue;
		else if (attributeName == "stroke-width") stroke_width = attributeValue;
		else if (attributeName == "stroke-opacity") stroke_opacity = attributeValue;
		else if (attributeName == "fill") fill = attributeValue;
		else if (attributeName == "fill-opacity") fill_opacity = attributeValue;
		firstAttribute = firstAttribute->next_attribute();
	}
}

RECTANGLES::~RECTANGLES()
{
}

VOID RECTANGLES::Draw(HDC hdc)
{
	Graphics graphics(hdc);
	int* Stroke = parseColor(stroke);
	Pen	pen(Color(stof(stroke_opacity) * 255, Stroke[0], Stroke[1], Stroke[2]), stof(stroke_width));
	int* Fill = parseColor(fill);
	SolidBrush brush(Color(stof(fill_opacity) * 255, Fill[0], Fill[1], Fill[2]));
	graphics.DrawRectangle(&pen, x, y, width, height);
	graphics.FillRectangle(&brush, x, y, width, height);
}

ELIP::ELIP(xml_node<>* node)
{
	xml_attribute<>* firstAttribute = node->first_attribute();
	//Default constructor
	x = y = rx = ry = 0, stroke = "", fill = "",
		stroke_width = "1", fill_opacity = "1", stroke_opacity = "1";
	//Copy constructor
	while (firstAttribute != NULL) {
		string attributeName = firstAttribute->name();
		string attributeValue = firstAttribute->value();
		if (attributeName == "cx") x = stoi(attributeValue);
		else if (attributeName == "cy") y = stoi(attributeValue);
		else if (attributeName == "rx") rx = stoi(attributeValue);
		else if (attributeName == "ry") ry = stoi(attributeValue);
		else if (attributeName == "stroke") stroke = attributeValue;
		else if (attributeName == "stroke-width") stroke_width = attributeValue;
		else if (attributeName == "stroke-opacity") stroke_opacity = attributeValue;
		else if (attributeName == "fill") fill = attributeValue;
		else if (attributeName == "fill-opacity") fill_opacity = attributeValue;
		firstAttribute = firstAttribute->next_attribute();
	}
}

ELIP::~ELIP()
{
}

VOID ELIP::Draw(HDC hdc)
{
	Graphics graphics(hdc);
	int* Stroke = parseColor(stroke);
	Pen	pen(Color(stof(stroke_opacity) * 255, Stroke[0], Stroke[1], Stroke[2]), stof(stroke_width));
	int* Fill = parseColor(fill);
	SolidBrush brush(Color(stof(fill_opacity) * 255, Fill[0], Fill[1], Fill[2]));
	graphics.DrawEllipse(&pen, x - rx, y - ry, rx * 2, ry * 2);
	graphics.FillEllipse(&brush, x - rx, y - ry, rx * 2, ry * 2);
}
