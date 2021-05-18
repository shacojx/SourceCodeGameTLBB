#ifndef __GAMESTRUCT_GUILD_H__
#define __GAMESTRUCT_GUILD_H__

#include "Type.h"
#include "GameDefine_Guild.h"

class SocketInputStream;
class SocketOutputStream;

// 帮会消息包的接口
struct GUILD_PACKET
{
	virtual UINT			GetPacketSize() const = 0;
	virtual VOID			Read(SocketInputStream& iStream) = 0;
	virtual VOID			Write(SocketOutputStream& oStream) const = 0;
};

// 询问帮会列表
struct GUILD_CGW_ASKLIST : public GUILD_PACKET
{
	UCHAR					m_SortType;
	USHORT					m_uStart;

	CampID_t				m_Camp; //查询者的阵营

	GUILD_CGW_ASKLIST()
	{
		m_SortType = 0;
		m_uStart = 0;
		m_Camp = -1;
	}

	virtual UINT			GetPacketSize() const { return sizeof(m_SortType) + sizeof(m_uStart) + sizeof(m_Camp); }
	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;
};

// 创建帮会
struct GUILD_CGW_CREATE : public GUILD_PACKET
{
	UCHAR					m_NameSize;
	CHAR					m_szGuildName[MAX_GUILD_NAME_SIZE];

	UCHAR					m_DescSize;
	CHAR					m_szGuildDesc[MAX_GUILD_DESC_SIZE];

	CampID_t				m_Camp; //帮主的阵营

	GUILD_CGW_CREATE()
	{
		m_NameSize = 0;
		memset((void*)m_szGuildName, 0, MAX_GUILD_NAME_SIZE);

		m_DescSize = 0;
		memset((void*)m_szGuildDesc, 0, MAX_GUILD_DESC_SIZE);

		m_Camp = -1;
	}

	virtual UINT			GetPacketSize() const { return sizeof(m_NameSize) + m_NameSize + sizeof(m_DescSize) + m_DescSize + sizeof(m_Camp); }
	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;
};

// 加入帮会
struct GUILD_CGW_JOIN : public GUILD_PACKET
{
	GuildID_t				m_GuildGUID;

	CampID_t				m_Camp; //加入者的阵营

	GUILD_CGW_JOIN()
	{
		m_GuildGUID = 0;
		m_Camp = -1;
	}

	virtual UINT			GetPacketSize() const { return sizeof(m_GuildGUID) + sizeof(m_Camp); }
	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;
};

// 询问帮会信息
struct GUILD_CGW_ASKINFO : public GUILD_PACKET
{
	enum
	{
		GUILD_MEMBER_INFO = 0,	//帮众信息
		GUILD_INFO,			//帮会信息
		GUILD_APPOINT_POS,	//帮会中可任命的职位
		GUILD_SELF_INFO,	//本人帮派信息

	};
	GuildID_t				m_GuildGUID;
	BYTE					m_Type;				//帮众信息，还是帮会信息

	virtual UINT			GetPacketSize() const { return sizeof(m_GuildGUID)+sizeof(BYTE); }
	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;
};

// 帮会任免职务
struct GUILD_CGW_APPOINT : public GUILD_PACKET
{
	GuildID_t				m_GuildGUID;
	GUID_t					m_CandidateGUID;
	UCHAR					m_NewPosition;

	virtual UINT			GetPacketSize() const
	{
		return sizeof(m_GuildGUID) + sizeof(m_CandidateGUID) + sizeof(m_NewPosition);
	}
	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;
};

// 帮会调整权限
struct GUILD_CGW_ADJUSTAUTH : public GUILD_PACKET
{
	GuildID_t				m_GuildGUID;
	GUID_t					m_CandidateGUID;
	UCHAR					m_NewAuthority;

	virtual UINT			GetPacketSize() const
	{
		return sizeof(m_GuildGUID) + sizeof(m_CandidateGUID) + sizeof(m_NewAuthority);
	}
	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;
};

// 招收新帮众
struct GUILD_CGW_RECRUIT : public GUILD_PACKET
{
	GUID_t					m_ProposerGUID;

