#include "stdafx.h"
#include "Rectangle.h"

using namespace Gdiplus;

void Rectangle::draw(Gdiplus::Graphics & graphics)
{
	int topLeft_X, topLeft_Y, bottomRight_X, bottomRight_Y;
	int width, height;
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

	width = topLeft_X - bottomRight_X;
	height = topLeft_Y - bottomRight_Y;

	// DrawRectangle expects top left x, top left y, width and height of shape
	graphics.DrawRectangle(pen.get(), topLeft_X, topLeft_Y, width, height);
}