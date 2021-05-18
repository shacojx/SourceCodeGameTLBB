// Dlg_Control_Console.cpp : implementation file
//

#include "stdafx.h"
#include "Debuger.h"
#include "Dlg_Control_Console.h"
#include ".\dlg_control_console.h"
#include "Global.h"
#include "TDGameInterface.h"


CDlg_Control_Console* g_theConsoleDlg = NULL;

// CDlg_Control_Console dialog
BEGIN_MESSAGE_MAP(CDlg_Control_Console::CInnerEdit, CEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()

void CDlg_Control_Console::CInnerEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(nChar == 13 && g_theConsoleDlg)
	{
		g_theConsoleDlg->OnBnClickedButtonSend();
	}
	// TODO: Add your message handler code here and/or call default
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

// CDlg_Control_Console dialog
IMPLEMENT_DYNAMIC(CDlg_Control_Console, CDialog)
CDlg_Control_Console::CDlg_Control_Console(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CDlg_Control_Console::IDD, pParent)
{
	g_theConsoleDlg = this;
}

CDlg_Control_Console::~CDlg_Control_Console()
{
}

void CDlg_Control_Console::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CONSOLE, m_comConsole);
}


BEGIN_MESSAGE_MAP(CDlg_Control_Console, CResizableDialog)
	ON_BN_CLICKED(IDC_BUTTON_SEND, OnBnClickedButtonSend)
END_MESSAGE_MAP()


// CDlg_Control_Console message handlers

BOOL CDlg_Control_Console::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	AddAnchor(IDC_COMBO_CONSOLE, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_BUTTON_SEND, TOP_LEFT, TOP_RIGHT);

	COMBOBOXINFO cbInfo;
	ZeroMemory(&cbInfo, sizeof(COMBOBOXINFO));
	cbInfo.cbSize = sizeof(COMBOBOXINFO);

	HWND hCom = GetDlgItem(IDC_COMBO_CONSOLE)->GetSafeHwnd();
	GetComboBoxInfo(hCom, &cbInfo);
	m_theInnnerEdit.SubclassWindow(cbInfo.hwndItem);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg_Control_Console::OnBnClickedButtonSend()
{
	// TODO: Add your control notification handler code here
	CString strCmd;
	m_comConsole.GetWindowText(strCmd);
	m_theInnnerEdit.SetSel(0, -1);

	g_pInterface->GM_SendCommand(strCmd);

	//Add to history
	INT nTotalCount = m_comConsole.GetCount();
	for(int i=0; i<nTotalCount; i++)
	{
		CString strTemp;
		m_comConsole.GetLBText(i, strTemp);
		if(strTemp == strCmd) return;

	}

	m_comConsole.InsertString(0, strCmd);
}
