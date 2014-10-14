#include "stdafx.h"
#include "Line.h"

using namespace Gdiplus;

void Line::draw(Gdiplus::Graphics & graphics)
{
	graphics.DrawLine(pen.get(), startPoint, endPoint);
}
