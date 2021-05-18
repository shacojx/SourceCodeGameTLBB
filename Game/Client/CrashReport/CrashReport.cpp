// CrashReport.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "CrashReport.h"
#include "CrashReportDlg.h"
#include "FtpTrans.h"
#include "xzip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCrashReportApp

BEGIN_MESSAGE_MAP(CCrashReportApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CCrashReportApp 构造

CCrashReportApp::CCrashReportApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CCrashReportApp 对象

CCrashReportApp theApp;

CHAR g_szCurrentDir[MAX_PATH] = {0};
CHAR g_szSmallInfoFile[MAX_PATH] = {0};
CHAR g_szBigInfoFile[MAX_PATH] = {0};
CHAR g_szDumpFile[MAX_PATH] = {0};

// CCrashReportApp 初始化

BOOL CCrashReportApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControls()。否则，将无法创建窗口。
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	/*SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CCrashReportDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用“确定”来关闭
		//对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用“取消”来关闭
		//对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	// 而不是启动应用程序的消息泵。*/
	LPTSTR lpCmdLine = this->m_lpCmdLine;

	CHAR szFileName[MAX_PATH] = {0};
	GetModuleFileName(GetModuleHandle(NULL), szFileName, MAX_PATH);
	PathRemoveFileSpec(szFileName);
	PathAddBackslash(szFileName);
	strcpy(g_szCurrentDir, szFileName);

	//this->m_pMainWnd->MessageBox(g_szCurrentDir);

	//分析命令行
	char* pMark1 = strchr(lpCmdLine, '"'); if(!pMark1) return 0;
	char* pMark2 = strchr(pMark1+1, '"');  if(!pMark2) return 0;

	char* pMark3 = strchr(pMark2+1, '"');  if(!pMark3) return 0;
	char* pMark4 = strchr(pMark3+1, '"');  if(!pMark4) return 0;

	char* pMark5 = strchr(pMark4+1, '"');  if(!pMark5) return 0;
	char* pMark6 = strchr(pMark5+1, '"');  if(!pMark6) return 0;

	*pMark2 = 0; _snprintf(g_szSmallInfoFile, MAX_PATH, "%s", pMark1+1);
	*pMark4 = 0; _snprintf(g_szBigInfoFile, MAX_PATH, "%s", pMark3+1);
	*pMark6 = 0; _snprintf(g_szDumpFile, MAX_PATH, "%s", pMark5+1);


	//GetCommandLine();
	SendReport();
	return FALSE;
}
void CCrashReportApp::SendReport(void)
{
	//-------生成------------------------------------------------
	CHAR szCurrentDir[MAX_PATH] = {0};
	::GetCurrentDirectory(MAX_PATH, szCurrentDir);

	CHAR szCrashFile[MAX_PATH] = {0};
	SYSTEMTIME timeNow;
	::GetLocalTime(&timeNow);
	_snprintf(szCrashFile, MAX_PATH, "crash%02d_%02d_%02d_%02d.zip",
		timeNow.wMonth, timeNow.wDay, timeNow.wHour, timeNow.wMinute,timeNow.wSecond);

	//-------------------------------------------------------
	CHAR szLocalFile[MAX_PATH] = {0};
	strncpy(szLocalFile, szCurrentDir, MAX_PATH);
	PathAppend(szLocalFile, szCrashFile);
	HZIP hz = CreateZip((void *)szLocalFile, 0, ZIP_FILENAME);
	if(!hz) 
	{

		this->m_pMainWnd->MessageBox("Create Crash.zip error");
		return;
	}

	ZipAdd(hz, "dump.dmp", (void *)g_szDumpFile, 0, ZIP_FILENAME);
	ZipAdd(hz, "info.txt", (void *)g_szBigInfoFile, 0, ZIP_FILENAME);

	CHAR szLogFile[MAX_PATH];

	strncpy(szLogFile, szCurrentDir, MAX_PATH);
	PathAppend(szLogFile, "WXRender.log");
	ZipAdd(hz, "WXRender.log", (void *)szLogFile, 0, ZIP_FILENAME);

	strncpy(szLogFile, szCurrentDir, MAX_PATH);
	PathAppend(szLogFile, "UISystem.log");
	ZipAdd(hz, "UISystem.log", (void *)szLogFile, 0, ZIP_FILENAME);

	strncpy(szLogFile, szCurrentDir, MAX_PATH);
	PathAppend(szLogFile, "../wxversion");
	ZipAdd(hz, "wxversion", (void *)szLogFile, 0, ZIP_FILENAME);

	CloseZip(hz);

	//--------------------------发送
	CFtpTrans CMyFtpTrans;
	CMyFtpTrans.SetAccessRight("wxsj","up");
	if(CMyFtpTrans.OpenConnection("ftp.hopecool.com "))
	{
		CMyFtpTrans.PutFile(szLocalFile,szCrashFile);
		CMyFtpTrans.CloseConnection();
		//this->m_pMainWnd->MessageBox("OK");
	}
}
