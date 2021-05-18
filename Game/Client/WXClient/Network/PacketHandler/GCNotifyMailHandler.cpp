#include "StdAfx.h"
#include "GCNotifyMail.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\DataPool\GMDP_Struct_Mail.h"
#include "..\..\Procedure\GameProcedure.h"

uint GCNotifyMailHandler::Execute( GCNotifyMail* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CMailPool* pMailPool = CDataPool::GetMe()->GetMailPool();

		// 设置剩余未收邮件数量
		pMailPool->SetMailLeftCount( (INT)pPacket->GetMailCount() );

		// 让 UI 得到消息（图标闪烁等等）
		pMailPool->InformUI();
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
