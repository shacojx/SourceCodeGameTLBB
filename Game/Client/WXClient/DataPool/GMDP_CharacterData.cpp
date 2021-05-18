#include "StdAfx.h"
#include "GMDP_CharacterData.h"
#include "GMDP_Struct_CharacterData.h"
#include "../Global.h"
#include "../Object/Character/Obj_Character.h"
#include "../Object/Character/Obj_PlayerNPC.h"
#include "../Object/Character/Obj_PlayerOther.h"
#include "../Object/Character/Obj_PlayerMySelf.h"
#include "../Object/ObjectManager.h"
#include "TDException.h"
#include "TDDBC_Struct.h"
#include "../DBC/GMDataBase.h"
#include "../Procedure/GameProcedure.h"
#include "../Action/GMActionSystem.h"
#include "../Event/GMEventSystem.h"
#include "../Object/Item/Obj_Item.h"
#include "TDUISystem.h"
#include "..\Sound\GMSoundSystem.h"
#include "..\DataPool\GMDataPool.h"
#include "../Interface/GMGameInterface.h"
#include "GCCharBaseAttrib.h"
#define CHECK_MUST_NOT(typeName) \
	if(	m_pObjRef->GetCharacterType() == CObject_Character::typeName) TDThrow("Character must not %s,(File:%s Line:%d)", #typeName, __FILE__, __LINE__);

CCharacterData::CCharacterData(CObject_Character* pCharObj)
{
	TDAssert(pCharObj);

	m_pObjRef = pCharObj;
	m_fSpeedRate = 1.f;

	switch(m_pObjRef->GetCharacterType())
	{
	case CObject_Character::CT_PLAYERMYSELF:
		m_pData = new SDATA_PLAYER_MYSELF;
		_Init_AsCharacter();
		_Init_AsNPC();
		_Init_AsPlayerOther();
		_Init_AsPlayerMySelf();
		break;
	case CObject_Character::CT_PLAYEROTHER:
		m_pData = new SDATA_PLAYER_OTHER;
		_Init_AsCharacter();
		_Init_AsNPC();
		_Init_AsPlayerOther();
		break;
	case CObject_Character::CT_MONSTER:
		m_pData = new SDATA_NPC;
		_Init_AsCharacter();
		_Init_AsNPC();
		break;

	default:
		TDThrow("Invalid Character type");
	}

}

CCharacterData::~CCharacterData()
{
	if(m_pData && m_pObjRef)
	{
		switch(m_pObjRef->GetCharacterType())
		{
		case CObject_Character::CT_PLAYERMYSELF:
			delete (SDATA_PLAYER_MYSELF*)m_pData;
			break;

		case CObject_Character::CT_PLAYEROTHER:
			delete (SDATA_PLAYER_OTHER*)m_pData;
			break;

		case CObject_Character::CT_MONSTER:
			delete (SDATA_NPC*)m_pData;
			break;

		default:
			TDThrow("Invalid Character type");
		}

		m_pData = NULL;
		m_pObjRef = NULL;
	}
}

VOID CCharacterData::_Init_AsCharacter(VOID)
{
	m_pData->m_nRaceID			= INVALID_ID;
	m_pData->m_nPortraitID		= -1;			// 默认头像的值
	m_pData->m_strName			= "";
	m_pData->m_fHPPercent		= 0.f;
	m_pData->m_fMPPercent		= 0.f;
	m_pData->m_fMoveSpeed		= -1.f;
	m_pData->m_CampData.CleanUp();
	m_pData->m_nOwnerID			= INVALID_ID;
	m_pData->m_OccupantGUID		= INVALID_GUID;
	m_pData->m_nModelID			= INVALID_ID;
	m_pData->m_nMountID			= INVALID_ID;
	m_pData->m_nLevel			= 0;
	m_pData->m_bFightState		= FALSE;
	m_pData->m_nRelative		= NPC_AI_TYPE_CANNOTATTACK;
	m_pData->m_bSit				= FALSE;
	m_pData->m_nStealthLevel	= 0;
	m_pData->m_nRage			= 0;
	m_pData->m_nAIType			= 0;			//默认为0
}

VOID CCharacterData::_Init_AsNPC(VOID)
{
}

VOID CCharacterData::_Init_AsPlayerOther(VOID)
{
	((SDATA_PLAYER_OTHER*)m_pData)->m_nMenPai			= -1;
	((SDATA_PLAYER_OTHER*)m_pData)->m_nHairMeshID		= INVALID_ID;
	((SDATA_PLAYER_OTHER*)m_pData)->m_uHairColor		= 0xFFFFFFFF;

	((SDATA_PLAYER_OTHER*)m_pData)->m_nFaceMeshID		= INVALID_ID;

	((SDATA_PLAYER_OTHER*)m_pData)->m_nEquipVer			= 0;
	for(INT i=0; i<HEQUIP_NUMBER; i++)
	{
		((SDATA_PLAYER_OTHER*)m_pData)->m_nEquipmentID[i]		= INVALID_ID;
		((SDATA_PLAYER_OTHER*)m_pData)->m_nEquipmentGemID[i]	= INVALID_ID;
	}

	((SDATA_PLAYER_OTHER*)m_pData)->m_bTeamFlag			= FALSE;
	((SDATA_PLAYER_OTHER*)m_pData)->m_bTeamLeaderFlag	= FALSE;
	((SDATA_PLAYER_OTHER*)m_pData)->m_bTeamFullFlag		= FALSE;
	((SDATA_PLAYER_OTHER*)m_pData)->m_bTeamFollowFlag	= FALSE;
	((SDATA_PLAYER_OTHER*)m_pData)->m_bIsInStall		= FALSE;
	((SDATA_PLAYER_OTHER*)m_pData)->m_strStallName		= "";

}

VOID CCharacterData::_Init_AsPlayerMySelf(VOID)
{
	((SDATA_PLAYER_MYSELF*)m_pData)->m_nHP			= 0;
	((SDATA_PLAYER_MYSELF*)m_pData)->m_nMP			= 0;
	((SDATA_PLAYER_MYSELF*)m_pData)->m_nExp			= -1;
	//((SDATA_PLAYER_MYSELF*)m_pData)->m_nMaxExp		= 0;
	((SDATA_PLAYER_MYSELF*)m_pData)->m_nMoney		= 0;
	((SDATA_PLAYER_MYSELF*)m_pData)->m_nRMB		= 0;
	//((SDATA_PLAYER_MYSELF*)m_pData)->m_nMaxVigor	= 0;
	((SDATA_PLAYER_MYSELF*)m_pData)->m_nDoubleExpTime_Num		= 1;
	//((SDATA_PLAYER_MYSELF*)m_pData)->m_nMaxEnergy	= 0;
	//((SDATA_PLAYER_MYSELF*)m_pData)->m_nGoodBadValue= 0;
	((SDATA_PLAYER_MYSELF*)m_pData)->m_guidCurrentPet.Reset();

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nGuild		= INVALID_ID;

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nSTR			= 0;
	((SDATA_PLAYER_MYSELF*)m_pData)->m_nSPR			= 0;
	((SDATA_PLAYER_MYSELF*)m_pData)->m_nCON			= 0;
	((SDATA_PLAYER_MYSELF*)m_pData)->m_nINT			= 0;
	((SDATA_PLAYER_MYSELF*)m_pData)->m_nDEX			= 0;
	((SDATA_PLAYER_MYSELF*)m_pData)->m_nPoint_Remain= 0;

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nAtt_Physics	= 0;
	((SDATA_PLAYER_MYSELF*)m_pData)->m_nAtt_Magic	= 0;
	((SDATA_PLAYER_MYSELF*)m_pData)->m_nDef_Physics	= 0;
	((SDATA_PLAYER_MYSELF*)m_pData)->m_nDef_Magic	= 0;
	((SDATA_PLAYER_MYSELF*)m_pData)->m_nMaxHP		= 0;
	((SDATA_PLAYER_MYSELF*)m_pData)->m_nMaxMP		= 0;
	((SDATA_PLAYER_MYSELF*)m_pData)->m_nHP_ReSpeed	= 0;
	((SDATA_PLAYER_MYSELF*)m_pData)->m_nMP_ReSpeed	= 0;
	((SDATA_PLAYER_MYSELF*)m_pData)->m_nHit			= 0;
	((SDATA_PLAYER_MYSELF*)m_pData)->m_nMiss		= 0;
	((SDATA_PLAYER_MYSELF*)m_pData)->m_nCritRate	= 0;
	((SDATA_PLAYER_MYSELF*)m_pData)->m_nAttackSpeed	= 0;

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nAtt_Cold	= 0;
	((SDATA_PLAYER_MYSELF*)m_pData)->m_nDef_Cold	= 0;
	((SDATA_PLAYER_MYSELF*)m_pData)->m_nAtt_Fire	= 0;
	((SDATA_PLAYER_MYSELF*)m_pData)->m_nDef_Fire	= 0;
	((SDATA_PLAYER_MYSELF*)m_pData)->m_nAtt_Light	= 0;
	((SDATA_PLAYER_MYSELF*)m_pData)->m_nDef_Light	= 0;
	((SDATA_PLAYER_MYSELF*)m_pData)->m_nAtt_Posion	= 0;
	((SDATA_PLAYER_MYSELF*)m_pData)->m_nDef_Posion	= 0;

	((SDATA_PLAYER_MYSELF*)m_pData)->m_bIsMinorPwdSetup = FALSE;
	((SDATA_PLAYER_MYSELF*)m_pData)->m_bIsMinorPwdUnlocked = FALSE;

	((SDATA_PLAYER_MYSELF*)m_pData)->m_theSkillClass.clear();
	((SDATA_PLAYER_MYSELF*)m_pData)->m_theSkill.clear();
	((SDATA_PLAYER_MYSELF*)m_pData)->m_theLifeAbility.clear();
	((SDATA_PLAYER_MYSELF*)m_pData)->m_theSprescr.clear();
}

VOID CCharacterData::Set_RaceID(INT nRaceID)
{
	if(m_pData->m_nRaceID != nRaceID)
	{
		m_pData->m_nRaceID = nRaceID;
		m_pObjRef->OnDataChanged_RaceID();
	}

	//Push事件
	STRING strReturn;
	if(m_pObjRef->IsSpecialObject(strReturn) )
	{
		CEventSystem::GetMe()->PushEvent( GE_UNIT_RACE_ID, strReturn.c_str());
	}

	//根据RaceID重新刷新名字
	Set_AIType(Get_AIType());
	Set_Name(Get_Name());
}

VOID CCharacterData::Set_PortraitID(INT nPortraitID)
{
	if(m_pData->m_nPortraitID != nPortraitID)
	{
		m_pData->m_nPortraitID = nPortraitID;
	}

	// 通知渲染层
	((CObject_PlayerOther*)m_pObjRef)->OnDataChanged_FaceImage();

	//Push事件, 通知界面
	STRING strReturn;
	if(m_pObjRef->IsSpecialObject(strReturn))
	{
		CEventSystem::GetMe()->PushEvent(GE_UNIT_FACE_IMAGE, strReturn.c_str());
	}
}

VOID CCharacterData::Set_Name(LPCTSTR szName)
{
	m_pData->m_strName = szName;
	m_pObjRef->OnDataChanged_Name();

	//Push事件
	STRING strReturn;
	if(m_pObjRef->IsSpecialObject(strReturn) )
	{
		CEventSystem::GetMe()->PushEvent( GE_UNIT_NAME, strReturn.c_str());
	}
}

VOID CCharacterData::Set_CurTitle(LPCTSTR szTitle)
{
	m_pData->m_strTitle = TransTitleStr(szTitle);
//	m_pObjRef->OnDataChanged_CurTitles();
}

VOID CCharacterData::Set_CurTitleType(BYTE bTitleType)
{
	m_pData->m_TitleType = bTitleType;
	m_pObjRef->OnDataChanged_CurTitles();
}

VOID CCharacterData::Set_HPPercent(FLOAT fPercent)
{
	FLOAT fOldHPPercent = m_pData->m_fHPPercent;
	m_pData->m_fHPPercent = fPercent;

	////如果是玩家自己
	//if(	m_pObjRef->GetCharacterType() == CObject_Character::CT_PLAYERMYSELF)
	//{
	//	Set_HP((INT)(fPercent*Get_MaxHP()));
	//}

	//Push事件
	STRING strReturn;
	if(m_pObjRef->IsSpecialObject(strReturn) )
	{
		CEventSystem::GetMe()->PushEvent( GE_UNIT_HP, strReturn.c_str());
	}

	if((fOldHPPercent > 0.f && m_pData->m_fHPPercent <= 0.f)
		|| (fOldHPPercent <= 0.f && m_pData->m_fHPPercent > 0.f))
	{
		m_pObjRef->OnDataChanged_Die();
	}
}

VOID CCharacterData::Set_MPPercent(FLOAT fPercent)
{
	m_pData->m_fMPPercent = fPercent;

	////如果是玩家自己
	//if(	m_pObjRef->GetCharacterType() == CObject_Character::CT_PLAYERMYSELF)
	//{
	//	Set_MP((INT)(fPercent*Get_MaxMP()));
	//}

	//Push事件
	STRING strReturn;
	if(m_pObjRef->IsSpecialObject(strReturn))
	{
		CEventSystem::GetMe()->PushEvent(GE_UNIT_MP, strReturn.c_str());
	}
}

VOID CCharacterData::Set_CampData(const _CAMP_DATA *pCampData)
{
	m_pData->m_CampData = *pCampData;

	//Push事件
	STRING strReturn;
	if(m_pObjRef->IsSpecialObject(strReturn) )
	{
		CEventSystem::GetMe()->PushEvent( GE_UNIT_CAMP_ID, strReturn.c_str());
	}

	//根据RaceID重新刷新名字
	Set_AIType(Get_AIType());
	//根据阵营重新刷新名字
	Set_Name(Get_Name());
}

VOID CCharacterData::Set_OwnerID(ObjID_t nOwnerID)
{
	m_pData->m_nOwnerID = nOwnerID;
}

VOID CCharacterData::Set_AIType(INT nAIType)
{
	m_pData->m_nAIType = nAIType;

	CObject* pMyself = (CObject*)CObjectManager::GetMe()->GetMySelf();

	DBC_DEFINEHANDLE(s_pCreatureDBC, DBC_CREATURE_ATT)
	
		const _DBC_CREATURE_ATT* pCreatureInfo = 
	(const _DBC_CREATURE_ATT*)(s_pCreatureDBC->Search_Index_EQU(Get_RaceID()));

	//TDAssert(pCreatureInfo);
	if(!pCreatureInfo) return;

	if(pCreatureInfo->nCannotBeAttack > 0)
	{
		Set_Relative(NPC_AI_TYPE_CANNOTATTACK);
	}
	else if( RELATION_FRIEND == CGameInterface::GetMe()->GetCampType(pMyself,m_pObjRef) )
	{
		Set_Relative(NPC_AI_TYPE_CANNOTATTACK);
	}
	else
	{
		if(nAIType == GCCharBaseAttrib::CHAR_AI_TYPE_CANNOTATTACK)
		{
			Set_Relative(NPC_AI_TYPE_CANNOTATTACK);
		}
		else if(nAIType == GCCharBaseAttrib::CHAR_AI_TYPE_SCANNPC)
		{
			Set_Relative(NPC_AI_TYPE_SCANNPC);
		}
		else if(nAIType == GCCharBaseAttrib::CHAR_AI_TYPE_NOTSCANNPC)
		{
			Set_Relative(NPC_AI_TYPE_NOTSCANNPC);
		}
	}

	//根据ai重新刷新名字
	Set_Name(Get_Name());
	return;
}

VOID CCharacterData::Set_OccupantGUID(GUID_t guid)
{
	m_pData->m_OccupantGUID = guid;

	//Push事件
	STRING strReturn;
	if(m_pObjRef->IsSpecialObject(strReturn) )
	{
		CEventSystem::GetMe()->PushEvent( GE_UNIT_RELATIVE, strReturn.c_str());
	}
}

VOID CCharacterData::Set_Relative(INT nRelative)
{
	m_pData->m_nRelative = nRelative;

	STRING strReturn;
	if(m_pObjRef->IsSpecialObject(strReturn) )
	{
		CEventSystem::GetMe()->PushEvent( GE_UNIT_RELATIVE, strReturn.c_str());
	}
}

VOID CCharacterData::Set_ModelID(INT nModelID)
{
	if ( nModelID != m_pData->m_nModelID )
	{
		m_pData->m_nModelID = nModelID;
		m_pObjRef->OnDataChanged_ModelID();
	}
}

VOID CCharacterData::Set_MountID(INT nMountID)
{
	if ( nMountID != m_pData->m_nMountID )
	{
		m_pData->m_nMountID = nMountID;
		m_pObjRef->OnDataChanged_MountID();
	}
}

VOID CCharacterData::Set_Level(INT nLevel)
{
	m_pData->m_nLevel = nLevel;

	m_pObjRef->OnDataChanged_Level();

	//Push事件
	STRING strReturn;
	if(m_pObjRef->IsSpecialObject(strReturn) )
	{
		CEventSystem::GetMe()->PushEvent(GE_UNIT_LEVEL, strReturn.c_str());
		CEventSystem::GetMe()->PushEvent(GE_UNIT_MAX_EXP, strReturn.c_str());
	}

	//根据Level重新刷新名字
	Set_Name(Get_Name());
}

VOID CCharacterData::Set_StealthLevel(INT nStealthLevel)
{
	if(m_pData->m_nStealthLevel != nStealthLevel)
	{
		m_pData->m_nStealthLevel = nStealthLevel;
		m_pObjRef->OnDataChanged_StealthLevel();
	}
}

VOID CCharacterData::Set_MoveSpeed(FLOAT fSpeed)
{
	const FLOAT fBaseMoveSpeed = 3.5f;
	BOOL bChanged = abs(fSpeed - m_pData->m_fMoveSpeed) > 0.2f ? TRUE : FALSE;
	if (bChanged)
	{// 速度有了明显的变化，需要改变其频率
	}
	m_pData->m_fMoveSpeed = fSpeed;
	m_fSpeedRate = fSpeed / fBaseMoveSpeed;
	m_pObjRef->OnDataChanged_MoveSpeed();

	//Push事件
	STRING strReturn;
	if(m_pObjRef->IsSpecialObject(strReturn))
	{
		CEventSystem::GetMe()->PushEvent(GE_UNIT_MOVE_SPEED, strReturn.c_str());
	}
}
//
//VOID CCharacterData::Set_FightState(BOOL bFightState)
//{
//	m_pData->m_bFightState = bFightState;
//
//	//Push事件
//	STRING strReturn;
//	if(m_pObjRef->IsSpecialObject(strReturn))
//	{
//		CEventSystem::GetMe()->PushEvent(GE_UNIT_FIGHT_STATE, strReturn.c_str());
//	}
//
//	if(m_pObjRef != NULL)
//	{
//		m_pObjRef->OnDataChanged_FightState();
//	}
//}

VOID CCharacterData::Set_Sit(BOOL bSit)
{
	if(m_pData->m_bSit != bSit)
	{
		m_pData->m_bSit = bSit;

		if(m_pObjRef != NULL)
		{
			m_pObjRef->OnDataChanged_Sit();
		}
	}
}

//------------------------------------------
INT CCharacterData::Get_MenPai(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);

	return ((SDATA_PLAYER_OTHER*)m_pData)->m_nMenPai;
}

