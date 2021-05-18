#include "StdAfx.h"
#include "CaptionButtonDialog.h"

CCaptionButtonDlg::CCaptionButtonDlg(UINT id, CWnd* pParent) : 
	CDialog(id, pParent)
{
	m_nPressed = -1;
}

CCaptionButtonDlg::~CCaptionButtonDlg()
{
	Clear();
}

BEGIN_MESSAGE_MAP(CCaptionButtonDlg, CDialog)
	ON_WM_NCHITTEST()
	ON_WM_NCPAINT()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////
// remove all the buttons
void CCaptionButtonDlg::Clear()
{
	while (m_buttons.size())
		RemoveButton(0);
}

/////////////////////////////////////////////////////////////
// remove the button at given position
void CCaptionButtonDlg::RemoveButton(int index)
{
	ASSERT(index>=0 && index<(int)m_buttons.size());

	std::vector<_button>::iterator	it = m_buttons.begin();
	for (int i=0; i<index; i++)
		it++;

	//destroy the image list handle
	ImageList_Destroy(it->himl);
	//erase from the list
	m_buttons.erase(it);

}

////////////////////////////////////////////////////////////////
// append a button
// return the count of buttons
int CCaptionButtonDlg::AddButton(UINT uID, int cx, int cy, HIMAGELIST himl, LPCTSTR lpszHint)
{
	if (m_tooltips.m_hWnd==NULL)
	{
		m_tooltips.CreateEx(NULL, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
			WS_EX_TOOLWINDOW|WS_EX_TOPMOST);
		ASSERT(m_tooltips.m_hWnd);

		TOOLINFO	info;
		memset(&info, 0, sizeof(info));
		info.cbSize		= sizeof(info);
		info.uFlags		= TTF_IDISHWND;
		info.hwnd		= m_hWnd;
		info.uId		= (UINT)m_hWnd;
		info.lpszText	= LPSTR_TEXTCALLBACK;
		m_tooltips.SendMessage(TTM_ADDTOOL, 0, (LPARAM)&info);

		m_tooltips.Activate(TRUE);
	}


	_button	btn;
	memset(&btn, 0, sizeof(btn));
	btn.uID = uID;
	btn.cx	= cx;
	btn.cy	= cy;
	btn.himl= himl;
	btn.uStatus = CAPTION_BTN_NORMAL;
	if (lpszHint)
	{
		_tcsncpy(btn.szHint, lpszHint, 80);
	}

	m_buttons.push_back(btn);

	return m_buttons.size();
}

///////////////////////////////////////////////////////////////////////////////////
// check or uncheck the button at postion index
void CCaptionButtonDlg::CheckButton(int index, bool bCheck)
{
	_button &btn = m_buttons[index];
	btn.uStatus = bCheck ? CAPTION_BTN_CHECKED : CAPTION_BTN_NORMAL;

	//Update visual effect
	if (m_hWnd)
	{
		//force repaint
		SendMessage(WM_NCPAINT, 1);
	}
}

/////////////////////////////////////////////////////////////////////////////////
// if the button at position index checked
bool CCaptionButtonDlg::IsButtonChecked(int index)
{
	_button &btn = m_buttons[index];

	return (btn.uStatus == CAPTION_BTN_CHECKED);
}

/////////////////////////////////////////////////////////////////////////////
// enable/disable button at given position
void CCaptionButtonDlg::EnableButton(int index, bool bEnable)
{
	_button &btn = m_buttons[index];
	btn.uStatus = bEnable ? CAPTION_BTN_NORMAL : CAPTION_BTN_DISABLED;

	//Update visual effect
	if (m_hWnd)
	{
		//force repaint
		SendMessage(WM_NCPAINT, 1);
	}
}

bool CCaptionButtonDlg::IsButtonEnabled(int index)
{
	_button &btn = m_buttons[index];

	return (btn.uStatus != CAPTION_BTN_DISABLED);
}

/////////////////////////////////////////////////////////////////////////////////
// Get the zero based index of the button, given the button ID
int CCaptionButtonDlg::GetButtonIndex(UINT uID)
{
	for (int i=0; i<(int)m_buttons.size(); i++)
	{
		if (m_buttons[i].uID == uID)
			return i;
	}

	return -1;
}

//////////////////////////////////////////////////////////////////////////////////
// get the topleft corner of the given button, in window coordinate
// override this function in the derived class to specify button position
// this default implementation only works for tool window without any existing caption buttons
POINT CCaptionButtonDlg::GetButtonPos(int index)
{
	//get the window rect
	CRect	rcWnd;
	GetWindowRect(&rcWnd);
	rcWnd.OffsetRect(-rcWnd.TopLeft());

	rcWnd.DeflateRect(2, 2);

	//locate the top right base point
	CPoint	pt(rcWnd.right, rcWnd.top);
	pt.x -= 19*3;
	pt.y += 4;

	switch (index)
	{
	case 0://autohide pin
		pt.x -= 16;
		break;
	}

	return pt;
}

