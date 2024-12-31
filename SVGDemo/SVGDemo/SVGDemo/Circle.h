#pragma once
#include "Shape.h"
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