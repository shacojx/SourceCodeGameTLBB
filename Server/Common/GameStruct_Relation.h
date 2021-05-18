// 好友消息结构

#ifndef __GAMESTRUCT_RELATION_H__
#define __GAMESTRUCT_RELATION_H__

#include "Type.h"

class SocketInputStream;
class SocketOutputStream;

/////////////////////////////////////////////////////////////////////////////////

// 关系人数据
struct _RELATION
{
	GUID_t								m_GUID;								// GUID
	UCHAR								m_uNameSize;
	CHAR								m_szName[MAX_CHARACTER_NAME];		// 名字
	INT									m_nLevel;							// 角色等级
	INT									m_nMenPai;							// 门派 MENPAI_ATTRIBUTE
	INT									m_nPortrait;						// 头像
	GuildID_t							m_GuildID;							// 帮会ID（用于发给服务端）
	UCHAR								m_uGuildNameSize;					// 
	CHAR								m_szGuildName[MAX_GUILD_NAME_SIZE];	// 帮会名称（用于 Server 发给客户端）
	UCHAR								m_uOnlineFlag;						// 是否在线（以下部分只有在线才发给客户端）
	UCHAR								m_uMoodSize;						// 心情长度
	CHAR								m_szMood[MOOD_DATA_SIZE];			// 心情
	UCHAR								m_uTitleSize;						// 称号名称
	CHAR								m_szTitle[MAX_CHARACTER_TITLE];		// 称号
	SceneID_t							m_SceneID;							// 所在场景（以下部分由 Server 设置）
	UCHAR								m_uTeamSize;						// 队伍人数（0 表示未组队）

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

	INT									GetLevel( ) { return m_nLevel; }
	VOID								SetLevel( INT level ) { m_nLevel = level; }

	INT									GetMenPai( ) { return m_nMenPai; }
	VOID								SetMenPai( INT menpai ) { m_nMenPai = menpai; }

	INT									GetPortrait( ) { return m_nPortrait; }
	VOID								SetPortrait( INT nPortrait ) { m_nPortrait = nPortrait; }

	GuildID_t							GetGuildID( ) { return m_GuildID; }
	VOID								SetGuildID( GuildID_t gid ) { m_GuildID = gid; }

	const CHAR*							GetGuildName( ) { return m_szGuildName; }
	VOID								SetGuildName( const CHAR* pName )
	{
		strncpy( m_szGuildName, pName, MAX_GUILD_NAME_SIZE-1 );
		m_uGuildNameSize = (UCHAR)strlen(m_szGuildName);
	}

	UCHAR								GetOnlineFlag( ) { return m_uOnlineFlag; }
	VOID								SetOnlineFlag( UCHAR flag ) { m_uOnlineFlag = flag; }

	const CHAR*							GetMood( ) { return m_szMood; }
	VOID								SetMood( const CHAR* pMood )
	{
		strncpy( m_szMood, pMood, MOOD_DATA_SIZE-1 );
		m_uMoodSize = (UCHAR)strlen(m_szMood);
	}

	const CHAR*							GetTitle( ) { return m_szTitle; }
	VOID								SetTitle( const CHAR* pTitle )
	{
		strncpy( m_szTitle, pTitle, MAX_CHARACTER_TITLE-1 );
		m_uTitleSize = (UCHAR)strlen(m_szTitle);
	}

	SceneID_t							GetSceneID( ) { return m_SceneID; }
	VOID								SetSceneID( SceneID_t sid ) { m_SceneID = sid; }

	UCHAR								GetTeamSize( ) { return m_uTeamSize; }
	VOID								SetTeamSize( UCHAR TeamSize ) { m_uTeamSize = TeamSize; }
};