	virtual UINT			GetPacketSize() const { return sizeof(m_ProposerGUID); }
	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;
};

// 开除帮众
struct GUILD_CGW_EXPEL : public GUILD_PACKET
{
	GUID_t					m_GuildUserGUID;

	virtual UINT			GetPacketSize() const { return sizeof(m_GuildUserGUID); }
	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;
};

// 提取帮资
struct GUILD_CGW_WITHDRAW : public GUILD_PACKET
{
	UINT					m_MoneyAmount;

	virtual UINT			GetPacketSize() const { return sizeof(m_MoneyAmount); }
	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;
};

// 存入帮资
struct GUILD_CGW_DEPOSIT : public GUILD_PACKET
{
	UINT					m_MoneyAmount;

	virtual UINT			GetPacketSize() const { return sizeof(m_MoneyAmount); }
	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;
};

// 离开帮会
struct GUILD_CGW_DEMISE : public GUILD_PACKET
{
	virtual UINT			GetPacketSize() const { return 0; }
	virtual VOID			Read(SocketInputStream& iStream) {}
	virtual VOID			Write(SocketOutputStream& oStream) const {}
};

// 离开帮会
struct GUILD_CGW_LEAVE : public GUILD_PACKET
{
	virtual UINT			GetPacketSize() const { return 0; }
	virtual VOID			Read(SocketInputStream& iStream) {}
	virtual VOID			Write(SocketOutputStream& oStream) const {}
};

// 更改帮会宗旨
struct GUILD_CGW_CHANG_DESC : public GUILD_PACKET
{
	CHAR					m_GuildDesc[MAX_GUILD_DESC_SIZE];
	BYTE					m_GuildDescLength;

	GUILD_CGW_CHANG_DESC()
	{
		memset(m_GuildDesc, 0, MAX_GUILD_DESC_SIZE);
		m_GuildDescLength	=	0;
	}

	virtual UINT			GetPacketSize() const { return sizeof(BYTE) + m_GuildDescLength; }
	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;
};

//帮会的可任命信息
struct GUILD_WGC_APPOINT_INFO : public GUILD_PACKET
{
	struct s
	{
		CHAR	m_PosName[MAX_GUILD_POS_NAME_SIZE];
		BYTE	m_PosID;
		s()
		{
			memset (m_PosName, 0, MAX_GUILD_POS_NAME_SIZE);
			m_PosID			=	0;
		}	
	} m_PosList[GUILD_POSITION_SIZE];
	INT iPosNum;

	GUILD_WGC_APPOINT_INFO()
	{
		iPosNum = 0;
	}

	virtual UINT			GetPacketSize() const
	{
		return	sizeof(iPosNum)+
				sizeof(s)*iPosNum;
	}

	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;

};

//帮会信息
struct GUILD_WGC_GUILD_INFO : public GUILD_PACKET
{
	CHAR			m_GuildName[MAX_GUILD_NAME_SIZE];
	CHAR			m_GuildCreator[MAX_CHARACTER_NAME];
	CHAR			m_GuildChairMan[MAX_CHARACTER_NAME];
	CHAR			m_CityName[MAX_CITY_NAME_SIZE];
	BYTE			m_nLevel;
	INT				m_nPortSceneID;				//入口场景
	INT				m_MemNum;					//人数
	INT				m_Longevity;				//资历 
	INT				m_Contribute;				//贡献度
	INT				m_Honor;					//人气
	INT				m_FoundedMoney;				//帮派资金
	INT				m_nIndustryLevel;			//工业度
	INT				m_nAgrLevel;				//农业度
	INT				m_nComLevel;				//商业度
	INT				m_nDefLevel;				//防卫度
	INT				m_nTechLevel;				//科技度
	INT				m_nAmbiLevel;				//扩张度
	BYTE			m_bAccess;

