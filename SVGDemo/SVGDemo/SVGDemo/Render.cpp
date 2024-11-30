#include "rapidxml.hpp"
#include "stdafx.h"
#include "Render.h"

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