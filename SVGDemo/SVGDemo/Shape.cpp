#include "rapidxml.hpp"
#include "stdafx.h"
#include "Shape.h"

using namespace std;
using namespace rapidxml;
using namespace Gdiplus;
//Function parse string rbg(a, b, c) to usable data
int* parseColor(string stroke) {

    int* Color = new int[3] { 255 };
    if (stroke == "none") return Color;
    regex rgbRegex(R"(rgb\(\s*\d+\s*,\s*\d+\s*,\s*\d+\s*\))");

    // Regex for Hexadecimal color
    regex hexRegex(R"(#([A-Fa-f0-9]{3}|[A-Fa-f0-9]{6}))");

    // Map for CSS Color Names (small example; expand as needed)
    std::map<std::string, std::string> cssColors = {
        {"aliceblue", "#F0F8FF"},
        {"antiquewhite", "#FAEBD7"},
        {"aqua", "#00FFFF"},
        {"aquamarine", "#7FFFD4"},
        {"azure", "#F0FFFF"},
        {"beige", "#F5F5DC"},
        {"bisque", "#FFE4C4"},
        {"black", "#000000"},
        {"blanchedalmond", "#FFEBCD"},
        {"blue", "#0000FF"},
        {"blueviolet", "#8A2BE2"},
        {"brown", "#A52A2A"},
        {"burlywood", "#DEB887"},
        {"cadetblue", "#5F9EA0"},
        {"chartreuse", "#7FFF00"},
        {"chocolate", "#D2691E"},
        {"coral", "#FF7F50"},
        {"cornflowerblue", "#6495ED"},
        {"cornsilk", "#FFF8DC"},
        {"crimson", "#DC143C"},
        {"cyan", "#00FFFF"},
        {"darkblue", "#00008B"},
        {"darkcyan", "#008B8B"},
        {"darkgoldenrod", "#B8860B"},
        {"darkgray", "#A9A9A9"},
        {"darkgreen", "#006400"},
        {"darkkhaki", "#BDB76B"},
        {"darkmagenta", "#8B008B"},
        {"darkolivegreen", "#556B2F"},
        {"darkorange", "#FF8C00"},
        {"darkorchid", "#9932CC"},
        {"darkred", "#8B0000"},
        {"darksalmon", "#E9967A"},
        {"darkseagreen", "#8FBC8F"},
        {"darkslateblue", "#483D8B"},
        {"darkslategray", "#2F4F4F"},
        {"darkturquoise", "#00CED1"},
        {"darkviolet", "#9400D3"},
        {"deeppink", "#FF1493"},
        {"deepskyblue", "#00BFFF"},
        {"dimgray", "#696969"},
        {"dodgerblue", "#1E90FF"},
        {"firebrick", "#B22222"},
        {"floralwhite", "#FFFAF0"},
        {"forestgreen", "#228B22"},
        {"fuchsia", "#FF00FF"},
        {"gainsboro", "#DCDCDC"},
        {"ghostwhite", "#F8F8FF"},
        {"gold", "#FFD700"},
        {"goldenrod", "#DAA520"},
        {"gray", "#808080"},
        {"green", "#008000"},
        {"greenyellow", "#ADFF2F"},
        {"honeydew", "#F0FFF0"},
        {"hotpink", "#FF69B4"},
        {"indianred", "#CD5C5C"},
        {"indigo", "#4B0082"},
        {"ivory", "#FFFFF0"},
        {"khaki", "#F0E68C"},
        {"lavender", "#E6E6FA"},
        {"lavenderblush", "#FFF0F5"},
        {"lawngreen", "#7CFC00"},
        {"lemonchiffon", "#FFFACD"},
        {"lightblue", "#ADD8E6"},
        {"lightcoral", "#F08080"},
        {"lightcyan", "#E0FFFF"},
        {"lightgoldenrodyellow", "#FAFAD2"},
        {"lightgray", "#D3D3D3"},
        {"lightgreen", "#90EE90"},
        {"lightpink", "#FFB6C1"},
        {"lightsalmon", "#FFA07A"},
        {"lightseagreen", "#20B2AA"},
        {"lightskyblue", "#87CEFA"},
        {"lightslategray", "#778899"},
        {"lightsteelblue", "#B0C4DE"},
        {"lightyellow", "#FFFFE0"},
        {"lime", "#00FF00"},
        {"limegreen", "#32CD32"},
        {"linen", "#FAF0E6"},
        {"magenta", "#FF00FF"},
        {"maroon", "#800000"},
        {"mediumaquamarine", "#66CDAA"},
        {"mediumblue", "#0000CD"},
        {"mediumorchid", "#BA55D3"},
        {"mediumpurple", "#9370DB"},
        {"mediumseagreen", "#3CB371"},
        {"mediumslateblue", "#7B68EE"},
        {"mediumspringgreen", "#00FA9A"},
        {"mediumturquoise", "#48D1CC"},
        {"mediumvioletred", "#C71585"},
        {"midnightblue", "#191970"},
        {"mintcream", "#F5FFFA"},
        {"mistyrose", "#FFE4E1"},
        {"moccasin", "#FFE4B5"},
        {"navajowhite", "#FFDEAD"},
        {"navy", "#000080"},
        {"oldlace", "#FDF5E6"},
        {"olive", "#808000"},
        {"olivedrab", "#6B8E23"},
        {"orange", "#FFA500"},
        {"orangered", "#FF4500"},
        {"orchid", "#DA70D6"},
        {"palegoldenrod", "#EEE8AA"},
        {"palegreen", "#98FB98"},
        {"paleturquoise", "#AFEEEE"},
        {"palevioletred", "#DB7093"},
        {"papayawhip", "#FFEFD5"},
        {"peachpuff", "#FFDAB9"},
        {"peru", "#CD853F"},
        {"pink", "#FFC0CB"},
        {"plum", "#DDA0DD"},
        {"powderblue", "#B0E0E6"},
        {"purple", "#800080"},
        {"rebeccapurple", "#663399"},
        {"red", "#FF0000"},
        {"rosybrown", "#BC8F8F"},
        {"royalblue", "#4169E1"},
        {"saddlebrown", "#8B4513"},
        {"salmon", "#FA8072"},
        {"sandybrown", "#F4A460"},
        {"seagreen", "#2E8B57"},
        {"seashell", "#FFF5EE"},
        {"sienna", "#A0522D" },
        {"skyblue", "#87CEEB"},
        {"yellow", "FFFF00" } };
    /*if (stroke == "") {
        Color[0] = Color[1] = Color[2] = 0;
        return Color;
    }*/
    Color[0] = Color[1] = Color[2] = 0;

    if (regex_search(stroke, rgbRegex)) {
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
        for (int i = 0; i < 3; i++)
            if (Color[i] >= 255) Color[i] = 255;
        return Color;
    }
    else if (regex_search(stroke, hexRegex)) {
        std::string fillColor;
        // Regular expressions to match clip-path and fill color
        std::regex fillColorRegex(R"((#[A-Fa-f0-9]{6}))");
        std::smatch match;
        // Extract fill color
        if (std::regex_search(stroke, match, fillColorRegex)) {
            fillColor = match[1]; // First capturing group
        }
        if (fillColor[0] != '#' || fillColor.size() != 7) {
            throw std::invalid_argument("Invalid color format. Expected format: #RRGGBB");
        }

        // Parse the RGB components
        unsigned int r, g, b;
        std::istringstream(fillColor.substr(1, 2)) >> std::hex >> r;
        std::istringstream(fillColor.substr(3, 2)) >> std::hex >> g;
        std::istringstream(fillColor.substr(5, 2)) >> std::hex >> b;
        Color[0] = r;
        Color[1] = g;
        Color[2] = b;
        return Color;
    }
    else if (cssColors.find(stroke) != cssColors.end()) {
        std::string fillColor = cssColors[stroke];
        unsigned int r, g, b;
        std::istringstream(fillColor.substr(1, 2)) >> std::hex >> r;
        std::istringstream(fillColor.substr(3, 2)) >> std::hex >> g;
        std::istringstream(fillColor.substr(5, 2)) >> std::hex >> b;
        Color[0] = r;
        Color[1] = g;
        Color[2] = b;
        return Color;
    }
    return Color;
}
//Function to parse string points: "x1,y1 x2,y2 x3,y3 ..." usable data

