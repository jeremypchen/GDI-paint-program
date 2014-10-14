/* Shape Factory Header */

#pragma once

#include "drawView.h"
#include "Shape.h"

class ShapeFactory
{

public:
	static std::shared_ptr<Shape> CreateShape(CDrawView::CurrentShape);
};