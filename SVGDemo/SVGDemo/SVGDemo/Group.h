#pragma once
#include "Shape.h"
#include "SVGParser.h"

class GROUP : public Shape{
private: 
	vector<Shape*> list;
public:
	GROUP(xml_node<>* node);
	~GROUP();
	VOID Draw(HDC hdc) override;
};