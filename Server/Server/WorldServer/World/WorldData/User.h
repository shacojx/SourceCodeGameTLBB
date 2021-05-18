#ifndef __WORLD_USER_H__
#define __WORLD_USER_H__

#include "Type.h"
#include "IDTable.h"
#include "DB_Struct.h"

enum USER_STATUS
{
	US_NONE = 0 ,		//空
	US_HOLDUSERDATA ,	//拥有当前玩家的档案信息
	US_CHANGESCENE,		//跨服务器切换场景
	US_NORMAL ,			//玩家正在游戏中，一切正常
	US_OFFLINE ,		//玩家处于断线状态中
	US_CRASH_DOWN,		//Server 崩溃的玩家
	US_WAIT_SHUTDOWN,	//Server 发送了安全停机的玩家

	US_SIZE,
};

struct USER_GUILD_DATA
{
	UINT					m_uAccess;					//权限

	USER_GUILD_DATA( )
	{
		CleanUp( );
	}
	VOID CleanUp( )
	{
	}
};

struct _RELATION_USER
{
	GUID_t					m_UserGUID;
	RELATION_TYPE			m_RelationType;
	INT						m_nFriendPoint;				// 目前这个友好度不会即时更新，仅用做判断是否需要上下线提示

	VOID					CleanUp()
	{
		m_UserGUID			= INVALID_ID;
		m_RelationType		= RELATION_TYPE_NONE;
		m_nFriendPoint		= 0;
	}
};

struct _RELATION_LIST
{
	INT						m_nCount;
	_RELATION_USER			m_RelationUsers[MAX_RELATION_SIZE];

	VOID					CleanUp()
	{
		m_nCount			= 0;

		for( INT i=0; i<MAX_RELATION_SIZE; ++i )
		{
			m_RelationUsers[i].CleanUp();
		}
	}
};

struct USER_RELATION_DATA
{
	Flag16					m_Flag;							//接收信息标志，见 enum GAME_SETTING_FLAG
	CHAR					m_szMood[MOOD_DATA_SIZE];		// 心情
	_RELATION_LIST			m_RelationList;					// 关系人类型

	IDTable					m_RelationGUIDTable;			// 好友和黑名单都在此缓存

	USER_RELATION_DATA()
	{
		CleanUp( );
		m_RelationGUIDTable.InitTable( MAX_RELATION_SIZE*2 );
	}
	VOID CleanUp( )
	{
		m_Flag.CleanUp( );
		memset( m_szMood, 0, sizeof(m_szMood) );
		m_RelationList.CleanUp();
		m_RelationGUIDTable.CleanUp();
	}
};


struct FULLUSERDATA;
//struct _CAMP_DATA;
class USER
{
public:

	USER( )
	{
		m_UserID			= INVALID_ID;
		CleanUp( );
	}
	void CleanUp( )
	{
		m_UserStatus		= US_NONE;
		m_pFullUserData		= NULL;
		m_PlayerID			= INVALID_ID;
		m_TeamID			= INVALID_ID;
		m_SceneID			= INVALID_ID;
		m_OldSceneID		= INVALID_ID;
		m_ChannelID			= INVALID_ID;
		m_uKey				= 0;
		m_SimpleData.CleanUp( );
		m_GuildData.CleanUp( );
		m_RelationData.CleanUp( );
		m_KickTime			= 0;
		m_LastTime			= 0;
	}

	// 取得玩家的 UserID
	inline ID_t				GetUserID() const;

	// 设置玩家的 UserID
	inline VOID				SetUserID( ID_t id );

	// 取得玩家状态
	inline UINT				UserStatus() const;

	// 设置玩家状态
	inline VOID				SetUserStatus( UINT uStatus );

	// 取得完整玩家数据
	inline FULLUSERDATA*	GetFullUserData() const;

	// 设置完整玩家数据
	inline VOID				SetFullUserData( FULLUSERDATA* pData );

	// 取得玩家的 ServerID
	inline ID_t				GetServerID() const;

	// 取得玩家 PlayerID
	inline PlayerID_t		GetPlayerID() const;

	// 设置玩家 PlayerID
	inline VOID				SetPlayerID( PlayerID_t pid );

	// 取得玩家 GUID
	inline GUID_t			GetGUID() const;

	// 设置玩家 GUID
	inline VOID				SetGUID( GUID_t guid );

	// 取得玩家 SceneID
	inline SceneID_t		GetSceneID() const;

	// 设置玩家 SceneID
	inline VOID				SetSceneID( SceneID_t SceneID );

	// 取得玩家 SceneID
	inline SceneID_t		GetOldSceneID() const;

	// 设置玩家 SceneID
	inline VOID				SetOldSceneID( SceneID_t SceneID );

	// 得到玩家常用数据
	inline const USER_SIMPLE_DATA* GetUserSimpleData() const;

	// 取得玩家名字
	inline const CHAR*		GetName() const;

	// 设置玩家名字
	inline VOID				SetName( const CHAR* szName );

	// 取得账户名字
	inline const CHAR*		GetAccount() const;

	// 设置账户名字
	inline VOID				SetAccount( const CHAR* szName );

	// 取得官网GUID
	inline GUID_t			GetAccountGUID() const;

	// 设置官网GUID
	inline VOID				SetAccountGUID( UINT uAccountGUID );

	// 取得玩家性别
	inline UINT				GetSex() const;

	// 设置玩家性别
	inline VOID				SetSex( UINT uSex );

	// 取得玩家的门派号
	inline INT				GetMenpai() const;