enum RELATION_REQUEST_TYPE
{
	REQ_NONE							= 0,
	REQ_RELATIONLIST,
	REQ_RELATIONINFO,
	REQ_VIEWPLAYER,										// 查看玩家
	REQ_ADDFRIEND,
	REQ_ADDTOBLACKLIST,
	REQ_TEMPFRIEND_TO_FRIEND,
	REQ_TEMPFRIEND_ADDTO_BLACKLIST,
	REQ_TRANSITION,
	REQ_DELFRIEND,
	REQ_DELFROMBLACKLIST,
	REQ_NEWGOODFRIEND,									// 增加一个亲密好友
	REQ_RELATIONONLINE,									// 请求在线玩家列表
	REQ_MODIFYMOOD,										// 修改自己的心情
	REQ_MODIFYSETTINGS,									// 修改联系人设置
	REQ_NOTIFY_ADDTEMPFRIEND,							// 通知对方被加为临时好友
};

enum RELATION_RETURN_TYPE
{
	RET_NONE							= 0,
	RET_RELATIONLIST,
	RET_RELATIONINFO,
	RET_VIEWPLAYER,										// 查看玩家
	RET_TARGETNOTONLINE,								// 目标不在线（用于向 World 询问某关系人的信息没有找到时的反馈）
	RET_ADDFRIEND,
	RET_ADDTOBLACKLIST,
	RET_TEMPFRIEND_TO_FRIEND,
	RET_TEMPFRIEND_ADDTO_BLACKLIST,
	RET_TRANSITION,
	RET_DELFRIEND,
	RET_DELFROMBLACKLIST,
	RET_ADDFRIENDNOTIFY,								// 通知好友已经被加了
	RET_ONLINELIST,										// 在线好友列表
	RET_RELATIONONLINE,									// 好友上线
	RET_RELATIONOFFLINE,								// 好友下线
	RET_NEWMOOD,										// 新的心情
	RET_NOTIFY_ADDTEMPFRIEND,							// 通知对方被加为临时好友

	RET_ERR_START,
	RET_ERR_TARGETNOTEXIST,								// 目标不存在或不在线
	RET_ERR_GROUPISFULL,								// 好友组已满
	RET_ERR_ISFRIEND,									// 已经是好友
	RET_ERR_ISBLACKNAME,								// 已经被加入黑名单
	RET_ERR_CANNOTTRANSITION,							// 不能转换
	RET_ERR_ISNOTFRIEND,								// 不是好友
	RET_ERR_ISNOTINBLACKLIST,							// 不在黑名单
	RET_ERR_SPOUSETOBLACKLIST,							// 将配偶加入黑名单
	RET_ERR_MASTERTOBLACKLIST,							// 将师傅加入黑名单
	RET_ERR_PRENTICETOBLACKLIST,						// 将徒弟加入黑名单
	RET_ERR_BROTHERTOBLACKLIST,							// 将结拜兄弟加入黑名单
	RET_ERR_DELSPOUSE,									// 删除配偶
	RET_ERR_DELMASTER,									// 删除师傅
	RET_ERR_DELPRENTICE,								// 删除徒弟
	RET_ERR_DELBROTHER,									// 删除结义兄弟
	RET_ERR_PASSWDMISMATCH,								// 密码不匹配
	RET_ERR_CANNOT_ADDFRIEND,							// 拒绝加为好友
	RET_ERR_CANNOTRECEIVEMAIL,							// 拒绝接收任何邮件
	RET_ERR_NOTRECVSTRANGEMAIL,							// 拒收陌生人邮件
	RET_ERR_ISENEMY,									// 非相同阵营

	RET_ERR_RELATIONUNKNOWN,							// 未知错误
};

// 参数为 GUID
struct RELATION_GUID
{
	//数据
	GUID_t								m_TargetGUID;		// 某玩家的 GUID

	//基本接口
	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//数据应用接口
	GUID_t								GetTargetGUID( ) { return m_TargetGUID; }
	VOID								SetTargetGUID( GUID_t guid ) { m_TargetGUID = guid; }
};

