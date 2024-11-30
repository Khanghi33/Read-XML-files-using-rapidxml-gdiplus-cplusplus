#include "rapidxml.hpp"
#include "stdafx.h"
#include "Path.h"
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;

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