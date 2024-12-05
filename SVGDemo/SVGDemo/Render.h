#pragma once
#include "Shape.h"
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;

class Render {
private:
	vector<Shape*> shapes;
	HDC* hdc;
public:
	Render(vector<Shape*> shapes, HDC* hdc);
	void GDIRender();
};
