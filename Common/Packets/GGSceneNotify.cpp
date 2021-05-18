#include "stdafx.h"



#include "GGSceneNotify.h"

BOOL GGSceneNotify::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_SourSceneID), sizeof(SceneID_t));
	iStream.Read( (CHAR*)(&m_DestSceneID), sizeof(SceneID_t));


	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GGSceneNotify::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION


	oStream.Write( (CHAR*)(&m_SourSceneID), sizeof(SceneID_t) ) ;
	oStream.Write( (CHAR*)(&m_DestSceneID), sizeof(SceneID_t) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GGSceneNotify::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GGSceneNotifyHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


