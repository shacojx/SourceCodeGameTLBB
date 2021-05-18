// DetailAttrib_Player.cpp

#include "StdAfx.h"
#include "TDTimeSystem.h"
#include "..\Procedure\GameProcedure.h"
#include "SkillDataMgr.h"
#include "DetailAttrib_Player.h"
#include "..\Event\GMEventSystem.h"
#include "..\DBC\GMDataBase.h"
#include "TDException.h"
#include "..\Action\GMActionSystem.h"

// temp code {
// 公式模拟
UINT RunFormula( UINT dwFormulaID, UINT dwParam1, UINT dwParam2, UINT dwParam3 )
{
	return (dwParam3 - dwParam2)/((dwParam1)?(dwParam1):(1)) + dwParam2;
}

INT RunFormula( UINT dwFormulaID, UINT dwParam1, INT nParam2, INT nParam3 )
{
	return (nParam3 - nParam2)/((dwParam1)?(dwParam1):(1)) + nParam2;
}

FLOAT RunFormula( UINT dwFormulaID, UINT dwParam1, FLOAT fParam2, FLOAT fParam3 )
{
	return (fParam3 - fParam2)/((dwParam1)?(dwParam1):(1)) + fParam2;
}
// }

/*
INT SClientSkill::GetCoolDownTime( VOID )const
{
	return m_pOwner->GetCoolDownTime( m_pSkillData->m_uCoolDownID );
}

INT SClientSkill::GetCoolDownTotalTime( VOID )const
{
	return m_pOwner->GetCoolDownTotalTime( m_pSkillData->m_uCoolDownID );
}
*/
CDetailAttrib_Player::CDetailAttrib_Player( VOID )
{
	m_dwCanPickMissionItemIDNum				= 0;
	UINT i;
	for ( i = 0; i < MAX_CHAR_CAN_PICK_MISSION_ITEM_NUM; i++ )
		m_adwCanPickMissionItemList[i]		= UINT_MAX;
/*
	for ( i = 0; i < COOLDOWN_LIST_SIZE; i++ )
	{
		m_anCoolDownTime[i]			= -1;
		m_anCoolDownTotalTime[i]	= -1;
	}
*/
}

CDetailAttrib_Player::~CDetailAttrib_Player( VOID )
{
/*
	CPrescrBuffer::iterator itPrescr;
	for(itPrescr=m_vPrescr.begin(); itPrescr!=m_vPrescr.end(); itPrescr++)
	{
		delete *itPrescr;
	}
	m_vPrescr.clear();

	CLifeAbilityMap::iterator idAbility;
	for(idAbility=m_mapLifeAbility.begin(); idAbility!=m_mapLifeAbility.end(); idAbility++)
	{
		delete idAbility->second;
	}
	m_mapLifeAbility.clear();

	CSkillMap::iterator itSkill;
	for(itSkill=m_mapSkill.begin(); itSkill!=m_mapSkill.end(); itSkill++)
	{
		delete itSkill->second;
	}
	m_mapSkill.clear();

	CXinFaMap::iterator itXinFa;
	for(itXinFa=m_mapXinFa.begin(); itXinFa!=m_mapXinFa.end(); itXinFa++)
	{
		delete itXinFa->second;
	}
	m_mapXinFa.clear();
*/
}

BOOL CDetailAttrib_Player::Init( VOID )
{
	return TRUE;
}

VOID CDetailAttrib_Player::Term( VOID )
{
	m_dwCanPickMissionItemIDNum				= 0;
	UINT i;
	for ( i = 0; i < MAX_CHAR_CAN_PICK_MISSION_ITEM_NUM; i++ )
		m_adwCanPickMissionItemList[i]		= UINT_MAX;

/*
	for ( i = 0; i < COOLDOWN_LIST_SIZE; i++ )
	{
		m_anCoolDownTime[i]			= -1;
		m_anCoolDownTotalTime[i]	= -1;
	}
*/
}

