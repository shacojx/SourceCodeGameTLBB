// ScriptCommandStruct.h
//
/////////////////////////////////////////////////////////

#ifndef __SCRIPTCOMMANDSTRUCT_H__
#define __SCRIPTCOMMANDSTRUCT_H__

#include "Type.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"

// script command
enum ENUM_SCRIPT_COMMAND
{
	SCRIPT_COMMAND_INVALID	= -1,
	SCRIPT_COMMAND_EVENT_LIST_RESPONSE,			// 事件列表返回
	SCRIPT_COMMAND_MISSION_RESPONSE,			// 任务事件的查询返回
	SCRIPT_COMMAND_MISSION_REGIE,				// 漕运任务查询返回
	SCRIPT_COMMAND_MISSION_DEMAND_RESPONSE,		// 任务需求的查询返回
	SCRIPT_COMMAND_MISSION_CONTINUE_RESPONSE,	// 任务的继续按钮事件返回
	SCRIPT_COMMAND_MISSION_TIPS,				// 任务提示
	SCRIPT_COMMAND_TRADE,						// 交易
	SCRIPT_COMMAND_SKILL_STUDY,					// 技能学习
};

#define	DEF_SCRIPT_STRING_LEN	(256)
struct ScriptString
{
	SHORT	m_Size ;
	CHAR	m_szString[DEF_SCRIPT_STRING_LEN];

	VOID Reset( VOID ){
		m_Size = 0 ;
		m_szString[0] = 0;
	}

	ScriptString &operator = ( UINT uValue ){
		return *this;
	}

	ScriptString &operator = ( const CHAR *pszString ){
		if ( pszString != NULL )
		{
			m_Size = (SHORT)strlen( pszString );
			if ( m_Size > DEF_SCRIPT_STRING_LEN - 1 )
				m_Size = DEF_SCRIPT_STRING_LEN - 1;

			if ( m_Size > 0 )
				memcpy( m_szString, pszString, m_Size );

			m_szString[m_Size] = 0;
		}
		else
		{
			m_Size = 0 ;
			m_szString[0] = 0;
		}
		return *this;
	}

	UINT GetBufSize( VOID )const;

	BOOL Read( SocketInputStream& iStream );
	BOOL Write( SocketOutputStream& oStream )const;
};

/////////////////////////////////////////////////////////////////
// 事件列表 
enum ENUM_EVENT_ITEM_TYPE
{
	EVENT_ITEM_TYPE_INVALID	= -1,	// 无效
	EVENT_ITEM_TYPE_SECTION,		// 节点
	EVENT_ITEM_TYPE_SCRIPT_ID,		// 选项
	EVENT_ITEM_TYPE_TEXT,			// 文本
};

struct ScriptEventItem
{
	INT					m_nType;	// ENUM_EVENT_ITEM_TYPE
	INT					m_state;
	INT					m_index;
	ScriptString		m_strString;
	ScriptID_t			m_idScript;

	VOID Reset( VOID ){
		m_nType			= EVENT_ITEM_TYPE_INVALID;
		m_strString.Reset();
	}

	UINT GetBufSize( VOID )const;

	VOID SetSection( const ScriptString *pstrSection ){
		m_nType	= EVENT_ITEM_TYPE_SECTION;
		m_strString = *pstrSection;
	}

	VOID SetScriptID( INT index,INT state,ScriptID_t idScript, const ScriptString *pstrSection ){
		m_nType	= EVENT_ITEM_TYPE_SCRIPT_ID;
		m_state = state;
		m_index = index;
		m_idScript = idScript;
		m_strString = *pstrSection;
	}

	VOID SetText( const ScriptString *pstrSection ){
		m_nType	= EVENT_ITEM_TYPE_TEXT;
		m_strString = *pstrSection;
	}

	BOOL Read( SocketInputStream& iStream );
	BOOL Write( SocketOutputStream& oStream )const;
};

#define MAX_EVENT_LIST_ITEM_COUNT	(16)
struct ScriptParam_EventList
{
	ObjID_t				m_idNPC;
	BYTE				m_yItemCount;
	ScriptEventItem		m_seiItem[MAX_EVENT_LIST_ITEM_COUNT];

	VOID Reset( VOID ){
		m_idNPC			= INVALID_ID;
		m_yItemCount	= 0;
		UINT i;
		for ( i = 0; i < MAX_EVENT_LIST_ITEM_COUNT; i++ )
		{
			m_seiItem[i].Reset();
		}
	}

