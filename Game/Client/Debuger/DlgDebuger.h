#pragma once


// CDlgDebuger dialog
#include "Resource.h"
#include "ResizableDialog.h"

#include "DlgProfile.h"
#include "DlgUI.h"
#include "CaptionButton.h"

#include "afxcmn.h"

class CDlgDebuger :  
	public CResizableDialog
	//public CDialog 
{
	DECLARE_DYNAMIC(CDlgDebuger)

public:
	CDlgUI*			GetDlgUI(void) { return &m_dlgUI; }

public:
	CDlgDebuger(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgDebuger();

// Dialog Data
	enum { IDD = IDD_DIALOG_DEBUGER };

	//子窗口
protected:
	void			CreateSubWindow(void);
	//效率测试
	CDlgProfile		m_dlgProfile;
	//UI窗口
	CDlgUI			m_dlgUI;

	//主窗口上按钮
	CCaptionButton*	m_pCaptionBtn;

protected:
	//最小化
	void			OnMinimizeWindow(void);

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL	OnInitDialog();
	void			OnCapButton(BOOL bBtnState);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CTabCtrl m_tabMain;
	afx_msg void OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg LRESULT OnWMU_Hitcapbutton(WPARAM wParam, LPARAM lParam);
};
