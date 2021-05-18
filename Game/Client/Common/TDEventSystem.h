/****************************************\
*										*
* 		 游戏逻辑事件管理器				*
*										*
\****************************************/

#pragma once
#include "TDNode.h"

//事件处理回调函数
enum GAME_EVENT_ID;
struct EVENT;
typedef VOID (__stdcall* FUNC_EVENT_HANDLE)(const EVENT* pEvent, UINT dwOwnerData);

struct EVENT_DEFINE
{
	typedef std::list< std::pair< FUNC_EVENT_HANDLE, UINT > > REGISTER_STRUCT;
	GAME_EVENT_ID		idEvent;
	LPCTSTR				szEvent;
	BOOL				delayProcess;
	REGISTER_STRUCT		listFuncNotify;
};

struct EVENT
{
	EVENT_DEFINE*			pEventDef;
	std::vector< STRING >	vArg;
	bool operator == (const EVENT& other)
	{
		if(other.pEventDef != pEventDef) return false;
		if(other.vArg.size() != vArg.size()) return false;
		for(register size_t i=0; i<vArg.size(); i++)
		{
			if(vArg[i] != other.vArg[i]) return false;
		}
		return true;
	}
};


//=====================================================
//游戏事件系统
class tEventSystem : public tNode
{
public:
	//产生事件
	/*
		!!!! 此函数不能作为虚接口  !!!
		可能会引起内存在不同模块之间分配释放
	
		virtual VOID	PushEvent(GAME_EVENT_ID id, const std::vector< STRING >& vParam) = 0;
	*/

	virtual VOID	PushEvent(GAME_EVENT_ID id)	= 0;
	virtual VOID	PushEvent(GAME_EVENT_ID id, INT iArg0)	= 0;
	virtual VOID	PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0)	= 0;
	virtual VOID	PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0, LPCTSTR szArg1)	= 0;
	virtual VOID	PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0, LPCTSTR szArg1, INT nArg2 )	= 0;
	virtual VOID	PushEvent(GAME_EVENT_ID id, INT iArg0, INT iArg1)	= 0;
	virtual VOID	PushEvent(GAME_EVENT_ID id, LPCTSTR szArg0, LPCTSTR szArg1, INT iArg2, INT iArg3)	= 0;

	//注册事件处理函数
	virtual VOID	RegisterEventHandle(const STRING& nameEvent, FUNC_EVENT_HANDLE funHandle, UINT dwOwnerData=NULL) = 0;
	//处理事件
	virtual VOID	ProcessAllEvent(VOID) = 0;
protected:
	WX_DECLARE_DYNAMIC(tEventSystem);
};