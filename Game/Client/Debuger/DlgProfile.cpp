// DlgProfile.cpp : implementation file
//

#include "stdafx.h"
#include "Debuger.h"
#include "DlgProfile.h"
#include ".\dlgprofile.h"
#include "Global.h"

// CDlgProfile dialog
CDlgProfile* g_this = NULL;
IMPLEMENT_DYNAMIC(CDlgProfile, CResizableDialog)
CDlgProfile::CDlgProfile(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CDlgProfile::IDD, pParent)
{
	g_this = this;
}

CDlgProfile::~CDlgProfile()
{
}

void CDlgProfile::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_PROFILE, m_treeProfile);
}


BEGIN_MESSAGE_MAP(CDlgProfile, CResizableDialog)
	ON_BN_CLICKED(IDC_BUTTON_FRESH, OnBnClickedButtonFresh)
END_MESSAGE_MAP()


// CDlgProfile message handlers

BOOL CDlgProfile::OnInitDialog()
{
	CResizableDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	AddAnchor(IDC_TREE_PROFILE, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_BUTTON_FRESH, BOTTOM_CENTER);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

VOID WINAPI CDlgProfile::_FreshProfileNode(const PROFILE_TREE* pProfileNodeTree)
{
	if(g_this)
	{
		g_this->FreshProfileNode(pProfileNodeTree);
	}
}

VOID CDlgProfile::FreshProfileNode(const PROFILE_TREE* pProfileNodeTree)
{
	HTREEITEM hOldRoot = m_treeProfile.GetRootItem();
	FreshProfileTree(TVI_ROOT, &(pProfileNodeTree->theRoot));
	if(hOldRoot) m_treeProfile.DeleteItem(hOldRoot);
}

VOID CDlgProfile::FreshProfileTree(HTREEITEM hToInsert, const RPOFILE_TREE_NODE* pProfileNode)
{
	TVINSERTSTRUCT tvis;
	
	LARGE_INTEGER gPer;
	QueryPerformanceFrequency(&gPer);

	CHAR szTemp[MAX_PATH];
	if(pProfileNode->pParentNode)
	{
		_snprintf(szTemp, MAX_PATH, "%s[%%%.1f][%.2fms*%d]\n", 
			pProfileNode->strName.c_str(), 
			((DOUBLE)(pProfileNode->nTotalTime.QuadPart)*100.0)/
			(DOUBLE)(pProfileNode->pParentNode->nTotalTime.QuadPart),
			pProfileNode->nTotalTime.QuadPart*1000.0/gPer.QuadPart,
			pProfileNode->nInvokeCount
			);
	}
	else
	{
		//是根节点(Root)
		_snprintf(szTemp, MAX_PATH, "%s[%%100]\n", pProfileNode->strName.c_str());
	}

	//
	//CHAR szTemp2[MAX_PATH];
	//{
	//	LARGE_INTEGER nTotal;
	//	nTotal.QuadPart= 0;
	//	stdext::hash_map< STRING, RPOFILE_TREE_NODE >::const_iterator it;
	//	for(it=pProfileNode->mapChild.begin(); it!=pProfileNode->mapChild.end(); it++)
	//	{
	//		nTotal.QuadPart += it->second.nTotalTime.QuadPart;
	//	}
	//	_snprintf(szTemp2, MAX_PATH, "%s %.1f/%.1f", szTemp,
	//		(DOUBLE)nTotal.QuadPart, (DOUBLE)(pProfileNode->nTotalTime.QuadPart));
	//}

	tvis.hParent = hToInsert;
	tvis.hInsertAfter = TVI_LAST;
	tvis.item.mask = TVIF_TEXT|TVIF_SELECTEDIMAGE|TVIF_PARAM;
	tvis.item.pszText = (LPTSTR)szTemp;
//	tvis.item.iImage = nImageIndex;
//	tvis.item.iSelectedImage = nImageIndex;
	tvis.item.lParam = (LPARAM)pProfileNode;
	HTREEITEM hItem =  m_treeProfile.InsertItem(&tvis);

	RPOFILE_TREE_NODE::HASH_TREENODE::const_iterator it;
	for(it=pProfileNode->mapChild.begin(); it!=pProfileNode->mapChild.end(); it++)
	{
		FreshProfileTree(hItem, it->second);
	}

	m_treeProfile.Expand(hItem, TVM_EXPAND);
}

void CDlgProfile::OnBnClickedButtonFresh()
{
	// TODO: Add your control notification handler code here
	TDProfile_DumpSruct(_FreshProfileNode);
}
