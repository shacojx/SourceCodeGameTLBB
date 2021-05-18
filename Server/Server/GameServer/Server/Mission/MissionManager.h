// MissionManager.h
// 
// 功能：任务管理，任务脚本数据的操作中枢
// 
/////////////////////////////////////////////////////////////////////////////////

#ifndef __MISSIONMANAGER_H__
#define __MISSIONMANAGER_H__

#include "Type.h"

struct MISSION_ITEM_INFO
{
	BYTE		m_yCount;
	UINT		m_uItemID;

	MISSION_ITEM_INFO &operator = ( const MISSION_ITEM_INFO &in )
	{
		if ( &in != this )
		{
			m_yCount		= in.m_yCount;
			m_uItemID		= in.m_uItemID;
		}
		return *this;
	}

	VOID Reset( VOID )
	{
		m_yCount	= 0;
		m_uItemID	= UINT_MAX;
	}

	UINT GetBufSize( VOID )const;

	BOOL Read( SocketInputStream& iStream );
	BOOL Write( SocketOutputStream& oStream )const;
};

enum ENUM_MISSION_DATA_ITEM_TYPE
{
	MISSION_DATA_ITEM_TYPE_INVALID	= -1,
	MISSION_DATA_ITEM_TYPE_SECTION,			// 标题
	MISSION_DATA_ITEM_TYPE_TEXT,			// 文本
	MISSION_DATA_ITEM_TYPE_ITEM,			// 物品
	MISSION_DATA_ITEM_TYPE_ITEM_RAND,		// 随机物品
	MISSION_DATA_ITEM_TYPE_ITEM_RADIO,		// 多选1物品
};

#define MISSION_DATA_ITEM_STRING_LENGTH	(256)
struct MISSION_DATA_ITEM
{
	INT							m_nType;		// ENUM_MISSION_DATA_ITEM_TYPE
	CHAR						*m_pszString;
	MISSION_ITEM_INFO			m_Item;

	MISSION_DATA_ITEM()
	{
		m_nType		= MISSION_DATA_ITEM_TYPE_INVALID;
		m_pszString	= NULL;
	}

	~MISSION_DATA_ITEM()
	{
		Reset();
	}

	MISSION_DATA_ITEM &operator = ( const MISSION_DATA_ITEM &in )
	{
		if ( &in != this )
		{
			switch ( in.m_nType )
			{
			case MISSION_DATA_ITEM_TYPE_SECTION:
				SetSection( in.m_pszString );
				break;
			case MISSION_DATA_ITEM_TYPE_TEXT:
				SetText( in.m_pszString );
				break;
			case MISSION_DATA_ITEM_TYPE_ITEM:
				SetItem( in.m_Item.m_uItemID, in.m_Item.m_yCount );
				break;
			case MISSION_DATA_ITEM_TYPE_ITEM_RAND:
				SetItemRand( );
				break;
			case MISSION_DATA_ITEM_TYPE_ITEM_RADIO:
				SetItemRadio( in.m_Item.m_uItemID, in.m_Item.m_yCount );
				break;
			default:
				Reset();
				break;
			}
		}
		return *this;
	}

	VOID Reset( VOID )
	{
		switch ( m_nType )
		{
		case MISSION_DATA_ITEM_TYPE_SECTION:
		case MISSION_DATA_ITEM_TYPE_TEXT:
			SAFE_DELETE_ARRAY( m_pszString );
			break;
		case MISSION_DATA_ITEM_TYPE_ITEM:
		case MISSION_DATA_ITEM_TYPE_ITEM_RAND:
		case MISSION_DATA_ITEM_TYPE_ITEM_RADIO:
			m_Item.Reset();
			break;
		default:
			break;
		}
		m_nType		= MISSION_DATA_ITEM_TYPE_INVALID;
	}

	VOID SetSection( const CHAR *pszText )
	{
		Reset();
		m_nType		= MISSION_DATA_ITEM_TYPE_SECTION;
		if ( pszText != NULL )
		{
			size_t uSize = strlen( pszText ) + 1;

			m_pszString	= new CHAR [uSize];
			memcpy( m_pszString, pszText, uSize );
		}
		else
		{
			m_pszString = NULL;
		}
	}

	VOID SetText( const CHAR *pszText )
	{
		Reset();
		m_nType		= MISSION_DATA_ITEM_TYPE_TEXT;
		if ( pszText != NULL )
		{
			size_t uSize = strlen( pszText ) + 1;

			m_pszString	= new CHAR [uSize];
			memcpy( m_pszString, pszText, uSize );
		}
		else
		{
			m_pszString = NULL;
		}
	}

	VOID SetItem( UINT uItemID, BYTE yItemCount )
	{
		Reset();
		m_nType				= MISSION_DATA_ITEM_TYPE_ITEM;
		m_Item.m_yCount		= yItemCount;
		m_Item.m_uItemID	= uItemID;
	}

