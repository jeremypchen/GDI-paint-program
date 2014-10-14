#include "stdafx.h"
#include "Shape.h"
#include "PenWeight.h"
#include <memory>

using namespace Gdiplus;

void Shape::setPen(std::shared_ptr<Pen> p)
{
	Color color;
	REAL width = p->GetWidth();
	p->GetColor(&color);

	// Call FlyWeight/Singleton pen method to retrieve pen
	pen = PenWeight::get().GetPen(color, width);
}

void Shape::setStartPoint(Gdiplus::Point sp)
{
	startPoint = sp;
}

void Shape::setEndPoint(Gdiplus::Point ep)
{
	endPoint = ep;
}

