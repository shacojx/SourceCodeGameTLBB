#pragma once


// CSplitWnd
#define NM_SW_CHANGED		(WM_USER+0XC0DE)

class CSplitWnd : public CStatic
{
	DECLARE_DYNAMIC(CSplitWnd)

public:
	CSplitWnd();
	virtual ~CSplitWnd();
	int				GetPosOffset(void) { return m_nPosOffset; }

protected:
	BOOL			m_bCapture;
	INT				m_nLastPos;
	INT				m_nPosOffset;

	const static int		INVALID_POS = 0X1FFFFFFF;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