	GUILD_WGC_GUILD_INFO()
	{
		CleanUp();
	}
	VOID CleanUp()
	{
		memset(m_GuildName, 0, MAX_GUILD_NAME_SIZE);
		memset(m_GuildCreator, 0, MAX_CHARACTER_NAME);
		memset(m_GuildChairMan, 0, MAX_CHARACTER_NAME);
		memset(m_CityName, 0, MAX_CITY_NAME_SIZE);
		m_nLevel			=	0;
		m_nPortSceneID		=	0;			//入口场景
		m_MemNum			=	0;			//人数
		m_Longevity			=	0;			//资历 
		m_Contribute		=	0;			//贡献度
		m_Honor				=	0;			//人气
		m_FoundedMoney		=	0;			//帮派资金
		m_nIndustryLevel	=	0;			//工业度
		m_nAgrLevel			=	0;			//农业度
		m_nComLevel			=	0;			//商业度
		m_nDefLevel			=	0;			//防卫度
		m_nTechLevel		=	0;			//科技度
		m_nAmbiLevel		=	0;			//扩张度
		m_bAccess			=	0;
	}

	virtual UINT			GetPacketSize() const
	{
		return	sizeof(m_nLevel) +
				sizeof(m_nPortSceneID) +
				sizeof(m_MemNum) +
				sizeof(m_Longevity) +
				sizeof(m_Contribute) +
				sizeof(m_Honor) +
				sizeof(m_FoundedMoney) +
				sizeof(m_nIndustryLevel) +
				sizeof(m_nAgrLevel) +
				sizeof(m_nComLevel) +
				sizeof(m_nDefLevel) +
				sizeof(m_nTechLevel) +
				sizeof(m_nAmbiLevel) +
				sizeof(m_bAccess) +
				MAX_CHARACTER_NAME + 
				MAX_CHARACTER_NAME + 
				MAX_CITY_NAME_SIZE + 
				MAX_GUILD_NAME_SIZE;
	}

	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;

};

// 帮众列表
struct GUILD_WGC_MEMBER_LIST : public GUILD_PACKET
{
	struct s
	{
		CHAR		m_szName[MAX_CHARACTER_NAME];
		GUID_t		m_Guid;
		BYTE		m_bLevel;
		CHAR		m_bMenPaiID;
		INT			m_iCurContribute;
		INT			m_iMaxContribute;
		INT			m_iJoinTime;
		INT			m_iLogOutTime;
		BYTE		m_bIsOnline;
		BYTE		m_bPosition;
		s()
		{
			memset(m_szName, 0, MAX_CHARACTER_NAME);
			m_Guid				= INVALID_ID;
			m_bLevel			=	0;
			m_bMenPaiID			=	-1;
			m_iCurContribute	=	0;
			m_iMaxContribute	=	0;
			m_iJoinTime			=	0;
			m_iLogOutTime		=	0;
			m_bIsOnline			=	0;
			m_bPosition			=	0;	
		}
	}	m_GuildMemberData[USER_ARRAY_SIZE];

	USHORT			m_uValidMemberCount;
	USHORT			m_uMemberCount;
	USHORT			m_uMemberMax;
	CHAR			m_GuildDesc[MAX_GUILD_DESC_SIZE];
	CHAR			m_GuildName[MAX_GUILD_NAME_SIZE];
	BYTE			m_bPosition;
	BYTE			m_bAccess;

	GUILD_WGC_MEMBER_LIST()
	{
		m_uValidMemberCount	=	0;
		m_uMemberCount	=	0;
		m_uMemberMax	=	0;
		m_bPosition		=	0;
		m_bAccess		=	0;
		memset(m_GuildDesc, 0, MAX_GUILD_DESC_SIZE);
		memset(m_GuildName, 0, MAX_GUILD_NAME_SIZE);
	}

	virtual UINT			GetPacketSize() const
	{
		return	sizeof(m_uValidMemberCount) +
				sizeof(m_uMemberCount) + 
				sizeof(m_uMemberMax) + 
				sizeof(m_bPosition) +
				sizeof(m_bAccess) +
				MAX_GUILD_DESC_SIZE + 
				MAX_GUILD_NAME_SIZE + 
				m_uMemberCount * sizeof(s);
	}

	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;

};

// 帮会列表
struct GUILD_WGC_LIST : public GUILD_PACKET
{
	USHORT					m_uStartIndex;
	USHORT					m_uGuildCount;
	UCHAR					m_uGuildListCount;
	CampID_t				m_Camp;

