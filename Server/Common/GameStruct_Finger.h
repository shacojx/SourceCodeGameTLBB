#ifndef __GAMESTRUCT_FINGER_H__
#define __GAMESTRUCT_FINGER_H__

#include "Type.h"

class SocketInputStream;
class SocketOutputStream;

/////////////////////////////////////////////////////////////////////////////////

enum FINGER_REQUEST_TYPE
{
	FREQ_NONE							= 0,
	FREQ_GUID,												// GUID 查找玩家
	FREQ_NAME,												// 昵称查找
	FREQ_ADVANCED,											// 高级查找
};

enum FINGER_RETURN_TYPE
{
	FRET_NONE							= 0,
	FRET_PLAYERLIST,										// 二级密码强制解除成功

	FRET_ERR_START,
	FRET_ERR_NORESULT,										// 没有任何匹配结果
};

enum ADVANCED_FINGER_TYPE
{
	AFT_INVALID							= -1,
	AFT_MENPAI,												// 通过门派查询
	AFT_GUILD,												// 通过帮会查询
	AFT_SEX,												// 通过性别查询
	AFT_LEVEL,												// 通过级别查询
	AFT_NUMBER,
};

// FREQ_GUID
struct FINGER_BY_GUID
{
	//数据
	GUID_t								m_TargetGUID;		// 某玩家的 GUID
	UCHAR								m_uOnlineFlag;		// 是否在线

	//基本接口
	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//数据应用接口
	GUID_t								GetTargetGUID( ) { return m_TargetGUID; }
	VOID								SetTargetGUID( GUID_t guid ) { m_TargetGUID = guid; }

	UCHAR								GetOnlineFlag( ) { return m_uOnlineFlag; }
	VOID								SetOnlineFlag( UCHAR flag ) { m_uOnlineFlag = flag; }
};

// FREQ_NAME
struct FINGER_BY_NAME
{
	//数据
	UCHAR								m_uNameSize;
	CHAR								m_szTargetName[MAX_CHARACTER_NAME];
	UCHAR								m_uOnlineFlag;		// 是否在线
	UCHAR								m_uPreciseFlag;		// 是否精确查找
	UINT								m_uPosition;		// 当前搜索位置

	//基本接口
	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//数据应用接口
	VOID								SetTargetName( const CHAR* pName )
	{
		strncpy( m_szTargetName, pName, sizeof(m_szTargetName)-1 );
		m_uNameSize = (UCHAR)strlen(m_szTargetName);
	}
	const CHAR*							GetTargetName( ) { return m_szTargetName; }

	UCHAR								GetOnlineFlag( ) { return m_uOnlineFlag; }
	VOID								SetOnlineFlag( UCHAR flag ) { m_uOnlineFlag = flag; }

	UCHAR								GetPreciseFlag( ) { return m_uPreciseFlag; }
	VOID								SetPreciseFlag( UCHAR flag ) { m_uPreciseFlag = flag; }

	UINT								GetPosition( ) { return m_uPosition; }
	VOID								SetPosition( UINT uPosition ) { m_uPosition = uPosition; }
};

// FREQ_ADVANCED
struct ADVANCED_FINGER
{
	//数据
	UCHAR								m_Flags;			// 每个位表示以下条件是否判断 ADVANCED_FINGER_TYPE
	UINT								m_uPosition;		// 当前搜索位置
	INT									m_nMenPai;			// 门派 MENPAI_ATTRIBUTE
	GuildID_t							m_GuildID;			// 帮会ID
	CHAR								m_Sex;				// 性别
	INT									m_nBottomLevel;		// 等级范围
	INT									m_nTopLevel;		// 等级范围

	//基本接口
	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//数据应用接口
	VOID								SetFingerCondition( ADVANCED_FINGER_TYPE eType, BOOL bFlag )
	{
		if ( bFlag )					{ m_Flags |= (1<<(INT)eType); }
		else							{ m_Flags &= (~(1<<(INT)eType)); }
	}
	BOOL								IsFingerCondition( ADVANCED_FINGER_TYPE eType ) const
	{
		if( m_Flags & (1<<(INT)eType) )	{ return TRUE; }

		return FALSE;
	}

