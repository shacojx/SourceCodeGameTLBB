
#ifndef __GAMESTRUCT_H__
#define __GAMESTRUCT_H__

#include "Type.h"
#include "GameDefine.h"
#include "Assertx.h"

//#pragma pack(push, 1)

//用来定义在世界的浮点位置
struct WORLD_POS
{
	//enum
	//{
	//	POS_ERROR,
	//	POS_TOP,
	//	POS_TOP_LEFT,
	//	POS_TOP_RIGHT,
	//	POS_LEFT,
	//	POS_RIGHT,
	//	POS_BOTTOM,
	//	POS_BOTTOM_LEFT,
	//	POS_BOTTOM_RIGHT,
	//	POS_SAME,
	//};
	FLOAT	 m_fX ;
	FLOAT	 m_fZ ;

	WORLD_POS(VOID)					: m_fX(0.0f), m_fZ(0.0f)	{}
	WORLD_POS(FLOAT fX, FLOAT fZ)	: m_fX(fX)	, m_fZ(fZ)		{}
	VOID	CleanUp( ){
		m_fX = 0.0f ;
		m_fZ = 0.0f ;
	};
	//INT RelaPos(WORLD_POS const& rhs)
	//{
	//	if( (fabs(m_fX-rhs.m_fX)+fabs(m_fZ-rhs.m_fZ))<0.0001f ) return POS_SAME;
	//	if( fabs(m_fX-rhs.m_fX) < 0.0001f )
	//	{
	//		if( m_fZ > rhs.m_fZ ) return POS_BOTTOM;
	//		else return POS_TOP;
	//	}
	//	if( fabs(m_fZ-rhs.m_fZ)<0.0001f )
	//	{
	//		if( m_fX > rhs.m_fX ) return POS_LEFT;
	//		else return POS_RIGHT;
	//	}
	//	if( m_fX > rhs.m_fX && m_fZ > rhs.m_fZ )
	//		return POS_BOTTOM_LEFT;
	//	if( m_fX > rhs.m_fX && m_fZ < rhs.m_fZ )
	//		return POS_TOP_LEFT;
	//	if( m_fX < rhs.m_fX && m_fZ > rhs.m_fZ )
	//		return POS_BOTTOM_RIGHT;
	//	if( m_fX < rhs.m_fX && m_fZ < rhs.m_fZ )
	//		return POS_TOP_RIGHT;
	//	return POS_ERROR;
	//}
	WORLD_POS& operator=(WORLD_POS const& rhs)
	{
		m_fX = rhs.m_fX;
		m_fZ = rhs.m_fZ;
		return *this;
	}
	BOOL	operator==(WORLD_POS& Ref)
	{
		return (fabs(m_fX-Ref.m_fX)+fabs(m_fZ-Ref.m_fZ))<0.0001f;

	}
	BOOL	operator==(const WORLD_POS& Ref)
	{
		return (fabs(m_fX-Ref.m_fX)+fabs(m_fZ-Ref.m_fZ))<0.0001f;

	}
};

//用来定义在世界的网格位置
struct MAP_POS
{
	Coord_t	 m_nX ;
	Coord_t	 m_nZ ;

	MAP_POS(VOID)					: m_nX(0)	, m_nZ(0)	{}
	MAP_POS(Coord_t nX, Coord_t nZ)	: m_nX(nX)	, m_nZ(nZ)	{}
	VOID	CleanUp( ){
		m_nX = 0 ;
		m_nX = 0 ;
	};
};




//效果状态
struct _EFFECT
{
	BOOL				m_bActive ;
	INT					m_Value ;	//效果值
	INT					m_Time ;	//效果时间

	_EFFECT( )
	{
		CleanUp( ) ;
	}

	VOID				CleanUp( ){
		m_bActive = FALSE ;
		m_Value = 0 ;
		m_Time = 0 ;
	};

	BOOL				IsActive( ){ return m_bActive ; } ;
	VOID				SetActive( BOOL bActive ){ m_bActive = bActive ; } ;
};

//怪物生成器初始化数据
struct _MONSTERCREATER_INIT
{
	CHAR				m_FileName[_MAX_PATH] ;
	WORLD_POS			m_Position ;
};


#define			DEFAULT_ITEMBOX_RECYCLE_TIME	300000		//300秒，5分钟



//装备定义
struct EQUIP_LIST
{
	GUID_t	m_GUID;			//装备类型ID
	UINT	m_uParam1;		//装备属性1
	UINT	m_uParam2;		//装备属性2
};
#define EQUIP_PLAYER_FIXNUM		(8)		//玩家身上最多可佩戴的装备数