	struct s
	{
		s()
		{
			m_GuildID = INVALID_ID;
			memset((void*)m_GuildName, 0, sizeof(m_GuildName));
			memset((void*)m_GuildDesc, 0, sizeof(m_GuildDesc));
			memset((void*)m_CityName, 0, sizeof(m_CityName));
			memset((void*)m_ChiefName, 0, sizeof(m_ChiefName));
			m_uGuildStatus		= 0;
			m_uGuildUserCount	= 0;
			m_bGuildLevel		= 0;
			m_PortSceneID		= -1;
			m_nFoundedTime		=	0;
		}

		GuildID_t			m_GuildID;
		CHAR				m_ChiefName[MAX_CHARACTER_NAME];
		CHAR				m_GuildName[MAX_GUILD_NAME_SIZE];
		CHAR				m_GuildDesc[MAX_GUILD_DESC_SIZE];
		CHAR				m_CityName[MAX_CITY_NAME_SIZE];
		INT					m_PortSceneID;
		UCHAR				m_uGuildStatus;
		UCHAR				m_uGuildUserCount;
		BYTE				m_bGuildLevel;
		INT					m_nFoundedTime;
	}		m_uGuild[MAX_GUILD_LIST_COUNT];

	GUILD_WGC_LIST()
	{
		m_uStartIndex = 0;
		m_uGuildCount = 0;
		m_uGuildListCount = 0;
		m_Camp = -1;
	}

	virtual UINT			GetPacketSize() const
	{
		return sizeof(m_uStartIndex) + sizeof(m_uGuildCount) + sizeof(m_uGuildListCount) + sizeof(m_Camp)
			 + m_uGuildListCount * sizeof(s);
	}

	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;
};

//个人帮派数据
struct GUILD_WGC_SELF_GUILD_INFO : public GUILD_PACKET
{
	CHAR			m_GuildName[MAX_GUILD_NAME_SIZE];
	BYTE			m_GuildNameSize;
	BYTE			m_bAccess;

	GUILD_WGC_SELF_GUILD_INFO()
	{
		CleanUp();
	}
	VOID CleanUp()
	{
		memset(m_GuildName, 0, MAX_GUILD_NAME_SIZE);
		m_GuildNameSize = 0;
		m_bAccess = 0;
	}

	virtual UINT			GetPacketSize() const
	{
		return	sizeof(BYTE) + sizeof(BYTE) + m_GuildNameSize;
	}

	virtual VOID			Read(SocketInputStream& iStream);
	virtual VOID			Write(SocketOutputStream& oStream) const;
};

struct _GUILD_PACKET
{
	UCHAR					m_uPacketType;			// 消息包类型
	virtual GUILD_PACKET*	GetPacket(INT nPacketType) const = 0;

	UINT					GetPacketSize() const
	{
		GUILD_PACKET*		pGuildPacket;

		pGuildPacket = GetPacket(m_uPacketType);
		if( pGuildPacket == NULL )
		{
			Assert( FALSE );
			return 0;
		}

		return (sizeof(m_uPacketType) + pGuildPacket->GetPacketSize());
	}

	VOID					Read(SocketInputStream& iStream);
	VOID					Write(SocketOutputStream& oStream) const;
};

struct _GUILD_CGW_PACKET : public _GUILD_PACKET
{
	GUILD_CGW_ASKLIST				m_PacketAskList;
	GUILD_CGW_CREATE				m_PacketCreate;
	GUILD_CGW_JOIN					m_PacketJoin;
	GUILD_CGW_ASKINFO				m_PacketAskInfo;
	GUILD_CGW_APPOINT				m_PacketAppoint;
	GUILD_CGW_ADJUSTAUTH			m_PacketJustAuthority;
	GUILD_CGW_RECRUIT				m_PacketRecruit;
	GUILD_CGW_EXPEL					m_PacketExpel;
	GUILD_CGW_WITHDRAW				m_PacketWithdraw;
	GUILD_CGW_DEPOSIT				m_PacketDeposit;
	GUILD_CGW_LEAVE					m_PacketLeave;
	GUILD_CGW_DEMISE				m_PacketDemise;
	GUILD_CGW_CHANG_DESC			m_PacketChangeDesc;

