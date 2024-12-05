#include "rapidxml.hpp"
#include "stdafx.h"
#include "SVGParser.h"
using namespace std;
using namespace rapidxml;
using namespace Gdiplus;

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
    xml_node<>* node = rootNode->first_node();
    while (node != NULL) { 
        string nodeName = node->name();
        /*fout << nodeName << " ";
        xml_attribute<>* firstAttribute = node->first_attribute();
        while (firstAttribute) {
            fout << firstAttribute->name() << ": " << firstAttribute->value() << " ";
            firstAttribute = firstAttribute->next_attribute();
        }
        fout << "\n";*/
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
        node = node->next_sibling();
    }
    fout.close();
}