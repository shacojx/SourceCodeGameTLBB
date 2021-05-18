#include "stdafx.h"
#include "stdafx.h"


#include "GWAskSceneData.h"



BOOL GWAskSceneData::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_SceneID), sizeof(SceneID_t));
	iStream.Read( (CHAR*)(&m_Type), sizeof(BYTE));
	iStream.Read( (CHAR*)(&m_bExtraLength), sizeof(BYTE));
	iStream.Read( (CHAR*)(m_ExtraData), m_bExtraLength);

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GWAskSceneData::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_SceneID), sizeof(SceneID_t) ) ;
	oStream.Write( (CHAR*)(&m_Type), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_bExtraLength), sizeof(BYTE));
	oStream.Write( (CHAR*)(m_ExtraData), m_bExtraLength);
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT GWAskSceneData::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return GWAskSceneDataHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


