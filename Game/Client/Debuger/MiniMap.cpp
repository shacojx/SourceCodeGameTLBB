// MiniMap.cpp : implementation file
//

#include "stdafx.h"
#include "Debuger.h"
#include "MiniMap.h"
#include ".\minimap.h"
#include "MemDc.h"
#include "TDUtil.h"
#include <math.h>
// CMiniMap

HCURSOR	CMiniMap::m_hHandArrow = NULL;

IMPLEMENT_DYNAMIC(CMiniMap, CStatic)
CMiniMap::CMiniMap()
{
	m_ptViewCenter.x = m_ptViewCenter.y = 0;
	m_fDir = 0.0f;
	m_hHandArrow = ::LoadCursor(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDC_CURSOR_HMOVE));
	m_bMouseOver = FALSE;
	m_bMouseDown = FALSE;
	m_nPanX = m_nPanY = 0;
}

CMiniMap::~CMiniMap()
{
	DestroyCursor(m_hHandArrow);
}


BEGIN_MESSAGE_MAP(CMiniMap, CStatic)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

void CMiniMap::SetMapFile(LPCTSTR szFileName, INT nWidth, INT nHeight, INT nWordWidth, INT nWorldHeight)
{
	if(m_bmpMap.m_hObject) m_bmpMap.DeleteObject();
	if(NULL == szFileName || szFileName[0] == '\0') return;

	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, szFileName, IMAGE_BITMAP, 0, 0, 
		LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE); 
	if(!hBitmap) return;

	m_strMapFile = szFileName;
	m_bmpMap.Attach(hBitmap);
	
	m_nMapWidth = nWidth;
	m_nMapHeight = nHeight;

	m_nWorldWidth = nWordWidth;
	m_nWorldHeight = nWorldHeight;

	m_fScaleWidth  = (FLOAT)m_nMapWidth/(FLOAT)m_nWorldWidth;
	m_fScaleHeight = (FLOAT)m_nMapHeight/(FLOAT)m_nWorldHeight;
}

void CMiniMap::SetViewCenter(FLOAT fX, FLOAT fY, float fDir)
{
	if(fX < 0 || fX > (FLOAT)m_nWorldWidth) return;
	if(fY < 0 || fY > (FLOAT)m_nWorldHeight) return;

	m_fPlayerPosX = fX;
	m_fPlayerPosY = fY;

	m_ptViewCenter.x = (INT)(fX*m_fScaleWidth);
	m_ptViewCenter.y = (INT)(fY*m_fScaleHeight);
	m_fDir = fDir;

	CRect rect;
	GetClientRect(&rect);

	m_rectView.SetRect(- rect.Width()/2, - rect.Height()/2, rect.Width()/2, rect.Height()/2);
	m_rectView.OffsetRect(m_ptViewCenter.x, m_ptViewCenter.y);
	m_rectView.OffsetRect(m_nPanX, m_nPanY);

}

