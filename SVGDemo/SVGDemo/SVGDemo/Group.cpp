#include "rapidxml.hpp"
#include "stdafx.h"
#include "Group.h"
using namespace std;
//group

GROUP::GROUP(xml_node<>* node)
{
    ofstream fout("log.txt", ios::app);
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
        if (attributeName == "fill") setFill(attributeValue);
        else if (attributeName == "stroke") setStroke(attributeValue);
        else if (attributeName == "stroke-width") setStrokeWidth(attributeValue);
        else if (attributeName == "style") parseStyle(attributeValue);
        else if (attributeName == "stroke-opacity") setStrokeOpacity(attributeValue);
        else if (attributeName == "fill-opacity") setFillOpacity(attributeValue);
        else if (attributeName == "transform") setTransform(attributeValue);
        firstAttribute = firstAttribute->next_attribute();
    }
    //Traversal
    xml_node<>* nodeChild = node->first_node();
    while (nodeChild != NULL) {
        string nodeName = nodeChild->name();
        /*fout << nodeName << " ";
        xml_attribute<>* firstAttribute = nodeChild->first_attribute();
        while (firstAttribute) {
            fout << firstAttribute->name() << ": " << firstAttribute->value() << " ";
            firstAttribute = firstAttribute->next_attribute();
        }
        fout << "\n";*/
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
        else if (nodeName == "g") {
            GROUP* g = new GROUP(nodeChild);
            list.push_back(g);
        }
        else if (nodeName == "path") {
            Path* path = new Path(nodeChild);
            list.push_back(path);
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
        nodeChild = nodeChild->next_sibling();
    }
    fout.close();
}
GROUP::GROUP()
{
    setStroke("");
    setFill("");
    setFillOpacity("1");
    setStrokeWidth("1");
    setStrokeOpacity("1");
}