//饰品定义
struct EMBELLISH_LIST
{
	GUID_t	m_GUID;			//饰品类型ID
	UINT	m_uParam1;		//饰品属性1
	UINT	m_uParam2;		//饰品属性2
};
#define EMBELLISH_PLAYER_FIXNUM		(6)		//玩家身上最多可佩戴的饰品数


//玩家基本序列化信息
struct PLAYER_OWN
{
	GUID_t    m_nGUID;						//玩家完全唯一ID
	CHAR	  m_szName[MAX_CHARACTER_NAME];	//玩家姓名
	
	Coord_t   m_nX;						//玩家位置X
	Coord_t   m_nZ;						//玩家位置Z

	FLOAT     m_fDir;						//玩家面朝的方向（范围：0~1.0）
										//
										//       0.25 
										//    \   |   /
										// 0.5  \ | /
										//   ----   ----  0.0 (1.0f)
										//      / | \
										//    /   |   \
										//       0.75

	PLAYER_OWN( )
	{
		m_nGUID = INVALID_ID ;
		memset( m_szName, 0, MAX_CHARACTER_NAME ) ;
		m_nX = 0 ;
		m_nZ = 0 ;
		m_fDir = 0.0 ;
	};
};

struct PLAYER_S
{
	GUID_t    m_nGUID;						//玩家完全唯一ID
	CHAR	  m_szName[MAX_CHARACTER_NAME];	//玩家姓名
	
	Coord_t   m_nX;						//玩家位置X
	Coord_t   m_nZ;						//玩家位置Z

	FLOAT     m_fDir;						//玩家面朝的方向（范围：0~1.0）
										//
										//       0.25 
										//    \   |   /
										// 0.5  \ | /
										//   ----   ----  0.0 (1.0f)
										//      / | \
										//    /   |   \
										//       0.75

	PLAYER_S( )
	{
		m_nGUID = INVALID_ID ;
		memset( m_szName, 0, MAX_CHARACTER_NAME ) ;
		m_nX = 0 ;
		m_nZ = 0 ;
		m_fDir = 0.0 ;
	};
};

struct VRECT
{
	INT nStartx ;
	INT nStartz ;
	INT nEndx ;
	INT nEndz ;

	VRECT( )
	{
		nStartx = 0 ;
		nStartz = 0 ;
		nEndx = 0 ;
		nEndz = 0 ;
	};

	BOOL IsContinue( INT x, INT z )const
	{
		if ( x < nStartx || x > nEndx || z < nStartz || z > nEndz )
			return FALSE;
		else
			return TRUE;
	}
};

//一级战斗属性结构
struct _ATTR_LEVEL1
{
	INT		m_pAttr[CATTR_LEVEL1_NUMBER] ;

	_ATTR_LEVEL1( )
	{
		CleanUp( ) ;
	};
	INT	Get( INT iAttr )const{
		Assert( iAttr>=0 && iAttr<CATTR_LEVEL1_NUMBER ) ;
		return m_pAttr[iAttr] ;
	};  
	VOID Set( INT iAttr, INT iValue ){
		Assert( iAttr>=0 && iAttr<CATTR_LEVEL1_NUMBER ) ;
		m_pAttr[iAttr] = iValue ;
	} ;
	VOID CleanUp()
	{
		memset( m_pAttr, 0, sizeof(INT)*CATTR_LEVEL1_NUMBER ) ;
	} ;
};

//二级战斗属性结构
struct _ATTR_LEVEL2
{
	INT		m_pAttr[CATTR_LEVEL2_NUMBER] ;

	_ATTR_LEVEL2( )
	{
		memset( m_pAttr, 0, sizeof(INT)*CATTR_LEVEL2_NUMBER ) ;
	}
	INT	Get( INT iAttr ){
		Assert( iAttr>=0 && iAttr<CATTR_LEVEL2_NUMBER ) ;
		return m_pAttr[iAttr] ;
	};
	VOID Set( INT iAttr, INT iValue ){
		Assert( iAttr>=0 && iAttr<CATTR_LEVEL2_NUMBER ) ;
		m_pAttr[iAttr] = iValue ;
	} ;
};

//角色所拥有的称号
//#开头的字符串代表是一个字符串资源ID,必须通过表格索引，服务器不用保留这个表格
#define IDTOSTRING(str, strid, strsize)	char str[strsize];\
										memset(str, 0, strsize);\
										sprintf(str, "#%d", strid);\

#define STRINGTOID(str, strid)			INT strid = atoi((CHAR*)(str+1));\
										

