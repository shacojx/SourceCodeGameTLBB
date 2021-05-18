#include "stdafx.h"
#include "GMGameInterface.h"
#include "GMGameInterface_Script.h"
#include "../Procedure/GamePro_Login.h"
#include "../NetWork/NetManager.h"
#include "LuaPlus.h"
#include "../Global.h"
#include "../Event/GMEventSystem.h"
#include "TDException.h"
#include "../Action/GMActionSystem.h"
#include "../DataPool/GMUIDataPool.h"
#include "../DataPool/GMDataPool.h"
#include "../Object/ObjectManager.h"
#include "../Object/Object.h"
#include "../Object/Character/Obj_Character.h"
#include "../Input/GMInputSystem.h"
#include "../world/worldmanager.h"
#include "tddbc_struct.h"
#include "../object/character/obj_playermyself.h"
#include "tdutil.h"
#include "ScriptCommandStruct.h"
#include "CGEventRequest.h"
#include "CGTeamInvite.h"								// 邀请加入队伍消息				
#include "CGTeamRetInvite.h"							// 邀请加入队伍消息返回
#include "CGTeamRetApply.h"								// 回答某人申请加入队伍消息
#include "CGTeamLeaderRetInvite.h"						// 回答队员邀请加入队伍消息
#include "CGReqManualAttr.h"							// 申请手动添加点数.
#include "CGAskTeamFollow.h"
#include "CGReturnTeamFollow.h"
#include "CGStopTeamFollow.h"

#include "CGPlayerDieResult.h"
#include "..\..\Common\database\tlbb_dbc.h"
#include "..\..\Common\BuffImpactMgr.h"
#include "TDDBC_Struct.h"
#include "..\DataPool\GMDP_CharacterData.h"
#include "CGAskLeanAbility.h"
#include "..\DBC\GMDataBase.h"

namespace SCRIPT_SANDBOX
{
	

	//===============================================================
	PlayerMySelf PlayerMySelf::s_MySelf;

	// 得到玩家的名字
	INT	PlayerMySelf::GetName(LuaPlus::LuaState* state)
	{
		state->PushString( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name());
		return 1;
	}
	// 得到角色身上的buff的数量
	//INT PlayerMySelf::GetBuffNumber( LuaPlus::LuaState* state )
	//{
	//	return 1;
	//}
	//INT PlayerMySelf::GetBuffByIndex( LuaPlus::LuaState* state )
	//{
	//	return 1;
	//}
	//得到角色身上的buff的数量
	INT     PlayerMySelf::GetBuffNumber( LuaPlus::LuaState* state )
	{
		CDataPool *pDataPool = CDataPool::GetMe();
		state->PushNumber(pDataPool->BuffImpact_GetCount());
		return 1;
	}

