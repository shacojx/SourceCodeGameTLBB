// HelperDlg.h : 头文件
//

#pragma once
#include "SplitWnd.h"
#include "explorer_helper.h"
#include "TreeCtrlEx.h"
#include "CaptionButtonDialog.h"

class CHelperItem;
class CCaptionButton;
// CHelperDlg 对话框
class CHelperDlg : public CCaptionButtonDlg
{
public:
	// go to spec item
	void		GotoHelperItem(HTREEITEM hItem);

// 构造
public:
	CHelperDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CHelperDlg();

// 对话框数据
	enum { IDD = IDD_HELPER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

protected:
	// resize all dialog control.
	void		RepositionDlg(void);
	// refresh all items
	void		RefreshHelpItems(void);
	// on item tree select item changed.
	void		OnItemSelectChanged(void);
	// on pin button pushed
	void		OnPinButtonPushed(void);
	// draw grip size
	void		DrawGripSize(void);

private:
	// refresh helper items to a tree item
	void		_refreshHelpItem(HTREEITEM hItem, CHelperItem& thisItem);

// 实现
protected:
	HICON			m_hIcon;
	CSplitWnd		m_staSplitVer;
	CTreeCtrlEx		m_treeItems;
	CExplorer_help	m_webHelper;
	CCaptionButton*	m_pCaptionBtn;

	INT				m_nLeftWidth;

	// 生成的消息映射函数
	DECLARE_MESSAGE_MAP()

	// virtual function
	virtual BOOL OnInitDialog();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void OnCancel();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	// process message
	afx_msg void	OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void	OnSize(UINT nType, int cx, int cy);
	afx_msg void	OnTvnSelchangedTreeItems(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void	OnClose();
	afx_msg LRESULT OnWMU_Hitcapbutton(WPARAM wParam, LPARAM lParam);
public:
	DECLARE_EVENTSINK_MAP()
	void BeforeNavigate2ExplorerHelper(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, BOOL* Cancel);
};
