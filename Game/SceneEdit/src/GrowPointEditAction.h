#pragma once

#include "Action.h"
#include "HitInfo.h"
#include <OgreMaterial.h>
#include <map>
#include <vector>
#include "LogicDataStructDef.h"

namespace WX 
{

	class LogicModel;

	#define		FUNCTION_KEY        0X00000000
	#define		FUNC_KEY_SHIFT		0X00000001
	#define		FUNC_KEY_CTRL		0X00000002
	#define		FUNC_KEY_ALT		0X00000004
	
	class CGrowPointInfo
	{
	public:
		CGrowPointInfo();
		~CGrowPointInfo();

	public:

		bool			m_bIsRecreateMaterial;	// 是否重新创建纹理
		unsigned long	m_dwInstanceId;			// 每一个实例的id
		unsigned long	m_dwTypeId;				// 每一个实例的id
	

		// 父节点scene Node
		Ogre::SceneNode*	m_pParentSceneNode;	

		// 用来显示区域编辑的总的节点.
		Ogre::SceneNode*	m_pSceneNode;	
		Ogre::Entity*		m_pEntity;
		Ogre::Vector3		m_position;			// 怪物的位置。
		WX::LogicModel*	m_pModel;

	public:

		// 创建一个生长点。
		bool CreateGrowPoint(const std::string& strMeshName);

		// 设置位置
		void SetGrowPointPos(Ogre::Vector3 pos);

		// 设置父节点
		void SetParentSceneNode(Ogre::SceneNode* pParentSceneNode);

		// 释放生长点的资源
		void ReleaseGrowPoint();

		// 是否选中.
		bool IsSelected(Ogre::Ray& ray, float& fdis);



		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//	材质操作。
		//

		// object 选中时使用的材质。
		Ogre::MaterialPtr m_pSelectedMaterial;

		// object 原来的材质.
		Ogre::MaterialPtr m_pOldMaterial;

		// 选中
		void SetSelMaterial();

		// 未选中.
		void SetUnSelMaterial();

	

	};


	typedef std::multimap<unsigned long, CGrowPointInfo>	SORT_GROWPOINT_MAP;
	typedef std::map<unsigned long, CGrowPointInfo>			GROWPOINT_MAP;
	typedef std::map<unsigned long, std::string>			ID_MESH_MAP;
	typedef std::pair<unsigned long, CGrowPointInfo >		GROWPOINT_PAIR;


	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	action
	//
	class CGrowPointEditAction: public Action
	{
	public:
		CGrowPointEditAction(SceneManipulator* sceneManipulator);
		~CGrowPointEditAction(void);

	public:

		///////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// 场景操作.
		//
		//

		// 是否显示临时选中的模型。
		bool  m_bIsShowTempObject;


		// 用来显示区域编辑的总的节点.
		CGrowPointInfo m_tempGrowPoint;


		// 屏幕鼠标坐标
		Point m_screenPt;

		// 是否显示场景.
		bool  m_bIsShow;

		// 当前的鼠标点
		Ogre::Vector3	m_curPoint;

		// 用来显示区域编辑的总的节点.
		Ogre::SceneNode*	m_pSceneNode;	

		// 创建区域编辑的场景节点
		void CreateSceneNode();

		// 隐藏区域编辑场景节点
		void HideScene(bool bHide);

		// 新添加的生长点的编号。
		unsigned long m_dwGrowPointInstanceId;

		// 当前选中的生长点
		unsigned long m_dwCurSelGrowPointInstanceId;

		// 当前编辑的生长点的类型id
		unsigned long m_dwGrowPointTypeId;
		
		// 生长点信息.
		GROWPOINT_MAP	m_GrowPointMap;

		// 生长点类型和模型映射文件。
		ID_MESH_MAP		m_typeMeshMap;

		// 添加类型。
		void AddGrowPointMeshType(unsigned long dwId, std::string strName);

		// 改变类型。
		void ChangeGrowPointMeshType(unsigned long dwId, std::string strName);

		// 重置计数器
		void ResetCounter();


		// 添加生长点。
		// 返回值0xffffffff 表示创建失败
		unsigned long AddNewGrowPoint(unsigned long dwGrowPointTypeId, Ogre::Vector3 pos);


		// 添加生长点。
		// 返回值0xffffffff 表示创建失败
		unsigned long AddNewGrowPoint(const std::string& strMeshName, Ogre::Vector3 pos);

		// 设置生长点的位置
		void SetGrowPointPos(unsigned long dwGrowPointInstanceId, Ogre::Vector3 pos);

		// 得到生长点的位置
		Ogre::Vector3 GetGrowPos(unsigned long dwGrowPointInstanceId);

		// 得到生长点逻辑位置
		Ogre::Vector3 GetGrowLogicPos(unsigned long dwGrowPointInstanceId);

		// 看一个生长点.
		void ViewGrowPos(unsigned long dwGrowPointInstanceId);

		// 是否选中生长点.
		bool IsSelectObject(Ogre::Ray& ray, CGrowPointInfo& GrowPointInfo, float& fdis);

		// 是否选中生长点.
		bool IsSelectObject(Ogre::Ray& ray, unsigned long dwGrowPointInstanceId, float& fdis);

		// 得到选中的生长点
		bool GetSelectedObject(unsigned long& dwGrowPointInstanceId);

		// 是否选中生长点.
		void SetSelectObjectLook(unsigned long dwGrowPointInstanceId);

		// 是否选中生长点.
		void SetUnSelectObjectLook(unsigned long dwGrowPointInstanceId);

		// 删除生长点。
		void DelGrowPoint(unsigned long dwGrowPointInstanceId);

		// 清空生长点数据
		void ClearGrowPointData();
	
		/////////////////////////////////////////////////////////////////////////////////////////////
		//
		// 功能键数据操作。
		//
		//
	public:

		// 记录键盘的按键状态.
		unsigned long     m_FuncKeyState;
		
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




		

		////////////////////////////////////////////////////////////////////////////////////
		//
		// 存盘和读盘操作
		//

		// 从区域读取文件
		unsigned long ReadGrowPointFromFile(std::string strFileName);
		
		// 把区域存储到文件
		unsigned long SaveGrowPointToFile(std::string strFileName);

			
		//
		// 存盘和读盘操作
		//
		////////////////////////////////////////////////////////////////////////////////////


			
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

		// 得到这个区域的名字.
		const String& getName(void) const;

	};

}
