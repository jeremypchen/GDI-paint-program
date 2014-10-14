#include "stdafx.h"
#include "EllipseShape.h"

using namespace Gdiplus;

void EllipseShape::draw(Gdiplus::Graphics & graphics)
{
	int topLeft_X, topLeft_Y, bottomRight_X, bottomRight_Y;
	int width, height;

	// Get topLeft and bottomRight points of Ellipse
	if (startPoint.X <= endPoint.X)
	{
		topLeft_X = startPoint.X;
		bottomRight_X = endPoint.X;
	}
	else
	{
		topLeft_X = endPoint.X;
		bottomRight_X = startPoint.X;
	}

	if (startPoint.Y <= endPoint.Y)
	{
		topLeft_Y = startPoint.Y;
		bottomRight_Y = endPoint.Y;
	}
	else
	{
		topLeft_Y = endPoint.Y;
		bottomRight_Y = startPoint.Y;
	}

	// Get widths and heights of Ellipse
	width = bottomRight_X - topLeft_X;
	height = bottomRight_Y - topLeft_Y;

	// DrawEllipse expects top left x, top left y, width and height of shape
	graphics.DrawEllipse(pen.get(), topLeft_X, topLeft_Y, width, height);
}

