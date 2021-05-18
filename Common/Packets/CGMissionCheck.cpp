#include "stdafx.h"
// CGMissionCheck.cpp
// 
/////////////////////////////////////////////////////

#include "CGMissionCheck.h"

using namespace Packets;


BOOL CGMissionCheck::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_idNPC), sizeof(ObjID_t) ) ;
	iStream.Read( (CHAR*)(&m_idScript), sizeof(ScriptID_t) ) ;
	iStream.Read( (CHAR*)(m_ItemIndexList), sizeof(BYTE)*ITEM_MAX_NUM ) ;
	iStream.Read( (CHAR*)(&m_PetIndex), sizeof(BYTE) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGMissionCheck::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_idNPC), sizeof(ObjID_t) ) ;
	oStream.Write( (CHAR*)(&m_idScript), sizeof(ScriptID_t) ) ;
	oStream.Write( (CHAR*)(m_ItemIndexList), sizeof(BYTE)*ITEM_MAX_NUM ) ;
	oStream.Write( (CHAR*)(&m_PetIndex), sizeof(BYTE) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGMissionCheck::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGMissionCheckHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

