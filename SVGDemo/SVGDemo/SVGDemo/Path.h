#pragma once
#include "Shape.h"
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;

class Path : public Shape
{
private:
	std::vector<std::string> data;
	Color* color = nullptr;
public:
	~Path();
	Path(xml_node<>* node);
	void parseFile(std::string filename);
	void parseElement(std::string d);
	void createGraphicsPath(GraphicsPath& graphicsPath);
	unsigned int hexToARGB(const std::string& hexColor);
	void setColor(std::string hexColor);
	std::vector<std::string> getData();
	Color getColor();
	VOID Draw(HDC hdc);
};

