#include "StdAfx.h"
#include "GCGuildError.h"

#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Event\GMEventSystem.h"

uint GCGuildErrorHandler::Execute( GCGuildError* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CHAR szText[_MAX_PATH];

		_snprintf(szText, _MAX_PATH, "其他帮会错误。");

		switch( pPacket->GetErrorCode() )
		{
		case GUILD_ERROR_WANTING			:
			{
				_snprintf(szText, _MAX_PATH, "不满足创建条件。");
			}
			break;
		case GUILD_ERROR_INVALID_NAME		:
			{
				_snprintf(szText, _MAX_PATH, "帮会名称非法。");
			}
			break;
		case GUILD_ERROR_DUPLICATED_NAME	:
			{
				_snprintf(szText, _MAX_PATH, "帮会名称与现有名称重复。");
			}
			break;
		case GUILD_ERROR_GUILD_FULL			:
			{
				_snprintf(szText, _MAX_PATH, "无法创建更多帮会。");
			}
			break;
		case GUILD_ERROR_MEMBER_FULL		:
			{
				_snprintf(szText, _MAX_PATH, "帮会满员了。");
			}
			break;
		case GUILD_ERROR_PROPOSER_FULL		:
			{
				_snprintf(szText, _MAX_PATH, "申请人已满。");
			}
			break;
		case GUILD_ERROR_IN_GUILD			:
			{
				_snprintf(szText, _MAX_PATH, "你已加入帮会。");
			}
			break;
		case GUILD_ERROR_NOT_EXIST			:
			{
				_snprintf(szText, _MAX_PATH, "帮会不存在。");
			}
			break;
		case GUILD_ERROR_UNAUTHORIZED		:
			{
				_snprintf(szText, _MAX_PATH, "没有权限。");
			}
			break;
		case GUILD_ERROR_NO_VACANCY			:
			{
				_snprintf(szText, _MAX_PATH, "没有职位空缺。");
			}
			break;
		case GUILD_ERROR_NO_QUALIFICATION	:
			{
				_snprintf(szText, _MAX_PATH, "被调整人员（职位）不够资格。");
			}
			break;
		case GUILD_ERROR_CASHBOX_FULL		:
			{
				_snprintf(szText, _MAX_PATH, "帮会金库已满。");
			}
			break;
		case GUILD_ERROR_POS_FULL		:
			{
				_snprintf(szText, _MAX_PATH, "这个职位人数已满，不能任命。");
			}
			break;
		case GUILD_ERROR					:
			{
				_snprintf(szText, _MAX_PATH, "未知帮会错误。");
			}
			break;
		case GUILD_ERROR_GUILD_ALREADY_EXIST:
			{
				_snprintf(szText, _MAX_PATH, "帮会已经存在。");
			}
			break;
		case GUILD_ERROR_ALREADY_IN_PROPOSER_LIST:
			{
				_snprintf(szText, _MAX_PATH, "您已经在申请列表中了。");
			}
			break;
		case GUILD_ERROR_INVALID_CAMP:
			{
				_snprintf(szText, _MAX_PATH, "您要申请的帮会与您不是一个阵营。");
			}
			break;
		default:
			Assert(FALSE);
			return 0;
		}

		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, szText );
//		CGameProcedure::s_pEventSystem->PushEvent( GE_CHAT_MESSAGE, "帮会", szText, 0 );
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
