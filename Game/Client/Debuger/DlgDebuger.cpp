// DlgDebuger.cpp : implementation file
//

#include "stdafx.h"
#include "Debuger.h"
#include "DlgDebuger.h"
#include ".\dlgdebuger.h"
#include "Global.h"
#include "TDGfxSystem.h"
#include "TDKernel.h"
#include "Global.h"
// CDlgDebuger dialog

IMPLEMENT_DYNAMIC(CDlgDebuger, CResizableDialog)
CDlgDebuger::CDlgDebuger(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CDlgDebuger::IDD, pParent)
{
	g_pDlgDebug = this;
	m_pCaptionBtn = new CCaptionButton;
}

CDlgDebuger::~CDlgDebuger()
{
	delete m_pCaptionBtn;
	m_pCaptionBtn = NULL;
}

void CDlgDebuger::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MAIN, m_tabMain);
}

DECLARE_USER_MESSAGE(WMU_HITCAPBUTTON)

BEGIN_MESSAGE_MAP(CDlgDebuger, CResizableDialog)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, OnTcnSelchangeTabMain)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SYSCOMMAND()
	ON_REGISTERED_MESSAGE(WMU_HITCAPBUTTON, OnWMU_Hitcapbutton)
END_MESSAGE_MAP()


// CDlgDebuger message handlers
BOOL CDlgDebuger::OnInitDialog()
{
	CResizableDialog::OnInitDialog();

	// TODO: 在此添加额外的初始化代码
	AddAnchor(IDC_TAB_MAIN, TOP_LEFT, BOTTOM_RIGHT);

	//屏蔽关闭按钮
	GetSystemMenu(FALSE)->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND|MF_DISABLED|MF_GRAYED);

	// get desktop size
	CRect rc;
	SystemParametersInfo(SPI_GETWORKAREA, NULL, &rc, NULL);
	rc.right = 256;
	SetMaximizedRect(rc);
	
	CRect rect;
	GetClientRect(&rect);
	MoveWindow(10, 150, rect.Width(), rect.Height(), TRUE);
	
	SetSizeGripVisibility(FALSE);
	UpdateSizeGrip();

	//创建字窗口
	CreateSubWindow();

	//Hook
	CCaptionButton::InitCapBtn(g_pGfxSystem->GetWindow());
	m_pCaptionBtn->SetBmpID(1, IDB_BITMAP_LAMPON, IDB_BITMAP_LAMPOFF, TRUE);
	m_pCaptionBtn->ChangeButtonState();

	::SetActiveWindow(g_pGfxSystem->GetWindow());

	m_pCaptionBtn->ChangeButtonState();

	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CDlgDebuger::OnSize(UINT nType, int cx, int cy)
{
	CResizableDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(m_tabMain.GetSafeHwnd())
	{
		CRect rect;
		m_tabMain.GetClientRect(&rect);
		m_tabMain.AdjustRect(FALSE, &rect);

		rect.top += 5;

		if(m_dlgProfile.GetSafeHwnd()) m_dlgProfile.MoveWindow(&rect);
		if( m_dlgUI.GetSafeHwnd() ) m_dlgUI.MoveWindow( &rect );
	}

	if(SIZE_MAXIMIZED == nType)
	{
		tGfxSystem* pGfx = (tGfxSystem*)g_pKernel->GetNode(_T("bin\\gfx"));
		if(!pGfx) return;

		HWND hMainWnd = pGfx->GetWindow();
		if(!hMainWnd) return;

		CRect rc;
		SystemParametersInfo(SPI_GETWORKAREA, NULL, &rc, NULL);
		rc.left = cx+6;

		::MoveWindow( hMainWnd, rc.left, rc.top, rc.Width(), rc.Height(), TRUE);
		::SendMessage(hMainWnd, WM_EXITSIZEMOVE, 0, 0);
	}
}

void CDlgDebuger::CreateSubWindow(void)
{
	m_tabMain.InsertItem(0, _T("效率"));
	m_tabMain.InsertItem(1, _T("控制"));

	CRect rect;
	m_tabMain.GetClientRect(&rect);
	m_tabMain.AdjustRect(FALSE, &rect);

	rect.top += 5;

	m_dlgProfile.Create(IDD_DIALOG_PROFILE, &m_tabMain);
	m_dlgProfile.MoveWindow(&rect);

	m_dlgUI.Create( IDD_DIALOG_UI, &m_tabMain );
	m_dlgUI.MoveWindow( &rect );

	m_dlgProfile.ShowWindow(SW_SHOW);
	m_dlgUI.ShowWindow(SW_HIDE);
	
}
void CDlgDebuger::OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int nSel = m_tabMain.GetCurSel();

	switch(nSel)
	{
	case 0:
		m_dlgProfile.ShowWindow(SW_SHOW);
		m_dlgUI.ShowWindow(SW_HIDE);
		break;

	case 1:
		m_dlgProfile.ShowWindow(SW_HIDE);
		m_dlgUI.ShowWindow(SW_SHOW);
		break;

	default:
		break;
	}


	*pResult = 0;
}

void CDlgDebuger::OnClose()
{
	// TODO: Add your message handler code here and/or call default

//	CDialog::OnClose();
}

void CDlgDebuger::OnDestroy()
{
	CResizableDialog::OnDestroy();

	// TODO: Add your message handler code here
	m_dlgProfile.DestroyWindow();
	m_dlgUI.DestroyWindow();
}

void CDlgDebuger::OnMinimizeWindow(void)
{
	ShowWindow(SW_HIDE);
	
	m_pCaptionBtn->ChangeButtonState();
}

void CDlgDebuger::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: Add your message handler code here and/or call default
	if ( nID == SC_MINIMIZE )
	{
		OnMinimizeWindow();
		return;
	}

	CResizableDialog::OnSysCommand(nID, lParam);
}

LRESULT CDlgDebuger::OnWMU_Hitcapbutton(WPARAM wParam, LPARAM lParam)
{
	switch ((UINT)wParam) 
	{
	case 1:	// Button #1 has been hit:
		OnCapButton((BOOL)lParam);
		break;
	default:
		return 0;;
	}
	return 0; // Allways return 0(LRESULT->void)
}

void CDlgDebuger::OnCapButton(BOOL bBtnState)
{
	if(bBtnState)
	{
		ShowWindow(SW_SHOW);
	}
	else
	{
		ShowWindow(SW_HIDE);
	}
}
