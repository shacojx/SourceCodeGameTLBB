/////////////////////////////////////////////////////////////////////////////
// Name:        CEventAreaEditDlg.h
// Author:      XX
// Created:     XX/XX/XX
// Copyright:   XX
/////////////////////////////////////////////////////////////////////////////

#ifndef __CEventAreaEditDlg_H__
#define __CEventAreaEditDlg_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "CEventAreaEditDlg.cpp"
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "res/EventArea_wdr.h"

// WDR: class declarations

#include <OgreString.h>
#include <OgreImage.h>
#include "PaintAction.h"
#include "WXSceneListener.h"
#include <string>

namespace WX 
{
    
	class SceneManipulator;
	class CEventAreaEditAction;
	
}

class CEventAreaEditDlg : public wxPanel, WX::SceneListener
{

///////////////////////////////////////////////////////////////////////////////////////////
//
// 构造函数。
//
public:

	

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 界面控件
//
//

public:

	 // 已经添加区域列表
	 wxListCtrl*	m_pRectListCtrl;

	 // 添加新id按钮.
	 wxButton*  m_pAddNewEventAreaIdBn;

	  // 添加Select
	 wxButton*  m_pSelectBn;

	  // 添加Draw
	 wxButton*  m_pDrawBn;


	 // id控件
	wxTextCtrl* m_pEventIdText;		

	// 脚本控件
	wxTextCtrl* m_pScriptidText;		

	 // 确定完成编辑区域按钮.
	 //wxButton*  m_pRegionOkBn;

	 // // 删除点的按钮.
	 //wxButton*  m_pDelRegionBn;

	 // // 确定完成编辑区域按钮.
	 //wxButton*  m_pAddNewRegionBn;


	//得到脚本id
	std::string GetScriptIdValue();

	// 清空界面
	void ClearUI();

	// 清空界面
	void RefreshDataUI();

	 
	 // 场景操作
	 WX::SceneManipulator* m_pSceneManipulator;

	 // 对应的action
	 WX::CEventAreaEditAction* m_pEditAction;

public:
	
	CEventAreaEditDlg(const wxString& resourceGroup = wxT("EventAreaEdit"));
  

    CEventAreaEditDlg(
							wxWindow *parent, 
							const wxString& resourceGroup = wxT("EventAreaEdit"),
							wxWindowID winid = wxID_ANY,
							const wxPoint& pos = wxDefaultPosition,
							const wxSize& size = wxDefaultSize,
							long style = wxTAB_TRAVERSAL | wxNO_BORDER,
							const wxString& name = "EventAreaEdit");
   
	
	~CEventAreaEditDlg(void);


public:


	/******/
	/////////////////////////////////////////////////////////////////////////////////
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
                const wxString& name = "EventAreaEdit");

	
	// 初始化数据
	void Init(const wxString& resourceGroup);

	// 初始化逻辑数据
	void InitData();

	// 清空数据
	void ClearData();

	// 从RegionAction中刷新数据, 读盘的时候使用
	void RefreshData();

	// 清除ui界面的数据
	void ClearUiData();

	// 读取数据
	void LoadData(std::string strFile);

	// 存储数据
	void SaveData(std::string strFile);

	// // 得到区域编辑的场景控制
	void GetEventAreaAction();
	
	
	// 得到action
	void GetAction();


	

public:

/*******/
	/////////////////////////////////////////////////////////////////////////////////////////
	//
	// 界面操作响应函数
	//

	
	// 按钮函数， 删除一整个区域.
	void OnAddNewId( wxCommandEvent &event );

	// 按钮函数， 选择
	void OnSelect( wxCommandEvent &event );

	// 按钮函数， 选择
	void OnDraw( wxCommandEvent &event );

	// 按钮函数， 删除一个区域
	void OnDeleteRect( wxCommandEvent &event );

	// 选择一个生长点的类型
	 void OnSelectEventAreaRect(wxListEvent& event);

	// 按钮函数， 增加一个新id
	void OnModifyArea( wxCommandEvent &event );





	/*******/
	///////////////////////////////////////////////////////////////////////////////////////////
	//
	// 逻辑数据操作
	//

	// 当前的区域id
	int m_CurEventAreaId;

	// 但当前的脚本id
	int m_iCurScriptId;

	// 操作类型.
	int m_OperatroType;


	// 设置编辑action的脚本id
	void SetActionScriptValue();



	/******/
	/////////////////////////////////////////////////////////////////////////////////////////
	//
	// 场景操作
	//

	// 激活区域编辑操作
	void SetActive(bool bShow);

	// 设置场景操作句柄.
	void SetSceneManipulator(WX::SceneManipulator* sceneManipulator);

	// 显示窗口.
	bool Show(bool show);


public:
   
	/***********************/
	//////////////////////////////////////////////////////////////////////////////////////////
	//
	// 界面回掉函数操作
	//

	virtual void onAddNewRect(unsigned long ulTypeId, unsigned long ulInstanceId);

	// 删除一个怪物
	virtual void onDelRect( unsigned long ulTypeId, unsigned long ulInstanceId);
	
	// 选择一个怪物
	virtual void onSelRect( unsigned long ulTypeId, unsigned long ulInstanceId);
	


	DECLARE_EVENT_TABLE()
	
	
};



#endif
