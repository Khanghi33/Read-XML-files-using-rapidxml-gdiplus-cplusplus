#include "rapidxml.hpp"
#include "stdafx.h"
#include "LinearGradient.h"
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;

// Initialize static member
LinearGradient* LinearGradient::instance = nullptr;

// Private constructor
LinearGradient::LinearGradient() {
	// Initialization logic if needed
}

// Get the singleton instance
LinearGradient* LinearGradient::getInstance() {
	if (!instance) {
		instance = new LinearGradient();
	}
	return instance;
}

void LinearGradient::parse(xml_node<>* node)
{
	xml_attribute<>* firstAttribute = node->first_attribute();
	PointF point1;
	PointF point2;
	string id;
	vector<Color> gradientStopColors;
	vector<REAL> gradientStopOffsets;

	while (firstAttribute != NULL) {
		string attributeName(firstAttribute->name());
		string attributeValue(firstAttribute->value());
		if (attributeName == "id") id = attributeValue;
		if (attributeName == "x1") point1.X = stof(attributeValue);
		if (attributeName == "y1") point1.Y = stof(attributeValue);
		if (attributeName == "x2") point2.X = stof(attributeValue);
		if (attributeName == "y2") point2.Y = stof(attributeValue);
		firstAttribute = firstAttribute->next_attribute();
	}
	xml_node<>* nodeChild = node->first_node();
	while (nodeChild != NULL) {
		string nodeName = nodeChild->name();
		if (nodeName == "stop") {
			parseStop(nodeChild, gradientStopColors, gradientStopOffsets);
		}
		nodeChild = nodeChild->next_sibling();
	}
	LinearGradientBrush* brush = new LinearGradientBrush(point1, point2, Color::Black, Color::White);
	brush->SetInterpolationColors(&gradientStopColors[0], &gradientStopOffsets[0], gradientStopColors.size());

	// Insert into the map
	colorMap.insert({ id, brush });
}

void LinearGradient::parseStop(xml_node<>* node, vector<Color>& gradientColors, vector<REAL>& gradientOffsets)
{
	xml_attribute<>* firstAttribute = node->first_attribute();
	while (firstAttribute != NULL) {
		string attributeName(firstAttribute->name());
		string attributeValue(firstAttribute->value());
		if (attributeName == "offset") {
			gradientOffsets.push_back(stof(attributeValue));
		}
		else if (attributeName == "stop-color") {
			int* fill = parseColor(attributeValue);
			gradientColors.push_back(Color(255, fill[0], fill[1], fill[2]));
		}
		firstAttribute = firstAttribute->next_attribute();
	}
}

LinearGradientBrush* LinearGradient::getBrush(string id)
{
	auto search = colorMap.find(id);
	if (search != colorMap.end()) {
		return search->second;
	}
	return NULL;
}

LinearGradient::~LinearGradient()
{
	for (auto& pair : colorMap) {
		delete pair.second; // Free memory for each brush
	}
	colorMap.clear();
}
