#ifndef __EVENTAREAEDITACTION_H__
#define __EVENTAREAEDITACTION_H__
#pragma once


#include "Action.h"
#include "HitInfo.h"
#include "ScanLine.h"
#include <OgreMaterial.h>
#include <map>
#include "LogicDataStructDef.h"
#include "CustomShape.h"
#include "eventareainfo.h"

namespace WX 
{
	#define		FUNCTION_KEY        0X00000000
	#define		FUNC_KEY_SHIFT		0X00000001
	#define		FUNC_KEY_CTRL		0X00000002
	#define		FUNC_KEY_ALT		0X00000004

	typedef struct  _EVENT_RECT_INFO
	{
		int			m_iEventAreaId;
		int			m_iScriptId;
		AREARECT	m_rect;
	}EVENT_RECT_INFO, *PEVENT_RECT_INFO;

	
	typedef enum _AREA_EDIT_OPERATRO_TYPE
	{

		SELECT,
		DRAW,

	}AREA_EDIT_OPERATRO_TYPE;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 事件区域信息
	//
	//
	class CEventAreaRectInfo
	{

	public:

		CEventAreaRectInfo();
		~CEventAreaRectInfo();

	public:

		int			m_iEventAreaId;
		int			m_iScriptId;
		AREARECT	m_rect;

	public:

		Ogre::SceneNode*	m_pParentSceneNode;		// 用来显示区域编辑的总的节点.
		Ogre::SceneNode*	m_pSceneNode;			// 点的场景节点.
		
		CCustomShape*		m_RectRenderShape;		// 显示信息.
		SceneManipulator*   m_pSceneManipulator;	// 用来对ogre操作的句柄。

		
	public:
		
		/////////////////////////////////////////////////////////////////////////////////
		//
		// 区域管理操作
		//

		// 设置父场景节点.
		void SetParentSceneNode(Ogre::SceneNode* pSceneNode);

		// 设置SceneManipulator句柄
		void SetSceneManipulator(SceneManipulator*  SceneManipulator);

		// 创建场景节点模型.
		unsigned long CreateSceneNode();

		// 创建显示的矩形
		//void BuildEventAreaRect();


	};






	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 事件区域action
	// 
	//

	typedef std::map<int, int>					EVENT_AREA_ID_RECT_ID_MAP;
	typedef std::map<int, EVENT_RECT_INFO>		ID_RECT_MAP;
	typedef std::map<int, CEventAreaInfo*>		SHOW_RECT_MAP;

	// 事件区域编辑
	class CEventAreaEditAction : public Action
	{
	public:
		 CEventAreaEditAction(SceneManipulator* sceneManipulator);
		~CEventAreaEditAction(void);


	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// 键盘输入操作。
		//

		// 记录键盘的按键状态.
		unsigned long     m_FuncKeyState;

		// 是否显示场景.
		bool  m_bIsShow;

			
		// 设置按下的功能键
		void SetFuncKey(unsigned long ulFuncKey);

		//  清空功能键.
		void ClearFuncKey();


	public:
		////////////////////////////////////////////////////////////////////////////////////
		//
		// 存盘和读盘操作
		//

		// 从区域读取文件
		unsigned long ReadEventAreaFromFile(std::string strFileName);
		
		// 把区域存储到文件
		unsigned long SaveEventAreaToFile(std::string strFileName);
		
			
	public:

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// action 操作。
		//
		
		// 得到这个action的名字.
		const String& getName(void) const;

		// 设置操作参数.
		void setParameter(const String& name, const String& value);

	public:

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// 响应相应的鼠标操作.
		//

		void OnMouseMove(const Point& pt);

		void OnMouseLBnDown(const Point& pt);

		void OnMouseLBnUp(const Point& pt);

		void OnMouseRBnDown(const Point& pt);

		void OnMouseRBnUp(const Point& pt);

		void OnMouseDrag(const Point& pt);


	public:
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//
		// 与渲染层相关的操作.
		//
		
		Ogre::SceneNode*	m_pSceneNode;			// 用来显示区域编辑的总的节点.

		CCustomShape*		m_pLineShape;  

		// 鼠标指示小球.	
		HitInfo mTerrainHitInfo;

		// 创建区域编辑的场景节点
		void CreateSceneNode();

		// 隐藏区域编辑场景节点
		void HideScene(bool bHide);

		// 创建显示的区域
		void CreateShowEventArea();

		// 根据矩形创建显示的事件区域
		void BuildEventAreaRect();

	public:

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// 逻辑数据操作
		//


		// 当前正在编辑的事件区域id
		int m_iCurEditEventAreaId;

		// 当前正在编辑的脚本id
		int m_iCurEditScriptid;

		// 当前编辑的最大id
		int m_iMaxEventAreaId;

		// 当前操作的区域矩形.
		CEventAreaInfo*  m_pCurOperatorRect;

		// 当前正在编辑的矩形点的id
		int  m_iEditRectId;

		// 当前编辑的id
		int m_iCurEditInstanceId;

		// 当前的操作状态.
		AREA_EDIT_OPERATRO_TYPE m_iOperator;

		// 两次鼠标按下的点
		Ogre::Vector3	m_PrePoint;
		Ogre::Vector3   m_CurPoint;

		// 当前生成的矩形.
		AREARECT m_AreaRect;

		// 把当前的点转换成规格矩形.
		void NormalRect();

		// 事件区域列表
		EVENT_AREA_ID_RECT_ID_MAP m_EventIdMap;

		// 矩形区域列表.
		ID_RECT_MAP				  m_RectMap;

		// 显示矩形列表.
		SHOW_RECT_MAP			  m_RectShowMap;

		// 清空信息列表
		void ReleaseRectInfoMap();

		// 初始化事件区域编辑
		void InitEventArea();

		// 得到地形上的坐标点
		bool  GetTerrainPos(const Point& pt, Ogre::Vector3& position);

		// 做画矩形的鼠标按下操作
		void DoDrawLBnDown(const Point& pt);

		// 做画矩形的鼠标托拽操作
		void DoDrawLBnDrag(const Point& pt);

		// 做画矩形的鼠标抬起操作
		void DoDrawLBnUp(const Point& pt);

		// 做选择矩形的鼠标抬起操作
		void DoSelectLBnUp(const Point& pt);

		// 初始化逻辑数据
		void InitLogicData();

		// 根据矩形id删除一个矩形
		void DeleteRect(int iId);

		// 不带redo undo 操作的删除
		void DeleteRectNoOperator(int iId);

		// 添加一个矩形.
		int AddRect(Ogre::Vector3 vectorUnDoPos, Ogre::Vector3 vectorReDoPos, int iId = -1);

		// 选中一个矩形
		void SelectEventAreaRect(int iId);

		// 更改区域
		void ModifyEventArea(int iInstanceId, int iEventId, int iScripId);
	

		

    protected:
        
		// 鼠标操作的虚函数.
        void _onMove(const Point& pt);
        void _onBegin(const Point& pt);
        void _onDrag(const Point& pt);
        void _onEnd(const Point& pt, bool canceled);

		// 当前激活active操作.
        void _onActive(bool active);


	};


}
#endif //__EVENTAREAEDITACTION_H__