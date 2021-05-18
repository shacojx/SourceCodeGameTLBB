#pragma once
#include "afxcmn.h"
// CDlgView dialog
#include "ResizableDialog.h"

class tGfxSystem;
class tInputSystem;
class CDlgView : 
	public CResizableDialog
//	public CDialog
{
	DECLARE_DYNAMIC(CDlgView)

public:
	void	Tick(void);

protected:
	tInputSystem*	m_pInputManager;
	tGfxSystem*		m_pRenderLayer;

public:
	CDlgView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgView();

// Dialog Data
	enum { IDD = IDD_DIALOG_VIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CSliderCtrl m_sliderZoom;
	CSliderCtrl m_sliderDirection;
	afx_msg void OnTimer(UINT nIDEvent);
};