	INT		PlayerMySelf::GetBuffIconNameByIndex( LuaPlus::LuaState* state )
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffIconNameByIndex param parameter error");
		}

		INT nIndex = args[2].GetInteger();
		CDataPool *pDataPool = CDataPool::GetMe();
		const _BUFF_IMPACT_INFO *pBuffImpactInfo = pDataPool->BuffImpact_GetByIndex( nIndex );
		if ( pBuffImpactInfo != NULL )
		{
			_BUFF_IMPACT *pBuffImpact = GetBuffImpactMgr()->GetBuffImpact( pBuffImpactInfo->m_nBuffID );
			if ( pBuffImpact != NULL )
			{
				if ( strlen( pBuffImpact->m_szIconName ) > 0 )
				{
					state->PushString(pBuffImpact->m_szIconName);
					return 1;
				}
				else
				{
					Assert( strlen( pBuffImpact->m_szIconName ) > 0 && "PlayerMySelf::GetBuffIconNameByIndex" );
				}
			}
			else
			{
				Assert( pBuffImpact != NULL > 0 && "PlayerMySelf::GetBuffIconNameByIndex" );
			}
		}
		else
		{
			Assert( pBuffImpactInfo != NULL > 0 && "PlayerMySelf::GetBuffIconNameByIndex" );
		}
		state->PushString("");
		return 1;
	}

	INT		PlayerMySelf::GetBuffToolTipsByIndex( LuaPlus::LuaState* state )
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffToolTipsByIndex param parameter error");
		}

		INT nIndex = args[2].GetInteger();
		CDataPool *pDataPool = CDataPool::GetMe();
		const _BUFF_IMPACT_INFO *pBuffImpactInfo = pDataPool->BuffImpact_GetByIndex( nIndex );
		if ( pBuffImpactInfo != NULL )
		{
			_BUFF_IMPACT *pBuffImpact = GetBuffImpactMgr()->GetBuffImpact( pBuffImpactInfo->m_nBuffID );
			if ( pBuffImpact != NULL )
			{
				if ( strlen( pBuffImpact->m_pszInfo ) > 0 )
				{
					state->PushString( pBuffImpact->m_pszInfo );
					return 1;
				}
				else
				{
					Assert( strlen( pBuffImpact->m_pszInfo ) > 0 && "PlayerMySelf::GetBuffToolTipsByIndex" );
				}
			}
			else
			{
				Assert( pBuffImpact != NULL > 0 && "PlayerMySelf::GetBuffToolTipsByIndex" );
			}
		}
		else
		{
			Assert( pBuffImpactInfo != NULL > 0 && "PlayerMySelf::GetBuffToolTipsByIndex" );
		}
		state->PushString("");
		return 1;
	}
	INT		PlayerMySelf::DispelBuffByIndex( LuaPlus::LuaState* state )
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:DispelBuffByIndex param parameter error");
		}

		INT nIndex = args[2].GetInteger();
		CDataPool *pDataPool = CDataPool::GetMe();
		pDataPool->BuffImpact_DispelByIndex( nIndex );
		return 1;
	}
	INT		PlayerMySelf::GetBuffTimeTextByIndex( LuaPlus::LuaState* state )
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:GetBuffTimeTextByIndex param parameter error");
		}

		INT nIndex = args[2].GetInteger();
		CDataPool *pDataPool = CDataPool::GetMe();
		INT nTime = pDataPool->BuffImpact_GetTimeByIndex( nIndex );
		state->PushInteger(nTime/1000);
		return 1;
	}
	INT		PlayerMySelf::SendReliveMessage_OutGhost( LuaPlus::LuaState* state )
	{
		CGPlayerDieResult msgDieResult;
		msgDieResult.SetResultCode( DIE_RESULT_CODE_OUT_GHOST );
		CNetManager::GetMe()->SendPacket(&msgDieResult);
		return 1;
	}

	INT		PlayerMySelf::SendReliveMessage_Relive( LuaPlus::LuaState* state )
	{
		CGPlayerDieResult msgDieResult;
		msgDieResult.SetResultCode( DIE_RESULT_CODE_RELIVE );
		CNetManager::GetMe()->SendPacket(&msgDieResult);
		return 1;
	}

	INT		PlayerMySelf::GetData( LuaPlus::LuaState* state )
	{
		LuaPlus::LuaStack args(state);
		if( !args[ 2 ].IsString() ) return 0;

		CCharacterData* pCharData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();

		std::string  str = args[ 2 ].GetString();
		if( str == "LEVEL" )	//等级
		{
			state->PushInteger(pCharData->Get_Level());
			return 1;
		}
		else if( str == "HP" )					//生命值
		{
			state->PushInteger(pCharData->Get_HP());
			return 1;
		}
		else if( str == "MP" )//魔法值
		{
			state->PushInteger(pCharData->Get_MP());
			return 1;
		}
		else if( str == "EXP" )					//经验
		{
			if(pCharData->Get_Exp() < 0)
			{
				state->PushInteger(0);
			}
			else
			{
				state->PushInteger(pCharData->Get_Exp());
			}
			return 1;
		}
		else if( str == "NEEDEXP" )				//升级经验
		{
			state->PushInteger(pCharData->Get_MaxExp());
			return 1;
		}
		else if( str == "STRIKEPOINT" )				//连击点
		{
			state->PushInteger(pCharData->Get_StrikePoint());
			return 1;
		}
		else if( str == "MONEY" )				//货币
		{
			state->PushInteger(pCharData->Get_Money());

			//同时输出转换为金、银、铜币的数值
			INT nMoney = pCharData->Get_Money();

			INT nGoldCoin;	
			INT nSilverCoin;
			INT nCopperCoin;

			nCopperCoin = nMoney % 100;

			if( nMoney >= 100 )
			{
				nSilverCoin = ((nMoney-nCopperCoin)/100) % 100;
			}
			else
			{
				nSilverCoin = 0;
			}

			if ( nMoney >= 10000 )
			{
				nGoldCoin = (((nMoney-nCopperCoin)/100)-nSilverCoin)/100;
			}
			else
				nGoldCoin = 0;

			state->PushInteger(nGoldCoin);
			state->PushInteger(nSilverCoin);
			state->PushInteger(nCopperCoin);
			return 4;
		}
		else if( str == "RMB" )			//人物元宝数量
		{
			state->PushInteger(pCharData->Get_RMB());
			return 1;
		}
		else if( str == "STR" )					//力量 力量
		{
			state->PushInteger(pCharData->Get_STR());
			return 1;
		}
		else if( str == "SPR" )					//灵气 灵力
		{
			state->PushInteger(pCharData->Get_SPR());
			return 1;
		}
		else if( str == "CON" )					//体制 体制
		{
			state->PushInteger(pCharData->Get_CON());
			return 1;
		}
		else if( str == "INT" )					//定力 智力
		{
			state->PushInteger(pCharData->Get_INT());
			return 1;
		}								   
		else if( str == "DEX" )					//身法 敏捷
		{
			state->PushInteger(pCharData->Get_DEX());
			return 1;
		}								   
		else if( str == "POINT_REMAIN" )			//剩余点数
		{
			state->PushInteger(pCharData->Get_PointRemain());
			return 1;
		}								   
		else if( str == "ATT_PHYSICS" )			//物理攻击力
		{
			state->PushInteger(pCharData->Get_AttPhysics());
			return 1;
		}								   
		else if( str == "DEF_PHYSICS" )			//物理防御力
		{
			state->PushInteger(pCharData->Get_DefPhysics());
			return 1;
		}
		else if( str == "ATT_MAGIC" )			//魔法攻击力
		{
			state->PushInteger(pCharData->Get_AttMagic());
			return 1;
		}								   
		else if( str == "DEF_MAGIC" )			//魔法防御力
		{
			state->PushInteger(pCharData->Get_DefMagic());
			return 1;
		}								   
		else if( str == "MAXHP" )				//最大生命值
		{
			state->PushInteger(pCharData->Get_MaxHP());
			return 1;
		}								   
		else if( str == "MAXMP" )				//最大魔法值
		{			
			state->PushInteger(pCharData->Get_MaxMP());
			return 1;
		}								   
		else if( str == "RAGE" )				//怒气
		{			
			state->PushInteger(pCharData->Get_Rage());
			return 1;
		}								   
		else if( str == "MAXRAGE" )				//最大怒气
		{			
			state->PushInteger(BASE_MAX_RAGE);
			return 1;
		}								   
		else if( str == "HP_RESPEED" )			//HP恢复速度  点/秒
		{			
			state->PushInteger(pCharData->Get_HPRespeed());
			return 1;
		}								   
		else if( str == "MP_RESPEED" )			//MP恢复速度  点/秒
		{
			state->PushInteger(pCharData->Get_MPRespeed());
			return 1;
		}								   
		else if( str == "HIT" )					//命中率
		{
			state->PushInteger(pCharData->Get_Hit());
			return 1;
		}								   
		else if( str == "MISS" )					//闪避率
		{
			state->PushInteger(pCharData->Get_Miss());
			return 1;
		}								   
		else if( str == "CRITRATE" )				//会心率
		{
			state->PushInteger(pCharData->Get_CritRate());
			return 1;								   
		}								   
		else if( str == "MOVESPEED" )			//移动速度
		{
			state->PushNumber(pCharData->Get_MoveSpeed());
			return 1;								   
		}								   
		else if( str == "ATTACKSPEED" )			//攻击速度
		{
			state->PushInteger(pCharData->Get_AttackSpeed());
			return 1;
		}
		else if( str == "ATTACKCOLD" )			//冰攻击
		{
			state->PushInteger(pCharData->Get_AttCold());
			return 1;								   
		}								   
		else if( str == "DEFENCECOLD" )			//冰防御
		{
			state->PushInteger(pCharData->Get_DefCold());
			return 1;								   
		}								   
		else if( str == "ATTACKFIRE" )			//火攻击
		{
			state->PushInteger(pCharData->Get_AttFire());
			return 1;								   
		}								   
		else if( str == "DEFENCEFIRE" )			//火防御
		{
			state->PushInteger(pCharData->Get_DefFire());
			return 1;								   
		}								   
		else if( str == "ATTACKLIGHT" )			//电攻击
		{
			state->PushInteger(pCharData->Get_AttLight());
			return 1;								   
		}								   
		else if( str == "DEFENCELIGHT" )			//电防御
		{
			state->PushInteger(pCharData->Get_DefLight());
			return 1;								   
		}								   
		else if( str == "ATTACKPOISON" )			//毒攻击
		{
			state->PushInteger(pCharData->Get_AttPosion());
			return 1;								   
		}								   
		else if( str == "DEFENCEPOISON" )		//毒防御
		{
			state->PushInteger(pCharData->Get_DefPosion());
			return 1;								   
		}								   
		else if( str == "MEMPAI" )				//门派
		{
			state->PushInteger(pCharData->Get_MenPai());
			return 1;								   
		}								   
		else if( str == "CAMP" )					//阵营  fujia 2007.12.14
		{
			state->PushInteger(pCharData->Get_CampData()->m_nCampID);
			return 1;
		}
		//else if( str == "VIGOR")
		//{
		//	state->PushInteger(pCharData->Get_Vigor());
		//	return 1;
		//}
		//else if( str == "MAXVIGOR")	
		//{
		//	state->PushInteger(pCharData->Get_MaxVigor());
		//	return 1;
		//}
		else if( str == "GUILD")
		{
			state->PushInteger(pCharData->Get_Guild());
		}
		//else if( str == "ENERGY")
		//{
		//	state->PushInteger(pCharData->Get_Energy());
		//	return 1;
		//}
		//else if( str == "MAXENERGY")
		//{
		//	state->PushInteger(pCharData->Get_MaxEnergy());
		//	return 1;
		//}
		else if( str == "PORTRAIT" )			//人物头像
		{
			
			// 得到玩家和自己的头像信息
			LPCSTR pImageName = NULL;
			DBC_DEFINEHANDLE(s_pFaceImageDBC, DBC_CHARACTER_FACE)
			//打开生物定义表
			const _DBC_CHAR_FACE* pFaceImage = NULL;
			pFaceImage = (const _DBC_CHAR_FACE*)(s_pFaceImageDBC->Search_Index_EQU(pCharData->Get_PortraitID()));
				
			if(pFaceImage)
			{
				pImageName = pFaceImage->pImageSetName;
			}
			//else // 默认状态不显示头像图标， 头像图标显示空
			//{
			//	pFaceImage = (const _DBC_CHAR_FACE*)(s_pFaceImageDBC->Search_LineNum_EQU(0));
			//	pImageName = pFaceImage->pImageSetName;
			//}//
					
			if(pImageName)
			{
				state->PushString(pImageName);
			}
			else
			{
				state->PushString(_T(""));
			}

			return 1;
		}
		

		state->PushInteger(-1);
		return 1;
	}

	//获得玩家的某项技能信息
	INT PlayerMySelf::GetSkillInfo( LuaPlus::LuaState* state )
	{
		LuaPlus::LuaStack args(state);

		if( !args[2].IsInteger() ) return 0;

		if( !args[3].IsString() ) return 0;

		INT	nSkillID	 = args[2].GetInteger();
		std::string  str = args[3].GetString();

		const SCLIENT_SKILL* pSkill = 
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Skill(nSkillID);

		if(str == "name")			// 名称
		{
			if(pSkill)
				state->PushString(pSkill->m_pDefine->m_lpszName);
			else
				state->PushString("");
		}
		else if(str == "learn")		// 是否学会
		{
			if(pSkill)
				state->PushBoolean(pSkill->m_bLeaned == TRUE);
			else
				state->PushBoolean(false);
		}
		else if(str == "iconname")	// 图标文件名
		{
			if(pSkill)
				state->PushString(pSkill->m_pDefine->m_lpszIconName);
			else
				state->PushString("");
		}
		else if(str == "passivity")	// 是否是被动技能
		{	
			if(pSkill)
				state->PushInteger(pSkill->m_pDefine->m_nPassiveFlag);
		}
		else if(str == "explain")	// 详细解释
		{	
			if(pSkill)
				state->PushString(pSkill->m_pDefine->m_pszDesc);
			else
				state->PushString("");
		}
		else if(str == "skilldata")	// 技能数据
		{	
			if(pSkill)
				state->PushString(pSkill->GetSkillDesc_Interface());
			else
				state->PushString("");
		}
		else if(str == "level")	// 技能等级
		{	
			
			if(pSkill)
			{
				state->PushInteger(pSkill->m_nLevel);
			}
		}
		return 1;
	}

	//获得玩家的某项心法信息
	INT PlayerMySelf::GetSkillClassInfo( LuaPlus::LuaState* state )
	{
		LuaPlus::LuaStack args(state);

		if( !args[2].IsInteger() ) return 0;

		if( !args[3].IsString() ) return 0;

		INT	nXinfaID	 = args[2].GetInteger();
		std::string  str = args[3].GetString();

		const SCLIENT_SKILLCLASS* pSkillClass = 
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_SkillClass(nXinfaID);

		if(str == "name")			// 名称
		{
			state->PushString(pSkillClass->m_pDefine->pszName);
		}
		//else if(str == "level")		// 等级
		//{
		//	state->PushInteger(pSkillClass->m_nLevel);
		//}
		else if(str == "iconname")	// 图标文件名
		{
			state->PushString(pSkillClass->m_pDefine->pszIconName);
		}
		else if(str == "explain")	// 详细解释
		{	
			state->PushString(pSkillClass->m_pDefine->pszDesc);
		}
		return 1;
	}

	//获得玩家的某项生活技能信息
	INT PlayerMySelf::GetAbilityInfo( LuaPlus::LuaState* state )
	{
		LuaPlus::LuaStack args(state);

		if( !args[2].IsInteger() ) return 0;

		if( !args[3].IsString() ) return 0;


		INT	nAbilityID	 = args[2].GetInteger();
		std::string  str = args[3].GetString();

		const SCLIENT_LIFEABILITY* pLifeAbility = 
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_LifeAbility(nAbilityID);

		if(str == "name")			// 名称
		{
			state->PushString(pLifeAbility->m_pDefine->szName);
		}
		else if(str == "level")		// 等级
		{
			state->PushInteger(pLifeAbility->m_nLevel);
		}
		else if(str == "maxlevel")	// 等级上限
		{
			state->PushInteger(pLifeAbility->m_pDefine->nLevelMax);
		}
		else if(str == "skillexp")	// 熟练度
		{
			state->PushInteger(pLifeAbility->m_nExp);
		}
		else if(str == "iconname")	// 图标文件名
		{
			state->PushString(pLifeAbility->m_pDefine->szIconName);
		}
		else if(str == "explain")	// 详细解释
		{	
			state->PushString(pLifeAbility->m_pDefine->szExplain);
		}
		else if(str == "popup")	// 是否弹出下级界面
		{	// 1.弹出合成界面，2.弹出镶嵌界面，-1.不弹界面
			state->PushInteger(pLifeAbility->m_pDefine->nPopup);
		}
		return 1;
	}

	// 角色请求学习生活技能
	INT PlayerMySelf::AskLeanAbility(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);

		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:PlayerMySelf param parameter error");
		}

		INT nAbilityID = args[2].GetInteger();

		// 玩家生活技能数据
		const SCLIENT_LIFEABILITY* pLifeAbility = 
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_LifeAbility(nAbilityID);

		if(!pLifeAbility) return 0;

		// 用本地数据先判定是否满足升级条件
		CObject_PlayerMySelf *pMyself = CObjectManager::GetMe()->GetMySelf();
		if(!pMyself) return 0;
		
		CCharacterData* pData = pMyself->GetCharacterData();
		if(!pData) return 0;

		if( pData->Get_Exp() < CDataPool::GetMe()->StudyAbility_GetNeedExp() )
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"经验值不够");
			return 0;
		}
		if( pData->Get_Money() < CDataPool::GetMe()->StudyAbility_GetNeedMoney() )
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "金钱不够" );
			return 0;
		}
		if( pLifeAbility->m_nExp < CDataPool::GetMe()->StudyAbility_GetSkillExp() )
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "技能熟练度不够" );
			return 0;
		}
		//任务等级是否满足
		if( CDataPool::GetMe()->StudyAbility_GetLevelLimit() > pData->Get_Level() )
		{
			char szText[_MAX_PATH];
			_snprintf( szText, _MAX_PATH, "人物等级不够，请到%d级以后再来", CDataPool::GetMe()->StudyAbility_GetLevelLimit() );
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, szText );
			return 0;
		}

		//向服务器发送升级请求
		CGAskLeanAbility Msg;
		Msg.SetAbilityTeacherInfo(nAbilityID);
		Msg.SetNeedMoney(CDataPool::GetMe()->StudyAbility_GetNeedMoney());
		Msg.SetNeedExpy(CDataPool::GetMe()->StudyAbility_GetNeedExp());
		Msg.SetNeedSkillExpy(CDataPool::GetMe()->StudyAbility_GetSkillExp());

		CNetManager::GetMe()->SendPacket(&Msg);
		return 0;
	}


	INT PlayerMySelf::GetPos( LuaPlus::LuaState* state)
	{
		
		CObject_PlayerMySelf* pCharObj = CObjectManager::GetMe()->GetMySelf();
		if( pCharObj )
		{
			const _DBC_SCENE_DEFINE* pSceneDef = CWorldManager::GetMe()->GetActiveScene()->GetSceneDefine();
			FLOAT x = pCharObj->GetPosition().x * 512 / pSceneDef->nXSize;
			FLOAT y = pCharObj->GetPosition().z * 512 / pSceneDef->nZSize;
			state->PushNumber( x );
			state->PushNumber( y );
			state->PushInteger( INT( pCharObj->GetFaceDir() * 180.f / TDU_PI ) );
		}
		else
		{
			state->PushNumber( 0 );
			state->PushNumber( 0 );
			state->PushInteger( 0 );
		}
		return 3;
	}

	INT PlayerMySelf::IsPresent(LuaPlus::LuaState* state)
	{
		if( CObjectManager::GetMe()->GetMySelf() )
		{
			state->PushBoolean(true);
		}
		else
		{
			state->PushBoolean(FALSE);
		}
		return 1;
	}
	
	//称号相关
	INT PlayerMySelf::GetAgnameNum(LuaPlus::LuaState* state)
	{
		INT	AgnameNum = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_TitleNum();
		Assert(AgnameNum<=MAX_TITLE_SIZE);
		state->PushInteger(AgnameNum);
		return 1;
	}

	INT PlayerMySelf::EnumAgname(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);

		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA: EnumAgname Wrong Param1");
		}
		if (!(args[3].IsString()))
		{
			TDThrow("LUA: EnumAgname Wrong Param2");
		}

		INT nIndex = args[2].GetInteger();
		const char* szFilter = args[3].GetString();

		//
		_TITLE_ Title = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Title(nIndex);
		if(Title.bFlag == _TITLE_::INVALID_TITLE )
		{
			state->PushString("尚空");
		}
		else if(Title.bFlag == _TITLE_::ID_TITLE)
		{
			INT TitleTBIndex = Title.ID;
			if(TitleTBIndex>0)
			{
				DBC_DEFINEHANDLE(pTitleDBC, DBC_TITLE_NAME);
				const _DBC_TITLE_NAME* pTitle = (const _DBC_TITLE_NAME*)pTitleDBC->Search_LineNum_EQU(TitleTBIndex);
				if(strncmp(szFilter, "name", 4) == 0)
					state->PushString( pTitle->szTitle);
				else
					state->PushString( pTitle->szTitleDesc);
			}
		}
		else
		{
			if(strncmp(szFilter, "name", 4) == 0)
				state->PushString( Title.szTitleData );
			else
				state->PushString( " " );

		}
	
		return 1;
	}

	INT PlayerMySelf::GetCurrentAgname(LuaPlus::LuaState* state)
	{
		if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurTitleType() == _TITLE::MOOD_TITLE)
		{
			state->PushString("");
			return 1;
		}
		const char* szCurTitle = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurTitle();
		/*if(strcmp(szCurTitle, "") != 0)
		{*/
			state->PushString(szCurTitle);
		/*}*/
		return 1;
		//INT TitleID = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurTitle();
		//state->PushInteger(TitleID);

	/*	_TITLE_	Title = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Title(TitleID);

		if(Title.bFlag == _TITLE_::INVALID_TITLE )
		{
			state->PushString("尚空");
		}
		else if(Title.bFlag == _TITLE_::ID_TITLE)
		{
			INT TitleTBIndex = Title.ID;
			if(TitleTBIndex>0)
			{
				DBC_DEFINEHANDLE(pTitleDBC, DBC_TITLE_NAME);
				const _DBC_TITLE_NAME* pTitle = (const _DBC_TITLE_NAME*)pTitleDBC->Search_LineNum_EQU(TitleTBIndex);
				state->PushString( pTitle->szTitle);
			}
		}
		else
		{
			state->PushString( Title.szTitleData );
		}*/
	}

	INT PlayerMySelf::AskChangeCurrentAgname(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA: AskChangeCurrentAgname Wrong Param1");
		}
		INT nIndex = args[2].GetInteger();
		
		_TITLE_ Title = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Title(nIndex);
	
		if(Title.bFlag == _TITLE_::INVALID_TITLE )
		{
			CGameInterface::GetMe()->Title_UpdateCurTitle(Title.bType, 0);
		}
		else if(Title.bFlag == _TITLE_::ID_TITLE)
		{
			INT TitleTBIndex = Title.ID;
			if(TitleTBIndex>0)
			{
				DBC_DEFINEHANDLE(pTitleDBC, DBC_TITLE_NAME);
				const _DBC_TITLE_NAME* pTitle = (const _DBC_TITLE_NAME*)pTitleDBC->Search_LineNum_EQU(TitleTBIndex);
				CGameInterface::GetMe()->Title_UpdateCurTitle(Title.bType, Title.ID);
			}
		}
		else
		{
			CGameInterface::GetMe()->Title_UpdateCurTitle( Title.bType, 0 );
		}
		
		return 0;
	}


	
	INT PlayerMySelf::SetNullAgname(LuaPlus::LuaState* state)
	{
		_TITLE_ Title;
		CGameInterface::GetMe()->Title_UpdateCurTitle(_TITLE::NO_TITLE, 0);
		return 0;
	}


	// 发送手动调整属性点数 
	INT PlayerMySelf::SendAskManualAttr(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA: SendAskManualAttr[2] Wrong Param1");
		}

		if (!(args[3].IsInteger()))
		{
			TDThrow("LUA: SendAskManualAttr[3] Wrong Param1");
		}

		if (!(args[4].IsInteger()))
		{
			TDThrow("LUA: SendAskManualAttr[4] Wrong Param1");
		}

		if (!(args[5].IsInteger()))
		{
			TDThrow("LUA: SendAskManualAttr[5] Wrong Param1");
		}

		if (!(args[6].IsInteger()))
		{
			TDThrow("LUA: SendAskManualAttr[6] Wrong Param1");
		}


		CGReqManualAttr msg;
		
		int iStr = args[2].GetInteger();
		int iSpr = args[3].GetInteger();
		int iCon = args[4].GetInteger();
		int iInt = args[5].GetInteger();
		int iDex = args[6].GetInteger();

		if((0 == iStr)&&
			( 0 == iSpr)&&
			( 0 == iCon)&&
			( 0 == iInt)&&
			( 0 == iDex))
		{
			return 0;
		}

		msg.SetStr(iStr);
		msg.SetSpr(iSpr);
		msg.SetCon(iCon);
		msg.SetInt(iInt);
		msg.SetDex(iDex);

		CNetManager::GetMe()->SendPacket(&msg);
	
		return 0;
	}
	
	//----------------------------------------------------------------------------------------------------------
	// 组队相关
	//
	
	//----
	// 同意加入队伍(有人邀请我)
	INT	PlayerMySelf::SendAgreeJoinTeam(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA: AskChangeCurrentAgname Wrong Param1");
		}
		int iTeamIndex = args[2].GetInteger();

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CGTeamRetInvite msg;
		
			//msg.SetGUID( CUIDataPool::GetMe()->GetTeamInvitorGUID());
			// 修改之后加一个参数
			GUID_t invitorGUID = CUIDataPool::GetMe()->GetTeamInvitorGUID(iTeamIndex);
			if(INVALID_GUID == invitorGUID)
			{
				return 0; 
			}
			msg.SetGUID(invitorGUID);

			msg.SetReturn(1);
			CNetManager::GetMe()->SendPacket(&msg);

			// 同意加入一个队伍后, 清空掉其他的队伍.
			CUIDataPool::GetMe()->ClearAllInviteTeam();
		}
		return 0;
	}

	// 拒绝加入队伍(有人邀请我)
	INT PlayerMySelf::SendRejectJoinTeam(LuaPlus::LuaState* state)
	{
		//if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		//{
		//	CGTeamRetInvite msg;
		//
		//	msg.SetGUID( CUIDataPool::GetMe()->GetTeamInvitorGUID());
		//	msg.SetReturn(0);
		//	CNetManager::GetMe()->SendPacket(&msg);

		//	// 删掉一个拒绝的队伍.
		//	CUIDataPool::GetMe()->EraseCurRejectTeam();
		//}
		//return 0;//

		// 得到队伍索引
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA: AskChangeCurrentAgname Wrong Param1");
		}
		int iTeamIndex = args[2].GetInteger();


		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CGTeamRetInvite msg;
		
			GUID_t invitorGUID = CUIDataPool::GetMe()->GetTeamInvitorGUID(iTeamIndex);
			if(INVALID_GUID == invitorGUID)
			{
				return 0; 
			}
			msg.SetGUID(invitorGUID);
			msg.SetReturn(0);
			CNetManager::GetMe()->SendPacket(&msg);

			// 删掉一个拒绝的队伍.
			CUIDataPool::GetMe()->EraseCurRejectTeam();
		}
		return 0;//

	}

		
	
	//-----
	// 同意加入队伍(有人申请加入队伍)
	INT	PlayerMySelf::SendAgreeJoinTeam_Apply(LuaPlus::LuaState* state)
	{
		//if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		//{
		//	CGTeamRetApply msg;
		//
		//	msg.SetSourGUID( CUIDataPool::GetMe()->GetSourceGUID());

		//	msg.SetReturn(1);
		//	CNetManager::GetMe()->SendPacket(&msg);
		//}
		//return 0;//

		
		// 得到队伍索引
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA: SendAgreeJoinTeam_Apply[2] Wrong Param1");
		}
		int iApplyIndex = args[2].GetInteger();

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CGTeamRetApply msg;
			GUID_t id = CUIDataPool::GetMe()->GetApplySourceGUID(iApplyIndex);
		
			if(INVALID_GUID == id)
			{
				return 0;
			}
			msg.SetSourGUID(id);
			msg.SetReturn(1);
			CNetManager::GetMe()->SendPacket(&msg);
		}
		return 0;//

	}

	// 拒绝加入队伍(有人申请加入队伍)
	INT PlayerMySelf::SendRejectJoinTeam_Apply(LuaPlus::LuaState* state)
	{
		//if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		//{
		//	CGTeamRetApply msg;
		//
		//	msg.SetSourGUID( CUIDataPool::GetMe()->GetSourceGUID());
		//	msg.SetReturn(0);
		//	CNetManager::GetMe()->SendPacket(&msg);
		//}
		//return 0;//

		
		// 得到队伍索引
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA: SendRejectJoinTeam_Apply[2] Wrong Param1");
		}
		int iApplyIndex = args[2].GetInteger();

		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CGTeamRetApply msg;
			GUID_t id = CUIDataPool::GetMe()->GetApplySourceGUID(iApplyIndex);
			if(INVALID_GUID == id)
			{
				return 0;
			}

			msg.SetSourGUID(id);
			msg.SetReturn(0);
			CNetManager::GetMe()->SendPacket(&msg);
		}
		return 0;//
	}



	//-----
	// 同意加入队伍(队员邀请某人加入队伍)
	INT	PlayerMySelf::SendAgreeJoinTeam_TeamMemberInvite(LuaPlus::LuaState* state)
	{
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CGTeamLeaderRetInvite msg;

			// 得到队伍索引
			LuaPlus::LuaStack args(state);
			if (!(args[2].IsInteger()))
			{
				TDThrow("LUA: SendRejectJoinTeam_Apply[2] Wrong Param1");
			}
			int iIndex = args[2].GetInteger();

			GUID_t SId;
			GUID_t DId;
			if(CUIDataPool::GetMe()->GetMemberInviteGUID(iIndex, SId, DId))
			{
				msg.SetSourGUID( SId );
				msg.SetDestGUID( DId );
				msg.SetReturn(1);
				CNetManager::GetMe()->SendPacket(&msg);
			}
		}
		return 0;
	}

	// 拒绝加入队伍(队员邀请某人加入队伍)
	INT PlayerMySelf::SendRejectJoinTeam_TeamMemberInvite(LuaPlus::LuaState* state)
	{
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CGTeamLeaderRetInvite msg;
		
			
			// 得到队伍索引
			LuaPlus::LuaStack args(state);
			if (!(args[2].IsInteger()))
			{
				TDThrow("LUA: SendRejectJoinTeam_Apply[2] Wrong Param1");
			}
			int iIndex = args[2].GetInteger();

			GUID_t SId;
			GUID_t DId;
			if(CUIDataPool::GetMe()->GetMemberInviteGUID(iIndex, SId, DId))
			{
				msg.SetSourGUID( SId );
				msg.SetDestGUID( DId );
				msg.SetReturn(0);
				CNetManager::GetMe()->SendPacket(&msg);
			}
		}
		return 0;
	}

	// 同意进入组队跟随模式
	INT PlayerMySelf::SendAgreeTeamFollow(LuaPlus::LuaState* state)
	{
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CGReturnTeamFollow Msg;

			Msg.SetReturn( TRUE );
			CNetManager::GetMe()->SendPacket(&Msg);
		}
		return 0;
	}

	// 拒绝进入组队跟随模式
	INT PlayerMySelf::SendRefuseTeamFollow(LuaPlus::LuaState* state)
	{
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "你拒绝进入组队跟随模式。" );

			CGReturnTeamFollow Msg;
			Msg.SetReturn( FALSE );
			CNetManager::GetMe()->SendPacket(&Msg);
		}
		return 0;
	}




	
	// 离开队伍
	INT PlayerMySelf::LeaveTeam(LuaPlus::LuaState* state)					
	{

		if(CUIDataPool::GetMe()->GetTeamOrGroup())
		{
			if(CUIDataPool::GetMe()->GetTeamOrGroup()->GetTeamMemberCount())
			{
				CUIDataPool::GetMe()->LeaveTeam();
			}
		}
		return 0;
	}

	// 解散队伍
	INT PlayerMySelf::DismissTeam(LuaPlus::LuaState* state)					
	{
		if(CUIDataPool::GetMe()->GetTeamOrGroup())
		{
			if(CUIDataPool::GetMe()->GetTeamOrGroup()->GetTeamMemberCount())
			{
				CUIDataPool::GetMe()->DismissTeam();
				SCRIPT_SANDBOX::Talk::s_Talk.TeamDestory();
			}
		}
		return 0;
	}

	// 删除当前选中队员
	INT PlayerMySelf::KickTeamMember(LuaPlus::LuaState* state)			
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:KickTeamMember");
		}	

		INT iIndex = args[2].GetInteger();

		if(CUIDataPool::GetMe()->GetTeamOrGroup())
		{
			if(CUIDataPool::GetMe()->GetTeamOrGroup()->GetTeamMemberCount())
			{
				CUIDataPool::GetMe()->KickTeamMember(iIndex);
			}
		}
		return 0;
	}

	
	// 删除当前选中队员
	INT PlayerMySelf::KickTeamMemberByIndex(LuaPlus::LuaState* state)			
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:KickTeamMemberByIndex");
		}	

		INT iIndex = args[2].GetInteger();
		
		if(CUIDataPool::GetMe()->GetTeamOrGroup())
		{
			if(CUIDataPool::GetMe()->GetTeamOrGroup()->GetTeamMemberCount())
			{

				CUIDataPool::GetMe()->KickTeamMember(iIndex);
			}
		}

		return 0;
	}


	//自建队伍
	INT	PlayerMySelf::CreateTeamSelf(LuaPlus::LuaState* state)
	{
		if ( CUIDataPool::GetMe()->GetTeamOrGroup()->HasTeam() )
		{
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "你已经在队伍中了，不能自建队伍。" );
			return 0;
		}

		CGTeamInvite msg;
		msg.SetSourObjID(CObjectManager::GetMe()->GetMySelf()->GetServerID());
		msg.SetDestName(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Name());
		CNetManager::GetMe()->SendPacket(&msg);

		return 0;
	}

		
	//任命队长消息
	INT	PlayerMySelf::AppointLeader(LuaPlus::LuaState* state)
	{
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA:AppointLeader");
		}	

		INT iIndex = args[2].GetInteger();

		if(CUIDataPool::GetMe()->GetTeamOrGroup())
		{
			if(CUIDataPool::GetMe()->GetTeamOrGroup()->GetTeamMemberCount())
			{

				CUIDataPool::GetMe()->AppointLeader(iIndex);
			}
		}
		return 0;

	}

	// 是否处于组队跟随状态
	INT	PlayerMySelf::InTeamFollowMode(LuaPlus::LuaState* state)
	{
		CCharacterData* pCharData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();

		BOOL bMode = pCharData->Get_TeamFollowFlag();

		if( bMode )
		{
			state->PushBoolean(true);
		}
		else
		{
			state->PushBoolean(false);
		}

		return 1;
	}

	// 取消跟随
	INT	PlayerMySelf::StopFollow(LuaPlus::LuaState* state)
	{
		CGStopTeamFollow Msg;
		CNetManager::GetMe()->SendPacket(&Msg);
		return 0;
	}

	// 组队跟随
	INT PlayerMySelf::TeamFrame_AskTeamFollow(LuaPlus::LuaState* state)
	{
		// 如果处于摆摊状态，不允许这样做
		if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你正在摆摊……");
			return 0;
		}

		CGAskTeamFollow Msg;
		CNetManager::GetMe()->SendPacket(&Msg);
		return 0;
	}


	// 得到队员guid
	INT PlayerMySelf::GetTeamMemberGUID(LuaPlus::LuaState* state)
	{

		// 得到队伍索引
		LuaPlus::LuaStack args(state);
		if (!(args[2].IsInteger()))
		{
			TDThrow("LUA: Send Add Friend Team");
		}
		int iIndex = args[2].GetInteger();

		GUID_t MyGUID;
		GUID_t SelGUID;
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			const TeamMemberInfo* pInfo = CUIDataPool::GetMe()->GetTeamOrGroup()->GetMemberByIndex(iIndex);

			if(pInfo)
			{
				MyGUID  = CObjectManager::GetMe()->GetMySelf()->GetServerGUID();
				SelGUID = pInfo->m_GUID;

				if(MyGUID != SelGUID)
				{
					state->PushInteger(SelGUID);
					return 1;
				}
				
			}
				
		}

		state->PushInteger(-1);
		return 1;
	}

	// 组队相关
	//----------------------------------------------------------------------------------------------------------


	
	// 得到玩家的称号
	INT	PlayerMySelf::GetCurTitle(LuaPlus::LuaState* state)
	{
		//心情不能出现在称号的位置
		if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurTitleType() == _TITLE::MOOD_TITLE)
		{
			state->PushString("");
			return 1;
		}
		state->PushString( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurTitle());
		return 1;
	}
	
	// 右键菜单相关
	INT PlayerMySelf::ShowMySelfContexMenu(LuaPlus::LuaState* state)
	{
		INT myId = CObjectManager::GetMe()->GetMySelf()->GetServerID();
		CGameInterface::GetMe()->Object_ShowContexMenu(myId, TRUE);
		return 0;
	}
	
	// 左键选中自己
	INT	PlayerMySelf::SelectMyselfAsTarget(LuaPlus::LuaState* state)
	{
		INT myId = CObjectManager::GetMe()->GetMySelf()->GetServerID();
		CGameInterface::GetMe()->Object_SelectAsMainTarget(myId, 1);
		return 0;
	}

	// 设置二级密码 2006-4-29
	INT	PlayerMySelf::SetSupperPassword(LuaPlus::LuaState* state)
	{
		if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsMinorPwdSetup(FALSE))
		{
			// 如果设置过密码， 就修改密码。
			CEventSystem::GetMe()->PushEvent(GE_MINORPASSWORD_OPEN_CHANGE_PASSWORD_DLG);
		}
		else
		{
			// 如果设置过密码， 就设置密码。
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsMinorPwdSetup();
		}
		return 0;
	}
}