VOID CCharacterData::Set_MenPai(INT nMenPai)
{
	CHECK_MUST_NOT(CT_MONSTER);
	
	BOOL bReset = (((SDATA_PLAYER_OTHER*)m_pData)->m_nMenPai != nMenPai);

	if(((SDATA_PLAYER_OTHER*)m_pData)->m_nMenPai != -1)
	{
		if((9 == ((SDATA_PLAYER_OTHER*)m_pData)->m_nMenPai) && (nMenPai != ((SDATA_PLAYER_OTHER*)m_pData)->m_nMenPai))
			CSoundSystemFMod::_PlayUISoundFunc(62);
	}

	((SDATA_PLAYER_OTHER*)m_pData)->m_nMenPai = nMenPai;
	
	//如果是自己的门派数据，重构心法/技能数据
	//if(bReset && m_pObjRef->GetCharacterType() == CObject_Character::CT_PLAYERMYSELF)
	//{
	//	_On_MenPaiChanged();
	//}

	SCRIPT_SANDBOX::Talk::s_Talk.MenPaiJoin((MenPaiID_t)nMenPai); //__

	//Push事件
	STRING strReturn;
	if(m_pObjRef->IsSpecialObject(strReturn))
	{
		CEventSystem::GetMe()->PushEvent(GE_UNIT_MENPAI, strReturn.c_str());
	}
}

