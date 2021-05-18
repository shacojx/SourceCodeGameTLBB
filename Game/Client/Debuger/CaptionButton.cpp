// CaptionButton.cpp: Implementierung der Klasse CCaptionButton.
//
//////////////////////////////////////////////////////////////////////
/*! 
\class		CCaptionButton 

*  \date		04/2004
*  \author	<b> Thomas Latuske </b>

UPDATES:
***	04.06.2004
- TRACKMOUSEEVENT structure added
- TrackMouseEvent(TRACKMOUSEEVENT lpEventTrack) - function added
- Message-handling for WM_NCMOUSELEAVE added
- changed some values in COLORADJUSTMENT structure
*** 11.06.2004
- solved Problem for disappering buttons on right-mouseclick
in NC-area
- solved problem for delay of drawing the buttons when dialog
is re-activated by a left-mouseclick in the NC-area
////////////////////////////////////////////////////////////////////
*/

#include "stdafx.h"
#include "CaptionButton.h"
#include "Global.h"
#include "DlgDebuger.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////


// static member initialization:
HWND CCaptionButton::m_hWndCap	= NULL;
BOOL CCaptionButton::m_bActive	= -1;
// Map of Buttons which have been hooked:
CMap<UINT, UINT&, CCaptionButton*, CCaptionButton*&> CCaptionButton::m_mapBtn;

//HRGN CCaptionButton::m_hrAllBtn	= 0;
//HRGN CCaptionButton::m_hrNC		= 0;

CCaptionButton::CCaptionButton()
{
	m_bEnable = TRUE;
	m_bBtnChecked = FALSE;
	m_bCheckBtn	= FALSE;
	m_bHighLight = FALSE;
	m_rc = (0,0,0,0);
	m_uiBtnCount = 0;
	m_CAdj = new COLORADJUSTMENT;
	m_TME.cbSize      = sizeof(TRACKMOUSEEVENT);
	m_TME.dwFlags     = TME_LEAVE;
	m_TME.hwndTrack   = m_hWndCap;
	m_TME.dwHoverTime = HOVER_DEFAULT;
}

CCaptionButton::~CCaptionButton()
{
	//DeleteObject(m_hrAllBtn);
	//DeleteObject(m_hrNC);
	delete m_CAdj;
}


// announce the user defined message:
DECLARE_USER_MESSAGE(WMU_HITCAPBUTTON)

// point = the position where the hit occured
// bHover = check if mouse was only hovering(TRUE) or clicked(FALSE)
// return value = button hovered/clicked
BOOL CCaptionButton::ButtonHitTest(CPoint point, BOOL bHover)
{
	if(m_bActive && m_bEnable)
	{
		CPoint hitPoint;
		::ScreenToClient(m_hWndCap, &point);
		hitPoint.x = point.x;
		hitPoint.y = point.y *(-1); // *(-1) because the point.y has coordinates of
		// the client area (i.e. -12) and we need the point in the NC area (i.e. 12)

		// if point is in the rect of the button, then toggle the state of the BOOL member:
		if(m_rc.PtInRect(hitPoint))
		{	// case hoverstate is requested:
			if(bHover && (bHover != m_bHighLight))
			{
				m_bHighLight = TRUE;
				// redraw the NC area
				::RedrawWindow(m_hWndCap, m_rc, NULL, RDW_INVALIDATE | RDW_FRAME);
				return TRUE;
			}
			else if(!bHover) // case button is clicked:
			{
				m_bHighLight = FALSE;
				ChangeButtonState();
				return TRUE;
			}
		}
		else if(m_bHighLight)
		{	// case mouse is not longer over this button:
			m_bHighLight = FALSE;
			// redraw the NC area
			::RedrawWindow(m_hWndCap, m_rc, NULL, RDW_INVALIDATE | RDW_FRAME);
		}
	}
	return FALSE;
}