	UINT GetBufSize( VOID )const;

	VOID AddItem( const ScriptEventItem *pItem ){
		if ( m_yItemCount < MAX_EVENT_LIST_ITEM_COUNT )
		{
			m_seiItem[m_yItemCount] = *pItem;
			m_yItemCount++;
		}
	}

	const ScriptEventItem *GetItem( BYTE yIndex )const{
		return (yIndex < m_yItemCount )?(&m_seiItem[yIndex]):(NULL);
	}

	BOOL Read( SocketInputStream& iStream );
	BOOL Write( SocketOutputStream& oStream )const;
};

/////////////////////////////////////////////////////////////////
// 任务信息
struct SMissionBonusItem
{
	BYTE		m_yCount;
	UINT		m_uItemID;

	VOID Reset( VOID ){
		m_yCount	= 0;
		m_uItemID	= UINT_MAX;
	}

	UINT GetBufSize( VOID )const;

	BOOL Read( SocketInputStream& iStream );
	BOOL Write( SocketOutputStream& oStream )const;
};
//任务需要杀死的NPC
struct SMissionDemandKill
{
	BYTE		m_yCount;
	UINT		m_uNPCID;

	VOID Reset( VOID ){
		m_yCount	= 0;
		m_uNPCID	= UINT_MAX;
	}

	UINT GetBufSize( VOID )const;

	BOOL Read( SocketInputStream& iStream );
	BOOL Write( SocketOutputStream& oStream )const;
};
enum ENUM_MISSION_BONUS_TYPE
{
	MISSION_BONUS_TYPE_INVALID	= -1,
	MISSION_BONUS_TYPE_MONEY,			// 金钱
	MISSION_BONUS_TYPE_ITEM,			// 物品
	MISSION_BONUS_TYPE_ITEM_RAND,		// 随机物品
	MISSION_BONUS_TYPE_ITEM_RADIO,		// 多选1物品
	MISSION_BONUS_TYPE_EXP,				// 奖励经验
};

// 奖励的结构
struct SMissionBonus
{
	INT							m_nType;		// ENUM_MISSION_BONUS_TYPE
	union{
		UINT					m_uMoney;
		UINT					m_uExp;
		SMissionBonusItem		m_ItemBonus;
	};

	VOID Reset( VOID ){
		m_nType		= MISSION_BONUS_TYPE_INVALID;
		m_ItemBonus.Reset();
	}

	VOID SetMoney( UINT uMoney ){
		m_nType		= MISSION_BONUS_TYPE_MONEY;
		m_uMoney	=uMoney;
	}

	VOID SetItem( UINT uItemID, BYTE yItemCount ){
		m_nType		= MISSION_BONUS_TYPE_ITEM;
		m_ItemBonus.m_yCount	= yItemCount;
		m_ItemBonus.m_uItemID	=uItemID;
	}

	VOID SetItemRand( VOID ){
		m_nType		= MISSION_BONUS_TYPE_ITEM_RAND;
	}

	VOID SetItemRadio( UINT uItemID, BYTE yItemCount ){
		m_nType		= MISSION_BONUS_TYPE_ITEM_RADIO;
		m_ItemBonus.m_yCount	= yItemCount;
		m_ItemBonus.m_uItemID	=uItemID;
	}

	UINT GetBufSize( VOID )const;

	BOOL Read( SocketInputStream& iStream );
	BOOL Write( SocketOutputStream& oStream )const;
};

#define MAX_MISSION_TEXT_COUNT		(8)
#define MAX_MISSION_BONUS_COUNT		(16)
struct ScriptParam_MissionInfo
{
	ObjID_t			m_idNPC;			// 向服务器返回时的参数
	ScriptID_t		m_idScript;			// 用于向服务器返回操作，如：接受任务（传到服务器的不是任务ID，而是脚本ID）
	MissionID_t		m_idMission;		// 用于客户端显示信息的查询
	BYTE			m_yTextCount;
	ScriptString	m_aText[MAX_MISSION_TEXT_COUNT];
	BYTE			m_yBonusCount;
	SMissionBonus	m_aBonus[MAX_MISSION_BONUS_COUNT];
	BOOL			m_IsPush;