// 删除好友
struct REQUEST_DEL_FRIEND : public RELATION_GUID
{
	//数据

	//基本接口
	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//数据应用接口
};

// 修改心情
struct REQUEST_MODIFY_MOOD
{
	//数据
	UCHAR								m_uMoodSize;						// 心情长度
	CHAR								m_szMood[MOOD_DATA_SIZE];			// 心情

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//数据应用接口
	const CHAR*							GetMood( ) { return m_szMood; }
	VOID								SetMood( const CHAR* pMood )
	{
		if( pMood == NULL || strlen(pMood) > MAX_MOOD_SIZE )
		{
			Assert( pMood );
			Assert( strlen(pMood) <= MAX_MOOD_SIZE );
			return;
		}

		strncpy( m_szMood, pMood, MOOD_DATA_SIZE-1 );
		m_uMoodSize = (UCHAR)strlen(m_szMood);
	}
};

// 查询关系人详细信息
struct REQUEST_RELATION_INFO
{
	//数据
	GUID_t								m_TargetGUID;		// 某玩家的 GUID
	UCHAR								m_uNameSize;
	CHAR								m_szTargetName[MAX_CHARACTER_NAME];

	//基本接口
	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//数据应用接口
	GUID_t								GetTargetGUID( ) { return m_TargetGUID; }
	VOID								SetTargetGUID( GUID_t guid ) { m_TargetGUID = guid; }

	VOID								SetTargetName( const CHAR* pName )
	{
		strncpy( m_szTargetName, pName, MAX_CHARACTER_NAME-1 );
		m_uNameSize = (UCHAR)strlen(m_szTargetName);
	}
	const CHAR*							GetTargetName( ) { return m_szTargetName; }
};

// 右建查看玩家
struct REQUEST_VIEW_PLAYER
{
	//数据
	UCHAR								m_uNameSize;
	CHAR								m_szTargetName[MAX_CHARACTER_NAME];

	//基本接口
	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//数据应用接口
	VOID								SetTargetName( const CHAR* pName )
	{
		strncpy( m_szTargetName, pName, MAX_CHARACTER_NAME-1 );
		m_uNameSize = (UCHAR)strlen(m_szTargetName);
	}
	const CHAR*							GetTargetName( ) { return m_szTargetName; }
};

// 增加一个关系人
struct REQUEST_ADD_RELATION
{
	//数据
	GUID_t								m_TargetGUID;		// 某玩家的 GUID
	UCHAR								m_uNameSize;
	CHAR								m_szTargetName[MAX_CHARACTER_NAME];
	UCHAR								m_uRelationType;	// 关系类型

	//基本接口
	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//数据应用接口
	GUID_t								GetTargetGUID( ) { return m_TargetGUID; }
	VOID								SetTargetGUID( GUID_t guid ) { m_TargetGUID = guid; }

	VOID								SetTargetName( const CHAR* pName )
	{
		strncpy( m_szTargetName, pName, MAX_CHARACTER_NAME-1 );
		m_uNameSize = (UCHAR)strlen(m_szTargetName);
	}
	const CHAR*							GetTargetName( ) { return m_szTargetName; }

	UCHAR								GetRelationType( ) { return m_uRelationType; }
	VOID								SetRelationType( UCHAR uRelationType ) { m_uRelationType = uRelationType; }
};

// 增加一个关系人，并且带上组号
struct REQUEST_ADD_RELATION_WITH_GROUP : public REQUEST_ADD_RELATION
{
	//数据
	UCHAR								m_uGroup;			// 组

	//基本接口
	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//数据应用接口
	UCHAR								GetGroup( ) { return m_uGroup; }
	VOID								SetGroup( UCHAR uGroup ) { m_uGroup = uGroup; }
};

// 参数为 GUID 和 UCHAR、UCHAR
struct RELATION_GUID_UCHAR_UCHAR
{
	//数据
	GUID_t								m_TargetGUID;		// 某玩家的 GUID
	UCHAR								m_uRelationType;	// 关系类型
	UCHAR								m_uGroup;			// 组