VOID CDetailAttrib_Player::Tick( VOID )
{
/*
	INT nTickTime = (INT)(CGameProcedure::s_pTimeSystem->GetLoopTime() * 1000.f);

	// to be continue...
	// 待优化
	UINT i;
	for ( i = 0; i < COOLDOWN_LIST_SIZE; i++ )
	{
		m_anCoolDownTime[i] -= nTickTime;
		if ( m_anCoolDownTime[i] < 0 )
		{
			m_anCoolDownTime[i] = 0;
		}
	}
*/
}
/*
SClientSkill *CDetailAttrib_Player::GetSkill( SkillID_t idSkill )
{
	CSkillMap::const_iterator itFind = m_mapSkill.find( idSkill );
	if ( itFind != m_mapSkill.end() )
		return itFind->second;
	return NULL;
}

SClientXinFa *CDetailAttrib_Player::GetXinFa( SkillID_t idXinFa )
{
	CXinFaMap::const_iterator itFind = m_mapXinFa.find( idXinFa );
	if ( itFind != m_mapXinFa.end() )
		return itFind->second;
	return NULL;
}

SClientLifeAbility *CDetailAttrib_Player::GetLifeAbility(INT idAbilityID)
{
	CLifeAbilityMap::const_iterator itFind = m_mapLifeAbility.find( idAbilityID );
	if ( itFind != m_mapLifeAbility.end() )
		return itFind->second;
	return NULL;
}

SClientPrescr *CDetailAttrib_Player::GetPrescr(INT nIndex)
{
	if(nIndex < 0 || nIndex >= (INT)m_vPrescr.size()) return NULL;

	return m_vPrescr[nIndex];
}

VOID CDetailAttrib_Player::ChangeSkillAttrib( const _OWN_SKILL *pSkillInit )
{
	AxTrace(0, 1, "0. ChangeSkill:%d", pSkillInit->m_nSkillID);
	SkillID_t idSkill;
	SClientSkill *pClientSkill;

	idSkill	= pSkillInit->m_nSkillID;
	CSkillMap::iterator itFind = m_mapSkill.find( idSkill );
	if ( itFind == m_mapSkill.end() )
	{
		pClientSkill = new SClientSkill;
		pClientSkill->m_nSkillID	= idSkill;
		BOOL bResult = FillSkillAttrib( pClientSkill );
		if ( bResult )
		{
			pClientSkill->m_nPosIndex = (INT)m_mapSkill.size();
			pClientSkill->m_bLeaned = true;
			m_mapSkill[idSkill] = pClientSkill;
		}
		else
		{
			delete pClientSkill;
			return ;
		}

	}
	else
	{
		pClientSkill = itFind->second;
		pClientSkill->m_bLeaned = true;
		AxTrace(0, 1, "1. LeanSkill:%d", pSkillInit->m_nSkillID);
	}
//	pClientSkill->m_ColdDownTime	= pSkillInit->GetCoolDownTime();
}

VOID CDetailAttrib_Player::ChangeXinFaAttrib( const _OWN_XINFA *pXinFaInit )
{
	SkillID_t idXinFa;
	SClientXinFa *pClientXinFa;

	//加载新法表
	DBC_DEFINEHANDLE(s_pXinFaDBC, DBC_SKILL_XINFA);

	idXinFa = pXinFaInit->m_nXinFaID;
	CXinFaMap::iterator itFind = m_mapXinFa.find( idXinFa );
	if ( itFind == m_mapXinFa.end() )
	{
		const _DBC_SKILL_XINFA* pXinFaDef = (const _DBC_SKILL_XINFA*)s_pXinFaDBC->Search_Index_EQU(idXinFa);
		if(!pXinFaDef)
		{
			//TDThrow("Invalid XinFa ID:%d", idXinFa);
			return;
		}

		pClientXinFa = new SClientXinFa;
		pClientXinFa->m_nXinFaID= idXinFa;
		pClientXinFa->m_pDefine = pXinFaDef;
		pClientXinFa->m_nLevel = -1;
		pClientXinFa->m_bLeaned = FALSE;
		pClientXinFa->m_nPosIndex = (INT)m_mapXinFa.size();
		m_mapXinFa[idXinFa] = pClientXinFa;
	}
	else
	{
		pClientXinFa = itFind->second;
	}

	BYTE yLevel = pXinFaInit->m_nLevel;
	if ( yLevel != pClientXinFa->m_nLevel )
	{
		pClientXinFa->m_nLevel	= yLevel;
		if(yLevel > 0)
			pClientXinFa->m_bLeaned = true;
		OnXinFaAttribChanged( idXinFa );
	}

	//通知ActionSystem
	CActionSystem::GetMe()->XinFa_Update();
}

SClientLifeAbility* CDetailAttrib_Player::FindAbility(INT idAbility, BOOL bCreate)
{
	SClientLifeAbility* pAbility;
	CLifeAbilityMap::iterator itFind = m_mapLifeAbility.find( idAbility );

	if ( itFind == m_mapLifeAbility.end() )
	{
		if( bCreate == FALSE ) return NULL;

		pAbility = new SClientLifeAbility;
		pAbility->m_nPosIndex = (INT)m_mapLifeAbility.size();
		static const tDataBase* pLifeAbilityDBC = NULL;
		if(!pLifeAbilityDBC) pLifeAbilityDBC  = CDataBaseSystem::GetMe()->GetDataBase(DBC_LIFEABILITY_DEFINE);
		pAbility->m_pDefine = (const _DBC_LIFEABILITY_DEFINE*)pLifeAbilityDBC->Search_Index_EQU(idAbility);

		TDAssert(pAbility->m_pDefine);

		m_mapLifeAbility[idAbility] = pAbility;
	}
	else
	{
		pAbility = itFind->second;
	}

	return pAbility;
}

VOID CDetailAttrib_Player::ChangeLifeAbilityAttrib( INT idAbility, const _OWN_ABILITY* pAbility)
{
	SClientLifeAbility* pNewAbility;

	if( pAbility->m_Level > 0 )
	{
		pNewAbility = FindAbility(idAbility, TRUE);
	}
	else
	{
		pNewAbility = FindAbility(idAbility);
	}

	if( pNewAbility == NULL )
	{
		return;
	}

	pNewAbility->m_Level = pAbility->m_Level;
	pNewAbility->m_Exp = pAbility->m_Exp;
}

//生活技能数据发生改变
VOID CDetailAttrib_Player::ChangeLifeAbilityAttrib( INT idAbility, BYTE type, uint value)
{
	SClientLifeAbility* pAbility;

	switch(type)
	{
	case 2: // update the level of a certain ability
		{
			if( value > 0 )
			{
				pAbility = FindAbility(idAbility, TRUE);
			}
			else
			{
				pAbility = FindAbility(idAbility);

				if( pAbility == NULL )
				{
					return;
				}
			}

			pAbility->m_Level = value;
			return;
		}
		break;
	case 3: // update the exp point of a certain ability
		{
			pAbility = FindAbility(idAbility);
			if( pAbility == NULL )
			{
				return;
			}

			pAbility->m_Exp = value;
			return;
		}
		break;
	default:
		break;
	}
}


VOID CDetailAttrib_Player::ChangePrescrAttrib(INT idPrescr, BOOL bAdd)
{
	SClientPrescr* pNewPrescr = NULL;
	CPrescrBuffer::iterator it = m_vPrescr.begin();

	for( ; it < m_vPrescr.end(); ++it )
	{
		if( (*it)->m_pDefine->nID == idPrescr )
		{
			if( bAdd == FALSE )
			{ // 放弃配方
				m_vPrescr.erase(it);
				return;
			}

			pNewPrescr = *it;
		}
	}

	if( bAdd == FALSE )
	{ // 没找到就不用放弃了
		return;
	}

	if( pNewPrescr == NULL )
	{
		pNewPrescr = new SClientPrescr;

		DBC_DEFINEHANDLE(pPrescrDBC, DBC_LIFEABILITY_ITEMCOMPOSE);
		pNewPrescr->m_pDefine = (const _DBC_LIFEABILITY_ITEMCOMPOSE*)pPrescrDBC->Search_Index_EQU(idPrescr);
		TDAssert(pNewPrescr->m_pDefine);

		m_vPrescr.push_back( pNewPrescr );
	}

}
*/

