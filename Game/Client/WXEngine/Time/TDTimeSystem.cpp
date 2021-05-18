#include "StdAfx.h"
#include "TDTimeSystem.h"
#include "TDException.h"
#include "TDProfile.h"

//保存所使用时间函数指针,防止用户使用简单的变速齿轮更改系统速度
typedef DWORD (__stdcall *PFNTIMEGETTIME)(VOID);
static PFNTIMEGETTIME	g_ptimeGetTime	= ::timeGetTime;
WX_IMPLEMENT_DYNAMIC(tTimeSystem, GETCLASS(tNode));
VOID tTimeSystem::Initial(VOID*)
{
	m_fRealTime = 0.0f;
	m_fLoopTime = 0.0f;
	m_fDeltaTime= 0.0f;
	m_fFPS		= 0.0f;
	m_dwTickCount = 0;

	m_dwTimeNow = (*g_ptimeGetTime)();
}

UINT tTimeSystem::CalSubTime(UINT timeStart, UINT timeEnd)
{
	if(timeEnd < timeStart) 
		return ((UINT)0XFFFFFFFF-timeStart) + timeEnd;
	else 
		return timeEnd-timeStart;
}

VOID tTimeSystem::Tick(VOID)
{
	//-------------------------------------------------------------
	static UINT	s_dwOldTime = 0;		//上次执行Tick的时间

	//-------------------------------------------------------------
	//桢数加一
	m_dwTickCount++;

	//-------------------------------------------------------------
	//记录当前时间
	m_dwTimeNow = (*g_ptimeGetTime)();
	//记录距上一桢所花费的时间， 考虑跨时间阙的问题(49.71天)
	UINT subTime = CalSubTime(s_dwOldTime, m_dwTimeNow);

	m_fDeltaTime = (FLOAT)subTime;

	//-------------------------------------------------------------
	//计算每桢所花费的平均时间
	const UINT	MAXTIME_CAL_LOOPTIME	= 20;	//每20ms计算一次每桢花费的时间
	{
		static UINT	s_dwLoopCnt = 0;		//桢累计计数
		static UINT	s_dwLoopHop = 0;		//时间累计计数
		s_dwLoopCnt++;
		s_dwLoopHop += subTime;
		if( s_dwLoopHop > MAXTIME_CAL_LOOPTIME )		
		{
			m_fLoopTime = (s_dwLoopHop/(FLOAT)s_dwLoopCnt)/1000.0f;

			s_dwLoopCnt=0;
			s_dwLoopHop=0;
		}
	}

	//-------------------------------------------------------------
	//计算桢率
	const UINT	MAXTIME_CAL_FPS			= 1000;	//每1秒计算一次fps
	{
		static UINT	s_dwLoopCnt = 0;		//桢累计计数
		static UINT	s_dwLoopHop = 0;		//时间累计计数
		s_dwLoopCnt++;
		s_dwLoopHop += subTime;
		if( s_dwLoopHop > MAXTIME_CAL_FPS )		
		{
			m_fFPS		= (FLOAT)(s_dwLoopCnt*1000)/s_dwLoopHop;

			s_dwLoopCnt=0;
			s_dwLoopHop=0;
		}
	}

	s_dwOldTime = m_dwTimeNow;
	m_fRealTime += m_fLoopTime;

	//--------------------------------------------------------
	//Profile: 如果有查询请求并且堆栈空
	TDProfile_Tick();
}
