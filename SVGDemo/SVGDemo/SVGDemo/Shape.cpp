#include "rapidxml.hpp"
#include "stdafx.h"
#include "Shape.h"

using namespace std;
using namespace rapidxml;
using namespace Gdiplus;
//Function parse string rbg(a, b, c) to usable data
int* parseColor(string stroke) {
	int* Color = new int[3];
    if (stroke == "") {
        Color[0] = Color[1] = Color[2] = 0;
        return Color;
    }
	int start = stroke.find('(');
	int end = stroke.find(')');
	stringstream ss(stroke.substr(start + 1, end - start - 1));
	string tmp;
	getline(ss, tmp, ',');
	stringstream(tmp) >> Color[0];
	getline(ss, tmp, ',');
	stringstream(tmp) >> Color[1];
	getline(ss, tmp);
	stringstream(tmp) >> Color[2];
	return Color;
}
//Function to parse string points: "x1,y1 x2,y2 x3,y3 ..." usable data
vector<Point> parsePoints(string points) {
	stringstream ss(points);
	string point;
	vector<Point> Points;
	while (getline(ss, point, ' ')) {
		int idx = point.find(',');
		Points.push_back(Point(stoi(point.substr(0, idx)),
			stoi(point.substr(idx + 1))));
	}
	return Points;
}
