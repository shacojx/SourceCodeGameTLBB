/*************************************************************************
文件名	: 	AIScript.h
版本号 :	0.0.1
功  能	:	怪物巡逻的相关处理
*************************************************************************/

#ifndef _PATROL_H_
#define _PATROL_H_

#include "Type.h"
#include "GameStruct.h"

struct PatrolPath
{
	struct PatrolUnit
	{
		INT m_nSettleTime;				// 该点的停留时间
		ScriptID_t m_ScriptID;			// 该点的事件脚本
		WORLD_POS m_PatrolPoint;		// 该点的实际坐标
		PatrolUnit()
		{
			m_nSettleTime = 0;
			m_ScriptID = -1;
		}
		VOID Cleanup(VOID)
		{
			m_nSettleTime = 0;
			m_ScriptID = -1;
		}
	};
		
	INT m_nPatrolPointCount;
	PatrolUnit* m_PatrolUnitList;

	PatrolPath()
	{
		m_nPatrolPointCount = 0;
		m_PatrolUnitList = NULL;
	}
	~PatrolPath()
	{
		CleanUp();
	}
	VOID CleanUp( VOID )
	{
		m_nPatrolPointCount = 0;
		SAFE_DELETE(m_PatrolUnitList);
	}
};

class Scene;
class PatrolPathMgr
{
public:

	PatrolPathMgr()
	{
		m_nPatrolPathCount = 0;
		m_PatrolPathList = NULL;
	}

	~PatrolPathMgr()
	{
		CleanUp();
	}

	VOID		Init( Scene* pScene );
	VOID		CleanUp( VOID );
	VOID		LoadPatrolPoint(const CHAR* filename);
	// 根据传入的patrolPointIndex值得到下一个要到达的patrolPointIndex
	INT			GetPatrolPoint(INT patrolPathIndex, INT patrolPointIndex, 
		BOOL& baHead, WORLD_POS& rTar, INT& rSettleTime, ScriptID_t& rScriptID);
	BOOL		FindPatrolID(INT PatrolID) const;

private:

	INT			m_nPatrolPathCount;
	PatrolPath* m_PatrolPathList;
	Scene*		m_pScene;


};

#endif