GROUP::~GROUP()
{
}
//string fullyTransform(const string& transform1, const  string& transform2) {
//    string newTransform = transform2;
//    stringstream ss(transform1); string tmp;
//    while (getline(ss, tmp, '(')) {
//        if (transform2.find(tmp) < 0 || transform2.find(tmp) > transform2.length()) {
//            if(tmp == "rotate") newTransform += " " + tmp + "(0)";
//            else newTransform += " " + tmp + "(0, 0)";
//        }
//    }
//    return newTransform;
//}
//string combineTransforms(const string& transform1, const  string& transform2) {
//    ofstream fout("log.txt", ios::app);
//    //regex pattern(R"(translate\((-?\d+),(-?\d+)\) rotate\((-?\d+)\) scale\((-?\d+)\))");
//    regex rotation_pattern(R"(rotate\(\s*(-?\d*\.?\d+)\s*\))");
//    regex translate_pattern(R"(translate\(\s*(-?\d+(?:\.\d+)?)\s*,?\s*(-?\d+(?:\.\d+)?)\s*\))");
//    regex scale_pattern1(R"(scale\(\s*(-?\d*\.?\d+)\s*\))");
//    regex scale_pattern2(R"(scale\(\s*(-?\d*\.?\d+)\s*,\s*(-?\d*\.?\d+)\s*\))");
//
//    smatch matches;
//
//    REAL tx1, ty1, rot1, scale1_1, scale1_2;
//    tx1 = ty1 = rot1 = 0; scale1_1 = scale1_2 = 1;
//    fout << "----------------start--------------\n";
//    fout << "Transform1: " << transform1 << "\n";
//    fout << "Transform2: " << transform2 << "\n";
//    if (regex_search(transform1, matches, translate_pattern)) {
//        tx1 = stof(matches[1]);
//        ty1 = stof(matches[2]);
//        //fout << "tx1, ty1: " << tx1 << " " << ty1 << "\n";
//    }
//
//    if (regex_search(transform1, matches, rotation_pattern)) {
//        rot1 = stof(matches[1]);
//        //fout << "rot1: " << rot1 << "\n";
//    }
//
//    if (regex_search(transform1, matches, scale_pattern1)) {
//        scale1_1 = scale1_2 = stof(matches[1]);
//        //fout << "scale1: " << scale1 << "\n";
//    }
//
//    if (regex_search(transform1, matches, scale_pattern2)) {
//        scale1_1 = stof(matches[1]);
//        scale1_2 = stof(matches[2]);
//        //fout << "scale1: " << scale1 << "\n";
//    }
//
//
//
//    REAL tx2, ty2, rot2, scale2_1, scale2_2;
//    tx2 = ty2 = rot2 = 0; scale2_1 = scale2_2 = 1;
//
//    if (regex_search(transform2, matches, translate_pattern)) {
//        tx2 = stof(matches[1]);
//        ty2 = stof(matches[2]);
//        //fout << "tx2, ty2: " << tx2 << " " << ty2 << "\n";
//    }
//
//    if (regex_search(transform2, matches, rotation_pattern)) {
//        rot2 = stof(matches[1]);
//        //fout << "rot2: " << rot2 << "\n";
//    }
//
//    if (regex_search(transform2, matches, scale_pattern1)) {
//        scale2_1 = scale2_2 = stof(matches[1]);
//    }
//
//    if (regex_search(transform2, matches, scale_pattern2)) {
//        scale2_1 = stof(matches[1]);
//        scale2_2 = stof(matches[2]);
//        //fout << "scale1: " << scale1 << "\n";
//    }
//
//    /*if (scale1 == 0) scale1 = 1;
//    if (scale2 == 0) scale2 = 1;*/
//
//    float total_tx = tx1 + tx2;
//    float total_ty = ty1 + ty2;
//    float total_rot = rot1 + rot2;
//    float total_scale1 = scale1_1 * scale2_1;
//    float total_scale2 = scale1_2 * scale2_2;
//
//    string newTransform = transform2 + " " + transform1;
//    string Translate = "translate(" + to_string(total_tx) + "," + to_string(total_ty) + ")";
//    string Rotate = "rotate(" + to_string(total_rot) + ")";
//    string Scale = "sclale(" + to_string(total_scale1) + "," + to_string(total_scale2) + ")";
//    regex_replace(newTransform, translate_pattern, Translate);
//    regex_replace(newTransform, rotation_pattern, Rotate);
//    regex_replace(newTransform, scale_pattern1, Scale);
//    regex_replace(newTransform, scale_pattern2, Scale);
//    /*string new_transform = "translate(" + to_string(total_tx) + ", " + to_string(total_ty) + ") rotate(" + to_string(total_rot) + ") scale(" + to_string(total_scale1) + ", " + to_string(total_scale2) + ")";
//    fout << new_transform << "\n";
//    fout << "-----------end-----------------\n";*/
//    return newTransform;
//}

VOID GROUP::Draw(HDC hdc)
{
    /*ofstream fout("log.txt", ios::app);
    for (auto& child : list) {
        child->setTransform(combineTransforms(child->getTransform(), this->getTransform()));
        if (child->getStroke() == "") child->setStroke(getStroke());
        if (child->getFill() == "") child->setFill(getFill());
        if (child->getStrokeWidth() == "1") child->setStrokeWidth(getStrokeWidth());
        if (child->getStrokeOpacity() == "1") child->setStrokeOpacity(getStrokeOpacity());
        if (child->getFillOpacity() == "1") child->setFillOpacity(getFillOpacity());
        child->Draw(hdc);
    }
    fout << getTransform() << "\n";*/
    ofstream fout("log.txt", ios::app);
    for (auto& child : list) {
        child->setTransform(this->getTransform() + child->getTransform());
        if (child->getStroke() == "") child->setStroke(getStroke());
        if (child->getFill() == "") child->setFill(getFill());
        if (child->getStrokeWidth() == "1") child->setStrokeWidth(getStrokeWidth());
        if (child->getStrokeOpacity() == "1") child->setStrokeOpacity(getStrokeOpacity());
        if (child->getFillOpacity() == "1") child->setFillOpacity(getFillOpacity());
        child->Draw(hdc);
    }
    fout << getTransform() << "\n";
}