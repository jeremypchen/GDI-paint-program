/* EllipseShape, derived from Shape class */

#pragma once
#include "Shape.h"

class EllipseShape : public Shape
{
public:
	void draw(Gdiplus::Graphics &);
};