	VOID SetItemRand( VOID )
	{
		Reset();
		m_nType				= MISSION_DATA_ITEM_TYPE_ITEM_RAND;
	}

	VOID SetItemRadio( UINT uItemID, BYTE yItemCount )
	{
		Reset();
		m_nType				= MISSION_DATA_ITEM_TYPE_ITEM_RADIO;
		m_Item.m_yCount		= yItemCount;
		m_Item.m_uItemID	= uItemID;
	}

	UINT GetBufSize( VOID )const;

	BOOL Read( SocketInputStream& iStream );
	BOOL Write( SocketOutputStream& oStream )const;
};

#define MISSION_DATA_ITEM_NUMBER	(16)
struct MISSION_DATA
{
	UINT					m_uCount;
	MISSION_DATA_ITEM		m_aItem[MISSION_DATA_ITEM_NUMBER];

	MISSION_DATA( )
	{
		m_uCount = 0;
	}

	~MISSION_DATA( )
	{
		Reset();
	}

	MISSION_DATA &operator = ( const MISSION_DATA &in )
	{
		if ( this != &in )
		{
			UINT i;
			for ( i = 0; i < in.m_uCount; i++ )
			{
				m_aItem[i]	= in.m_aItem[i];
			}
			for ( ; i < m_uCount; i++ )
			{
				m_aItem[i].Reset();
			}
			m_uCount = in.m_uCount;
		}
		return *this;
	}

	VOID Reset( )
	{
		UINT i;
		for ( i = 0; i < m_uCount; i++ )
		{
			m_aItem[i].Reset();
		}
		m_uCount = 0;
	}


	VOID AddSection( const CHAR *pszText )
	{
		if ( m_uCount >= MISSION_DATA_ITEM_NUMBER )
		{
			Assert( m_uCount < MISSION_DATA_ITEM_NUMBER && "MISSION_DATA::AddSection" );
			return ;
		}
		m_aItem[m_uCount].SetSection( pszText );
	}

	VOID AddText( const CHAR *pszText )
	{
		if ( m_uCount >= MISSION_DATA_ITEM_NUMBER )
		{
			Assert( m_uCount < MISSION_DATA_ITEM_NUMBER && "MISSION_DATA::AddText" );
			return ;
		}
		m_aItem[m_uCount].SetText( pszText );
	}

	VOID AddItem( UINT uItemID, BYTE yItemCount )
	{
		if ( m_uCount >= MISSION_DATA_ITEM_NUMBER )
		{
			Assert( m_uCount < MISSION_DATA_ITEM_NUMBER && "MISSION_DATA::AddItem" );
			return ;
		}
		m_aItem[m_uCount].SetItem( uItemID, yItemCount );
	}

	VOID AddItemRand( VOID )
	{
		if ( m_uCount >= MISSION_DATA_ITEM_NUMBER )
		{
			Assert( m_uCount < MISSION_DATA_ITEM_NUMBER && "MISSION_DATA::AddItemRand" );
			return ;
		}
		m_aItem[m_uCount].SetItemRand( );
	}

	VOID AddItemRadio( UINT uItemID, BYTE yItemCount )
	{
		if ( m_uCount >= MISSION_DATA_ITEM_NUMBER )
		{
			Assert( m_uCount < MISSION_DATA_ITEM_NUMBER && "MISSION_DATA::AddItemRadio" );
			return ;
		}
		m_aItem[m_uCount].SetItemRadio( uItemID, yItemCount );
	}
};

class Mission
{
public:
	Mission( );
	~Mission( );

	BOOL Init( MissionID_t idMission, ScriptID_t idScript, SceneID_t idScene );
	VOID CleanUp( );

	const MISSION_DATA *GetDetailInfo( )const
	{
		return &m_DetailInfo;
	}

	//const MISSION_DATA *GetAcceptInfo( )const
	//{
	//	return &m_AcceptInfo;
	//}

	//const MISSION_DATA *GetDemandInfo( )const
	//{
	//	return &m_DemandInfo;
	//}

	//const MISSION_DATA *GetSubmitInfo( )const
	//{
	//	return &m_SubmitInfo;
	//}

protected:
	MissionID_t			m_idMission;
	ScriptID_t			m_idScript;
	MISSION_DATA		m_DetailInfo;
	//MISSION_DATA		m_AcceptInfo;
	//MISSION_DATA		m_DemandInfo;
	//MISSION_DATA		m_SubmitInfo;
};

class MissionManager
{
public:
	MissionManager( );
	~MissionManager( );

	BOOL Init( UINT uMissionNumber );
	VOID CleanUp( );

	const Mission *GetMission( MissionID_t idMission, ScriptID_t idScript, SceneID_t idScene );

protected:
	UINT			m_uMissionLength;
	Mission			**m_papMission;
};

#endif // __MISSIONMANAGER_H__