///////////////////////////////////////////////////////////////////////////////////
// get the rect of given button in window coordinate
RECT CCaptionButtonDlg::GetButtonRect(int index)
{
	_button btn = m_buttons[index];

	POINT	pt = GetButtonPos(index);
	RECT	rc;
	rc.left = pt.x;
	rc.top  = pt.y;
	rc.right = rc.left + btn.cx;
	rc.bottom = rc.top + btn.cy;

	return rc;
}

////////////////////////////////////////////////////////////////////////////////////
// get button under point, in window coordinate
int CCaptionButtonDlg::GetButtonAtPos(POINT pt)
{
	for (int i=0; i<(int)m_buttons.size(); i++)
	{
		CRect	rc = GetButtonRect(i);
		if (rc.PtInRect(pt))
			return i;
	}
	return -1;
}

////////////////////////////////////////////////////////////////////////////////////
// WM_NCHITTEST	 message handler
UINT CCaptionButtonDlg::OnNcHitTest(CPoint pt)
{
	BOOL bHandled = FALSE;
	LRESULT hit = CDialog::OnNcHitTest(pt);

	UpdateStatus();

	CRect	rcWnd;
	GetWindowRect(&rcWnd);
	pt.Offset(-rcWnd.TopLeft());

	int nButton = GetButtonAtPos(pt);
	if (nButton != -1 && m_buttons[nButton].uStatus!=CAPTION_BTN_DISABLED)
	{
		bHandled = TRUE;
	}

	RelayEvent();

	return hit;
}

///////////////////////////////////////////////////////////////////////////////////
// WM_NCPAINT message handler
// draw the buttons
VOID CCaptionButtonDlg::OnNcPaint(VOID)
{
	CDialog::OnNcPaint();

	CDC *dc = FromHandle(m_hWnd)->GetWindowDC();

	for (int i=0; i<(int)m_buttons.size(); i++)
	{
		_button btn = m_buttons[i];
		POINT	pt = GetButtonPos(i);

		ImageList_Draw(btn.himl, btn.uStatus, dc->m_hDC, pt.x, pt.y, ILD_NORMAL);
	}

	::ReleaseDC(m_hWnd, dc->m_hDC);
}

///////////////////////////////////////////////////////////////////////////////////
// WM_NCLBUTTONDOWN messange handler
// if pressed on a button, SetCapture if not already captured
// and push the button
VOID CCaptionButtonDlg::OnNcLButtonDown(UINT hit, CPoint pt)
{
	BOOL bHandled = FALSE;

	CRect	rcWnd;
	GetWindowRect(&rcWnd);
	pt.Offset(-rcWnd.TopLeft());

	int nButton = GetButtonAtPos(pt);
	if (nButton != -1)
	{
		_button &btn = m_buttons[nButton];
		if (btn.uStatus != CAPTION_BTN_DISABLED)
		{
			if (::GetCapture()!=m_hWnd)
				SetCapture();

			m_nPressed = nButton;
			bHandled = TRUE;

			if (btn.uStatus != CAPTION_BTN_CHECKED)
				btn.uStatus = CAPTION_BTN_PUSHED;

			//force repaint
			SendMessage(WM_NCPAINT, 1);
		}
	}

	if(!bHandled)
	{
		CDialog::OnNcLButtonDown(hit, pt);
	}
}

///////////////////////////////////////////////////////////////////////////////////
// WM_MOUSEMOVE message handler
// valid only if mouse captured
VOID CCaptionButtonDlg::OnMouseMove(UINT hit, CPoint pt)
{
	BOOL bHandled = FALSE;

	if (::GetCapture()==m_hWnd && m_nPressed != -1)
	{
		bHandled = TRUE;
		UpdateStatus();
	}

	if(!bHandled)
	{
		CDialog::OnMouseMove(hit, pt);
	}
}