void CMiniMap::OnPaint()
{
	CPaintDC dcOrg(this); // device context for painting
	// TODO: Add your message handler code here
	CRect rect;
	GetClientRect(&rect);
	CMemDC dc(&dcOrg, &rect);

	dc.FillSolidRect(&rect, RGB(0, 0, 0));
	if(!m_bmpMap.m_hObject) return;

	CDC dcBmp;
	dcBmp.CreateCompatibleDC(&dc);
	dcBmp.SelectObject(&m_bmpMap);

	int nStartX, nStartY;
	int nMapStartX, nMapStartY;

	if(m_rectView.left < 0 ) 
	{
		nStartX = -m_rectView.left;
		nMapStartX = 0;
	}
	else 
	{
		nStartX = 0;
		nMapStartX = m_rectView.left;
	}

	if(m_rectView.top < 0) 
	{
		nStartY = -m_rectView.top;
		nMapStartY = 0;
	}
	else 
	{
		nStartY = 0;
		nMapStartY = m_rectView.top;
	}

	//画地图
	dc.BitBlt(nStartX, nStartY, rect.Width(), rect.Height(), &dcBmp, nMapStartX, nMapStartY, SRCCOPY);
	
	//static nIndex;
	//char szTemp[32];
	//_snprintf(szTemp, 32, "%d", nIndex++);
	//dc.SetTextColor(RGB(255, 255, 255));
	//dc.SetBkMode(TRANSPARENT);
	//dc.TextOut(0, 0, szTemp, (int)strlen(szTemp));

	//画玩家自己
	//int nCenterX = rect.Width()/2-m_nPanX;
	//int nCenterY = rect.Height()/2-m_nPanY;

	//CRect rectMe(nCenterX - 3, nCenterY - 3, nCenterX + 3, nCenterY + 3);
	static CPen penMe;
	if(!penMe.m_hObject) penMe.CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
	CPen* pOldPen = dc.SelectObject(&penMe);

	//dc.MoveTo(rectMe.left, rectMe.top);
	//dc.LineTo(rectMe.left, rectMe.bottom);
	//dc.LineTo(rectMe.right, rectMe.bottom);
	//dc.LineTo(rectMe.right, rectMe.top);
	//dc.LineTo(rectMe.left, rectMe.top);


	//static CPen penTarget;
	//if(!penTarget.m_hObject) penTarget.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	//dc.SelectObject(&penTarget);


	const int nArrowLength = 15;
	const int nArrowBLength = 8;

	POINT ptStart, ptEnd;
	POINT ptArrB1, ptArrB2;

	ptStart.x = rect.Width()/2-m_nPanX;
	ptStart.y = rect.Height()/2-m_nPanY;

	ptEnd.x = (int)(ptStart.x + nArrowLength*(float)sin(m_fDir));
	ptEnd.y = (int)(ptStart.y + nArrowLength*(float)cos(m_fDir));

	ptArrB1.x = (int)(ptEnd.x + nArrowBLength * (float)sin(m_fDir + 150.0f*TDU_PI/180.0f));
	ptArrB1.y = (int)(ptEnd.y + nArrowBLength * (float)cos(m_fDir + 150.0f*TDU_PI/180.0f));

	ptArrB2.x = (int)(ptEnd.x + nArrowBLength * (float)sin(m_fDir - 150.0f*TDU_PI/180.0f));
	ptArrB2.y = (int)(ptEnd.y + nArrowBLength * (float)cos(m_fDir - 150.0f*TDU_PI/180.0f));


	dc.MoveTo(ptStart.x, ptStart.y);
	dc.LineTo(ptEnd.x, ptEnd.y);

	dc.LineTo(ptArrB1.x, ptArrB1.y);
	
	dc.MoveTo(ptEnd.x, ptEnd.y);
	dc.LineTo(ptArrB2.x, ptArrB2.y);

	dc.SelectObject(pOldPen);

}

void CMiniMap::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(GetSafeHwnd())
	{
		SetViewCenter(m_fPlayerPosX, m_fPlayerPosY, m_fDir);
	}
}

void CMiniMap::Update(void)
{
	Invalidate(FALSE);
}

void CMiniMap::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CStatic::OnMouseMove(nFlags, point);

	if(m_bMouseDown)
	{
		int nXStep = point.x - m_ptMouseDragBegin.x;
		int nYStep = point.y - m_ptMouseDragBegin.y;

		m_nPanX -= nXStep;
		m_nPanY -= nYStep;

		m_ptMouseDragBegin = point;
	}
	//if (m_bMouseOver)
	//{
	//	CRect oRect;
	//	GetClientRect(&oRect);

	//	//check if the mouse is in the rect
	//	if (oRect.PtInRect(point) == false)
	//	{
	//		m_bMouseOver = false;
	//		//Release the Mouse capture previously take
	//		ReleaseCapture();
	//		RedrawWindow();
	//		return;
	//	}
	//}else
	//{
	//	m_bMouseOver = true;
	//	RedrawWindow();
	//	//capture the mouse
	//	SetCapture();
	//}
}

BOOL CMiniMap::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default
	if (m_bMouseDown)
	{
		::SetCursor(m_hHandArrow);

		return TRUE;
	}
	else
	{
		::SetCursor(::LoadCursor(NULL, IDC_ARROW));
		return TRUE;
	}

	return FALSE;

//	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}

void CMiniMap::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_bMouseDown = TRUE;
	SetCapture();
	
	::SetCursor(m_hHandArrow);

	m_ptMouseDragBegin = point;

	CStatic::OnLButtonDown(nFlags, point);
}

void CMiniMap::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_bMouseDown = FALSE;
	ReleaseCapture();

	::SetCursor(::LoadCursor(NULL, IDC_ARROW));

	CStatic::OnLButtonUp(nFlags, point);
}

void CMiniMap::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_nPanX = m_nPanY = 0;
	CStatic::OnLButtonDblClk(nFlags, point);
}
