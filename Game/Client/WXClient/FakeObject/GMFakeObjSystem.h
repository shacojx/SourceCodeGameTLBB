/****************************************\
*										*
* 	  用于在ui上显示的物体管理器接口	*
*										*
\****************************************/

#pragma once
#include "TDFakeObjSystem.h"

class CFakeObjSystem : public tFakeObjSystem
{
	//-----------------------------------------------------------
	//功能接口
	//-----------------------------------------------------------
protected:
	struct FakeObject
	{
		STRING			strName;		//名字，UI控件根据该名字控制
		INT				nObjectID;		//逻辑Obj对象ID
		BOOL			bVisible;		//当前该对象是否需要显示
		STRING			strWindowName;	//显示该模型的窗口名
		STRING			strCameraName;	//该窗口的摄象机名字
	};

public:
	//创建/销毁在ui上显示的obj对象
	virtual tObject*		NewFakeObject(LPCTSTR szClassName, LPCTSTR szObjName, LPCTSTR szCameraName);
	virtual VOID			DestroyObject(LPCTSTR szObjName);

	//UI需要显示某物体
	virtual VOID			OnUIHook(LPCTSTR szWindowName, LPCTSTR szObjName);
	//UI不再需要显示某物体
	virtual VOID			OnUIUnHook(LPCTSTR szWindowName);

	//UI显示/隐藏
	virtual	VOID			OnUIShown(LPCTSTR szWindowName, float fAspectRatio);
	virtual VOID			OnUIHiden(LPCTSTR szWindowName);

	//旋转模型
	virtual VOID			RotateObj_Begin(LPCTSTR szObjName, FLOAT fAngle);
	virtual VOID			RotateObj_End(LPCTSTR szObjName);
	virtual VOID			Obj_ChangeAction(LPCTSTR szObjName, INT SkillID,INT SkillLoop);

	//-----------------------------------------------------------
	//受管理的obj对象，最多32个
	//-----------------------------------------------------------
protected:

	//FakeObj链表，最多32个
	std::map< STRING, FakeObject > m_listFakeMap;
	
	//以窗口名为索引
	std::map< STRING, FakeObject* > m_listFakeUI;

	//正在旋转中的模型
	FakeObject*						m_pRotateObj;
	FakeObject*						m_pActionObj;
	FLOAT							m_fRotateSpeed;

	//-----------------------------------------------------------
	//核心继承
	//-----------------------------------------------------------
public:
	virtual VOID	Initial(VOID*);
	virtual VOID	Release(VOID);
	virtual VOID	Tick(VOID);

	
public:
	CFakeObjSystem();
	virtual ~CFakeObjSystem();

	static CFakeObjSystem*	GetMe(void) { return s_pMe; }

protected:
	static CFakeObjSystem* s_pMe;
protected:
	WX_DECLARE_DYNAMIC(CFakeObjSystem);
};