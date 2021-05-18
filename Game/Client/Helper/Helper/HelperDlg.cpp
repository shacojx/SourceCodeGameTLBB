// HelperDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Helper.h"
#include "HelperDlg.h"
#include "HelperSystem.h"
#include "HelperItemset.h"
#include "HelperItem.h"
#include ".\helperdlg.h"
#include "HelperDocHostUIHandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CHelperDlg 对话框

CHelperDlg::CHelperDlg(CWnd* pParent /*=NULL*/)
	: CCaptionButtonDlg(CHelperDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nLeftWidth = 0;
}

CHelperDlg::~CHelperDlg()
{
}

void CHelperDlg::DoDataExchange(CDataExchange* pDX)
{
	CCaptionButtonDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_SPLIT, m_staSplitVer);
	DDX_Control(pDX, IDC_TREE_ITEMS, m_treeItems);
	DDX_Control(pDX, IDC_EXPLORER_HELPER, m_webHelper);
}

BEGIN_MESSAGE_MAP(CHelperDlg, CCaptionButtonDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_ITEMS, OnTvnSelchangedTreeItems)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CHelperDlg 消息处理程序

BOOL CHelperDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// Reposition all controls
	CRect rect;
	m_treeItems.GetClientRect(&rect);
	m_nLeftWidth = rect.Width();
	RepositionDlg();
	CenterWindow();

	// Add hook control btn(Caption button)
	static CImageList s_ilPin;
	s_ilPin.Create(IDB_BITMAP_PIN, 16, 5, RGB(255, 255, 255));
	AddButton(ID_BUTTON_PIN, 16, 16, s_ilPin.m_hImageList, "Stay visible");
	OnPinButtonPushed();

	// Add standard icons
	m_treeItems.AddIcon(AfxGetApp()->LoadIcon(IDI_ICON_FOLDER_CLOSE));
	m_treeItems.AddIcon(AfxGetApp()->LoadIcon(IDI_ICON_FOLDER_OPEN));
	m_treeItems.AddIcon(AfxGetApp()->LoadIcon(IDI_ICON_DOCUMENT));

	// Refresh item list
	RefreshHelpItems();

	// Go to blank page
	m_webHelper.Navigate("about:blank", 0, 0, 0, 0);

	//取得IcustomDoc接口
	//ICustomDoc* pCustDoc = 0;
	//m_webHelper.get_Document()->QueryInterface(&pCustDoc);
	//if(pCustDoc)
	//{
	//	pCustDoc->SetUIHandler(new HelperDocHostUIHandler());
	//	pCustDoc->Release();
	//}

	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CHelperDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();

		// Draw grip size
		DrawGripSize();
	}
}

void CHelperDlg::DrawGripSize(void)
{
	static CImageList s_ilGripSize;
	if(s_ilGripSize.m_hImageList == 0)
	{
		s_ilGripSize.Create(IDB_BITMAP_GRIP_SIZE, 12, 1, RGB(255, 0, 255));
	}

	CRect rect;
	GetWindowRect(&rect);

	CDC *dc = GetWindowDC();
	ImageList_Draw(s_ilGripSize.m_hImageList, 0, dc->m_hDC, rect.Width()-12-3, rect.Height()-12-3, ILD_NORMAL);
	::ReleaseDC(m_hWnd, dc->m_hDC);
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CHelperDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CHelperDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class
	LPNMHDR pnmh = (LPNMHDR) lParam;

	if(pnmh->code == NM_SW_CHANGED )
	{
		if(pnmh->hwndFrom = GetDlgItem(IDC_STATIC_SPLIT)->GetSafeHwnd())
		{
			m_nLeftWidth += m_staSplitVer.GetPosOffset();
			RepositionDlg();
			Invalidate();
		}
	}

	return CDialog::OnNotify(wParam, lParam, pResult);
}

void CHelperDlg::RepositionDlg(void)
{
	CRect rectClient;
	GetClientRect(&rectClient);

	if(m_treeItems.GetSafeHwnd())
	{
		m_treeItems.MoveWindow(
			11, 
			11, 
			m_nLeftWidth,
			rectClient.Height()- 21);
	}

	if(m_staSplitVer.GetSafeHwnd())
	{
		m_staSplitVer.MoveWindow(
			m_nLeftWidth + 11 + 5, 
			11,
			5, 
			rectClient.Height() - 21);
	}

	if(m_webHelper.GetSafeHwnd())
	{
		m_webHelper.MoveWindow(
			m_nLeftWidth + 11 + 15, 
			11,
			rectClient.Width() - m_nLeftWidth - 36, 
			rectClient.Height() - 21);
	}
}

void CHelperDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(GetSafeHwnd())
	{
		RepositionDlg();

		Invalidate();
	}
}

void CHelperDlg::RefreshHelpItems(void)
{
	CHelperItemSet* pTheItemSet = CHelperSystem::GetMe()->GetHelpItemSet();

	// insert root item
	_refreshHelpItem(TVI_ROOT, *(pTheItemSet->GetRootItem()));
}

void CHelperDlg::_refreshHelpItem(HTREEITEM hItem, CHelperItem& helperItem)
{
	// get child buf
	CHelperItem::ChildBuf& theChild = helperItem.GetChildBuf();
	
	CHelperItem::ChildBuf::iterator it;
	// insert root item's child item
	for(it=theChild.begin(); it!=theChild.end(); it++)
	{
		CHelperItem& thisItem = *it;

		// insert this

		HTREEITEM hThisItem;
		if(thisItem.GetChildBuf().empty())
		{
			// It's a document
			hThisItem = m_treeItems.InsertItem(thisItem.GetName().c_str(), 2, 2, hItem);
		}
		else
		{
			// It's a folder
			hThisItem = m_treeItems.InsertItem(thisItem.GetName().c_str(), 0, 1, hItem);
		}

		// set user data
		m_treeItems.SetItemData(hThisItem, (DWORD_PTR)&thisItem);
		thisItem.SetUserData((int)hThisItem);

		_refreshHelpItem(hThisItem, thisItem);
	}
}

void CHelperDlg::OnTvnSelchangedTreeItems(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	OnItemSelectChanged();
	*pResult = 0;
}

void CHelperDlg::OnItemSelectChanged(void)
{
	HTREEITEM hItem = m_treeItems.GetSelectedItem();
	if(hItem == NULL) return;
	
	CHelperItem* pHelperItem = (CHelperItem*)(DWORD_PTR)m_treeItems.GetItemData(hItem);
	if(!pHelperItem) return;

	if(pHelperItem->GetURL().empty()) return;

	m_webHelper.Navigate(pHelperItem->GetURL().c_str(), 0, 0, 0, 0);
}

void CHelperDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::OnCancel();
}

BOOL CHelperDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if(LOWORD(wParam) == IDCANCEL || LOWORD(wParam) == IDOK)
	{
		return TRUE;
	}
	else if(LOWORD(wParam) == ID_BUTTON_PIN)
	{
		OnPinButtonPushed();
	}

	return CDialog::OnCommand(wParam, lParam);
}

void CHelperDlg::OnPinButtonPushed(void)
{
	bool bAlwaysTop = (GetWindowLong(m_hWnd, GWL_EXSTYLE) & WS_EX_TOPMOST)!=0;
	
	bAlwaysTop = !bAlwaysTop;

	CheckButton(0, bAlwaysTop);

	if(bAlwaysTop)
	{
		::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
	}
	else
	{
		::SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
	}

}

void CHelperDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	DestroyWindow();
}

void CHelperDlg::GotoHelperItem(HTREEITEM hItem)
{
	m_treeItems.SelectItem(hItem);
}

LRESULT CHelperDlg::OnWMU_Hitcapbutton(WPARAM wParam, LPARAM lParam)
{
	switch ((UINT)wParam) 
	{
	case 1:	// Button #1 has been hit:
//		OnCapButton((BOOL)lParam);
		break;
	default:
		return 0;;
	}
	return 0; // Allways return 0(LRESULT->void)
}
BEGIN_EVENTSINK_MAP(CHelperDlg, CCaptionButtonDlg)
	ON_EVENT(CHelperDlg, IDC_EXPLORER_HELPER, 250, BeforeNavigate2ExplorerHelper, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
END_EVENTSINK_MAP()

void CHelperDlg::BeforeNavigate2ExplorerHelper(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, BOOL* Cancel)
{
	CStringW strURLIn(URL->bstrVal);
	CStringW strProtol = strURLIn.Mid(0, 7);
	if(strProtol.CompareNoCase(L"anch://")==0)
	{
		*Cancel = TRUE;
		CString strAnchor = CString(strURLIn.Mid(7));
		strAnchor.TrimRight("/\\");
		CHelperSystem::GetMe()->GotoAnchor((LPCTSTR)strAnchor);
	}
}
