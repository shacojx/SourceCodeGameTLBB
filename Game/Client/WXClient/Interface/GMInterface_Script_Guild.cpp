#include "StdAfx.h"
#include "TDException.h"
#include "TDTimeSystem.h"
#include "TDEventDefine.h"
#include "TDUISystem.h"
#include "TDUtil.h"
#include "LuaPlus.h"
#include "../../../Common/DB_Struct.h"
#include "../../../Common/GameDefine_Guild.h"
#include "../../../Common/GameStruct_Guild.h"
#include "..\Network\NetManager.h"
#include "../DBC/GMDataBase.h"
#include "../DataPool/GMDataPool.h"
#include "../DataPool/GMDP_Struct_Guild.h"
#include "../DataPool/GMDP_CharacterData.h"
#include "../Object/Character/Obj_Character.h"
#include "../Object/Character/Obj_PlayerNPC.h"
#include "../Object/Character/Obj_PlayerMySelf.h"
#include "../Object/ObjectManager.h"
#include "../Event/GMEventSystem.h"
#include "../Procedure/GameProcedure.h"

#include "../Common/Packets/CGGuild.h"
#include "../Common/Packets/CGGuildApply.h"
#include "../Common/Packets/CGGuildJoin.h"

#include "GMInterface_Script_Guild.h"

namespace SCRIPT_SANDBOX
{
	Guild Guild::s_Guild;
	LuaPlus::LuaObject* Guild::s_pMetaTable = NULL;

	enum
	{
		ERR_GUILD_ALREADYIN_MSG = 0,	//玩家已经在一个帮会中
		ERR_GUILD_NOTIN_MSG,			//玩家不在帮会中
		ERR_GUILD_NOPOW_MSG,			//你的权限不够
		ERR_GUILD_NOTHAVEASSCHIEF_MSG,	//没有副帮主

		ERR_GUILD_CREATE_LEVEL_TOO_LOW,	//创建等级过低
		ERR_GUILD_NAME_EMPTY,			//帮会名为空
		ERR_GUILD_NAME_INVALID,			//名字里含有敏感词
		ERR_GUILD_NAME_CANTUSE,			//名字是完全过滤表中的词
		ERR_GUILD_DESC_EMPTY,			//描述为空
		ERR_GUILD_DESC_INVALID,			//描述里含有敏感词
		ERR_GUILD_MONEY_NOT_ENOUGH,		//建立帮会金钱不足

		ERR_GUILD_JOIN_LEVEL_TOO_LOW,	//加入等级过低
		ERR_GUILD_POW_NORECRUIT,		//没有权限收人
		ERR_GUILD_POW_NOEXPEL,			//没有权限踢人
	};

	Guild::Guild()
	{
		m_MsgArray.resize(0);
		m_iMemberBak = -1;
	}

	Guild::~Guild()
	{
	}

	INT Guild::CreateGuild(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);

		if(!(args[2].IsString()))
		{
			TDThrow("LUA: Guild CreateGuild Wrong Param1");
			return -1;
		}

		if(!(args[3].IsString()))
		{
			TDThrow("LUA: Guild CreateGuild Wrong Param2");
			return -1;
		}

		STRING szGuildName = args[2].GetString();
		STRING szGuildDesc = args[3].GetString();

		if(!szGuildName.empty() && !szGuildDesc.empty())
		{
			//帮会创建资格检查
			if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Level()<40)
			{
				ShowMsg(ERR_GUILD_CREATE_LEVEL_TOO_LOW);
				state->PushInteger(-1);
				return 1;
			}