struct _TITLE
{
	enum
	{//标示当前title
		NO_TITLE = 0,
		LEVEL_TITLE,
		LIFE_TITLE,
		RANK_TITLE,
		STORY_TITLE,
		KILLBOSS_TITLE,
		TREASURE_TITLE,
		DY_TITLE0,
		DY_TITLE1,
		DY_TITLE2,
		DY_TITLE3,
		DY_TITLE4,
		DY_TITLE5,
		DY_TITLE6,
		DY_TITLE7,
		DY_TITLE8,
		DY_TITLE9,
		NICK_TITLE,
		SPOUSE_TITLE,
		BANGPAI_TITLE,
		JIEBAI_TITLE,
		SHITU_TITLE,
		SHANGDIAN_TITLE,
		MOOD_TITLE,


		MAX_NUM_TITLE,
	};

	BYTE				m_LevelTitle;
	BYTE				m_LifeTitle;
	BYTE				m_RankTitle;
	BYTE				m_StoryTitle;
	BYTE				m_KillBossTitle;
	BYTE				m_TreasureTitle;
	SHORT				m_aDyTitle[MAX_DYNAMIC_TITLE_SIZE];
	CHAR				m_szNick[MAX_NICK_NAME];		//角色昵称
	CHAR				m_szSpouse[MAX_NICK_NAME];		//配偶名称
	CHAR				m_szBangPai[MAX_NICK_NAME];		//帮派名称
	CHAR				m_szJieBai[MAX_NICK_NAME];		//结拜名称
	CHAR				m_szShiTu[MAX_NICK_NAME];		//师徒称号
	CHAR				m_szShangDian[MAX_NICK_NAME];	//商店称号

	BYTE				m_CurTitle;						//当前称号类型

	VOID	CleanUp()
	{
		memset(this,0,sizeof(_TITLE));
	}
};

struct	ITEM_PICK_CTL
{
	ObjID_t				OwnerID;			//最终的拾取者ID
	uint				uBetTime;			//系统赌博时间
	UCHAR				uMaxBetPoint;		//最大Bet点数
	PICK_RULER			ePickRuler;			//系统控制符号


	ITEM_PICK_CTL()
	{
		CleanUp();
	}

	VOID	CleanUp()
	{
		OwnerID			=	INVALID_ID;		//无所有者
		ePickRuler		=	IPR_FREE_PICK;	//自由拾取
		uBetTime		=	0;				//可以拾取
		uMaxBetPoint	=	0;
	}
};

typedef	ITEM_PICK_CTL	IPC;

#define MAX_PICKER_COUNT	6


//队伍能参与拾取的人员列表
struct TEAM_PICKER
{
	UINT		m_uCount;
	ObjID_t		m_PickerID[MAX_PICKER_COUNT];

	TEAM_PICKER()
	{
		memset(this,0,sizeof(TEAM_PICKER));
	}

	VOID	AddPicker(ObjID_t id)
	{
		for(UINT nIndex=0;nIndex<m_uCount;nIndex++)
		{
			if(m_PickerID[nIndex]==id)
				return;
		}
		m_PickerID[m_uCount] = id;
		m_uCount++;

	}


};




//最大伤害纪录
#define		MAX_DAMAGE_REC_COUNT	10


//伤害纪录
struct DAMAGE_RECORD
{
	ObjID_t		m_Killer;
	TeamID_t	m_TeamID;
	UINT		m_uDamage;
	
	DAMAGE_RECORD()
	{
		CleanUp();
	}
	void	CleanUp()
	{
		m_Killer	= INVALID_ID;
		m_TeamID	= INVALID_ID;
		m_uDamage	= 0;
	}
};



//伤害队列
struct DAMAGE_MEM_LIST
{
	UINT				m_uCount;
	DAMAGE_RECORD		m_DamageRec[MAX_DAMAGE_REC_COUNT];
	
	DAMAGE_MEM_LIST()
	{
		CleanUp();
	}

	void	CleanUp()
	{
		m_uCount	=	0;
		for(int i = 0;i<MAX_DAMAGE_REC_COUNT;i++)
			m_DamageRec[i].CleanUp();
	}
	
	void	AddMember(ObjID_t	KillerID,TeamID_t KillerTeam,UINT	Damage)
	{
		if(KillerTeam!=INVALID_ID)
		{
			m_DamageRec[m_uCount].m_Killer	=	INVALID_ID;
			m_DamageRec[m_uCount].m_TeamID	=	KillerTeam;
			m_DamageRec[m_uCount].m_uDamage	=	Damage;
		}
		else
		{
			m_DamageRec[m_uCount].m_Killer	=	KillerID;
			m_DamageRec[m_uCount].m_TeamID	=	INVALID_ID;
			m_DamageRec[m_uCount].m_uDamage	=	Damage;
		}

		m_uCount++;
	}

