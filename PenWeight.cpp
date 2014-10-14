/* Flyweight and Singleton class so pens can be stored and reused */

#include "stdafx.h"
#include "PenWeight.h"

using namespace Gdiplus;

std::shared_ptr<Pen> PenWeight::GetPen(Gdiplus::Color const & color, float width)
{
	// Map to store existing pens. Key: string, Value: Shared pointer to pen
	static std::unordered_map<std::string, std::shared_ptr<Pen>> existingPens;

	int rValue = color.GetR();
	int gValue = color.GetG();
	int bValue = color.GetB();
	std::stringstream penKeyStream;
	penKeyStream.precision(1);	// So float precision of width is to one decimal
	
	// Create the key based on RGB values and width. ex: "(255,255,255);4f"
	penKeyStream << "(" << rValue << "," << gValue << "," << bValue << ");" << width;
	std::string penKey = penKeyStream.str();

	// If existingPens does not have this key
	if (existingPens.find(penKey) == existingPens.end())
	{
		// Make the pen, insert into existingPens, and return the pen
		auto newPen = std::make_shared<Pen>(color, width);
		existingPens.emplace(std::make_pair(penKey, newPen));

		return newPen;
	}
	// If existingPens does have this key, return the pen
	else
	{
		return existingPens.at(penKey);
	}
}