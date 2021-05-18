#pragma once

#include "Action.h"
#include "HitInfo.h"
#include "ScanLine.h"
#include <OgreMaterial.h>
#include <map>
#include <vector>
#include "LogicDataStructDef.h"

class CEditDobject_NT;
namespace WX 
{
	class LogicModel;

	#define		FUNCTION_KEY        0X00000000
	#define		FUNC_KEY_SHIFT		0X00000001
	#define		FUNC_KEY_CTRL		0X00000002
	#define		FUNC_KEY_ALT		0X00000004
	

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 怪物信息定义
	//

	#define		REFRESH_TIME		1000		// 怪物刷新时间， 单位毫秒
	#define     AREA_RADIUS			2			// 活动半径1.
	#define		SHOP_COUNT			4			// 商店个数.

	typedef std::vector<Ogre::Vector3> LINE_VECTOR;
	class CMonsterInfo
	{

	public:

		enum ACTIVE_TYPE
		{
			AREA = 0,
			LINE,
		};


	public:
		CMonsterInfo();
		~CMonsterInfo();

	public:
		unsigned long	m_dwInstanceId;			// 每一个实例的id
		

		// 用来显示的object
		CEditDobject_NT*     m_pDobject;
		WX::LogicModel*	m_pModel;

		int				m_iRefreshTime;			// 刷新时间。
		int				m_ActiveType;			// 活动类型。

		float			m_fAreaRadius;			// 如果活动类型是区域，记录活动范围的半径 
		LINE_VECTOR		m_lineVector;			// 如果活动类型是路线，记录活动范围的路线

		// 在生成的文件中填写的数据项
		unsigned long	m_dwObjectId;			// 外形id， 通过他可以找到外形文件, 就是怪物的类型。
       	int				m_iBaseAIId;			// 基础AI编号。
		std::string		m_strAdvanceAIFile;		// 特殊ai脚本文件
		std::string		m_strEvent;				// 事件脚本文件.
		unsigned long   m_dwMonsterGUID;		// 怪物的guid
		Ogre::Vector3	m_position;				// 怪物的位置。
		int				m_iGroupId;				// 组id
		int				m_iTeamId;				// 队的id
		float			m_fMonstDir;			// 怪物的方向
		int				m_iReputationID;		// 怪物的势力
		int				m_iLevel;				// 怪物的等级

		int				m_iShop[SHOP_COUNT];	// 商店			

	

	public:

		void DeleteDObject();

	};
	typedef std::map<unsigned long, CMonsterInfo> MONSTERINFO_MAP;
	

