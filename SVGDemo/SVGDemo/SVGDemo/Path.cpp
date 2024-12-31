#include "rapidxml.hpp"
#include "stdafx.h"
#include "Path.h"
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;
//path


Path::~Path()
{
    delete color;
}

Path::Path(xml_node<>* node)
{
    /*xml_attribute<>* firstAttribute = node->first_attribute();
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
    }*/




    xml_attribute<>* firstAttribute = node->first_attribute();
    //Default constructor
    setStroke("");
    setFill("");
    setFillOpacity("1");
    setStrokeWidth("1");
    setStrokeOpacity("1");
    //Copy constructor
    while (firstAttribute != NULL)
    {
        string attributeName(firstAttribute->name());
        string attributeValue(firstAttribute->value());
        if (attributeName == "d") {
            parseElement(attributeValue);
        }
        else if (attributeName == "fill") setFill(attributeValue);
        else if (attributeName == "style") parseStyle(attributeValue);
        else if (attributeName == "stroke") setStroke(attributeValue);
        else if (attributeName == "stroke-width") setStrokeWidth(attributeValue);
        else if (attributeName == "stroke-opacity") setStrokeOpacity(attributeValue);
        else if (attributeName == "fill-opacity") setFillOpacity(attributeValue);
        else if (attributeName == "transform") setTransform(attributeValue);
        firstAttribute = firstAttribute->next_attribute();
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
            int count = 0;
            //while (stream >> point1.X >> point1.Y) {
            //    
            //    if (count > 0) {
            //        graphicsPath.AddLine(currentPoint, point1);
            //        graphicsPath.CloseFigure();
            //    }
            //    if (lastCommand == 'M' || lastCommand == 'm') {
            //        graphicsPath.CloseFigure();
            //    }
            //    graphicsPath.StartFigure();
            //    /*if (lastStartPoint.X == -1.0)
            //    lastStartPoint = point1;*/
            //    //graphicsPath.StartFigure();
            //    currentPoint = point1;
            //    fout << command << " " << point1.X << " " << point1.Y << endl;
            //    count++;
            //}
            while (stream >> point1.X >> point1.Y) {
                if (count == 0) {
                    // For the first point, just move the pen
                    currentPoint = point1;
                    graphicsPath.StartFigure(); // Start a new figure
                }
                else {
                    // For subsequent points, draw lines
                    graphicsPath.AddLine(currentPoint, point1);
                    currentPoint = point1; // Update the current point
                }

                fout << command << " " << point1.X << " " << point1.Y << endl;
                count++;
            }
        }
        else if (command == 'm') {
            PointF point1;
            int count = 0;
            //while (stream >> point1.X >> point1.Y) {

            //    if (count > 0) {
            //        graphicsPath.AddLine(currentPoint, point1);
            //        graphicsPath.CloseFigure();
            //    }
            //    if (lastCommand != 'O') {
            //        point1.X += currentPoint.X;
            //        point1.Y += currentPoint.Y;
            //    }
            //    if (lastCommand == 'M' || lastCommand == 'm') {
            //        graphicsPath.CloseFigure();
            //    }

            //    /*if (lastStartPoint.X == -1.0)
            //    lastStartPoint = point1;*/
            //    graphicsPath.StartFigure();
            //    currentPoint = point1;
            //    fout << command << " " << point1.X << " " << point1.Y << endl;
            //    count++;
            //}
            while (stream >> point1.X >> point1.Y) {
                if (lastCommand != 'O') {
                    point1.X += currentPoint.X;
                    point1.Y += currentPoint.Y;
                }
                if (count == 0) {
                    // For the first point, just move the pen
                    currentPoint = point1;
                    graphicsPath.StartFigure(); // Start a new figure
                }
                else {
                    // For subsequent points, draw lines
                    graphicsPath.AddLine(currentPoint, point1);
                    currentPoint = point1; // Update the current point
                }

                fout << command << " " << point1.X << " " << point1.Y << endl;
                count++;
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
                PointF point{ x, currentPoint.Y };
                graphicsPath.AddLine(currentPoint, point);
                currentPoint = point;
            }
        }
        else if (command == 'v') {
            REAL y;
            while (stream >> y) {
                PointF point{ currentPoint.X, currentPoint.Y + y };
                graphicsPath.AddLine(currentPoint, point);
                currentPoint = point;
            }
        }
        else if (command == 'V') {
            REAL y;
            while (stream >> y) {
                PointF point{ currentPoint.X, y };
                graphicsPath.AddLine(currentPoint, point);
                currentPoint = point;
            }
        }
        else if (command == 'Q') {
            PointF point1, point2;

            // Parse the six floating point values
            while (stream >> point1.X >> point1.Y >> point2.X >> point2.Y) {
                PointF controlPoint1{ currentPoint.X + (2.0f / 3.0f) * (point1.X - currentPoint.X) , currentPoint.Y + (2.0f / 3.0f) * (point1.Y - currentPoint.Y) };
                PointF controlPoint2{ point2.X + (2.0f / 3.0f) * (point1.X - point2.X), point2.Y + (2.0f / 3.0f) * (point1.Y - point2.Y) };

                graphicsPath.AddBezier(currentPoint, controlPoint1, controlPoint2, point2);
                currentPoint = point2;
                fout << command << " " << point1.X << " " << point1.Y << " " << point2.X << " " << point2.Y << " " << endl;
            }
        }
        else if (command == 'q') {
            PointF point1, point2;

            // Parse the six floating point values
            while (stream >> point1.X >> point1.Y >> point2.X >> point2.Y) {
                point1.X += currentPoint.X;
                point1.Y += currentPoint.Y;
                point2.X += currentPoint.X;
                point2.Y += currentPoint.Y;

                PointF controlPoint1{ currentPoint.X + (2.0f / 3.0f) * (point1.X - currentPoint.X) , currentPoint.Y + (2.0f / 3.0f) * (point1.Y - currentPoint.Y) };
                PointF controlPoint2{ point2.X + (2.0f / 3.0f) * (point1.X - point2.X), point2.Y + (2.0f / 3.0f) * (point1.Y - point2.Y) };

                graphicsPath.AddBezier(currentPoint, controlPoint1, controlPoint2, point2);
                currentPoint = point2;
                fout << command << " " << point1.X << " " << point1.Y << " " << point2.X << " " << point2.Y << " " << endl;
            }
        }
        else if (command == 'z' || command == 'Z') {
            graphicsPath.CloseFigure();
        }

        lastCommand = command;
    }
}