	UINT								GetPosition( ) { return m_uPosition; }
	VOID								SetPosition( UINT uPosition ) { m_uPosition = uPosition; }

	INT									GetMenPai( ) const { return m_nMenPai; }
	VOID								SetMenPai( INT menpai )
	{
		m_nMenPai = menpai;
		SetFingerCondition( AFT_MENPAI, TRUE );
	}

	GuildID_t							GetGuildID( ) const { return m_GuildID; }
	VOID								SetGuildID( GuildID_t gid )
	{
		m_GuildID = gid;
		SetFingerCondition( AFT_GUILD, TRUE );
	}

	CHAR								GetSex( ) const { return m_Sex; }
	VOID								SetSex( CHAR sex )
	{
		m_Sex = sex;
		SetFingerCondition( AFT_SEX, TRUE );
	}

	INT									GetBottomLevel( VOID ) const { return m_nBottomLevel; }
	VOID								SetBottomLevel( INT nLevel )
	{
		m_nBottomLevel = nLevel;
		SetFingerCondition( AFT_LEVEL, TRUE );
	}

	INT									GetTopLevel( VOID ) const { return m_nTopLevel; }
	VOID								SetTopLevel( INT nLevel )
	{
		m_nTopLevel = nLevel;
		SetFingerCondition( AFT_LEVEL, TRUE );
	}
};

// e.g.
// setup a finger request
// CGFinger Msg;
// CG_FINGER* pFinger = Msg.GetFinger();
// pFinger->CleanUp();
// pFinger->m_Type = FREQ_GUID;
// pFinger->m_FingerByGUID.CleanUp();
// pFinger->m_FingerByGUID.SetTargetGUID( guid );

struct CG_FINGER
{
	UCHAR								m_Type;				// enum FINGER_REQUEST_TYPE

	union
	{
		FINGER_BY_GUID					m_FingerByGUID;
		FINGER_BY_NAME					m_FingerByName;
		ADVANCED_FINGER					m_AdvancedFinger;
	};

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

};

struct GW_FINGER : public CG_FINGER
{
	//数据
	GUID_t								m_GUID;				// 自己的 GUID

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//数据应用接口
	GUID_t								GetGUID( ) { return m_GUID; }
	VOID								SetGUID( GUID_t guid ) { m_GUID = guid; }
};

/////////////////////////////////////////////////////////////////////////////////

// 昵称 + GUID + OnlineFlag + Level + Sex + MenpaiID + GuildID + 头像
struct FINGER_PLAYER_INFO
{
	//数据
	GUID_t								m_GUID;								// GUID
	UCHAR								m_uNameSize;
	CHAR								m_szName[MAX_CHARACTER_NAME];		// 名字
	UCHAR								m_uOnlineFlag;						// 是否在线
	INT									m_nLevel;							// 角色等级
	CHAR								m_Sex;								// 性别
	INT									m_nMenPai;							// 门派 MENPAI_ATTRIBUTE
	GuildID_t							m_GuildID;							// 帮会ID（用于发给服务端）
	UCHAR								m_uGuildNameSize;					// 
	CHAR								m_szGuildName[MAX_GUILD_NAME_SIZE];	// 帮会名称（用于 Server 发给客户端）
	INT									m_nPortrait;						// 头像

	VOID								CleanUp();
	UINT								GetSize() const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//数据应用接口
	GUID_t								GetGUID( ) { return m_GUID; }
	VOID								SetGUID( GUID_t guid ) { m_GUID = guid; }

	const CHAR*							GetName( ) { return m_szName; }
	VOID								SetName( const CHAR* pName )
	{
		strncpy( m_szName, pName, MAX_CHARACTER_NAME-1 );
		m_uNameSize = (UCHAR)strlen(m_szName);
	}