VOID CDetailAttrib_Player::AddMission( const _OWN_MISSION *pMission )
{
	if ( m_listMission.m_Count == MAX_CHAR_MISSION_NUM )
		return ;

	UINT dwIndex = GetMissionIndexByID( pMission->m_ndMission );
	if ( dwIndex != UINT_MAX )
		return ;

	dwIndex = UINT_MAX;
	UINT i;
	for ( i = 0; i < MAX_CHAR_MISSION_NUM; i++ )
	{
		if ( m_listMission.m_aMission[i].m_ndMission == INVALID_ID )
		{
			dwIndex = i;
			break;
		}
	}

	if ( dwIndex == UINT_MAX )
		return ;

	m_listMission.m_aMission[dwIndex] = *pMission;
	m_listMission.m_Count++;
}

VOID CDetailAttrib_Player::ModifyMission( const _OWN_MISSION *pMission )
{
	UINT dwIndex = GetMissionIndexByID( pMission->m_ndMission );
	if ( dwIndex != UINT_MAX )
		return ;

	m_listMission.m_aMission[dwIndex] = *pMission;
}

VOID CDetailAttrib_Player::RemoveMission( MissionID_t idMission )
{
	UINT dwIndex = GetMissionIndexByID( idMission );
	if ( dwIndex != UINT_MAX && m_listMission.m_aMission[dwIndex].m_ndMission != INVALID_ID )
		return ;
	m_listMission.m_aMission[dwIndex].Cleanup();
	m_listMission.m_Count--;
}

