// SplitWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Helper.h"
#include "SplitWnd.h"


// CSplitWnd

IMPLEMENT_DYNAMIC(CSplitWnd, CStatic)
CSplitWnd::CSplitWnd()
{
	m_bCapture = FALSE;
	m_nLastPos = INVALID_POS;
	m_nPosOffset = 0;
}

CSplitWnd::~CSplitWnd()
{
}


BEGIN_MESSAGE_MAP(CSplitWnd, CStatic)
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CSplitWnd message handlers
void CSplitWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CStatic::OnPaint()

	CRect rectClient;
	GetClientRect(&rectClient);


	dc.FillSolidRect(&rectClient, RGB(200, 200, 200));
	CRect rect2(rectClient);
	
	rect2.InflateRect(-2, -2, -2, -2);
	dc.FillSolidRect(&rect2, RGB(200, 220, 220));

	dc.Draw3dRect(&rectClient, RGB(255, 255, 255), RGB(128, 128, 128));

	if(m_bCapture)
	{
		CPoint point;
		GetCursorPos(&point);

		::ScreenToClient(GetSafeHwnd(), &point);

		CPen pen(PS_SOLID, 1, RGB(55, 55, 55));
		CPen* pOldPen = dc.SelectObject(&pen);

		int nOldRop = dc.SetROP2(R2_XORPEN);

		if(m_nLastPos != INVALID_POS)
		{
			dc.MoveTo(m_nLastPos-1, 0);
			dc.LineTo(m_nLastPos-1, rectClient.Height());

			dc.MoveTo(m_nLastPos+1, 0);
			dc.LineTo(m_nLastPos+1, rectClient.Height());
		}

		dc.MoveTo(point.x-1, 0);
		dc.LineTo(point.x-1, rectClient.Height());
		dc.MoveTo(point.x+1, 0);
		dc.LineTo(point.x+1, rectClient.Height());
		m_nLastPos = point.x;

		dc.SetROP2(nOldRop);
		dc.SelectObject(pOldPen);
	}
	else
	{
		if(INVALID_POS != m_nLastPos)
		{
			//CPen pen(PS_SOLID, 1, RGB(55, 55, 55));
			//CPen* pOldPen = dc.SelectObject(&pen);

			//int nOldRop = dc.SetROP2(R2_XORPEN);

			//dc.MoveTo(m_nLastPos-1, 0);
			//dc.LineTo(m_nLastPos-1, rectClient.Height());

			//dc.MoveTo(m_nLastPos+1, 0);
			//dc.LineTo(m_nLastPos+1, rectClient.Height());

			//dc.SetROP2(nOldRop);
			//dc.SelectObject(pOldPen);

			m_nLastPos = INVALID_POS;
		}
	}

}

BOOL CSplitWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	::SetCursor(AfxGetApp()->LoadStandardCursor(MAKEINTRESOURCE(IDC_SIZEWE)));
	return TRUE;
	
//	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}

void CSplitWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_bCapture)
	{
		CPoint pointParent;
		GetCursorPos(&pointParent);
		::ScreenToClient(GetParent()->GetSafeHwnd(), &pointParent);

		CRect rectParent;
		GetParent()->GetClientRect(&rectParent);

		if(pointParent.x < 100 || pointParent.x > 300)
		{
			return;
		}

		Invalidate();
	}

	CStatic::OnMouseMove(nFlags, point);
}

void CSplitWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_bCapture)
	{
		ReleaseCapture();
		m_bCapture = FALSE;

		Invalidate();
		
		NMHDR nmNotify;
		ZeroMemory(&nmNotify, sizeof(NMHDR));

		nmNotify.idFrom = GetDlgCtrlID();
		nmNotify.hwndFrom = GetSafeHwnd();
		nmNotify.code = NM_SW_CHANGED;

		m_nPosOffset = m_nLastPos == INVALID_POS ? 0 : m_nLastPos;
		::SendMessage(GetParent()->GetSafeHwnd(), WM_NOTIFY, (WPARAM)GetDlgCtrlID(), (LPARAM)&nmNotify);
	}

	CStatic::OnLButtonUp(nFlags, point);
}

void CSplitWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SetCapture();
	m_bCapture = TRUE;
	m_nPosOffset = 0;

	CStatic::OnLButtonDown(nFlags, point);
}