	void	AddMember(DAMAGE_RECORD&	dRec)
	{
		if(dRec.m_TeamID!=INVALID_ID)
		{
			m_DamageRec[m_uCount].m_Killer	=	INVALID_ID;
			m_DamageRec[m_uCount].m_TeamID	=	dRec.m_TeamID;
			m_DamageRec[m_uCount].m_uDamage	=	dRec.m_uDamage;
		}
		else
		{
			m_DamageRec[m_uCount].m_Killer	=	dRec.m_Killer;
			m_DamageRec[m_uCount].m_TeamID	=	INVALID_ID;
			m_DamageRec[m_uCount].m_uDamage	=	dRec.m_uDamage;
		}
		m_uCount++;
	}
	
	DAMAGE_RECORD*	FindMember(ObjID_t KillerID,TeamID_t KillerTeam)
	{
		for(UINT i =0;i<m_uCount;i++)
		{
			if((m_DamageRec[i].m_Killer == KillerID && KillerID!=INVALID_ID)||
				(m_DamageRec[i].m_TeamID == KillerTeam && KillerTeam!=INVALID_ID))
			{
					return &m_DamageRec[i];
					
			}
		}
		return NULL;
	}
	
	


};


struct _OWN_ABILITY 
{
	// AbilityID_t	m_Ability_ID; 不需要 ID，索引就是 ID
	WORD		m_Level; // 技能等级
	WORD		m_Exp; // 技能熟练度
};


#define		MAX_MONSTER_DROP_TASK_ITEM		5
#define		MAX_MONSTER_KILLER_NUM			18

struct CHAR_OWNER_DROP_LIST 
{
	ObjID_t		HumanID;
	UINT		DropItemIndex[MAX_MONSTER_DROP_TASK_ITEM];
	UINT		DropCount;

	CHAR_OWNER_DROP_LIST()
	{
		CleanUp();
	}

	VOID	CleanUp()
	{
		memset(this,0,sizeof(*this));
	}

	VOID	AddItem(UINT ItemIndex)
	{
		Assert(DropCount<MAX_MONSTER_DROP_TASK_ITEM);
		DropItemIndex[DropCount] = ItemIndex;
		DropCount++;
	}
};

struct MONSTER_OWNER_LIST 
{
	CHAR_OWNER_DROP_LIST	OwnerDropList[MAX_MONSTER_KILLER_NUM];
	UINT					OwnerCount;

	MONSTER_OWNER_LIST()
	{
		CleanUp();
	}

	VOID	CleanUp()
	{
		memset(this,0,sizeof(*this));
	}

	VOID	AddItem(UINT uItemIndex,ObjID_t	HumanID)
	{
		for(UINT i =0;i<OwnerCount;i++)
		{
			if(OwnerDropList[i].HumanID	== HumanID)
			{
				OwnerDropList[i].AddItem(uItemIndex);
			}
		}
	}

	VOID	AddOwner(ObjID_t	HumanID)
	{
		OwnerDropList[OwnerCount].HumanID	=	HumanID;
		OwnerCount++;
	}
};



struct RELATION_MEMBER
{
	GUID_t			m_MemberGUID ;
	CHAR			m_szMemberName[MAX_CHARACTER_NAME] ;
	INT				m_nLevel;							//角色等级
	INT				m_nMenPai;							//门派 MENPAI_ATTRIBUTE
	INT				m_nPortrait;						// 头像
	GuildID_t		m_GuildID;							//帮会ID
	
	struct ReMember_ExtData
	{
		INT			m_nLevel;							//角色等级
		INT			m_nMenPai;							//门派 MENPAI_ATTRIBUTE
		INT			m_nPortrait;						//头像
		GuildID_t	m_GuildID;							//帮会ID
	};

	RELATION_MEMBER( )
	{
		CleanUp( );
	};

	VOID CleanUp( )
	{
		m_MemberGUID = INVALID_ID;
		memset( m_szMemberName, 0, sizeof(m_szMemberName) );
		m_nLevel = 0;
		m_nMenPai = 9;
		m_nPortrait = -1;
		m_GuildID = INVALID_ID;
	};

	ReMember_ExtData	GetExtData()
	{
		ReMember_ExtData ExtData;
		
		ExtData.m_nLevel = m_nLevel;
		ExtData.m_nMenPai = m_nMenPai;
		ExtData.m_nPortrait = m_nPortrait;
		ExtData.m_GuildID = m_GuildID;
		return ExtData;
	}

	VOID			 SetExtData(ReMember_ExtData& ExtData)
	{
		m_nLevel = ExtData.m_nLevel;
		m_nMenPai = ExtData.m_nMenPai;
		m_nPortrait = ExtData.m_nPortrait;
		m_GuildID = ExtData.m_GuildID;
	}
};

