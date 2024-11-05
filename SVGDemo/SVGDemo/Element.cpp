#include "stdafx.h"
#include "Element.h"

using namespace std;
using namespace rapidxml;
using namespace Gdiplus;
//Function parse string rbg(a, b, c) to usable data
int* parseColor(string stroke) {
	int* Color = new int[3];
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
//Default constructor for POLYGON class
POLYGON::POLYGON()
{
	points = "";
	stroke = "";
	fill = "";
	fill_opacity = "1";
	stroke_width = "1";
	stroke_opacity = "1";
}
//Copy constructor for POLYGON 
POLYGON::POLYGON(string Points, string Stroke, string Fill, string Stroke_width, string Fill_opacity, string Stroke_opacity)
{
	points = Points;
	stroke = Stroke;
	fill = Fill;
	fill_opacity = Fill_opacity;
	stroke_width = Stroke_width;
	stroke_opacity = Stroke_opacity;
}
//Copy constructor for POLYGON to read data from xml node
POLYGON::POLYGON(xml_node<>* node)
{
	//Default Constructor
	points = "";
	stroke = "";
	fill = "";
	fill_opacity = "1";
	stroke_width = "1";
	stroke_opacity = "1";
	//Copy Constructor
	xml_attribute<>* firstAttribute = node->first_attribute();
	while (firstAttribute != NULL) {
		string nameAttribute = firstAttribute->name();
		if (nameAttribute == "points") points = firstAttribute->value();
		else if (nameAttribute == "fill") fill = firstAttribute->value();
		else if (nameAttribute == "fill-opacity") fill_opacity = firstAttribute->value();
		else if (nameAttribute == "stroke") stroke = firstAttribute->value();
		else if (nameAttribute == "stroke-width") stroke_width = firstAttribute->value();
		else if (nameAttribute == "stroke-opacity") stroke_opacity = firstAttribute->value();
		firstAttribute = firstAttribute->next_attribute();
	}
}
//Destructor
POLYGON::~POLYGON()
{
}
VOID POLYGON::Draw(HDC hdc)
{
	Graphics graphics(hdc);
	int* Stroke = parseColor(stroke);
	Pen			  pen(Color(stof(stroke_opacity) * 255, Stroke[0], Stroke[1], Stroke[2]), stof(stroke_width));
	int* Fill = parseColor(fill);
	cout << "Fill: " << Fill[0] << " " << Fill[1] << " " << Fill[2] << endl;
	SolidBrush    brush(Color(stof(fill_opacity) * 255, Fill[0], Fill[1], Fill[2]));
	vector<Point> Points = parsePoints(points);
	Point* points = Points.data();
	graphics.FillPolygon(&brush, points, Points.size());
	graphics.DrawPolygon(&pen, points, Points.size());
}

POLYLINE::POLYLINE(string Points, string Stroke, string Fill, string Stroke_width, string Fill_opacity, string Stroke_opacity)
{
	points = Points;
	stroke = Stroke;
	fill = Fill;
	fill_opacity = Fill_opacity;
	stroke_width = Stroke_width;
	stroke_opacity = Stroke_opacity;
}

POLYLINE::POLYLINE()
{
	points = "";
	stroke = "";
	fill = "";
	fill_opacity = "1";
	stroke_width = "1";
	stroke_opacity = "1";
}

POLYLINE::POLYLINE(xml_node<>* node)
{
	//Default Constructor
	points = "";
	stroke = "";
	fill = "";
	fill_opacity = "1";
	stroke_width = "1";
	stroke_opacity = "1";
	//Copy Constructor
	xml_attribute<>* firstAttribute = node->first_attribute();
	while (firstAttribute != NULL) {
		string nameAttribute = firstAttribute->name();
		if (nameAttribute == "points") points = firstAttribute->value();
		else if (nameAttribute == "fill") fill = firstAttribute->value();
		else if (nameAttribute == "fill-opacity") fill_opacity = firstAttribute->value();
		else if (nameAttribute == "stroke") stroke = firstAttribute->value();
		else if (nameAttribute == "stroke-opacity") stroke_opacity = firstAttribute->value();
		else if (nameAttribute == "stroke-width") stroke_width = firstAttribute->value();
		firstAttribute = firstAttribute->next_attribute();
	}
}

POLYLINE::~POLYLINE()
{
}

VOID POLYLINE::Draw(HDC hdc)
{
	Graphics graphics(hdc);
	int* Stroke = parseColor(stroke);
	Pen	pen(Color(stof(stroke_opacity) * 255, Stroke[0], Stroke[1], Stroke[2]), stof(stroke_width));
	int* Fill = parseColor(fill);
	cout << "Fill: " << Fill[0] << " " << Fill[1] << " " << Fill[2] << endl;
	SolidBrush brush(Color(stof(fill_opacity) * 255, Fill[0], Fill[1], Fill[2]));
	vector<Point> Points = parsePoints(points);
	Point* points = Points.data();
	graphics.DrawLines(&pen, points, Points.size());
	graphics.FillPolygon(&brush, points, Points.size());
}

TXT::TXT(string content, int x, int y, string fill, int textSize) : content(content), x(x), y(y), fill(fill), textSize(textSize) {}

TXT::TXT()
{
	content = "";
	x = 0;
	y = 0;
	fill = "1";
	textSize = 0;
}

TXT::TXT(xml_node<>* node)
{
	string x1, y1, textSize1;
	xml_attribute<>* firstAttribute = node->first_attribute();
	content = node->value();
	//Default constructor
	x = 0, y = 0, fill = "1", textSize = 1;
	//Copy constructor
	while (firstAttribute != NULL)
	{
		string attributeName(firstAttribute->name());
		string attributeValue(firstAttribute->value());
		if (attributeName == "x") x1 = attributeValue;
		else if (attributeName == "y") y1 = attributeValue;
		else if (attributeName == "fill") fill = attributeValue;
		else if (attributeName == "font-size") textSize1 = attributeValue;
		firstAttribute = firstAttribute->next_attribute();
	}
	x = atoi(x1.c_str());
	y = atoi(y1.c_str());
	textSize = atoi(textSize1.c_str());
}

TXT::~TXT()
{
}

VOID TXT::Draw(HDC hdc)
{
	Graphics graphics(hdc);
	int* Fill = parseColor(fill);
	FontFamily  fontFamily(L"Times New Roman");
	Font        font(&fontFamily, textSize, FontStyleRegular, UnitPixel);
	PointF      pointF(x, y);
	SolidBrush  solidBrush(Color(255, Fill[0], Fill[1], Fill[2]));
	wstring wideText(content.begin(), content.end());
	graphics.DrawString(wideText.c_str(), -1, &font, PointF(x, y - textSize), &solidBrush);
	Pen      pen(Color(200, 0, 200));
}

Circle::Circle() : cx(0), cy(0), r(0), stroke(""), fill(""), stroke_width(""), fill_opacity(""), stroke_opacity("") {}

Circle::Circle(int cx, int cy, int r, string stroke, string fill, string stroke_width, string fill_opactiy, string stroke_opacity) {
	this->cx = cx;
	this->cy = cy;
	this->r = r;
	this->stroke = stroke;
	this->fill = fill;
	this->stroke_width = stroke_width;
	this->fill_opacity = fill_opacity;
	this->stroke_opacity = stroke_opacity;
}

Circle::Circle(xml_node<>* node) {
	string x1, y1, r1;
	xml_attribute<>* firstAttribute = node->first_attribute();
	//Default constructor
	cx = 0, cy = 0, r = 0, stroke = "", fill = "", stroke_width = "1", fill_opacity = "1", stroke_opacity = "1";
	//Copy constructor
	while (firstAttribute != NULL)
	{
		string attributeName(firstAttribute->name());
		string attributeValue(firstAttribute->value());
		if (attributeName == "cx") x1 = attributeValue;
		else if (attributeName == "cy") y1 = attributeValue;
		else if (attributeName == "r") r1 = attributeValue;
		else if (attributeName == "fill") fill = attributeValue;
		else if (attributeName == "stroke") stroke = attributeValue;
		else if (attributeName == "stroke-width") stroke_width = attributeValue;
		else if (attributeName == "stroke-opacity") stroke_opacity = attributeValue;
		else if (attributeName == "fill-opacity") fill_opacity = attributeValue;
		firstAttribute = firstAttribute->next_attribute();
	}
	cx = atoi(x1.c_str());
	cy = atoi(y1.c_str());
	r = stoi(r1);
}

Circle::~Circle() {}

VOID Circle::Draw(HDC hdc)
{
	Graphics graphics(hdc);
	int* Stroke = parseColor(stroke);
	Pen	pen(Color(stof(stroke_opacity) * 255, Stroke[0], Stroke[1], Stroke[2]), stof(stroke_width));
	int* Fill = parseColor(fill);
	SolidBrush brush(Color(stof(fill_opacity) * 255, Fill[0], Fill[1], Fill[2]));
	int d = r * 2;
	graphics.DrawEllipse(&pen, cx - r, cy - r, d, d);
	graphics.FillEllipse(&brush, cx - r, cy - r, d, d);
}

LINE::LINE(xml_node<>* node)
{
	xml_attribute<>* firstAttribute = node->first_attribute();
	//Default constructor
	x1 = y1 = x2 = y2 = 0, stroke = "", stroke_width = "1", stroke_opacity = "1";
	//Copy constructor
	while (firstAttribute != NULL) {
		string attributeName = firstAttribute->name();
		string attributeValue = firstAttribute->value();
		if (attributeName == "x1") x1 = stoi(attributeValue);
		else if (attributeName == "y1") y1 = stoi(attributeValue);
		else if (attributeName == "x2") x2 = stoi(attributeValue);
		else if (attributeName == "y2") y2 = stoi(attributeValue);
		else if (attributeName == "stroke") stroke = attributeValue;
		else if (attributeName == "stroke-width") stroke_width = attributeValue;
		else if (attributeName == "stroke-opacity") stroke_opacity = attributeValue;
		firstAttribute = firstAttribute->next_attribute();
	}
}

LINE::~LINE()
{
}

VOID LINE::Draw(HDC hdc)
{
	Graphics graphics(hdc);
	int* Stroke = parseColor(stroke);
	Pen	pen(Color(stof(stroke_opacity) * 255, Stroke[0], Stroke[1], Stroke[2]), stof(stroke_width));
	graphics.DrawLine(&pen, x1, y1, x2, y2);
}

RECTANGLES::RECTANGLES(xml_node<>* node)
{
	xml_attribute<>* firstAttribute = node->first_attribute();
	//Default constructor
	x = y = height = width = 0, stroke = "", fill = "",
		stroke_width = "1", fill_opacity = "1", stroke_opacity = "1";
	while (firstAttribute != NULL) {
		string attributeName = firstAttribute->name();
		string attributeValue = firstAttribute->value();
		if (attributeName == "x") x = stoi(attributeValue);
		else if (attributeName == "y") y = stoi(attributeValue);
		else if (attributeName == "height") height = stoi(attributeValue);
		else if (attributeName == "width") width = stoi(attributeValue);
		else if (attributeName == "stroke") stroke = attributeValue;
		else if (attributeName == "stroke-width") stroke_width = attributeValue;
		else if (attributeName == "stroke-opacity") stroke_opacity = attributeValue;
		else if (attributeName == "fill") fill = attributeValue;
		else if (attributeName == "fill-opacity") fill_opacity = attributeValue;
		firstAttribute = firstAttribute->next_attribute();
	}
}

RECTANGLES::~RECTANGLES()
{
}

VOID RECTANGLES::Draw(HDC hdc)
{
	Graphics graphics(hdc);
	int* Stroke = parseColor(stroke);
	Pen	pen(Color(stof(stroke_opacity) * 255, Stroke[0], Stroke[1], Stroke[2]), stof(stroke_width));
	int* Fill = parseColor(fill);
	SolidBrush brush(Color(stof(fill_opacity) * 255, Fill[0], Fill[1], Fill[2]));
	graphics.DrawRectangle(&pen, x, y, width, height);
	graphics.FillRectangle(&brush, x, y, width, height);
}

ELIP::ELIP(xml_node<>* node)
{
	xml_attribute<>* firstAttribute = node->first_attribute();
	//Default constructor
	x = y = rx = ry = 0, stroke = "", fill = "",
		stroke_width = "1", fill_opacity = "1", stroke_opacity = "1";
	//Copy constructor
	while (firstAttribute != NULL) {
		string attributeName = firstAttribute->name();
		string attributeValue = firstAttribute->value();
		if (attributeName == "cx") x = stoi(attributeValue);
		else if (attributeName == "cy") y = stoi(attributeValue);
		else if (attributeName == "rx") rx = stoi(attributeValue);
		else if (attributeName == "ry") ry = stoi(attributeValue);
		else if (attributeName == "stroke") stroke = attributeValue;
		else if (attributeName == "stroke-width") stroke_width = attributeValue;
		else if (attributeName == "stroke-opacity") stroke_opacity = attributeValue;
		else if (attributeName == "fill") fill = attributeValue;
		else if (attributeName == "fill-opacity") fill_opacity = attributeValue;
		firstAttribute = firstAttribute->next_attribute();
	}
}

ELIP::~ELIP()
{
}

VOID ELIP::Draw(HDC hdc)
{
	Graphics graphics(hdc);
	int* Stroke = parseColor(stroke);
	Pen	pen(Color(stof(stroke_opacity) * 255, Stroke[0], Stroke[1], Stroke[2]), stof(stroke_width));
	int* Fill = parseColor(fill);
	SolidBrush brush(Color(stof(fill_opacity) * 255, Fill[0], Fill[1], Fill[2]));
	graphics.DrawEllipse(&pen, x - rx, y - ry, rx * 2, ry * 2);
	graphics.FillEllipse(&brush, x - rx, y - ry, rx * 2, ry * 2);
}

Path::~Path()
{
    delete color;
}

Path::Path(xml_node<>* node)
{
    xml_attribute<>* firstAttribute = node->first_attribute();
    while (firstAttribute != NULL) {
        string attributeName = firstAttribute->name();
        string attributeValue = firstAttribute->value();
        if (attributeName == "d") {
            parseElement(attributeValue);
        }
        if (attributeName == "fill") {
            setColor(attributeValue);
        }
        firstAttribute = firstAttribute->next_attribute();
    }
}

void Path::parseFile(std::string filename)
{
    xml_document<> doc;
    xml_node<>* rootNode;
    string svgPath;

    // Read the xml file into a vector
    ifstream file(filename);
    vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    buffer.push_back('\0');
    // Parse the buffer using the xml file parsing library into doc 
    doc.parse<0>(&buffer[0]);

    rootNode = doc.first_node();
    xml_node<>* node = rootNode->first_node();

    while (node != NULL) {
        string nodeName = node->name();
        cout << nodeName << endl;
        xml_attribute<>* firstAttribute = node->first_attribute();
        while (firstAttribute != NULL) {
            string attributeName = firstAttribute->name();
            string attributeValue = firstAttribute->value();
            if (attributeName == "d") {
                parseElement(attributeValue);
            }
            if (attributeName == "fill") {
                setColor(attributeValue);
            }
            firstAttribute = firstAttribute->next_attribute();
        }
        node = node->next_sibling();
    }
}

void Path::parseElement(string d)
{
    std::vector<std::string> tokens;
    // Regex to match a command character followed by its numerical data
    std::regex regex(R"([MmLlHhVvCcSsQqTtAaZz][^MmLlHhVvCcSsQqTtAaZz]*)");
    std::sregex_iterator iter(d.begin(), d.end(), regex);
    std::sregex_iterator end;

    // Collect each match as a separate token
    while (iter != end) {
        tokens.push_back(iter->str());
        ++iter;
    }
    data = tokens;
}

void Path::createGraphicsPath(GraphicsPath& graphicsPath)
{
    PointF currentPoint = PointF{ 0,0 };
    PointF* lastStartPoint = NULL;
    ofstream fout("log.txt");
    for (auto it : data) {
        char command = it[0];
        if (command == 'C') {
            PointF point1, point2, point3;
            std::replace(it.begin(), it.end(), ',', ' ');
            std::istringstream stream(it.substr(1)); // Skip the first character

            // Parse the six floating point values
            stream >> point1.X >> point1.Y >> point2.X >> point2.Y >> point3.X >> point3.Y;
            graphicsPath.AddBezier(currentPoint, point1, point2, point3);
            currentPoint = point3;
            fout << command << " " << point1.X << " " << point1.Y << " " << point2.X << " " << point2.Y << " " << point3.X << " " << point3.Y << endl;
        }
        else if (command == 'c') {
            PointF point1, point2, point3;
            std::replace(it.begin(), it.end(), ',', ' ');
            std::istringstream stream(it.substr(1)); // Skip the first character

            // Parse the six floating point values
            stream >> point1.X >> point1.Y >> point2.X >> point2.Y >> point3.X >> point3.Y;
            point1.X += currentPoint.X;
            point1.Y += currentPoint.Y;
            point2.X += currentPoint.X;
            point2.Y += currentPoint.Y;
            point3.X += currentPoint.X;
            point3.Y += currentPoint.Y;
            graphicsPath.AddBezier(currentPoint, point1, point2, point3);

            currentPoint = point3;

            fout << command << " " << point1.X << " " << point1.Y << " " << point2.X << " " << point2.Y << " " << point3.X << " " << point3.Y << endl;
        }
        else if (command == 'M') {
            PointF point1;
            std::replace(it.begin(), it.end(), ',', ' ');
            std::istringstream stream(it.substr(1)); // Skip the first character
            stream >> point1.X >> point1.Y;

            if (lastStartPoint == NULL) {
                lastStartPoint = new PointF(point1);
            }
            currentPoint = point1;
            fout << command << " " << point1.X << " " << point1.Y << endl;
        }
        else if (command == 'm') {
            PointF point1;
            std::replace(it.begin(), it.end(), ',', ' ');
            std::istringstream stream(it.substr(1)); // Skip the first character
            stream >> point1.X >> point1.Y;
            point1.X += currentPoint.X;
            point1.Y += currentPoint.Y;
            currentPoint = point1;
            fout << command << " " << point1.X << " " << point1.Y << endl;
        }
        else if (command == 'S') {
            PointF point1, point2;
            std::replace(it.begin(), it.end(), ',', ' ');
            std::istringstream stream(it.substr(1)); // Skip the first character

            // Parse the six floating point values
            stream >> point1.X >> point1.Y >> point2.X >> point2.Y;
            graphicsPath.AddBezier(currentPoint, currentPoint, point1, point2);
            currentPoint = point2;
            fout << command << " " << point1.X << " " << point1.Y << " " << point2.X << " " << point2.Y << " " << endl;
        }
        else if (command == 's') {
            PointF point1, point2;
            std::replace(it.begin(), it.end(), ',', ' ');
            std::istringstream stream(it.substr(1)); // Skip the first character

            // Parse the six floating point values
            stream >> point1.X >> point1.Y >> point2.X >> point2.Y;
            point1.X += currentPoint.X;
            point1.Y += currentPoint.Y;
            point2.X += currentPoint.X;
            point2.Y += currentPoint.Y;
            graphicsPath.AddBezier(currentPoint, currentPoint, point1, point2);


            currentPoint = point2;

            fout << command << " " << point1.X << " " << point1.Y << " " << point2.X << " " << point2.Y << endl;
        }
        else if (command == 'L') {
            PointF point1;
            std::replace(it.begin(), it.end(), ',', ' ');
            std::istringstream stream(it.substr(1)); // Skip the first character

            // Parse the six floating point values
            stream >> point1.X >> point1.Y;
            graphicsPath.AddLine(currentPoint, point1);


            currentPoint = point1;
            fout << command << " " << point1.X << " " << point1.Y << endl;
        }
        else if (command == 'l') {
            PointF point1;
            std::replace(it.begin(), it.end(), ',', ' ');
            std::istringstream stream(it.substr(1)); // Skip the first character

            // Parse the six floating point values
            stream >> point1.X >> point1.Y;
            point1.X += currentPoint.X;
            point1.Y += currentPoint.Y;
            graphicsPath.AddLine(currentPoint, point1);


            currentPoint = point1;
            fout << command << " " << point1.X << " " << point1.Y << endl;
        }
        if (command == 'A') {
            const float PI = 3.14159265358979323846f;
            auto degreesToRadians = [&](float degrees) {
                return degrees * (PI / 180.0f);
                };
            float rx, ry, xAxisRotation;
            int largeArcFlag, sweepFlag;
            float x1, y1;
            float x0 = currentPoint.X;
            float y0 = currentPoint.Y;
            float cx, cy, startAngle, sweepAngle;
            std::replace(it.begin(), it.end(), ',', ' ');
            std::istringstream stream(it.substr(1)); // Skip the first character

            // Parse the six floating point values
            stream >> rx >> ry >> xAxisRotation >> largeArcFlag >> sweepFlag >> x1 >> y1;

            // Step 1: Compute (x1', y1') in the transformed coordinate space
            float phi = degreesToRadians(xAxisRotation);
            float dx2 = (x0 - x1) / 2.0f;
            float dy2 = (y0 - y1) / 2.0f;
            float x1p = cos(phi) * dx2 + sin(phi) * dy2;
            float y1p = -sin(phi) * dx2 + cos(phi) * dy2;

            // Step 2: Correct the radii if they are too small
            float rx_sq = rx * rx;
            float ry_sq = ry * ry;
            float x1p_sq = x1p * x1p;
            float y1p_sq = y1p * y1p;

            float lambda = (x1p_sq / rx_sq) + (y1p_sq / ry_sq);
            if (lambda > 1) {
                rx *= sqrt(lambda);
                ry *= sqrt(lambda);
                rx_sq = rx * rx;
                ry_sq = ry * ry;
            }

            // Step 3: Calculate the center of the ellipse (cx', cy') in the transformed space
            float sign = (largeArcFlag != sweepFlag) ? 1.0f : -1.0f;
            float sq = ((rx_sq * ry_sq) - (rx_sq * y1p_sq) - (ry_sq * x1p_sq)) / ((rx_sq * y1p_sq) + (ry_sq * x1p_sq));
            sq = (sq < 0) ? 0 : sq;
            float coef = sign * sqrt(sq);
            float cxp = coef * ((rx * y1p) / ry);
            float cyp = coef * (-(ry * x1p) / rx);

            // Step 4: Compute the center of the ellipse (cx, cy) in the original coordinate space
            cx = cos(phi) * cxp - sin(phi) * cyp + (x0 + x1) / 2.0f;
            cy = sin(phi) * cxp + cos(phi) * cyp + (y0 + y1) / 2.0f;

            // Step 5: Compute the start angle and sweep angle
            float theta1 = atan2((y1p - cyp) / ry, (x1p - cxp) / rx);
            float deltaTheta = atan2((-y1p - cyp) / ry, (-x1p - cxp) / rx) - theta1;

            // Normalize deltaTheta based on sweepFlag
            if (sweepFlag == 0 && deltaTheta > 0) {
                deltaTheta -= 2 * PI;
            }
            else if (sweepFlag == 1 && deltaTheta < 0) {
                deltaTheta += 2 * PI;
            }

            // Convert radians to degrees for output
            startAngle = theta1 * (180.0f / PI);
            sweepAngle = deltaTheta * (180.0f / PI);

            float theta = degreesToRadians(xAxisRotation);

            // Calculate terms for x and y bounds based on rotation
            float cosTheta = cos(theta);
            float sinTheta = sin(theta);

            // Calculate the bounding box dimensions
            float xRadiusEffect = sqrt((rx * cosTheta) * (rx * cosTheta) + (ry * sinTheta) * (ry * sinTheta));
            float yRadiusEffect = sqrt((rx * sinTheta) * (rx * sinTheta) + (ry * cosTheta) * (ry * cosTheta));

            // Calculate bounding box coordinates
            float xMin = cx - xRadiusEffect;
            float xMax = cx + xRadiusEffect;
            float yMin = cy - yRadiusEffect;
            float yMax = cy + yRadiusEffect;


            graphicsPath.AddArc(xMin, yMax, rx * 2, ry * 2, startAngle, sweepAngle);
            PointF point(x1, y1);
            currentPoint = point;
        }
        else if (command == 'a') {
            PointF point1, point2, point3;
            std::replace(it.begin(), it.end(), ',', ' ');
            std::istringstream stream(it.substr(1)); // Skip the first character

            // Parse the six floating point values
            stream >> point1.X >> point1.Y >> point2.X >> point2.Y >> point3.X >> point3.Y;
            point1.X += currentPoint.X;
            point1.Y += currentPoint.Y;
            point2.X += currentPoint.X;
            point2.Y += currentPoint.Y;
            point3.X += currentPoint.X;
            point3.Y += currentPoint.Y;
            graphicsPath.AddBezier(currentPoint, point1, point2, point3);

            currentPoint = point3;

            fout << command << " " << point1.X << " " << point1.Y << " " << point2.X << " " << point2.Y << " " << point3.X << " " << point3.Y << endl;
        }
        if (command == 'z' || command == 'Z') {
            if (lastStartPoint != NULL) {
                graphicsPath.AddLine(currentPoint, *lastStartPoint);
            }
        }
    }
    delete lastStartPoint;
}

unsigned int Path::hexToARGB(const std::string& hexColor)
{
    if (hexColor[0] != '#' || hexColor.size() != 7) {
        throw std::invalid_argument("Invalid color format. Expected format: #RRGGBB");
    }

    // Parse the RGB components
    unsigned int r, g, b;
    std::istringstream(hexColor.substr(1, 2)) >> std::hex >> r;
    std::istringstream(hexColor.substr(3, 2)) >> std::hex >> g;
    std::istringstream(hexColor.substr(5, 2)) >> std::hex >> b;

    // Construct ARGB with full opacity (0xFF for alpha)
    unsigned int argb = (0xFF << 24) | (r << 16) | (g << 8) | b;
    return argb;
}

void Path::setColor(std::string hexColor)
{
    if (hexColor[0] != '#' || hexColor.size() != 7) {
        throw std::invalid_argument("Invalid color format. Expected format: #RRGGBB");
    }

    // Parse the RGB components
    unsigned int r, g, b;
    std::istringstream(hexColor.substr(1, 2)) >> std::hex >> r;
    std::istringstream(hexColor.substr(3, 2)) >> std::hex >> g;
    std::istringstream(hexColor.substr(5, 2)) >> std::hex >> b;
    this->color = new Color(255, r, g, b);
    // Construct ARGB with full opacity (0xFF for alpha)
}

std::vector<std::string> Path::getData()
{
    return this->data;
}

Color Path::getColor()
{
    return *this->color;
}

void Path::draw(HDC hdc)
{
    Graphics graphics(hdc);
    Pen* pen = new Pen(*this->color, 1);
    Brush* brush = new SolidBrush(*this->color);
    GraphicsPath graphicsPath;
    createGraphicsPath(graphicsPath);
    graphics.DrawPath(pen, &graphicsPath);
    graphics.FillPath(brush, &graphicsPath);
    delete pen;
    delete brush;
}