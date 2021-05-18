/////////////////////////////////////////////////////////////////////////////
// Name:        MonsterEditDlg.h
// Author:      XX
// Created:     XX/XX/XX
// Copyright:   XX
/////////////////////////////////////////////////////////////////////////////

#ifndef __MonsterEditDlg_H__
#define __MonsterEditDlg_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "MonsterEditDlg.cpp"
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "./res/MonsterEdit_wdr.h"

// WDR: class declarations


#include <wx/panel.h>
#include <wx/image.h>

#include <OgreString.h>
#include <OgreImage.h>
#include "PaintAction.h"
#include "WXSceneListener.h"

#include <map>

namespace WX 
{
    class SceneManipulator;
	class CMonsterEditaction;		// 操作不可行走区域场景编辑的类
}

typedef std::map<int, std::string>	ID_NAME_DLG_MAP;//
typedef std::map<int, wxTreeItemId> ID_TREE_ITEM_MAP;
typedef std::map<int, int> ID_LOOK_ID_MAP;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 怪物信息
//
#define SHOP_COUNT	4
class CMonsterInstanceInfo
{


public:
	CMonsterInstanceInfo()
	{
		for(int i = 0; i < SHOP_COUNT; i++)
		{
			m_iShopArray[i] = -1;
		}
	}

	~CMonsterInstanceInfo()
	{

	}

public:
	unsigned long	m_dwInstanceId;			// 每一个实例的id
	
	int				m_iRefreshTime;			// 刷新时间。
	int				m_ActiveType;			// 活动类型。

	float			m_fAreaRadius;			// 如果活动类型是区域，记录活动范围的半径 
	int				m_iLineid;
	
	// 在生成的文件中填写的数据项
	unsigned long	m_dwObjectId;			// 外形id， 通过他可以找到外形文件, 就是怪物的类型。
    int				m_iBaseAIId;			// 基础AI编号。
	int             m_iAdvanceAIId;			// 高级aiId
	int             m_EvenId;				// 事件id
	std::string     m_strInstanceName;		// 怪物实例名字.
	std::string     m_strTitleName;			// 称号名字

	std::string		m_strAdvanceAIFile;		// 特殊ai脚本文件
	std::string		m_strEvent;				// 事件脚本文件.
	unsigned long   m_dwMonsterGUID;		// 怪物的guid
	Ogre::Vector3	m_position;				// 怪物的位置。
	int				m_iGroupId;				// 组id
	int				m_iTeamId;				// 队的id
	float			m_fMonstDir;			// 怪物的方向
	int				m_iReputationID;		// 怪物的势力

	int				m_iLevel;				//怪物的等级
	int				m_iType;				//NPC还是怪物
	int				m_iCampId;				//阵营

	int             m_iShopArray[SHOP_COUNT];



public:

	void DeleteDObject()
	{

	}

};

typedef std::map<unsigned long, CMonsterInstanceInfo> INSTANCE_MONSTERINFO_MAP;






class CMonsterEditDlg : public wxPanel, WX::SceneListener
{


public:

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 要编辑的逻辑数据
	//

		// 怪物实例表
		INSTANCE_MONSTERINFO_MAP		m_MonsterInfoTable;

		// 当前编辑的怪物实例id
		unsigned long		m_ulCurEditMonsterInstance;

		// id 与外形id对应的映射表.
		ID_NAME_DLG_MAP  m_IdNameMap;
		
		// 得到怪物id， 与名字的映射表.
		void LoadMonsterIdNameTable();

		// 已经添加的所有怪物的个数.
		unsigned long m_dwAddedMonsterCount;

		// 是否做了修改.
		bool m_bIsModify;


		// 添加一个怪物
		void AddNewMonster(unsigned long ulMonsterId, unsigned long ulInstanceId);
		
		// 删除一个怪物.
		void DelMonster(unsigned long ulMonsterId, unsigned long ulInstanceId);

		// 删除怪物.
		void DelMonster();

		// 修改选择的怪物的参数.
		void ModifySelectData();

		// 修改选择的怪物的参数.
		void ModifyMultiData();

		// 选择一个怪物.
		void SelMonster(unsigned long ulMonsterId, unsigned long ulInstanceId);

		// 修改选择的怪物的参数.
		void ModifyMonsterInfo();