	union u
	{ // 比最大的消息包长了一个虚表长度
		CHAR				u_AskListSize[sizeof(GUILD_CGW_ASKLIST)];
		CHAR				u_CreateSize[sizeof(GUILD_CGW_CREATE)];
		CHAR				u_JoinSize[sizeof(GUILD_CGW_JOIN)];
		CHAR				u_AskInfoSize[sizeof(GUILD_CGW_ASKINFO)];
		CHAR				u_AppointSize[sizeof(GUILD_CGW_APPOINT)];
		CHAR				u_JustAuthoritySize[sizeof(GUILD_CGW_ADJUSTAUTH)];
		CHAR				u_RecruitSize[sizeof(GUILD_CGW_RECRUIT)];
		CHAR				u_ExpelSize[sizeof(GUILD_CGW_EXPEL)];
		CHAR				u_WithdrawSize[sizeof(GUILD_CGW_WITHDRAW)];
		CHAR				u_DepositSize[sizeof(GUILD_CGW_DEPOSIT)];
		CHAR				u_LeaveSize[sizeof(GUILD_CGW_LEAVE)];
		CHAR				u_DemiseSize[sizeof(GUILD_CGW_DEMISE)];
		CHAR				u_ChangeDescSize[sizeof(GUILD_CGW_CHANG_DESC)];
	};

	_GUILD_CGW_PACKET() {}

	virtual GUILD_PACKET*	GetPacket(INT nPacketType) const
	{
		GUILD_PACKET*		pGuildPacket;

		switch(nPacketType)
		{
		case GUILD_PACKET_CG_ASKLIST:
		case GUILD_PACKET_GW_ASKLIST: pGuildPacket = (GUILD_PACKET*)&m_PacketAskList; break;

		case GUILD_PACKET_CG_CREATE:
		case GUILD_PACKET_GW_CREATE: pGuildPacket = (GUILD_PACKET*)&m_PacketCreate; break;
		
		case GUILD_PACKET_CG_JOIN:
		case GUILD_PACKET_GW_JOIN: pGuildPacket = (GUILD_PACKET*)&m_PacketJoin; break;
		
		case GUILD_PACKET_CG_ASKINFO:
		case GUILD_PACKET_GW_ASKINFO: pGuildPacket = (GUILD_PACKET*)&m_PacketAskInfo; break;
		
		case GUILD_PACKET_CG_APPOINT:
		case GUILD_PACKET_GW_APPOINT: pGuildPacket = (GUILD_PACKET*)&m_PacketAppoint; break;
		
		case GUILD_PACKET_CG_ADJUSTAUTHORITY:
		case GUILD_PACKET_GW_ADJUSTAUTHORITY: pGuildPacket = (GUILD_PACKET*)&m_PacketJustAuthority; break;
		
		case GUILD_PACKET_CG_RECRUIT:
		case GUILD_PACKET_GW_RECRUIT: pGuildPacket = (GUILD_PACKET*)&m_PacketRecruit; break;
		
		case GUILD_PACKET_CG_EXPEL:
		case GUILD_PACKET_GW_EXPEL: pGuildPacket = (GUILD_PACKET*)&m_PacketExpel; break;
		
		case GUILD_PACKET_CG_WITHDRAW:
		case GUILD_PACKET_GW_WITHDRAW: pGuildPacket = (GUILD_PACKET*)&m_PacketWithdraw; break;
		
		case GUILD_PACKET_CG_DEPOSIT:
		case GUILD_PACKET_GW_DEPOSIT: pGuildPacket = (GUILD_PACKET*)&m_PacketDeposit; break;
		
		case GUILD_PACKET_CG_LEAVE:
		case GUILD_PACKET_GW_LEAVE: pGuildPacket = (GUILD_PACKET*)&m_PacketLeave; break;
		
		case GUILD_PACKET_CG_DISMISS:
		case GUILD_PACKET_GW_DISMISS: pGuildPacket = (GUILD_PACKET*)&m_PacketLeave; break;
		
		case GUILD_PACKET_CG_DEMISE:
		case GUILD_PACKET_GW_DEMISE: pGuildPacket = (GUILD_PACKET*)&m_PacketDemise; break;
		
		case GUILD_PACKET_CG_CHANGEDESC:
		case GUILD_PACKET_GW_CHANGEDESC: pGuildPacket = (GUILD_PACKET*)&m_PacketChangeDesc; break;
		
		default:
			Assert( FALSE );
			pGuildPacket = NULL;
		}

		return pGuildPacket;
	}

