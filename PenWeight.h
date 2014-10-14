/* Flyweight and Singleton class so pens can be stored and reused */

#pragma once

#include "stdafx.h"
#include "singleton.h"
#include <unordered_map>
#include <memory>
#include <sstream>

using namespace Gdiplus;

class PenWeight : public Singleton<PenWeight>
{
	DECLARE_SINGLETON(PenWeight);

public:
	static std::shared_ptr<Pen> GetPen(Gdiplus::Color const & color, float width);
};
	