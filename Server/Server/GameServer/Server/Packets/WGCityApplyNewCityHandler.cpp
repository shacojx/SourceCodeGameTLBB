#include "stdafx.h"

#include "WGCityApplyNewCity.h"
#include "Log.h"
#include "ServerManager.h"
#include "GamePlayer.h"
#include "PlayerPool.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "DynamicSceneManager.h"

uint WGCityApplyNewCityHandler::Execute( WGCityApplyNewCity* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	//_____________________________________________
	return PACKET_EXE_CONTINUE;

	if( pPlayer->IsServerPlayer() )
	{//服务器收到世界服务器发来的数据
		//入口场景ID
		SceneID_t		PortSceneID	= pPacket->GetSceneID();
		GUID_t			PlayerGuid	= pPacket->GetPlayerGuid();
	
		CITYSCENE_SELECT cityselect;
		cityselect.m_PortSceneID	=	PortSceneID;
		cityselect.m_PlayerGuid		=	PlayerGuid;

		DYNAMICSCENE_SELECT DySelect;
		DySelect.m_CitySelect		= cityselect;
		DySelect.m_SceneDataType	= (INT)SCENE_TYPE_CIT;

		//由damon线程去load城市
		g_pDynamicSceneManager->SelectScene(&DySelect);
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
