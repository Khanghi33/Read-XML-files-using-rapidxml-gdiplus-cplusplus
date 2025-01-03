#include "rapidxml.hpp"
#include "stdafx.h"
#include "LinearGradient.h"
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;
//linear
// 
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

// Parses the given XML node representing a <linearGradient> element.
void LinearGradient::parse(xml_node<>* node)
{
	// Pointer to the first attribute of the <linearGradient> node.
	xml_attribute<>* firstAttribute = node->first_attribute();

	// Get the singleton instance of the ViewBox class to retrieve viewBox dimensions.
	ViewBox* v = ViewBox::getInstance();

	// Initialize default gradient endpoints as the full viewBox dimensions.
	PointF point1(v->getViewBoxWidth(), v->getViewBoxHeight());
	PointF point2(v->getViewBoxWidth(), v->getViewBoxHeight());

	string id;
	bool gradientSpace = false;
	vector<Color> gradientStopColors;
	vector<REAL> gradientStopOffsets;
	string transform = "";

	// Iterate over all attributes of the <linearGradient> node.
	while (firstAttribute != NULL) {
		string attributeName(firstAttribute->name());
		string attributeValue(firstAttribute->value());

		// Store the unique id of the gradient.
		if (attributeName == "id") id = attributeValue;

		// Parse the x1 coordinate of the gradient's start point.
		if (attributeName == "x1") 
		{
			if (attributeValue.find('%') != std::string::npos) {
				point1.X = stof(attributeValue) * point1.X / 100.0;		// Convert percentage to absolute value.
			}
			else point1.X = stof(attributeValue);		// Directly convert to float.
		}

		// Parse the y1 coordinate of the gradient's start point.
		if (attributeName == "y1") 
		{
			if (attributeValue.find('%') != std::string::npos) {
				point1.Y = stof(attributeValue) * point1.Y / 100.0;
			}
			else point1.Y = stof(attributeValue);
		}

		// Parse the x2 coordinate of the gradient's end point.
		if (attributeName == "x2")
		{
			if (attributeValue.find('%') != std::string::npos) {
				point2.X = stof(attributeValue) * point2.X / 100.0;
			}
			else point2.X = stof(attributeValue);
		}

		// Parse the y2 coordinate of the gradient's end point.
		if (attributeName == "y2")
		{
			if (attributeValue.find('%') != std::string::npos) {
				point2.Y = stof(attributeValue) * point2.Y / 100.0;
			}
			else point2.Y = stof(attributeValue);
		}
		// Parse the gradient transform attribute, if present.
		if (attributeName == "gradientTransform") {
			transform = attributeValue;
		}
		// Determine the gradient's coordinate system (userSpaceOnUse or objectBoundingBox).
		if (attributeName == "gradientUnits") {
			if (attributeValue == "userSpaceOnUse") gradientSpace = true;
		}
		firstAttribute = firstAttribute->next_attribute();
	}
	if (gradientSpace == true) {
		userSpaceOnUse[id] = true;
	}
	else userSpaceOnUse[id] = false;

	// Process all child <stop> nodes of the <linearGradient> node.
	xml_node<>* nodeChild = node->first_node();
	while (nodeChild != NULL) {
		string nodeName = nodeChild->name();
		if (nodeName == "stop") {
			parseStop(nodeChild, gradientStopColors, gradientStopOffsets);
		}
		nodeChild = nodeChild->next_sibling();
	}
	//LinearGradientBrush* brush = new LinearGradientBrush(point1, point2, Color::Black, Color::White);
	//brush->SetInterpolationColors(&gradientStopColors[0], &gradientStopOffsets[0], gradientStopColors.size());
	//applyGradientTransform(*brush, transform);
	//// Insert into the map
	//colorMap.insert({ id, brush });

	// Create a LinearGradientBrush if the gradient has valid stop colors and offsets.
	if (!gradientStopColors.empty() && !gradientStopOffsets.empty()) {
		LinearGradientBrush* brush = new LinearGradientBrush(
			point1, point2, gradientStopColors.front(), gradientStopColors.back()
		);
		brush->SetInterpolationColors(&gradientStopColors[0], &gradientStopOffsets[0], gradientStopColors.size());
		applyGradientTransform(*brush, transform);
		colorMap.insert({ id, brush });
	}
}

