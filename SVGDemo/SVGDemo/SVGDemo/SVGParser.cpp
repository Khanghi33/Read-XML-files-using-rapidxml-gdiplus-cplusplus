#include "rapidxml.hpp"
#include "stdafx.h"
#include "SVGParser.h"
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;
//svgparser

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
    ViewBox* viewBox = ViewBox::getInstance();
    viewBox->parse(rootNode);
    xml_node<>* node = rootNode->first_node();
    while (node != NULL) { 
        string nodeName = node->name();
        //fout << nodeName << "\n";
        if (nodeName == "polygon") {
            POLYGON* poly = new POLYGON(node);
            shapes.push_back(poly);
        }
        else if (nodeName == "polyline") {
            POLYLINE* poly = new POLYLINE(node);
            shapes.push_back(poly);
        }
        else if (nodeName == "circle") {
            CIRCLE* cir = new CIRCLE(node);
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
        else if (nodeName == "g") {
            GROUP* g = new GROUP(node);
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