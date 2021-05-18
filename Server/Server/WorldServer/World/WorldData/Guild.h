/////////////////////////////////////////////////////////////////////////////////
//文件名：Guild.h
//功能描述：帮会数据和逻辑功能
/////////////////////////////////////////////////////////////////////////////////


#ifndef __GUILD_H__
#define __GUILD_H__

#include "Type.h"
#include "GameDefine_Guild.h"
#include "DB_Struct.h"

#define MAX_USER_IN_GUILD			160
#define USER_ARRAY_SIZE				MAX_USER_IN_GUILD + MAX_PROPOSER_IN_GUILD

//帮会中成员的信息
struct GUILDUSER
{
	GUID_t				m_UserGUID;							//玩家GUID
	CHAR				m_szUserName[MAX_CHARACTER_NAME];	//玩家名字
	GUILD_POSITION		m_Position;							//职务
	UINT				m_uAccess;							//权限
	BOOL				m_bOnlineFlag;						//是否在线，此条不需要存储
	UINT				m_uLastLoginTime;					//最后上线时间
	UINT				m_uLevel;							//人物级别
	CHAR				m_bMenPaiID;						//门派
	INT					m_iCurContribute;					//贡献
	INT					m_iMaxContribute;					//最大贡献
	INT					m_iJoinTime;						//加入时间

	GUILDUSER( )
	{
		CleanUp( );
	};
	VOID CleanUp( )
	{	
		m_UserGUID		= INVALID_ID;
		memset( m_szUserName, 0, sizeof(CHAR)*MAX_CHARACTER_NAME );
		m_Position		= GUILD_POSITION_TRAINEE;
		m_uAccess		= 0;
		m_bOnlineFlag	= FALSE;
		m_uLastLoginTime= 0;
		m_uLevel		=	0;
		m_bMenPaiID		=	-1;
		m_iCurContribute=	0;
		m_iMaxContribute=	0;
		m_iJoinTime		=	0;
	}
};

struct _GUILD_INIT
{
	GuildID_t			m_GuildID;							//帮会ID
	CHAR				m_GuildName[MAX_GUILD_NAME_SIZE];	//帮会名称
	GUILD_STATUS		m_Status;							//帮会的状态
	GUID_t				m_ChieftainGUID;					//帮主 GUID
	INT					m_nProposerCount;					//申请人数
	INT					m_UserCount;						//帮会用户数量
	INT					m_MaxUserSize;						//帮会人口上限
	INT					m_GuildPoint;						//帮会贡献点
	INT					m_GuildMoney;						//帮会资金
	INT					m_nTime;							//成立时间或创建剩余时间
	GUILDUSER			m_aGuildUser[USER_ARRAY_SIZE];		//帮会成员

	_GUILD_INIT( )
	{
		m_GuildID = INVALID_ID;
		memset( m_GuildName, 0, sizeof(CHAR)*MAX_GUILD_NAME_SIZE );
		m_Status = GUILD_STATUS_INVALID;
		m_ChieftainGUID = INVALID_ID;
		m_nProposerCount = 0; // 申请人数
		m_UserCount = 0;
		m_MaxUserSize = 0;
		m_GuildPoint = 0;
		m_GuildMoney = 0;
		m_nTime = 0;
		for( INT i=0; i<USER_ARRAY_SIZE; i++ )
		{
			m_aGuildUser[i].CleanUp();
		}
	};
	~_GUILD_INIT( )
	{
	};
};

class Packet;
class City;
class USER;

class Guild
{
public :
	Guild( );
	~Guild( );
	//-------------------------------------------------------------------------------------------------
	//enum
	//-------------------------------------------------------------------------------------------------
	enum PacketType
	{
		PACKET_RETURN	= 1, // 返回操作
	};

	//操作类型
	enum OPT_TYPE
	{
		OPT_NONE	= 0,	// 返回操作
		OPT_ASK_MEMBER_LIST,			//请求当前所有帮派成员列表
		OPT_ASK_DETAIL_INFO,			//请求帮派的详细信息
		OPT_ASK_APPOINT_POS_INFO,		//请求帮派的可任命信息
		OPT_ASK_SELF_INFO,				//请求本人帮派信息
	};

	//-------------------------------------------------------------------------------------------------
	//初始化
	//-------------------------------------------------------------------------------------------------
	BOOL				Init( _GUILD_INIT* pInit );
	BOOL				InitFromShareMem();
	VOID				ValidateShareMem();
	VOID				CleanUp( );

	//-------------------------------------------------------------------------------------------------
	//基本功能
	//-------------------------------------------------------------------------------------------------
	BOOL				HeartBeat( UINT uTime=0 );		//逻辑心跳接口
	BOOL				IsMemberFull( ) { return (m_UserCount >= m_MaxUserSize); }// 是否帮会已经满员
	BOOL				IsProposerFull( ) { return (m_nProposerCount >= MAX_PROPOSER_IN_GUILD); }// 是否已经有太多玩家申请
	BOOL				IsAuthorized( GUID_t userGUID, GUILD_AUTHORITY authority );	// 判断是否有某项权限
	BOOL				IsSameCamp(CampID_t nCamp) { return GetAmbi() == (INT)nCamp; } //是否同阵营