///////////////////////////////////////////////////////////////////////////////
// WM_LBUTTONUP message handler
// valid only if mouse captured
// fire button clicked event if one button clicked
VOID CCaptionButtonDlg::OnLButtonUp(UINT hit, CPoint pt)
{
	BOOL bHandled = FALSE;
	if (::GetCapture()==m_hWnd && m_nPressed != -1)
	{
		bHandled = TRUE;
		::ReleaseCapture();

		CPoint	pt;
		::GetCursorPos(&pt);
		CRect	rcWnd;
		GetWindowRect(&rcWnd);
		pt.Offset(-rcWnd.TopLeft());

		int nButton = GetButtonAtPos(pt);
		if (m_nPressed != -1 && nButton == m_nPressed)
		{
			//fire click event
			OnCaptionButtonClicked(nButton);
		}

		m_nPressed = -1;
		//force repaint
		SendMessage(WM_NCPAINT, 1);
	}

	if(!bHandled)
	{
		CDialog::OnLButtonUp(hit, pt);
	}
}

///////////////////////////////////////////////////////////////////////////////
// WM_ACTIVATE messsage handler
// force repaint of non-client area
VOID CCaptionButtonDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	SendMessage(WM_NCPAINT);

	CDialog::OnActivate(nState, pWndOther, bMinimized);
}

/////////////////////////////////////////////////////////////////////////////////////
// fire button cliced event
// can be override in derived class
// the default implementation send WM_COMMAND with button ID
void CCaptionButtonDlg::OnCaptionButtonClicked(int index)
{
	_button &btn = m_buttons[index];
	SendMessage(WM_COMMAND, MAKELONG(btn.uID, 0), 0);

}

///////////////////////////////////////////////////////////////////////////
// update all button status and repaint
void CCaptionButtonDlg::UpdateStatus()
{
	CPoint	pt;
	::GetCursorPos(&pt);
	CRect	rcWnd;
	GetWindowRect(&rcWnd);
	pt.Offset(-rcWnd.TopLeft());

	bool	bRepaint = false;
	for (int i=0; i<(int)m_buttons.size(); i++)
	{
		_button &btn = m_buttons[i];

		if (btn.uStatus != CAPTION_BTN_DISABLED)
		{
			CRect	rc = GetButtonRect(i);
			UINT	uNewStatus;
			if (rc.PtInRect(pt))
			{
				if (btn.uStatus==CAPTION_BTN_CHECKED)
					uNewStatus = CAPTION_BTN_CHECKED;
				else if (m_nPressed == i) //mouse pressed in button
					uNewStatus = CAPTION_BTN_PUSHED;
				else
					uNewStatus = CAPTION_BTN_HOVER;
			}
			else
			{
				if (btn.uStatus!=CAPTION_BTN_CHECKED) //If old status is not checked, set it normal
					uNewStatus = CAPTION_BTN_NORMAL;
				else
					uNewStatus = CAPTION_BTN_CHECKED;
			}

			if (btn.uStatus != uNewStatus)//repaint only if status changed
			{
				bRepaint = true;
				btn.uStatus = uNewStatus;
			}
		}
	}

	if (bRepaint)
	{
		SendMessage(WM_NCPAINT, 1);
	}
}

///////////////////////////////////////////////////////////////////////////////////
// tooltip processing
void CCaptionButtonDlg::RelayEvent()
{
	if(!m_tooltips.GetSafeHwnd()) return;

	CPoint	pt;
	::GetCursorPos(&pt);
	CRect	rcWnd;
	GetWindowRect(&rcWnd);
	pt.Offset(-rcWnd.TopLeft());

	MSG	msg;
	memset(&msg, 0, sizeof(msg));
	msg.hwnd		= m_hWnd;
	msg.message		= WM_MOUSEMOVE;
	msg.lParam		= MAKELONG(pt.x, pt.y);
	msg.pt			= pt;

	static nLastButton = -1;
	int nButton = GetButtonAtPos(pt);
	if (nButton != nLastButton)//change hovered button
	{
		m_tooltips.Pop();
		nLastButton = nButton;
	}

	m_tooltips.RelayEvent(&msg);
}


BOOL CCaptionButtonDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class
	LPNMHDR pnmh = (LPNMHDR) lParam;

	if(pnmh->code == TTN_GETDISPINFO )
	{
		NMTTDISPINFO *lptt = (NMTTDISPINFO *)(LPNMHDR)lParam;

		CPoint	pt;
		::GetCursorPos(&pt);
		CRect	rcWnd;
		GetWindowRect(&rcWnd);
		pt.Offset(-rcWnd.TopLeft());

		int nButton = GetButtonAtPos(pt);
		if (nButton != -1)
		{
			_button &btn = m_buttons[nButton];

			if (btn.uStatus != CAPTION_BTN_DISABLED)
				strncpy(lptt->szText, btn.szHint, 80);
		}

		lptt->uFlags = TTF_IDISHWND;
		lptt->lpszText = lptt->szText;
	}

	return CDialog::OnNotify(wParam, lParam, pResult);
}
