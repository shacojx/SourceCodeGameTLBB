#ifndef _WORLDTIME_INFO_H_
#define _WORLDTIME_INFO_H_

#include "Type.h"


class WorldTimeInfo
{
public:
	WorldTimeInfo();
	~WorldTimeInfo();

	BOOL			Init();
	VOID			CleanUp();

	BOOL			HeartBeat(UINT uTime);
	
	WORLD_TIME		GetWorldTime()
	{
		return m_WorldTime;
	}
protected:
	WORLD_TIME		m_WorldTime;
	CMyTimer		m_LoginTimer ;
};


extern		WorldTimeInfo*	g_pWorldTimeInfo;

#endif