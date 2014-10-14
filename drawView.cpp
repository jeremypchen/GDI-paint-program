// drawView.cpp : implementation of the CDrawView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include <memory>

#include "drawView.h"
#include "ShapeFactory.h"

using namespace Gdiplus;

CDrawView::CDrawView()
	: m_BitmapImage(1024, 768)
	, m_GraphicsImage(&m_BitmapImage)
	, pen(Color(0, 0, 0))
	, currentShape(LINE)						// Default shape to start is a Line
	, previouslySaved(FALSE)					// Default is that it's not previously saved, so Save As handler will be called when Save is pushed
	, save_filename()							// save_filename is empty to start
{
}

BOOL CDrawView::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

LRESULT CDrawView::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CPaintDC dc(m_hWnd);

	Graphics graphics(dc);
	graphics.DrawImage(&m_BitmapImage, 0, 0);

	if (shape)
	{
		shape->draw(graphics);
	}

	return 0;
}

/* When left mouse button is pushed */
LRESULT CDrawView::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int start_xPos = GET_X_LPARAM(lParam);
	int start_yPos = GET_Y_LPARAM(lParam);
	Gdiplus::Point startPoint = Point(start_xPos, start_yPos);
	std::shared_ptr<Pen> myPen(pen.Clone());

	// Call factory method to return proper shape based on currentShape
	shape = (ShapeFactory::CreateShape(currentShape));			

	shape->setPen(myPen);
	shape->setStartPoint(startPoint);

	return 0;
}

/* When left mouse button is released */
LRESULT CDrawView::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// If there is a shape to draw
	if (shape != NULL)
	{
		int end_xPos = GET_X_LPARAM(lParam);
		int end_yPos = GET_Y_LPARAM(lParam);
		Gdiplus::Point endPoint = Point(end_xPos, end_yPos);

		shape->setEndPoint(endPoint);
		shape->draw(m_GraphicsImage);

		// When new shape is drawn, redoList can be cleared
		redoList.clear();

		RedrawWindow();

		// Push shape onto undo list in case it needs to be undrawn, and then reset
		undoList.push_back(shape);
		shape.reset();
	}

	return 0;
}

/* When mouse is moved */
LRESULT CDrawView::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// If there is a shape being drawn
	if (shape)
	{
		int end_xPos = GET_X_LPARAM(lParam);
		int end_yPos = GET_Y_LPARAM(lParam);
		Gdiplus::Point endPoint = Point(end_xPos, end_yPos);

		// Set the endpoint so a line preview can be drawn
		shape->setEndPoint(endPoint);

		RedrawWindow();
	}

	return 0;
}
