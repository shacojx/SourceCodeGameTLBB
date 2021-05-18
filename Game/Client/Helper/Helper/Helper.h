// Helper.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error 在包含用于 PCH 的此文件之前包含“stdafx.h”
#endif

#include "resource.h"		// 主符号
#include "HelperDlg.h"
#include "ComWnd.h"


class CHelperApp : public CWinApp
{
public:
	// get anchor 
	const CString&	GetAnchor(void) const { return m_strAnchor; }
	
	// get commucation cwnd;
	CWnd*			GetCommucationWnd(void) { return &m_wndCommucation; }

public:
	CHelperApp();
	virtual ~CHelperApp();

// 重写
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance(); // return app exit code

protected:
	// the Main dialog
	CHelperDlg*		m_pMainDialog;
	// the dialog used to accept order from game client.
	CComWnd			m_wndCommucation;
	// anchor of item
	CString			m_strAnchor;

	DECLARE_MESSAGE_MAP()
};

extern CHelperApp theApp;