	static UINT				GetPacketMaxSize()
	{
		return (sizeof(UCHAR) + sizeof(u) - sizeof(GUILD_PACKET));
	}

};

struct _GUILD_WGC_PACKET : public _GUILD_PACKET
{
	GUILD_WGC_LIST				m_PacketList;
	GUILD_WGC_MEMBER_LIST		m_PacketMemberList;
	GUILD_WGC_GUILD_INFO		m_PacketGuildInfo;
	GUILD_WGC_APPOINT_INFO		m_PacketAppointList;
	GUILD_WGC_SELF_GUILD_INFO	m_PacketSelfGuildInfo;

	union u
	{ // 比最大的消息包长了一个虚表长度
		CHAR				u_ListSize[sizeof(GUILD_WGC_LIST)];
		CHAR				u_MemberListSize[sizeof(GUILD_WGC_MEMBER_LIST)];
		CHAR				u_GuildInfoSize[sizeof(GUILD_WGC_GUILD_INFO)];
		CHAR				u_AppointInfoSize[sizeof(GUILD_WGC_APPOINT_INFO)];
		CHAR				u_SelfGuildInfoSize[sizeof(GUILD_WGC_SELF_GUILD_INFO)];
	};

	_GUILD_WGC_PACKET() {}

	virtual GUILD_PACKET*	GetPacket(INT nPacketType) const
	{
		GUILD_PACKET*		pGuildPacket;

		switch(nPacketType)
		{
		case GUILD_PACKET_WG_LIST:
		case GUILD_PACKET_GC_LIST: pGuildPacket = (GUILD_PACKET*)&m_PacketList; break;

		case GUILD_PACKET_WG_MEMBER_LIST:
		case GUILD_PACKET_GC_MEMBER_LIST: pGuildPacket = (GUILD_PACKET*)&m_PacketMemberList; break;

		case GUILD_PACKET_WG_GUILD_INFO:
		case GUILD_PACKET_GC_GUILD_INFO: pGuildPacket = (GUILD_PACKET*)&m_PacketGuildInfo; break;

		case GUILD_PACKET_WG_APPOINT_INFO:
		case GUILD_PACKET_GC_APPOINT_INFO: pGuildPacket = (GUILD_PACKET*)&m_PacketAppointList; break;

		case GUILD_PACKET_WG_SELF_GUILD_INFO:
		case GUILD_PACKET_GC_SELF_GUILD_INFO: pGuildPacket = (GUILD_PACKET*)&m_PacketSelfGuildInfo; break;

		default:
			Assert( FALSE );
			pGuildPacket = NULL;
		}

		return pGuildPacket;
	}

	static UINT				GetPacketMaxSize()
	{
		return (sizeof(UCHAR) + sizeof(u) - sizeof(GUILD_PACKET));
	}

};

struct _GUILD_RETURN
{
	CHAR					m_ReturnType;
	GuildID_t				m_GuildID;
	GUID_t					m_GUID; // 需要重设 GuildID 的玩家的 GUID
	GUID_t					m_GUIDChanged; // 需要重设 GuildID 的玩家的 GUID
	BYTE					m_PosID;
	UCHAR					m_SourNameSize;
	CHAR					m_SourName[MAX_CHARACTER_NAME];
	UCHAR					m_DestNameSize;
	CHAR					m_DestName[MAX_CHARACTER_NAME];
	UCHAR					m_GuildNameSize;
	CHAR					m_GuildName[MAX_GUILD_NAME_SIZE];
	UCHAR					m_PositionNameSize;
	CHAR					m_PositionName[MAX_GUILD_POS_NAME_SIZE];
	UCHAR					m_ChangedPositionNameSize;
	CHAR					m_ChangedPositionName[MAX_GUILD_POS_NAME_SIZE];

