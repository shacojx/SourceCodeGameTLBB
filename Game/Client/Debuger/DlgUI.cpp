// DlgUI.cpp : implementation file
//

#include "stdafx.h"
#include "Debuger.h"
#include "DlgUI.h"
#include ".\dlgui.h"
#include "TDGameInterface.h"
#include "Global.h"
#include "Dlg_Control_Console.h"
#include "Dlg_Control_SceneInfo.h"
#include "Dlg_Control_Setting.h"
#include "Dlg_Control_InfoTrack.h"
// CDlgUI dialog

IMPLEMENT_DYNAMIC(CDlgUI, CResizableDialog)
CDlgUI::CDlgUI(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUI::IDD, pParent)
{
}

CDlgUI::~CDlgUI()
{
}

void CDlgUI::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgUI, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgUI message handlers

BOOL CDlgUI::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CRect rect;
	GetClientRect(&rect);

	//Create our RollupCtrl into DialogBar
	m_wndRollupCtrl.Create(WS_VISIBLE|WS_CHILD, rect, this, 2);	

	static CDlg_Control_Console theDlg_Console;
	m_pConsoleDlg = &theDlg_Console;
	m_pConsoleDlg->Create(MAKEINTRESOURCE(IDD_DIALOG_CONTROL_CONSOLE), &m_wndRollupCtrl);
	AddPage("控制台", m_pConsoleDlg);

	static CDlg_Control_SceneInfo theDlg_Info;
	m_pSceneInfoDlg = &theDlg_Info;
	m_pSceneInfoDlg->Create(MAKEINTRESOURCE(IDD_DIALOG_CONTROL_SCENEINFO), &m_wndRollupCtrl);
	AddPage("场景", m_pSceneInfoDlg);

	static CDlg_Control_Setting theDlg_Setting;
	m_pSettingDlg = &theDlg_Setting;
	m_pSettingDlg->Create(MAKEINTRESOURCE(IDD_DIALOG_CONTROL_SETTING), &m_wndRollupCtrl);
	AddPage("设置", m_pSettingDlg);

	static CDlg_Control_InfoTrack theDlg_InfoTrack;
	m_pInfotrackDlg = &theDlg_InfoTrack;
	m_pInfotrackDlg->Create(MAKEINTRESOURCE(IDD_DIALOG_CONTROL_INFOTRACK), &m_wndRollupCtrl);
	AddPage("轨迹", m_pInfotrackDlg);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgUI::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(m_wndRollupCtrl.GetSafeHwnd())
	{
		m_wndRollupCtrl.MoveWindow(0, 0, cx, cy);
	}
}

void CDlgUI::AddPage(LPCTSTR szName, CDialog* pDlg)
{
	if(m_wndRollupCtrl.GetSafeHwnd())
	{
		m_wndRollupCtrl.InsertPage(szName, pDlg);
	}
}

HWND CDlgUI::GetRollupCtrl(void)
{
	return m_wndRollupCtrl.GetSafeHwnd();
}

