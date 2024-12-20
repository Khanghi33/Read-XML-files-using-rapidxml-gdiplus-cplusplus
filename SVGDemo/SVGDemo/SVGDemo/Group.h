#pragma once
#include "Shape.h"
#include "SVGParser.h"

class GROUP : public Shape {
private:
	vector<Shape*> list;
public:
	GROUP();
	GROUP(xml_node<>* node);
	~GROUP();
	VOID Draw(HDC hdc);
};

string combineTransforms(const string& transform1, const string& transform2);
string fullyTransform(const string& transform1, const  string& transform2);