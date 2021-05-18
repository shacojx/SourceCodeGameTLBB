#include "StdAfx.h"
#include "GCPlayerShopRecordList.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "TDException.h"

UINT GCPlayerShopRecordListHandler::Execute( GCPlayerShopRecordList* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			_PLAYERSHOP_GUID							nShopID			= pPacket->GetShopID();		//商店ID
			BYTE										bEntryNum		= pPacket->GetEntryNum();
			GCPlayerShopRecordList::MessageEntry_t*		pEntryList		= pPacket->GetEntryList();
			INT											nStallPage		= pPacket->GetPage();

			RecordList_t* pRecordList = CDataPool::GetMe()->GetRecordList();
			pRecordList->CleanUp();
			RecordEntry_t	TempRecordEntry;

			for(INT i = 0; i<bEntryNum; i++)
			{
				TempRecordEntry.szContent = pEntryList[i].szContent;
				pRecordList->m_RecordEntryList[i] = TempRecordEntry;
			}
			pRecordList->m_nRecordNum = bEntryNum;

			// 如果没有记录，就给出屏幕提示
			if( 0 == bEntryNum )
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你的店铺目前没有伙伴操作记录。");
				return PACKET_EXE_CONTINUE;
			}
			//...打开(更新)记录
			CEventSystem::GetMe()->PushEvent(GE_PS_OPEN_MESSAGE, nStallPage);
		}

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