// the function name says it:
void CCaptionButton::ChangeButtonState()
{
	m_bBtnChecked ? m_bBtnChecked = FALSE : m_bBtnChecked = TRUE;
	// redraw the NC area
	::RedrawWindow(m_hWndCap, m_rc, NULL, RDW_INVALIDATE | RDW_FRAME);
	::SendMessage(g_pDlgDebug->GetSafeHwnd(), WMU_HITCAPBUTTON , (WPARAM)m_uiBtnCount, (LPARAM)m_bBtnChecked);
	
}



// dc = the dc to change values
void CCaptionButton::PaintBmpActive(CDC *dc)
{
	if(!m_bActive)
	{	// set the stretch-draw-mode to HALFTONE:
		dc->SetStretchBltMode(HALFTONE);
		// get the current COLORADJUSTMET struct:
		dc->GetColorAdjustment(m_CAdj);
		// change some values of the struct:	
		m_CAdj->caColorfulness	= -80;
		m_CAdj->caBrightness	= 15;
		// set the changed struct:
		dc->SetColorAdjustment(m_CAdj);
	}   

	if(!m_bEnable)
	{	// set the stretch-draw-mode to HALFTONE:
		dc->SetStretchBltMode(HALFTONE);
		// get the current COLORADJUSTMET struct:
		dc->GetColorAdjustment(m_CAdj);
		// change some values of the struct:	
		m_CAdj->caColorfulness	= -80;
		m_CAdj->caBrightness	= -25;
		// set the changed struct:
		dc->SetColorAdjustment(m_CAdj);
	}   

	/*
	m_CAdj->caBlueGamma		= 2500;
	m_CAdj->caRedGamma		= 2500;
	m_CAdj->caGreenGamma	= 2500;
	m_CAdj->caContrast		= 10;
	m_CAdj->caFlags			= NULL;
	m_CAdj->caIlluminantIndex= ILLUMINANT_DAYLIGHT;
	m_CAdj->caRedGreenTint	= 0;
	m_CAdj->caReferenceWhite	= 10000;
	m_CAdj->caReferenceBlack	= 0;
	m_CAdj->caSize			= sizeof(m_CAdj);
	*/
}




// uiBtnCount = the no. of the button to paint
// return value = button checked TRUE/FALSE
BOOL CCaptionButton::PaintBmpCheckBtn(UINT uiBtnCount/*= 1*/)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	UINT BmpID = 0;
	CSize sizeButton(0,0);
	CBitmap m_bmpBitmap;
	BITMAP bm;
	CDC dcMemo;
	CDC *dc = FromHandle(m_hWndCap)->GetWindowDC();

	m_uiBtnCount = uiBtnCount;

	// calculate the caption rectangle:
	CalcCapRect(&m_rc);

	if(GetWindowLong(m_hWndCap, GWL_EXSTYLE) & WS_EX_TOOLWINDOW)
	{
		sizeButton.cx = GetSystemMetrics(SM_CXSMSIZE);
		sizeButton.cy = GetSystemMetrics(SM_CYSMSIZE);
	}
	else
	{
		sizeButton.cx = GetSystemMetrics(SM_CXSIZE);
		sizeButton.cy = GetSystemMetrics(SM_CYSIZE);
	}


	// this "m_rc" is re-used here for the size of the button:
	m_rc.right -= sizeButton.cx;
	m_rc.left = m_rc.right - sizeButton.cx+2;


	// find state and load bitmap from the resource:
	m_bBtnChecked ? BmpID = m_idBmpDown : BmpID = m_idBmpUp;

	// load the bitmap into the handle:
	HBITMAP	hBmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(BmpID),
		IMAGE_BITMAP,
		m_rc.Width(),
		m_rc.Height(),
		LR_LOADMAP3DCOLORS);
	VERIFY(hBmp);

	// get the bitmap handle into the CBitmap:
	m_bmpBitmap.Attach(hBmp);

	// get an Bitmap to retrieve the size:
	m_bmpBitmap.GetBitmap(&bm);

	// make the new dcMemo compatible to the existing one(dc):
	dcMemo.CreateCompatibleDC(dc);

	// select the new bitmap into the DC and catch the old one to re-select:
	CBitmap* pOldBitmap = dcMemo.SelectObject(&m_bmpBitmap);

	// call this Fn to draw the Bmp active or inactive:
	PaintBmpActive(dc);


	// blow the bitmap to the titlebar:
	dc->StretchBlt(	m_rc.left,	m_rc.top,
		m_rc.Width(), m_rc.Height(),
		&dcMemo,
		0,  0,
		bm.bmWidth, bm.bmHeight,
		SRCCOPY);

	// re-select the old bitmap:
	dcMemo.SelectObject(pOldBitmap);
	DeleteObject(pOldBitmap);

	// paint a 3D frame around the bitmap:
	PaintBtnFrame(dc);

	// clean up:
	::ReleaseDC(m_hWndCap ,dcMemo.m_hDC);
	::ReleaseDC(m_hWndCap ,dc->m_hDC);

	return m_bBtnChecked;
}