VOID CCharacterData::_On_MenPaiChanged(VOID)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	//门派改变，刷新所有门派心法和技能

	//---------------------------------------
	//加载属于该门派所有心法
	//{
	//	SKILLCLASS_MAP& mapSkillClass = ((SDATA_PLAYER_MYSELF*)m_pData)->m_theSkillClass;
	//	//清空所有旧心法
	//	mapSkillClass.clear();

	//	//加载心法表
	//	DBC_DEFINEHANDLE(s_pXinFaDBC, DBC_SKILL_XINFA);
	//	INT nNum = (INT)s_pXinFaDBC->GetRecordsNum();
	//	for(INT i=0; i<nNum; i++)
	//	{
	//		const _DBC_SKILL_XINFA* pXinFaDef = (const _DBC_SKILL_XINFA*)s_pXinFaDBC->Search_LineNum_EQU(i);
	//		if(pXinFaDef->nMenpaiID != Get_MenPai()) continue;

	//		Set_XinFa(pXinFaDef->nID, -1);
	//	}
	//}

	//---------------------------------------
	//加载属于该门派所有技能
	{
		SSKILL_MAP& mapSkill = ((SDATA_PLAYER_MYSELF*)m_pData)->m_theSkill;

		//清空旧门派技能
		mapSkill.clear();

		//重新加载技能表
		DBC_DEFINEHANDLE(s_pSkillDBC, DBC_SKILL_DATA);
		INT nNum = (INT)s_pSkillDBC->GetRecordsNum();

		for(INT i=0; i<nNum; i++)
		{
			const _DBC_SKILL_DATA* pSkillDef = (const _DBC_SKILL_DATA*)s_pSkillDBC->Search_LineNum_EQU(i);
			
			if(pSkillDef->m_nClassByUser != A_SKILL_FOR_PLAYER) continue; //不是玩家用的技能

			if(pSkillDef->m_nMenPai == Get_MenPai() || INVALID_ID == pSkillDef->m_nMenPai)
			{
				//属于该门派的技能 或是 通用技能
				Time_t nActiveTime = pSkillDef->m_nActiveTime==-1?-1:0;
				Set_Skill(pSkillDef->m_nID, 1, FALSE);
			}
		}

		//设置激活技能为物理攻击
		CGameInterface::GetMe()->Skill_SetActive(CActionSystem::GetMe()->GetAction_SkillID(0));
	}

	//----------------------------------------------------------------------------------------------------------
	//门派改变时聊天窗口的变化
	{
		if(9 == Get_MenPai())
		{
			SCRIPT_SANDBOX::Talk::s_Talk.MenPaiLeave();
		}
		else
		{
			SCRIPT_SANDBOX::Talk::s_Talk.MenPaiJoin((MenPaiID_t)Get_MenPai());
		}
	}
}

INT	CCharacterData::Get_Guild(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);
	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nGuild;
}

VOID CCharacterData::Set_Guild(INT nGuild)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);
	((SDATA_PLAYER_MYSELF*)m_pData)->m_nGuild = nGuild;

	if(((SDATA_PLAYER_MYSELF*)m_pData)->m_nGuild != INVALID_ID)
	{
		SCRIPT_SANDBOX::Talk::s_Talk.GuildJoin(Get_Guild());
	}
	else
	{
		SCRIPT_SANDBOX::Talk::s_Talk.GuildLeave();
	}
}

INT CCharacterData::Get_HairMesh(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);

	return ((SDATA_PLAYER_OTHER*)m_pData)->m_nHairMeshID;
}

VOID CCharacterData::Set_HairMesh(INT nHairMesh)
{
	CHECK_MUST_NOT(CT_MONSTER);

	if(((SDATA_PLAYER_OTHER*)m_pData)->m_nHairMeshID != nHairMesh)
	{
		((SDATA_PLAYER_OTHER*)m_pData)->m_nHairMeshID = nHairMesh;
		((CObject_PlayerOther*)m_pObjRef)->OnDataChanged_HairMesh();
		//每次换新发型都刷新一次发色
		((CObject_PlayerOther*)m_pObjRef)->OnDataChanged_HairColor();
	}

	//Push事件
	STRING strReturn;
	if(m_pObjRef->IsSpecialObject(strReturn))
	{
		CEventSystem::GetMe()->PushEvent(GE_UNIT_HAIR_MESH, strReturn.c_str());
	}
}

UINT CCharacterData::Get_HairColor(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);

	return ((SDATA_PLAYER_OTHER*)m_pData)->m_uHairColor;
}

VOID CCharacterData::Set_HairColor(UINT uHairColor)
{
	CHECK_MUST_NOT(CT_MONSTER);

	if(((SDATA_PLAYER_OTHER*)m_pData)->m_uHairColor != uHairColor)
	{
		((SDATA_PLAYER_OTHER*)m_pData)->m_uHairColor = uHairColor;
		((CObject_PlayerOther*)m_pObjRef)->OnDataChanged_HairColor();
	}

	//Push事件
	STRING strReturn;
	if(m_pObjRef->IsSpecialObject(strReturn))
	{
		CEventSystem::GetMe()->PushEvent(GE_UNIT_HAIR_COLOR, strReturn.c_str());
	}
}

INT CCharacterData::Get_FaceMesh(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);

	return ((SDATA_PLAYER_OTHER*)m_pData)->m_nFaceMeshID;
}

