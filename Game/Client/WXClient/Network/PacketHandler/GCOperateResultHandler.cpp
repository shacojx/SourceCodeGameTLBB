#include "stdafx.h"
#include <string>
#include "..\WxRender\RenderSystem.h"
#include "TDEventDefine.h"
#include "TDEventSystem.h"
#include "OResultDef.h"
#include "..\..\Procedure\GameProcedure.h"
#include "GCOperateResult.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Event\GMEventSystem.h"

uint GCOperateResultHandler::Execute( GCOperateResult* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	OPERATE_RESULT oCode = (OPERATE_RESULT)(pPacket->getResult());
	LPCSTR lpszText = GetOResultText( oCode );

	CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE, lpszText);

	switch( oCode )
	{
	case OR_NEED_SETMINORPASSWORD:
		CEventSystem::GetMe()->PushEvent(GE_MINORPASSWORD_OPEN_SET_PASSWORD_DLG);
		break;
	case OR_NEED_UNLOCKMINORPASSWORD:
		CEventSystem::GetMe()->PushEvent(GE_MINORPASSWORD_OPEN_UNLOCK_PASSWORD_DLG);
		break;
	default:
		break;
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
