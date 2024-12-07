#pragma once
#include "Shape.h"
#include "ViewBox.h"
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;

class TXT : public Shape {
private:
	string content;
	REAL x;
	REAL y;
	int textSize;
	string textAnchor = "";
public:
	TXT(string content, int x, int y, string fill, int textSize);
	TXT();
	TXT(xml_node<>* node);
	~TXT();
	void setTextAnchor(string textAnchor);
	VOID Draw(HDC hdc);
};