VOID CCharacterData::Set_FaceMesh(INT nFaceMesh)
{
	CHECK_MUST_NOT(CT_MONSTER);

	if(((SDATA_PLAYER_OTHER*)m_pData)->m_nFaceMeshID != nFaceMesh)
	{
		((SDATA_PLAYER_OTHER*)m_pData)->m_nFaceMeshID = nFaceMesh;
		((CObject_PlayerOther*)m_pObjRef)->OnDataChanged_FaceMesh();
	}

	//Push事件
	STRING strReturn;
	if(m_pObjRef->IsSpecialObject(strReturn))
	{
		CEventSystem::GetMe()->PushEvent(GE_UNIT_FACE_MESH, strReturn.c_str());
	}

}

INT CCharacterData::Get_EquipVer(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);

	return ((SDATA_PLAYER_OTHER*)m_pData)->m_nEquipVer;
}

VOID CCharacterData::Set_EquipVer(INT nEquipVer)
{
	CHECK_MUST_NOT(CT_MONSTER);

	BOOL bNeedUpdate = ((SDATA_PLAYER_OTHER*)m_pData)->m_nEquipVer != nEquipVer;

	((SDATA_PLAYER_OTHER*)m_pData)->m_nEquipVer = nEquipVer;

	if(bNeedUpdate)
	{
		((CObject_PlayerOther*)m_pObjRef)->OnDataChanged_EquipVer();

		//Push事件
		STRING strReturn;
		if(m_pObjRef->IsSpecialObject(strReturn))
		{
			CEventSystem::GetMe()->PushEvent(GE_UNIT_EQUIP_VER, strReturn.c_str());
		}
	}
}

INT CCharacterData::Get_Equip(HUMAN_EQUIP point) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	TDAssert(point>=0 && point<HEQUIP_NUMBER);

	//如果是玩家自己，存储物品实例
	return	((SDATA_PLAYER_OTHER*)m_pData)->m_nEquipmentID[point];
}

VOID CCharacterData::Set_Equip(HUMAN_EQUIP point, INT nID)
{
	CHECK_MUST_NOT(CT_MONSTER);
	TDAssert(point>=0 && point<HEQUIP_NUMBER);
	
	if(((SDATA_PLAYER_OTHER*)m_pData)->m_nEquipmentID[point] != nID)
	{
		((SDATA_PLAYER_OTHER*)m_pData)->m_nEquipmentID[point] = nID;

		//通知角色层，更改外表
		((CObject_PlayerOther*)m_pObjRef)->OnDataChanged_Equip(point);

		//Push事件
		STRING strReturn;
		if(m_pObjRef->IsSpecialObject(strReturn) )
		{
			CEventSystem::GetMe()->PushEvent(GE_UNIT_EQUIP, strReturn.c_str());

			if ( point == HEQUIP_WEAPON )
			{
				CEventSystem::GetMe()->PushEvent(GE_UNIT_EQUIP_WEAPON, strReturn.c_str());
			}
		}
	}
}

INT CCharacterData::Get_EquipGem(HUMAN_EQUIP point) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	TDAssert(point>=0 && point<HEQUIP_NUMBER);

	//如果是玩家自己，存储物品实例
	return	((SDATA_PLAYER_OTHER*)m_pData)->m_nEquipmentGemID[point];
}

VOID CCharacterData::Set_EquipGem(HUMAN_EQUIP point, INT nID)
{
	CHECK_MUST_NOT(CT_MONSTER);
	TDAssert(point>=0 && point<HEQUIP_NUMBER);
	
	((SDATA_PLAYER_OTHER*)m_pData)->m_nEquipmentGemID[point] = nID;

	//通知角色层，更改外表
	((CObject_PlayerOther*)m_pObjRef)->OnDataChanged_Equip(point);

	//Push事件
	STRING strReturn;
	if(m_pObjRef->IsSpecialObject(strReturn) )
	{
		CEventSystem::GetMe()->PushEvent(GE_UNIT_EQUIP, strReturn.c_str());

		if ( point == HEQUIP_WEAPON )
		{
			CEventSystem::GetMe()->PushEvent(GE_UNIT_EQUIP_WEAPON, strReturn.c_str());
		}
	}
}

BOOL CCharacterData::Get_HaveTeamFlag(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);

	return ((SDATA_PLAYER_OTHER*)m_pData)->m_bTeamFlag;
}

VOID CCharacterData::Set_HaveTeamFlag(BOOL flag)
{
	CHECK_MUST_NOT(CT_MONSTER);

	((SDATA_PLAYER_OTHER*)m_pData)->m_bTeamFlag = flag;
}

BOOL CCharacterData::Get_TeamLeaderFlag(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);

	return ((SDATA_PLAYER_OTHER*)m_pData)->m_bTeamLeaderFlag;
}

VOID CCharacterData::Set_TeamLeaderFlag(BOOL flag)
{
	CHECK_MUST_NOT(CT_MONSTER);

	((SDATA_PLAYER_OTHER*)m_pData)->m_bTeamLeaderFlag = flag;

	m_pObjRef->OnDataChanged_TeamLeaderFlag();
}

BOOL CCharacterData::Get_TeamFullFlag(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);

	return ((SDATA_PLAYER_OTHER*)m_pData)->m_bTeamFullFlag;
}

VOID CCharacterData::Set_TeamFullFlag(BOOL flag)
{
	CHECK_MUST_NOT(CT_MONSTER);

	((SDATA_PLAYER_OTHER*)m_pData)->m_bTeamFullFlag = flag;
}

BOOL CCharacterData::Get_TeamFollowFlag(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);

	return ((SDATA_PLAYER_OTHER*)m_pData)->m_bTeamFollowFlag;
}

VOID CCharacterData::Set_TeamFollowFlag(BOOL flag)
{
	CHECK_MUST_NOT(CT_MONSTER);

	((SDATA_PLAYER_OTHER*)m_pData)->m_bTeamFollowFlag = flag;
}

INT CCharacterData::Get_TeamFollowListCount(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);

	return ((SDATA_PLAYER_OTHER*)m_pData)->m_nTeamFollowListCount;
}

VOID CCharacterData::Set_TeamFollowListCount(INT count)
{
	CHECK_MUST_NOT(CT_MONSTER);

	((SDATA_PLAYER_OTHER*)m_pData)->m_nTeamFollowListCount = count;
}

GUID_t CCharacterData::Get_TeamFollowedMember(INT index) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	Assert( index>=0 && index<Get_TeamFollowListCount() );

	return ((SDATA_PLAYER_OTHER*)m_pData)->m_aTeamFollowList[index];
}

VOID CCharacterData::Set_TeamFollowedMember(INT index, GUID_t guid)
{ // 必须先设置 m_nTeamFollowListCount 才允许进行赋值以避免错误操作
	CHECK_MUST_NOT(CT_MONSTER);
	Assert( index>=0 && index<Get_TeamFollowListCount() );

	((SDATA_PLAYER_OTHER*)m_pData)->m_aTeamFollowList[index] = guid;
}

_TITLE_	CCharacterData::Get_Title(INT index) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	TDAssert(index>=0 && index<MAX_TITLE_SIZE);
	return ((SDATA_PLAYER_OTHER*)m_pData)->m_nTitleList[index];
}

VOID CCharacterData::Set_Title(INT index, _TITLE_ title)
{
	CHECK_MUST_NOT(CT_MONSTER);
	TDAssert(index>=0 && index<MAX_TITLE_SIZE);
	((SDATA_PLAYER_OTHER*)m_pData)->m_nTitleList[index] = title;
//	((CObject_PlayerOther*)m_pObjRef)->OnDataChanged_Titles(index);

	//Push事件
	STRING strReturn;
	if(m_pObjRef->IsSpecialObject(strReturn) )
	{
		CEventSystem::GetMe()->PushEvent(GE_UNIT_TITLE, strReturn.c_str());
	}

}

VOID CCharacterData::Set_TitleNum(INT num )
{
	CHECK_MUST_NOT(CT_MONSTER);
	TDAssert(num>=0 && num<MAX_TITLE_SIZE);
	((SDATA_PLAYER_OTHER*)m_pData)->m_nTitleNum = num;
}

INT CCharacterData::Get_TitleNum() const
{
	CHECK_MUST_NOT(CT_MONSTER);
	return ((SDATA_PLAYER_OTHER*)m_pData)->m_nTitleNum;
}

INT	CCharacterData::Get_CurTitleIndex() const
{
	CHECK_MUST_NOT(CT_MONSTER);
	return ((SDATA_PLAYER_OTHER*)m_pData)->m_nCurTitleIndex;
}
VOID CCharacterData::Set_CurTitleIndex(INT index )
{
	CHECK_MUST_NOT(CT_MONSTER);
	TDAssert(index>=0 && index<MAX_TITLE_SIZE);
	((SDATA_PLAYER_OTHER*)m_pData)->m_nCurTitleIndex = index;
}