	UCHAR								GetOnlineFlag( ) { return m_uOnlineFlag; }
	VOID								SetOnlineFlag( UCHAR flag ) { m_uOnlineFlag = flag; }

	INT									GetLevel( ) { return m_nLevel; }
	VOID								SetLevel( INT level ) { m_nLevel = level; }

	CHAR								GetSex( ) const { return m_Sex; }
	VOID								SetSex( CHAR sex ) { m_Sex = sex; }

	INT									GetMenPai( ) { return m_nMenPai; }
	VOID								SetMenPai( INT menpai ) { m_nMenPai = menpai; }

	GuildID_t							GetGuildID( ) { return m_GuildID; }
	VOID								SetGuildID( GuildID_t gid ) { m_GuildID = gid; }

	const CHAR*							GetGuildName( ) { return m_szGuildName; }
	VOID								SetGuildName( const CHAR* pName )
	{
		strncpy( m_szGuildName, pName, MAX_GUILD_NAME_SIZE-1 );
		m_uGuildNameSize = (UCHAR)strlen(m_szGuildName);
	}

	INT									GetPortrait( ) { return m_nPortrait; }
	VOID								SetPortrait( INT nPortrait ) { m_nPortrait = nPortrait; }
};

#define FINGER_PLAYER_COUNT	10

// FRET_PLAYERLIST
struct RETURN_FINGER_PLAYERLIST
{
	//数据
	UCHAR								m_uPlayerCount;
	FINGER_PLAYER_INFO					m_FingerPlayerInfo[FINGER_PLAYER_COUNT];
	// 以下数据为了记录玩家的搜索状态，以实现 <下一页> 功能
	UCHAR								m_FingerFlag;					// 是否查找过全部范围(没有下一页)
	UINT								m_uPosition;					// 当前搜索位置

	VOID								CleanUp();
	UINT								GetSize() const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//数据应用接口
	UCHAR								GetPlayerCount( ) { return m_uPlayerCount; }

	FINGER_PLAYER_INFO*					GetPlayerInfo( INT idx )
	{
		Assert( idx < m_uPlayerCount );
		return &(m_FingerPlayerInfo[idx]);
	}

	FINGER_PLAYER_INFO*					GetLastPlayerInfo( )
	{
		return &(m_FingerPlayerInfo[m_uPlayerCount-1]);
	}

	// 设置方法
	// 1. World 设置数据
	VOID								AddPlayerInfo( const USER_SIMPLE_DATA* pPlayerInfo );

	// 2. World 发到 Server 后，Server 可以直接调用这个函数循环赋值
	VOID								AddPlayerInfo( const FINGER_PLAYER_INFO* pPlayerInfo )
	{
		Assert( pPlayerInfo && m_uPlayerCount < FINGER_PLAYER_COUNT );
		memcpy( (void*)&m_FingerPlayerInfo[m_uPlayerCount++], pPlayerInfo, sizeof(pPlayerInfo) );
	}

	UCHAR								GetFingerFlag( ) { return m_FingerFlag; }
	VOID								SetFingerFlag( UCHAR uFlag ) { m_FingerFlag = uFlag; }

	UINT								GetPosition( ) { return m_uPosition; }
	VOID								SetPosition( UINT uPosition ) { m_uPosition = uPosition; }
};

struct GC_FINGER
{
	UCHAR								m_Type;				// enum FINGER_RETURN_TYPE

	union
	{
		RETURN_FINGER_PLAYERLIST		m_FingerPlayerList;
	};

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

};

struct WG_FINGER : public GC_FINGER
{
	//数据
	PlayerID_t							m_PlayerID;			// 本人的 PlayerID

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//数据应用接口
	PlayerID_t							GetPlayerID( ) { return m_PlayerID; }
	VOID								SetPlayerID( PlayerID_t pid ) { m_PlayerID = pid; }
};

//////////////////////////////////////////////////////////////////////////

#endif // __GAMESTRUCT_FINGER_H__
