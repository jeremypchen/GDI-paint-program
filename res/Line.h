/* Line class, dervied from Shape*/

#pragma once
#include "Shape.h"

class Line : public Shape
{
public:
	void draw(Gdiplus::Graphics &);
};