VOID CCharacterData::Clear_AllTitles()
{
	CHECK_MUST_NOT(CT_MONSTER);
	memset(((SDATA_PLAYER_OTHER*)m_pData)->m_nTitleList, 0, MAX_TITLE_SIZE*sizeof(_TITLE_));
	((SDATA_PLAYER_OTHER*)m_pData)->m_nTitleNum = 0;
}


//---------------------------------
INT CCharacterData::Get_HP(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nHP;
}

VOID CCharacterData::Set_HP(INT nHP)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nHP = nHP;

	CEventSystem::GetMe()->PushEvent(GE_UNIT_HP, "player" );
}

INT CCharacterData::Get_MP(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nMP;
}

VOID CCharacterData::Set_MP(INT nMP)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nMP = nMP;

	CEventSystem::GetMe()->PushEvent(GE_UNIT_MP, "player" );
}

INT CCharacterData::Get_Exp(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nExp;
}

VOID CCharacterData::Set_Exp(INT nExp)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	if(((SDATA_PLAYER_MYSELF*)m_pData)->m_nExp != -1)
	{
		//经验值变更
		BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;
		if(nExp > ((SDATA_PLAYER_MYSELF*)m_pData)->m_nExp)
		{
			if(bLog)
			{
				int nExpDoubleNum=GETLOW(((SDATA_PLAYER_MYSELF*)m_pData)->m_nDoubleExpTime_Num);
				int nTotalExp=nExp -((SDATA_PLAYER_MYSELF*)m_pData)->m_nExp;
				if(nExpDoubleNum >1)
				{
					STRING strExpMsg = COLORMSGFUNC("WIN_EXP", nTotalExp/nExpDoubleNum);
					ADDTALKMSG(strExpMsg);
					STRING strDoubleExpMsg = COLORMSGFUNC("WIN_ADD_EXP",nTotalExp-nTotalExp/nExpDoubleNum );
					ADDTALKMSG(strDoubleExpMsg);

				}
				else
				{
					STRING strExpMsg = COLORMSGFUNC("WIN_EXP", nTotalExp);
					ADDTALKMSG(strExpMsg);

				}

			}
		}
		else if(nExp == ((SDATA_PLAYER_MYSELF*)m_pData)->m_nExp)
		{
			if(bLog)
			{
				STRING strTemp = COLORMSGFUNC("EXP_OVERFLOW");
				ADDTALKMSG(strTemp);
				strTemp = NOCOLORMSGFUNC("EXP_OVERFLOW");
				ADDNEWDEBUGMSG(strTemp);
			}
		}
	}

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nExp = nExp;

	CEventSystem::GetMe()->PushEvent( GE_UNIT_EXP, "player" );

}

INT CCharacterData::Get_MaxExp(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	DBC_DEFINEHANDLE(s_pPlayerExpDBC, DBC_CHARACTER_EXPLEVEL);
	const _DBC_CHARACTER_EXPLEVEL* pCharExpLevel = (const _DBC_CHARACTER_EXPLEVEL*)s_pPlayerExpDBC->Search_Index_EQU(Get_Level());

	if(!pCharExpLevel)	return -1;
	else				return pCharExpLevel->nEffectNeed;
}

INT CCharacterData::Get_Money(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nMoney;
}

VOID CCharacterData::Set_Money(INT nMoney)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nMoney = nMoney;

	CEventSystem::GetMe()->PushEvent( GE_UNIT_MONEY, "player" );

	if(0 < Get_Level() && 100 >= Get_Level())
	{
		DBC_DEFINEHANDLE(s_pLvMaxMoneyDBC, DBC_LV_MAXMONEY);
		const _DBC_LV_MAXMONEY* pMax = (const _DBC_LV_MAXMONEY*)s_pLvMaxMoneyDBC->Search_Index_EQU(Get_Level());
		if(!pMax) return;

		BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;
		
		INT myBankMoney = CDataPool::GetMe()->UserBank_GetBankMoney();
		if((((SDATA_PLAYER_MYSELF*)m_pData)->m_nMoney + myBankMoney) > pMax->nMaxMoney && bLog)
		{
			STRING strWarn = COLORMSGFUNC("MONEY_OVERFLOW");
			ADDTALKMSG(strWarn);
		}
			
	}
}

PET_GUID_t CCharacterData::Get_CurrentPetGUID(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_guidCurrentPet;
}

VOID CCharacterData::Set_CurrentPetGUID(PET_GUID_t guid)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	if ( guid != ((SDATA_PLAYER_MYSELF*)m_pData)->m_guidCurrentPet )
	{
		((SDATA_PLAYER_MYSELF*)m_pData)->m_guidCurrentPet = guid;

		CActionSystem::GetMe()->UpdateToolBarForPetSkill();
		CEventSystem::GetMe()->PushEvent(GE_UPDATE_PET_PAGE);
	}
}

INT CCharacterData::Get_Rage(VOID)const
{
	return ((SDATA_CHARACTER*)m_pData)->m_nRage;
}

VOID CCharacterData::Set_Rage(INT nRage)
{
	((SDATA_CHARACTER*)m_pData)->m_nRage = nRage;

	STRING strReturn;
	if(m_pObjRef->IsSpecialObject(strReturn) )
	{
		CEventSystem::GetMe()->PushEvent( GE_UNIT_RAGE, strReturn.c_str());
	}
}

INT CCharacterData::Get_StrikePoint(VOID)const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);
	
	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nStrikePoint;
}

VOID CCharacterData::Set_StrikePoint(INT nStrikePoint)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nStrikePoint = nStrikePoint;
	CEventSystem::GetMe()->PushEvent( GE_UNIT_STRIKEPOINT, "player" );
}

INT CCharacterData::Get_RMB(VOID)const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nRMB;
}

VOID CCharacterData::Set_RMB(INT nRMB)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);
	((SDATA_PLAYER_MYSELF*)m_pData)->m_nRMB = nRMB;
	CEventSystem::GetMe()->PushEvent( GE_UNIT_RMB, "player");
}

//INT CCharacterData::Get_MaxVigor(VOID)const
//{
//	CHECK_MUST_NOT(CT_MONSTER);
//	CHECK_MUST_NOT(CT_PLAYEROTHER);
//
//	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nMaxVigor;
//}
//
//VOID CCharacterData::Set_MaxVigor(INT nMaxVigor)
//{
//	CHECK_MUST_NOT(CT_MONSTER);
//	CHECK_MUST_NOT(CT_PLAYEROTHER);
//
//	((SDATA_PLAYER_MYSELF*)m_pData)->m_nMaxVigor = nMaxVigor;
//	CEventSystem::GetMe()->PushEvent( GE_UNIT_MAX_VIGOR, "player");
//}
//
INT CCharacterData::Get_DoubleExpTime_Num(VOID)const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nDoubleExpTime_Num;
}

VOID CCharacterData::Set_DoubleExpTime_Num(INT nDoubleExpTime_Num)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;
	if(nDoubleExpTime_Num < ((SDATA_PLAYER_MYSELF*)m_pData)->m_nDoubleExpTime_Num)
	{
		if(bLog)
		{
			//STRING strMsg = COLORMSGFUNC("GMDP_CharacterData_Energy_Lost", ((SDATA_PLAYER_MYSELF*)m_pData)->m_nEnergy - nEnergy);
			//ADDTALKMSG(strMsg);
		}
	}

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nDoubleExpTime_Num = nDoubleExpTime_Num;
	//CEventSystem::GetMe()->PushEvent( GE_UNIT_ENERGY, "player");
}
//
//INT CCharacterData::Get_MaxEnergy(VOID)const
//{
//	CHECK_MUST_NOT(CT_MONSTER);
//	CHECK_MUST_NOT(CT_PLAYEROTHER);
//
//	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nMaxEnergy;
//}
//
//VOID CCharacterData::Set_MaxEnergy(INT nMaxEnergy)
//{
//	CHECK_MUST_NOT(CT_MONSTER);
//	CHECK_MUST_NOT(CT_PLAYEROTHER);
//
//	((SDATA_PLAYER_MYSELF*)m_pData)->m_nMaxEnergy = nMaxEnergy;
//	CEventSystem::GetMe()->PushEvent( GE_UNIT_MAX_ENERGY, "player");
//}

//INT CCharacterData::Get_GoodBadValue(VOID)const
//{
//	CHECK_MUST_NOT(CT_MONSTER);
//	CHECK_MUST_NOT(CT_PLAYEROTHER);
//
//	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nGoodBadValue;
//}
//
//VOID CCharacterData::Set_GoodBadValue(INT nValue)
//{
//	CHECK_MUST_NOT(CT_MONSTER);
//	CHECK_MUST_NOT(CT_PLAYEROTHER);
//
//	((SDATA_PLAYER_MYSELF*)m_pData)->m_nGoodBadValue = nValue;
//}