vector<Point> parsePoints(string points) {
    ofstream fout("log.txt", ios::app);
    vector<Point> Points;
    // Regular expression to match coordinate pairs
    std::regex pointRegex(R"(\s*([-?\d.]+)\s*[,\s]?\s*([-?\d.]+)\s*)");
    std::smatch matches;

    
    if (regex_search(points, matches, pointRegex)) {
        std::sregex_iterator it(points.begin(), points.end(), pointRegex);
        std::sregex_iterator end;

        // Parse each coordinate pair
        while (it != end) {
            float x = std::stof((*it)[1]);
            float y = std::stof((*it)[2]);
            Points.emplace_back(x, y);
            ++it;
            //fout << x << " " << y << "\n";
        }
    }
    //if (points.find(',')) {
    //    std::sregex_iterator it(points.begin(), points.end(), pointRegexCommand);
    //    std::sregex_iterator end;

    //    // Parse each coordinate pair
    //    while (it != end) {
    //        float x = std::stof((*it)[1]);
    //        float y = std::stof((*it)[2]);
    //        Points.emplace_back(x, y);
    //        ++it;
    //        fout << x << " " << y << "\n";
    //    }
    //}
    //else {
    //    auto coordinatesBegin = std::sregex_iterator(points.begin(), points.end(), pointRegexNoCommand);
    //    auto coordinatesEnd = std::sregex_iterator();

    //    for (auto it = coordinatesBegin; it != coordinatesEnd; ++it) {
    //        float x = std::stof((*it)[1]);
    //        float y = std::stof((*it)[2]);
    //        Points.emplace_back(x, y);
    //        fout << x << " " << y << "\n";
    //    }
    //}
    return Points;
}



//vector<Point> parsePoints(string points) {
//    ofstream fout("log.txt", ios::app);
//	stringstream ss(points);
//	string point;
//	vector<Point> Points;
//	while (getline(ss, point, ' ')) {
//		int idx = point.find(',');
//		Points.push_back(Point(static_cast<REAL>(stof(point.substr(0, idx))),
//			static_cast<REAL>(stof(point.substr(idx + 1)))));
//        fout << static_cast<REAL>(stof(point.substr(0, idx))) << " " << static_cast<REAL>(stof(point.substr(idx + 1))) << "\n";
//    }
//	return Points;
//}
