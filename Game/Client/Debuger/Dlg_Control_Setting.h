#pragma once

#include "ResizableDialog.h"

// CDlg_Control_Setting dialog

class CDlg_Control_Setting :
//	public CDialog
	public CResizableDialog
{
	DECLARE_DYNAMIC(CDlg_Control_Setting)

public:
	CDlg_Control_Setting(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_Control_Setting();

// Dialog Data
	enum { IDD = IDD_DIALOG_CONTROL_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonTo800x600();
	afx_msg void OnBnClickedButtonTo1024X768();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckFullscreen();
};