		// 减少怪物的方向
		void DecMonsterDir();

		// 增加怪物方向。
		void IncMonsterDir();


	//
	// 要编辑的逻辑数据
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////

		



	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 需要使用的控件数据
	//

	// 怪物列表控件。
	wxListCtrl*	m_pMonsterListCtrl;

	// 已经添加的怪物列表。
	wxTreeCtrl* m_pAddedMonsterTreeCtrl;


	//////////////////////////
	//
	
	// 修改按钮.
	wxButton*  m_pModifyBn;

	// 修改多个怪物按钮.
	wxButton*  m_pMultiModifyBn;

	//怪物活动范围按钮.
	wxRadioButton* pArea;
	wxRadioButton* pLine;

	// 活动半径文本。
	wxTextCtrl* m_pAreaText;

	// 活动半径文本。
	wxTextCtrl* m_pRefreshText;

	//路线编辑开关。
	wxCheckBox *m_pLineCheck;

	// 基本ai
	wxChoice*   m_pChoiceBaseAiId;

	// 高级ai
	wxChoice*   m_pChoiceAIFile;

	// 事件脚本
	wxComboBox*   m_pChoiceEvenFile;


	// 商店1
	wxChoice*   m_pShop[SHOP_COUNT];

	// 怪物类型
	wxChoice*   m_pMonsterType;
	// 
	//////////////////////////


	// 修改怪物信息按钮.
	wxButton*  m_pModifyMonsterBn;
	wxButton*  m_pDirDecBn;
	wxButton*  m_pDirIncBn;


	// 活动半径文本。
	wxTextCtrl* m_pMonsterNameText;				// 怪物名字
	wxTextCtrl* m_pMonsterLevelText;			// 怪物等级
    wxTextCtrl* m_pMonsterExpText;				// 怪物经验
	wxTextCtrl* m_pMonsterMoveSpeedText;		// 怪物移动速度
	wxTextCtrl* m_pMonsterfreshTimeText;		// 怪物刷新时间
	wxTextCtrl* m_pMonsterRadiusText;			// 怪物活动半径
	wxTextCtrl* m_pMonsterCampIdText;			// 怪物阵营id
	wxTextCtrl* m_pMonsterGUIDText;			    // 怪物guid
	// 以上是用户只读的选项

	wxTextCtrl* m_pMonsterBaseAIText;			// 怪物基础AI ID
	wxTextCtrl* m_pMonsterAdvanceAIText;		// 怪物高级AI文件。
	wxTextCtrl* m_pMonsterEventText;			// 怪物事件文件
	wxTextCtrl* m_pMonsterGroupIdText;			// 群id， 服务器优化使用
	wxTextCtrl* m_pMonsterTeamIdText;			// 怪物队伍id
	wxTextCtrl* m_pMonsterDirText;				// 怪物方向
	wxTextCtrl* m_pMonsterInstanceName;			// 怪物实例名字
	wxTextCtrl* m_pMonsterTitleName;			// 怪物称号名字
	wxTextCtrl* m_pMonsterPatrol;				// 怪物巡逻路线
	wxTextCtrl* m_pMonsterReputationID;			// 怪物势力id





	// 已经添加怪物树的根节点.
	wxTreeItemId	m_AddedMonsterRoot;

	// 怪物id与添加到树中的item对应的列表。
	ID_TREE_ITEM_MAP	m_Id_TreeItem_Map;

	// 已经添加的怪物表。
	ID_TREE_ITEM_MAP	m_Id_AddedMonsterMap;


	// 当前选择的怪物列表索引
	int m_iCurSelListCtrlIndex;
	
	////////////////////////////////////////////////////////////////////////////////////////
	//
	// 读取文件表操作
	//

	// 得到所有的控件.
	void GetAllControl();

	// 初始化怪物列表控件表头.
	void InitMonsterListCtrlTable();

	// 初始化怪物列表树.
	void InitAddedMonsterTree();

	// 读取基本ai表
	void GetBaseAITable();

	// 读取高级ai
	void GetAIFile();

	// 得到事件脚本
	void GetEventFile();

	// 读取商店信息
	void GetShopInfo();


	// 重置数据信息2006-4-5
	void ReSetDataInfo();
	//
	// 读取文件表操作
	//
	////////////////////////////////////////////////////////////////////////////////////////

