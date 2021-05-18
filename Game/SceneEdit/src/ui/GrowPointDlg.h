/////////////////////////////////////////////////////////////////////////////
// Name:        GrowPointDlg.h
// Author:      XX
// Created:     XX/XX/XX
// Copyright:   XX
/////////////////////////////////////////////////////////////////////////////

#ifndef __GrowPointDlg_H__
#define __GrowPointDlg_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "GrowPointDlg.cpp"
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "./res/GrowPointEdit_wdr.h"

// WDR: class declarations

#include <wx/panel.h>
#include <wx/image.h>

#include <OgreString.h>
#include <OgreImage.h>
#include "PaintAction.h"
#include "WXSceneListener.h"

typedef std::map<int, std::string>	ID_NAME_DLG_MAP;//
typedef std::map<int, wxTreeItemId> ID_TREE_ITEM_MAP;

namespace WX 
{
    class SceneManipulator;
	class CGrowPointEditAction;		// 操作不可行走区域场景编辑的类
}


class SceneInfoMapDialog;
class FairyEditorFrame;
class CGrowPointEditDlg : public wxPanel, WX::SceneListener
{

///////////////////////////////////////////////////////////////////////////////////////////
//
// 构造函数。
//
public:

	CGrowPointEditDlg(const wxString& resourceGroup = wxT("GrowPointEdit"));

	CGrowPointEditDlg(
						wxWindow *parent, 
						const wxString& resourceGroup = wxT("GrowPointEdit"),
						wxWindowID winid = wxID_ANY,
						const wxPoint& pos = wxDefaultPosition,
						const wxSize& size = wxDefaultSize,
						long style = wxTAB_TRAVERSAL | wxNO_BORDER,
						const wxString& name = "GrowPointEdit"
						);


	~CGrowPointEditDlg()
	{

	}

//
// 构造函数。
//
///////////////////////////////////////////////////////////////////////////////////////////



	//****************************************************************************************************/
	//
	// 初始化函数 清除函数。
	//

	// 创建
    bool Create(
				wxWindow *parent,
                wxWindowID winid = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxTAB_TRAVERSAL | wxNO_BORDER,
                const wxString& name = "GrowPointEdit");


	// 显示窗口
	bool Show(bool show);

	
	//
	// 初始化函数 清除函数。
	//
	//****************************************************************************************************/



//////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 逻辑数据
//
//
public:

	// id 与名字列表
	ID_NAME_DLG_MAP		m_IdNameMap;

	// id 与mesh列表
    ID_NAME_DLG_MAP		m_IdMeshMap;

	//id 与tree item列表.
	ID_TREE_ITEM_MAP    m_IdTreeItemMap;

	// 已经添加的怪物
	ID_TREE_ITEM_MAP    m_AddedTreeItemMap;


	// 添加一个生长点
	void AddNewGrowPoint(unsigned long ulMonsterId, unsigned long ulInstanceId);

	// 删除一个生长点
	void DelGrowPoint(unsigned long ulTypeId, unsigned long ulInstanceId);

	// 添加action的mesh表
	void SetActionMeshType();


//
// 逻辑数据
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 界面控件操作
//

public:

	//导航图界面
	SceneInfoMapDialog* m_pNavigationDlg;

	// 父窗口指针
	FairyEditorFrame* m_pParent;

	// 已经添加怪物树的根节点.
	wxTreeItemId	m_AddedGrowPointRoot;

	// 怪物列表控件。
	wxListCtrl*	m_pGrowPointListCtrl;

	// 已经添加的怪物列表。
	wxTreeCtrl* m_pAddedGrowPointTreeCtrl;

	// 初始化怪物列表的表头.
	void InitGrowPointListCtrlTable();

	// 初始化怪物列表树.
	void InitAddedGrowPointTree();

	// 得到所有的控件.
	void GetAllControl();

	// 清空ui
	void ClearUI();

	// 设置父窗口
	void SetParent(FairyEditorFrame* pParent);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 控件响应函数
	//

	// 选择一个生长点的类型
	 void OnGrowPointTypeSelected(wxListEvent& event);

	 // 选择已经添加生长点
	 void OnGrowPointAddedSelected(wxTreeEvent& event);

//
// 界面控件操作
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 场景操作
	//

	public:

	WX::SceneManipulator*		m_pSceneManipulator;
	WX::CGrowPointEditAction*	m_pGrowPointEditAction;

	// 得到action
	void GetAction();

	// 设置场景操作句柄
	void SetSceneManipulator(WX::SceneManipulator* sceneManipulator);


	// 激活场景
	void SetActive(bool bShow);

	//
	// 场景操作
	//
	////////////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 界面通知回掉函数
	//

	public:

	virtual void onAddNewGrowPoint(unsigned long ulTypeId, unsigned long ulInstanceId);
	// 删除一个生长点
	virtual void onDelGrowPoint( unsigned long ulTypeId, unsigned long ulInstanceId);
	// 选择一个生长点
	virtual void onSelGrowPoint( unsigned long ulTypeId, unsigned long ulInstanceId);

	
	//
	// 界面通知回掉函数
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	

	DECLARE_EVENT_TABLE()
	
};


#endif