VOID CDetailAttrib_Player::UpdateCanPickMissionItemList( UINT dwItemCount, const UINT *paItemList )
{
	m_dwCanPickMissionItemIDNum = dwItemCount;
	memcpy( m_adwCanPickMissionItemList, paItemList, sizeof( UINT ) * m_dwCanPickMissionItemIDNum );
}

VOID CDetailAttrib_Player::AddCanPickMissionItem( UINT dwItemDataID )
{
	if ( m_dwCanPickMissionItemIDNum < MAX_CHAR_CAN_PICK_MISSION_ITEM_NUM )
	{
		m_adwCanPickMissionItemList[m_dwCanPickMissionItemIDNum++] = dwItemDataID;
	}
}

VOID CDetailAttrib_Player::RemoveCanPickMissionItem( UINT dwItemDataID )
{
	UINT i;
	for ( i= 0; i < m_dwCanPickMissionItemIDNum; i++ )
	{
		if ( m_adwCanPickMissionItemList[i] == dwItemDataID )
		{
			m_dwCanPickMissionItemIDNum--;
			m_adwCanPickMissionItemList[i] = m_adwCanPickMissionItemList[m_dwCanPickMissionItemIDNum];
			m_adwCanPickMissionItemList[m_dwCanPickMissionItemIDNum]	= UINT_MAX;
			return ;
		}
	}
}

/*
VOID CDetailAttrib_Player::UpdateCoolDownList( const UINT *pdwCoolDownList, UINT dwUpdateNum )
{
	if ( dwUpdateNum == 0 )
		return ;

	UINT i, dwNumCur;
	dwNumCur = 0;
	for ( i = 0; i < COOLDOWN_LIST_SIZE; i++ )
	{
		if ( pdwCoolDownList[i] != -1 )
		{
			m_anCoolDownTime[i]			= (INT)(pdwCoolDownList[i]);
			m_anCoolDownTotalTime[i]	= (INT)(pdwCoolDownList[i]);
			dwNumCur++;
			//刷新的ActionSystem
			CActionSystem::GetMe()->UpdateCoolDown(i);

			if ( dwUpdateNum == dwNumCur )
				break;

		}
	}
}
*/
UINT CDetailAttrib_Player::GetMissionIndexByID( MissionID_t idMission )const
{
	UINT i;
	for ( i = 0; i < MAX_CHAR_MISSION_NUM; i++ )
	{
		if ( m_listMission.m_aMission[i].m_ndMission == idMission )
		{
			return i;
		}
	}
	return UINT_MAX;
}

/*
BOOL CDetailAttrib_Player::FillSkillAttrib( SClientSkill *pSkill )
{
	const SSkillData	*pSkillData;
	const SClientXinFa	*pXinFa;
	SkillID_t			idSkill, idXinFa;
	UINT				dwXinFaLevel;

	pSkill->m_pOwner	= this;
	idSkill				= pSkill->m_nSkillID;
	pSkillData			= GetSkillDataMgr()->GetSkillData( idSkill );
	if ( pSkillData != NULL )
	{
		idXinFa		= (SkillID_t)(pSkillData->m_uXinFaID);
		pXinFa		= GetXinFa( idXinFa );
		if ( pXinFa != NULL )
			dwXinFaLevel	= (UINT)(pXinFa->m_nLevel);
		else
			dwXinFaLevel	= 1;

		pSkill->m_pSkillData			= pSkillData;
		//pSkill->m_ColdDownTime			= 0;
		//pSkill->m_nGatherTime			= -1;
		//pSkill->m_nDepleteMP			= 0;
		return TRUE;
	}
	else
	{
		pSkill->m_pSkillData			= NULL;
		//pSkill->m_ColdDownTime			= -1;
		//pSkill->m_nGatherTime			= -1;
		//pSkill->m_nDepleteMP			= 0;
		return FALSE;
	}
}
*/

