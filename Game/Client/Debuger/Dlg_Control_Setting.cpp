// Dlg_Control_Setting.cpp : implementation file
//

#include "stdafx.h"
#include "Debuger.h"
#include "Dlg_Control_Setting.h"
#include ".\dlg_control_setting.h"
#include "Global.h"
#include "TDGameInterface.h"
#include "TDVariable.h"

// CDlg_Control_Setting dialog

IMPLEMENT_DYNAMIC(CDlg_Control_Setting, CResizableDialog)
CDlg_Control_Setting::CDlg_Control_Setting(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CDlg_Control_Setting::IDD, pParent)
{
}

CDlg_Control_Setting::~CDlg_Control_Setting()
{
}

void CDlg_Control_Setting::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg_Control_Setting, CResizableDialog)
	ON_BN_CLICKED(IDC_BUTTON_TO_800X600, OnBnClickedButtonTo800x600)
	ON_BN_CLICKED(IDC_BUTTON_TO_1024X768, OnBnClickedButtonTo1024X768)
	ON_BN_CLICKED(IDC_CHECK_FULLSCREEN, OnBnClickedCheckFullscreen)
END_MESSAGE_MAP()


// CDlg_Control_Setting message handlers

void CDlg_Control_Setting::OnBnClickedButtonTo800x600()
{
	// TODO: Add your control notification handler code here
	g_pVariableSystem->SetVariable("Screen_Resoution", "800,600");
}

void CDlg_Control_Setting::OnBnClickedButtonTo1024X768()
{
	// TODO: Add your control notification handler code here
	g_pVariableSystem->SetVariable("Screen_Resoution", "1024,768");
}

BOOL CDlg_Control_Setting::OnInitDialog()
{
	CResizableDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	AddAnchor(IDC_STATIC_RESOLUATION, TOP_CENTER);
	AddAnchor(IDC_BUTTON_TO_800X600, TOP_CENTER);
	AddAnchor(IDC_BUTTON_TO_1024X768, TOP_CENTER);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg_Control_Setting::OnBnClickedCheckFullscreen()
{
	// TODO: Add your control notification handler code here
	if(BST_CHECKED == IsDlgButtonChecked(IDC_CHECK_FULLSCREEN))
	{
		g_pVariableSystem->SetVariable("Screen_FullScreen", "1");
		::EnableWindow(GetDlgItem(IDC_BUTTON_TO_800X600)->GetSafeHwnd(), FALSE);
		::EnableWindow(GetDlgItem(IDC_BUTTON_TO_1024X768)->GetSafeHwnd(), FALSE);
	}
	else
	{
		g_pVariableSystem->SetVariable("Screen_FullScreen", "0");
		::EnableWindow(GetDlgItem(IDC_BUTTON_TO_800X600)->GetSafeHwnd(), TRUE);
		::EnableWindow(GetDlgItem(IDC_BUTTON_TO_1024X768)->GetSafeHwnd(), TRUE);
	}
}