struct MarriageInfo
{
	GUID_t		m_SpouseGUID;								// 配偶的 GUID
//	UINT		m_uWeddingTime;								// 婚礼时间

	MarriageInfo()
	{
		CleanUp();
	}

	VOID CleanUp()
	{
		m_SpouseGUID = INVALID_ID;
	}
};

struct PrenticeInfo
{
//	UINT		m_uRecruitingTime;							// 收徒时间
	time_t		m_BetrayingTime;							// 最后一次叛师时间
	UINT		m_uMoralPoint;								// 师德点
	UCHAR		m_uPrenticeCount;							// 徒弟数量
	GUID_t		m_PrenticeGUID[MAX_PRENTICE_COUNT];			// 徒弟的 GUID

	PrenticeInfo()
	{
		CleanUp();
	}

	VOID CleanUp()
	{
		m_BetrayingTime = 0;
		m_uMoralPoint = 0;
		m_uPrenticeCount = 0;

		for( INT i=0; i<MAX_PRENTICE_COUNT; ++i )
		{
			m_PrenticeGUID[i] = INVALID_ID;
		}
	}

};

struct MasterInfo
{
	GUID_t		m_MasterGUID;								// 师傅的 GUID
//	UINT		m_uApprenticingTime;						// 拜师时间
//	UINT		m_uBetrayingTime;							// 上次叛师时间
//	UINT		m_uBetrayTimes;								// 叛师次数

	MasterInfo()
	{
		CleanUp();
	}

	VOID CleanUp()
	{
		m_MasterGUID = INVALID_ID;
	}

};

class SocketOutputStream ;
class SocketInputStream ;
//邮件
struct MAIL
{



	struct MailInfo
	{
		GUID_t		m_GUID;								// 发信人 GUID
		BYTE		m_SourSize ;
		INT			m_nPortrait;						// 发信人头像
		BYTE		m_DestSize ;
		WORD		m_ContexSize ;
		UINT		m_uFlag ;							//邮件标志 enum MAIL_TYPE
		time_t		m_uCreateTime ;						//邮件创建时间
		CampID_t	m_Camp;
		//执行邮件应用参数
		UINT		m_uParam0 ;
		UINT		m_uParam1 ;
		UINT		m_uParam2 ;
		UINT		m_uParam3 ;
	};

	VOID	GetMailInfo(MailInfo& mInfo)
	{
		mInfo.m_GUID		= m_GUID;
		mInfo.m_SourSize	= m_SourSize;
		mInfo.m_nPortrait	= m_nPortrait;
		mInfo.m_DestSize	= m_DestSize;
		mInfo.m_ContexSize	= m_ContexSize;
		mInfo.m_uFlag		= m_uFlag;
		mInfo.m_uCreateTime	= m_uCreateTime;
		mInfo.m_uParam0		= m_uParam0;
		mInfo.m_uParam1		= m_uParam1;
		mInfo.m_uParam2		= m_uParam2;
		mInfo.m_uParam3		= m_uParam3;
		mInfo.m_Camp		= m_Camp;
	}

	VOID	SetMailInfo(MailInfo& mInfo)
	{
		m_GUID			= 	mInfo.m_GUID;
		m_SourSize		= 	mInfo.m_SourSize;
		m_nPortrait		= 	mInfo.m_nPortrait;
		m_DestSize		= 	mInfo.m_DestSize;
		m_ContexSize	= 	mInfo.m_ContexSize;
		m_uFlag			= 	mInfo.m_uFlag;
		m_uCreateTime	= 	mInfo.m_uCreateTime;
		m_uParam0		= 	mInfo.m_uParam0;
		m_uParam1		= 	mInfo.m_uParam1;
		m_uParam2		= 	mInfo.m_uParam2;
		m_uParam3		= 	mInfo.m_uParam3;
		m_Camp			=	mInfo.m_Camp;
	}
	GUID_t		m_GUID;								// 发信人 GUID
	BYTE		m_SourSize ;
	CHAR		m_szSourName[MAX_CHARACTER_NAME] ;	//发信人

	INT			m_nPortrait;						// 发信人头像

	BYTE		m_DestSize ;
	CHAR		m_szDestName[MAX_CHARACTER_NAME] ;	//收信人

	WORD		m_ContexSize ;
	CHAR		m_szContex[MAX_MAIL_CONTEX] ;		//内容

	UINT		m_uFlag ;							//邮件标志 enum MAIL_TYPE
	time_t		m_uCreateTime ;						//邮件创建时间

	CampID_t	m_Camp;								//发信人阵营