	_GUILD_RETURN()
	{
		m_ReturnType	=	GUILD_RETURN_INVALID;
		m_GuildID		=	INVALID_ID;
		m_GUID			=	INVALID_ID;
		m_GUIDChanged	=	INVALID_ID;
		m_PosID			=	0;
		m_SourNameSize	=	0;
		memset((void*)m_SourName, 0, sizeof(m_SourName));
		m_DestNameSize = 0;
		memset((void*)m_DestName, 0, sizeof(m_DestName));
		m_GuildNameSize = 0;
		memset((void*)m_GuildName, 0, sizeof(m_GuildName));
		m_PositionNameSize	=	0;
		memset((void*)m_PositionName, 0, sizeof(m_PositionName));	
		m_ChangedPositionNameSize	=	0;
		memset((void*)m_ChangedPositionName, 0, sizeof(m_ChangedPositionNameSize));	

	}

	UINT					GetPacketSize() const
	{
		UINT uSize = sizeof(m_ReturnType);

		switch( m_ReturnType )
		{
		case GUILD_RETURN_CREATE:
		case GUILD_RETURN_JOIN:
			{
				uSize +=	sizeof(m_GuildID)
							+ sizeof(m_GuildNameSize) + m_GuildNameSize;
			}
			break;
		case GUILD_RETURN_RESPONSE:
			{
				uSize += sizeof(m_GuildID) + sizeof(m_GUID) + sizeof(m_SourNameSize) + m_SourNameSize;
			}
			break;
		case GUILD_RETURN_EXPEL:
			{
				uSize +=  sizeof(m_SourNameSize) + m_SourNameSize
						+ sizeof(m_DestNameSize) + m_DestNameSize
						+ sizeof(m_GUID);
			}
			break;
		case GUILD_RETURN_REJECT:
			{
				uSize +=  sizeof(m_GuildNameSize) + m_GuildNameSize
					+ sizeof(m_DestNameSize) + m_DestNameSize
					+ sizeof(m_GUID);
			}
			break;
		case GUILD_RETURN_RECRUIT:
			{
				uSize += sizeof(m_GuildID)
						+ sizeof(m_GUIDChanged)
						+ sizeof(m_SourNameSize) + m_SourNameSize
						+ sizeof(m_DestNameSize) + m_DestNameSize
						+ sizeof(m_GuildNameSize) + m_GuildNameSize
						+ sizeof(m_PositionNameSize) + m_PositionNameSize
						+ sizeof(m_GUID);
			}
			break;
		case GUILD_RETURN_LEAVE:
			{
				uSize +=  sizeof(m_GUID) 
						+ sizeof(m_GuildNameSize) + m_GuildNameSize
						+ sizeof(m_DestNameSize) + m_DestNameSize;
			}
			break;
		case GUILD_RETURN_FOUND:
			{

			}
			break;
		case GUILD_RETURN_DISMISS:
			{
			
			}
			break;
		case GUILD_RETURN_PROMOTE:
			{
				uSize +=  sizeof(m_SourNameSize) + m_SourNameSize
						+ sizeof(m_DestNameSize) + m_DestNameSize
						+ sizeof(m_PositionNameSize) + m_PositionNameSize
						+ sizeof(m_GuildNameSize) + m_GuildNameSize
						+ sizeof(m_PosID)
						+ sizeof(m_GUID)
						+ sizeof(m_GuildID);
			}
			break;
		case GUILD_RETURN_DEMOTE:
			{
				uSize +=  sizeof(m_SourNameSize) + m_SourNameSize
						+ sizeof(m_DestNameSize) + m_DestNameSize
						+ sizeof(m_PositionNameSize) + m_PositionNameSize
						+ sizeof(m_GuildNameSize) + m_GuildNameSize
						+ sizeof(m_PosID)
						+ sizeof(m_GUID)
						+ sizeof(m_GuildID);
			}
			break;
		case GUILD_RETURN_AUTHORIZE:
		case GUILD_RETURN_DEPRIVE_AUTHORITY:
			{
			}
			break;
		case GUILD_RETURN_DEMISE:
			{
				uSize += sizeof(m_GuildID)
					+ sizeof(m_GUID)
					+ sizeof(m_GUIDChanged)
					+ sizeof(m_SourNameSize) + m_SourNameSize
					+ sizeof(m_DestNameSize) + m_DestNameSize
					+ sizeof(m_GuildNameSize) + m_GuildNameSize
					+ sizeof(m_PositionNameSize) + m_PositionNameSize
					+ sizeof(m_ChangedPositionNameSize) + m_ChangedPositionNameSize;
			}
			break;
		case GUILD_RETURN_NAME:
			{

			}
			break;
		case GUILD_RETURN_WITHDRAW:
		case GUILD_RETURN_DEPOSIT:
		default:
			Assert(FALSE);
			return 0;
		}

		return uSize;
	}

