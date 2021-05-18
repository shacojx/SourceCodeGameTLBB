#ifndef __REGIONEDITDLGSECTOR_H__
#define __REGIONEDITDLGSECTOR_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "RegionEditDlgSelector.h"
#endif

#include <wx/panel.h>
#include <wx/image.h>

#include <OgreString.h>
#include <OgreImage.h>
#include "PaintAction.h"
#include "WXSceneListener.h"

namespace WX 
{
    class SceneManipulator;
	class CRegionEditAction;		// 操作不可行走区域场景编辑的类
}

class CRegionEditDlgSector : public wxPanel, WX::SceneListener
{

public:

	 // 不可行走区域列表
	 wxListBox* m_pListboxRegion;

	  // 不可行走区域列表
	 wxListBox* m_pListboxPointInRegion;

	 // 删除点的按钮.
	 wxButton*  m_pDelPointBn;

	  // 确定完成编辑区域按钮.
	 wxButton*  m_pRegionOkBn;

	  // 删除点的按钮.
	 wxButton*  m_pDelRegionBn;

	  // 确定完成编辑区域按钮.
	 wxButton*  m_pAddNewRegionBn;

	  // 是否能飞过的标记
	 wxTextCtrl* m_pFlyableCB;
	 
	 // 场景操作
	 WX::SceneManipulator* m_pSceneManipulator;

	 WX::CRegionEditAction* m_pRegionEditAction;

public:
	
	CRegionEditDlgSector(const wxString& resourceGroup = wxT("RegionEdit"));
  

    CRegionEditDlgSector(
							wxWindow *parent, 
							const wxString& resourceGroup = wxT("RegionEdit"),
							wxWindowID winid = wxID_ANY,
							const wxPoint& pos = wxDefaultPosition,
							const wxSize& size = wxDefaultSize,
							long style = wxTAB_TRAVERSAL | wxNO_BORDER,
							const wxString& name = "RegionEdit");
   
	
	~CRegionEditDlgSector(void);


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
                const wxString& name = "RegionEdit");

	
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
	void OnDelRegion( wxCommandEvent &event );

	// 按钮函数， 删除一整个区域.
	void OnAddNewRegion( wxCommandEvent &event );

	// 完成一个区域的编辑
	void OnFinishRegion( wxCommandEvent &event );

	// 事件函数,  当这个面板激活的时候调用
	bool Show(bool show = true);

	// 当区域列表框选项改变时调用.
	void OnRegionListBox(wxCommandEvent &event);

	// 点选择listbox响应的事件函数
	void OnPointListBox(wxCommandEvent &event);

	// 删除最后一个点, 其实调用的是回退操作。
	void OnDelPointInRegion( wxCommandEvent &event );

	// 区域是否轻功通过标记
	void OnFlyable( wxCommandEvent &event );

	//// 按钮函数， 删除一个点.
	//void OnDelPointInRegion( wxCommandEvent &event );

	/*******/
	///////////////////////////////////////////////////////////////////////////////////////////
	//
	// 逻辑数据操作
	//

	// 删除一个区域
	void DelRegion(unsigned long ulId);

	// 添加一个区域.
	void AddNewRegion();

	// 在当前的区域添加一个新的点
	void RegionAddNewPoint(unsigned long ulId, bool bRefresh = false);

	// 处理当前选中的区域
	void DoSelRegion();

	// 完成一个区域的编辑
	void FinishRegion();


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

	void DelRegion();





	/******/
	/////////////////////////////////////////////////////////////////////////////////////////
	//
	// 场景操作
	//

	// 激活区域编辑操作
	void SetActive(bool bShow);

	// 设置场景操作句柄.
	void SetSceneManipulator(WX::SceneManipulator* sceneManipulator);

public:
   
	/***********************/
	//////////////////////////////////////////////////////////////////////////////////////////
	//
	// 界面回掉函数操作
	//
	// 添加一个物体， 暂时没用
    virtual void onAddObject(const WX::ObjectPtr& object);

	//不可行走区域回掉函数
	virtual void onRegionAddNewPoint(unsigned long ulRegionId,unsigned long ulPointId, Ogre::Vector3 position);

	// 删除一个点的回调函数
	virtual void onRegionDelPoint(unsigned long ulRegionId,unsigned long ulPointId);

	// 选中一个区域
	virtual void onRegionSel(unsigned long ulRegionId);

	DECLARE_EVENT_TABLE()
};

extern CRegionEditDlgSector* g_pRegionEdit;

#endif //__REGIONEDITDLGSECTOR_H__