	//执行邮件应用参数
	UINT		m_uParam0 ;
	UINT		m_uParam1 ;
	UINT		m_uParam2 ;
	UINT		m_uParam3 ;

	MAIL( )
	{
		CleanUp( ) ;
	};
	VOID CleanUp( )
	{
		m_GUID = INVALID_INDEX;
		m_SourSize = 0 ;
		memset( m_szSourName, 0, sizeof(CHAR)*MAX_CHARACTER_NAME ) ;

		m_nPortrait = -1;

		m_DestSize = 0 ;
		memset( m_szDestName, 0, sizeof(CHAR)*MAX_CHARACTER_NAME ) ;

		m_ContexSize = 0 ;
		memset( m_szContex, 0, sizeof(CHAR)*MAX_MAIL_CONTEX ) ;

		m_uFlag = MAIL_TYPE_NORMAL ;
		m_uCreateTime = 0 ;

		m_uParam0 = 0 ;
		m_uParam1 = 0 ;
		m_uParam2 = 0 ;
		m_uParam3 = 0 ;
	};
	VOID Read( SocketInputStream& iStream ) ;
	VOID Write( SocketOutputStream& oStream ) const ;
};


#define MAX_MAIL_SIZE 20
struct MAIL_LIST
{
	MAIL		m_aMail[MAX_MAIL_SIZE] ;
	BYTE		m_Count ;//邮件数量
	BYTE		m_TotalLeft ;//用户帐号里的邮件剩余数量

	MAIL_LIST( )
	{
		CleanUp( ) ;
	};
	VOID CleanUp( )
	{
		m_Count = 0 ;
		m_TotalLeft = 0 ;
		for( INT i=0;i<MAX_MAIL_SIZE; i++ )
		{
			m_aMail[i].CleanUp() ;
		}
	};
	VOID Read( SocketInputStream& iStream ) ;
	VOID Write( SocketOutputStream& oStream ) const ;
};

// 批量邮件，指发送给不同人的同内容邮件
#define MAX_RECEIVER 100
struct BATCH_MAIL
{
	GUID_t				m_GUID;								// GUID

	BYTE				m_SourSize;
	CHAR				m_szSourName[MAX_CHARACTER_NAME];	//发信人

	BYTE				m_ReceiverCount;					//收信人数量
	struct
	{
		BYTE			m_DestSize;
		CHAR			m_szDestName[MAX_CHARACTER_NAME];	//收信人
	}m_Receivers[MAX_RECEIVER];

	WORD				m_ContentSize;
	CHAR				m_szContent[MAX_MAIL_CONTEX];		//内容

	UCHAR				m_uFlag;							//邮件标志 enum MAIL_TYPE
	time_t				m_uCreateTime;						//邮件创建时间

	CampID_t			m_Camp;								//发信者的阵营

	BATCH_MAIL()		{ CleanUp(); }

	GUID_t				GetGUID( )
	{
		return m_GUID;
	}
	VOID				SetGUID( GUID_t guid )
	{
		m_GUID = guid;
	}

	const CHAR*			GetSourName()
	{
		return m_szSourName;
	}
	VOID				SetSourName( const CHAR* szName )
	{
		strncpy(m_szSourName, szName, MAX_CHARACTER_NAME - 1);
		m_SourSize = (UCHAR)strlen(m_szSourName);
	}

	BYTE				GetReceiverCount()
	{
		return m_ReceiverCount;
	}
	const CHAR*			GetDestName(BYTE idx)
	{
		if( idx >= m_ReceiverCount )
		{
			Assert( idx );
			return NULL;
		}

		return m_Receivers[idx].m_szDestName;
	}
	VOID				AddDestName( const CHAR* szName )
	{
		strncpy(m_Receivers[m_ReceiverCount].m_szDestName, szName, MAX_CHARACTER_NAME - 1);
		m_Receivers[m_ReceiverCount].m_DestSize = (UCHAR)strlen(m_Receivers[m_ReceiverCount].m_szDestName);
		++m_ReceiverCount;
	}

	const CHAR*			GetMailContent()
	{
		return m_szContent;
	}
	VOID				SetMailContent( const CHAR* szContent )
	{
		strncpy(m_szContent, szContent, MAX_MAIL_CONTEX - 1);
		m_ContentSize = (UCHAR)strlen(m_szContent);
	}

	UCHAR				GetMailFlag()				
	{
		return m_uFlag;
	}
	VOID				SetMailFlag(UCHAR uFlag)
	{
		m_uFlag = uFlag;
	}

	time_t				GetCreateTime()
	{
		return m_uCreateTime;
	}
	VOID				SetCreateTime(time_t uCreateTime)
	{
		m_uCreateTime = uCreateTime;
	}

