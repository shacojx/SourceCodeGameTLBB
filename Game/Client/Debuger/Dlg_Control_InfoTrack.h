#pragma once

#include "ResizableDialog.h"

// CDlg_Control_InfoTrack dialog

class CDlg_Control_InfoTrack : 
	public CResizableDialog
{
	DECLARE_DYNAMIC(CDlg_Control_InfoTrack)

public:
	//÷ÿππ
	void			Reset(INT nWidth, INT nHeight);
	//ª≠µ„
	void			AddPoint(INT nX, INT nY);

protected:
	class CInfoTrack : public CStatic
	{
	protected:
		DECLARE_MESSAGE_MAP()
	public:
		afx_msg void OnPaint();

		CDlg_Control_InfoTrack* m_pParent;
	};

	CBitmap			m_bmpInfo;
	CInfoTrack		m_staInfoTrack;
	POINT			m_ptLastPoint;

public:
	CDlg_Control_InfoTrack(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlg_Control_InfoTrack();

// Dialog Data
	enum { IDD = IDD_DIALOG_CONTROL_INFOTRACK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
