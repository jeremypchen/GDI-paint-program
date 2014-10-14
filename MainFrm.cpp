// MainFrm.cpp : implmentation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "drawView.h"
#include "MainFrm.h"

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
		return TRUE;

	return m_view.PreTranslateMessage(pMsg);
}

BOOL CMainFrame::OnIdle()
{
	UIUpdateToolBar();
	return FALSE;
}

LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// create command bar window
	HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
	// attach menu
	m_CmdBar.AttachMenu(GetMenu());
	// load command bar images
	m_CmdBar.LoadImages(IDR_MAINFRAME);
	// remove old menu
	SetMenu(NULL);

	HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndCmdBar);
	AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

	CreateSimpleStatusBar();

	m_hWndClient = m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL, WS_EX_CLIENTEDGE);
	m_view.SetScrollSize(1,1);
	UIAddToolBar(hWndToolBar);
	UISetCheck(ID_VIEW_TOOLBAR, 1);
	UISetCheck(ID_VIEW_STATUS_BAR, 1);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	return 0;
}

LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	bHandled = FALSE;
	return 1;
}

LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	PostMessage(WM_CLOSE);
	return 0;
}

LRESULT CMainFrame::OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// Clear undo and redo lists, as well as the screen to white (Color(255,255,255))
	m_view.undoList.clear();
	m_view.redoList.clear();

	m_view.m_GraphicsImage.Clear(Gdiplus::Color(255, 255, 255));

	m_view.RedrawWindow();

	return 0;
}

LRESULT CMainFrame::OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	static BOOL bVisible = TRUE;	// initially visible
	bVisible = !bVisible;
	CReBarCtrl rebar = m_hWndToolBar;
	int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
	rebar.ShowBand(nBandIndex, bVisible);
	UISetCheck(ID_VIEW_TOOLBAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
	::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
	UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

/* Pen Color Change Handler */
LRESULT CMainFrame::OnPenColor(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// Initialize this to current color
	Gdiplus::Color oldColor;
	m_view.pen.GetColor(&oldColor);

	COLORREF color = COLORREF(oldColor.GetValue()); // GetRValue(oldColor), GetGValue(color), GetBValue(color));
	CColorDialog colorDialog(color);
	colorDialog.DoModal();
	// Grab color selected
	color = colorDialog.GetColor();
	Gdiplus::Color newColor(GetRValue(color), GetGValue(color), GetBValue(color));
	
	m_view.pen.SetColor(newColor);

	return 0;
}

/* Pen Width Change Handler */
LRESULT CMainFrame::OnWidth(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// Uncheck all, then recheck the selected width, to ensure
	// only one width is selected
	m_CmdBar.GetMenu().CheckMenuItem(ID_WIDTH_0, MF_UNCHECKED);
	m_CmdBar.GetMenu().CheckMenuItem(ID_WIDTH_1, MF_UNCHECKED);
	m_CmdBar.GetMenu().CheckMenuItem(ID_WIDTH_2, MF_UNCHECKED);
	m_CmdBar.GetMenu().CheckMenuItem(ID_WIDTH_3, MF_UNCHECKED);
	m_CmdBar.GetMenu().CheckMenuItem(ID_WIDTH_4, MF_UNCHECKED);
	m_CmdBar.GetMenu().CheckMenuItem(ID_WIDTH_5, MF_UNCHECKED);
	m_CmdBar.GetMenu().CheckMenuItem(wID, MF_CHECKED);

	// Change the width of the pen based on what width was selected
	switch (wID){
	case(ID_WIDTH_0) :
		m_view.pen.SetWidth(0.5);
		break;

	case(ID_WIDTH_1) :
		m_view.pen.SetWidth(1.0);
		break;

	case(ID_WIDTH_2) :
		m_view.pen.SetWidth(1.5);
		break;

	case(ID_WIDTH_3) :
		m_view.pen.SetWidth(2.0);
		break;

	case(ID_WIDTH_4) :
		m_view.pen.SetWidth(2.5);
		break;

	case(ID_WIDTH_5) :
		m_view.pen.SetWidth(5);
	}

	return 0;
}

/* Undo Button Handler */
LRESULT CMainFrame::OnUndo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// If there are shapes to be undrawn
	if (!m_view.undoList.empty())
	{
		// Put the shape that will be undrawn onto the redo list
		m_view.redoList.push_back(m_view.undoList.back());
		m_view.undoList.pop_back();

		m_view.m_GraphicsImage.Clear(Gdiplus::Color(255, 255, 255));

		/* After clearing the screen, redraw all the items from the undoList,
		which effectively redraws what was on the screen originally
		minus the shape that was undrawn */
		for (std::shared_ptr<Shape> shape : m_view.undoList)
		{
			shape->draw(m_view.m_GraphicsImage);
		}

		m_view.RedrawWindow();
	}

	return 0;
}