	void InitMonsterType();



	// 是否禁止区域编辑
	void EnableAreaEdit(bool bEnable = true);

	// 显示选中的怪物的信息.
	void ShowMonsterInfo( unsigned long ulInstanceId);

	// 显示选中怪物类型的信息.
	void ShowMonsterTypeInfo(int iSelIndex);

	// 通过索引显示怪物的信息
	void ShowMonsterTypeInfoByIndex(int iSelIndex);

	// 通过treeitem得到怪物外形id
	unsigned long GetMonsterIdByTreeItem(wxTreeItemId treeItem);

	// 清空界面.
	void ClearUI();

	// 清空action数据
	void ClearActionData();

	// 清空计数器
	void ClearCounter();

	//
	// 需要使用的控件数据
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////




public:

	////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 场景操作
	//
	WX::SceneManipulator*	m_pSceneManipulator;
	WX::CMonsterEditaction*	m_pMonsterEditAction;

	// 得到action
	void GetAction();

	// 设置场景操作句柄
	void SetSceneManipulator(WX::SceneManipulator* sceneManipulator);


	// 激活场景
	void SetActive(bool bShow);

public:

	CMonsterEditDlg(const wxString& resourceGroup = wxT("MonsterEdit"));

	CMonsterEditDlg(
					wxWindow *parent, 
					const wxString& resourceGroup = wxT("MonsterEdit"),
					wxWindowID winid = wxID_ANY,
					const wxPoint& pos = wxDefaultPosition,
					const wxSize& size = wxDefaultSize,
					long style = wxTAB_TRAVERSAL | wxNO_BORDER,
					const wxString& name = "MonsterEdit"
					);


	
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
                const wxString& name = "MonsterEdit");


	// 显示窗口
	bool Show(bool show);

	
	//
	// 初始化函数 清除函数。
	//
	//****************************************************************************************************/
										

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 控件的事件响应函数。
	//

		// 选择一个类型的怪物。
		 void OnSelected(wxListEvent& event);

		 // 修改怪物参数按钮.
		void OnModifyData( wxCommandEvent &event );

		// 修改多个怪物参数的按钮.
		void OnModifyMultiData( wxCommandEvent &event );

		// 修改怪物信息
		void OnModifyMonsterInfo( wxCommandEvent &event );

		// 减少怪物的方向
		void OnDecMonsterDir( wxCommandEvent &event );

		// 增加怪物方向。
		void OnIncMonsterDir( wxCommandEvent &event );

		void OnEditReputationID( wxCommandEvent &event );

		void OnSelectedType( wxCommandEvent &event );

		void OnSelectedAIFile( wxCommandEvent &event );
	//
	//
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 界面通知回掉函数
	//

	// 添加一个怪物
	virtual void onAddNewMonster(unsigned long ulMonsterId, unsigned long ulInstanceId);

	// 删除一个怪物
	virtual void onDelMonster( unsigned long ulMonsterId, unsigned long ulInstanceId);

	// 选择一个怪物
	virtual void onSelMonster( unsigned long ulMonsterId, unsigned long ulInstanceId);

	//
	// 界面通知回掉函数
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 得到怪物信息.
	//

	// 得到guid
	unsigned long GetGuid();

	// 得到基础ai
	int GetBaseAIId();

	// 得到高级ai
	int GetAdvanceAIId();

	// 得到type
	int GetMonsterType();

	// 得到阵营
	int GetCamp();

	// 得到事件id
	int GetEvenId();

	// 得到组id
	int GetGroupId();

	// 得到队伍id
	int GetTeamId();

	// 得到实例名字
	std::string GetInstanceName();

	// 得到称号名字
	std::string GetTitleName();

	int GetLevel();

    // 得到方向
	float GetDir();

	// 得到刷新时间
	int GetRefreshTime();

	// 得到位置
	Ogre::Vector3 GetMonsterPos();

	// 得到巡逻路线
	int GetPatrolId();

	// 得到商店索引
	int GetShopIndex(int iIndex);

	int GetReputationID();

	// 读取怪物
	unsigned long ReadMonsterFromFile(std::string& strFileName);

	// 存储怪物
	unsigned long SaveMonsterToFile(std::string& strFileName);

	//
	// 得到怪物信息.
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	

	

	DECLARE_EVENT_TABLE()
};


#endif
