// GCCharMoveResultHandler.cpp

#include "StdAfx.h"

#include "GCCharMoveResult.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"
#include "..\..\Object\ObjectCommandDef.h"

uint GCCharMoveResultHandler::Execute( GCCharMoveResult* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		INT oResult = pPacket->getResult();
		if(OR_FAILED(oResult))
		{
			if( oResult == OR_CUT_PATHROUTE )
			{
				INT nHandle = pPacket->getHandleID();
				const WORLD_POS* pWorldPos = pPacket->getTargetPos();
				BYTE numPos = pPacket->getNumTargetPos();
				if( numPos > 0 )
				{
					if( CObjectManager::GetMe()->GetMySelf()->ModifyMoveCommand(nHandle, numPos, pWorldPos) == FALSE )
					{
						CObjectManager::GetMe()->GetMySelf()->OnMoveStop( );

						const WORLD_POS* pEndPos = pWorldPos+numPos-1;
						SCommand_Object cmdTemp;
						cmdTemp.m_wID			= OC_TELEPORT;
						cmdTemp.m_afParam[0]	= pEndPos->m_fX;
						cmdTemp.m_afParam[1]	= pEndPos->m_fZ;
						CObjectManager::GetMe()->GetMySelf()->PushCommand(&cmdTemp );
					}
				}
				else
				{
					CObjectManager::GetMe()->GetMySelf()->OnMoveStop( );
				}
			}
			else
			{
				CObjectManager::GetMe()->GetMySelf()->OnMoveStop( );
			}
		}
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

