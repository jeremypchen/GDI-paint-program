// drawView.h : interface of the CDrawView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>

#include "Shape.h"

#include <list>

class CDrawView : public CScrollWindowImpl<CDrawView>
{
public:
	DECLARE_WND_CLASS(NULL)

	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP(CDrawView)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		CHAIN_MSG_MAP(CScrollWindowImpl<CDrawView>)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	CDrawView();

	enum CurrentShape { LINE, ELLIPSE, RECTANGLE };

	Gdiplus::Bitmap m_BitmapImage;
	Gdiplus::Graphics m_GraphicsImage;
	Gdiplus::Pen pen;

	bool previouslySaved;
	std::string save_filename;

	std::shared_ptr<Shape> shape;
	CurrentShape currentShape;

	std::list<std::shared_ptr<Shape>> undoList;
	std::list<std::shared_ptr<Shape>> redoList;
};
