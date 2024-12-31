#include "rapidxml.hpp"
#include "stdafx.h"
#include "Rectangle.h"
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;
//rectangle

RECTANGLES::RECTANGLES(xml_node<>* node)
{
	xml_attribute<>* firstAttribute = node->first_attribute();
	//Default constructor
	x = y = height = width = 0;
	setStroke("");
	setFill("");
	setFillOpacity("1");
	setStrokeWidth("1");
	setStrokeOpacity("1");
	while (firstAttribute != NULL) {
		string attributeName = firstAttribute->name();
		string attributeValue = firstAttribute->value();
		if (attributeName == "x") x = stof(attributeValue);
		else if (attributeName == "y") y = stof(attributeValue);
		else if (attributeName == "height") height = stof(attributeValue);
		else if (attributeName == "width") width = stof(attributeValue);
		else if (attributeName == "stroke") setStroke(attributeValue);
		else if (attributeName == "style") parseStyle(attributeValue);
		else if (attributeName == "stroke-width") setStrokeWidth(attributeValue);
		else if (attributeName == "stroke-opacity") setStrokeOpacity(attributeValue);
		else if (attributeName == "fill") setFill(attributeValue);
		else if (attributeName == "fill-opacity") setFillOpacity(attributeValue);
		else if (attributeName == "transform") setTransform(attributeValue);
		firstAttribute = firstAttribute->next_attribute();
	}
}

RECTANGLES::~RECTANGLES()
{
}

VOID RECTANGLES::Draw(HDC hdc)
{
	//use log.txt to determine whether if draw works 
	ofstream fout("log.txt", ios::app);
	Graphics graphics(hdc);
	ViewBox* v = ViewBox::getInstance();
	v->applyViewBox(graphics);
	applyTransform(graphics);
	//Set up pen color and draw
	int* Stroke = parseColor(getStroke());
	Pen	pen(Color(stof(getStrokeOpacity()) * 255, Stroke[0], Stroke[1], Stroke[2]), stof(getStrokeWidth()));
	int* Fill = parseColor(getFill());
	SolidBrush brush(Color(stof(getFillOpacity()) * 255, Fill[0], Fill[1], Fill[2]));
	if (getStroke() != "none" && getStroke() != "") graphics.DrawRectangle(&pen, x, y, width, height);
	if (getFill() != "none" && getFill() != "") graphics.FillRectangle(&brush, x, y, width, height);
}

//rectangle.cpp

/*
_________                        _______                      _  ._    .__                         _         _        ___               ___
_                          _      ___                     ___      _.             ___
_         _                 ___  _                 ____  _                 ____              .__           ___.__                 ___
\__    __/_.__.______   __    /   ____/ ___   ___   ____/  |_|  |_ |__| __    __   ___  |  | _    |__| _____/ ____\___    ____/ ____\  | ______    ____  _  |  | ___/ ____\___
_  _  _______/ ____\___ \_ |____  _  __/ ____\  | _    |__|____ _  _  ___/ ____\|__|___
_  _  ___/ ____\|__|___
_  _  ___/ ____\___    __ |__|_  _  __/ ____\__|___ __  _  ___/ ____\
  |    | <   |  |\____ \_/ _ \   \____  \ /  _ \ /     \_/ _ \   __\  |  \|  |/    \  / ___\  \_  \ |  |/ /    |  |/    \   _\\_  \  /    \   _\|  |/ /\_  \  /    \ \/ \/ /  |/ /\   _\\_  \\ \/ \/ /    \   _\\_  \ | _ \ \/ \/ /\   __\|  |/ /    |  \_  \\ \/ \/ /\   _\ |  \_  \\ \/ \/ /\   _\ |  \_  \\ \/ \/ /\   _\\_  \  /  _ \|  \ \/ \/ /\   _\|  \_  \\ \/ \/ /\   __\
  |    |  \___  ||  |_> >  __/   /        (  <_> )  Y Y  \  ___/|  | |   Y  \  |   |  \/ /_/  >  / _ \|    <     |  |   |  \  |   / _ \|   |  \  |  |    <  / _ \|   |  \     /|    <  |  |   / _ \\     /   |  \  |   / _ \| \_\ \     /  |  |  |    <     |  |/ _ \\     /  |  |   |  |/ _ \\     /  |  |   |  |/ _ \\     /  |  |   / _ \(  <_> )  |\     /  |  |  |  |/ __ \\     /  |  |
  |____|  / ___||   __/ \__  > /_______  /\____/|__|_|  /\___  >__| |___|  /__|___|  /\___  /  (__  /__|_ \/\__|  |___|  /__|  (__  /___|  /__|  |__|_ \(__  /___|  /\/\_/ |__|_ \ |__|  (__  /\/\_/|___|  /__|  (__  /___  /\/\_/   |__|  |__|_ \/\__|  (__  /\/\_/   |__/\__|  (__  /\/\_/   |__/\__|  (__  /\/\_/   |__|  (__  /\____/|__| \/\_/   |__|  |__(__  /\/\_/   |__|
		  \/     |__|        \/          \/             \/     \/          \/        \//_____/        \/     \/\______|    \/           \/     \/           \/     \/     \/            \/            \/           \/           \/    \/                    \/\______|    \/            \______|    \/            \______|    \/                    \/                                  \/
*/
