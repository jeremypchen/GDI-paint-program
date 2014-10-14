/* Rectangle class, derived from Shape*/

#pragma once
#include "Shape.h"

class RectangleShape : public Shape
{
public:
	void draw(Gdiplus::Graphics &);
};