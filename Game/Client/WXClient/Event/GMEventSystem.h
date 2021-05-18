/****************************************\
*										*
* 			   游戏事件处理器			*
*										*
\****************************************/
#pragma once

#include <queue>
#include "TDEventSystem.h"
#include "TDEventDefine.h"

//游戏事件处理器
class CEventSystem : public tEventSystem
{
public:
	//产生事件
	VOID	PushEvent(GAME_EVENT_ID id, const std::vector< STRING >& vParam);

	virtual VOID	PushEvent(GAME_EVENT_ID id);
	virtual VOID	PushEvent(GAME_EVENT_ID id, INT iArg0);
	virtual VOID	PushEvent(GAME_EVENT_ID id, FLOAT fArg0);
	virtual VOID	PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0);
	virtual VOID	PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0, LPCTSTR szArg1);
	virtual VOID	PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0, LPCTSTR szArg1, INT nArg2 );
	virtual VOID	PushEvent(GAME_EVENT_ID id, INT iArg0, INT iArg1);
	virtual VOID	PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0, LPCTSTR szArg1, INT iArg2, INT iArg3);
	//注册事件处理函数
	virtual VOID	RegisterEventHandle(const STRING& nameEvent, FUNC_EVENT_HANDLE funHandle, UINT uOwnerData=NULL);
	//处理事件
	virtual VOID	ProcessAllEvent(VOID);

public:
	virtual VOID	Initial(VOID*);
	virtual VOID	Release(VOID);
	virtual VOID	Tick(VOID);

protected:
	void	_PushEvent(const EVENT& event);
	void	_ProcessEvent(const EVENT& event);

	//通过事件名称检索表
	std::map< STRING, EVENT_DEFINE* >					m_mapEventIndex_AsName;
	//通过事件ID检索表
	std::map< GAME_EVENT_ID, EVENT_DEFINE* >			m_mapEventIndex_AsID;
	
	//事件队列
	std::list< EVENT >		m_queueEvent;

	//慢速处理队列, 每桢一个，防止过多的消息同时涌现
	std::list< EVENT >		m_delayQueueEvent;
	UINT					m_dwLastTickCount;

public:
	CEventSystem();
	virtual ~CEventSystem();
	static CEventSystem*	GetMe(VOID) { return s_pMe; }

protected:
	static CEventSystem* s_pMe;

protected:
	WX_DECLARE_DYNAMIC(CEventSystem);
};