INT CCharacterData::Get_STR(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nSTR;
}

VOID CCharacterData::Set_STR(INT nSTR)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nSTR = nSTR;

	CEventSystem::GetMe()->PushEvent( GE_UNIT_STR, "player" );
}

INT CCharacterData::Get_SPR(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nSPR;
}

VOID CCharacterData::Set_SPR(INT nSPR)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nSPR = nSPR;
	CEventSystem::GetMe()->PushEvent( GE_UNIT_SPR, "player" );
}

INT CCharacterData::Get_CON(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nCON;
}

VOID CCharacterData::Set_CON(INT nCON)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nCON = nCON;
	CEventSystem::GetMe()->PushEvent( GE_UNIT_CON, "player" );
}

INT CCharacterData::Get_INT(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nINT;
}

VOID CCharacterData::Set_INT(INT nINT)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nINT = nINT;
	CEventSystem::GetMe()->PushEvent( GE_UNIT_INT, "player" );
}

INT CCharacterData::Get_DEX(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nDEX;
}

VOID CCharacterData::Set_DEX(INT nDEX)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nDEX = nDEX;
	CEventSystem::GetMe()->PushEvent( GE_UNIT_DEX, "player" );
}

INT CCharacterData::Get_PointRemain(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nPoint_Remain;
}

VOID CCharacterData::Set_PointRemain(INT nPoint)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nPoint_Remain = nPoint;
	CEventSystem::GetMe()->PushEvent( GE_UNIT_POINT_REMAIN, "player" );
}

INT CCharacterData::Get_AttPhysics(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nAtt_Physics;
}

VOID CCharacterData::Set_AttPhysics(INT nAttPhysics)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nAtt_Physics = nAttPhysics;
	CEventSystem::GetMe()->PushEvent( GE_UNIT_ATT_PHYSICS, "player" );
}

INT CCharacterData::Get_AttMagic(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nAtt_Magic;
}

VOID CCharacterData::Set_AttMagic(INT nAttMagic)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nAtt_Magic = nAttMagic;
	CEventSystem::GetMe()->PushEvent( GE_UNIT_ATT_MAGIC, "player" );
}

INT CCharacterData::Get_DefPhysics(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nDef_Physics;
}

VOID CCharacterData::Set_DefPhysics(INT nDefPhysics)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nDef_Physics = nDefPhysics;
	CEventSystem::GetMe()->PushEvent( GE_UNIT_DEF_PHYSICS, "player" );
}

INT CCharacterData::Get_DefMagic(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nDef_Magic;
}

VOID CCharacterData::Set_DefMagic(INT nDefMagic)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nDef_Magic = nDefMagic;
	CEventSystem::GetMe()->PushEvent( GE_UNIT_DEF_MAGIC, "player" );
}

INT CCharacterData::Get_MaxHP(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nMaxHP;
}

VOID CCharacterData::Set_MaxHP(INT nMaxHP)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nMaxHP = nMaxHP;
	CEventSystem::GetMe()->PushEvent( GE_UNIT_MAX_HP, "player" );
}

INT CCharacterData::Get_MaxMP(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nMaxMP;
}

VOID CCharacterData::Set_MaxMP(INT nMaxMP)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nMaxMP = nMaxMP;
	CEventSystem::GetMe()->PushEvent( GE_UNIT_MAX_MP, "player" );
}

INT CCharacterData::Get_HPRespeed(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nHP_ReSpeed;
}

VOID CCharacterData::Set_HPRespeed(INT nHPRespeed)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nHP_ReSpeed = nHPRespeed;
	CEventSystem::GetMe()->PushEvent( GE_UNIT_HP_RE_SPEED, "player" );
}

INT CCharacterData::Get_MPRespeed(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nMP_ReSpeed;
}

VOID CCharacterData::Set_MPRespeed(INT nMPRespeed)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nMP_ReSpeed = nMPRespeed;
	CEventSystem::GetMe()->PushEvent( GE_UNIT_MP_RE_SPEED, "player" );
}

INT CCharacterData::Get_Hit(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nHit;
}

VOID CCharacterData::Set_Hit(INT nHit)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nHit = nHit;
	CEventSystem::GetMe()->PushEvent( GE_UNIT_HIT, "player" );
}

INT CCharacterData::Get_Miss(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nMiss;
}

VOID CCharacterData::Set_Miss(INT nMiss)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nMiss = nMiss;
	CEventSystem::GetMe()->PushEvent( GE_UNIT_MISS, "player" );
}

INT CCharacterData::Get_CritRate(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nCritRate;
}

VOID CCharacterData::Set_CritRate(INT nCritRate)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nCritRate = nCritRate;
	CEventSystem::GetMe()->PushEvent( GE_UNIT_CRIT_RATE, "player" );
}

INT CCharacterData::Get_AttackSpeed(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nAttackSpeed;
}

VOID CCharacterData::Set_AttackSpeed(INT nSpeed)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nAttackSpeed = nSpeed;
	CEventSystem::GetMe()->PushEvent( GE_UNIT_ATT_SPEED, "player" );
}

INT CCharacterData::Get_AttCold(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nAtt_Cold;
}

VOID CCharacterData::Set_AttCold(INT nAttCold)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nAtt_Cold = nAttCold;
	CEventSystem::GetMe()->PushEvent( GE_UNIT_ATT_COLD, "player" );
}

INT CCharacterData::Get_DefCold(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nDef_Cold;
}

VOID CCharacterData::Set_DefCold(INT nDefCold)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nDef_Cold = nDefCold;
	CEventSystem::GetMe()->PushEvent( GE_UNIT_DEF_COLD, "player" );
}

INT CCharacterData::Get_AttFire(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nAtt_Fire;
}

VOID CCharacterData::Set_AttFire(INT nAttFire)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nAtt_Fire = nAttFire;
	CEventSystem::GetMe()->PushEvent( GE_UNIT_ATT_FIRE, "player" );
}

INT CCharacterData::Get_DefFire(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nDef_Fire;
}

VOID CCharacterData::Set_DefFire(INT nDefFire)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nDef_Fire = nDefFire;
	CEventSystem::GetMe()->PushEvent( GE_UNIT_DEF_FIRE, "player" );
}

INT CCharacterData::Get_AttLight(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nAtt_Light;
}

VOID CCharacterData::Set_AttLight(INT nAttLight)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nAtt_Light = nAttLight;
	CEventSystem::GetMe()->PushEvent( GE_UNIT_ATT_LIGHT, "player" );
}

INT CCharacterData::Get_DefLight(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nDef_Light;
}

VOID CCharacterData::Set_DefLight(INT nDefLight)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nDef_Light = nDefLight;
	CEventSystem::GetMe()->PushEvent( GE_UNIT_DEF_LIGHT, "player" );
}

INT CCharacterData::Get_AttPosion(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nAtt_Posion;
}

VOID CCharacterData::Set_AttPosion(INT nAttPosion)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nAtt_Posion = nAttPosion;
	CEventSystem::GetMe()->PushEvent( GE_UNIT_ATT_POSION, "player" );
}

INT CCharacterData::Get_DefPosion(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_nDef_Posion;
}

VOID CCharacterData::Set_DefPosion(INT nDefPosion)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	((SDATA_PLAYER_MYSELF*)m_pData)->m_nDef_Posion = nDefPosion;
	CEventSystem::GetMe()->PushEvent( GE_UNIT_DEF_POSION, "player" );
}

BOOL CCharacterData::IsLimitMove(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_bLimitMove;
}

VOID CCharacterData::SetLimitMove(BOOL bSet)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	((SDATA_PLAYER_MYSELF*)m_pData)->m_bLimitMove = bSet;
}
//
//BOOL CCharacterData::IsLimitUseSkill(VOID) const
//{
//	CHECK_MUST_NOT(CT_MONSTER);
//	CHECK_MUST_NOT(CT_PLAYEROTHER);
//
//	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_bLimitUseSkill;
//}
//
//VOID CCharacterData::SetLimitUseSkill(BOOL bSet)
//{
//	CHECK_MUST_NOT(CT_MONSTER);
//	CHECK_MUST_NOT(CT_PLAYEROTHER);
//
//	((SDATA_PLAYER_MYSELF*)m_pData)->m_bLimitUseSkill = bSet;
//}
//
//BOOL CCharacterData::IsLimitHandle(VOID) const
//{
//	CHECK_MUST_NOT(CT_MONSTER);
//	CHECK_MUST_NOT(CT_PLAYEROTHER);
//
//	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_bLimitHandle;
//}
//
//VOID CCharacterData::SetLimitHandle(BOOL bSet)
//{
//	CHECK_MUST_NOT(CT_MONSTER);
//	CHECK_MUST_NOT(CT_PLAYEROTHER);
//
//	((SDATA_PLAYER_MYSELF*)m_pData)->m_bLimitHandle = bSet;
//}