	//基本接口
	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//数据应用接口
	GUID_t								GetTargetGUID( ) { return m_TargetGUID; }
	VOID								SetTargetGUID( GUID_t guid ) { m_TargetGUID = guid; }

	UCHAR								GetRelationType( ) { return m_uRelationType; }
	VOID								SetRelationType( UCHAR uRelationType ) { m_uRelationType = uRelationType; }

	UCHAR								GetGroup( ) { return m_uGroup; }
	VOID								SetGroup( UCHAR uGroup ) { m_uGroup = uGroup; }
};

struct CG_RELATION
{
	UCHAR								m_Type;			//enum CG_TYPE

	union
	{
		RELATION_GUID					m_RelationGUID;
		REQUEST_RELATION_INFO			m_RequestInfo;
		REQUEST_VIEW_PLAYER				m_ViewPlayer;
		REQUEST_ADD_RELATION			m_AddRelation;
		REQUEST_ADD_RELATION_WITH_GROUP	m_AddRelationWithGroup;
		RELATION_GUID_UCHAR_UCHAR		m_RelationGUIDUCHARUCHAR;
		REQUEST_DEL_FRIEND				m_DelFriend;
		REQUEST_MODIFY_MOOD				m_ModifyMood;
	};

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

};

/////////////////////////////////////////////////////////////////////////////////

// 参数为 GUID 和 UCHAR
struct RELATION_GUID_UCHAR
{
	//数据
	GUID_t								m_TargetGUID;		// 某玩家的 GUID
	UCHAR								m_uRelationType;	// 关系类型

	//基本接口
	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//数据应用接口
	GUID_t								GetTargetGUID( ) { return m_TargetGUID; }
	VOID								SetTargetGUID( GUID_t guid ) { m_TargetGUID = guid; }

	UCHAR								GetRelationType( ) { return m_uRelationType; }
	VOID								SetRelationType( UCHAR uRelationType ) { m_uRelationType = uRelationType; }
};

// 修改联系人设置
struct REQUEST_MODIFY_SETTINGS
{
	//数据
	USHORT								m_uSettings;	// 关系类型

	//基本接口
	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//数据应用接口
	USHORT								GetSettings( ) { return m_uSettings; }
	VOID								SetSettings( UCHAR uSettings ) { m_uSettings = uSettings; }
};

//////////////////////////////////////////////////////////////////////////

struct GW_RELATION
{
	UCHAR								m_Type;
	GUID_t								m_GUID;		// 自己的 GUID

	union
	{
		RELATION_GUID					m_RelationGUID;
		REQUEST_RELATION_INFO			m_RequestInfo;
		REQUEST_VIEW_PLAYER				m_ViewPlayer;
		REQUEST_ADD_RELATION_WITH_GROUP	m_AddRelationWithGroup;
		REQUEST_ADD_RELATION			m_AddRelation;
		RELATION_GUID_UCHAR				m_TransitionRelation;
		REQUEST_MODIFY_SETTINGS			m_Settings;
		REQUEST_MODIFY_MOOD				m_ModifyMood;
	};

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//数据应用接口
	GUID_t								GetGUID( ) { return m_GUID; }
	VOID								SetGUID( GUID_t guid ) { m_GUID = guid; }
};

/////////////////////////////////////////////////////////////////////////////////

struct RETURN_ADD_RELATION
{
	UCHAR								m_uRelationType;	// 关系类型
	UCHAR								m_uGroup;			// 组

	_RELATION							m_Relation;			// 关系人详细信息，如果加入成功则携带

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//数据应用接口
	UCHAR								GetRelationType( ) { return m_uRelationType; }
	VOID								SetRelationType( UCHAR uRelationType ) { m_uRelationType = uRelationType; }

