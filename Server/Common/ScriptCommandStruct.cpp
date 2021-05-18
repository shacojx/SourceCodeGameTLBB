#include "stdafx.h"
// ScriptCommandStruct.cpp

#include "Type.h"
#include "ScriptCommandStruct.h"

UINT ScriptString::GetBufSize( VOID )const
{
__ENTER_FUNCTION

	UINT uSize = sizeof(SHORT)+m_Size*sizeof(CHAR) ;
	return uSize;

__LEAVE_FUNCTION

	return 0;
}

BOOL ScriptString::Read( SocketInputStream& iStream )
{
__ENTER_FUNCTION
	iStream.Read( (CHAR*)(&m_Size), sizeof(SHORT));
	if ( m_Size>0 )
	{
		iStream.Read( m_szString, m_Size*sizeof(CHAR) );
		m_szString[m_Size] = 0;
	}
	else
	{
		m_Size=0 ;
		m_szString[0] = 0;
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL ScriptString::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION
	oStream.Write( (CHAR*)(&m_Size), sizeof(SHORT));
	if ( m_Size>0 )
	{
		oStream.Write( m_szString, m_Size*sizeof(CHAR) );
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT ScriptEventItem::GetBufSize( VOID )const
{
__ENTER_FUNCTION

	UINT uSize;
	uSize = sizeof ( m_nType );
	switch ( m_nType )
	{
	case EVENT_ITEM_TYPE_TEXT:
	case EVENT_ITEM_TYPE_SECTION:
		uSize += m_strString.GetBufSize();
		break;
	case EVENT_ITEM_TYPE_SCRIPT_ID:
		uSize += m_strString.GetBufSize();
		uSize += sizeof ( m_idScript );
		uSize += sizeof ( m_state );
		uSize += sizeof ( m_index );
		break;
	default:
		break;
	}
		
	return uSize;

__LEAVE_FUNCTION

	return 0;
}

BOOL ScriptEventItem::Read( SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_nType), sizeof(m_nType));
	switch ( m_nType )
	{
	case EVENT_ITEM_TYPE_TEXT:
	case EVENT_ITEM_TYPE_SECTION:
		m_strString.Read( iStream );
		break;
	case EVENT_ITEM_TYPE_SCRIPT_ID:
		iStream.Read( (CHAR*)(&m_index), sizeof(m_index));
		iStream.Read( (CHAR*)(&m_state), sizeof(m_state));
		iStream.Read( (CHAR*)(&m_idScript), sizeof(m_idScript));
		m_strString.Read( iStream );
		break;
	default:
		break;
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL ScriptEventItem::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_nType), sizeof(m_nType));
	switch ( m_nType )
	{
	case EVENT_ITEM_TYPE_TEXT:
	case EVENT_ITEM_TYPE_SECTION:
		m_strString.Write( oStream );
		break;
	case EVENT_ITEM_TYPE_SCRIPT_ID:
		oStream.Write( (CHAR*)(&m_index), sizeof(m_index));
		oStream.Write( (CHAR*)(&m_state), sizeof(m_state));
		oStream.Write( (CHAR*)(&m_idScript), sizeof(m_idScript));
		m_strString.Write( oStream );
		break;
	default:
		break;
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT ScriptParam_EventList::GetBufSize( VOID )const
{
	UINT uSize;
	BYTE i;
	uSize = sizeof( m_idNPC );
	uSize += sizeof( m_yItemCount );
	for ( i = 0; i < m_yItemCount; i++ )
	{
		uSize += m_seiItem[i].GetBufSize();
	}
	return uSize;
}

BOOL ScriptParam_EventList::Read( SocketInputStream& iStream )
{
__ENTER_FUNCTION

	BYTE i;
	iStream.Read( (CHAR*)(&m_idNPC), sizeof(m_idNPC));
	iStream.Read( (CHAR*)(&m_yItemCount), sizeof(m_yItemCount));
	for ( i = 0; i < m_yItemCount; i++ )
	{
		m_seiItem[i].Read( iStream );
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL ScriptParam_EventList::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	BYTE i;
	oStream.Write( (CHAR*)(&m_idNPC), sizeof(m_idNPC));
	oStream.Write( (CHAR*)(&m_yItemCount), sizeof(m_yItemCount));
	for ( i = 0; i < m_yItemCount; i++ )
	{
		m_seiItem[i].Write( oStream );
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT SMissionBonusItem::GetBufSize( VOID )const
{
__ENTER_FUNCTION

	return sizeof( m_yCount ) + sizeof( m_uItemID );

__LEAVE_FUNCTION
}

BOOL SMissionBonusItem::Read( SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_yCount), sizeof(m_yCount));
	iStream.Read( (CHAR*)(&m_uItemID), sizeof(m_uItemID));

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL SMissionBonusItem::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_yCount), sizeof(m_yCount));
	oStream.Write( (CHAR*)(&m_uItemID), sizeof(m_uItemID));

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT SMissionDemandKill::GetBufSize( VOID )const
{
__ENTER_FUNCTION

	return sizeof( m_yCount ) + sizeof( m_uNPCID );

__LEAVE_FUNCTION
}

BOOL SMissionDemandKill::Read( SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_yCount), sizeof(m_yCount));
	iStream.Read( (CHAR*)(&m_uNPCID), sizeof(m_uNPCID));

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL SMissionDemandKill::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_yCount), sizeof(m_yCount));
	oStream.Write( (CHAR*)(&m_uNPCID), sizeof(m_uNPCID));

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT SMissionBonus::GetBufSize( VOID )const
{
__ENTER_FUNCTION
	UINT uSize;
	uSize = sizeof( m_nType );
	switch ( m_nType )
	{
	case MISSION_BONUS_TYPE_MONEY:
		uSize += sizeof( m_uMoney );
		break;
	case MISSION_BONUS_TYPE_ITEM:
		uSize += m_ItemBonus.GetBufSize();
		break;
	case MISSION_BONUS_TYPE_ITEM_RAND:
		break;
	case MISSION_BONUS_TYPE_ITEM_RADIO:
		uSize += m_ItemBonus.GetBufSize();
		break;
	default:
		break;
	}
	return uSize;

__LEAVE_FUNCTION

	return 0;
}

