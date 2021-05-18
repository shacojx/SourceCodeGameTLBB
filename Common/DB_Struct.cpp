#include "stdafx.h"

#include "DB_Struct.h"


VOID _HUMAN_DB_LOAD::CleanUp( )
{
//基本信息
	m_PortraitID		= -1;
	memset( m_Name, 0, MAX_CHARACTER_NAME ) ;
	memset( m_Title, 0, MAX_CHARACTER_TITLE ) ;
	m_GUID				= INVALID_ID ;
	m_Sex				= FALSE ;
	m_CreateDate		= 0 ;
	memset( &m_Titles, 0, sizeof(_TITLE));
	m_Level				= 0;
	m_RMBMoney			= 0;
	m_BankRMB			= 0;
	m_VigorRegeneRate	= 0;
	m_DoubleExpTime_Num			= 1;
	m_GmRight			= 0;
	m_EnergyRegeneRate	= 0;
	m_Exp				= 0;
	m_Money				= 0;
	memset( m_Passwd, 0, MAX_PWD ) ;
	m_uPwdDelTime		= 0;
	m_HairColor			= 0xFFFFFFFF;
	m_FaceColor			= 0xFF;
	m_HairModel			= -1;
	m_FaceModel			= -1;

//基本状态
	m_StartScene		= INVALID_ID ;
	m_BakScene			= INVALID_ID ;
	m_GuildID			= INVALID_ID ;
	m_TeamID			= INVALID_ID ;
	m_Position.CleanUp();
	m_BakPosition.CleanUp();
	m_OnlineTime		= 0;
	m_LastLoginTime		= 0;
	m_LastLogoutTime	= 0;
	m_LeftDieTime		= 0;
	m_DBVersion			= 0;

//战斗信息
	m_CampData.CleanUp();
	m_MenPai			= 0;
	m_NeiShang			= 0;
	m_HP				= 0;
	m_MP				= 0;
	m_StrikePoint		= 0;
	m_Rage				= 0;
	m_nPKValue			= 0;
	m_guidCurrentPet.Reset();
	m_BaseAttrLevel1.CleanUp();
	memset(m_Reserve,0,sizeof(INT)*MAX_RESERVE);

	m_Level1Points		= 0;
//
//玩家商店
//
	for(UINT i =0; i<MAX_SHOP_NUM_PER_PLAYER; i++)
	{
		m_ShopGuid[i].Reset();
	}

	for(UINT i =0; i<MAX_FAVORITE_SHOPS; i++)
	{
		m_FavoriteList[i].Reset();
	}

	//称号
	m_Titles.CleanUp();
}

VOID _PET_SKILL::CleanUp( VOID )
{
	m_nSkillID	= INVALID_ID;
}

VOID _PET_DB_LOAD::CleanUp( )
{
//基本信息
	m_GUID.Reset();
	m_SpouseGUID.Reset();
	m_nDataID			= INVALID_ID;
	memset( m_szName, 0, MAX_CHARACTER_NAME );
	memset( m_szNick, 0, MAX_NICK_NAME );

	m_nLevel			= 0;
	m_nTakeLevel		= 0;
	m_nAttackType		= 0;
	m_AIType			= _PET_DB_LOAD::PET_AI_INVALID;
	m_CampData.CleanUp();
	m_nHP				= 0;
	m_nLife				= 0;
	m_byPetType			= _PET_DB_LOAD::PET_TYPE_INVALID;
	m_byGeneration		= 0;
	m_byHappiness			= 0;

	m_nStrPerception	= 0;
	m_nConPerception	= 0;
	m_nDexPerception	= 0;
	m_nSprPerception	= 0;
	m_nIntPerception	= 0;
	m_nGenGu			= 0;
	m_fGrowRate			= 0;
	m_nRemainPoint		= 0;
	m_nExp				= 0;
	
	for (INT i = 0; i < PET_MAX_SKILL_COUNT; ++i)
	{
		m_SkillList[i].CleanUp();
	}
	m_CooldownList.CleanUp();
}

VOID _PET_DB_LIST_LOAD::CleanUp( VOID )
{
	UINT i;
	for ( i = 0; i < HUMAN_PET_MAX_COUNT; i++ )
	{
		m_aPetDB[i].CleanUp();
	}
}

VOID _BANK_DB_LOAD::CleanUp( )
{
	memset( m_pItem, 0, MAX_BANK_SIZE*sizeof(_ITEM) ) ;
	m_Count			=	0;
	m_CurEndIndex	=	RENTBOX2_START_INDEX;//默认只打开一个租赁包
	m_Money			=	0;
}

VOID _BAG_DB_LOAD::CleanUp( )
{
	memset( m_pItem, 0, MAX_BAG_SIZE*sizeof(_ITEM) ) ;

	m_Count = 0 ;
	m_BaseBagSize	=	60;
	//m_TaskBagSize_Nouse	=	0;
	//m_MatBagSize_Nouse	=	0;
	
}

VOID _EQUIP_DB_LOAD::CleanUp( )
{
	memset( m_pItem, 0, HEQUIP_NUMBER*sizeof(_ITEM) ) ;

	m_Flags = 0 ;
}

VOID	_ABILITY_DB_LOAD::CleanUp()
{
	memset(m_aABility, 0, sizeof(_OWN_ABILITY) * MAX_CHAR_ABILITY_NUM);
	memset(m_aPrescr, 0, MAX_CHAR_PRESCRIPTION_BYTE);
}
VOID _SKILL_DB_LOAD::CleanUp( )
{
	memset( m_aSkill, 0, sizeof(m_aSkill));

	m_Count = 0 ;
}

VOID _XINFA_DB_LOAD::CleanUp( )
{
	memset( m_aXinFa, 0, sizeof(m_aXinFa));

	m_Count = 0 ;
}

VOID _IMPACT_DB_LOAD::CleanUp( )
{
	memset( m_aImpacts, 0, sizeof(m_aImpacts));

	m_Count = 0 ;
}

VOID _MISSION_DB_LOAD::CleanUp( )
{
	INT i;
	for ( i = 0; i < MAX_CHAR_MISSION_NUM; i++ )
		m_aMission[i].Cleanup();

	m_Count = 0 ;

	memset( m_aMissionHaveDoneFlags, 0, sizeof(UINT)*MAX_CHAR_MISSION_FLAG_LEN );
	memset( m_aMissionData, 0, sizeof(INT)*MAX_CHAR_MISSION_DATA_NUM ) ;

}

VOID _RELATION_DB_LOAD::CleanUp( )
{
	INT i;
	for( i=0;  i<MAX_RELATION_SIZE; i++ )
	{
		m_aRelation[i].CleanUp( );
	}
	for( i=0; i<RELATION_GROUP_NUMBER; i++ )
	{
		m_aRelationCount[i] = 0;
	}
//	m_Flag.CleanUp();
	memset((void*)m_szMood, 0, sizeof(m_szMood));
	m_MarriageInfo.CleanUp() ;
	m_PrenticeInfo.CleanUp() ;
	m_MasterInfo.CleanUp() ;
}