	CampID_t			GetSourCamp()
	{
		return m_Camp;
	}
	VOID				SetSourCamp(CampID_t id)
	{
		m_Camp = id;
	}

	VOID				CleanUp();
	UINT				GetSize() const;
	VOID				Read( SocketInputStream& iStream );
	VOID				Write( SocketOutputStream& oStream ) const;
};


struct USER_SIMPLE_DATA
{
	CHAR				m_Account[MAX_CHARACTER_NAME];	// 此用户的账户名字
	GUID_t				m_AccountGUID;					// 官网GUID
	CHAR				m_Name[MAX_CHARACTER_NAME];		// 此用户的角色名字
	GUID_t				m_GUID;							// 此用户的唯一号
	UINT				m_uMenPai;						// 门派
	INT					m_nPortrait;					// 头像
	UCHAR				m_uFaceMeshID;					// 脸部模型
	UCHAR				m_uHairMeshID;					// 头发模型
	UINT				m_uHairColor;					// 发色
	INT					m_nLevel;						// 级别
	USHORT				m_uSex;							// 性别
	CHAR				m_szTitle[MAX_CHARACTER_TITLE];	// 称号
	GuildID_t			m_GuildID;						// 帮会 ID

	USER_SIMPLE_DATA( )
	{
		CleanUp( ) ;
	}
	VOID CleanUp( )
	{
		memset( m_Name, 0, sizeof(m_Name) );
		memset( m_Account, 0, sizeof(m_Account) );
		m_GUID = INVALID_ID;
		m_AccountGUID = INVALID_ID;
		m_uMenPai = 9;
		m_nPortrait = -1;
		m_nLevel = 0;
		m_uSex = 0;
		memset( m_szTitle, 0, sizeof(m_szTitle) );
		m_GuildID = INVALID_ID;
	}
};

#define		MAX_SQL_LENGTH				4096
#define		MAX_LONG_SQL_LENGTH			204800

struct DB_QUERY
{
	UCHAR	m_SqlStr[MAX_SQL_LENGTH];		//执行的Sql语句
	
	VOID	Clear()
	{
		memset(m_SqlStr,0,MAX_SQL_LENGTH);
	}

	VOID	Parse(const CHAR* pTemplate,...);
	
};

struct LONG_DB_QUERY
{
	UCHAR	m_SqlStr[MAX_LONG_SQL_LENGTH];		//执行的Sql语句

	VOID	Clear()
	{
		memset(m_SqlStr,0,MAX_LONG_SQL_LENGTH);
	}

	VOID	Parse(const CHAR* pTemplate,...);
};

struct DB_CHAR_EQUIP_LIST
{
	DB_CHAR_EQUIP_LIST()
	{
		CleanUp();
	}

	VOID	CleanUp()
	{
		memset(this,0,sizeof(*this));
	}
	UINT		m_Equip[HEQUIP_NUMBER];			//装备
};

struct DB_CHAR_BASE_INFO
{

	DB_CHAR_BASE_INFO()
	{
		CleanUp();
	}

	VOID	CleanUp()
	{
		memset(this,0,sizeof(*this));
	}

	GUID_t				m_GUID;							//角色全局编号
	BOOL				m_Sex;							//性别
	CHAR				m_Name[MAX_CHARACTER_NAME];		//角色名字
	INT					m_Level;						//角色等级
	UINT				m_HairColor;					//头发颜色	
	BYTE				m_FaceColor;					//脸形颜色
	BYTE				m_HairModel;					//头发模型
	BYTE				m_FaceModel;					//脸形模型
	SceneID_t			m_StartScene;					//角色所在场景
	INT					m_Menpai;						//角色门派
	INT					m_HeadID;						//头部编号
	CampID_t			m_Camp;							//阵营编号
	DB_CHAR_EQUIP_LIST	m_EquipList;					//装备列表
};

// 队员列表中的队员信息项
struct TEAM_LIST_ENTRY
{
	GUID_t				m_GUID;							// GUID
	SceneID_t			m_SceneID;						// 场景ID
	UINT				m_ExtraID;						// 队员的 PlayerID(WG) 或 ObjID(GC)
	UCHAR				m_NameSize;						// 姓名长度
	CHAR				m_Name[MAX_CHARACTER_NAME];		// 队员的名字
	INT					m_nPortrait;					// 头像
	USHORT				m_uDataID;						// 队员的性别

	TEAM_LIST_ENTRY( )
	{
		CleanUp( );
	};

	VOID				CleanUp( )
	{
		m_GUID			= INVALID_ID;
		m_SceneID		= INVALID_ID;
		m_ExtraID		= INVALID_ID;
		m_NameSize		= 0;
		memset			(m_Name, 0, sizeof(m_Name));
		m_nPortrait		= -1;
		m_uDataID		= 0;
	};

