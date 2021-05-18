#ifndef __ONLINEUSER_H__
#define __ONLINEUSER_H__

#include "Type.h"

#include "Guild.h"
#include "IDTable.h"
#include "StrTable.h"

#include "User.h"

class OnlineUser
{
public :
	OnlineUser( ) ;
	~OnlineUser( ) ;

	BOOL			Init( UINT MaxWorldUsers = MAX_WORLD_USER ) ;
	BOOL			HeartBeat( DWORD dwTime=0 ) ;


public :
	USER*			FindUser( GUID_t guid ) ;
	USER*			FindUser( const CHAR* szName ) ;
	USER*			FindUserByAccountName( const CHAR* szAccountName ) ;

	//测试
	USER*			LoadUser( GUID_t guid ) ;
	void			InitUserData( FULLUSERDATA* pData, GUID_t guid ) ;
	//测试

	void			RemoveUser( ID_t ServerID );
	void			RemoveUser(ID_t ServerID,USER_STATUS UserUS );

	void			ChangeUserStatus(ID_t ServerID,USER_STATUS UserUS );


	USER*			NewUser( ) ;
	void			DelUser( USER* pUser ) ;


	FULLUSERDATA*	NewData( ) ;
	void			DelData( FULLUSERDATA* pData ) ;

	BOOL			Register( USER* pUser ) ;
	BOOL			UnRegister( USER* pUser ) ;

	UINT			Select( const CHAR* szName, RETURN_FINGER_PLAYERLIST* pPlayerList, BOOL bPreciseFlag, INT nPosition = 0 );

	UINT			Select( INT nMenPai, GuildID_t GuildID, CHAR Sex, INT m_nBottomLevel, INT m_nTopLevel,
							RETURN_FINGER_PLAYERLIST* pPlayerList, INT nPosition = 0 );
public :
	//玩家登陆游戏
	BOOL			OnUserLogin( USER* pUser ) ;

	//玩家切换场景
	BOOL			OnUserChangeScene( USER* pUser, SceneID_t OldSceneID, SceneID_t NewSceneID ) ;

	//玩家断线处理接口，
	VOID			OnUserOffLine( USER* pUser );

	//玩家退出处理接口，
	VOID			OnUserRemove( USER* pUser );

	// 玩家离开队伍接口，
	BOOL			UserLeaveTeam( USER* pUser );

	// 提升新队长，
	BOOL			AppointTeamLeader( USER* pOldLeader, USER* pNewLeader );

	INT				GetTotalCount(){ return m_UserCount ; } ;
	USER*			GetUserByIndex(INT nIndex){ 
		if( nIndex<0 || nIndex>=MAX_WORLD_USER )
			return NULL ;
		return &m_aUser[nIndex] ;
	};

protected :
	USER				m_aUser[MAX_WORLD_USER] ;
	INT					m_UserCount ;

	FULLUSERDATA*		m_pUserData[MAX_WORLD_USER] ;
	INT					m_iLeftUserDataCount ;
	INT					m_iUserDataOffset;

	IDTable				m_GUIDTable ;
	StrTable			m_NameTable ;
	StrTable			m_AccountNameTable ;

	CMyTimer			m_HeartBeatTimer ;

	//____________________________________________
	INT				m_MaxWorldUsers;

	struct START_SCENE //出生地点
	{
		INT		nSceneNum ; //场景号码
		FLOAT	fStartX ; //出生地点
		FLOAT	fStartZ ;
	};
	START_SCENE		m_StartScene[MAX_CAMP_NUM];
};

extern OnlineUser*	g_pOnlineUser ;

#endif