// dc = the dc to paint to
void CCaptionButton::PaintBtnFrame(CDC *dc)
{
	if(m_bHighLight && !m_bBtnChecked && !m_bCheckBtn)
	{
		dc->Draw3dRect(m_rc,RGB(255, 255, 255),RGB(155, 155, 150));
		return;
	}


	if(m_bCheckBtn)
	{	
		if(m_bActive)
			dc->Draw3dRect(m_rc,RGB(0, 0, 0),RGB(0, 0, 0));
		else
			dc->Draw3dRect(m_rc,RGB(230, 230, 230),RGB(233, 233, 233));
	}
	else if(!m_bCheckBtn)
	{
		if(!m_bBtnChecked && m_bActive)
			dc->Draw3dRect(m_rc,RGB(235, 235, 235),RGB(0, 0, 0));
		else if(m_bBtnChecked && m_bActive)
			dc->Draw3dRect(m_rc,RGB(0, 0, 0),RGB(255, 255, 255));
		else
			dc->Draw3dRect(m_rc,RGB(230, 230, 230),RGB(233, 233, 233));
	}
}




// pCapRect = pointer to the caption rectangle
void CCaptionButton::CalcCapRect(CRect *pCapRect)
{
	CSize sizeButton(0,0);
	CSize sizeCaption(0,0);

	// Get the needed system metrics:
	CSize sizeFrame (GetSystemMetrics(SM_CXFRAME),  GetSystemMetrics(SM_CYFRAME));

	if(GetWindowLong(m_hWndCap, GWL_EXSTYLE) & WS_EX_TOOLWINDOW)
	{
		sizeButton.cx = GetSystemMetrics(SM_CXSMSIZE);
		sizeButton.cy = GetSystemMetrics(SM_CYSMSIZE);
		sizeCaption.cy= GetSystemMetrics(SM_CYSMCAPTION);
	}
	else
	{
		sizeButton.cx = GetSystemMetrics(SM_CXSIZE);
		sizeButton.cy = GetSystemMetrics(SM_CYSIZE);
		sizeCaption.cy= GetSystemMetrics(SM_CYCAPTION);
	}

	// Get the window rectangle and window style:
	CRect rcWindow;
	FromHandle(m_hWndCap)->GetWindowRect(rcWindow);

	DWORD dwStyle = FromHandle(m_hWndCap)->GetStyle();


	// Set the rectangle based on the system metrics:
	if (dwStyle & WS_THICKFRAME)
	{
		pCapRect->left = 0;
		pCapRect->top = ((sizeCaption.cy+sizeFrame.cy)-sizeButton.cy)+1;
		// assume there are already 3 buttons
		pCapRect->right = rcWindow.Width()-(((1+m_uiBtnCount)*sizeButton.cx))-3;

	}
	else if (dwStyle & WS_MINIMIZEBOX||dwStyle & WS_MAXIMIZEBOX)
	{
		pCapRect->left = 0;
		pCapRect->top = ((sizeCaption.cy+sizeFrame.cy)-sizeButton.cy);
		// assume there are already 3 buttons
		pCapRect->right = rcWindow.Width()-(((1+m_uiBtnCount)*sizeButton.cx))-3;
	}
	else if (dwStyle & WS_SYSMENU)
	{
		pCapRect->left = 0;
		pCapRect->top = ((sizeCaption.cy+sizeFrame.cy)-sizeButton.cy);
		// assume there is only 1 button
		pCapRect->right = rcWindow.Width()-((m_uiBtnCount*sizeButton.cx))-2; 
	}
	else if (dwStyle & WS_CAPTION)
	{
		pCapRect->left = 0;
		pCapRect->top = ((sizeCaption.cy+sizeFrame.cy)-sizeButton.cy);
		// assume there is no button
		pCapRect->right = rcWindow.Width()-((m_uiBtnCount*sizeButton.cx)-sizeButton.cx)-2; 
	}

	pCapRect->bottom = ((pCapRect->top - sizeFrame.cy)) + sizeButton.cy;
}