BOOL CCharacterData::IsHaveCanActionFlag1( VOID )const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_bCanActionFlag1;
}

VOID CCharacterData::SetCanActionFlag1( BOOL bSet )
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	((SDATA_PLAYER_MYSELF*)m_pData)->m_bCanActionFlag1 = bSet;
}

BOOL CCharacterData::IsHaveCanActionFlag2( VOID )const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_bCanActionFlag2;
}

VOID CCharacterData::SetCanActionFlag2( BOOL bSet )
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	((SDATA_PLAYER_MYSELF*)m_pData)->m_bCanActionFlag2 = bSet;
}

// 是否已设置二级密码
BOOL CCharacterData::Get_IsMinorPwdSetup(  BOOL bOpenDlg ) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	BOOL bIsSetupPw = ((SDATA_PLAYER_MYSELF*)m_pData)->m_bIsMinorPwdSetup;
	if(!bIsSetupPw)
	{
		// 如果没有设置密码， 就打开设置密码对话框。
		if(bOpenDlg)
		{
			CEventSystem::GetMe()->PushEvent(GE_MINORPASSWORD_OPEN_SET_PASSWORD_DLG);
		}
	}
	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_bIsMinorPwdSetup;
}

VOID CCharacterData::Set_SetMinorPwdSetup( INT bSet )
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	((SDATA_PLAYER_MYSELF*)m_pData)->m_bIsMinorPwdSetup = bSet;
}

// 是否已经解锁二级密码
BOOL CCharacterData::Get_IsMinorPwdUnlocked(  BOOL bOpenDlg ) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	BOOL bIsUnLockPw = ((SDATA_PLAYER_MYSELF*)m_pData)->m_bIsMinorPwdUnlocked;
	if(!bIsUnLockPw)
	{
		if(bOpenDlg)
		{
			// 如果没有解锁密码， 就打开解锁密码对话框。
			CEventSystem::GetMe()->PushEvent(GE_MINORPASSWORD_OPEN_UNLOCK_PASSWORD_DLG);
		}
	}
	return bIsUnLockPw;
}

VOID CCharacterData::Set_SetMinorPwdUnlocked( INT nSet )
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	if(nSet==1)
	{
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "解锁密码成功");
		((SDATA_PLAYER_MYSELF*)m_pData)->m_bIsMinorPwdUnlocked = TRUE;
	}
	else if(nSet==0)
	{
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "解锁密码失败");
		((SDATA_PLAYER_MYSELF*)m_pData)->m_bIsMinorPwdUnlocked = FALSE;
	}
	else
	{
		((SDATA_PLAYER_MYSELF*)m_pData)->m_bIsMinorPwdUnlocked = FALSE;
	}
}

//-------------------------------
const SKILLCLASS_MAP& CCharacterData::Get_SkillClass(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return ((SDATA_PLAYER_MYSELF*)m_pData)->m_theSkillClass;
}

const SCLIENT_SKILLCLASS* CCharacterData::Get_SkillClass(INT nID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	SKILLCLASS_MAP::iterator it = ((SDATA_PLAYER_MYSELF*)m_pData)->m_theSkillClass.find(nID);
	if(it == ((SDATA_PLAYER_MYSELF*)m_pData)->m_theSkillClass.end()) return NULL;

	return &(it->second);
}

VOID CCharacterData::Set_SkillClass(INT nID, INT nLevel)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	SKILLCLASS_MAP& mapSkillClass = ((SDATA_PLAYER_MYSELF*)m_pData)->m_theSkillClass;

	SKILLCLASS_MAP::iterator it = mapSkillClass.find(nID);

	//新的技能系
	if(it == mapSkillClass.end())
	{
		//查找心法定义
		DBC_DEFINEHANDLE(s_pXinFaDBC, DBC_SKILL_XINFA);
		const _DBC_SKILL_XINFA* pDefine = (const _DBC_SKILL_XINFA*)s_pXinFaDBC->Search_Index_EQU(nID);
		if(!pDefine) return;

		SCLIENT_SKILLCLASS newSkillClass;
		newSkillClass.m_pDefine = pDefine;
		newSkillClass.m_nPosIndex = (INT)mapSkillClass.size();
		mapSkillClass.insert(std::make_pair(nID, newSkillClass));
	}
	//已有心法
	//else
	//{
	//	it->second.m_nLevel = nLevel;
	//	it->second.m_bLeaned = nLevel > 0;
	//}

	// 当心法改变时应该改变技能
	//Update_SkillByXinFaID( nID );

	//通知ActionSystem
	//CActionSystem::GetMe()->SkillClass_Update();

//	((CObject_PlayerMySelf*)m_pObjRef)->OnDataChanged_XinFa(nID);
}

const SSKILL_MAP& CCharacterData::Get_Skill(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return (((SDATA_PLAYER_MYSELF*)m_pData)->m_theSkill);
}

const SCLIENT_SKILL* CCharacterData::Get_Skill(INT nID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	SSKILL_MAP::iterator it = ((SDATA_PLAYER_MYSELF*)m_pData)->m_theSkill.find(nID);
	if(it == ((SDATA_PLAYER_MYSELF*)m_pData)->m_theSkill.end()) return NULL;

	return &(it->second);
}

VOID CCharacterData::Set_Skill(INT nID, BYTE nLevel, BOOL bLean)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	DBC_DEFINEHANDLE(s_pSkillDBC, DBC_SKILL_DATA);
	DBC_DEFINEHANDLE(s_pSkillDepleteDBC, DBC_SKILL_DEPLETE);

	const _DBC_SKILL_DATA* pDefine = (const _DBC_SKILL_DATA*)s_pSkillDBC->Search_Index_EQU(nID);
	if(!pDefine) return;

	INT nSkillID = nID * 100 + nLevel;
	if( nID == 0 )
	{
		nSkillID = nID;
		nLevel = 0;
	}

	SSKILL_MAP& mapSkill = ((SDATA_PLAYER_MYSELF*)m_pData)->m_theSkill;
	SSKILL_MAP::iterator it = mapSkill.find(nSkillID);

	//新的技能
	if(it == mapSkill.end())
	{
		INT nDepleteID = INVALID_ID;
		if ( pDefine->m_nSkillClass != INVALID_ID )
		{
			//是否是本门派的门派技能
			if(pDefine->m_nMenPai != this->Get_MenPai()) return;

			const SCLIENT_SKILLCLASS* pSkillClass = Get_SkillClass( pDefine->m_nSkillClass );
			if ( pSkillClass != NULL )
				nDepleteID = pDefine->m_anSkillByLevel[nLevel-1];
		}
		if ( nDepleteID == INVALID_ID )
			nDepleteID = pDefine->m_anSkillByLevel[0];

		const _DBC_SKILL_DEPLETE* pDeplete = (const _DBC_SKILL_DEPLETE*)s_pSkillDepleteDBC->Search_Index_EQU(nDepleteID);

		SCLIENT_SKILL newSkill;
		newSkill.m_pDefine = pDefine;
		newSkill.m_pDeplete = pDeplete;
		newSkill.m_bLeaned = bLean;
		newSkill.m_nLevel = nLevel;
		newSkill.m_nPosIndex = (INT)mapSkill.size();
		mapSkill.insert(std::make_pair(nSkillID, newSkill));
	}
	else
	{
		it->second.m_bLeaned = bLean;
	}

	//通知ActionSystem
	CActionSystem::GetMe()->UserSkill_Update();

	//查找心法定义
	//SSKILL_MAP& mapSkill = ((SDATA_PLAYER_MYSELF*)m_pData)->m_theSkill;

	//SSKILL_MAP::iterator it = mapSkill.find(nID);

	////新的技能
	//if(it == mapSkill.end())
	//{
	//	//查找心法定义
	//	DBC_DEFINEHANDLE(s_pSkillDBC, DBC_SKILL_DATA);
	//	const _DBC_SKILL_DATA* pDefine = (const _DBC_SKILL_DATA*)s_pSkillDBC->Search_Index_EQU(nID);
	//	if(!pDefine) return;
	//	//
	//	INT nDepleteID = INVALID_ID;
	//	if ( pDefine->m_nSkillClass != INVALID_ID )
	//	{
	//		//是否是本门派的门派技能
	//		if(pDefine->m_nMenPai != this->Get_MenPai()) 
	//		{
	//			return;
	//		}

	//		const SCLIENT_SKILLCLASS* pSkillClass = Get_SkillClass( pDefine->m_nSkillClass );
	//		if ( pSkillClass != NULL )
	//		{
	//			if ( nLevel >= 0 && nLevel < 12 )
	//				nDepleteID = pDefine->m_anSkillByLevel[nLevel];
	//		}
	//	}
	//	if ( nDepleteID == INVALID_ID )
	//	{
	//		nDepleteID = pDefine->m_anSkillByLevel[0];
	//	}
	//	const _DBC_SKILL_DEPLETE* pDeplete = NULL;
	//	if ( nDepleteID != INVALID_ID )
	//	{
	//		DBC_DEFINEHANDLE(s_pSkillDepleteDBC, DBC_SKILL_DEPLETE);
	//		pDeplete = (const _DBC_SKILL_DEPLETE*)s_pSkillDepleteDBC->Search_Index_EQU(nDepleteID);
	//	}

	//	SCLIENT_SKILL newSkill;
	//	newSkill.m_pDefine = pDefine;
	//	newSkill.m_pDeplete = pDeplete;
	//	newSkill.m_bLeaned = bLean;
	//	newSkill.m_nPosIndex = (INT)mapSkill.size();

	//	mapSkill.insert(std::make_pair(nID, newSkill));
	//}
	//else
	////已有心法
	//{
	//	it->second.m_bLeaned = bLean;
	//}