// Parses a <stop> node and extracts its color and offset values.
void LinearGradient::parseStop(xml_node<>* node, vector<Color>& gradientColors, vector<REAL>& gradientOffsets)
{
	// Pointer to the first attribute of the <stop> node.
	xml_attribute<>* firstAttribute = node->first_attribute();

	// Iterate over all attributes of the <stop> node.
	while (firstAttribute != NULL) {
		string attributeName(firstAttribute->name());
		string attributeValue(firstAttribute->value());

		// Parse the offset value of the stop.
		if (attributeName == "offset") {
			if (attributeValue.find('%') != std::string::npos) {
				gradientOffsets.push_back(stof(attributeValue) / 100.0);
			}
			else gradientOffsets.push_back(stof(attributeValue));
		}
		
		// Parse the stop color if specified directly.
		else if (attributeName == "stop-color") {
			int* fill = parseColor(attributeValue);
			gradientColors.push_back(Color(255, fill[0], fill[1], fill[2]));
		}
		// Parse the style attribute to extract stop-color information.
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
	//std::string transformString = transform;

	//// Combined regex for rotate, scale, translate, and matrix
	//std::regex transformRegex(R"((rotate|scale|translate|matrix)\((-?\d+(\.\d+)?)(?:,?\s*(-?\d+(\.\d+)?))?(?:,?\s*(-?\d+(\.\d+)?))?(?:,?\s*(-?\d+(\.\d+)?))?(?:,?\s*(-?\d+(\.\d+)?))?\))");
	//std::smatch match;

	//// Search for all matches in the string
	//std::string::const_iterator searchStart(transformString.cbegin());
	//while (std::regex_search(searchStart, transformString.cend(), match, transformRegex)) {
	//	std::string type = match[1];  // Capture the type: rotate, scale, translate, or matrix

	//	if (type == "rotate") {
	//		REAL value = std::stof(match[2]); // First parameter
	//		brush.RotateTransform(value);
	//	}
	//	else if (type == "scale") {
	//		REAL value1 = std::stof(match[2]);
	//		REAL value2 = match[4].matched ? std::stof(match[4]) : value1; // Second parameter (if exists)
	//		brush.ScaleTransform(value1, value2);
	//	}
	//	else if (type == "translate") {
	//		REAL value1 = std::stof(match[2]);
	//		REAL value2 = match[4].matched ? std::stof(match[4]) : 0.0f; // Second parameter (if exists)
	//		brush.TranslateTransform(value1, value2);
	//	}
	//	else if (type == "matrix") {
	//		// Matrix parameters: a, b, c, d, e, f
	//		REAL a = std::stof(match[2]);
	//		REAL b = std::stof(match[4]);
	//		REAL c = std::stof(match[6]);
	//		REAL d = std::stof(match[8]);
	//		REAL e = std::stof(match[10]);
	//		REAL f = std::stof(match[12]);

	//		// Create a GDI+ Matrix
	//		Matrix matrix(a, b, c, d, e, f);

	//		// Apply the matrix transform to the graphics object
	//		brush.MultiplyTransform(&matrix);
	//	}

	//	searchStart = match.suffix().first;
	//}
	std::string transformString = transform;

	// Combined regex for rotate, scale, translate, and matrix
	std::regex transformRegex(R"((rotate|scale|translate|matrix)\(([^)]+)\))");
	std::smatch match;

	// Search for all matches in the string
	std::string::const_iterator searchStart(transformString.cbegin());
	while (std::regex_search(searchStart, transformString.cend(), match, transformRegex)) {
		std::string type = match[1];  // Capture the type: rotate, scale, translate, or matrix

		std::cout << "Enter values for " << type << " (separated by space): ";
		std::string value = match[2];
		std::replace(value.begin(), value.end(), ',', ' ');
		istringstream in(value);
		if (type == "rotate") {
			REAL angle;
			in >> angle; // Read input for rotation angle
			brush.RotateTransform(angle);
		}
		else if (type == "scale") {
			REAL sx, sy;
			in >> sx; // Read input for scale values
			if (in >> sy) {
				brush.ScaleTransform(sx, sy);
			}
			else brush.ScaleTransform(sx, sx);
		}
		else if (type == "translate") {
			REAL tx, ty;
			in >> tx >> ty; // Read input for translation values
			brush.TranslateTransform(tx, ty);
		}
		else if (type == "matrix") {
			// Matrix parameters: a, b, c, d, e, f
			REAL a, b, c, d, e, f;
			in >> a >> b >> c >> d >> e >> f; // Read input for matrix values

			// Create a GDI+ Matrix
			Matrix matrix(a, b, c, d, e, f);

			// Apply the matrix transform to the graphics object
			brush.MultiplyTransform(&matrix);
		}

		searchStart = match.suffix().first;
	}
}
// matrix cread Gdi ++

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