// bActive = reference to the state of the window
// return value = the state of the window/dialogue:
BOOL CCaptionButton::isWindowActive(BOOL &bActive)
{
	/* Paul di Lascia's "Witchcraft: ;-) */
	// Mimic MFC kludge to stay active if WF_STAYACTIVE bit is on
	//
	if (FromHandle(m_hWndCap)->m_nFlags & WF_STAYACTIVE)
		bActive = TRUE;

	if (!FromHandle(m_hWndCap)->IsWindowEnabled())	// but not if disabled
		bActive = FALSE;

	if (bActive == m_bActive)
		return TRUE;		// nothing to do

	m_bActive = bActive;	// update the state of the Bitmaps

	// do not call the base class because it will call Default()
	// and we may have changed bActive.
	return (BOOL)::DefWindowProc(m_hWndCap, WM_NCACTIVATE, bActive, 0L);
}




// return value = previous button state
BOOL CCaptionButton::EnableButton(BOOL bEnable)
{
	if(m_bEnable == bEnable)	return m_bEnable;

	BOOL bOld = m_bEnable;
	m_bEnable = bEnable;
	::RedrawWindow(m_hWndCap, m_rc, NULL, RDW_INVALIDATE | RDW_FRAME);
	return bOld;
}




// hWnd = Handle of the window to put the button to
// return value = window successful hooked or not
BOOL CCaptionButton::InitCapBtn(HWND hWnd)
{
	ASSERT(::IsWindow(hWnd));  // window must already exist
	CCaptionButton::m_hWndCap = hWnd;
	if(!HookDlg())	
		return FALSE;

	return TRUE;
}







void CCaptionButton::SetBmpID(UINT iBtnNo,	// No. of the button (R-L order)
							  UINT ID_Bmp_up, UINT ID_Bmp_down/*= 0*/,	 // bitmap ID's
							  BOOL bCheckBtn/*= FALSE*/)	// this is a checkbutton or not
{
	// set the var if button is a checknutton or not:	
	m_bCheckBtn = bCheckBtn;

	// fill the CMap with the class instances:
	m_mapBtn[iBtnNo] = this;

	// set the bitmap id's:
	m_idBmpUp = ID_Bmp_up;

	if(!ID_Bmp_down)
	{
		m_idBmpDown = ID_Bmp_up;
	}
	else m_idBmpDown = ID_Bmp_down;
}




// redirects the messages for the given window by attaching HookedWindowProc to it:
// return value = WindowProc changed or not:
BOOL CCaptionButton::HookDlg()
{
	ASSERT(::IsWindow(CCaptionButton::m_hWndCap));  // window must already exist
	if(!(m_pWndProcOld = (WNDPROC)(LONG_PTR)::SetWindowLong(CCaptionButton::m_hWndCap , GWL_WNDPROC, (DWORD)(DWORD_PTR)CCaptionButton::HookedWindowProc)))
		return FALSE;

	return TRUE;
}




