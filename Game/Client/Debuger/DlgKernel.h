#pragma once

#include "TDKernel.h"
#include "ResizableDialog.h"
#include "afxcmn.h"

class tObjectSystem;
class CDlgKernel : 
	public CResizableDialog
//	public CDialog
{
	DECLARE_DYNAMIC(CDlgKernel)
protected:
	CTreeCtrl		m_treeNode;
	HIMAGELIST		m_hImageList;
	tObjectSystem*	m_pObjMan;

	bool			m_bAutoFresh;
	float			m_fFreshTime;

public:
	CDlgKernel(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgKernel();

// Dialog Data
	enum { IDD = IDD_DIALOG_KERNEL };

protected:
	void	OnNodeTreeSelChange(void);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonManual();
};
