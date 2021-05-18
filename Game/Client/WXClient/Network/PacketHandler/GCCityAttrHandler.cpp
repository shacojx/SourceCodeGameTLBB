#include "stdAfx.h"
#include "..\..\World\WorldManager.h"
#include "GCCityAttr.h"
#include "..\..\Procedure\GameProcedure.h"

uint GCCityAttrHandler::Execute( GCCityAttr* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			if(pPacket->IsUpdateAttrib(CITY_UPDATE_CITY_LEVEL))
			{
				return PACKET_EXE_CONTINUE;
			}
			if(pPacket->IsUpdateAttrib(CITY_UPDATE_CITY_NAME))
			{
				return PACKET_EXE_CONTINUE;
			}
			if(pPacket->IsUpdateAttrib(CITY_UPDATE_BUILDING))
			{
				CScene* pScene = (CScene*)CWorldManager::GetMe()->GetActiveScene();
				Assert(pScene);
				pScene->UpdateCityBuilding(pPacket->GetBuildingData().m_BuildingID, pPacket->GetBuildingData().m_BuildingLevel);
				return PACKET_EXE_CONTINUE;
			}
		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
