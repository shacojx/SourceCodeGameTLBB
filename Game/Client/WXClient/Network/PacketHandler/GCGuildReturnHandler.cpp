#include "StdAfx.h"
#include "GCGuildReturn.h"

#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Object.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"
#include "..\..\Object\Character\Obj_Character.h"

uint GCGuildReturnHandler::Execute( GCGuildReturn* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		_GUILD_RETURN* pReturn = pPacket->GetGuildReturn();
		CHAR szText[_MAX_PATH];

		_snprintf(szText, _MAX_PATH, "其他帮会操作。");

		switch( pReturn->m_ReturnType )
		{
		case GUILD_RETURN_RESPONSE:
			{
				_snprintf(szText, _MAX_PATH, "%s响应本帮会的建立。", pReturn->m_SourName);
			}
			break;
		case GUILD_RETURN_CREATE:
			{
				_snprintf(szText, _MAX_PATH, "建立帮会成功。");
			}
			break;
		case GUILD_RETURN_JOIN:
			{
				_snprintf(szText, _MAX_PATH, "你已经向%s帮会发出了申请，请等待该帮会处理。");
			}
			break;
		case GUILD_RETURN_EXPEL:
			{
				STRING strTemp = "";
				if(pReturn->m_GUID == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{//自己被踢出帮会
					CDataPool::GetMe()->Guild_ClearMemInfo();
					CDataPool::GetMe()->Guild_ClearDetailInfo();
					//刷新界面
					CEventSystem::GetMe()->PushEvent(GE_GUILD_FORCE_CLOSE);
					strTemp = "%s将%s开除出本帮会";
					_snprintf(szText, _MAX_PATH, strTemp.c_str(), pReturn->m_SourName, pReturn->m_DestName);
				}
				else
				{//别人看到
					INT iMemNum = CDataPool::GetMe()->Guild_GetMemInfoNum();
					for(INT i = 0; i<iMemNum; i++)
					{
						if(CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_Guid == pReturn->m_GUID)
						{//改变职位
							CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->CleanUp();
							break;
						}
					}
					//刷新界面
					CEventSystem::GetMe()->PushEvent(GE_GUILD_UPDATE_MEMBERINFO);
				}
			}
			break;
		case GUILD_RETURN_REJECT:
			{
				STRING strTemp = "";
				if(pReturn->m_GUID == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{//自己的申请被拒绝了
					CDataPool::GetMe()->Guild_ClearMemInfo();
					CDataPool::GetMe()->Guild_ClearDetailInfo();
					//刷新界面
					CEventSystem::GetMe()->PushEvent(GE_GUILD_FORCE_CLOSE);

					_snprintf(szText, _MAX_PATH, "%s帮会拒绝了你的申请", pReturn->m_GuildName);
				}
				else
				{//别人看到
					INT iMemNum = CDataPool::GetMe()->Guild_GetMemInfoNum();
					for(INT i = 0; i<iMemNum; i++)
					{
						if(CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_Guid == pReturn->m_GUID)
						{//改变职位
							CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->CleanUp();
							break;
						}
					}
					_snprintf(szText, _MAX_PATH, "你拒绝了%s的申请", pReturn->m_DestName);

					//刷新界面
					CEventSystem::GetMe()->PushEvent(GE_GUILD_UPDATE_MEMBERINFO);
				}
			}
			break;
		case GUILD_RETURN_LEAVE:
			{
				//在数据池中查找这个玩家,改变职位
				if(pReturn->m_GUID == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{//是自己退出帮会
					CDataPool::GetMe()->Guild_ClearMemInfo();
					CDataPool::GetMe()->Guild_ClearDetailInfo();

					_snprintf(szText, _MAX_PATH, "你已离开帮会");
					//刷新界面
					CEventSystem::GetMe()->PushEvent(GE_GUILD_FORCE_CLOSE);
				}
				else
				{//其他帮众看到某人退出帮会
					INT iMemNum = CDataPool::GetMe()->Guild_GetMemInfoNum();
					for(INT i = 0; i<iMemNum; i++)
					{
						if(CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_Guid == pReturn->m_GUID)
						{//改变职位
							CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->CleanUp();
							break;
						}
					}
					
					//帮会频道
					//
					//刷新界面
					CEventSystem::GetMe()->PushEvent(GE_GUILD_UPDATE_MEMBERINFO);
				}
			}
			break;
		case GUILD_RETURN_RECRUIT:
			{
				STRING strTemp = "";
				//在数据池中查找这个玩家,改变职位
				INT iMemNum = CDataPool::GetMe()->Guild_GetMemInfoNum();
				for(INT i = 0; i<iMemNum; i++)
				{
					if(CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_Guid == pReturn->m_GUID)
					{//改变职位
						CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_bPosition = (BYTE)GUILD_POSITION_MEMBER;
						break;
					}
				}

				CDataPool::GetMe()->Guild_SetMaxMemNum(CDataPool::GetMe()->Guild_GetMaxMemNum()+1);

				//刷新界面
				CEventSystem::GetMe()->PushEvent(GE_GUILD_UPDATE_MEMBERINFO);

				//被接收人
				if(pReturn->m_GUID == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{
					_snprintf(szText, _MAX_PATH, "欢迎加入%s,大家齐心协力振兴我帮", pReturn->m_GuildName);
				}

				//接收人
				if(pReturn->m_GUIDChanged == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{
					_snprintf(szText, _MAX_PATH, "新帮众%s加入成功", pReturn->m_DestName);
				}

				strTemp = COLORMSGFUNC("Guild_New_Member", pReturn->m_DestName, pReturn->m_GuildName);
				//添加帮会聊天消息
				ADD_GUILD_MSG(strTemp);
			}
			break;
		case GUILD_RETURN_FOUND:
			{
				_snprintf(szText, _MAX_PATH, "帮会正式成立。");
			}
			break;
		case GUILD_RETURN_DISMISS:
			{
				_snprintf(szText, _MAX_PATH, "帮会解散。");
			}
			break;
		case GUILD_RETURN_PROMOTE:
			{
				//输出字符串
				//在数据池中查找这个玩家,改变职位
				INT iMemNum = CDataPool::GetMe()->Guild_GetMemInfoNum();
				for(INT i = 0; i<iMemNum; i++)
				{
					if(CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_Guid == pReturn->m_GUID)
					{//改变职位
						CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_bPosition = (BYTE)pReturn->m_PosID;
						break;
					}
				}

				//你的帮会权限改变了。
				if(pReturn->m_GUID == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{
					_snprintf(szText, _MAX_PATH, "你的帮会权限改变了");
				}
				else
				{
					_snprintf(szText, _MAX_PATH, "任命成功");
				}

				CEventSystem::GetMe()->PushEvent(GE_GUILD_UPDATE_MEMBERINFO);
			}
			break;
		case GUILD_RETURN_DEMOTE:
			{
				//输出字符串
				//在数据池中查找这个玩家,改变职位
				INT iMemNum = CDataPool::GetMe()->Guild_GetMemInfoNum();
				for(INT i = 0; i<iMemNum; i++)
				{
					if(CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_Guid == pReturn->m_GUID)
					{//改变职位
						CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_bPosition = (BYTE)pReturn->m_PosID;
						break;
					}
				}

				//你的帮会权限改变了。
				if(pReturn->m_GUID == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{
					_snprintf(szText, _MAX_PATH, "你的帮会权限改变了");
				}
				else
				{
					_snprintf(szText, _MAX_PATH, "任命成功");
				}

				
				CEventSystem::GetMe()->PushEvent(GE_GUILD_UPDATE_MEMBERINFO);
			}
			break;
		case GUILD_RETURN_AUTHORIZE:
		case GUILD_RETURN_DEPRIVE_AUTHORITY:
			{

			}
			break;
		case GUILD_RETURN_DEMISE:
			{
				if(pReturn->m_GUIDChanged == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{
					//你的权限改变了，恭喜你成为帮会帮主。
					_snprintf(szText, _MAX_PATH, "恭喜你成为帮会帮主");
					break;
				}

				if(pReturn->m_GUID == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
				{
					//操作成功你的权限改变了
					_snprintf(szText, _MAX_PATH, "操作成功");
					break;
				}

				INT iMemNum = CDataPool::GetMe()->Guild_GetMemInfoNum();
				for(INT i = 0; i<iMemNum; i++)
				{
					if(CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_Guid == pReturn->m_GUIDChanged)
					{//改变职位
						CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_bPosition = GUILD_POSITION_CHIEFTAIN;
					}

					if(CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_Guid == pReturn->m_GUID)
					{//改变职位
						CDataPool::GetMe()->Guild_GetMemInfoByIndex(i)->m_bPosition = GUILD_POSITION_MEMBER;
					}
				}

				CEventSystem::GetMe()->PushEvent(GE_GUILD_UPDATE_MEMBERINFO);
			}
			break;
		case GUILD_RETURN_WITHDRAW:
		case GUILD_RETURN_DEPOSIT:
		default:
			Assert(FALSE);
			return 0;
		}

		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, szText );
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}

