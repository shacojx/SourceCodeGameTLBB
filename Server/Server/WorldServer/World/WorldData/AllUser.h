#ifndef __ALLUSER_H__
#define __ALLUSER_H__

#include "Type.h"
#include "IDTable.h"

class AllUser
{
public :
	AllUser( );
	~AllUser( );

	BOOL						Init( );
	VOID						CleanUp( );

	//向数据库里增加一个上线玩家记录
	BOOL						AddUser( const USER_SIMPLE_DATA* pSimpleUser );

	//根据角色guid搜索玩家
	UINT						Select( GUID_t guid, RETURN_FINGER_PLAYERLIST* pPlayerList );

	//根据完整角色名字搜索玩家
	UINT						Select( const CHAR* szName, RETURN_FINGER_PLAYERLIST* pPlayerList, BOOL bPreciseFlag, INT nPosition = 0 );

protected:
	USER_SIMPLE_DATA*			m_pUsers;			//用户数据池
	UINT						m_uMaxUserCount;	//用户数据池大小
	UINT						m_uCurUserCount;	//当前用户数量
	UINT						m_uSelectOffset;	//当前搜索起始位置

	IDTable						m_IDTable;	//快速索引表

};


extern AllUser* g_pAllUser;


#endif
