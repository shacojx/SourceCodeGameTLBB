// DlgInfo.cpp : implementation file
//

#include "stdafx.h"
#include "Debuger.h"
#include "DlgInfo.h"
#include ".\dlginfo.h"


// CDlgInfo dialog

IMPLEMENT_DYNAMIC(CDlgInfo, CDialog)
CDlgInfo::CDlgInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInfo::IDD, pParent)
{
}

CDlgInfo::~CDlgInfo()
{
}

void CDlgInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgInfo, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgInfo message handlers

void CDlgInfo::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(cx<=0 || cy<=0) return;

	CWnd* pChildWnd = GetWindow(GW_CHILD);
	if(pChildWnd) pChildWnd->MoveWindow(0, 0, cx, cy);
}