	UCHAR								GetGroup( ) { return m_uGroup; }
	VOID								SetGroup( UCHAR uGroup ) { m_uGroup = uGroup; }

	_RELATION*							GetRelationData( ) { return &m_Relation; }
	VOID								SetRelationData( const _RELATION* pRelationData )
	{
		Assert( pRelationData );
		memcpy( (void*)&m_Relation, pRelationData, sizeof(m_Relation) );
	}
};

struct RETURN_RELATION_INFO
{
	UCHAR								m_uRelationType;	// 关系类型
	UCHAR								m_uGroup;			// 组
	INT									m_nFriendpoint;		// 关系值

	_RELATION							m_Relation;			// 关系人详细信息

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//数据应用接口
	UCHAR								GetRelationType( ) { return m_uRelationType; }
	VOID								SetRelationType( UCHAR uRelationType ) { m_uRelationType = uRelationType; }

	UCHAR								GetGroup( ) { return m_uGroup; }
	VOID								SetGroup( UCHAR uGroup ) { m_uGroup = uGroup; }

	INT									GetFriendPoint( ) { return m_nFriendpoint; }
	VOID								SetFriendPoint( INT nFp ) { m_nFriendpoint = nFp; }

	_RELATION*							GetRelationData( ) { return &m_Relation; }
	VOID								SetRelationData( const _RELATION* pRelationData )
	{
		Assert( pRelationData );
		memcpy( (void*)&m_Relation, pRelationData, sizeof(m_Relation) );
	}
};

// 右键查看玩家信息
struct RETURN_VIEW_PLAYER
{
	GUID_t								m_GUID;								// GUID
	UCHAR								m_uNameSize;
	CHAR								m_szName[MAX_CHARACTER_NAME];		// 1.名字
	//UCHAR								m_uTitleSize;
	//CHAR								m_szTitle[MAX_CHARACTER_TITLE];		// 2.称号
	//INT									m_nLevel;							// 3.角色等级
	//INT									m_nMenPai;							// 4.门派 MENPAI_ATTRIBUTE
	//UCHAR								m_uGuildNameSize;
	//CHAR								m_szGuildName[MAX_GUILD_NAME_SIZE];	// 5.帮会名称（用于 Server 发给客户端）
	//UCHAR								m_uSpouseNameSize;
	//CHAR								m_szSpouseName[MAX_CHARACTER_NAME];	// 6.配偶名字
	//INT									m_nModelID;							// 7.模型
	//UINT								m_uHairColor;						// 8.头发颜色

	//UINT								m_WeaponID;							// 9.武器
	//UINT								m_CapID;							// 10.帽子
	//UINT								m_ArmourID;							// 11.衣服
	//UINT								m_CuffID;							// 12.护腕
	//UINT								m_FootID;							// 13.靴子

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//数据应用接口
	VOID								FillData( const RETURN_VIEW_PLAYER* pViewPlayer )
	{
		Assert( pViewPlayer );
		memcpy( (void*)this, pViewPlayer, sizeof(*this) );
	}

	GUID_t								GetGUID( ) { return m_GUID; }
	VOID								SetGUID( GUID_t guid ) { m_GUID = guid; }

	const CHAR*							GetName( ) { return m_szName; }
	VOID								SetName( const CHAR* pName )
	{
		strncpy( m_szName, pName, MAX_CHARACTER_NAME-1 );
		m_uNameSize = (UCHAR)strlen(m_szName);
	}
};

struct RETURN_NOTIFY_FRIEND
{
	GUID_t								m_GUID;								// GUID
	UCHAR								m_uNameSize;
	CHAR								m_szName[MAX_CHARACTER_NAME];		// 名字

	VOID								CleanUp( );
	UINT								GetSize( ) const;
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
};