	//-------------------------------------------------------------------------------------------------
	// 帮会的事件接口
	//-------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------------
	// 创建帮会，还不算正式帮会，FALSE 表示帮会名称与现有名称重名
	BOOL				OnGuildCreate( const USER* pCreater, const CHAR* szName, CampID_t nCamp );
	VOID				OnGuildCreateFailed();
	VOID				OnGuildFound();
	BOOL				OnGuildDestroy( GUID_t createrGUID );

	//-------------------------------------------------------------------------------------------------
	// 任免职务，权限变更，权限判定
	GUILD_ADMIN_t::ORESULT	OnUserPositionChange( GUID_t userGUID, GUILD_POSITION position );//任免职务的原则,一定是先免职为普通帮众,再升职为某个官员
	
	VOID				OnUserAuthorityChange( GUID_t userGUID, GUILD_AUTHORITY authority );
	VOID				OnPositionAuthorityChange( GUILD_POSITION position, GUILD_AUTHORITY authority );
	VOID				OnUserAccessChange( GUID_t userGUID, INT newAccess ); //玩家权限变化
	VOID				OnGuildPointChange( GUID_t userGUID, INT nMoney ); //帮会贡献点发生变化
	BOOL				CanDoIt(OPT_TYPE Opt, GUILD_POSITION position, GUILD_AUTHORITY authority);//根据职位，权限操作判定操作
	BOOL				CanAppointedBy(GUILD_POSITION position, const GUILDUSER* pUser);//是否能被此人任命
	CHAR*				GetPosName(INT iPosition);//获得此帮会中当前职位的名字

	//-------------------------------------------------------------------------------------------------
	// 申请入帮，或者响应一个帮会
	GUILD_ERROR_TYPE	OnUserEnter( const USER* pUser, CampID_t nCamp );
	GUILD_ERROR_TYPE	OnRecruitUser( GUID_t userGUID );// 招收帮众
	GUILD_ERROR_TYPE	OnUserLeave( GUID_t userGUID );// 开除帮众离开帮会
	GUILD_ERROR_TYPE	OnUserLeaveByIndex( INT iIndex );// 开除帮众离开帮会
	GUILD_ERROR_TYPE	OnDemise();// 禅让，和任免职务不同的是，禅让需要将现有职务的人挤下去
	VOID				OnGuildMoneyChange( GUID_t userGUID, INT nMoney ); //帮会资金发生变化// 支取金额存入金额

	//-------------------------------------------------------------------------------------------------
	// 帮会玩家上下线
	VOID				OnUserLogin( GUID_t userGUID );
	VOID				OnUserLogoff( GUID_t userGUID );

	//-------------------------------------------------------------------------------------------------
	// 广播，对 position 以上职务的帮众广播操作返回消息
	VOID				BroadCast_Return( Packet* pPacket, GUILD_POSITION positon = GUILD_POSITION_MEMBER );

	//-------------------------------------------------------------------------------------------------
	//操作锁
	BOOL				IsLock(){return m_IsLock;}
	VOID				SetLock(BOOL bIsLock){m_IsLock	=	bIsLock;}

	//-------------------------------------------------------------------------------------------------
	//数据接口
	//-------------------------------------------------------------------------------------------------

	//此帮会的ID
	GuildID_t			GetGuildID( ){ return m_GuildID; }

	// 帮会名称
	const CHAR*			GetGuildName( ) { return m_GuildName; }

	// 帮会宗旨
	const CHAR*			GetGuildDesc( ) { return m_GuildDesc; }

	// 帮会宗旨
	VOID				SetGuildDesc(CHAR*	pszGuildDesc)
	{
		memset(m_GuildDesc, 0, MAX_GUILD_DESC_SIZE);
		memcpy(m_GuildDesc, pszGuildDesc, MAX_GUILD_DESC_SIZE); 
	}

	BYTE				GetGuildLevel(){ return m_GuildLevel;}
	VOID				SetGuildLevel(BYTE GuildLevel){m_GuildLevel = GuildLevel;}

	//帮主名
	CHAR*				GetGuildChiefName(){return m_GuildChiefName;}

	//创建者名
	CHAR*				GetCreatorName( ) { return m_GuildCreatorName; }

	//帮派的领地
	City*				GetCity(){ return m_pCity;}

	//成立时间
	INT					GetFoundedTime(){return m_nTime;}

	// 取得帮众
	const GUILDUSER*	GetGuildUser( GUID_t userGUID );

	// 通过索引获得帮众
	GUILDUSER*	GetGuildUserByIndex( INT nIndex);