/* Redo Button Handler */
LRESULT CMainFrame::OnRedo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// If there are shapes to redraw
	if (!m_view.redoList.empty())
	{
		// Move most recent item to redraw onto the back of the undo list 
		m_view.undoList.push_back(m_view.redoList.back());
		m_view.redoList.pop_back();

		// Draw this image that needs to be redrawn
		m_view.undoList.back()->draw(m_view.m_GraphicsImage);
		m_view.RedrawWindow();

	}

	return 0;
}

/* Shape Handler */
LRESULT CMainFrame::OnShape(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// Uncheck all, then recheck the selected shape, to ensure
	// only one shape is selected
	m_CmdBar.GetMenu().CheckMenuItem(ID_SHAPE_LINE, MF_UNCHECKED);
	m_CmdBar.GetMenu().CheckMenuItem(ID_SHAPE_ELLIPSE, MF_UNCHECKED);
	m_CmdBar.GetMenu().CheckMenuItem(ID_SHAPE_RECTANGLE, MF_UNCHECKED);
	m_CmdBar.GetMenu().CheckMenuItem(wID, MF_CHECKED);

	switch (wID){
	case(ID_SHAPE_LINE) :
		m_view.currentShape = m_view.CurrentShape::LINE;
		break;
	case (ID_SHAPE_ELLIPSE) :
		m_view.currentShape = m_view.CurrentShape::ELLIPSE;
		break;
	case (ID_SHAPE_RECTANGLE) :
		m_view.currentShape = m_view.CurrentShape::RECTANGLE;
		break;
	}

	return 0;
}

/* Save Button Handler */
LRESULT CMainFrame::OnSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// If haven't previously saved the file, then call OnSaveAs Handler and return 0;
	if (m_view.previouslySaved == FALSE)
	{
		BOOL bHandled = TRUE;
		OnSaveAs(HIWORD(NULL), LOWORD(NULL), (HWND)NULL, (BOOL&) bHandled);

		return 0;
	}
	// Otherwise save to same file
	else 
	{
		CLSID pngClsid;
		CLSIDFromString(L"{557cf406-1a04-11d3-9a73-0000f81ef32e}", &pngClsid);
		m_view.m_BitmapImage.Save(CA2W(m_view.save_filename.c_str()), &pngClsid); // Gets save_filename from m_view

		return 0;
	}
}

/* Save As Button Handler */
LRESULT CMainFrame::OnSaveAs(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	char filter[] = { "PNG Files (*.png)" };

	// OFN_OVERWRITEPROMPT asks if user wants to overwrite file if same name is used
	CFileDialog saveDialog(FALSE, _T("png"), _T(".png"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter);

	if (saveDialog.DoModal() == IDOK)
	{
		CLSID pngClsid;
		CLSIDFromString(L"{557cf406-1a04-11d3-9a73-0000f81ef32e}", &pngClsid);
		m_view.m_BitmapImage.Save(CA2W(saveDialog.m_szFileName), &pngClsid); // Gets save_filename from the dialog box

		// Set previouslySaved to TRUE so pressing Save button will not trigger Save As again
		m_view.previouslySaved = TRUE;	
		m_view.save_filename = saveDialog.m_szFileName;
	}

	return 0;
}

/* Open Button Handler */
LRESULT CMainFrame::OnOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	char filter[] = { "PNG Files\0*.png\0All Files\0*.*\0"};

	CFileDialog openDialog(TRUE, _T("png"), _T(".png"), OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter);

	if (openDialog.DoModal() == IDOK)
	{
		Gdiplus::Bitmap myFile(CA2W(openDialog.m_szFileName));
		m_view.m_GraphicsImage.DrawImage(&myFile, 0, 0);

		m_view.undoList.clear();
		m_view.redoList.clear();

		// Set previouslySaved to TRUE so pressing Save button will not trigger Save As
		m_view.previouslySaved = TRUE;
		m_view.save_filename = openDialog.m_szFileName;

		m_view.RedrawWindow();
	}

	return 0;
}