	//
	// 怪物信息定义
	//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	class CMonsterEditaction : public Action
	{


	public:

		 CMonsterEditaction(SceneManipulator* sceneManipulator);
    	~CMonsterEditaction(void);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 成员数据定义.
	//
	public:


		// 点击怪物的控制时间;
		unsigned long m_ulAddtime;
		// 是否添加操作。
		bool m_bIsAddOperator;

		// 用来显示区域编辑的总的节点.
		Ogre::SceneNode*	m_pSceneNode;			

		// 临时选中的物体对象.
		CEditDobject_NT* m_tempObject;

		// 记录键盘的按键状态.
		unsigned long     m_FuncKeyState;

		// 是否显示场景.
		bool  m_bIsShow;

		// 怪物列表。
		MONSTERINFO_MAP	m_MonsterInfoMap;

		// 注册在地图中的怪物的列表.
		POS_ID_MAP		m_MonsterPosMap;

		// 当前可以添加怪物实例id, 添加新的怪使用
		// 
		// 从 0 开始， 0xffffffff表示不合法id 
		unsigned long m_dwMonsterInstanceId;

		// 当前编辑的怪物的名称。
		String	m_strCurEditMonsterName;

		// 当前编辑的怪物外形的id
		unsigned long   m_dwCurEditMonsterId;

		// 从 0 开始， 0xffffffff表示不合法id 
		unsigned long m_dwCurSelMonsterInstanceId;

		// 当前编辑的怪物外形的id
		unsigned long   m_dwGUID;

		// 怪物id与外形名字映射表。
		ID_NAME_MAP m_IdNameMap;

		// id与外形映射表
		ID_ID_MAP   m_Id_LookId_Map;

		// 得到怪物id， 与名字的映射表.
		void LoadMonsterIdNameTable();

		// 得到怪物类型表
		void LoadMonsterTypeTable();

		// 当前的鼠标点
		Ogre::Vector3	m_curPoint;


	//
	// 成员数据定义.
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////////////////////////////
	//
	// 存盘和读盘操作
	//

	// 从区域读取文件
	unsigned long ReadEditMonsterFromFile(std::string strFileName);
	
	// 把区域存储到文件
	unsigned long SaveEditMonsterToFile(std::string strFileName);

		
	//
	// 存盘和读盘操作
	//
	////////////////////////////////////////////////////////////////////////////////////




	/////////////////////////////////////////////////////////////////////////////////////////
	//
	// 场景的操作
	//

	
	// 创建区域编辑的场景节点
	void CreateSceneNode();

	// 隐藏区域编辑场景节点
	void HideScene(bool bHide);

	// 根据地形的高度改变编辑的区域。
	void SetMonsterHeight();

	// 摄像机的位置操作.
	void SetCamera(float fx, float fy);

	//
	// 场景的操作
	//
	/////////////////////////////////////////////////////////////////////////////////////////
	
	

	/////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 功能键数据操作。
	//
	//

	
	// 设置操作参数.
	void setParameter(const String& name, const String& value);

	// 设置按下的功能键
	void SetFuncKey(unsigned long ulFuncKey);

	//  清空功能键.
	void ClearFuncKey();


	//
	//
	// 功能键数据操作。
	//
	///////////////////////////////////////////////////////////////////////////////////////////////

	
	protected:
    

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 外部调用的虚函数。
	//

	// 鼠标操作的虚函数.
    void _onMove(const Point& pt);
    void _onBegin(const Point& pt);
    void _onDrag(const Point& pt);
    void _onEnd(const Point& pt, bool canceled);

	// 当前激活active操作.
    void _onActive(bool active);

	//
	// 外部调用的虚函数。
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	

	public:
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 怪物编辑操作。
	//

	//设置新的编辑模型。
	void SetNewEditObject(String& name, unsigned long dwObjectId);

	//设置新的编辑模型。
	void SetNewEditObject(const char* pName, unsigned long dwObjectId);

	// 更换模型。
	void ChangeModel(String& name);

	// 更换模型。
	void ChangeModel(const char* pName);

	// 添加一个怪物
	bool AddNewMonster(const char* pName,  Ogre::Vector3& pos, unsigned long dwObjectId);

	// 添加一个怪物
	bool AddNewMonster(String& name,  Ogre::Vector3& pos, unsigned long dwObjectId);

	// 添加一个怪物
	bool AddNewMonster(Ogre::Vector3& pos, unsigned long dwObjectId, bool bTransPos = false);

	// 初始化编辑的数据
	void InitEditData();

	// 释放所有的怪物。
	void ReleaseAllMonster();

	// 得到选中的怪物
	unsigned long  SelectObject( const Point& pt);

	// 是否选中怪物
	bool IsSelectObject(Ogre::Ray& ray, CMonsterInfo& monsterInfo, float& fdis);

	// 进行选择物体的操作
	void DoSelectObject(const Point& pt);

	// 进行添加物体的操作
	void DoAddNewMonster(const Point& pt);

	// 删除物体
	void DelSelectMonster();

	
	// 删除物体
	void DelMonster(unsigned long dwInstanceId);

	// 进行删除物体的操作
	void DoDelMonster(const Point& pt);

	// 根据怪物实例id， 得到怪物的信息指针
	CMonsterInfo* GetMonster(unsigned long dwInstanceId);

	// 改变所有的怪物的刷新时间。
	void ChangeAllMonsterRefreshTime(int iTime = 1000);

	// 改变所有怪物的活动半径。
	void ChangeAllMonsterRadius(float fRadius = 2.0f);

	// 改变刷新时间和活动半径。
	void ChangeAllMonsterRefreshTimeRadius(int iTime = 1000, float fRadius = 2.0f);

	// 改变某一类怪的刷新时间。
	void ChangeMonsterRefreshTime_Type(unsigned long dwObjectId, int iTime = 1000);

	// 改变某一类怪的活动半径。
	void ChangeMonsterRefreshRadius_Type(unsigned long dwObjectId, float fRadius = 2.0f);

	// 改变某一类怪的刷新时间和活动半径。
	void ChangeMonsterRefreshTimeRadius_Type(unsigned long dwObjectId, int iTime = 1000, float fRadius = 2.0f);

	// 改变怪物的刷新时间。
	void ChangeMonsterRefreshTime(unsigned long dwInstanceId, int iTime = 1000);

	// 改变怪物的刷新半径。
	void ChangeMonsterRefreshRadius(unsigned long dwInstanceId, float fRadius = 2.0f);

	// 改变怪物的刷新时间和半径。
	void ChangeMonsterRefreshTimeRadius(unsigned long dwInstanceId, int iTime = 1000, float fRadius = 2.0f);

	// 改变怪物的方向
	void ChangeMonsterDir(unsigned long dwInstanceId, float radian);

	//清空计数器
	void ClearCounter();

	// 设置已经修改
	void SetModifyed(bool bModifyed = true);

	// 得到id和外形映射表
	void GetMonsterIdLookTable();

	// 得到怪物的位置
	Ogre::Vector3   GetMonsterPos(unsigned long ulInstance);



	//
	// 怪物编辑操作。
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

		
	public:

		// 得到这个区域的名字.
		const String& getName(void) const;

		
	};
}// namespace WX 
