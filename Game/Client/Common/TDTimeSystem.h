#pragma once

#include "TDNode.h"

//时间系统,用于系统时间上的计算和祯速控制
class tTimeSystem : public tNode
{
public:
	//取得最近20ms以来每祯平均花费的时间
	FLOAT			GetLoopTime(VOID) const { return m_fLoopTime; }
	//取得上一祯花费的时间
	FLOAT			GetDeltaTime(VOID) const { return m_fDeltaTime; }
	//取得根据m_fLoopTime累积的系统运行时间
	FLOAT			GetRealTime(VOID) const { return m_fRealTime; }
	//取得当前时间
	UINT			GetTimeNow(VOID) const { return m_dwTimeNow; }
	//取得桢率
	FLOAT			GetFPS(VOID) const { return m_fFPS; }
	//计算时间差,考虑时间阙(49.71天)
	UINT			CalSubTime(UINT timeStart, UINT timeEnd);
	//得到桢数
	UINT			GetTickCount(VOID) { return m_dwTickCount; }

public:
	//节点初始化
	virtual VOID						Initial(VOID*);
	//逻辑轮循函数
	virtual VOID						Tick(VOID);

private:
	UINT		m_dwTimeNow;		//当前时间
	FLOAT		m_fLoopTime;		//最近20ms以来每祯平均花费的时间
	FLOAT		m_fDeltaTime;		//上一桢所花费的时间
	FLOAT		m_fRealTime;		//根据m_fLoopTime累积的系统运行时间
	FLOAT		m_fFPS;				//当前的桢率
	UINT		m_dwTickCount;		//桢总数

	WX_DECLARE_DYNAMIC(tTimeSystem);
};