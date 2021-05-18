#include "StdAfx.h"
#include "GCUseItemResult.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\event\GMEventSystem.h"

uint GCUseItemResultHandler::Execute(GCUseItemResult* pPacket,Player* pPlayer)
{
	__ENTER_FUNCTION
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
 		 USEITEM_RESULT res = pPacket->GetResult();
		switch(res) 
		{
		case USEITEM_SUCCESS:
			//药瓶使用成功  how can i get that???
			break;
		case USEITEM_CANNT_USE:
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "此物品不可以使用");
			}
			break;
		case USEITEM_LEVEL_FAIL:
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "等级不够");
			}
			break;
		case USEITEM_TYPE_FAIL:
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "物品类型错误!");
			}
			break;
		case USEITEM_TARGET_TYPE_FAIL:
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "目标类型错误!");
			}
			break;
		case USEITEM_SKILL_FAIL:
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "物品附加技能错误!");
			}
			break;
		case USEITEM_IDENT_TYPE_FAIL: //卷轴类型错误
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "不能鉴定这种物品！");
			}
			break;
		case USEITEM_IDENT_TARGET_TYPE_FAIL://目标类型错误
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "不能鉴定这种物品！");
			}
			break;
		case USEITEM_IDENT_LEVEL_FAIL://卷轴等级不够
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO, "鉴定卷轴等级不够！");
			}
			break;
		default:
			break;
		}

	}

	// 取消鉴定状态
	CDataPool::GetMe()->Identify_UnLock();

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}