// The window procedure through which the messages will be routed:
LRESULT CALLBACK CCaptionButton::HookedWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// the no. of map-entrys:
	UINT iMC = (UINT)m_mapBtn.GetCount();
	// the pointer which will be retrieved from the map:
	CCaptionButton* pBtn = NULL;
	// instance counter for the "for-loops":
	UINT n = 1;
	BOOL b;
	// Handle messages here:
	switch (message) {
		case WM_NCACTIVATE:
			// intercept WM_NCACTIVATE and call the static function isWindowActive!
			// do not call the base class because it will call Default() 
			// and we may have changed bActive.
			// get object for this buttons from the map:
			b = CCaptionButton::isWindowActive((BOOL &)wParam);
			// Now it's time to repaint the button(s):
			// get object for this buttons from the map:
			for(; n <= iMC; n++)
			{
				VERIFY(m_mapBtn.Lookup(n, pBtn));
				pBtn->PaintBmpCheckBtn(n);
			}
			return b;
			break;
		case WM_NCLBUTTONDOWN:
			//		case WM_NCLBUTTONUP:
		case WM_NCLBUTTONDBLCLK:
			// get object for this buttons from the map:
			for(; n <= iMC; n++)
			{
				VERIFY(m_mapBtn.Lookup(n, pBtn));
				if(pBtn->ButtonHitTest((CPoint)lParam))	return 0;
			}
			break;
		case WM_NCRBUTTONDOWN:
		case WM_NCRBUTTONUP:
		case WM_NCRBUTTONDBLCLK:
		case WM_NCMBUTTONDOWN:
		case WM_NCMBUTTONUP:
		case WM_NCMBUTTONDBLCLK:
			return 0;
			break;
		case WM_NCMOUSEMOVE:
			// get object for this buttons from the map:
			for(; n <= iMC; n++)
			{
				VERIFY(m_mapBtn.Lookup(n, pBtn));
				if(pBtn->ButtonHitTest((CPoint)lParam, TRUE))	return 0;
				_TrackMouseEvent(&pBtn->m_TME);
			}

			break;
		case WM_NCMOUSELEAVE:
			// get object for this buttons from the map:
			for(; n <= iMC; n++)
			{
				VERIFY(m_mapBtn.Lookup(n, pBtn));
				pBtn->ButtonHitTest((CPoint)lParam, FALSE);
				_TrackMouseEvent(&pBtn->m_TME);
			}

			break;
		case WM_NCPAINT:
			::DefWindowProc(hWnd, WM_NCPAINT, wParam, lParam);
			// get object for this buttons from the map:
			for(; n <= iMC; n++)
			{
				VERIFY(m_mapBtn.Lookup(n, pBtn));
				pBtn->PaintBmpCheckBtn(n);
			}
			return 0;
			break;
		case WM_SETTEXT:
			// if you want to change the window text (Title) intercept WM_SETTEXT:
			// call this static function here:
			if(lParam != NULL)
			{
				CCaptionButton::OnMsgSetText((LPCSTR)lParam);
			}
			return 0;
			break;
	}

	// if the message is not handled yet then return to the original window proc:
	return ::CallWindowProc(m_pWndProcOld, hWnd, message, wParam, lParam);
}




// lpText = pointer to the text of the caption-bar
void CCaptionButton::OnMsgSetText(LPCSTR lpText)
{
	CString sText;
	FromHandle(m_hWndCap)->GetWindowText(sText);
	if(sText == lpText) return;	// nothin' todo

	// Turn WS_VISIBLE off before callinf DefWindowProc,
	// so DefWindowProc won't paint and thereby cause flicker.
	//
	DWORD dwStyle = FromHandle(m_hWndCap)->GetStyle();
	if (dwStyle & WS_VISIBLE)
		SetWindowLong(m_hWndCap, GWL_STYLE, (dwStyle & ~ WS_VISIBLE));

	::DefWindowProc(m_hWndCap, WM_SETTEXT, 0L, (LPARAM)lpText);

	if (dwStyle & WS_VISIBLE)
		SetWindowLong(m_hWndCap, GWL_STYLE, dwStyle);

	// At this point, nothing has happened (since WS_VISIBLE was off).
	// Now it's time to paint.
	::RedrawWindow(m_hWndCap, NULL, NULL, RDW_INVALIDATE | RDW_FRAME | RDW_ERASENOW);
}