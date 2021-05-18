#ifndef __PathFindDLG_H__
#define __PathFindDLG_H__
/////////////////////////////////////////////////////////////////////////////
// Name:        PathFindDlg.h
// Author:      XX
// Created:     XX/XX/XX
// Copyright:   XX
/////////////////////////////////////////////////////////////////////////////


#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "PathFindDlg.cpp"
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <OgreString.h>
#include <OgreImage.h>
#include "PaintAction.h"
#include "WXSceneListener.h"

namespace WX 
{

	class SceneManipulator;
	class CPathFindAction;

}

class CPathFindEditDlg : public wxPanel, WX::SceneListener
{

	///////////////////////////////////////////////////////////////////////////////////////////
	//
	// 构造函数。
	//
public:


public:

	// 不可行走区域列表
	wxListBox* m_pListboxPointInRegion;

	// 删除点的按钮.
	wxButton*  m_pDelPointBn;

	wxTextCtrl* m_pPointX;
	wxTextCtrl* m_pPointZ;


	// 场景操作
	WX::SceneManipulator* m_pSceneManipulator;

	WX::CPathFindAction* m_pAction;

public:

	CPathFindEditDlg(const wxString& resourceGroup = wxT("PathFindEdit"));


	CPathFindEditDlg(
		wxWindow *parent, 
		const wxString& resourceGroup = wxT("PathFindEdit"),
		wxWindowID winid = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxTAB_TRAVERSAL | wxNO_BORDER,
		const wxString& name = "PathFindEdit");


	~CPathFindEditDlg(void);


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
		const wxString& name = "PathFindEdit");


	// 初始化逻辑数据
	void InitData();

	// 清空数据
	void ClearData();

	// 从RegionAction中刷新数据, 读盘的时候使用
	void RefreshData();

	// 清除ui界面的数据
	void ClearUiData();

	// 读取数据
	void LoadData(std::string& strFile);

	// 存储数据
	void SaveData(std::string strFile);

	// // 得到区域编辑的场景控制
	void GetRegionAction();



public:

	////////////////////////////////////////////////////////////////////////////////////////
	//
	// 不可行走区域数据.
	//
	unsigned long m_ulCurRegionId;		// 当前编辑的区域id.
	unsigned long m_ulCurPointId;		// 当前编辑的点的id.
	unsigned long m_ulCurLineId;		// 当前编辑的线的id.

	bool		  m_bIsChangeCamera;	    // 是否改变摄像机位置。
	bool		  m_bIsEditingRegion;		// 是否正在编辑区域。

	int			  m_iCurSelRegionIndex;

	/*******/
	/////////////////////////////////////////////////////////////////////////////////////////
	//
	// 界面操作响应函数
	//


	// 设置正在编辑区域标记。
	void SetEditRegionFlag(bool bIsEdit);

	// 按钮函数， 删除一整个区域.
	//void OnDelRegion( wxCommandEvent &event );

	// 按钮函数， 删除一整个区域.
	//void OnAddNewRegion( wxCommandEvent &event );

	// 完成一个区域的编辑
	//void OnFinishRegion( wxCommandEvent &event );

	// 事件函数,  当这个面板激活的时候调用
	bool Show(bool show = true);

	// 点选择listbox响应的事件函数
	void OnPointListBox(wxCommandEvent &event);

	// 删除最后一个点, 其实调用的是回退操作。
	void OnDelPointInRegion( wxCommandEvent &event );

	void OnEditXYZ( wxCommandEvent &event );

	/*******/
	///////////////////////////////////////////////////////////////////////////////////////////
	//
	// 逻辑数据操作
	//

	// 添加一个区域.
	void AddNewRegion();

	// 在当前的区域添加一个新的点
	void RegionAddNewPoint(unsigned long ulId, bool bRefresh = false);


	///////////////////////////////////////////////////////////////////////////////////////////
	//
	// 点的逻辑操作
	//

	// 选中点的操作
	void DoSelPoint();

	// 把点的信息刷新到界面上
	void RefreshRegionPointData(unsigned long ulRegionId);

	// 删除区域中的一个点
	void DelRegionPoint(unsigned long ulPointId);





	/******/
	/////////////////////////////////////////////////////////////////////////////////////////
	//
	// 场景操作
	//

	// 激活区域编辑操作
	void SetActive(bool bShow);

	// 激活
	void ActiveAction(bool bActive);


	// 设置场景操作句柄.
	void SetSceneManipulator(WX::SceneManipulator* sceneManipulator);

public:

	/***********************/
	//////////////////////////////////////////////////////////////////////////////////////////
	//
	// 界面回掉函数操作
	//

	virtual void onAddDijkPoint(unsigned long ulPointId, Ogre::Vector3 position);

	virtual void onDelDijkPoint(unsigned long ulPointId);

	virtual void onSelDijkPoint(unsigned long ulPointId);

	DECLARE_EVENT_TABLE()

};




#endif

