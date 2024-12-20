#pragma once
#include "Shape.h"
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;

class TXT : public Shape {
private:
	string content;
	REAL x, y, textSize;
public:
	TXT(string content, int x, int y, string fill, int textSize);
	TXT();
	TXT(xml_node<>* node);
	~TXT();

	VOID Draw(HDC hdc);
};