	VOID Reset( VOID ){
		m_idNPC			= INVALID_ID;
		m_idScript		= INVALID_ID;
		m_idMission		= INVALID_ID;
		m_yTextCount	= 0;
		m_IsPush		= 0;
		UINT i;
		for ( i = 0; i < MAX_MISSION_TEXT_COUNT; i++ )
		{
			m_aText[i].Reset();
		}
		m_yBonusCount	= 0;
		for ( i = 0; i < MAX_MISSION_BONUS_COUNT; i++ )
		{
			m_aBonus[i].Reset();
		}
	}

	VOID AddText( const ScriptString *pstrText ){
		if ( m_yTextCount < MAX_MISSION_TEXT_COUNT )
		{
			m_aText[m_yTextCount] = *pstrText;
			m_yTextCount++;
		}
	}

	VOID AddBonus( const SMissionBonus *pBonus ){
		if ( m_yBonusCount < MAX_MISSION_BONUS_COUNT )
		{
			m_aBonus[m_yBonusCount] = *pBonus;
			m_yBonusCount++;
		}
	}

	UINT GetBufSize( VOID )const;

	BOOL Read( SocketInputStream& iStream );
	BOOL Write( SocketOutputStream& oStream )const;
};

/////////////////////////////////////////////////////////////////
// 漕运任务查询返回
typedef ScriptParam_EventList	ScriptParam_MissionRegie;

/////////////////////////////////////////////////////////////////
// 任务需求信息

// 需求结构定义
typedef SMissionBonusItem SMissionDemandItem;

#define MAX_MISSION_DEMAND_COUNT		(8)
struct ScriptParam_MissionDemandInfo
{
	enum
	{
		MISSION_NODONE	=0,
		MISSION_DONE,
		MISSION_CHECK,
	};
	ObjID_t				m_idNPC;			// 向服务器返回时的参数
	ScriptID_t			m_idScript;			// 向服务器返回时的参数
	MissionID_t			m_idMission;		// 用于客户端显示信息的查询
	INT					m_bDone;			// 完成标志 0:未完成，1:完成，2:需要二次判定
	BYTE				m_yTextCount;
	ScriptString		m_aText[MAX_MISSION_TEXT_COUNT];
	BYTE				m_yDemandCount;
	SMissionDemandItem	m_aDemandItem[MAX_MISSION_DEMAND_COUNT];

	VOID Reset( VOID ){
		m_idNPC			= INVALID_ID;
		m_idScript		= INVALID_ID;
		m_idMission		= INVALID_ID;
		m_bDone			= FALSE;
		m_yDemandCount	= 0;
		m_yTextCount	= 0;
		UINT i;
		for ( i = 0; i < MAX_MISSION_TEXT_COUNT; i++ )
		{
			m_aText[i].Reset();
		}
		for ( i = 0; i < MAX_MISSION_DEMAND_COUNT; i++ )
		{
			m_aDemandItem[i].Reset();
		}
	}

	VOID AddText( const ScriptString *pstrText ){
		if ( m_yTextCount < MAX_MISSION_TEXT_COUNT )
		{
			m_aText[m_yTextCount] = *pstrText;
			m_yTextCount++;
		}
	}

	VOID AddDemandItem( const SMissionDemandItem *pItem ){
		if ( m_yDemandCount < MAX_MISSION_DEMAND_COUNT )
		{
			m_aDemandItem[m_yDemandCount] = *pItem;
			m_yDemandCount++;
		}
	}

	UINT GetBufSize( VOID )const;

	BOOL Read( SocketInputStream& iStream );
	BOOL Write( SocketOutputStream& oStream )const;
};

/////////////////////////////////////////////////////////////////
// 任务的继续按钮事件返回(返回任务的奖励等等)
struct ScriptParam_MissionContinueInfo
{
	ObjID_t				m_idNPC;			// 向服务器返回时的参数
	ScriptID_t			m_idScript;			// 向服务器返回时的参数
	MissionID_t			m_idMission;		// 用于客户端显示信息的查询
	BYTE				m_yTextCount;						// 说明性文本的数目
	ScriptString		m_aText[MAX_MISSION_TEXT_COUNT];	// 说明性文本
	BYTE				m_yBonusCount;						// 任务道具奖励的数目
	SMissionBonus		m_aBonus[MAX_MISSION_BONUS_COUNT];	// 任务道具列表