	TEAM_LIST_ENTRY&	operator= ( const TEAM_LIST_ENTRY& entry )
	{
		m_GUID			= entry.m_GUID;
		m_SceneID		= entry.m_SceneID;
		m_ExtraID		= entry.m_ExtraID;
		m_NameSize		= entry.m_NameSize;
		strncpy			( m_Name, entry.m_Name, sizeof(m_Name) - 1 );
		m_nPortrait		= entry.m_nPortrait;
		m_uDataID		= entry.m_uDataID;
		return			*this;
	}

	VOID				SetGUID( GUID_t guid ) { m_GUID = guid; }
	GUID_t				GetGUID( ) const { return m_GUID; }

	VOID				SetSceneID( SceneID_t SceneID ) { m_SceneID = SceneID; }
	SceneID_t			GetSceneID( ) const { return m_SceneID; }

	VOID				SetExtraID( UINT id ) { m_ExtraID = id; }
	UINT				GetExtraID( ) const { return m_ExtraID; }

	VOID				SetName( const CHAR* pName )
	{
		strncpy			( m_Name, pName, MAX_CHARACTER_NAME-1 );
		m_NameSize		= (UCHAR)strlen(m_Name);
	}
	const CHAR*			GetName( ) const { return m_Name; }

	VOID				SetIcon( INT icon ) { m_nPortrait = icon; }
	INT					GetIcon( ) const { return m_nPortrait; }

	VOID				SetDataID(USHORT dataid) { m_uDataID = dataid; }
	USHORT				GetDataID() const { return m_uDataID; }

	UINT				GetSize() const;
	VOID				Read( SocketInputStream& iStream );
	VOID				Write( SocketOutputStream& oStream ) const;
};

//ID List
typedef struct _ObjID_List
{
	enum
	{
		MAX_LIST_SIZE = 512,
	};
	_ObjID_List()
	{
		CleanUp();
	}
	VOID CleanUp(VOID)
	{
		m_nCount=0;
		memset((VOID*)m_aIDs, INVALID_ID, sizeof(m_aIDs));
	}
	INT m_nCount;
	ObjID_t m_aIDs[MAX_LIST_SIZE];
} ObjID_List;

//玩家商店的唯一ID
struct _PLAYERSHOP_GUID
{
	ID_t	m_World ;		//世界号: 
	ID_t	m_Server ;		//服务端程序号：
	ID_t	m_Scene ;		//场景号
	INT		m_PoolPos ;		//数据池位置

	_PLAYERSHOP_GUID()
	{
		Reset();
	}
	_PLAYERSHOP_GUID& operator=(_PLAYERSHOP_GUID const& rhs)
	{
		m_PoolPos	= rhs.m_PoolPos;
		m_Server	= rhs.m_Server;
		m_World		= rhs.m_World;
		m_Scene		= rhs.m_Scene;
		return *this;
	}
	BOOL	operator ==(_PLAYERSHOP_GUID& Ref)	const
	{
		return (Ref.m_Scene==m_Scene)&&(Ref.m_PoolPos==m_PoolPos)&&(Ref.m_Server==m_Server)&&(Ref.m_World==m_World);
	}
	BOOL	isNull() const
	{
		return (m_Scene ==INVALID_ID)&&(m_World ==INVALID_ID)&&(m_PoolPos==-1)&&(m_Server == INVALID_ID);
	}
	VOID	Reset()
	{
		m_PoolPos = -1;
		m_Server = INVALID_ID;
		m_World	 = INVALID_ID;	
		m_Scene	 = INVALID_ID;
	}
};

enum  SM_COMMANDS
{
	CMD_UNKNOW,
	CMD_SAVE_ALL,
	CMD_CLEAR_ALL,

};


struct SM_COMMANDS_STATE
{
	SM_COMMANDS	cmdType;	
	union 
	{
		INT		iParam[6];
		FLOAT	fParam[6];
		CHAR	cParam[24];
	};
};


struct GLOBAL_CONFIG
{
	GLOBAL_CONFIG()
	{
		Commands.cmdType = CMD_UNKNOW;
	}
	SM_COMMANDS_STATE		Commands;
};

//后面的文件会用到前面的定义
#include "GameStruct_Item.h"
#include "GameStruct_Skill.h"
#include "GameStruct_Scene.h"

#include "GameStruct_Relation.h"
#include "GameStruct_Guild.h"
#include "GameStruct_City.h"
#include "GameStruct_Script.h"

#include "GameStruct_MinorPasswd.h"
#include "GameStruct_Finger.h"

#endif
