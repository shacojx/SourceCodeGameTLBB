#pragma once

#include "ResizableDialog.h"
#include "TDProfile.h"
#include "afxcmn.h"
// CDlgProfile dialog

class CDlgProfile : 
	public CResizableDialog
//	public CDialog
{
	DECLARE_DYNAMIC(CDlgProfile)

public:
	//节点更新函数
	static VOID	WINAPI _FreshProfileNode(const PROFILE_TREE* pProfileNodeTree);

protected:
	VOID	FreshProfileNode(const PROFILE_TREE* pProfileNodeTree);
	VOID	FreshProfileTree(HTREEITEM hToInsert, const RPOFILE_TREE_NODE* pProfileNode);

public:
	CDlgProfile(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgProfile();

// Dialog Data
	enum { IDD = IDD_DIALOG_PROFILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonFresh();
	CTreeCtrl m_treeProfile;
};
