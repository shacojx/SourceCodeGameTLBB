// DlgView.cpp : implementation file
//

#include "stdafx.h"
#include "Debuger.h"
#include "DlgView.h"
#include ".\dlgview.h"
#include "Global.h"
#include "TDKernel.h"
#include "TDGfxSystem.h"
#include "TDMath.h"
#include "TDException.h"
#include "TDInputSystem.h"
#include "TDMath.h"
#include "TDUtil.h"

IMPLEMENT_DYNAMIC(CDlgView, CResizableDialog)
CDlgView::CDlgView(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CDlgView::IDD, pParent)
{
}

CDlgView::~CDlgView()
{
}

void CDlgView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_ZOOM, m_sliderZoom);
	DDX_Control(pDX, IDC_SLIDER_DIRECTION, m_sliderDirection);
}


BEGIN_MESSAGE_MAP(CDlgView, CResizableDialog)
	ON_WM_HSCROLL()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgView message handlers

BOOL CDlgView::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	AddAnchor(IDC_STATIC_CAMERA_GROUP, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_SLIDER_ZOOM, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_STATIC_ZOOM_IN, TOP_RIGHT);

	AddAnchor(IDC_SLIDER_DIRECTION, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_STATIC_DIRECTION_360, TOP_RIGHT);

	AddAnchor(IDC_BUTTON_UP, TOP_CENTER);
	AddAnchor(IDC_BUTTON_DOWN, TOP_CENTER);
	AddAnchor(IDC_BUTTON_LEFT, TOP_CENTER);
	AddAnchor(IDC_BUTTON_RIGHT, TOP_CENTER);

	AddAnchor(IDC_STATIC_INPUT_GROUP, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_EDIT_INPUT, TOP_LEFT, TOP_RIGHT);

	m_sliderZoom.SetRange( 0, 100);
	m_sliderZoom.SetPos(100);
	m_sliderDirection.SetRange( 0, 360);
	m_sliderDirection.SetPos(0);
	
	m_pInputManager = (tInputSystem*)g_pKernel->GetNode(_T("bin\\input"));
	m_pRenderLayer = (tGfxSystem*)g_pKernel->GetNode(_T("bin\\gfx"));

	SetTimer(101, 10, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	int nZoomPos = m_sliderZoom.GetPos();
	m_pRenderLayer->Camera_SetZoom(nZoomPos/100.0f);

	int nDir = m_sliderDirection.GetPos();
	m_pRenderLayer->Camera_Direction(nDir * 2.0f * (float)TDU_PI / 360.0f);

	CResizableDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDlgView::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(101 == nIDEvent)
	{
		//int nZoomPos = m_sliderZoom.GetPos();
		//float fStep = 0.1f * (5-nZoomPos/25);

		//fVector2 fvPos;
		//m_pRenderLayer->Camera_GetLookAt(fvPos);
		//if((BST_PUSHED & GetDlgItem( IDC_BUTTON_UP)->SendMessage(BM_GETSTATE, 0, 0)))
		//{
		//	fvPos.y += fStep;
		//	m_pRenderLayer->Camera_SetLookAt(fvPos);
		//}
		//if((BST_PUSHED & GetDlgItem( IDC_BUTTON_DOWN)->SendMessage(BM_GETSTATE, 0, 0)))
		//{
		//	fvPos.y -= fStep;
		//	m_pRenderLayer->Camera_SetLookAt(fvPos);
		//}
		//if((BST_PUSHED & GetDlgItem( IDC_BUTTON_LEFT)->SendMessage(BM_GETSTATE, 0, 0)))
		//{
		//	fvPos.x += fStep;
		//	m_pRenderLayer->Camera_SetLookAt(fvPos);
		//}
		//if((BST_PUSHED & GetDlgItem( IDC_BUTTON_RIGHT)->SendMessage(BM_GETSTATE, 0, 0)))
		//{
		//	fvPos.x -= fStep;
		//	m_pRenderLayer->Camera_SetLookAt(fvPos);
		//}

	}
	CResizableDialog::OnTimer(nIDEvent);
}

void CDlgView::Tick(void)
{
	POINT pt = m_pInputManager->MouseGetPos();
	DWORD dwFlag = m_pInputManager->MouseGetFlag();

	TCHAR szTemp[MAX_PATH] = {0};
	_sntprintf(szTemp, MAX_PATH, 
		_T("(%d, %d)\r\n")
		_T("Flag:%d%d%d%d %d%d%d%d%d%d%d%d"), 
		pt.x, pt.y,
		dwFlag&0X800	? 1 : 0,
		dwFlag&0X400	? 1 : 0,
		dwFlag&0X200	? 1 : 0,
		dwFlag&0X100	? 1 : 0,

		dwFlag&0X80		? 1 : 0,
		dwFlag&0X40		? 1 : 0,
		dwFlag&0X20		? 1 : 0,
		dwFlag&0X10		? 1 : 0, 
		dwFlag&0X8		? 1 : 0, 
		dwFlag&0X4		? 1 : 0, 
		dwFlag&0X2		? 1 : 0, 
		dwFlag&0X1		? 1 : 0);

	CString strOld;
	GetDlgItem(IDC_EDIT_INPUT)->GetWindowText(strOld);
	if(strOld == szTemp) return;

	GetDlgItem(IDC_EDIT_INPUT)->SetWindowText(szTemp);
}
