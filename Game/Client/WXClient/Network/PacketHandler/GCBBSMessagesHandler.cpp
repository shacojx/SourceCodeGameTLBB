
#include "stdafx.h"
#include "GCBBSMessages.h"

#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Object.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"
#include "..\..\Object\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "TDException.h"


uint GCBBSMessagesHandler::Execute( GCBBSMessages* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			BYTE	MessageNum	=	pPacket->GetMessageNum();
			UINT	Serial		=	pPacket->GetSerial();
			CHAR*	pszTitle	=	pPacket->GetTitle();
			if(CObjectManager::GetMe()->GetMySelf()->GetServerID() == pPacket->GetObjID())
			{//自己的留言板
				if(Serial != CDataPool::GetMe()->MyStallMsg_GetSerial())
				{//有新数据
                    CDataPool::GetMe()->MyStallMsg_ClearUp();
					//每个物品添加到数据池中
					for(INT i=0; i<MessageNum; i++)
					{
						GCBBSMessages::_MESSAGE_T* pMessageData = pPacket->GetMessageData();
						CDataPool::GetMe()->MyStallMsg_SetIDByIndex(i, pMessageData[i].nID);
						CDataPool::GetMe()->MyStallMsg_SetTimeByIndex(i, pMessageData[i].nHour, pMessageData[i].nMin);
						CDataPool::GetMe()->MyStallMsg_SetHasReplyByIndex(i, pMessageData[i].bHasReply);
						CDataPool::GetMe()->MyStallMsg_SetAuthorNameByIndex(i, pMessageData[i].AuthorName);
						CDataPool::GetMe()->MyStallMsg_SetProposedMessageByIndex(i, pMessageData[i].szMessage);
						if(pMessageData[i].bHasReply)
						{
							CDataPool::GetMe()->MyStallMsg_SetReTimeByIndex(i, pMessageData[i].nReHour, pMessageData[i].nReMin);
							CDataPool::GetMe()->MyStallMsg_SetReplyMessageByIndex(i, pMessageData[i].szReplyMessage);
						}
					}
					CDataPool::GetMe()->MyStallMsg_SetMessageNum(MessageNum);
					CDataPool::GetMe()->MyStallMsg_SetSerial(Serial);
					CDataPool::GetMe()->MyStallMsg_SetTitle(pszTitle);

					//更新UI
					CEventSystem::GetMe()->PushEvent(GE_OPEN_STALL_MESSAGE,"sale");
				}
				else
				{//版本号相同取本地的就行了
					//更新UI
					CEventSystem::GetMe()->PushEvent(GE_OPEN_STALL_MESSAGE,"sale");
				}
			}
			else
			{//别人的留言板
				CDataPool::GetMe()->OtStallMsg_ClearUp();
				//每个物品添加到数据池中
				for(INT i=0; i<MessageNum; i++)
				{
					GCBBSMessages::_MESSAGE_T* pMessageData = pPacket->GetMessageData();
					CDataPool::GetMe()->OtStallMsg_SetIDByIndex(i, pMessageData[i].nID);
					CDataPool::GetMe()->OtStallMsg_SetTimeByIndex(i, pMessageData[i].nHour, pMessageData[i].nMin);
					CDataPool::GetMe()->OtStallMsg_SetHasReplyByIndex(i, pMessageData[i].bHasReply);
					CDataPool::GetMe()->OtStallMsg_SetAuthorNameByIndex(i, pMessageData[i].AuthorName);
					CDataPool::GetMe()->OtStallMsg_SetProposedMessageByIndex(i, pMessageData[i].szMessage);
					if(pMessageData[i].bHasReply)
					{
						CDataPool::GetMe()->OtStallMsg_SetReTimeByIndex(i, pMessageData[i].nReHour, pMessageData[i].nReMin);
						CDataPool::GetMe()->OtStallMsg_SetReplyMessageByIndex(i, pMessageData[i].szReplyMessage);
					}
				}

				CDataPool::GetMe()->OtStallMsg_SetSerial(Serial);
				CDataPool::GetMe()->OtStallMsg_SetTitle(pszTitle);
				CDataPool::GetMe()->OtStallMsg_SetMessageNum(MessageNum);
				//更新UI
				CEventSystem::GetMe()->PushEvent(GE_OPEN_STALL_MESSAGE,"buy");
			}
		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
