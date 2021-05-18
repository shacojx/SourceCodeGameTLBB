// ComWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Helper.h"
#include "ComWnd.h"
#include "HelperSystem.h"

// CComWnd dialog

IMPLEMENT_DYNAMIC(CComWnd, CDialog)
CComWnd::CComWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CComWnd::IDD, pParent)
{
}

CComWnd::~CComWnd()
{
}

void CComWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CComWnd, CDialog)
	ON_WM_COPYDATA()
END_MESSAGE_MAP()


// CComWnd message handlers
BOOL CComWnd::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	LPHELPER_COMMAND pCmd = (LPHELPER_COMMAND)(pCopyDataStruct->lpData);
	if(!pCmd) return TRUE;

	CHelperSystem::GetMe()->GotoAnchor(pCmd->pContents);
	
	return TRUE;
}