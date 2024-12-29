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

	ViewBox* v = ViewBox::getInstance();


	PointF point1(v->getViewBoxWidth(), v->getViewBoxHeight());
	PointF point2(v->getViewBoxWidth(), v->getViewBoxHeight());
	string id;
	bool gradientSpace = false;
	vector<Color> gradientStopColors;
	vector<REAL> gradientStopOffsets;
	string transform = "";
	while (firstAttribute != NULL) {
		string attributeName(firstAttribute->name());
		string attributeValue(firstAttribute->value());
		if (attributeName == "id") id = attributeValue;

		if (attributeName == "x1") 
		{
			if (attributeValue.find('%') != std::string::npos) {
				point1.X = stof(attributeValue) * point1.X / 100.0;
			}
			else point1.X = stof(attributeValue);
		}

		if (attributeName == "y1") 
		{
			if (attributeValue.find('%') != std::string::npos) {
				point1.Y = stof(attributeValue) * point1.Y / 100.0;
			}
			else point1.Y = stof(attributeValue);
		}

		if (attributeName == "x2")
		{
			if (attributeValue.find('%') != std::string::npos) {
				point2.X = stof(attributeValue) * point2.X / 100.0;
			}
			else point2.X = stof(attributeValue);
		}

		if (attributeName == "y2")
		{
			if (attributeValue.find('%') != std::string::npos) {
				point2.Y = stof(attributeValue) * point2.Y / 100.0;
			}
			else point2.Y = stof(attributeValue);
		}
		if (attributeName == "gradientTransform") {

		}
		if (attributeName == "gradientUnits") {
			if (attributeValue == "userSpaceOnUse") gradientSpace = true;
		}
		firstAttribute = firstAttribute->next_attribute();
	}
	if (gradientSpace == true) {
		userSpaceOnUse[id] = true;
	}
	else userSpaceOnUse[id] = false;

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
			if (attributeValue.find('%') != std::string::npos) {
				gradientOffsets.push_back(stof(attributeValue) / 100.0);
			}
			else gradientOffsets.push_back(stof(attributeValue));
		}
		
		else if (attributeName == "stop-color") {
			int* fill = parseColor(attributeValue);
			gradientColors.push_back(Color(255, fill[0], fill[1], fill[2]));
		}
		else if (attributeName == "style") {
			std::regex regex(R"(([a-zA-Z\-]+):([^;]+);)");
			string style = attributeValue;
			auto begin = std::sregex_iterator(style.begin(), style.end(), regex);
			auto end = std::sregex_iterator();

			for (auto it = begin; it != end; ++it) {
				std::smatch match = *it;
				std::string key = match[1].str();
				std::string value = match[2].str();
				if (key == "stop-color") {
					int* fill = parseColor(value);
					gradientColors.push_back(Color(255, fill[0], fill[1], fill[2]));
				}
			}

		}
		firstAttribute = firstAttribute->next_attribute();
	}
}

LinearGradientBrush* LinearGradient::getBrush(string id, RectF* rect)
{
	auto search = colorMap.find(id);
	if (search != colorMap.end()) {
		if (!userSpaceOnUse[id]) { // Assuming userSpaceOnUse is a valid map or data structure
			LinearGradientBrush* temp = search->second;

			// Get interpolation colors and offsets
			int count = temp->GetInterpolationColorCount();
			if (count > 0) {
				std::vector<REAL> offsets(count);
				std::vector<Color> colors(count);
				temp->GetInterpolationColors(colors.data(), offsets.data(), count);

				// Calculate gradient start and end points based on the rectangle
				PointF startPoint(rect->X, rect->Y); // Top-left corner
				PointF endPoint(rect->X + rect->Width, rect->Y + rect->Height); // Bottom-right corner

				// Create a new LinearGradientBrush using the calculated points
				LinearGradientBrush* brush = new LinearGradientBrush(startPoint, endPoint, Color::Black, Color::White);
				brush->SetInterpolationColors(colors.data(), offsets.data(), count);

				return brush;
			}
		}

		// Return the existing brush if no new one is created
		return search->second;
	}

	return nullptr;
}

LinearGradient::~LinearGradient()
{
	for (auto& pair : colorMap) {
		delete pair.second; // Free memory for each brush
	}
	colorMap.clear();
}

void applyGradientTransform(LinearGradientBrush& brush, string transform)
{

}
