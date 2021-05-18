// Helper.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "Helper.h"
#include "HelperSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHelperApp

BEGIN_MESSAGE_MAP(CHelperApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CHelperApp 构造

CHelperApp::CHelperApp() :
	m_pMainDialog(0)
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

CHelperApp::~CHelperApp()
{
}

// 唯一的一个 CHelperApp 对象

CHelperApp theApp;


// CHelperApp 初始化

BOOL CHelperApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControls()。否则，将无法创建窗口。
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	//---------------------------------------
	// ansys command line
	CString strCmdLine = m_lpCmdLine;

	CString strWinName;
	int nWinNameKey = strCmdLine.Find("-w:");
	if(nWinNameKey >= 0)
	{
		do
		{
			int nFirstDot = strCmdLine.Find("\"", nWinNameKey);
			if(nFirstDot < 0) break;

			int nSecondDot = strCmdLine.Find("\"", nFirstDot+1);
			if(nSecondDot < 0) break;

			strWinName = strCmdLine.Mid(nFirstDot+1, nSecondDot-nFirstDot-1);
		}
		while(false);
	}
	else 
	{
		// used by next key.
		nWinNameKey = 0;
	}

	int nAhchorKey = strCmdLine.Find("-a:", nWinNameKey);
	if(nAhchorKey >= 0)
	{
		do
		{
			int nFirstDot = strCmdLine.Find("\"", nAhchorKey);
			if(nFirstDot < 0) break;

			int nSecondDot = strCmdLine.Find("\"", nFirstDot+1);
			if(nSecondDot < 0) break;

			m_strAnchor = strCmdLine.Mid(nFirstDot+1, nSecondDot-nFirstDot-1);
		}
		while(false);
	}

	//---------------------------------------
	// Init help system
	new CHelperSystem;

	//---------------------------------------
	// create commucation dialog
	m_wndCommucation.Create(IDD_DIALOG_COM, 0);
	m_wndCommucation.SetWindowText(strWinName);

	//---------------------------------------
	// create main dialog
	m_pMainDialog = new CHelperDlg();

	m_pMainDialog->Create(MAKEINTRESOURCE(IDD_HELPER_DIALOG));
	m_pMainDialog->ShowWindow(SW_SHOW);

	m_pMainWnd = m_pMainDialog;

	//---------------------------------------
	// Go to anchor item
	static  HELPER_COMMAND cmd;
	ZeroMemory(&cmd, sizeof(HELPER_COMMAND));
	_snprintf(cmd.pContents, MAX_PATH, "%s", (LPCTSTR)((CHelperApp*)AfxGetApp())->GetAnchor());

	static COPYDATASTRUCT   cCopyData;
	ZeroMemory(&cCopyData, sizeof(COPYDATASTRUCT));

	cCopyData.dwData = 0;
	cCopyData.cbData = sizeof(unsigned char)*4 + strlen(cmd.pContents) + 1;
	cCopyData.lpData = &cmd;

	m_wndCommucation.SendMessage(WM_COPYDATA, 0, (LPARAM)&cCopyData);

	return TRUE;
}

int CHelperApp::ExitInstance()
{
	m_wndCommucation.DestroyWindow();

	delete m_pMainDialog; m_pMainDialog = 0;
	delete CHelperSystem::GetMe();

	return CWinApp::ExitInstance();
}

