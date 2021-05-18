#include "StdAfx.h"
#include "GCMail.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\DataPool\GMDP_Struct_Mail.h"
#include "..\..\DataPool\GMDP_Struct_Relation.h"
#include "..\..\Procedure\GameProcedure.h"

uint GCMailHandler::Execute( GCMail* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CMailPool* pMailPool = CDataPool::GetMe()->GetMailPool();
		MAIL_LIST* pMailList = pPacket->GetMailList();

		// 加入新邮件
		for( INT i=0; i<pMailList->m_Count; ++i )
		{
			INT nIndex;

			if( pMailList->m_aMail[i].m_uFlag == MAIL_TYPE_NORMAL2 ||
				pMailList->m_aMail[i].m_uFlag == MAIL_TYPE_SYSTEM )
			{
				std::string strTemp = "";
				strTemp = NOCOLORMSGFUNC("SERVER_MAIL");
				_snprintf(pMailList->m_aMail[i].m_szSourName, 30, "%s", strTemp.c_str() );
				pMailList->m_aMail[i].m_SourSize = (BYTE)strlen(pMailList->m_aMail[i].m_szSourName);
			}

			nIndex = pMailPool->AddMail( &(pMailList->m_aMail[i]) );

			if( nIndex > -1 )
			{
				CDataPool::GetMe()->GetRelation()->AddMail( pMailList->m_aMail[i].m_szSourName, nIndex );
			}
		}

		pMailPool->SetMailLeftCount( pMailList->m_TotalLeft );

		// 让 UI 显示最后一封邮件
		pMailPool->ShowLastMail();
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
