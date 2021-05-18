// DlgKernel.cpp : implementation file
//

#include "stdafx.h"
#include "Debuger.h"
#include "DlgKernel.h"
#include ".\dlgkernel.h"
#include "Global.h"
#include "TDException.h"
#include "TDObjectSystem.h"

// CDlgKernel dialog

IMPLEMENT_DYNAMIC(CDlgKernel, CResizableDialog)
CDlgKernel::CDlgKernel(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CDlgKernel::IDD, pParent)
{
	m_hImageList = NULL;
	m_bAutoFresh = false;
	m_fFreshTime = 1.0f;
}

CDlgKernel::~CDlgKernel()
{
	if(m_hImageList)
	{
		ImageList_Destroy(m_hImageList);
	}
}

void CDlgKernel::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_NODE, m_treeNode);
}


BEGIN_MESSAGE_MAP(CDlgKernel, CResizableDialog)
	ON_BN_CLICKED(IDC_BUTTON_MANUAL, OnBnClickedButtonManual)
END_MESSAGE_MAP()


// CDlgKernel message handlers

BOOL CDlgKernel::OnInitDialog()
{
	CResizableDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	AddAnchor(IDC_STATIC_FRESH_GROUP, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_EDIT_FRESH_SPEED, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_STATIC_SEC, TOP_RIGHT);
	AddAnchor(IDC_BUTTON_MANUAL, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_TREE_NODE, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_EDIT_DESC, BOTTOM_LEFT, BOTTOM_RIGHT);

	GetDlgItem(IDC_EDIT_FRESH_SPEED)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_FRESH_SPEED)->SetWindowText(_T("1"));
//	SetTimer(101, 100, NULL);

	//Load standard icon
	m_hImageList = ImageList_Create(16, 16, ILC_COLOR16, 10, 256);
	ImageList_SetBkColor(m_hImageList, GetSysColor(COLOR_WINDOW));
	TreeView_SetImageList(m_treeNode.GetSafeHwnd(), m_hImageList, TVSIL_NORMAL);

	HICON hIcon;
	// 0. Unknown
	hIcon = AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDI_ICON_UNKNOWN));
	ImageList_AddIcon(m_hImageList, hIcon);
	// 1. Folder
	hIcon = AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDI_ICON_FOLDER));
	ImageList_AddIcon(m_hImageList, hIcon);
	// 2. Bin
	hIcon = AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDI_ICON_BIN));
	ImageList_AddIcon(m_hImageList, hIcon);
	// 3. Player
	hIcon = AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDI_ICON_PLAYER));
	ImageList_AddIcon(m_hImageList, hIcon);

	m_pObjMan = (tObjectSystem*)g_pKernel->GetNode(_T("bin\\objman"));
	TDAssert(m_pObjMan);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgKernel::OnBnClickedButtonManual()
{
	// TODO: Add your control notification handler code here
	//g_pKernel->Debug_OutputTree(m_theKernelInfo);

	//HTREEITEM hOldRoot = m_treeNode.GetRootItem();
	//FreshKernelList(TVI_ROOT, m_theKernelInfo);
	//if(hOldRoot) m_treeNode.DeleteItem(hOldRoot);
}
