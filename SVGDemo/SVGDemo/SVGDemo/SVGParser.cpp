#include "rapidxml.hpp"
#include "stdafx.h"
#include "SVGParser.h"
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;

// Returns the list of parsed shapes stored in the `shapes` vector.
vector<Shape*> SVGParser::getShapes()
{
    return this->shapes;
}

// Constructor that initializes the SVGParser with the given filename.
SVGParser::SVGParser(string filename)
{
    this->filename = filename;
}

// Parses the SVG file, extracts its elements, and creates appropriate shape objects.
void SVGParser::parse()
{
    ofstream fout("log.txt", ios::app);
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

    // Parse the viewBox attribute using the ViewBox singleton instance.
    ViewBox* viewBox = ViewBox::getInstance();
    viewBox->parse(rootNode);

    // Iterate through all child nodes of the root node to process individual elements.
    xml_node<>* node = rootNode->first_node();
    while (node != NULL) { 
        string nodeName = node->name();
        //fout << nodeName << "\n";
        // Check the node name and create corresponding shape objects.
        if (nodeName == "polygon") {
            POLYGON* poly = new POLYGON(node); // Create a polygon shape.
            shapes.push_back(poly); 
        }
        else if (nodeName == "polyline") {
            POLYLINE* poly = new POLYLINE(node);     // Create a polyline shape.
            shapes.push_back(poly);
        }
        else if (nodeName == "circle") {
            CIRCLE* cir = new CIRCLE(node); // Create a circle shape.
            shapes.push_back(cir);
        }
        else if (nodeName == "text") {
            TXT* text = new TXT(node);  // Create a text element.
            shapes.push_back(text);
        }
        else if (nodeName == "rect") {
            RECTANGLES* rect = new RECTANGLES(node);    // Create a rectangle shape.
            shapes.push_back(rect);
        }
        else if (nodeName == "line") {
            LINE* line = new LINE(node);    // Create a line shape.
            shapes.push_back(line);
        }
        else if (nodeName == "ellipse") {
            ELIP* elip = new ELIP(node);    // Create an ellipse shape.
            shapes.push_back(elip);
        }
        else if (nodeName == "path") {
            Path* path = new Path(node);    // Create a path shape.
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
        else if (nodeName == "g") {
            GROUP* g = new GROUP(node);     // Create a group of shapes.
            shapes.push_back(g);
        }
        else if (nodeName == "defs") {
            xml_node<>* childNode = node->first_node();
            while (childNode != NULL) {
                string childNodeName = childNode->name();
                if (childNodeName == "linearGradient") {
                    LinearGradient* gradient = LinearGradient::getInstance();
                    gradient->parse(childNode);
                }
                childNode = childNode->next_sibling();
            }
        }
        node = node->next_sibling();
    }
}


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