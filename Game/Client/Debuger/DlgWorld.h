#pragma once

// CDlgWorld dialog
#include "ResizableDialog.h"
#include "afxwin.h"
#include "MiniMap.h"

class tWorldSystem;
class CDlgWorld : 
//	public CDialog
	public CResizableDialog
{
	DECLARE_DYNAMIC(CDlgWorld)

public:
	void				Tick(void);
	void				MiniMap_SetCenter(FLOAT fX, FLOAT fY, float fDir);
	void				MiniMap_SetFileName(LPCTSTR szFileName, INT nWidth, INT nHeight, INT nWorldWidth, INT nWorldHeight);
	void				OnPostFilterChanged(void);
	void				PostFilter_Enable(BOOL bEnable);
	void				PostFilter_SetFloodlighting_Param(FLOAT fBlur, FLOAT fShine);

	void				OnLightMapChanged(void);

protected:
	void				RefreshSceneDesc(void);
	void				RefreshPostFilter(void);

	tWorldSystem*		m_pWorldManager;
	INT				m_nActiveSceneID;
	CMiniMap			m_staMiniMap;

	CSliderCtrl			m_sliderBlur;
	CSliderCtrl			m_sliderShine;

	CStatic				m_staBlur;
	CStatic				m_staShine;

public:
	CDlgWorld(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgWorld();

// Dialog Data
	enum { IDD = IDD_DIALOG_WORLD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedRadioPfNull();
	afx_msg void OnBnClickedRadioPfHeatvision();
	afx_msg void OnBnClickedRadioPfFloodlighting();
	afx_msg void OnBnClickedRadioPfBloom();
	afx_msg void OnBnClickedRadioLmNull();
	afx_msg void OnBnClickedRadioLmMiddle();
	afx_msg void OnBnClickedRadioLmHigh();
};
