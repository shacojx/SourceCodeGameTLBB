#pragma once


// CDlg_Control_Console dialog
#include "ResizableDialog.h"
#include "afxwin.h"

class CDlg_Control_Console :
//	public CDialog
	public CResizableDialog
{
	DECLARE_DYNAMIC(CDlg_Control_Console)

public:
	CDlg_Control_Console(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_Control_Console();

// Dialog Data
	enum { IDD = IDD_DIALOG_CONTROL_CONSOLE };

protected:
	class CInnerEdit : public CEdit
	{
	protected:
		DECLARE_MESSAGE_MAP()
	public:
		afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	};

	CInnerEdit	m_theInnnerEdit;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSend();
	CComboBox m_comConsole;
};
