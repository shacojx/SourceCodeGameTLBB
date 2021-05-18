// Dlg_Control_InfoTrack.cpp : implementation file
//

#include "stdafx.h"
#include "Debuger.h"
#include "Dlg_Control_InfoTrack.h"
#include ".\dlg_control_infotrack.h"


// CInfoTrack

BEGIN_MESSAGE_MAP(CDlg_Control_InfoTrack::CInfoTrack, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CDlg_Control_InfoTrack::CInfoTrack::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CStatic::OnPaint() for painting messages

	RECT rect;
	GetClientRect(&rect);

	CDC dcBmp;
	dcBmp.CreateCompatibleDC(&dc);
	CBitmap* pOld = (CBitmap*)(dcBmp.SelectObject(&(m_pParent->m_bmpInfo)));
	
	dc.BitBlt(0, 0, rect.right-rect.left, rect.bottom-rect.top, &dcBmp, 0, 0, SRCCOPY);

	dcBmp.SelectObject(pOld);
	dcBmp.DeleteDC();
}

// CDlg_Control_InfoTrack dialog

IMPLEMENT_DYNAMIC(CDlg_Control_InfoTrack, CResizableDialog)
CDlg_Control_InfoTrack::CDlg_Control_InfoTrack(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CDlg_Control_InfoTrack::IDD, pParent)
{
}

CDlg_Control_InfoTrack::~CDlg_Control_InfoTrack()
{
}

void CDlg_Control_InfoTrack::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg_Control_InfoTrack, CResizableDialog)
END_MESSAGE_MAP()


// CDlg_Control_InfoTrack message handlers
void CDlg_Control_InfoTrack::Reset(INT nWidth, INT nHeight)
{
	CDC* pDC = GetDC();
	m_bmpInfo.DeleteObject();
	m_bmpInfo.CreateCompatibleBitmap(pDC, nWidth, nHeight);

	CDC dcBmp;
	dcBmp.CreateCompatibleDC(pDC);
	CBitmap* pOld = (CBitmap*)(dcBmp.SelectObject(&m_bmpInfo));

	dcBmp.FillSolidRect(0, 0, nWidth, nHeight, RGB(255, 255, 255));

	dcBmp.SelectObject(pOld);
	ReleaseDC(pDC);

	m_ptLastPoint.x = m_ptLastPoint.y = -1;
}

void CDlg_Control_InfoTrack::AddPoint(INT nX, INT nY)
{
	CDC* pDC = GetDC();
	
	CDC dcBmp;
	dcBmp.CreateCompatibleDC(pDC);
	CBitmap* pOld = (CBitmap*)(dcBmp.SelectObject(&m_bmpInfo));

	if(m_ptLastPoint.x < 0 || m_ptLastPoint.y < 0)
	{
		dcBmp.SetPixel(nX, nY, RGB(0, 0, 0));
	}
	else
	{
		dcBmp.MoveTo(m_ptLastPoint.x, m_ptLastPoint.y);
		dcBmp.LineTo(nX, nY);
	}

	m_ptLastPoint.x = nX;
	m_ptLastPoint.y = nY;

	dcBmp.SelectObject(pOld);
	ReleaseDC(pDC);

	m_staInfoTrack.InvalidateRect(0, FALSE);
}

BOOL CDlg_Control_InfoTrack::OnInitDialog()
{
	CResizableDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	AddAnchor(IDC_STATIC_INFO, TOP_LEFT, BOTTOM_RIGHT);

	m_staInfoTrack.SubclassWindow(GetDlgItem(IDC_STATIC_INFO)->GetSafeHwnd());
	m_staInfoTrack.m_pParent = this;

	Reset(800, 600);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
