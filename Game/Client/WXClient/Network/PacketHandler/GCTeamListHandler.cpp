#include "StdAfx.h"
#include "GCTeamList.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\DataPool\DataPoolCommandDef.h"

uint GCTeamListHandler::Execute( GCTeamList* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CUIDataPool* pDataPool = (CUIDataPool*)(CGameProcedure::s_pDataPool);

		// 先清除队伍信息
		pDataPool->ClearTeamInfo();

		for( INT i=0; i<pPacket->GetMemberCount(); ++i )
		{
			const TEAM_LIST_ENTRY& entry = pPacket->GetTeamMember( i );

			SCommand_DPC cmdTemp;
			cmdTemp.m_wID			= DPC_UPDATE_TEAM_OR_GROUP;

			cmdTemp.m_adwParam[0]	= TEAM_RESULT_TEAMREFRESH;
			cmdTemp.m_adwParam[1]	= entry.GetGUID();
			cmdTemp.m_adwParam[2]	= pPacket->GetTeamID();
			cmdTemp.m_adwParam[3]	= entry.GetExtraID();
			cmdTemp.m_adwParam[4]	= entry.GetSceneID();
			cmdTemp.m_adwParam[5]	= entry.GetIcon();
			cmdTemp.m_adwParam[6]	= entry.GetDataID();
			strncpy((char*)&cmdTemp.m_apParam[7], entry.GetName(), strlen(entry.GetName())+1);
			pDataPool->OnCommand_( &cmdTemp );
		}
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
