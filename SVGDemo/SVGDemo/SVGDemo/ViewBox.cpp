#include "rapidxml.hpp"
#include "stdafx.h"
#include "ViewBox.h"
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;

// Define the static member
ViewBox* ViewBox::instance = nullptr;

ViewBox::ViewBox()
{

}

void ViewBox::applyViewBox(Graphics& g)
{
	/*if (check) {
		REAL scaleX = canvasWidth / width;
		REAL scaleY = canvasHeight / height;

		REAL scale = (scaleX > scaleY) ? scaleX : scaleY;

		g.ScaleTransform(scale, scale);
		g.TranslateTransform(-x, -y);
	}*/
	REAL scaleX = canvasWidth / width;
	REAL scaleY = canvasHeight / height;

	REAL scale = (scaleX > scaleY) ? scaleX : scaleY;

	g.ScaleTransform(scale, scale);
	//g.TranslateTransform(-x, -y);
}

ViewBox* ViewBox::getInstance()
{
	if (instance == nullptr) instance = new ViewBox();
	return instance;
}

void ViewBox::parse(xml_node<>* node)
{
	xml_attribute<>* firstAttribute = node->first_attribute();
	
	//Copy constructor
	while (firstAttribute != NULL)
	{
		string attributeName(firstAttribute->name());
		string attributeValue(firstAttribute->value());

		if (attributeName == "width") width = stof(attributeValue);
		if (attributeName == "height") height = stof(attributeValue);
		if (attributeName == "viewBox") parseViewBox(attributeValue);

		firstAttribute = firstAttribute->next_attribute();
	}
}

void ViewBox::parseViewBox(std::string value)
{
	istringstream stream(value);
	stream >> x >> y >> width >> height;
	check = true;
}
