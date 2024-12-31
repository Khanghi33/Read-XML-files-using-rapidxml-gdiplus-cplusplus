#pragma once
#include "stdafx.h"
#include "rapidxml.hpp"
#include "Shape.h"
#include <string>
#include <sstream>
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;
class ViewBox
{
private:
	REAL canvasWidth = 600.0;
	REAL canvasHeight = 400.0;
	REAL x = 0.0;
	REAL y = 0.0;
	REAL width = 600.0;
	REAL height = 400.0;
	bool check = false;
	static ViewBox* instance;
	ViewBox();
public:
	REAL getCanvasWidth() { return canvasWidth; }
	REAL getCanvasHeight() { return canvasHeight; }
	REAL getViewBoxWidth() { return width; }
	REAL getViewBoxHeight() { return height; }
	ViewBox(const ViewBox& other) = delete;
	ViewBox& operator=(const ViewBox& other) = delete;
	void applyViewBox(Graphics& g);
	static ViewBox* getInstance();
	void parse(xml_node<>* node);
	void parseViewBox(std::string value);
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







