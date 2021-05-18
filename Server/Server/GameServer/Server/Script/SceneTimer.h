//-----------------------------------------------------------------------------
// 文件名 : SceneTimer.h
// 模块	:	Script
// 功能	 :  隶属于场景的计时器,脚本系统专用
// 修改历史:
//-----------------------------------------------------------------------------
#ifndef __SCENETIMER__H_
#define __SCENETIMER__H_

#include "Type.h"

class LuaInterface;
class Scene;

class SceneTimer	
{
public:
	CMyTimer			mRefeshTimer;


#define FuncNameLen 32

	typedef struct  _MyTimers
	{
		CHAR scriptFuncName[FuncNameLen];

		INT selfId;
		INT scriptId;

		CMyTimer timer;
	}MyTimer;

	MyTimer* m_pTimers;
	MyTimer* head;
	INT index;
	INT maxCount;
	Scene* m_pScene;

	//仅测试
#if _DEBUG
	INT m_currentUse;
#endif
	

	SceneTimer();

	~SceneTimer();

	VOID CleanUp() ;

	BOOL IsInit()
	{ 
		if( m_pScene==NULL ) return FALSE ;
		else return TRUE ;
	};
	
	VOID CreateTimer(INT maxCount,Scene* pScene);

	VOID InitOne(INT i);

	INT GetTimer();

	VOID StartTheTimer(INT sceneTimerIndex,INT selfId,INT scriptId,const CHAR* funcName,INT tickTime);

	VOID FreeTimer(INT index);

	BOOL CheckTimer(INT index);

	VOID OnTimer(UINT uTime);

	Scene* GetScene()
	{
		return m_pScene;
	}
};

#endif