	// 取得帮众数量
	INT					GetGuildUserCount( ) { return m_UserCount; }

	// 取得预备帮众数量
	INT					GetGuildProposerCount( ) { return m_nProposerCount; }

	// 取得帮众数量
	INT					GetGuildUserMax( ) { return m_MaxUserSize; }

	// 帮会状态
	GUILD_STATUS		GetGuildStatus( ) { return m_Status; }

	// 帮主 GUID
	GUID_t				GetChieftainGUID( ) { return m_ChieftainGUID; }

	//-------------------------------------------------------------------------------------------------
	//帮派属性接口
	
	//资历
	INT					GetLongevity(){ return m_Longevity;}
	VOID				SetLongevity(INT Longevity){ m_Longevity = Longevity;}

	//贡献度
	INT					GetContribute(){ return m_Contribute;}
	VOID				SetContribute(INT Contribute){ m_Contribute = Contribute;}

	//人气
	INT					GetHonor(){ return m_Honor;}
	VOID				SetHonor(INT Honor){ m_Honor = Honor;}

	//工业度
	INT					GetIndustry(){ return m_nIndustryLevel;}
	VOID				SetIndustry(INT nIndustryLevel){ m_nIndustryLevel = nIndustryLevel;}

	//农业度
	INT					GetAgr(){ return m_nAgrLevel;}
	VOID				SetAgr(INT nAgrLevel){ m_nAgrLevel = nAgrLevel;}

	//商业度
	INT					GetCom(){ return m_nComLevel;}
	VOID				SetCom(INT nComLevel){ m_nComLevel = nComLevel;}

	//防卫度
	INT					GetDef(){ return m_nDefLevel;}
	VOID				SetDef(INT nDefLevel){ m_nDefLevel = nDefLevel;}

	//科技度
	INT					GetTech(){ return m_nTechLevel;}
	VOID				SetTech(INT nTechLevel){ m_nTechLevel = nTechLevel;}

	//扩张度
	INT					GetAmbi(){ return m_nAmbiLevel;}
	VOID				SetAmbi(INT nAmbiLevel){ m_nAmbiLevel = nAmbiLevel;}

	//帮会资金
	INT					GetMoney(){ return m_GuildMoney;}
	VOID				SetMoney(INT GuildMoney){ m_GuildMoney = GuildMoney;}


protected :
	//-------------------------------------------------------------------------------------------------
	//帮会内部逻辑及功能接口
	VOID				AddGuildUser( GUILDUSER& GuildUser );

	
protected :
	inline GUILDUSER*	ILGetGuildUser( GUID_t userGUID );
	inline INT			ILGetGuildUserIndex( GUID_t userGUID );

	GuildID_t			m_GuildID; //帮会ID
	CHAR				m_GuildName[MAX_GUILD_NAME_SIZE];		//帮会名称
	CHAR				m_GuildDesc[MAX_GUILD_DESC_SIZE];		//帮会描述
	CHAR				m_GuildChiefName[MAX_CHARACTER_NAME];	//帮主名
	CHAR				m_GuildCreatorName[MAX_CHARACTER_NAME];	//创建者名
	BYTE				m_GuildLevel;
	GUILD_STATUS		m_Status;								//帮会的状态
	GUID_t				m_ChieftainGUID;						//帮主的 GUID
	INT					m_nProposerCount;						//申请人数
	INT					m_UserCount;							//帮会用户数量
	INT					m_MaxUserSize;							//帮会人口上限
	INT					m_GuildPoint;							//帮会贡献点
	GUILDUSER			m_aGuildUser[USER_ARRAY_SIZE];			//帮会成员
	BOOL				m_IsLock;								//当前工会是否被锁住，某些操作，只能同时存在一个，如创建城市
	CityID_t			m_CityID;								//绑定的城市ID
	City*				m_pCity;								//绑定的城市指针

	// GUILD_STATUS_UNCOMMITTED 时，表示剩余毫秒数
	// GUILD_STATUS_NORMAL 时，表示帮会成立时间
	INT					m_nTime;
	GuildSMU*			m_pGuildSmu;
protected:
	//-------------------------------------------------------------------------------------------------
	//帮派属性
	INT					m_Longevity;				//资历
	INT					m_Contribute;				//贡献度
	INT					m_Honor;					//人气
	INT					m_nIndustryLevel;			//工业度
	INT					m_nAgrLevel;				//农业度
	INT					m_nComLevel;				//商业度
	INT					m_nDefLevel;				//防卫度
	INT					m_nTechLevel;				//科技度
	INT					m_nAmbiLevel;				//扩张度
	INT					m_GuildMoney;				//帮会资金

	//-------------------------------------------------------------------------------------------------
	//管理结构
	GUILD_ADMIN_t		m_AdminLayOut;							


private :

	// 用于存储心跳传来的时间戳
	UINT				m_uTimeStamp;

	// 分配帮众
	INT					m_nUserMallocIndex;
};


#endif