//	((CObject_PlayerMySelf*)m_pObjRef)->OnDataChanged_Skill(nID);
}

VOID CCharacterData::Skill_CleanAll(void)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	SSKILL_MAP& mapSkill = ((SDATA_PLAYER_MYSELF*)m_pData)->m_theSkill;
	mapSkill.clear();

	//通知ActionSystem
	CActionSystem::GetMe()->UserSkill_Update();
}

//VOID CCharacterData::Update_SkillByXinFaID( INT nXinFaID )
//{
//	CHECK_MUST_NOT(CT_MONSTER);
//	CHECK_MUST_NOT(CT_PLAYEROTHER);
//
//	SSKILL_MAP& mapSkill = ((SDATA_PLAYER_MYSELF*)m_pData)->m_theSkill;
//	SSKILL_MAP::iterator itCur, itEnd;
//	itCur = mapSkill.begin();
//	itEnd = mapSkill.end();
//	SCLIENT_SKILL *pSkill;
//	while( itCur != itEnd )
//	{
//		pSkill = &(itCur->second);
//		if ( pSkill->m_pDefine->m_nXinFaID == nXinFaID )
//		{
//			//
//			INT nDepleteID = INVALID_ID;
//			if ( pSkill->m_pDefine->m_nXinFaID != INVALID_ID )
//			{
//				const SCLIENT_SKILLCLASS* pClass = Get_SkillClass( pSkill->m_pDefine->m_nXinFaID );
//				if ( pClass != NULL )
//				{
//					INT nLevel = pClass->m_nLevel / 10;
//					if ( nLevel >= 0 && nLevel < 12 )
//						nDepleteID = pSkill->m_pDefine->m_anSkillByLevel[nLevel];
//				}
//			}
//			if ( nDepleteID == INVALID_ID )
//			{
//				nDepleteID = pSkill->m_pDefine->m_anSkillByLevel[0];
//			}
//			const _DBC_SKILL_DEPLETE* pDeplete = NULL;
//			if ( nDepleteID != INVALID_ID )
//			{
//				DBC_DEFINEHANDLE(s_pSkillDepleteDBC, DBC_SKILL_DEPLETE);
//				pDeplete = (const _DBC_SKILL_DEPLETE*)s_pSkillDepleteDBC->Search_Index_EQU(nDepleteID);
//			}
//
//			pSkill->m_pDeplete = pDeplete;
//		}
//
//		itCur++;
//	}
//
//	//通知ActionSystem
//	CActionSystem::GetMe()->UserSkill_Update();
//}

const SLIFEABILITY_MAP& CCharacterData::Get_LifeAbility(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return (((SDATA_PLAYER_MYSELF*)m_pData)->m_theLifeAbility);
}

const SCLIENT_LIFEABILITY* CCharacterData::Get_LifeAbility(INT nID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	SLIFEABILITY_MAP::iterator it = ((SDATA_PLAYER_MYSELF*)m_pData)->m_theLifeAbility.find(nID);
	if(it == ((SDATA_PLAYER_MYSELF*)m_pData)->m_theLifeAbility.end()) return NULL;

	return &(it->second);
}

VOID CCharacterData::Set_LifeAbility(INT nID, INT nLevel, INT nExp)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	SLIFEABILITY_MAP& mapLifeAbility = ((SDATA_PLAYER_MYSELF*)m_pData)->m_theLifeAbility;

	SLIFEABILITY_MAP::iterator it = mapLifeAbility.find(nID);

	if ( nExp > 0 )
	{
		nExp /= 100; // 100 以下的部分只用于服务器端程序内部计算
	}

	//新的生活技能
	if(it == mapLifeAbility.end())
	{
		//查找心法定义
		DBC_DEFINEHANDLE(s_pLifeAbilityDBC, DBC_LIFEABILITY_DEFINE);
		const _DBC_LIFEABILITY_DEFINE* pDefine = (const _DBC_LIFEABILITY_DEFINE*)s_pLifeAbilityDBC->Search_Index_EQU(nID);

		TDAssert(pDefine);

		SCLIENT_LIFEABILITY newLifeAbility;
		newLifeAbility.m_pDefine = pDefine;
		newLifeAbility.m_nPosIndex = (INT)mapLifeAbility.size();
		newLifeAbility.m_nLevel = nLevel;

		if ( nExp < 0 )
		{
			nExp = 0;
		}

		newLifeAbility.m_nExp = nExp;

		mapLifeAbility.insert(std::make_pair(nID, newLifeAbility));
	}
	//已有心法
	{
		if(nLevel >= 0) it->second.m_nLevel = nLevel;
		if(nExp >= 0) it->second.m_nExp = nExp;
	}

	//通知ActionSystem
	CActionSystem::GetMe()->UserLifeAbility_Update();

//	((CObject_PlayerMySelf*)m_pObjRef)->OnDataChanged_LifeAbility(nID);
}

const SPRESCR_MAP& CCharacterData::Get_Prescr(VOID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	return (((SDATA_PLAYER_MYSELF*)m_pData)->m_theSprescr);
}

const SCLIENT_PRESCR* CCharacterData::Get_Prescr(INT nID) const
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	SPRESCR_MAP::iterator it = ((SDATA_PLAYER_MYSELF*)m_pData)->m_theSprescr.find(nID);
	if(it == ((SDATA_PLAYER_MYSELF*)m_pData)->m_theSprescr.end()) return NULL;

	return &(it->second);
}

VOID CCharacterData::Set_Prescr(INT nID, BOOL bLean)
{
	CHECK_MUST_NOT(CT_MONSTER);
	CHECK_MUST_NOT(CT_PLAYEROTHER);

	SPRESCR_MAP& mapSprescr = ((SDATA_PLAYER_MYSELF*)m_pData)->m_theSprescr;

	SPRESCR_MAP::iterator it = mapSprescr.find(nID);

	//新的配方
	if(bLean && it==mapSprescr.end())
	{
		//查找配方定义
		DBC_DEFINEHANDLE(s_pPrescrDBC, DBC_LIFEABILITY_ITEMCOMPOSE);
		const _DBC_LIFEABILITY_ITEMCOMPOSE* pDefine = 
			(const _DBC_LIFEABILITY_ITEMCOMPOSE*)s_pPrescrDBC->Search_Index_EQU(nID);

		SCLIENT_PRESCR newSprescr;
		newSprescr.m_pDefine = pDefine;

		mapSprescr.insert(std::make_pair(nID, newSprescr));
	}
	else if(!bLean && it!=mapSprescr.end())
	{
		mapSprescr.erase(it);
	}

//	((CObject_PlayerMySelf*)m_pObjRef)->OnDataChanged_Sprescr(nID);
}

BOOL	CCharacterData::Get_IsInStall(VOID) const
{//获得当前摊位
	return ((SDATA_PLAYER_OTHER*)m_pData)->m_bIsInStall;
}
VOID	CCharacterData::Set_IsInStall(BOOL bIsInStall)
{//设置摊位
	((SDATA_PLAYER_OTHER*)m_pData)->m_bIsInStall = bIsInStall;
	m_pObjRef->OnDataChanged_IsInStall();
}

LPCTSTR	CCharacterData::Get_StallName(VOID) const
{//获得摊位名
	return ((SDATA_PLAYER_OTHER*)m_pData)->m_strStallName.c_str();
}
VOID	CCharacterData::Set_StallName(LPCTSTR szName)
{//设置摊位名
	((SDATA_PLAYER_OTHER*)m_pData)->m_strStallName = szName;
	m_pObjRef->OnDataChanged_StallName();
}



