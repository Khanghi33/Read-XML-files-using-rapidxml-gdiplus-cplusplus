#include "stdafx.h"
#include "Element.h"

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
        if (attributeName == "style") {
            parseColor(attributeValue);
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
    ofstream out("data.txt", ios::app);
    for (auto it : tokens) {
        out << it << endl;
    }
}

void Path::createGraphicsPath(GraphicsPath& graphicsPath)
{
    PointF currentPoint{ 0,0 };
    PointF lastControlPoint{ 0, 0 };
    char lastCommand = 'O';
    ofstream fout("log.txt", ios::app);
    for (auto it : data) {
        char command = it[0];
        std::replace(it.begin(), it.end(), ',', ' ');
        std::istringstream stream(it.substr(1)); // Skip the first character
        if (command == 'C') {
            PointF point1, point2, point3;


            // Parse the six floating point values
            while (stream >> point1.X >> point1.Y >> point2.X >> point2.Y >> point3.X >> point3.Y) {
                graphicsPath.AddBezier(currentPoint, point1, point2, point3);
                currentPoint = point3;
                lastControlPoint = point2;
                fout << command << " " << point1.X << " " << point1.Y << " " << point2.X << " " << point2.Y << " " << point3.X << " " << point3.Y << endl;
            }
        }
        else if (command == 'c') {
            PointF point1, point2, point3;

            // Parse the six floating point values
            while (stream >> point1.X >> point1.Y >> point2.X >> point2.Y >> point3.X >> point3.Y) {
                point1.X += currentPoint.X;
                point1.Y += currentPoint.Y;
                point2.X += currentPoint.X;
                point2.Y += currentPoint.Y;
                point3.X += currentPoint.X;
                point3.Y += currentPoint.Y;
                graphicsPath.AddBezier(currentPoint, point1, point2, point3);
                lastControlPoint = point2;
                currentPoint = point3;
            }

            fout << command << " " << point1.X << " " << point1.Y << " " << point2.X << " " << point2.Y << " " << point3.X << " " << point3.Y << endl;
        }
        else if (command == 'M') {
            PointF point1;
            while (stream >> point1.X >> point1.Y)  {
                /*if (lastStartPoint.X == -1.0)
                lastStartPoint = point1;*/
                graphicsPath.StartFigure();
                currentPoint = point1;
                fout << command << " " << point1.X << " " << point1.Y << endl;
            }
        }
        else if (command == 'm') {
            PointF point1;
            while (stream >> point1.X >> point1.Y) {
                point1.X += currentPoint.X;
                point1.Y += currentPoint.Y;
                if (lastCommand == 'M' || lastCommand == 'm') {
                    graphicsPath.CloseFigure();
                }
                
                /*if (lastStartPoint.X == -1.0)
                lastStartPoint = point1;*/
                graphicsPath.StartFigure();
                currentPoint = point1;
                fout << command << " " << point1.X << " " << point1.Y << endl;
            }
        }
        else if (command == 'S') {
            PointF point1, point2;

            // Parse the six floating point values
            while (stream >> point1.X >> point1.Y >> point2.X >> point2.Y) {
                if (lastCommand == 'c' || lastCommand == 'C' || lastCommand == 's' || lastCommand == 'S') {
                    PointF controlPoint = { 2 * currentPoint.X - lastControlPoint.X, 2 * currentPoint.Y - lastControlPoint.Y };
                    graphicsPath.AddBezier(currentPoint, controlPoint, point1, point2);
                }
                else {
                    graphicsPath.AddBezier(currentPoint, currentPoint, point1, point2);
                }
                currentPoint = point2;
                lastControlPoint = point1;
                fout << command << " " << point1.X << " " << point1.Y << " " << point2.X << " " << point2.Y << " " << endl;
            }
            
        }
        else if (command == 's') {
            PointF point1, point2;

            
            
            // Parse the six floating point values
            while (stream >> point1.X >> point1.Y >> point2.X >> point2.Y) {
                point1.X += currentPoint.X;
                point1.Y += currentPoint.Y;
                point2.X += currentPoint.X;
                point2.Y += currentPoint.Y;
                
                if (lastCommand == 'c' || lastCommand == 'C' || lastCommand == 's' || lastCommand == 'S') {
                    PointF controlPoint = { 2 * currentPoint.X - lastControlPoint.X, 2 * currentPoint.Y - lastControlPoint.Y };
                    graphicsPath.AddBezier(currentPoint, controlPoint, point1, point2);
                }
                else {
                    graphicsPath.AddBezier(currentPoint, currentPoint, point1, point2);
                }
                currentPoint = point2;
                lastControlPoint = point1;
                fout << command << " " << point1.X << " " << point1.Y << " " << point2.X << " " << point2.Y << " " << endl;
            }
            
        }
        else if (command == 'L') {
            PointF point1;


            // Parse the six floating point values
            while (stream >> point1.X >> point1.Y) {
                graphicsPath.AddLine(currentPoint, point1);
                currentPoint = point1;
                fout << command << " " << point1.X << " " << point1.Y << endl;
            }
        }
        else if (command == 'l') {
            PointF point1;

            // Parse the six floating point values
            while (stream >> point1.X >> point1.Y) {
                point1.X += currentPoint.X;
                point1.Y += currentPoint.Y;
                graphicsPath.AddLine(currentPoint, point1);


                currentPoint = point1;
                fout << command << " " << point1.X << " " << point1.Y << endl;
            }
        }
        else if (command == 'A') {


            double x1, y1; // Start point
            double x2, y2; // End point
            double rx, ry; // Radii
            double xAxisRotation; // Rotation of the ellipse
            bool largeArcFlag, sweepFlag;
            RectF boundingRect;  // Output bounding rectangle
            double startAngle;   // Output start angle
            double sweepAngle;    // Output sweep angle
            double dx, dy;
            const float PI = 3.14159265358979323846f;
            auto DegreesToRadians = [PI](float degrees) {
                return degrees * (PI / 180.0f);
                };
            auto RadiansToDegrees = [PI](double radians) {
                return radians * (180.0f / PI);
                };
            // Parse the six floating point values
            while (stream >> rx >> ry >> xAxisRotation >> largeArcFlag >> sweepFlag >> dx >> dy) {
                x1 = currentPoint.X;
                y1 = currentPoint.Y;
                x2 = dx;
                y2 = dy;
                if (rx == 0 || ry == 0) {
                    boundingRect = RectF(static_cast<float>(x1), static_cast<float>(y1), 0, 0);
                    startAngle = sweepAngle = 0;
                    graphicsPath.AddArc(boundingRect, startAngle, sweepAngle);
                    return;
                }

                // Step 2: Transform endpoints to the ellipse coordinate system
                double cosRotation = std::cos(DegreesToRadians(xAxisRotation));
                double sinRotation = std::sin(DegreesToRadians(xAxisRotation));

                double dx = (x1 - x2) / 2.0;
                double dy = (y1 - y2) / 2.0;

                double x1Prime = cosRotation * dx + sinRotation * dy;
                double y1Prime = -sinRotation * dx + cosRotation * dy;

                // Step 3: Correct radii if necessary
                double rxSq = rx * rx;
                double rySq = ry * ry;
                double x1PrimeSq = x1Prime * x1Prime;
                double y1PrimeSq = y1Prime * y1Prime;

                double radiiCorrection = (x1PrimeSq / rxSq) + (y1PrimeSq / rySq);
                if (radiiCorrection > 1) {
                    double scaleFactor = std::sqrt(radiiCorrection);
                    rx *= scaleFactor;
                    ry *= scaleFactor;
                    rxSq = rx * rx;
                    rySq = ry * ry;
                }

                // Step 4: Compute center of the ellipse
                double sign = (largeArcFlag != sweepFlag) ? 1 : -1;
                double factor = sign * std::sqrt(max(0.0f, ((rxSq * rySq) - (rxSq * y1PrimeSq) - (rySq * x1PrimeSq)) / ((rxSq * y1PrimeSq) + (rySq * x1PrimeSq))));

                double cxPrime = factor * (rx * y1Prime / ry);
                double cyPrime = factor * -(ry * x1Prime / rx);

                double cx = cosRotation * cxPrime - sinRotation * cyPrime + (x1 + x2) / 2.0;
                double cy = sinRotation * cxPrime + cosRotation * cyPrime + (y1 + y2) / 2.0;

                // Step 5: Compute angles
                double theta1 = std::atan2((y1Prime - cyPrime) / ry, (x1Prime - cxPrime) / rx);
                double deltaTheta = std::atan2((-y1Prime - cyPrime) / ry, (-x1Prime - cxPrime) / rx) - theta1;

                if (!sweepFlag && deltaTheta > 0) {
                    deltaTheta -= 2 * PI;
                }
                else if (sweepFlag && deltaTheta < 0) {
                    deltaTheta += 2 * PI;
                }

                // Step 6: Set GDI+ parameters
                boundingRect = RectF(static_cast<float>(cx - rx), static_cast<float>(cy - ry),
                    static_cast<float>(2 * rx), static_cast<float>(2 * ry));
                startAngle = RadiansToDegrees(theta1);
                sweepAngle = RadiansToDegrees(deltaTheta);
                graphicsPath.AddArc(boundingRect, startAngle, sweepAngle);
                REAL x = x2;
                REAL y = y2;
                currentPoint = PointF{ x, y };
            }
        }
        else if (command == 'a') {
            double x1, y1; // Start point
            double x2, y2; // End point
            double rx, ry; // Radii
            double xAxisRotation; // Rotation of the ellipse
            bool largeArcFlag, sweepFlag;
            RectF boundingRect;  // Output bounding rectangle
            double startAngle;   // Output start angle
            double sweepAngle;    // Output sweep angle
            double dx, dy;
            const float PI = 3.14159265358979323846f;
            auto DegreesToRadians = [PI](double degrees) {
                return degrees * (PI / 180.0f);
            };
            auto RadiansToDegrees = [PI](double radians) {
                return radians * (180.0f / PI);
            };
            // Parse the six floating point values
            while (stream >> rx >> ry >> xAxisRotation >> largeArcFlag >> sweepFlag >> dx >> dy) {
                x1 = currentPoint.X;
                y1 = currentPoint.Y;
                x2 = dx + currentPoint.X;
                y2 = dy + currentPoint.Y;
                if (rx == 0 || ry == 0) {
                    boundingRect = RectF(static_cast<float>(x1), static_cast<float>(y1), 0, 0);
                    startAngle = sweepAngle = 0;
                    graphicsPath.AddArc(boundingRect, startAngle, sweepAngle);
                    return;
                }

                // Step 2: Transform endpoints to the ellipse coordinate system
                double cosRotation = std::cos(DegreesToRadians(xAxisRotation));
                double sinRotation = std::sin(DegreesToRadians(xAxisRotation));

                double dx = (x1 - x2) / 2.0;
                double dy = (y1 - y2) / 2.0;

                double x1Prime = cosRotation * dx + sinRotation * dy;
                double y1Prime = -sinRotation * dx + cosRotation * dy;

                // Step 3: Correct radii if necessary
                double rxSq = rx * rx;
                double rySq = ry * ry;
                double x1PrimeSq = x1Prime * x1Prime;
                double y1PrimeSq = y1Prime * y1Prime;

                double radiiCorrection = (x1PrimeSq / rxSq) + (y1PrimeSq / rySq);
                if (radiiCorrection > 1) {
                    double scaleFactor = std::sqrt(radiiCorrection);
                    rx *= scaleFactor;
                    ry *= scaleFactor;
                    rxSq = rx * rx;
                    rySq = ry * ry;
                }

                // Step 4: Compute center of the ellipse
                double sign = (largeArcFlag != sweepFlag) ? 1 : -1;
                double factor = sign * std::sqrt(max(0.0f, ((rxSq * rySq) - (rxSq * y1PrimeSq) - (rySq * x1PrimeSq)) / ((rxSq * y1PrimeSq) + (rySq * x1PrimeSq))));

                double cxPrime = factor * (rx * y1Prime / ry);
                double cyPrime = factor * -(ry * x1Prime / rx);

                double cx = cosRotation * cxPrime - sinRotation * cyPrime + (x1 + x2) / 2.0;
                double cy = sinRotation * cxPrime + cosRotation * cyPrime + (y1 + y2) / 2.0;

                // Step 5: Compute angles
                double theta1 = std::atan2((y1Prime - cyPrime) / ry, (x1Prime - cxPrime) / rx);
                double deltaTheta = std::atan2((-y1Prime - cyPrime) / ry, (-x1Prime - cxPrime) / rx) - theta1;

                if (!sweepFlag && deltaTheta > 0) {
                    deltaTheta -= 2 * PI;
                }
                else if (sweepFlag && deltaTheta < 0) {
                    deltaTheta += 2 * PI;
                }

                // Step 6: Set GDI+ parameters
                boundingRect = RectF(static_cast<float>(cx - rx), static_cast<float>(cy - ry),
                    static_cast<float>(2 * rx), static_cast<float>(2 * ry));
                startAngle = RadiansToDegrees(theta1);
                sweepAngle = RadiansToDegrees(deltaTheta);
                graphicsPath.AddArc(boundingRect, startAngle, sweepAngle);
                REAL x = x2;
                REAL y = y2;
                currentPoint = PointF{ x, y };
            }
        }
        else if (command == 'h') {
            REAL x;
            while (stream >> x) {
                PointF point{ currentPoint.X + x, currentPoint.Y };
                graphicsPath.AddLine(currentPoint, point);
                currentPoint = point;
            }
        }
        else if (command == 'H') {
            REAL x;
            while (stream >> x) {
                PointF point{x, currentPoint.Y };
                graphicsPath.AddLine(currentPoint, point);
                currentPoint = point;
            }
        }
        else if (command == 'v') {
            REAL y;
            while (stream >> y) {
                PointF point{ currentPoint.X, currentPoint.Y + y};
                graphicsPath.AddLine(currentPoint, point);
                currentPoint = point;
            }
        }
        else if (command == 'V') {
            REAL y;
            while (stream >> y) {
                PointF point{ currentPoint.X, y};
                graphicsPath.AddLine(currentPoint, point);
                currentPoint = point;
            }
        }
        else if (command == 'z' || command == 'Z') {
                graphicsPath.CloseFigure();
        }

        lastCommand = command;
    }
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

void Path::parseColor(std::string attribute)
{
    std::string fillColor;

    // Regular expressions to match clip-path and fill color
    std::regex fillColorRegex(R"(fill:(#[A-Fa-f0-9]+))");

    std::smatch match;
    // Extract fill color
    if (std::regex_search(attribute, match, fillColorRegex)) {
        fillColor = match[1]; // First capturing group
    }
    ofstream out("color.txt", ios::out);
    setColor(fillColor);
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

VOID Path::Draw(HDC hdc)
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

vector<Shape*> SVGParser::getShapes()
{
    return this->shapes;
}

SVGParser::SVGParser(string filename)
{
    this->filename = filename;
}

void SVGParser::parse()
{
    // Read XML
    xml_document<> doc;
    xml_node<>* rootNode;
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
        if (nodeName == "polygon") {
            POLYGON* poly = new POLYGON(node);
            shapes.push_back(poly);
        }
        else if (nodeName == "polyline") {
            POLYLINE* poly = new POLYLINE(node);
            shapes.push_back(poly);
        }
        else if (nodeName == "circle") {
            Circle* cir = new Circle(node);
            shapes.push_back(cir);
        }
        else if (nodeName == "text") {
            TXT* text = new TXT(node);
            shapes.push_back(text);
        }
        else if (nodeName == "rect") {
            RECTANGLES* rect = new RECTANGLES(node);
            shapes.push_back(rect);
        }
        else if (nodeName == "line") {
            LINE* line = new LINE(node);
            shapes.push_back(line);
        }
        else if (nodeName == "ellipse") {
            ELIP* elip = new ELIP(node);
            shapes.push_back(elip);
        }
        else if (nodeName == "path") {
            Path* path = new Path(node);
            shapes.push_back(path);
            /*ofstream fout("log.txt");
            vector<string> data = path->getData();
            for (auto it : data) {
                fout << it << endl;
            }
            Color color = path->getColor();
            fout << "r: " << color.GetR() << " g:" << color.GetG() << " b:" << color.GetB() << " a:" << color.GetA();*/
            /*path->draw(hdc);
            path->~Path();*/
        }
        node = node->next_sibling();
    }
}

Render::Render(vector<Shape*> shapes, HDC* hdc)
{
    this->shapes = shapes;
    this->hdc = hdc;
}

void Render::GDIRender()
{
    for (auto shape : shapes) {
        shape->Draw(*hdc);
    }
}
