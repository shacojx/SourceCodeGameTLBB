#include "stdafx.h"
#include "stdafx.h"


#include "WGRetSceneData.h"



BOOL WGRetSceneData::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_SceneID), sizeof(SceneID_t));
	iStream.Read( (CHAR*)(&m_SceneInitData), sizeof(SCENE_INIT_DATA));
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL WGRetSceneData::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_SceneID), sizeof(SceneID_t) ) ;
	oStream.Write( (CHAR*)(&m_SceneInitData), sizeof(SCENE_INIT_DATA) ) ;
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT WGRetSceneData::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return WGRetSceneDataHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


