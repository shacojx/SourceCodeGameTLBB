#pragma once
#include "afxwin.h"

#include "ResizableDialog.h"

// CDlg_Control_SceneInfo dialog
struct EVENT;

class CDlg_Control_SceneInfo :
//	public CDialog
	public CResizableDialog
{
	DECLARE_DYNAMIC(CDlg_Control_SceneInfo)

public:
	static VOID	WINAPI	_OnSceneTransed(const EVENT* pEvent, UINT dwOwnerData);

protected:
	VOID OnSceneTransed(const EVENT* pEvent);

public:
	CDlg_Control_SceneInfo(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_Control_SceneInfo();

// Dialog Data
	enum { IDD = IDD_DIALOG_CONTROL_SCENEINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CStatic m_staInfo;
};