			if(INVALID_ID == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild())
			{
				//完全匹配过滤
				if(!CGameProcedure::s_pUISystem->CheckStringFullCompare(szGuildName, "guild"))
				{
					ShowMsg(ERR_GUILD_NAME_CANTUSE);
					state->PushInteger(-1);
					return 1;
				}
                
				//敏感字符过滤
				if(!CGameProcedure::s_pUISystem->CheckStringFilter(szGuildName))
				{
					ShowMsg(ERR_GUILD_NAME_INVALID);
					state->PushInteger(-1);
					return 1;
				}

				//敏感字符过滤
				if(!CGameProcedure::s_pUISystem->CheckStringFilter(szGuildDesc))
				{
					ShowMsg(ERR_GUILD_DESC_INVALID);
					state->PushInteger(-1);
					return 1;
				}

				//名称非法字符过滤
				if(!TDU_CheckStringValid(szGuildName.c_str()))
				{
					ShowMsg(ERR_GUILD_NAME_INVALID);
					state->PushInteger(-1);
					return 1;
				}

				m_MsgArray.push_back(szGuildName);
				m_MsgArray.push_back(szGuildDesc);

				//检测通过
				state->PushInteger(1);
				//显示确认框
				CEventSystem::GetMe()->PushEvent(GE_GUILD_CREATE_CONFIRM, szGuildName.c_str());
			}
			else
			{
				//已经拥有一个帮会MSG
				ShowMsg(ERR_GUILD_ALREADYIN_MSG);
				state->PushInteger(-1);
			}
		}
		else
		{
			if(szGuildName.empty())
			{
				ShowMsg(ERR_GUILD_NAME_EMPTY);
				state->PushInteger(-1);
			}

			if(szGuildDesc.empty())
			{
				ShowMsg(ERR_GUILD_DESC_EMPTY);
				state->PushInteger(-1);
			}
		}

		return 1;
	}

	INT Guild::CreateGuildConfirm(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			TDThrow("LUA: Guild CreateGuildConfirm Wrong Param1");
			return -1;
		}

		INT confirmId = args[2].GetInteger();
		if(1 == confirmId && m_MsgArray.size() == 2) //create
		{
			
			if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Money()<500000)
			{
				ShowMsg(ERR_GUILD_MONEY_NOT_ENOUGH);
				return 0;
			}

			//发送创建帮会信息包
			CGGuildApply pk;

			pk.SetGuildNameSize((BYTE)m_MsgArray[0].size());
			pk.SetGuildName((CHAR*)m_MsgArray[0].c_str());
			pk.SetGuildDescSize((BYTE)m_MsgArray[1].size());
			pk.SetGuildDesc((CHAR*)m_MsgArray[1].c_str());
			
			CNetManager::GetMe()->SendPacket(&pk);
		}
		else if(confirmId == 2)	//destory
		{
			//发送帮会删除包
			CGGuild ck;
			ck.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_DISMISS;

			CNetManager::GetMe()->SendPacket(&ck);
		}
		else if(confirmId == 3) //quit
		{
			//发送退出帮会包
			CGGuild dk;
			
			dk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_LEAVE;
			GUILD_CGW_LEAVE* pLeave = (GUILD_CGW_LEAVE*)dk.GetGuildPacket()->GetPacket(GUILD_PACKET_CG_LEAVE);

			if(pLeave)
			{
				CNetManager::GetMe()->SendPacket(&dk);
			}
		}

		m_MsgArray.clear();
		return 0;
	}

	INT Guild::AskGuildDetailInfo(LuaPlus::LuaState* state)
	{
		if(INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild())
		{
			CGGuild pk;

			pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_ASKINFO;

			GUILD_CGW_ASKINFO* pAskInfo = (GUILD_CGW_ASKINFO*)pk.GetGuildPacket()->GetPacket(GUILD_PACKET_CG_ASKINFO);
			if(pAskInfo)
			{
				pAskInfo->m_GuildGUID = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild();
				pAskInfo->m_Type = GUILD_CGW_ASKINFO::GUILD_INFO;
				CNetManager::GetMe()->SendPacket(&pk);
			}
		}
		else
		{
			//不在帮会MSG
			ShowMsg(ERR_GUILD_NOTIN_MSG);
		}

		return 0;
	}

	INT Guild::AskGuildMembersInfo(LuaPlus::LuaState* state)
	{
		if(INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild())
		{
			CGGuild pk;

			pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_ASKINFO;

			GUILD_CGW_ASKINFO* pAskInfo = (GUILD_CGW_ASKINFO*)pk.GetGuildPacket()->GetPacket(GUILD_PACKET_CG_ASKINFO);
			if(pAskInfo)
			{
				pAskInfo->m_GuildGUID = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild();
				pAskInfo->m_Type = GUILD_CGW_ASKINFO::GUILD_MEMBER_INFO;
				CNetManager::GetMe()->SendPacket(&pk);
			}
		}
		else
		{
			//不在帮会MSG
			ShowMsg(ERR_GUILD_NOTIN_MSG);
		}

		return 0;
	}

	INT Guild::AskGuildAppointPosInfo(LuaPlus::LuaState* state)
	{

		LuaPlus::LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			TDThrow("LUA: Guild AskGuildAppointPosInfo Wrong Param1");
			return -1;
		}
		
		if(INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild())
		{
			//客户端玩家权限检查等

			CGGuild pk;
			pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_ASKINFO;

			GUILD_CGW_ASKINFO* pAskInfo = (GUILD_CGW_ASKINFO*)pk.GetGuildPacket()->GetPacket(GUILD_PACKET_CG_ASKINFO);
			if(pAskInfo)
			{
				pAskInfo->m_GuildGUID = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild();
				pAskInfo->m_Type = GUILD_CGW_ASKINFO::GUILD_APPOINT_POS;
				CNetManager::GetMe()->SendPacket(&pk);
			}

			m_iMemberBak = args[2].GetInteger();
		}
		else
		{
			//不在帮会MSG
			ShowMsg(ERR_GUILD_NOTIN_MSG);
		}
		return 0;
	}

	INT Guild::ChangeGuildLeader(LuaPlus::LuaState* state)
	{
		return 0;
	}

	INT Guild::DestoryGuild(LuaPlus::LuaState* state)
	{
		if(INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild())
		{
			//客户端玩家权限检查等
			if(CDataPool::GetMe()->Guild_GetCurPosition() == GUILD_POSITION_CHIEFTAIN)
			{
				CEventSystem::GetMe()->PushEvent(GE_GUILD_DESTORY_CONFIRM, CDataPool::GetMe()->Guild_GetName());
			}
		}
		else
		{
			//不在帮会MSG
			ShowMsg(ERR_GUILD_NOTIN_MSG);
		}
		return 0;		
	}

	INT Guild::FixGuildInfo(LuaPlus::LuaState* state)
	{
		return 0;
	}

	INT Guild::AdjustMemberAuth(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			m_iMemberBak = -1;
			TDThrow("LUA: Guild AdjustMemberAuth Wrong Param1");
			return -1;
		}
		INT lidx = args[2].GetInteger();

		if(INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild())
		{
			//客户端玩家权限检查等
			if(CDataPool::GetMe()->Guild_GetCurAccess()&GUILD_AUTHORITY_ASSIGN)
			{
				if(lidx >= 0 && CDataPool::GetMe()->Guild_GetAppointInfoByIndex(lidx) && m_iMemberBak >= 0)
				{
					GuildAppointPos_t* pAppoint = CDataPool::GetMe()->Guild_GetAppointInfoByIndex(lidx);

					CGGuild pk;
					pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_APPOINT;
					GUILD_CGW_APPOINT* pAuth = (GUILD_CGW_APPOINT*)pk.GetGuildPacket()->GetPacket(GUILD_PACKET_CG_APPOINT);
					
					if(pAuth && pAppoint && CDataPool::GetMe()->Guild_GetMemInfoByIndex(m_iMemberBak))
					{
						//发送职位变更包
						pAuth->m_GuildGUID = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild();
						pAuth->m_CandidateGUID = CDataPool::GetMe()->Guild_GetMemInfoByIndex(m_iMemberBak)->m_Guid;
						pAuth->m_NewPosition = (UCHAR)pAppoint->m_PosID;

						CNetManager::GetMe()->SendPacket(&pk);
					}
				}
			}
			else
			{
				//权限不够
				ShowMsg(ERR_GUILD_NOPOW_MSG);
			}
		}
		else
		{
			//不在帮会MSG
			ShowMsg(ERR_GUILD_NOTIN_MSG);
		}

		m_iMemberBak = -1;
		return 0;
	}

	INT Guild::GetGuildInfo(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			TDThrow("LUA: Guild GetGuildInfo Wrong Param1");
			return -1;
		}

		if(!(args[3].IsString()))
		{
			TDThrow("LUA: Guild GetGuildInfo Wrong Param2");
			return -1;
		}

		INT idx = args[2].GetInteger();
		STRING strOp = args[3].GetString();

		if(idx >= CDataPool::GetMe()->Guild_GetInfoNum()) return 0;
		if(strOp.empty()) return 0;

		GuildInfo_t* pGuild = CDataPool::GetMe()->Guild_GetInfoByIndex(idx);

		if(pGuild && pGuild->m_GuildID != INVALID_ID)
		{
			if(strOp == "ID")
			{
				state->PushInteger(pGuild->m_GuildID);
			}
			else if(strOp == "ChiefName")
			{
				state->PushString(pGuild->m_ChiefName.c_str());
			}
			else if(strOp == "Name")
			{
				state->PushString(pGuild->m_GuildName.c_str());
			}
			else if(strOp == "Desc")
			{
				state->PushString(pGuild->m_GuildDesc.c_str());
			}
			else if(strOp == "CityName")
			{
				state->PushString(pGuild->m_CityName.c_str());
			}
			else if(strOp == "Status")
			{
				state->PushInteger((INT)pGuild->m_uGuildStatus);
			}
			else if(strOp == "Level")
			{
				state->PushInteger((INT)pGuild->m_bGuildLevel);
			}
			else if(strOp == "FoundTime")
			{
				STRING strTime;
				ConvertServerTime(pGuild->m_nFoundedTime, strTime);

				state->PushString(strTime.c_str());
			}
			else if(strOp == "Count")
			{
				state->PushInteger((INT)pGuild->m_uGuildUserCount);
			}
		}
		else
		{
			return 0;
		}

		return 1;
	}

	INT Guild::GetGuildNum(LuaPlus::LuaState* state)
	{
		state->PushInteger(CDataPool::GetMe()->Guild_GetInfoNum());
		return 1;
	}

	INT Guild::GetMembersInfo(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			TDThrow("LUA: Guild GetMembersInfo Wrong Param1");
			return -1;
		}
		INT idx = args[2].GetInteger();

		if(!(args[3].IsString()))
		{
			TDThrow("LUA: Guild GetMembersInfo Wrong Param2");
			return -1;
		}
		STRING strOp = args[3].GetString();

		GuildMemberInfo_t* pMember = CDataPool::GetMe()->Guild_GetMemInfoByIndex(idx);
		if(pMember && strOp == "GUID")
		{
			state->PushInteger((INT)pMember->m_Guid);
		}
		else
		if(pMember && pMember->m_Guid != INVALID_ID)
		{
			if(strOp == "Name")
			{
				state->PushString(pMember->m_szName.c_str());
			}
			else if(strOp == "Level")
			{
				state->PushInteger(pMember->m_bLevel);
			}
			else if(strOp == "MenPai")
			{
				state->PushInteger(pMember->m_bMenPaiID);
			}
			else if(strOp == "CurCon")
			{
				state->PushInteger(pMember->m_iCurContribute);
			}
			else if(strOp == "MaxCon")
			{
				state->PushInteger(pMember->m_iMaxContribute);
			}
			else if(strOp == "JoinTime")
			{
				STRING strTime;
				ConvertServerTime(pMember->m_iJoinTime, strTime);

				state->PushString(strTime.c_str());
			}
			else if(strOp == "LogOutTime")
			{
				STRING strTime;
				ConvertServerTime(pMember->m_iLogOutTime, strTime);

				state->PushString(strTime.c_str());
			}
			else if(strOp == "Position")
			{
				state->PushInteger(pMember->m_bPosition);
			}
			else if(strOp == "Online")
			{
				state->PushInteger(pMember->m_bIsOnline);
			}
			else if(strOp == "ShowColor")
			{
				state->PushString(GetShowColor_For_Lua(idx).c_str());
			}
		}

		return 1;
	}

	INT Guild::GetMyGuildInfo(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);

		if(!(args[2].IsString()))
		{
			TDThrow("LUA: Guild GetMyGuildInfo Wrong Param1");
			return -1;
		}
		STRING strOp = args[2].GetString();

		if(INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild())
		{
			if(strOp == "Name")
			{
				state->PushString(CDataPool::GetMe()->Guild_GetName());
			}
			else if(strOp == "Desc")
			{
				state->PushString(CDataPool::GetMe()->Guild_GetDesc());
			}
			else if(strOp == "Appoint")
			{
				if(!(args[3].IsInteger()))
				{
					TDThrow("LUA: Guild GetMyGuildInfo Wrong Param2");
					return -1;
				}
				INT idx = args[3].GetInteger();

				GuildAppointPos_t* pAppoint = CDataPool::GetMe()->Guild_GetAppointInfoByIndex(idx);
				if(pAppoint && pAppoint->m_PosID != -1)
				{
					state->PushString(pAppoint->m_szPosName.c_str());
				}
			}
		}
		return 1;
	}

	INT Guild::GetMyGuildDetailInfo(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if(!(args[2].IsString()))
		{
			TDThrow("LUA: Guild GetMyGuildDetailInfo Wrong Param1");
			return -1;
		}
		STRING strOp = args[2].GetString();

		if(INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild())
		{
			GuildDetailInfo_t* pDetail = CDataPool::GetMe()->Guild_GetDetailInfo();
			if(pDetail)
			{
				if(strOp == "Name")
				{
					state->PushString(pDetail->m_GuildName.c_str());
				}
				else if(strOp == "Creator")
				{
					state->PushString(pDetail->m_GuildCreator.c_str());
				}
				else if(strOp == "ChiefName")
				{
					state->PushString(pDetail->m_GuildChairMan.c_str());
				}
				else if(strOp == "CityName")
				{
					state->PushString(pDetail->m_CityName.c_str());
				}
				else if(strOp == "Level")
				{
					state->PushInteger((INT)pDetail->m_nLevel);
				}
				else if(strOp == "Scene")
				{
					state->PushInteger(pDetail->m_nPortSceneID);
				}
				else if(strOp == "MemNum")
				{
					state->PushInteger(pDetail->m_MemNum);
				}
				else if(strOp == "Lon")
				{
					state->PushInteger(pDetail->m_Longevity);
				}
				else if(strOp == "Con")
				{
					state->PushInteger(pDetail->m_Contribute);
				}
				else if(strOp == "Honor")
				{
					state->PushInteger(pDetail->m_Honor);
				}
				else if(strOp == "FMoney")
				{
					state->PushInteger(pDetail->m_FoundedMoney);
				}
				else if(strOp == "Ind")
				{
					state->PushInteger(pDetail->m_nIndustryLevel);
				}
				else if(strOp == "Agr")
				{
					state->PushInteger(pDetail->m_nAgrLevel);
				}
				else if(strOp == "Com")
				{
					state->PushInteger(pDetail->m_nComLevel);
				}
				else if(strOp == "Def")
				{
					state->PushInteger(pDetail->m_nDefLevel);
				}
				else if(strOp == "Tech")
				{
					state->PushInteger(pDetail->m_nTechLevel);
				}
				else if(strOp == "Ambi")
				{
					state->PushInteger(pDetail->m_nAmbiLevel);
				}
			}// end of if(pDetail)
		}
		return 1;
	}

	INT Guild::GetMyGuildPower(LuaPlus::LuaState* state)
	{
		const char* szOk = "1";
		const char* szFalse = "0";

		STRING strR = "";
		BYTE pow = CDataPool::GetMe()->Guild_GetCurAccess();
		
		strR += (pow&GUILD_AUTHORITY_ASSIGN)?szOk:szFalse;
		strR += (pow&GUILD_AUTHORITY_AUTHORIZE)?szOk:szFalse;
		strR += (pow&GUILD_AUTHORITY_RECRUIT)?szOk:szFalse;
		strR += (pow&GUILD_AUTHORITY_EXPEL)?szOk:szFalse;
		strR += (pow&GUILD_AUTHORITY_DEMISE)?szOk:szFalse;
		strR += (pow&GUILD_AUTHORITY_WITHDRAW)?szOk:szFalse;
		strR += (pow&GUILD_AUTHORITY_DEPOSIT)?szOk:szFalse;
		strR += (pow&GUILD_AUTHORITY_LEAVE)?szOk:szFalse;

        state->PushString(strR.c_str());

		return 1;
	}

	INT Guild::GetMembersNum(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			TDThrow("LUA: Guild GetMembersNum Wrong Param1");
			return -1;
		}
		INT sel = args[2].GetInteger();

		enum
		{
			TOTAL_NUMBERS = 1,	//全部帮众，这个数值包含已经被踢出的原帮众的信息
			MAX_NUMBERS,		//最多帮众，不同帮会能包含的最大帮众数是不同的

			REAL_NUMBERS,		//真实帮众，真正的有效的数据，包含申请中的人和正式的人

			SHOW_MEMBERS_NUMBERS,	//Lua显示成员的list控件需要的数量
			SHOW_TRAINEES_NUMBERS,	//Lua显示预备的list控件需要的数量
		};

		INT Num = 0;
		switch(sel) {
		case TOTAL_NUMBERS:
			Num = CDataPool::GetMe()->Guild_GetMemInfoNum();
			break;
		case MAX_NUMBERS:
			Num = CDataPool::GetMe()->Guild_GetMaxMemNum();
			break;
		case REAL_NUMBERS:
			for(INT i = 0; i < CDataPool::GetMe()->Guild_GetMemInfoNum(); ++i)
			{
				GuildMemberInfo_t* pMember = CDataPool::GetMe()->Guild_GetMemInfoByIndex(i);
				if(pMember && pMember->m_Guid != INVALID_ID &&
				   pMember->m_bIsOnline == 1 && pMember->m_bPosition > GUILD_POSITION_TRAINEE)
				Num++;
			}
			break;
		case SHOW_MEMBERS_NUMBERS:
			Num = (INT)m_AllMembers.size();
			break;
		case SHOW_TRAINEES_NUMBERS:
			Num = (INT)m_AllTrainees.size();
			break;
		default:
			Num = -1;
			break;
		}

		state->PushInteger(Num);
		return 1;
	}

	INT Guild::GetShowMembersIdx(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			TDThrow("LUA: Guild GetShowMembersIdx Wrong Param1");
			return -1;
		}
		INT showIdx = args[2].GetInteger();

		if(showIdx < 0 || showIdx >= (INT)(m_AllMembers.size())) return 0;

        state->PushInteger(m_AllMembers[showIdx]);
		return 1;
	}

	INT Guild::GetShowTraineesIdx(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			TDThrow("LUA: Guild GetShowTraineesIdx Wrong Param1");
			return -1;
		}

		INT showIdx = args[2].GetInteger();

		if(showIdx < 0 || showIdx >= (INT)(m_AllTrainees.size())) return 0;

		state->PushInteger(m_AllTrainees[showIdx]);
		return 1;
	}

	INT Guild::JoinGuild(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);

		if(!(args[2].IsInteger()))
		{
			TDThrow("LUA: Guild JoinGuild Wrong Param1");
			return -1;
		}

		INT selGuild = args[2].GetInteger();
		if(selGuild >=0 && selGuild < CDataPool::GetMe()->Guild_GetInfoNum())
		{
			GuildInfo_t* pGuild = CDataPool::GetMe()->Guild_GetInfoByIndex(selGuild);
			if(pGuild && pGuild->m_GuildID != INVALID_ID)
			{
				if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Level()<10)
				{
					ShowMsg(ERR_GUILD_JOIN_LEVEL_TOO_LOW);
					return 0;
				}

				//发送加入帮会的消息包
				CGGuildJoin pk;
				pk.SetGuildID(pGuild->m_GuildID);

				CNetManager::GetMe()->SendPacket(&pk);
			}
		}
		return 0;
	}

	INT Guild::KickGuild(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if(!(args[2].IsInteger()))
		{
			TDThrow("LUA: Guild KickGuild Wrong Param1");
			return -1;
		}

		INT idx = args[2].GetInteger();
		GuildMemberInfo_t* pMember = CDataPool::GetMe()->Guild_GetMemInfoByIndex(idx);
		if(pMember && pMember->m_Guid != INVALID_ID)
		{
			if(CDataPool::GetMe()->Guild_GetCurAccess()&GUILD_AUTHORITY_EXPEL)
			{
				//发送踢出帮会的消息包
				CGGuild pk;
				
				pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_EXPEL;
				GUILD_CGW_EXPEL* pExpel = (GUILD_CGW_EXPEL*)pk.GetGuildPacket()->GetPacket(GUILD_PACKET_CG_EXPEL);
				if(pExpel)
				{
					pExpel->m_GuildUserGUID = pMember->m_Guid;
					CNetManager::GetMe()->SendPacket(&pk);
				}
			}
			else
			{
				//权限不够
				ShowMsg(ERR_GUILD_POW_NOEXPEL);
			}
		}

		return 0;
	}

	INT Guild::RecruitGuild(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if(!(args[2].IsInteger()))
		{
			TDThrow("LUA: Guild RecruitGuild Wrong Param1");
			return -1;
		}

		INT idx = args[2].GetInteger();
		GuildMemberInfo_t* pMember = CDataPool::GetMe()->Guild_GetMemInfoByIndex(idx);
		if(pMember && pMember->m_Guid != INVALID_ID && pMember->m_bPosition == GUILD_POSITION_TRAINEE)
		{
			if(CDataPool::GetMe()->Guild_GetCurAccess()&GUILD_AUTHORITY_RECRUIT)
			{
				//发送招收会员的消息包
				CGGuild pk;

				pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_RECRUIT;
				GUILD_CGW_RECRUIT* pRecruit = (GUILD_CGW_RECRUIT*)pk.GetGuildPacket()->GetPacket(GUILD_PACKET_CG_RECRUIT);
				if(pRecruit)
				{
					pRecruit->m_ProposerGUID = pMember->m_Guid;
					CNetManager::GetMe()->SendPacket(&pk);
				}
			}
			else
			{
				//权限不够
				ShowMsg(ERR_GUILD_POW_NORECRUIT);
			}
		}

		return 0;
	}

	INT Guild::QuitGuild(LuaPlus::LuaState* state)
	{
		if(INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild())
		{
			if(CDataPool::GetMe()->Guild_GetCurAccess()&GUILD_AUTHORITY_LEAVE)
			{
				LPCTSTR pGuildName = CDataPool::GetMe()->Guild_GetName();
				//显示确认框
				CEventSystem::GetMe()->PushEvent(GE_GUILD_QUIT_CONFIRM, pGuildName);
			}
			else
			{
				//权限不够
				ShowMsg(ERR_GUILD_NOPOW_MSG);
			}
		}
		return 0;
	}

	INT Guild::DemisGuild(LuaPlus::LuaState* state)
	{
		//是否有帮会
		if(INVALID_ID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Guild())
		{
			//是否是帮主
			if(CDataPool::GetMe()->Guild_GetCurPosition() == GUILD_POSITION_CHIEFTAIN)
			{
				BOOL bAss = FALSE;
                //帮里是否有副帮主
				for(INT i = 0; i < CDataPool::GetMe()->Guild_GetMemInfoNum(); ++i)
				{
					GuildMemberInfo_t* pMember = CDataPool::GetMe()->Guild_GetMemInfoByIndex(i);
					if(pMember && pMember->m_Guid != INVALID_ID)
					{
						if(pMember->m_bPosition == GUILD_POSITION_ASS_CHIEFTAIN)
						{
							bAss = TRUE;
							break;
						}
					}
				}

				if(bAss && CDataPool::GetMe()->Guild_GetCurAccess()&GUILD_AUTHORITY_DEMISE)
				{
					//发送禅让包
					CGGuild pk;
					pk.GetGuildPacket()->m_uPacketType = GUILD_PACKET_CG_DEMISE;
					GUILD_CGW_DEMISE* pDemis = (GUILD_CGW_DEMISE*)pk.GetGuildPacket()->GetPacket(GUILD_PACKET_CG_DEMISE);

					if(pDemis)
					{
						CNetManager::GetMe()->SendPacket(&pk);
					}
				}

				//显示错误信息
				if(!bAss)
				{
					ShowMsg(ERR_GUILD_NOTHAVEASSCHIEF_MSG);
				}

				if(!(CDataPool::GetMe()->Guild_GetCurAccess()&GUILD_AUTHORITY_DEMISE))
				{
					ShowMsg(ERR_GUILD_NOPOW_MSG);
				}
			}
		}
		return 0;
	}

	INT	Guild::PrepareMembersInfomation(LuaPlus::LuaState* state)
	{
		PerpareMembersBeforeShow();
		return 0;
	}

	VOID Guild::ShowMsg(INT msgType, BOOL bTalk, BOOL bTip)
	{
		STRING strMsg = "";

		switch(msgType) {
		case ERR_GUILD_ALREADYIN_MSG:
			strMsg = "请先离开你现在的帮会";//NOCOLORMSGFUNC("GMGameInterface_Script_Guild_AlreadyIn");
			break;
		case ERR_GUILD_NOTIN_MSG:
			strMsg = "你还没有加入帮会";//NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Not_Guild_Member");
			break;
		case ERR_GUILD_NOPOW_MSG:
			strMsg = "你没有权限进行这种操作";//NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Pow_CantDo");
			break;
		case ERR_GUILD_NOTHAVEASSCHIEF_MSG:
			strMsg = "任命了副帮主才可以进行禅让操作";//NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Change_Demiss_No_Assistant_Chief");
			break;
		case ERR_GUILD_CREATE_LEVEL_TOO_LOW:
			strMsg = "40级以上才能申请建帮";//NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Level_Too_Low_To_Create");
			break;
		case ERR_GUILD_NAME_EMPTY:
			strMsg = "请输入帮会名";//NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Name_Empty_Create");
			break;
		case ERR_GUILD_NAME_INVALID:
			strMsg = NOCOLORMSGFUNC("ERRORSpecialString");
			break;
		case ERR_GUILD_NAME_CANTUSE:
			strMsg = "帮会名称重复";//NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Name_CantUse_Create");
			break;
		case ERR_GUILD_DESC_EMPTY:
			strMsg = "请输入帮会宗旨";//NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Desc_Empty_Create");
			break;
		case ERR_GUILD_DESC_INVALID:
			strMsg = NOCOLORMSGFUNC("ERRORSpecialString");
			break;
		case ERR_GUILD_MONEY_NOT_ENOUGH:
			strMsg = "你身上的金钱不足以支付建帮费";//NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Money_Too_Low_To_Create");
			break;
		case ERR_GUILD_JOIN_LEVEL_TOO_LOW:
			strMsg = "10级后才能申请加入帮会";//NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Level_Too_Low_To_Join");
			break;
		case ERR_GUILD_POW_NORECRUIT:
			strMsg = "你没有权限接收新帮众";//NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Pow_No_Recruit");
			break;
		case ERR_GUILD_POW_NOEXPEL:
			strMsg = "你没有权限拒绝接收新帮众";//NOCOLORMSGFUNC("GMGameInterface_Script_Guild_Pow_No_Expel");
			break;
		default:
			break;
		}
		
		m_MsgArray.clear();

		if(strMsg.empty()) return;
		if(!bTalk && !bTip) return;

		if(bTalk)
		{
			ADDTALKMSG(strMsg);
		}
		
		if(bTip)
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strMsg.c_str());
		}
	}

	VOID Guild::ConvertServerTime(INT dTime, STRING& strTime)
	{
		INT min,hour,day,month,year;

		min = dTime % 100;
		dTime /= 100;

		hour = dTime % 100;
		dTime /= 100;

		day = dTime % 100;
		dTime /= 100;

		month = dTime % 100;
		dTime /= 100;

		//year = 2000 + dTime % 100;
		year = dTime % 100;
		dTime /= 100;

		//CHAR szFormat[] = "%d年%d月%d日 %d:%d";
		//CHAR szFormat[] = "%d年%d月%d日";
		CHAR szFormat[] = "%02d-%02d-%02d";
		CHAR szDate[32] = {0};
		//_snprintf(szDate,sizeof(szDate), szFormat, year,month,day,hour,min);
		//_snprintf(szDate,sizeof(szDate), szFormat, year,month,day);
		_snprintf(szDate,sizeof(szDate),szFormat, month,day,year);

		strTime = szDate;
	}

	STRING Guild::GetShowColor_For_Lua(INT idx)
	{
		STRING strColor;

		GuildMemberInfo_t* pMember = CDataPool::GetMe()->Guild_GetMemInfoByIndex(idx);
		if(pMember && pMember->m_Guid != INVALID_ID)
		{
			switch(pMember->m_bPosition) {
			case GUILD_POSITION_CHIEFTAIN:
			case GUILD_POSITION_ASS_CHIEFTAIN:
			case GUILD_POSITION_ELITE_MEMBER:
			case GUILD_POSITION_COM:
			case GUILD_POSITION_AGRI:
			case GUILD_POSITION_INDUSTRY:
			case GUILD_POSITION_HR:
				strColor = (pMember->m_bIsOnline > 0)?m_LuaShowColors.m_OnlineLeaderColor:m_LuaShowColors.m_LeaveLeaderColor;
				break;
			case GUILD_POSITION_TRAINEE:
				strColor = (pMember->m_bIsOnline > 0)?m_LuaShowColors.m_OnlineMemberColor:m_LuaShowColors.m_LeaveMemberColor;
				break;
			case GUILD_POSITION_MEMBER:
				strColor = (pMember->m_bIsOnline > 0)?m_LuaShowColors.m_OnlineTraineeColor:m_LuaShowColors.m_LeaveTraineeColor;
				break;
			default:
				break;
			}
		}

		return strColor;
	}

	VOID Guild::PerpareMembersBeforeShow()
	{
		MEMBER_TABLE	onlineLeaders;		//在线领导
		MEMBER_TABLE	leaveLeaders;		//离线领导

		MEMBER_TABLE	onlineMembers;		//在线成员
		MEMBER_TABLE	leaveMembers;		//离线成员

		MEMBER_TABLE	onlineTrainees;		//在线预备成员
		MEMBER_TABLE	leaveTrainees;		//离线预备成员

		//clear
		m_AllMembers.clear();
		m_AllTrainees.clear();

		for(INT i = 0; i < CDataPool::GetMe()->Guild_GetMemInfoNum(); ++i)
		{
			GuildMemberInfo_t* pMember = CDataPool::GetMe()->Guild_GetMemInfoByIndex(i);
			if(pMember && pMember->m_Guid != INVALID_ID)
			{
				Name_Idx curMember;
				curMember.m_MemberIdx = i;
				curMember.m_MemberName = pMember->m_szName;
				curMember.m_Position = pMember->m_bPosition;

				switch(pMember->m_bPosition) 
				{
				case GUILD_POSITION_CHIEFTAIN:
				case GUILD_POSITION_ASS_CHIEFTAIN:
				case GUILD_POSITION_ELITE_MEMBER:
				case GUILD_POSITION_COM:
				case GUILD_POSITION_AGRI:
				case GUILD_POSITION_INDUSTRY:
				case GUILD_POSITION_HR:
					(pMember->m_bIsOnline > 0)?onlineLeaders.push_back(curMember):leaveLeaders.push_back(curMember);
					break;
				case GUILD_POSITION_TRAINEE:
					(pMember->m_bIsOnline > 0)?onlineTrainees.push_back(curMember):leaveTrainees.push_back(curMember);
					break;
				case GUILD_POSITION_MEMBER:
					(pMember->m_bIsOnline > 0)?onlineMembers.push_back(curMember):leaveMembers.push_back(curMember);
					break;
				default:
					break;
				}
			}
		}

		//领导排序
		std::stable_sort(onlineLeaders.begin(), onlineLeaders.end(), SimpleCmpFunc::less_strcmp);
		std::stable_sort(onlineLeaders.begin(), onlineLeaders.end(), SimpleCmpFunc::less_position);

		std::stable_sort(leaveLeaders.begin(), leaveLeaders.end(), SimpleCmpFunc::less_strcmp);
		std::stable_sort(leaveLeaders.begin(), leaveLeaders.end(), SimpleCmpFunc::less_position);

		//正式帮众排序
		std::stable_sort(onlineMembers.begin(), onlineMembers.end(), SimpleCmpFunc::less_strcmp);
		std::stable_sort(leaveMembers.begin(), leaveMembers.end(), SimpleCmpFunc::less_strcmp);

		//生成所有成员表
		//0.在线领导
		for(UINT i=0; i<onlineLeaders.size(); ++i)
		{
			m_AllMembers.push_back(onlineLeaders.at(i).m_MemberIdx);
		}
		//1.在线成员
		for(UINT i=0; i<onlineMembers.size(); ++i)
		{
			m_AllMembers.push_back(onlineMembers.at(i).m_MemberIdx);
		}
		//2.离线领导
		for(UINT i=0; i<leaveLeaders.size(); ++i)
		{
			m_AllMembers.push_back(leaveLeaders.at(i).m_MemberIdx);
		}
		//3.离线成员
		for(UINT i=0; i<leaveMembers.size(); ++i)
		{
			m_AllMembers.push_back(leaveMembers.at(i).m_MemberIdx);
		}

		//预备帮众排序
		std::stable_sort(onlineTrainees.begin(), onlineTrainees.end(), SimpleCmpFunc::less_strcmp);
		std::stable_sort(leaveTrainees.begin(), leaveTrainees.end(), SimpleCmpFunc::less_strcmp);

		//生成所有预备成员表
		//0.在线预备成员
		for(UINT i=0; i<onlineTrainees.size(); ++i)
		{
			m_AllTrainees.push_back(onlineTrainees.at(i).m_MemberIdx);
		}
		//1.离线预备成员
		for(UINT i=0; i<leaveTrainees.size(); ++i)
		{
			m_AllTrainees.push_back(leaveTrainees.at(i).m_MemberIdx);
		}
	}

//  [4/16/2006] SimpleCmpFunc------------------------------------------------------------------
	BOOL SimpleCmpFunc::less_strcmp(const Guild::Name_Idx& s1, const Guild::Name_Idx& s2)
	{
		if( 0 <= strcmp(s1.m_MemberName.c_str(),s2.m_MemberName.c_str()))
			return FALSE;
		else
			return TRUE;
	}

	BOOL SimpleCmpFunc::less_strcoll(const Guild::Name_Idx& s1, const Guild::Name_Idx& s2)
	{
		if( 0 <= strcoll(s1.m_MemberName.c_str(),s2.m_MemberName.c_str()))
			return FALSE;
		else
			return TRUE;
	}

	BOOL SimpleCmpFunc::less_position(const Guild::Name_Idx& s1, const Guild::Name_Idx& s2)
	{
		if(s1.m_Position <= s2.m_Position)
			return FALSE;
		else
			return TRUE;
	}
}