// Debuger.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Debuger.h"
#include "Global.h"
#include "TDTheDebuger.h"
#include "TDPlugin.h"
#include "TDException.h"
#include "TDKernel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CDebugerApp

BEGIN_MESSAGE_MAP(CDebugerApp, CWinApp)
END_MESSAGE_MAP()


// CDebugerApp construction

CDebugerApp::CDebugerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CDebugerApp object

CDebugerApp theApp;


// CDebugerApp initialization

BOOL CDebugerApp::InitInstance()
{
	CWinApp::InitInstance();
	return TRUE;
}

/////////////////////////////////////////////////////
// 导出函数

//Magic Code
extern "C" UINT DllMagicCode = DLL_MAGIC_CODE|TD_ENGINE_VERSION;

//插件初始化
extern "C" void __stdcall DllInit( tKernel* pKernel )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	TDAssert(pKernel);

	g_pKernel = pKernel;
	//注册类
	g_pKernel->ReisgerClass(GETCLASS(TDDebuger));
}

//插件释放
extern "C" void __stdcall DllRelease( void )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
}
