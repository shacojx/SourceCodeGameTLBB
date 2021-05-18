

#ifndef __GAMESTRUCT_CAMP_H__
#define __GAMESTRUCT_CAMP_H__
#include "Type.h"
/////////////////////////////////////////////////////////////////////////////////
// 客户端用的DBC结构

// 阵营结构
struct _DBC_CAMP_DATA
{
	enum
	{
		MAX_CAMP_ID = 15, //最大阵营ID
		CAMP_LIST_SIZE = MAX_CAMP_ID +1,
	};
	VOID	ClearUp(VOID)
	{
		m_uCampID=-1;
		memset((VOID*)&m_nIsEnemyFlag, 0, sizeof(m_nIsEnemyFlag));
	}
	UINT	m_uCampID;					// ID
	BOOL	m_nIsEnemyFlag[CAMP_LIST_SIZE];
};
typedef _DBC_CAMP_DATA SCampData;

#endif //__GAMESTRUCT_CAMP_H__