	VOID Reset( VOID ){
		m_idNPC			= INVALID_ID;
		m_idScript		= INVALID_ID;
		m_idMission		= INVALID_ID;
		m_yBonusCount	= 0;
		m_yTextCount	= 0;
		UINT i;
		for ( i = 0; i < MAX_MISSION_TEXT_COUNT; i++ )
		{
			m_aText[i].Reset();
		}
		for ( i = 0; i < MAX_MISSION_BONUS_COUNT; i++ )
		{
			m_aBonus[i].Reset();
		}
	}

	VOID AddText( const ScriptString *pstrText ){
		if ( m_yTextCount < MAX_MISSION_TEXT_COUNT )
		{
			m_aText[m_yTextCount] = *pstrText;
			m_yTextCount++;
		}
	}

	VOID AddBonus( const SMissionBonus *pBonus ){
		if ( m_yBonusCount < MAX_MISSION_DEMAND_COUNT )
		{
			m_aBonus[m_yBonusCount] = *pBonus;
			m_yBonusCount++;
		}
	}

	UINT GetSelectItemID(INT nIndex)
	{
		INT i;
		for(i=0;i<m_yBonusCount;i++)
		{
			if( MISSION_BONUS_TYPE_ITEM_RADIO == m_aBonus[i].m_nType )
				break;
		}
		return m_aBonus[i + nIndex].m_ItemBonus.m_uItemID;
	}

	UINT GetBufSize( VOID )const;

	BOOL Read( SocketInputStream& iStream );
	BOOL Write( SocketOutputStream& oStream )const;
};

/////////////////////////////////////////////////////////////////
// 任务提示
struct ScriptParam_MissionTips
{
	ScriptString		m_strText;

	VOID Reset( VOID ){
		m_strText.Reset();
	}

	UINT GetBufSize( VOID )const;

	BOOL Read( SocketInputStream& iStream );
	BOOL Write( SocketOutputStream& oStream )const;
};

/////////////////////////////////////////////////////////////////
// 交易
#define MAX_TRADE_ITEM_COUNT	(128)
struct STradeItem
{
	UINT		m_uDataID;				// Excel中的Index
	_ITEM_TYPE	m_typeItem;				// 道具类型
	BYTE		m_yCount;				// 道具数量

	VOID Reset( VOID ){
		m_uDataID	= UINT_MAX;
		m_typeItem.CleanUp();
		m_yCount	= 0;
	}

	UINT GetBufSize( VOID )const;

	BOOL Read( SocketInputStream& iStream );
	BOOL Write( SocketOutputStream& oStream )const;
};

// 交易消息所用到的结构
struct ScriptParam_Trade
{
	BYTE			m_yItemCount;							// 道具数目
	STradeItem		m_aTradeItem[MAX_TRADE_ITEM_COUNT];		// 道具列表
	BOOL			m_bRepair;								// 是否有修理功能

	VOID Reset( VOID ){
		m_yItemCount	= 0;
		UINT i;
		for( i = 0; i < MAX_TRADE_ITEM_COUNT; i++ )
		{
			m_aTradeItem[i].Reset( );
		}
	}

	UINT GetBufSize( VOID )const;

	BOOL Read( SocketInputStream& iStream );
	BOOL Write( SocketOutputStream& oStream )const;
};

//技能学习消息的结构体
#define MAX_SKILL_ITEM_COUNT (128)
struct SSkillItem
{
	INT nSkillID;
	BYTE nLevel;
	BYTE SkillType;
	UINT GetBufSize( VOID )const;

	BOOL Read( SocketInputStream& iStream );
	BOOL Write( SocketOutputStream& oStream )const;
};

struct ScriptParam_SkillStudy
{
	BYTE			m_yStudyCount;							// 技能数目
	SSkillItem		m_aSkillItem[MAX_SKILL_ITEM_COUNT];		// 技能列表
	INT				m_nReserve;

	VOID Reset( VOID ){
		m_yStudyCount	= 0;
		m_nReserve		= INVALID_ID;
		memset( &m_aSkillItem, 0, sizeof(SSkillItem)*MAX_SKILL_ITEM_COUNT );
	}

	UINT GetBufSize( VOID )const;

	BOOL Read( SocketInputStream& iStream );
	BOOL Write( SocketOutputStream& oStream )const;
};
#endif
