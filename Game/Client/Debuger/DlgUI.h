#pragma once

#include "ResizableDialog.h"
#include "afxwin.h"
#include "RollupCtrl.h"

// CDlgUI dialog
class CDlg_Control_Console;
class CDlg_Control_SceneInfo;
class CDlg_Control_Setting;
class CDlg_Control_InfoTrack;

class CDlgUI : public CDialog
{
	DECLARE_DYNAMIC(CDlgUI)

public:
	void			AddPage(LPCTSTR szName, CDialog* pDlg);
	HWND			GetRollupCtrl(void);

public:
	CDlg_Control_Console*	m_pConsoleDlg;
	CDlg_Control_SceneInfo* m_pSceneInfoDlg;
	CDlg_Control_Setting*	m_pSettingDlg;
	CDlg_Control_InfoTrack*	m_pInfotrackDlg;

public:
	CDlgUI(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgUI();

// Dialog Data
	enum { IDD = IDD_DIALOG_UI };
protected:
	CRollupCtrl		m_wndRollupCtrl;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