BOOL SMissionBonus::Read( SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_nType), sizeof(m_nType));
	switch ( m_nType )
	{
	case MISSION_BONUS_TYPE_MONEY:
		iStream.Read( (CHAR*)(&m_uMoney), sizeof(m_uMoney));
		break;
	case MISSION_BONUS_TYPE_ITEM:
		m_ItemBonus.Read( iStream );
		break;
	case MISSION_BONUS_TYPE_ITEM_RAND:
		break;
	case MISSION_BONUS_TYPE_ITEM_RADIO:
		m_ItemBonus.Read( iStream );
		break;
	default:
		break;
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL SMissionBonus::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_nType), sizeof(m_nType));
	switch ( m_nType )
	{
	case MISSION_BONUS_TYPE_MONEY:
		oStream.Write( (CHAR*)(&m_uMoney), sizeof(m_uMoney));
		break;
	case MISSION_BONUS_TYPE_ITEM:
		m_ItemBonus.Write( oStream );
		break;
	case MISSION_BONUS_TYPE_ITEM_RAND:
		break;
	case MISSION_BONUS_TYPE_ITEM_RADIO:
		m_ItemBonus.Write( oStream );
		break;
	default:
		break;
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT ScriptParam_MissionInfo::GetBufSize( VOID )const
{
__ENTER_FUNCTION
	UINT uSize;
	BYTE i;
	uSize = sizeof( m_idNPC );
	uSize += sizeof( m_idScript );
	uSize += sizeof( m_idMission );
	uSize += sizeof( m_yTextCount );
	uSize += sizeof( m_IsPush );

	for ( i = 0; i < m_yTextCount; i++ )
	{
		uSize += m_aText[i].GetBufSize();
	}
	uSize += sizeof( m_yBonusCount );
	for ( i = 0; i < m_yBonusCount; i++ )
	{
		uSize += m_aBonus[i].GetBufSize();
	}
	return uSize;

__LEAVE_FUNCTION

	return 0;
}

BOOL ScriptParam_MissionInfo::Read( SocketInputStream& iStream )
{
__ENTER_FUNCTION

	BYTE i;
	iStream.Read( (CHAR*)(&m_idNPC), sizeof(m_idNPC));
	iStream.Read( (CHAR*)(&m_idScript), sizeof(m_idScript));
	iStream.Read( (CHAR*)(&m_idMission), sizeof(m_idMission));
	iStream.Read( (CHAR*)(&m_yTextCount), sizeof(m_yTextCount));
	iStream.Read( (CHAR*)(&m_IsPush), sizeof(m_IsPush));
	for ( i = 0; i < m_yTextCount; i++ )
	{
		m_aText[i].Read( iStream );
	}
	iStream.Read( (CHAR*)(&m_yBonusCount), sizeof(m_yBonusCount));
	for ( i = 0; i < m_yBonusCount; i++ )
	{
		m_aBonus[i].Read( iStream );
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL ScriptParam_MissionInfo::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	BYTE i;
	oStream.Write( (CHAR*)(&m_idNPC), sizeof(m_idNPC));
	oStream.Write( (CHAR*)(&m_idScript), sizeof(m_idScript));
	oStream.Write( (CHAR*)(&m_idMission), sizeof(m_idMission));
	oStream.Write( (CHAR*)(&m_yTextCount), sizeof(m_yTextCount));
	oStream.Write( (CHAR*)(&m_IsPush), sizeof(m_IsPush));
	for ( i = 0; i < m_yTextCount; i++ )
	{
		m_aText[i].Write( oStream );
	}
	oStream.Write( (CHAR*)(&m_yBonusCount), sizeof(m_yBonusCount));
	for ( i = 0; i < m_yBonusCount; i++ )
	{
		m_aBonus[i].Write( oStream );
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT ScriptParam_MissionDemandInfo::GetBufSize( VOID )const
{
__ENTER_FUNCTION

	UINT uSize;
	BYTE i;
	uSize = sizeof( m_idNPC );
	uSize += sizeof( m_idScript );
	uSize += sizeof( m_idMission );
	uSize += sizeof( m_bDone );
	uSize += sizeof( m_yTextCount );
	for ( i = 0; i < m_yTextCount; i++ )
	{
		uSize += m_aText[i].GetBufSize();
	}
	uSize += sizeof( m_yDemandCount );
	for ( i = 0; i < m_yDemandCount; i++ )
	{
		uSize += m_aDemandItem[i].GetBufSize();
	}

	return uSize;

__LEAVE_FUNCTION

	return 0;
}

BOOL ScriptParam_MissionDemandInfo::Read( SocketInputStream& iStream )
{
__ENTER_FUNCTION
	BYTE i;

	iStream.Read( (CHAR*)(&m_idNPC), sizeof(m_idNPC));
	iStream.Read( (CHAR*)(&m_idScript), sizeof(m_idScript));
	iStream.Read( (CHAR*)(&m_idMission), sizeof(m_idMission));
	iStream.Read( (CHAR*)(&m_bDone), sizeof(m_bDone));
	iStream.Read( (CHAR*)(&m_yTextCount), sizeof(m_yTextCount));
	for ( i = 0; i < m_yTextCount; i++ )
	{
		m_aText[i].Read( iStream );
	}
	iStream.Read( (CHAR*)(&m_yDemandCount), sizeof(m_yDemandCount));
	for ( i = 0; i < m_yDemandCount; i++ )
	{
		m_aDemandItem[i].Read( iStream );
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL ScriptParam_MissionDemandInfo::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	BYTE i;
	oStream.Write( (CHAR*)(&m_idNPC), sizeof(m_idNPC));
	oStream.Write( (CHAR*)(&m_idScript), sizeof(m_idScript));
	oStream.Write( (CHAR*)(&m_idMission), sizeof(m_idMission));
	oStream.Write( (CHAR*)(&m_bDone), sizeof(m_bDone));
	oStream.Write( (CHAR*)(&m_yTextCount), sizeof(m_yTextCount));
	for ( i = 0; i < m_yTextCount; i++ )
	{
		m_aText[i].Write( oStream );
	}
	oStream.Write( (CHAR*)(&m_yDemandCount), sizeof(m_yDemandCount));
	for ( i = 0; i < m_yDemandCount; i++ )
	{
		m_aDemandItem[i].Write( oStream );
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT ScriptParam_MissionContinueInfo::GetBufSize( VOID )const
{
__ENTER_FUNCTION
	UINT uSize;
	BYTE i;
	uSize = sizeof( m_idNPC );
	uSize += sizeof( m_idScript );
	uSize += sizeof( m_idMission );
	uSize += sizeof( m_yTextCount );
	for ( i = 0; i < m_yTextCount; i++ )
	{
		uSize += m_aText[i].GetBufSize();
	}
	uSize += sizeof( m_yBonusCount );
	for ( i = 0; i < m_yBonusCount; i++ )
	{
		uSize += m_aBonus[i].GetBufSize();
	}
	return uSize;

__LEAVE_FUNCTION

	return 0;
}

BOOL ScriptParam_MissionContinueInfo::Read( SocketInputStream& iStream )
{
__ENTER_FUNCTION

	BYTE i;
	iStream.Read( (CHAR*)(&m_idNPC), sizeof(m_idNPC));
	iStream.Read( (CHAR*)(&m_idScript), sizeof(m_idScript));
	iStream.Read( (CHAR*)(&m_idMission), sizeof(m_idMission));
	iStream.Read( (CHAR*)(&m_yTextCount), sizeof(m_yTextCount));
	for ( i = 0; i < m_yTextCount; i++ )
	{
		m_aText[i].Read( iStream );
	}
	iStream.Read( (CHAR*)(&m_yBonusCount), sizeof(m_yBonusCount));
	for ( i = 0; i < m_yBonusCount; i++ )
	{
		m_aBonus[i].Read( iStream );
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL ScriptParam_MissionContinueInfo::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	BYTE i;
	oStream.Write( (CHAR*)(&m_idNPC), sizeof(m_idNPC));
	oStream.Write( (CHAR*)(&m_idScript), sizeof(m_idScript));
	oStream.Write( (CHAR*)(&m_idMission), sizeof(m_idMission));
	oStream.Write( (CHAR*)(&m_yTextCount), sizeof(m_yTextCount));
	for ( i = 0; i < m_yTextCount; i++ )
	{
		m_aText[i].Write( oStream );
	}
	oStream.Write( (CHAR*)(&m_yBonusCount), sizeof(m_yBonusCount));
	for ( i = 0; i < m_yBonusCount; i++ )
	{
		m_aBonus[i].Write( oStream );
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT ScriptParam_MissionTips::GetBufSize( VOID )const
{
__ENTER_FUNCTION
	UINT uSize;
	uSize = m_strText.GetBufSize();
	return uSize;

__LEAVE_FUNCTION

	return 0;
}

BOOL ScriptParam_MissionTips::Read( SocketInputStream& iStream )
{
__ENTER_FUNCTION

	m_strText.Read( iStream );
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL ScriptParam_MissionTips::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	m_strText.Write( oStream );
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT STradeItem::GetBufSize( VOID )const
{
__ENTER_FUNCTION
	UINT uSize;
	uSize = sizeof( m_uDataID ) + sizeof( m_typeItem ) + sizeof( m_yCount );
	return uSize;

__LEAVE_FUNCTION

	return 0;
}

BOOL STradeItem::Read( SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_uDataID), sizeof(m_uDataID));
	iStream.Read( (CHAR*)(&m_typeItem), sizeof(m_typeItem));
	iStream.Read( (CHAR*)(&m_yCount), sizeof(m_yCount));
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL STradeItem::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_uDataID), sizeof(m_uDataID));
	oStream.Write( (CHAR*)(&m_typeItem), sizeof(m_typeItem));
	oStream.Write( (CHAR*)(&m_yCount), sizeof(m_yCount));
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT ScriptParam_Trade::GetBufSize( VOID )const
{
__ENTER_FUNCTION
	UINT uSize;
	uSize = sizeof( m_yItemCount );
	BYTE i;
	for( i = 0; i < m_yItemCount; i++ )
	{
		m_aTradeItem[i].GetBufSize();
	}
	uSize += sizeof( m_bRepair );
	return uSize;

__LEAVE_FUNCTION

	return 0;
}

BOOL ScriptParam_Trade::Read( SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_yItemCount), sizeof(m_yItemCount));
	BYTE i;
	for( i = 0; i < m_yItemCount; i++ )
	{
		m_aTradeItem[i].Read( iStream );
	}
	iStream.Read( (CHAR*)(&m_bRepair), sizeof(m_bRepair));
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL ScriptParam_Trade::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_yItemCount), sizeof(m_yItemCount));
	BYTE i;
	for( i = 0; i < m_yItemCount; i++ )
	{
		m_aTradeItem[i].Write( oStream );
	}
	oStream.Write( (CHAR*)(&m_bRepair), sizeof(m_bRepair));
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT SSkillItem::GetBufSize( VOID )const
{
	__ENTER_FUNCTION
		UINT uSize;
	uSize = sizeof( nSkillID ) + sizeof( nSkillID )+sizeof(SkillType);
	return uSize;

	__LEAVE_FUNCTION

		return 0;
}

BOOL SSkillItem::Read( SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&nSkillID), sizeof(nSkillID));
	iStream.Read( (CHAR*)(&nLevel), sizeof(nLevel));
	iStream.Read( (CHAR*)(&SkillType), sizeof(SkillType));
	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL SSkillItem::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)(&nSkillID), sizeof(nSkillID));
	oStream.Write( (CHAR*)(&nLevel), sizeof(nLevel));
	oStream.Write( (CHAR*)(&SkillType), sizeof(SkillType));
	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

UINT ScriptParam_SkillStudy::GetBufSize( VOID )const
{
	__ENTER_FUNCTION
		UINT uSize;
	uSize = sizeof( m_yStudyCount ) + sizeof( SSkillItem )* m_yStudyCount+sizeof(m_nReserve);
	return uSize;

	__LEAVE_FUNCTION

		return 0;
}

BOOL ScriptParam_SkillStudy::Read( SocketInputStream& iStream )
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&m_yStudyCount), sizeof(m_yStudyCount));
	iStream.Read( (CHAR*)(&m_aSkillItem), sizeof(SSkillItem)*m_yStudyCount);
	iStream.Read( (CHAR*)(&m_nReserve), sizeof(m_nReserve));
	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL ScriptParam_SkillStudy::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

		oStream.Write( (CHAR*)(&m_yStudyCount), sizeof(m_yStudyCount));
	oStream.Write( (CHAR*)(&m_aSkillItem), sizeof(SSkillItem)*m_yStudyCount);
	oStream.Write( (CHAR*)(&m_nReserve), sizeof(m_nReserve));
	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}