	VOID					Read(SocketInputStream& iStream);
	VOID					Write(SocketOutputStream& oStream) const;
};

struct GUILD_POS_t
{
	BYTE	NumPos;
	BYTE	MaxNumPos;
	CHAR	szPosName[MAX_GUILD_POS_NAME_SIZE];
	INT		Posidx[MAX_NUM_PER_POSITION];

	VOID CleanUp()
	{
		NumPos = 0;
		MaxNumPos = 0;
		memset(szPosName, 0, MAX_GUILD_POS_NAME_SIZE);
		for(INT i = 0; i<MAX_NUM_PER_POSITION; i++)
		{
			Posidx[i] = -1;
		}
	}
};

//不同级别帮会对应的不同的行政结构
struct GUILD_ADMIN_t
{
	enum ORESULT
	{
		RET_SUCCESS = 0,
		RET_NOT_EXIST,	//此人在本职位不存在
		RET_POS_FULL,	//职位人都满了
		RET_NO_USER,	//玩家不存在

		RET_UNKOWN,
	};
	enum
	{
		POS_LEVEL0_BEGIN = 0,

		POS_CHAIR_MAN,				//帮主
		POS_ASS_CHAIR_MAN ,			//副帮主
		POS_MEMBER,					//帮众

		POS_LEVEL1_BEGIN,

		POS_ELITE_MEMBER,			//精英帮众
		POS_COM,					//商业官员
		POS_AGRI,					//农业官员
		POS_INDUSTRY,				//工业官员
		POS_HR,						//人事官员

		POS_LEVEL2_BEGIN,

		POS_LEVEL_END,						
	};

	GUILD_POS_t		m_PosList[POS_LEVEL_END];

	GUILD_POS_t*	Get(INT iPos)
	{
		return &m_PosList[iPos];
	}

	ORESULT	Remove(INT iPos, INT iIndex)
	{
		GUILD_POS_t* pGuildPos = Get(iPos);

		for(INT i = 0; i<pGuildPos->MaxNumPos; i++)
		{
			if(pGuildPos->Posidx[i] == iIndex)
			{
				pGuildPos->Posidx[i] = pGuildPos->Posidx[pGuildPos->NumPos-1];
				pGuildPos->Posidx[pGuildPos->NumPos-1] = -1;
				pGuildPos->NumPos--;
				return RET_SUCCESS;
			}
		}
		return RET_NOT_EXIST;
	}


	ORESULT	Add(INT iPos, INT iIndex)
	{
		GUILD_POS_t* pGuildPos = Get(iPos);
		if(pGuildPos->NumPos == pGuildPos->MaxNumPos)
			return RET_POS_FULL;
		pGuildPos->Posidx[pGuildPos->NumPos] = iIndex;
		pGuildPos->NumPos++;
		return RET_SUCCESS;
	}

	VOID CleanUp(INT iLevelBegin = POS_LEVEL0_BEGIN)
	{
		for(INT i = iLevelBegin; i<POS_LEVEL_END; i++)
		{
			m_PosList[i].CleanUp();
		}
	}
};

#endif // __GAMESTRUCT_GUILD_H__