//unsigned int Path::hexToARGB(const std::string& hexColor)
//{
//    if (hexColor[0] != '#' || hexColor.size() != 7) {
//        throw std::invalid_argument("Invalid color format. Expected format: #RRGGBB");
//    }
//
//    // Parse the RGB components
//    unsigned int r, g, b;
//    std::istringstream(hexColor.substr(1, 2)) >> std::hex >> r;
//    std::istringstream(hexColor.substr(3, 2)) >> std::hex >> g;
//    std::istringstream(hexColor.substr(5, 2)) >> std::hex >> b;
//
//    // Construct ARGB with full opacity (0xFF for alpha)
//    unsigned int argb = (0xFF << 24) | (r << 16) | (g << 8) | b;
//    return argb;
//}
//
//void Path::parseColor(std::string attribute)
//{
//    std::string fillColor;
//
//    // Regular expressions to match clip-path and fill color
//    //std::regex fillColorRegex(R"(fill:(#[A-Fa-f0-9]+))");
//    std::regex fillColorRegex(R"(fill:(#[A-Fa-f0-9]{6}))");
//    std::smatch match;
//    // Extract fill color
//    if (std::regex_search(attribute, match, fillColorRegex)) {
//        fillColor = match[1]; // First capturing group
//    }
//    ofstream out("color.txt", ios::out);
//    setColor(fillColor);
//}
//
//void Path::setColor(std::string hexColor)
//{
//    if (hexColor[0] != '#' || hexColor.size() != 7) {
//        throw std::invalid_argument("Invalid color format. Expected format: #RRGGBB");
//    }
//
//    // Parse the RGB components
//    unsigned int r, g, b;
//    std::istringstream(hexColor.substr(1, 2)) >> std::hex >> r;
//    std::istringstream(hexColor.substr(3, 2)) >> std::hex >> g;
//    std::istringstream(hexColor.substr(5, 2)) >> std::hex >> b;
//    this->color = new Color(255, r, g, b);
//    // Construct ARGB with full opacity (0xFF for alpha)
//}

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
    /*Graphics graphics(hdc);
    if (this->color == NULL) {
        this->color = new Color(255, 0, 0, 0);
    }
    Pen* pen = new Pen(*this->color, 1);
    Brush* brush = new SolidBrush(*this->color);
    GraphicsPath graphicsPath;
    createGraphicsPath(graphicsPath);
    graphics.DrawPath(pen, &graphicsPath);
    graphics.FillPath(brush, &graphicsPath);
    delete pen;
    delete brush;*/





    Graphics graphics(hdc);
    ViewBox* v = ViewBox::getInstance();
    v->applyViewBox(graphics);
    applyTransform(graphics);


    //Set up color pen and draw
    int* Stroke = parseColor(getStroke());
    Pen	pen(Color(stof(getStrokeOpacity()) * 255, Stroke[0], Stroke[1], Stroke[2]), stof(getStrokeWidth()));
    int* Fill = parseColor(getFill());
    SolidBrush brush(Color(stof(getFillOpacity()) * 255, Fill[0], Fill[1], Fill[2]));

    GraphicsPath graphicsPath;
    createGraphicsPath(graphicsPath);






    if (getStroke() != "" && getStroke() != "none") {
        graphics.DrawPath(&pen, &graphicsPath);
    }
    
    if (getGradientId(getFill()) != "") {
        string id = getGradientId(getFill());


        // Get the bounds of the path
        RectF bounds;
        graphicsPath.GetBounds(&bounds);

        // Get the gradient instance
        LinearGradient* gradient = LinearGradient::getInstance();

        // Pass the bounds to the getBrush method
        LinearGradientBrush* gradientBrush = gradient->getBrush(id, &bounds);

        // Fill the path with the gradient brush
        if (gradientBrush != nullptr) {
            graphics.FillPath(gradientBrush, &graphicsPath);
        }

        // Clean up if necessary (if getBrush dynamically allocates the brush)
        delete gradientBrush;
    }
    /*else if (getFill() != "" && getFill() != "none")
    {
        graphics.FillPath(&brush, &graphicsPath);
    }*/
    else if (getFill() != "none") {
        graphics.FillPath(&brush, &graphicsPath);
    }

}