	// 设置玩家的门派号
	inline VOID				SetMenpai(INT nMenpai);

	// 得到玩家等级
	inline INT				GetLevel() const;

	// 设置玩家等级
	inline VOID				SetLevel(INT nLevel);

	// 得到玩家的头像
	inline INT				GetPortrait() const;

	// 设置玩家头像
	inline VOID				SetPortrait(INT nPortrait);

	// 得到脸部模型
	inline UCHAR			GetFaceModel() const;

	// 设置脸部模型
	inline VOID				SetFaceModel(UCHAR uFaceMeshID);

	// 得到头发模型
	inline UCHAR			GetHairModel() const;

	// 设置头发模型
	inline VOID				SetHairModel(UCHAR uHairMeshID);

	// 得到发色
	inline UINT				GetHairColor() const;

	// 设置发色
	inline VOID				SetHairColor(UINT uHairColor);

	// 取得玩家的队伍号
	inline TeamID_t			GetTeamID() const;

	// 设置玩家队伍号
	inline VOID				SetTeamID( TeamID_t tid );

	// 取得玩家的称号
	inline const CHAR*		GetTitle() const;

	// 设置玩家的称号
	inline VOID				SetTitle(const CHAR* szTitle);

	// 得到玩家帮会 ID
	inline GuildID_t		GetGuildID() const;

	// 设置玩家帮会 ID
	inline VOID				SetGuildID(GuildID_t gid);

	// 得到玩家创建的聊天频道号
	inline ChannelID_t		GetChannelID() const;

	// 设置玩家创建的聊天频道号
	inline VOID				SetChannelID( ChannelID_t cid );

	// 得到用户验证码
	inline UINT				GetKey() const;

	// 设置用户验证码
	inline VOID				SetKey( UINT uKey );

	// 清除帮会数据
	inline VOID				CleanUpGuildData();

	// 清除玩家关系数据
	inline VOID				CleanUpRelationData();

	// 增加一个关系人
	VOID					AddRelation( GUID_t guid, const _RELATION_USER* pUser );

	// 移除一个关系人
	VOID					DelRelation( GUID_t guid );

	// 得到和某玩家的关系状态
	enum RELATION_TYPE		GetRelationType( GUID_t guid );

	// 判断 guid 是否亲密好友
	BOOL					IsGoodFriend(GUID_t guid);

	// 增加一个亲密好友
	VOID					AddGoodFriend(GUID_t guid);

	// 取得玩家的心情
	inline const CHAR*		GetMood() const;

	// 保存心情
	inline VOID				SetMood( const CHAR* szMood );

	// 保存设置
	inline VOID				SetSettings(USHORT uSettings);

	// 是否拒绝被加为好友
	inline BOOL				IsCannotAddFriend();

	// 是否拒绝收发邮件
	inline BOOL				IsCannotRecvMail();

	// 是否拒绝陌生人的邮件
	inline BOOL				IsCannotRecvStrangerMail();

	// 是否拒绝组队邀请
	inline BOOL				IsRefuseTeamInvite();

	// 填充密友在线列表
	VOID					FillOnlineList( RETURN_ONLINE_LIST* pSend );

	// 通知在线密友上线信息
	VOID					InformOnlineMsg();

	// 通知在线密友下线信息
	VOID					InformOfflineMsg();

	// 给该玩家发送某好友上线的信息
	VOID					SendRelationOnlineMsg(USER* pUser);

	// 给该玩家发送某好友下线的信息
	VOID					SendRelationOfflineMsg(USER* pUser);

	// 返回是否可以发送邮件，如果可以的话，则返回 0，否则返回错误号，错误号参见 enum RELATION_RETURN_TYPE
	UCHAR					ValidateMail(const MAIL* pMail);

	// 收到一封邮件
	BOOL					RecvMail(const MAIL* pMail);

	// 心跳处理
	BOOL					HeartBeat( UINT uTime );

	// 设置踢除时间
	VOID					ResetKickTime( );

	const _CAMP_DATA*		GetUserCampData();

	VOID 					SetUserCampData( _CAMP_DATA* pData );

	BYTE					GetUserAge();

	VOID 					SetUserAge( BYTE age );
public:
	BOOL					IsEnemy( const _CAMP_DATA* pData );

private:
	ID_t					m_UserID;			//在线玩家的用户 ID, 即在线用户列表的数组下标
	UINT					m_UserStatus;		//在线玩家的状态， enum USER_STATUS
	FULLUSERDATA*			m_pFullUserData;	//在线玩家的完整数据
	PlayerID_t				m_PlayerID;			//此用户所在场景的PlayerID
	SceneID_t				m_SceneID;			//此用户所在场景ID
	SceneID_t				m_OldSceneID;		//此用户所在场景ID
	TeamID_t				m_TeamID;			//此用户所在的队伍ID
	ChannelID_t				m_ChannelID;		//此用户创建的聊天频道号
	UINT					m_uKey;				//用户验证码
	USER_SIMPLE_DATA		m_SimpleData;		//此用户的基本信息
	USER_GUILD_DATA			m_GuildData;		//帮派相关数据
	USER_RELATION_DATA		m_RelationData;		//联系人相关数据
	INT						m_KickTime;			//没有消息时候的踢除剩余时间
	UINT					m_LastTime;			//上一次调用时间
	_CAMP_DATA				m_CampData;			//玩家阵营
	BYTE					m_Age;				//玩家年龄
};

#include "User.inl"

#endif // __WORLD_USER_H__
