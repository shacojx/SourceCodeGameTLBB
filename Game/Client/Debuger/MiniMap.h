#pragma once


// CMiniMap

class CMiniMap : public CStatic
{
	DECLARE_DYNAMIC(CMiniMap)

public:
	void			SetMapFile(LPCTSTR szFileName, INT nWidth, INT nHeight, INT nWordWidth, INT nWorldHeight);
	void			SetViewCenter(FLOAT fX, FLOAT fY, float fDir);
	void			Update(void);

protected:
	CString			m_strMapFile;
	INT				m_nMapWidth;
	INT				m_nMapHeight;
	INT				m_nWorldWidth;
	INT				m_nWorldHeight;

	FLOAT			m_fScaleWidth;
	FLOAT			m_fScaleHeight;

	FLOAT			m_fPlayerPosX;
	FLOAT			m_fPlayerPosY;

	CPoint			m_ptViewCenter;
	float			m_fDir;

	CRect			m_rectView;

	INT				m_nPanX;
	INT				m_nPanY;

	CBitmap			m_bmpMap;

protected:
	static HCURSOR	m_hHandArrow;

	BOOL			m_bMouseOver;
	BOOL			m_bMouseDown;

	CPoint			m_ptMouseDragBegin;

public:
	CMiniMap();
	virtual ~CMiniMap();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg UINT OnNcHitTest(CPoint point) { return HTCLIENT; }
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};


