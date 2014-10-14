/* Abstract Base Shape class */

#pragma once

#include <memory>

class Shape
{
protected:
	Gdiplus::Point startPoint;
	Gdiplus::Point endPoint;
	std::shared_ptr<Gdiplus::Pen> pen;

public:
	virtual void draw(Gdiplus::Graphics &) = 0;		// Pure virtual function

	void setPen(std::shared_ptr<Gdiplus::Pen>);
	void setStartPoint(Gdiplus::Point);
	void setEndPoint(Gdiplus::Point);
};