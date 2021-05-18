#include "stdafx.h"


#include "CGCityAskAttr.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"
#include "GCCityAttr.h"

uint CGCityAskAttrHandler::Execute( CGCityAskAttr* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;
	//消息的合法性
	if(pScene->m_SceneInitData.m_nDataType != SCENE_TYPE_CIT)
	{
		return PACKET_EXE_CONTINUE;
	}
	//当前场景中的建筑物刷过来，将来再整合消息
	for(INT i = 0; i<MAX_CITY_BUILDING_NUM; i++)
	{
		if(pScene->m_SceneInitData.m_CityData.m_Builds[i].m_BuildingID != INVALID_ID)
		{
			GCCityAttr MsgToClient;
			MsgToClient.SetUpdateAttrib(CITY_UPDATE_BUILDING, TRUE);
			MsgToClient.SetBuildingIndex(i);
			MsgToClient.SetBuildingData(pScene->m_SceneInitData.m_CityData.m_Builds[i]);
			pGamePlayer->SendPacket(&MsgToClient);
		}
	}
	
	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