struct _RELATION_ONLINE
{
	GUID_t								m_GUID;								// 在线的亲密好友
	UCHAR								m_uMoodSize;						// 心情长度
	CHAR								m_szMood[MOOD_DATA_SIZE];			// 心情

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//数据应用接口
	GUID_t								GetGUID( ) const { return m_GUID; }
	VOID								SetGUID( GUID_t guid ) { m_GUID = guid; }

	const CHAR*							GetMood( ) const { return m_szMood; }
	VOID								SetMood( const CHAR* pMood )
	{
		strncpy( m_szMood, pMood, MOOD_DATA_SIZE-1 );
		m_uMoodSize = (UCHAR)strlen(m_szMood);
	}
};

// 用于给玩家在线密友列表
struct RETURN_ONLINE_LIST
{
	UCHAR								m_uOnlineCount;						// 在线人数
	_RELATION_ONLINE					m_OnlineInfo[RELATION_BLACKNAME_OFFSET-RELATION_FRIEND_OFFSET];

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//数据应用接口
	UCHAR								GetOnlineCount() { return m_uOnlineCount; }

	const _RELATION_ONLINE*				GetOnlineRelation( UCHAR uIndex )
	{
		if( uIndex >= m_uOnlineCount )
		{
			Assert(uIndex < m_uOnlineCount);
			return NULL;
		}

		return &(m_OnlineInfo[uIndex]);
	}
	VOID								AddOnlineRelation( const _RELATION_ONLINE* pOnlineInfo )
	{
		if( m_uOnlineCount >= RELATION_BLACKNAME_OFFSET-RELATION_FRIEND_OFFSET )
		{
			Assert( m_uOnlineCount < RELATION_BLACKNAME_OFFSET-RELATION_FRIEND_OFFSET );
			return;
		}

		m_OnlineInfo[m_uOnlineCount++] = *pOnlineInfo;
	}
};

// 用于通知有密友上线
struct RETURN_NOTIFY_ONLINE
{
	UCHAR								m_uNameSize;
	CHAR								m_szTargetName[MAX_CHARACTER_NAME];
	UCHAR								m_uMoodSize;						// 心情长度
	CHAR								m_szMood[MOOD_DATA_SIZE];			// 心情

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//数据应用接口
	const CHAR*							GetTargetName( ) { return m_szTargetName; }
	VOID								SetTargetName( const CHAR* pName )
	{
		strncpy( m_szTargetName, pName, MAX_CHARACTER_NAME-1 );
		m_uNameSize = (UCHAR)strlen(m_szTargetName);
	}

	const CHAR*							GetMood( ) { return m_szMood; }
	VOID								SetMood( const CHAR* pMood )
	{
		strncpy( m_szMood, pMood, MOOD_DATA_SIZE-1 );
		m_uMoodSize = (UCHAR)strlen(m_szMood);
	}
};

struct WG_RELATION
{
	UCHAR								m_Type;
	PlayerID_t							m_PlayerID;			// 本人的 PlayerID

	union
	{
		RETURN_ADD_RELATION				m_AddRelation;
		RETURN_RELATION_INFO			m_RelationInfo;
		RETURN_VIEW_PLAYER				m_ViewPlayer;
		RELATION_GUID					m_RelationGUID;
		RETURN_NOTIFY_FRIEND			m_NotifyFriend;
		RETURN_ONLINE_LIST				m_RelationOnline;
		RETURN_NOTIFY_ONLINE			m_NotifyOnline;
		REQUEST_VIEW_PLAYER				m_PlayerName;
	};

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//数据应用接口
	PlayerID_t							GetPlayerID( ) { return m_PlayerID; }
	VOID								SetPlayerID( PlayerID_t pid ) { m_PlayerID = pid; }
};

/////////////////////////////////////////////////////////////////////////////////