/*
VOID CDetailAttrib_Player::OnMenPaiChanged(UINT dwMenpaiID)
{
	//门派发生改变，刷新所有心法数据
	//---------------------------------------
	//加载属于该门派所有心法
	{
		//清空所有旧心法
		m_mapXinFa.clear();
		//加载心法表
		DBC_DEFINEHANDLE(s_pXinFaDBC, DBC_SKILL_XINFA);
		INT nNum = (INT)s_pXinFaDBC->GetRecordsNum();
		for(INT i=0; i<nNum; i++)
		{
			const _DBC_SKILL_XINFA* pXinFaDef = (const _DBC_SKILL_XINFA*)s_pXinFaDBC->Search_LineNum_EQU(i);
			if(pXinFaDef->nMenpaiID != dwMenpaiID) continue;

			//属于该门派的心法
			SClientXinFa* pClientXinFa = new SClientXinFa;
			pClientXinFa->m_nXinFaID	= pXinFaDef->nID;
			pClientXinFa->m_pDefine = pXinFaDef;
			pClientXinFa->m_nLevel = 0;
			pClientXinFa->m_bLeaned = FALSE;  //没有学会
			pClientXinFa->m_nPosIndex = (INT)m_mapXinFa.size();
			m_mapXinFa[pXinFaDef->nID] = pClientXinFa;
		}
	}

	//---------------------------------------
	//加载属于该门派所有技能
	{
		//清空旧门派技能
		CSkillMap::iterator it, itNext;
		for(it=m_mapSkill.begin(); it!=m_mapSkill.end(); it = itNext)
		{
			itNext = it;
			itNext++;

			SClientSkill* pSkill = it->second;
			if( pSkill->m_pSkillData->m_nMenPai != -1 && 
				pSkill->m_pSkillData->m_nMenPai != dwMenpaiID)
			{
				m_mapSkill.erase(it);
			}
		}


		//加载技能表
		DBC_DEFINEHANDLE(s_pSkillDBC, DBC_SKILL_DATA);
		INT nNum = (INT)s_pSkillDBC->GetRecordsNum();

		for(INT i=0; i<nNum; i++)
		{
			const _DBC_SKILL_DATA* pSkillDef = (const _DBC_SKILL_DATA*)s_pSkillDBC->Search_LineNum_EQU(i);
			//AxTrace(0, 3, "SkillRead:%d menpai=%d %d", i, pSkillDef->m_nMenPai, dwMenpaiID);
			if(pSkillDef->m_nMenPai != dwMenpaiID) continue;

			//属于该门派的技能
			SClientSkill* pClientSkill = new SClientSkill;
			pClientSkill->m_nSkillID = (SkillID_t)pSkillDef->m_uID;

			FillSkillAttrib( pClientSkill );

			pClientSkill->m_nPosIndex = (INT)m_mapSkill.size();
			pClientSkill->m_bLeaned = FALSE;
			m_mapSkill[pClientSkill->m_nSkillID] = pClientSkill;
		}
	}

	//通知ActionSystem
	CActionSystem::GetMe()->UserSkill_Update();
	CActionSystem::GetMe()->XinFa_Update();
}

VOID CDetailAttrib_Player::OnXinFaAttribChanged( SkillID_t idXinFa )
{
	const SSkillData *pSkillData;
	SClientSkill *pClientSkill;
	CSkillMap::iterator itCur, itEnd;

	itEnd = m_mapSkill.end();
	for ( itCur = m_mapSkill.begin(); itCur != itEnd; itCur++ )
	{
		pClientSkill = itCur->second;
		pSkillData = GetSkillDataMgr()->GetSkillData( pClientSkill->m_nSkillID );
		if ( pSkillData != NULL && pSkillData->m_uXinFaID == idXinFa )
		{
			FillSkillAttrib( pClientSkill );
		}
	}
}
*/
VOID CDetailAttrib_Player::OnMissionChanged( UINT dwIndex )
{
}
