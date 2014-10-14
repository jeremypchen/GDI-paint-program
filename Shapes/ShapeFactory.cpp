#include "stdafx.h"
#include "ShapeFactory.h"
#include "Line.h"
#include "EllipseShape.h"
#include "RectangleShape.h"

std::shared_ptr<Shape> ShapeFactory::CreateShape(CDrawView::CurrentShape shape)
{
	// Return proper shape based on currentShape
	switch (shape){
	case (CDrawView::CurrentShape::LINE) :
		return std::make_shared<Line>();
		break;
	case (CDrawView::CurrentShape::ELLIPSE) :
		return std::make_shared<EllipseShape>();
		break;
	case (CDrawView::CurrentShape::RECTANGLE) :
		return std::make_shared<RectangleShape>();
		break;
	default :
		return std::make_shared<Line>();
	}
}