struct _FRIEND_INFO
{
	GUID_t								m_GUID;				// GUID
	UCHAR								m_uNameSize;
	CHAR								m_szTargetName[MAX_CHARACTER_NAME];
	UCHAR								m_uRelationType;	// 关系类型
	UCHAR								m_uGroup;			// 组
	INT									m_nFriendpoint;		// 关系值
	VOID								CleanUp()
	{
		m_GUID							= INVALID_ID;
		m_uNameSize						= 0;
		memset((void*)m_szTargetName, 0, sizeof(m_szTargetName));
		m_uGroup						= 0;
		m_nFriendpoint					= 0;
	}
	UINT								GetSize() const
	{
		UINT							uSize;
		uSize							= sizeof(m_GUID) + sizeof(m_uNameSize) + m_uNameSize
										+ sizeof(m_uRelationType) + sizeof(m_uGroup) + sizeof(m_nFriendpoint);
		return uSize;
	}
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;
};

struct _BLACKNAME_INFO
{
	GUID_t								m_GUID;				// GUID
	UCHAR								m_uNameSize;
	CHAR								m_szTargetName[MAX_CHARACTER_NAME];
	VOID								CleanUp()
	{
		m_GUID							= INVALID_ID;
		m_uNameSize						= 0;
		memset((void*)m_szTargetName, 0, sizeof(m_szTargetName));
	}
	UINT								GetSize() const
	{
		UINT							uSize;
		uSize							= sizeof(m_GUID) + sizeof(m_uNameSize) + m_uNameSize;
		return uSize;
	}
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;
};

struct _OWN_RELATION;

// 好友列表以及黑名单
struct GC_RELATIONLIST
{
	//数据
	UCHAR								m_uFriendCount;				// 好友数
	UCHAR								m_uBlackCount;				// 黑名单数
	UCHAR								m_uMoodSize;
	CHAR								m_szMood[MOOD_DATA_SIZE];	// 自己的心情
	_FRIEND_INFO						m_FriendInfo[RELATION_BLACKNAME_OFFSET - RELATION_FRIEND_OFFSET];
	_BLACKNAME_INFO						m_BlackNameInfo[MAX_RELATION_SIZE - RELATION_BLACKNAME_OFFSET];

	//基本接口
	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;

	//数据应用接口
	UCHAR								GetFriendCount( ) { return m_uFriendCount; }

	UCHAR								GetBlackCount( ) { return m_uBlackCount; }

	const CHAR*							GetMood( ) { return m_szMood; }
	VOID								SetMood( const CHAR* pMood )
	{
		strncpy( m_szMood, pMood, MOOD_DATA_SIZE-1 );
		m_uMoodSize = (UCHAR)strlen(m_szMood);
	}

	VOID								AddFriend( const _OWN_RELATION* pRelation );
	const _FRIEND_INFO*					GetFriend( INT index );

	VOID								AddBlackName( const _OWN_RELATION* pRelation );
	const _BLACKNAME_INFO*				GetBlackName( INT index );
};

struct GC_RELATION
{
	UCHAR								m_Type;

	union
	{
		GC_RELATIONLIST					m_RelationList;
		RETURN_ADD_RELATION				m_NewRelation;
		RETURN_RELATION_INFO			m_RelationInfo;
		RETURN_VIEW_PLAYER				m_ViewPlayer;
		RELATION_GUID_UCHAR_UCHAR		m_RelationGUIDUCHARUCHAR;
		RELATION_GUID					m_RelationGUID;
		RETURN_NOTIFY_FRIEND			m_NotifyFriend;
		RETURN_ONLINE_LIST				m_RelationOnline;
		RETURN_NOTIFY_ONLINE			m_NotifyOnline;
		REQUEST_MODIFY_MOOD				m_NewMood;
		REQUEST_VIEW_PLAYER				m_PlayerName;
	};

	VOID								CleanUp( );
	UINT								GetSize( ) const;
	VOID								Read( SocketInputStream& iStream );
	VOID								Write( SocketOutputStream& oStream ) const;
};

//////////////////////////////////////////////////////////////////////////

#endif
