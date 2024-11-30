#include "stdafx.h"
#include "rapidxml.hpp"
#include "Group.h"

GROUP::GROUP(xml_node<>* node)
{
	ofstream fout("log.txt", ios::app);
    xml_attribute<>* firstAttribute = node->first_attribute();
	//Traversal
    if (firstAttribute) setTransform(firstAttribute->value());
    fout << getTransform() << "\n";
    xml_node<>* nodeChild = node->first_node();
    while (nodeChild != NULL) {
        string nodeName = nodeChild->name();
        if (nodeName == "polygon") {
            POLYGON* poly = new POLYGON(nodeChild);
            list.push_back(poly);
        }
        else if (nodeName == "polyline") {
            POLYLINE* poly = new POLYLINE(nodeChild);
            list.push_back(poly);
        }
        else if (nodeName == "circle") {
            CIRCLE* cir = new CIRCLE(nodeChild);
            list.push_back(cir);
        }
        else if (nodeName == "text") {
            TXT* text = new TXT(nodeChild);
            list.push_back(text);
        }
        else if (nodeName == "rect") {
            RECTANGLES* rect = new RECTANGLES(nodeChild);
            list.push_back(rect);
        }
        else if (nodeName == "line") {
            LINE* line = new LINE(nodeChild);
            list.push_back(line);
        }
        else if (nodeName == "ellipse") {
            ELIP* elip = new ELIP(nodeChild);
            list.push_back(elip);
        }
        //else if (nodeName == "path") {
        //    Path* path = new Path(node);
        //    list.push_back(path);
        //    /*ofstream fout("log.txt");
        //    vector<string> data = path->getData();
        //    for (auto it : data) {
        //        fout << it << endl;
        //    }
        //    Color color = path->getColor();
        //    fout << "r: " << color.GetR() << " g:" << color.GetG() << " b:" << color.GetB() << " a:" << color.GetA();*/
        //    /*path->draw(hdc);
        //    path->~Path();*/
        //}
        nodeChild = nodeChild->next_sibling();
    }
	fout.close();
}

GROUP::~GROUP()
{
}

VOID GROUP::Draw(HDC hdc)
{
    for (auto& child : list) {
        child->setTransform(getTransform());
        child->Draw(hdc);
    }
}
