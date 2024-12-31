#pragma once
#include "Shape.h"
#include "ViewBox.h"
#include "LinearGradient.h"
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;

// The CIRCLE class inherits from the Shape class and represents a circle with attributes such as center coordinates, radius, and styling properties.
// It provides constructors for initializing a circle with explicit parameters or parsing data from an XML node, and a method for rendering the circle.

class CIRCLE : public Shape {
private:
	// - cx, cy: Represent the x and y coordinates of the circle's center.
	// - r: Represents the radius of the circle.
	REAL cx, cy, r;
public:
	CIRCLE(); //Default constructor initializes the circle with default values.
	CIRCLE(int cx, int cy, int r, string stroke, string fill, string stroke_width, string fill_opactiy, string stroke_opacity); //   Initializes the circle with specific coordinates, radius, and styling attributes.
	CIRCLE(xml_node<>* node);// Parses circle attributes from an XML node, enabling deserialization.
	~CIRCLE(); // Cleans up resources if necessary (not explicitly detailed in the provided code but included for proper lifecycle management).
	VOID Draw(HDC hdc); //Draws the circle on the provided device context(HDC) using its attributes for position, size, and style.
};