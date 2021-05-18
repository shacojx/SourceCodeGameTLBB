#include "stdafx.h"


#include "GameTable.h"
#include "Log.h"
#include "Ini.h"
#include "FileDataMgr.h"

#include "TLBB_DBC.h"
#include "FileDef.h"
#include "AIScript.h"
#include "State.h"

using namespace DBC;

_BASE_VALUE_TBL				g_BaseValueTbl ;
_MONSTER_ATTREX_TBL			g_MonsterAttrExTbl ;
_NPC_PAOPAO_INDEX_TBL		g_NPCPaoPaoIndexTbl[MAXTYPE_NUMBER];
// pet
_PET_ATTR_TBL				g_PetAttrTbl ;
_PET_ATTRPOINT_DIS_TBL		g_PetAttrPointDisTbl;
_PET_LEVELUP_TBL			g_PetLevelUpTbl;
_PET_DOMESTICATION_MONEY_TBL		g_PetDomesticationMoneyTbl;
_PET_SKILLDISTR_TBL			g_PetSkillDistrTbl[PET_MAX_NUM];
_PET_SKILLINDEX_TBL			g_PetSkillIndexTbl[PET_SKILL_NUM];
_PET_STUDYSKILL_TBL			g_PetStudySkillTbl;
_PET_APPERCEIVESKILL_TBL	g_PetApperceiveSkillTbl[PET_APPERCEIVESKILLRATE_NUM];
FLOAT						g_PetAIStrategyTble[PET_SKILL_TYPE][PET_AITYPE];

_MONSTER_AI_TBL				g_MonsterAITbl ;
_ATTR_LEVELUP_TBL			g_AttrLevelupTbl ;
_LEVEL_EXP_TBL				g_LevelExpTbl;
//_XINFA_UPLEVEL_SPEND_TBL	g_XinfaUpLevelSpendTbl;
_EXP_ATT_TBL				g_ExpAttTbl;
_MONSTER_SPEAK_TBL			g_MonsterSpeakTbl ;
_GROW_POINT_INFO_TBL		g_GrowPointInfoTbl;
_PLANTS_INFO_TBL			g_PlantInfoTbl;
_QUESTION_LIB				g_QuestionLib ;
//Skill
XinFaDataMgr_T				g_XinFaDataMgr;
SkillTemplateDataMgr_T		g_SkillTemplateDataMgr;
SkillDataMgr_T				g_SkillDataMgr;
ImpactSEDataMgr_T			g_ImpactSEDataMgr;
ImpactDataMgr_T				g_StandardImpactMgr;
//end Skill
//impact collection
IDCollectionMgr_T			g_IDCollectionMgr;
//camp and stand
CampAndStandDataMgr_T		g_CampAndStandDataMgr;
//Special Obj
SpecialObjDataMgr_T			g_SpecialObjDataMgr;
//end
CFileDataMgr				g_AIFileDataMgr;
AIScript*					g_pAIScriptList[AISCRIPT_NUM];
_IPRANGE					g_IPRange ;

//级别与金钱上限相对应
_LEVEL_MAX_MONEY			g_LevelMoneyTbl;
_CITY_SCENE					g_CitySceneTbl;
_CHAT_CONSUME				g_ChatConsumeTbl ;
_HAIR_STYLE					g_HairStyleTbl;
_GM_TABLE					g_GMTbl ;

//普通任务表
_MISSION_DATA				g_MissionDataTable;
_MISSION_REWARD				g_MissionRewardTable;
_MISSION_PUNISH				g_MissionPunishTable;
_MISSION_DIALOG				g_MissionDialogTable;
map<INT,vector<_MISSION_LIST_t> >	g_PromulgatorIndex;
map<INT,vector<_MISSION_LIST_t> >	g_SubmitorIndex;
map<INT,vector<_MISSION_LIST_t> >	g_PromulgatorAndSubmitorIndex;
map<INT,_MISSION_LIST_t>			g_MissionIndexByScriptID;

TableInit					g_TableInit ;
/////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////
TableInit::TableInit( )
{
}

TableInit::~TableInit( )
{
	CleanUpAIScrpt();
	CleanUpAIStateList();
	CleanMissionData();
	CleanMissionReward();
	CleanMissionPunish();
	CleanMissionDialog();
}

BOOL TableInit::Init( )
{
	BOOL bRet ;

	InitBaseValueTable( ) ;
	InitMonsterAttrExTable( ) ;
	InitNPCPaoPaoIndexTable();
	InitPetAttrTable( );
	InitMonsterAITable( ) ;
	InitPetConfigTable( );
	InitPetAttrPointDistribute( );
	InitPetLevelUpTable();
	InitPetDomesticationMoney();
	InitPetSkillDistributeTable( );
	InitPetSkillIndexTable( );
	InitPetStudySkillTable( );
	InitPetApperceiveSkillTable( );
	InitPetAIStrategyTable( );
	InitAIScriptFile( );
	InitAIStateList();
	bRet=InitCampAndStandDataMgr( );Assert(bRet);
	bRet=InitXinFaDataMgr( );Assert(bRet);
	bRet=InitSkillTemplateDataMgr( );Assert(bRet);
	bRet=InitSkillDataMgr( );Assert(bRet);
	bRet=InitImpactSEDataMgr( );Assert(bRet);
	bRet=InitSpecialObjDataMgr( );Assert(bRet);
	bRet=InitIdCollectionMgr( );Assert(bRet);
	bRet=InitStandardImpactMgr( );Assert(bRet);
	InitAttrLevelupTable( ) ;
	InitLevelExpTable( );
	//InitXinfaUpLevelSpendTable( );
	InitMonsterSpeakTable( );
	InitExpAttTable( );
	InitGrowPointTable( );
	InitQuestionLib( ) ;
	InitIPRange( ) ;
	InitLevelMoneyTable( );
	InitCitySceneTable( );
	InitChatConsume( ) ;
	InitHairStyleTable( );
	InitGMTable( ) ;

	InitMissionDataTable();
	InitMissionRewardTable();
	InitMissionPunishTable();
	InitMissionDialogTable();
	InitMissionListTable();

	return TRUE ;
}
BOOL TableInit::ReloadCombatTables(VOID)
{
	BOOL bRet ;
	bRet=InitCampAndStandDataMgr( );Assert(bRet);
	bRet=InitXinFaDataMgr( );Assert(bRet);
	bRet=InitSkillTemplateDataMgr( );Assert(bRet);
	bRet=InitSkillDataMgr( );Assert(bRet);
	bRet=InitImpactSEDataMgr( );Assert(bRet);
	bRet=InitSpecialObjDataMgr( );Assert(bRet);
	bRet=InitIdCollectionMgr( );Assert(bRet);
	bRet=InitStandardImpactMgr( );Assert(bRet);
	return TRUE ;
}

VOID TableInit::InitBaseValueTable( )
{//读取基本系数表的数据

	__ENTER_FUNCTION

	Ini ini( FILE_BASE_VALUE ) ;
	for( INT i=0; i<MATTRIBUTE_NUMBER; i++ )
	{
		for( INT j=0; j<AINFOTYPE_NUMBER; j++ )
		{
			CHAR szSection[256], szName[256] ;
			sprintf( szSection, "MENPAI%d", i ) ;
			sprintf( szName, "AINFOTYPE%d", j ) ;
			g_BaseValueTbl.m_Table[j][i] = ini.ReadInt( szSection, szName ) ;
		}
	}

	Log::SaveLog( SERVER_LOGFILE, "Load BaseValueTable.ini ... OK! " ) ;

	__LEAVE_FUNCTION

	
}

VOID	TableInit::InitLevelExpTable()
{
	__ENTER_FUNCTION

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_PLAYER_EXP_LEVEL );

	INT iTableCount = ThirdFile.GetRecordsNum();
	
	Assert(iTableCount==MAX_PLAYER_EXP_LEVEL);

	for(INT i = 0;	i<MAX_PLAYER_EXP_LEVEL;i++)
	{
		
		g_LevelExpTbl.m_Table[i][0]	=	i;
		g_LevelExpTbl.m_Table[i][1]	=	ThirdFile.Search_Posistion(i,1)->iValue;
	}

	Log::SaveLog( SERVER_LOGFILE, "Load PlayerExpLevel.txt ... OK! " ) ;
	
	__LEAVE_FUNCTION
}

//VOID	TableInit::InitXinfaUpLevelSpendTable()
//{
//	__ENTER_FUNCTION
//
//	DBCFile ThirdFile(0);
//	BOOL ret = ThirdFile.OpenFromTXT( FILE_XINFA_STUDY_SPEND );
//
//
//	//行数
//	INT iTableCount = ThirdFile.GetRecordsNum();
//	
//	for(INT iXinfa=0; iXinfa<MAX_ALL_XINFA_NUM; iXinfa++)
//	{
//		for(INT iLevel=0; iLevel<MAX_XINFA_LEVEL_NUM; iLevel++)
//		{
//			g_XinfaUpLevelSpendTbl.m_Table[iXinfa][iLevel][0] = ThirdFile.Search_Posistion(iLevel+2,0)->iValue;
//			g_XinfaUpLevelSpendTbl.m_Table[iXinfa][iLevel][1] = ThirdFile.Search_Posistion(iLevel+2,iXinfa*2 + 1)->iValue;
//			g_XinfaUpLevelSpendTbl.m_Table[iXinfa][iLevel][2] = ThirdFile.Search_Posistion(iLevel+2,iXinfa*2 + 2)->iValue;
//		}
//	}
//
//	Log::SaveLog( SERVER_LOGFILE, "Load XinFaStudySpend_V1.txt ... OK! " ) ;
//	
//	__LEAVE_FUNCTION
//}

VOID	TableInit::InitMonsterSpeakTable()
{
__ENTER_FUNCTION
#define BASENUM (1000)
	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_NPC_CHAT );

	INT iTableCount = ThirdFile.GetRecordsNum();
	INT nLastID = 0 ,	
		nNewID  = 0 ,
		nIndex  = 0 ;

	for(INT i = 0;	i<iTableCount; i++)
	{
		nNewID = ThirdFile.Search_Posistion(i,0)->iValue ;
		const CHAR* pString = ThirdFile.Search_Posistion(i,1)->pString ;
		if( !nLastID ) 
		{//是第一项谈话
			g_MonsterSpeakTbl.m_TypeCount++ ;
			g_MonsterSpeakTbl.m_NumberOfType[nIndex]++ ;
		}
        else if ( nLastID && nLastID/BASENUM==nNewID/BASENUM )
		{//同一类型的谈话项
			g_MonsterSpeakTbl.m_NumberOfType[nIndex]++ ;
		}
		else
		{//下一类型的谈话项
			nIndex++ ;
			g_MonsterSpeakTbl.m_TypeCount++ ;
			g_MonsterSpeakTbl.m_NumberOfType[nIndex]++ ;
		}

		g_MonsterSpeakTbl.m_Table[i].nSpeakID = nNewID;
		memset((VOID*)(g_MonsterSpeakTbl.m_Table[i].szSpeakContent),0,MAX_MONSTERSPEAK_CONTENT);
		memcpy((VOID*)(g_MonsterSpeakTbl.m_Table[i].szSpeakContent),pString,sizeof(pString));
		g_MonsterSpeakTbl.m_Table[i].nSpeakID = nNewID ;
		g_MonsterSpeakTbl.m_Count++ ;
		nLastID = nNewID ;
		
	}

	Log::SaveLog( SERVER_LOGFILE, "Load NPCChat.txt ... OK! " ) ;

__LEAVE_FUNCTION

}

uint	TableInit::GetLevelExp(uint iLevel)
{
	
	__ENTER_FUNCTION
	
		return g_LevelExpTbl.m_Table[iLevel-1][1];
	
	__LEAVE_FUNCTION

}

VOID TableInit::InitMonsterAttrExTable( )
{//读取基本系数表的数据
	__ENTER_FUNCTION
	enum MonsterAttrData
	{
		MonsterType				=	0,
		MonsterName				=	1,
		MonsterCannotFight		=	2,
		MonsterLevel			=	3,
		MonsterExp          	=   4,
		MonsterFriendValue		=	5,
		MonsterOutLook			=	6,
		MonsterBaseAI			=	7,
		MonsterExtAI			=	8,
		MonsterCamp				=	9,
		MonsterDropSearchRange	=	10,
		MonsterDropTeamCount	=	11,
		MonsterMinDamagePercent	=	12,
		MonsterCanHandle		=	13,
		MonsterBossFlag			=	14,
		Monster_CATTR_LEVEL2_ATTACKRATE_P		=	15,
		Monster_CATTR_LEVEL2_DEFENCE_P			=   16,			
		Monster_CATTR_LEVEL2_ATTACKRATE_M		=	17,		
		Monster_CATTR_LEVEL2_DEFENCE_M			=	18,			
		Monster_CATTR_LEVEL2_MAXHP				=	19,				
		Monster_CATTR_LEVEL2_MAXMP				=	20,				
		Monster_CATTR_LEVEL2_RESTOREHP			=	21,			
		Monster_CATTR_LEVEL2_RESTOREMP			=	22,			
		Monster_CATTR_LEVEL2_HIT				=	23,				
		Monster_CATTR_LEVEL2_MISS				=	24,				
		Monster_CATTR_LEVEL2_CRITRATE			=	25,			
		Monster_CATTR_LEVEL2_SPEED				=	26,
		MonsterWalkSpeed						=	27,
		Monster_CATTR_LEVEL2_ATTACKSPEED		=	28,		
		Monster_CATTR_LEVEL2_ATTACKCOLD			=	29,		
		Monster_CATTR_LEVEL2_DEFENCECOLD		=	30,		
		Monster_CATTR_LEVEL2_ATTACKFIRE			=	31,		
		Monster_CATTR_LEVEL2_DEFENCEFIRE		=	32,		
		Monster_CATTR_LEVEL2_ATTACKLIGHT		=	33,		
		Monster_CATTR_LEVEL2_DEFENCELIGHT		=	34,		
		Monster_CATTR_LEVEL2_ATTACKPOISON		=	35,		
		Monster_CATTR_LEVEL2_DEFENCEPOISON		=	36,	
		MonsterBossSnapImmID					=	37,
		MonsterBossDurationImmID				=	38,
		MonsterAttackAnimTime					=	46,
		MonsterAttackCooldownTime				=	47,

	};	
		
	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_MONSTER_ATTREX );

	INT iTableCount			=	ThirdFile.GetRecordsNum();
	INT iColumnCount		=	ThirdFile.GetFieldsNum();
	
	Assert(iTableCount>0 && iTableCount<MAXTYPE_NUMBER);
	g_MonsterAttrExTbl.m_Count	=	iTableCount;
	
	
	INT iValue;
	INT iType;
	Assert(Monster_CATTR_LEVEL2_DEFENCEPOISON<iColumnCount);

	for(INT i =0;i<iTableCount;i++)
	{
		iValue		=	ThirdFile.Search_Posistion(i,MonsterType)->iValue;

		iType	    =	iValue;
		if( iType>=MAXTYPE_NUMBER )
		{
			AssertEx(FALSE,"怪物表中填写的怪物号超出范围") ;
			continue ;
		}

		if(g_MonsterAttrExTbl.m_MaxType<(uint)iType)
			g_MonsterAttrExTbl.m_MaxType = iType;

		g_MonsterAttrExTbl.m_TableExt[iType].m_Type	=	iType;

		strncpy( g_MonsterAttrExTbl.m_TableExt[iType].m_Name, ThirdFile.Search_Posistion(i,MonsterName)->pString, sizeof( g_MonsterAttrExTbl.m_TableExt[i].m_Name ) - 1 );

		iValue		=	ThirdFile.Search_Posistion(i,MonsterLevel)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_Level	=	iValue;

		iValue		=	ThirdFile.Search_Posistion(i,MonsterCannotFight)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_CannotFight	=	iValue;

		iValue		=	ThirdFile.Search_Posistion(i,MonsterExp)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_BaseExp	=	iValue;

		iValue		=	ThirdFile.Search_Posistion(i,MonsterOutLook)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_OutLook	=	iValue;

		iValue		=	ThirdFile.Search_Posistion(i,MonsterBaseAI)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_BaseAI	=	iValue;

		iValue		=	ThirdFile.Search_Posistion(i,MonsterExtAI)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_ExtAI	=	iValue;

		iValue		=	ThirdFile.Search_Posistion(i,MonsterCamp)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_Camp	=	iValue;

		iValue		=	ThirdFile.Search_Posistion(i,MonsterDropSearchRange)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_DropSearchRange	=	(FLOAT)iValue;

		iValue		=	ThirdFile.Search_Posistion(i,MonsterDropTeamCount)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_SearchTeamCount	=	iValue;

		iValue		=	ThirdFile.Search_Posistion(i,MonsterMinDamagePercent)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_MinDamagePercent	=	iValue;

		iValue		=	ThirdFile.Search_Posistion(i,MonsterCanHandle)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_CanHandle	=	iValue;

		iValue		=	ThirdFile.Search_Posistion(i,MonsterFriendValue)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_FriendValue	=	iValue;

		iValue		=	ThirdFile.Search_Posistion(i,MonsterWalkSpeed)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_WalkSpeed	=	iValue;

		iValue		=	ThirdFile.Search_Posistion(i,MonsterBossFlag)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_BossFlag	=	iValue;

		iValue		=	ThirdFile.Search_Posistion(i,MonsterBossSnapImmID)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_SnapSkillImmID	=	iValue;

		iValue		=	ThirdFile.Search_Posistion(i,MonsterBossDurationImmID)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_DurationSkillImmID	=	iValue;

		iValue		=	ThirdFile.Search_Posistion(i,MonsterAttackAnimTime)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_AttackAnimTime	=	iValue;

		iValue		=	ThirdFile.Search_Posistion(i,MonsterAttackCooldownTime)->iValue;
		g_MonsterAttrExTbl.m_TableExt[iType].m_AttackCooldownTime	=	iValue;
		

		g_MonsterAttrExTbl.m_Table[iType][CATTR_LEVEL2_ATTACKRATE_P] = ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_ATTACKRATE_P	)->iValue;
		g_MonsterAttrExTbl.m_Table[iType][CATTR_LEVEL2_DEFENCE_P]	 = ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_DEFENCE_P		)->iValue;
		g_MonsterAttrExTbl.m_Table[iType][CATTR_LEVEL2_ATTACKRATE_M] = ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_ATTACKRATE_M	)->iValue;
		g_MonsterAttrExTbl.m_Table[iType][CATTR_LEVEL2_DEFENCE_M]	 = ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_DEFENCE_M		)->iValue;
		g_MonsterAttrExTbl.m_Table[iType][CATTR_LEVEL2_MAXHP]		 = ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_MAXHP			)->iValue;
		g_MonsterAttrExTbl.m_Table[iType][CATTR_LEVEL2_MAXMP]		 = ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_MAXMP			)->iValue;
		g_MonsterAttrExTbl.m_Table[iType][CATTR_LEVEL2_RESTOREHP]    = ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_RESTOREHP		)->iValue;
		g_MonsterAttrExTbl.m_Table[iType][CATTR_LEVEL2_RESTOREMP]	 = ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_RESTOREMP		)->iValue;
		g_MonsterAttrExTbl.m_Table[iType][CATTR_LEVEL2_HIT]			 = ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_HIT			)->iValue;
		g_MonsterAttrExTbl.m_Table[iType][CATTR_LEVEL2_MISS]	     = ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_MISS			)->iValue;
		g_MonsterAttrExTbl.m_Table[iType][CATTR_LEVEL2_CRITRATE]     = ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_CRITRATE		)->iValue;
		g_MonsterAttrExTbl.m_Table[iType][CATTR_LEVEL2_SPEED]	     = ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_SPEED			)->iValue;
		g_MonsterAttrExTbl.m_Table[iType][CATTR_LEVEL2_ATTACKSPEED]  = ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_ATTACKSPEED	)->iValue;
		g_MonsterAttrExTbl.m_Table[iType][CATTR_LEVEL2_ATTACKCOLD]	 = ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_ATTACKCOLD		)->iValue;
		g_MonsterAttrExTbl.m_Table[iType][CATTR_LEVEL2_DEFENCECOLD]  = ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_DEFENCECOLD	)->iValue;
		g_MonsterAttrExTbl.m_Table[iType][CATTR_LEVEL2_ATTACKFIRE]	 = ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_ATTACKFIRE		)->iValue;
		g_MonsterAttrExTbl.m_Table[iType][CATTR_LEVEL2_DEFENCEFIRE]  = ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_DEFENCEFIRE	)->iValue;
		g_MonsterAttrExTbl.m_Table[iType][CATTR_LEVEL2_ATTACKLIGHT]	 = ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_ATTACKLIGHT	)->iValue;
		g_MonsterAttrExTbl.m_Table[iType][CATTR_LEVEL2_DEFENCELIGHT] = ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_DEFENCELIGHT	)->iValue;
		g_MonsterAttrExTbl.m_Table[iType][CATTR_LEVEL2_ATTACKPOISON] = ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_ATTACKPOISON	)->iValue;
		g_MonsterAttrExTbl.m_Table[iType][CATTR_LEVEL2_DEFENCEPOISON] = ThirdFile.Search_Posistion(i,Monster_CATTR_LEVEL2_DEFENCEPOISON	)->iValue;
		
		
		g_MonsterAttrExTbl.m_TableExt[iType].m_Used	= TRUE;
	}



	Log::SaveLog( SERVER_LOGFILE, "Load MonsterAttrExTable.ini ... OK! " ) ;
	
	__LEAVE_FUNCTION
}

VOID TableInit::InitPetAttrTable( )
{//读取基本系数表的数据

	__ENTER_FUNCTION

	enum PetAttrData
	{
		PetID	 = 0,
		PetName,
		PetTakeLevel,
		PetCamp,
		IsVarPet,		// 是否为变异宠物
		IsBabyPet,		// 是否为宝宝宠物
		PetFoodType	,
		PetSkillCount,
		VoluntarySkill,
		PassiveSkill1,
		PassiveSkill2,
		PassiveSkill3,
		PassiveSkill4,
		PetLife	,
        PetStrPerception,
		PetConPerception,
		PetDexPerception,
		PetSprPerception,
		PetIntPerception,
		PetGrowRate0,
		PetGrowRate1,
		PetGrowRate2,
		PetGrowRate3,
		PetGrowRate4,
		CowardiceRate,
		WarinessRate,	
		LoyalismRate,	
		CanninessRate,
		ValourRate,	
	
		PetAttrEnd,
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_PET_ATTR );

	INT iTableCount			=	ThirdFile.GetRecordsNum();
	INT iColumnCount		=	ThirdFile.GetFieldsNum();
	
	Assert(iTableCount>0 && iTableCount<MAXTYPE_NUMBER);
	g_PetAttrTbl.m_Count = iTableCount;
	
	
	INT iValue	= 0;
	INT iType	= 0;

	for(INT i =0; i < iTableCount; i++)
	{
		iValue		=	ThirdFile.Search_Posistion(i, PetID)->iValue;
		iType	    =	iValue;

		if( iType>=MAXTYPE_NUMBER )
		{
			AssertEx(FALSE,"宠物表ID超出范围值") ;
			continue ;
		}

		if(g_PetAttrTbl.m_MaxType < (uint)iType)
			g_PetAttrTbl.m_MaxType = iType;

		// 宠物编号
		g_PetAttrTbl.m_TableExt[iType].m_Type	=	iType;
		// 宠物名称
		strncpy( g_PetAttrTbl.m_TableExt[iType].m_Name, ThirdFile.Search_Posistion(i, PetName)->pString, sizeof( g_PetAttrTbl.m_TableExt[i].m_Name ) - 1 );
		// 宠物可携带等级
		iValue		=	ThirdFile.Search_Posistion(i, PetTakeLevel)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_TakeLevel	=	iValue;
		// 是否为变异宠物
		iValue		=	ThirdFile.Search_Posistion(i, IsVarPet)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_bVarPet  =	iValue;
		// 是否为宝宝宠物
		iValue		=	ThirdFile.Search_Posistion(i, IsBabyPet)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_bBabyPet	=	iValue;
		// 宠物食物类型
		iValue		=	ThirdFile.Search_Posistion(i, PetFoodType)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_FoodType	=	iValue;
		// 标准寿命
		iValue		=	ThirdFile.Search_Posistion(i, PetLife)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_Life	=	iValue;
		// 被动技能最大数量
		iValue		=   ThirdFile.Search_Posistion(i, PetSkillCount)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_PassiveSkillCount =	iValue;
		// 主动技能
		iValue		=   ThirdFile.Search_Posistion(i, VoluntarySkill)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_VoluntarySkill =	iValue;
		// 被动技能1
		iValue		=   ThirdFile.Search_Posistion(i, PassiveSkill1)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_PassiveSkill1 =	iValue;
		// 被动技能2
		iValue		=   ThirdFile.Search_Posistion(i, PassiveSkill2)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_PassiveSkill2 =	iValue;
		// 被动技能3
		iValue		=   ThirdFile.Search_Posistion(i, PassiveSkill3)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_PassiveSkill3 =	iValue;
		// 被动技能4
		iValue		=   ThirdFile.Search_Posistion(i, PassiveSkill4)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_PassiveSkill4 =	iValue;
		// 标准力量资质
		iValue		=	ThirdFile.Search_Posistion(i, PetStrPerception)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_StrPerception	=	iValue;
		// 标准体质资质
		iValue		=	ThirdFile.Search_Posistion(i, PetConPerception)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_ConPerception	=	iValue;
		// 标准灵气资质
		iValue		=	ThirdFile.Search_Posistion(i, PetDexPerception)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_DexPerception	=	iValue;
		// 标准身法资质
		iValue		=	ThirdFile.Search_Posistion(i, PetSprPerception)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_SprPerception	=	iValue;
		// 标准定力资质
		iValue		=	ThirdFile.Search_Posistion(i, PetIntPerception)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_IntPerception	=	iValue;
		// 成长率0
		iValue		=	ThirdFile.Search_Posistion(i, PetGrowRate0)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_GrowRate0	=	iValue;
		// 成长率1
		iValue		=	ThirdFile.Search_Posistion(i, PetGrowRate1)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_GrowRate1	=	iValue;
		// 成长率2
		iValue		=	ThirdFile.Search_Posistion(i, PetGrowRate2)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_GrowRate2	=	iValue;
		// 成长率3
		iValue		=	ThirdFile.Search_Posistion(i, PetGrowRate3)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_GrowRate3	=	iValue;
		// 成长率4
		iValue		=	ThirdFile.Search_Posistion(i, PetGrowRate4)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_GrowRate4	=	iValue;
		// 胆小
		iValue		=	ThirdFile.Search_Posistion(i, CowardiceRate)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_CowardiceRate	=	iValue;
		// 谨慎
		iValue		=	ThirdFile.Search_Posistion(i, WarinessRate)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_WarinessRate	=	iValue;
		// 忠诚
		iValue		=	ThirdFile.Search_Posistion(i, LoyalismRate)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_LoyalismRate	=	iValue;
		// 精明
		iValue		=	ThirdFile.Search_Posistion(i, CanninessRate)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_CanninessRate	=	iValue;
		// 勇猛
		iValue		=	ThirdFile.Search_Posistion(i, ValourRate)->iValue;
		g_PetAttrTbl.m_TableExt[iType].m_ValourRate	=	iValue;
	
	}

	Log::SaveLog( SERVER_LOGFILE, "Load PetAttrTable.txt ... OK! " ) ;
	
	__LEAVE_FUNCTION
}

VOID TableInit::InitMonsterAITable( )
{
	__ENTER_FUNCTION

	Ini ini( FILE_MONSTER_AI ) ;

	g_MonsterAITbl.m_Count = ini.ReadInt( "INFO", "AINUMBER" ) ;
	Assert( g_MonsterAITbl.m_Count>=0 && g_MonsterAITbl.m_Count<MAXAI_NUMBER ) ;

	for( INT i=0; i<g_MonsterAITbl.m_Count; i++ )
	{
		for( INT j=0; j<AIPARAM_NUMBER; j++ )
		{
			CHAR szSection[256], szName[256] ;
			sprintf( szSection, "AI%d", i ) ;
			sprintf( szName, "AIPARAM%d", j ) ;
			g_MonsterAITbl.m_Table[i][j] = ini.ReadInt( szSection, szName ) ;
		}
	}

	Log::SaveLog( SERVER_LOGFILE, "Load MonsterAITable.ini ... OK! " ) ;

	__LEAVE_FUNCTION
}

VOID TableInit::InitPetConfigTable( )
{
	__ENTER_FUNCTION

	Ini ini( FILE_PET_CONFIG ) ;

	m_PetConfig.m_BabyPetRate = ini.ReadInt("System", "BabyPetRate");
    m_PetConfig.m_VariancePetRate = ini.ReadInt("System", "VariancePetRate");

	m_PetConfig.m_WilenessPetRate_TakeLevel = ini.ReadInt("System", "WilenessPetRate_TakeLevel");
	m_PetConfig.m_WilenessPetRate_Delta1 = ini.ReadInt("System", "WilenessPetRate_Delta1");
	m_PetConfig.m_WilenessPetRate_Delta2 = ini.ReadInt("System", "WilenessPetRate_Delta2");
	m_PetConfig.m_WilenessPetRate_Delta3 = ini.ReadInt("System", "WilenessPetRate_Delta3");

	m_PetConfig.m_GrowRate0 = ini.ReadInt("System", "GrowRate0");
	m_PetConfig.m_GrowRate1 = ini.ReadInt("System", "GrowRate1");
	m_PetConfig.m_GrowRate2 = ini.ReadInt("System", "GrowRate2");
	m_PetConfig.m_GrowRate3 = ini.ReadInt("System", "GrowRate3");
	m_PetConfig.m_GrowRate4 = ini.ReadInt("System", "GrowRate4");

	CHAR szName[256];
	for (INT i = 0; i < GENGU_NUM; ++i)
	{
		memset(szName, 0, sizeof(szName));
		sprintf(szName, "GenGu%d_Begin", i);
		m_PetConfig.m_vGenGu[i].m_Begin = ini.ReadInt("System", szName);
		
		sprintf(szName, "GenGu%d_End", i);
		m_PetConfig.m_vGenGu[i].m_End = ini.ReadInt("System", szName);

		sprintf(szName, "GenGu%d_Rate", i);
		m_PetConfig.m_vGenGu[i].m_Rate = ini.ReadInt("System", szName);
	}

	for (INT j = 0; j < SPetConfig::VARIANCEPET_LEVEL_NUM; ++j)
	{
		tsnprintf(szName, sizeof(szName), "rateoflevel%dvariancepet", j+1);
		INT nBaseRate = 0;
		if (j > 0)
		{
			nBaseRate = m_PetConfig.m_aRateOfLevelVariancePet[j-1];
		}
		INT nRet = 0;
		ini.ReadIntIfExist("System", szName, nRet);
		m_PetConfig.m_aRateOfLevelVariancePet[j] = nBaseRate + nRet;
	}

	m_PetConfig.m_IntelligenceRange = ini.ReadInt("System", "IntelligenceRange");

	m_PetConfig.m_MoveSpeed = ini.ReadInt("System", "MoveSpeed");
	m_PetConfig.m_AttackSpeed = ini.ReadInt("System", "AttackSpeed");

	// 初始HP、物攻、魔攻、物防、魔防、命中、闪避、会心（八个）
	m_PetConfig.m_BaseHP = ini.ReadInt("System", "BaseHP");
	m_PetConfig.m_BasePhyAttack = ini.ReadInt("System", "BasePhyAttack");
	m_PetConfig.m_BaseMgcAttack = ini.ReadInt("System", "BaseMgcAttack");
	m_PetConfig.m_BasePhyDefence = ini.ReadInt("System", "BasePhyDefence");
	m_PetConfig.m_BaseMgcDefence = ini.ReadInt("System", "BaseMgcDefence");
	m_PetConfig.m_BaseHit = ini.ReadInt("System", "BaseHit");
	m_PetConfig.m_BaseMiss = ini.ReadInt("System", "BaseMiss");
	m_PetConfig.m_BaseCritical = ini.ReadInt("System", "BaseCritical");

	// 体质对HP，力量对物攻，灵气对魔攻，体质对物防，定力对魔防
	// 敏捷对闪避，敏捷对会心,敏捷对命中的影响系数
	m_PetConfig.m_Con_HP_Pram = ini.ReadInt("System", "Con_HP_Pram") / 1000.f;
	m_PetConfig.m_Str_PhyAttack_Pram = ini.ReadInt("System", "Str_PhyAttack_Pram") / 1000.f;
	m_PetConfig.m_Spr_MgcAttack_Pram = ini.ReadInt("System", "Spr_MgcAttack_Pram") / 1000.f;
	m_PetConfig.m_Con_PhyDefence_Pram = ini.ReadInt("System", "Con_PhyDefence_Pram") / 1000.f;
	m_PetConfig.m_Int_MgcDefence_Pram = ini.ReadInt("System", "Int_MgcDefence_Pram") / 1000.f;
	m_PetConfig.m_Dex_Miss_Pram = ini.ReadInt("System", "Dex_Miss_Pram") / 1000.f;
	m_PetConfig.m_Dex_Critical_Pram = ini.ReadInt("System", "Dex_Critical_Pram") / 1000.f;
	m_PetConfig.m_Dex_Hit_Pram = ini.ReadInt("System", "Dex_Hit_Pram") / 1000.f;

	// 等级对HP、物攻、魔攻、物防、魔防、闪避、会心、命中的影响系数
	m_PetConfig.m_Level_HP_Pram = ini.ReadInt("System", "Level_HP_Pram") / 1000.f;
	m_PetConfig.m_Level_PhyAttack_Pram = ini.ReadInt("System", "Level_PhyAttack_Pram") / 1000.f;
	m_PetConfig.m_Level_MgcAttack_Pram = ini.ReadInt("System", "Level_MgcAttack_Pram") / 1000.f;
	m_PetConfig.m_Level_PhyDefence_Pram = ini.ReadInt("System", "Level_PhyDefence_Pram") / 1000.f;
	m_PetConfig.m_Level_MgcDefence_Pram = ini.ReadInt("System", "Level_MgcDefence_Pram") / 1000.f;
	m_PetConfig.m_Level_Miss_Pram = ini.ReadInt("System", "Level_Miss_Pram") / 1000.f;
	m_PetConfig.m_Level_Critical_Pram = ini.ReadInt("System", "Level_Critical_Pram") / 1000.f;
	m_PetConfig.m_Level_Hit_Pram = ini.ReadInt("System", "Level_Hit_Pram") / 1000.f;

	Log::SaveLog( SERVER_LOGFILE, "Load PetConfigTable.ini ... OK! " ) ;

	__LEAVE_FUNCTION
}

VOID TableInit::InitPetAttrPointDistribute( )
{
	__ENTER_FUNCTION

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_PET_ATTRPOINTDISTRIBUTE );

	INT iTableCount			=	ThirdFile.GetRecordsNum();
	INT iColumnCount		=	ThirdFile.GetFieldsNum();

	INT iValue;
	for(INT i =0; i < iTableCount; i++)
	{
		// 属性点分配方案号
		iValue		=	ThirdFile.Search_Posistion(i, 0)->iValue;
		g_PetAttrPointDisTbl.m_Table[i].m_ID = iValue;
        iValue		=	ThirdFile.Search_Posistion(i, 1)->iValue;
		g_PetAttrPointDisTbl.m_Table[i].m_StrRate = iValue;
		iValue		=	ThirdFile.Search_Posistion(i, 2)->iValue;
		g_PetAttrPointDisTbl.m_Table[i].m_ConRate = iValue;
		iValue		=	ThirdFile.Search_Posistion(i, 3)->iValue;
		g_PetAttrPointDisTbl.m_Table[i].m_SprRate = iValue;
		iValue		=	ThirdFile.Search_Posistion(i, 4)->iValue;
		g_PetAttrPointDisTbl.m_Table[i].m_DexRate = iValue;
		iValue		=	ThirdFile.Search_Posistion(i, 5)->iValue;
		g_PetAttrPointDisTbl.m_Table[i].m_IntRate = iValue;
	
		++g_PetAttrPointDisTbl.m_Count;
	}

	__LEAVE_FUNCTION
}

VOID TableInit::InitPetLevelUpTable( )
{
	__ENTER_FUNCTION

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_PET_LEVELUP );

	INT iTableCount			=	ThirdFile.GetRecordsNum();
	INT iColumnCount		=	ThirdFile.GetFieldsNum();

	INT iLevel = 0;
	INT iExp = 0;
	for(INT i =0; i < iTableCount; i++)
	{
		// 级别
		iLevel	=	ThirdFile.Search_Posistion(i, 0)->iValue;
		// 该级别升级所需经验
		iExp	=	ThirdFile.Search_Posistion(i, 1)->iValue;
		g_PetLevelUpTbl.m_Table[iLevel] = iExp;
	}

	__LEAVE_FUNCTION

}

VOID TableInit::InitPetDomesticationMoney( )
{
	__ENTER_FUNCTION

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_PET_DOMESTICATION_MONEY );

	INT iTableCount			=	ThirdFile.GetRecordsNum();
	INT iColumnCount		=	ThirdFile.GetFieldsNum();

	INT iLevel = 0;
	FLOAT fDomesticationMoney = 0;
	for(INT i =0; i < iTableCount; i++)
	{
		// 级别
		iLevel			=	ThirdFile.Search_Posistion(i, 0)->iValue;
		// 该级别升级所需经验
		fDomesticationMoney	=	ThirdFile.Search_Posistion(i, 1)->fValue;
		g_PetDomesticationMoneyTbl.m_Table[iLevel] = fDomesticationMoney;
	}

	__LEAVE_FUNCTION

}

VOID TableInit::InitPetSkillDistributeTable( )
{// 初始化技能分配表

	__ENTER_FUNCTION


	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_PET_SKILLDISTRIBUTE );

	INT iTableCount			=	ThirdFile.GetRecordsNum();
	INT iColumnCount		=	ThirdFile.GetFieldsNum();

	INT iPetID = 0;
	INT iRange = 0;
    for(INT i = 0; i < iTableCount; i++)
	{
		iPetID = ThirdFile.Search_Posistion(i, 0)->iValue;
		g_PetSkillDistrTbl[i].m_PetID = iPetID;
		for (INT j = 2; j < iColumnCount; j++)
		{
			iRange = ThirdFile.Search_Posistion(i, j)->iValue;
			if (iRange > 0)
			{
				for (INT n = 0; n < PET_MAX_SKILL_COUNT; ++n)
				{
					if (g_PetSkillDistrTbl[i].m_Table[n].m_nSkillIndex == -1)
					{
						g_PetSkillDistrTbl[i].m_Table[n].m_nSkillIndex = j - 2;
						g_PetSkillDistrTbl[i].m_Table[n].m_nSkillRange = iRange;
						break;
					}
				}
			}
		}
	}
	__LEAVE_FUNCTION

}

VOID TableInit::InitPetSkillIndexTable( )
{// 初始化技能索引表，此表主要是为了以后维护方便！
	__ENTER_FUNCTION

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_PET_SKILLINDEXTBL );

	INT iTableCount			=	ThirdFile.GetRecordsNum();
	INT iColumnCount		=	ThirdFile.GetFieldsNum();

	INT iIndex = 0;
	INT idSkill = 0;
	memset(g_PetSkillIndexTbl, 0, sizeof(_PET_SKILLINDEX_TBL) * PET_SKILL_NUM);

	for (INT i = 0; i < iTableCount; i++)
	{
		iIndex = ThirdFile.Search_Posistion(i, 0)->iValue;
		idSkill = ThirdFile.Search_Posistion(i, 1)->iValue;
		g_PetSkillIndexTbl[i].m_nIndex = iIndex;
		g_PetSkillIndexTbl[i].m_nSkillID = idSkill;
	}
	__LEAVE_FUNCTION
}

VOID TableInit::InitPetStudySkillTable( )
{
	__ENTER_FUNCTION

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_PET_STUDYSKILLTBL );

	INT iTableCount			=	ThirdFile.GetRecordsNum();
	INT iColumnCount		=	ThirdFile.GetFieldsNum();

	INT nID;
	INT		nMaxSpaceCount;	// 总空格数量
	INT		nSpaceCount;		// 当前空格数量
	for (INT i = 0; i < iTableCount; ++i)
	{
		Assert(i < PET_STUDYSKILLRATE_NUM);

		nMaxSpaceCount	= ThirdFile.Search_Posistion(i, 1)->iValue;
		nSpaceCount		= ThirdFile.Search_Posistion(i, 2)->iValue;
		nID = MAKE_PET_STUDYSKILL_ID( nMaxSpaceCount, nSpaceCount );

		g_PetStudySkillTbl.m_aStudySkill[nID].m_nID					= nID;
		g_PetStudySkillTbl.m_aStudySkill[nID].m_nMaxSpaceCount		= nMaxSpaceCount;
		g_PetStudySkillTbl.m_aStudySkill[nID].m_nSpaceCount			= nSpaceCount;
		g_PetStudySkillTbl.m_aStudySkill[nID].m_nNewPercent			= ThirdFile.Search_Posistion(i, 3)->iValue;;
	}
	__LEAVE_FUNCTION
}

VOID TableInit::InitPetApperceiveSkillTable( )
{
	__ENTER_FUNCTION

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_PET_APPERCEIVESKILLTBL );

	INT iTableCount			=	ThirdFile.GetRecordsNum();
	INT iColumnCount		=	ThirdFile.GetFieldsNum();

	memset(g_PetApperceiveSkillTbl, 0, sizeof(_PET_APPERCEIVESKILL_TBL) * PET_APPERCEIVESKILLRATE_NUM);

	for (INT i = 0; i < iTableCount; ++i)
	{
		Assert(i < PET_APPERCEIVESKILLRATE_NUM);
	
		g_PetApperceiveSkillTbl[i].m_nSkillCount = ThirdFile.Search_Posistion(i, 1)->iValue;
		g_PetApperceiveSkillTbl[i].m_nLearnedVoluntarySkillCount = ThirdFile.Search_Posistion(i, 2)->iValue;
		g_PetApperceiveSkillTbl[i].m_nLearnedPassiveSkillCount = ThirdFile.Search_Posistion(i, 3)->iValue;
		g_PetApperceiveSkillTbl[i].m_nApperceiveVoluntarySkillRate = ThirdFile.Search_Posistion(i, 4)->iValue;
		g_PetApperceiveSkillTbl[i].m_nApperceivePassiveSkillRate = ThirdFile.Search_Posistion(i, 5)->iValue;
	}
	__LEAVE_FUNCTION

}

VOID	TableInit::InitPetAIStrategyTable()
{
	__ENTER_FUNCTION

	enum 
	{
		AITypeBegin = 2,
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_PET_AISTRATEGYTBL );

	INT iTableCount			=	ThirdFile.GetRecordsNum();
	INT iColumnCount		=	ThirdFile.GetFieldsNum();

	memset(g_PetAIStrategyTble, 0, sizeof(FLOAT) * PET_AITYPE * PET_SKILL_TYPE);
	
	for (INT i = 0; i < iTableCount; ++i)
	{
		Assert(i < PET_SKILL_TYPE);
		for (INT j = AITypeBegin; j < iColumnCount; ++j)
		{	
			Assert(j < PET_AITYPE + AITypeBegin);
			g_PetAIStrategyTble[i][j-AITypeBegin] = ThirdFile.Search_Posistion(i, j)->fValue;
		}
	}
	__LEAVE_FUNCTION
}

VOID	TableInit::InitExpAttTable()
{
	__ENTER_FUNCTION


	enum ExpAttTBLData
	{
		TBDeltaLevel	= 0,
		TBRate			= 1	
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_EXP_ATTENUATION );


	INT iTableCount			=	ThirdFile.GetRecordsNum();
	INT iColumnCount		=	ThirdFile.GetFieldsNum();

	Assert(iTableCount==MAX_100_PLUS_DELTA_LEVEL);

	for(INT i =0;i<MAX_100_PLUS_DELTA_LEVEL;i++)
	{
		g_ExpAttTbl.m_Table[i] = ThirdFile.Search_Posistion(i,TBRate)->fValue;
	}
	
	Log::SaveLog( SERVER_LOGFILE, "Load ExpAttenuation.txt ... OK! " );

	__LEAVE_FUNCTION
}

INT CompareGrowPointTB(const VOID* pArg1,const VOID* pArg2)
{	
	_GROW_POINT_INFO* pTB1	=	(_GROW_POINT_INFO*)pArg1;
	_GROW_POINT_INFO* pTB2	=	(_GROW_POINT_INFO*)pArg2;

	if(pTB1->m_GrowPointIndex>pTB2->m_GrowPointIndex)
		return 1;
	else if(pTB1->m_GrowPointIndex<pTB2->m_GrowPointIndex)
		return -1;
	else
		return 0;

}


_GROW_POINT_INFO*	_GROW_POINT_INFO_TBL::Get(INT GrowType)
{
	_GROW_POINT_INFO	tb;

	tb.m_GrowPointIndex	=	GrowType;

	_GROW_POINT_INFO* pResult = (_GROW_POINT_INFO*)bsearch(&tb,
		m_Table,
		m_Count,
		sizeof(_GROW_POINT_INFO),
		(INT (*)(const VOID*, const VOID*))CompareGrowPointTB);

	if(pResult)
		return pResult;
	else
	{
		Assert(FALSE);
		Log::SaveLog( SERVER_LOGFILE, "Search _GROW_POINT_INFO  GrowPointType:%d Get Errors", 
			GrowType) ;
	}
	return	0;

}
VOID	TableInit::InitGrowPointTable()
{
	__ENTER_FUNCTION
	
	enum	GrowPointTBLData
	{
		TBIndex				=	0,
		TBName				=	1,
		TBMesh				=	2,
		TBOpenAbility		=	3,
		TBOpenTask			=	4,
		TBTips				=	5,
		TBOpenTime			=	6,
		TBScriptID			=	7,
		TBOpenAnim			=	8
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_GROWPOINT );
	

	INT iTableCount		=		ThirdFile.GetRecordsNum();
	INT iColumnCount	=		ThirdFile.GetFieldsNum();

	Assert(TBOpenAnim<iColumnCount);
	//Assert(iTableCount<MAX_GROW_POINT_TYPE);

	g_GrowPointInfoTbl.m_Count	=	iTableCount;
	g_GrowPointInfoTbl.m_Table	=	new _GROW_POINT_INFO[iTableCount];
	for(INT i = 0 ;i<iTableCount;i++)
	{
		g_GrowPointInfoTbl.m_Table[i].m_GrowPointIndex	=	ThirdFile.Search_Posistion(i,TBIndex)->iValue;
		g_GrowPointInfoTbl.m_Table[i].m_OpenTime		=	ThirdFile.Search_Posistion(i,TBOpenTime)->iValue;
		g_GrowPointInfoTbl.m_Table[i].m_OpenAnim		=	ThirdFile.Search_Posistion(i,TBOpenAnim)->iValue;
		g_GrowPointInfoTbl.m_Table[i].m_OpenAbility		=	ThirdFile.Search_Posistion(i,TBOpenAbility)->iValue;
		g_GrowPointInfoTbl.m_Table[i].m_ScriptID		=	ThirdFile.Search_Posistion(i,TBScriptID)->iValue;
		
			
	}
	Log::SaveLog( SERVER_LOGFILE, "Load GrowPoint.txt ... OK! " );

	__LEAVE_FUNCTION
}

VOID TableInit::InitPlantTable()
{
	__ENTER_FUNCTION
	
	enum	PlantTBLData
	{
		TBIndex		=	0,
		TBType		=	1,
		TBName		=	2,
		TBProduct	=	3
	};
	
	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_PLANTS );


	INT iTableCount		=		ThirdFile.GetRecordsNum();
	INT iColumnCount	=		ThirdFile.GetFieldsNum();

	Assert(TBProduct<iColumnCount);

	g_PlantInfoTbl.m_Count		=		iTableCount;
	g_PlantInfoTbl.m_Table		=		new _PLANTS_INFO[iTableCount];

	for(INT i = 0;i<iTableCount;i++)
	{
		g_PlantInfoTbl.m_Table[i].m_PlantIndex	=	ThirdFile.Search_Posistion(i,TBIndex)->iValue;
		g_PlantInfoTbl.m_Table[i].m_PlantType	=	ThirdFile.Search_Posistion(i,TBType)->iValue;
		g_PlantInfoTbl.m_Table[i].m_Product		=	ThirdFile.Search_Posistion(i,TBProduct)->iValue;

	}

	Log::SaveLog( SERVER_LOGFILE, "Load Plant.txt ... OK! " );
	__LEAVE_FUNCTION
}

VOID TableInit::InitAttrLevelupTable( )
{
	__ENTER_FUNCTION
		
	enum	AttrLevelupTBData
	{
		TBLevel					=			0,
		TBDataBegin				=			1
	};

	DBCFile ThirdFile(0);

	BOOL ret = ThirdFile.OpenFromTXT( FILE_ATTR_LEVEL_UP );

	INT		iTableCount		=	ThirdFile.GetRecordsNum();
	INT		iTableColumn	=	ThirdFile.GetFieldsNum();
	INT		iColumn			=	0;
	
	Assert(iTableCount==MAX_TABLE_DEFINE_LEVEL+1);

	for(INT i = 0;	i<MAX_TABLE_DEFINE_LEVEL+1;i++)
	{
		for(INT j = 0;j<MATTRIBUTE_NUMBER;j++)
		{
			for(INT k=0;k<CATTR_LEVEL1_NUMBER;k++)
			{
				iColumn		=	TBDataBegin+j*CATTR_LEVEL1_NUMBER+k;

				Assert(iColumn<iTableColumn);
				INT iValue = ThirdFile.Search_Posistion(i,iColumn)->iValue;
				g_AttrLevelupTbl.Set(k,j,i,iValue);
			}
		}
	}

	Log::SaveLog( SERVER_LOGFILE, "Load AttrLevelupTable.ini ... OK! " ) ;
	
	__LEAVE_FUNCTION
}
//skill
BOOL 			TableInit::InitXinFaDataMgr()
{
	__ENTER_FUNCTION
	BOOL bRet = FALSE;
	if(TRUE == g_XinFaDataMgr.InitFromDB( FILE_XINFA ))
	{
		Log::SaveLog( SERVER_LOGFILE, "Load XinFa_v1.txt ... OK! " ) ;
		bRet = TRUE;
	}
	else
	{
		Log::SaveLog( SERVER_LOGFILE, "Load XinFa_v1.txt ... Failed! " ) ;
		bRet = FALSE;
	}
	return bRet;
	__LEAVE_FUNCTION
	return FALSE;
}
BOOL 			TableInit::InitSkillTemplateDataMgr()
{
	__ENTER_FUNCTION
	BOOL bRet = FALSE;
	if(TRUE == g_SkillTemplateDataMgr.InitFromDB( FILE_SKILL_TEMPLATE ))
	{
		Log::SaveLog( SERVER_LOGFILE, "Load SkillTemplate_v1.txt ... OK! " ) ;
		bRet = TRUE;
	}
	else
	{
		Log::SaveLog( SERVER_LOGFILE, "Load SkillTemplate_v1.txt ... Failed! " ) ;
		bRet = FALSE;
	}
	return bRet;
	__LEAVE_FUNCTION
	return FALSE;
}
BOOL			TableInit::InitSkillDataMgr()
{
	__ENTER_FUNCTION
	BOOL bRet = FALSE;
	if(TRUE == g_SkillDataMgr.InitFromDB( FILE_SKILL_DATA ))
	{
		Log::SaveLog( SERVER_LOGFILE, "Load SkillData_V1.txt ... OK! " ) ;
		bRet = TRUE;
	}
	else
	{
		Log::SaveLog( SERVER_LOGFILE, "Load SkillData_V1.txt ... Failed! " ) ;
		bRet = FALSE;
	}
	return bRet;
	__LEAVE_FUNCTION
	return FALSE;
}
BOOL			TableInit::InitImpactSEDataMgr()
{
	__ENTER_FUNCTION
	BOOL bRet = FALSE;
	if(TRUE == g_ImpactSEDataMgr.InitFromDB( FILE_IMPACT_SEDATA ))
	{
		Log::SaveLog( SERVER_LOGFILE, "Load ImpactSEData_v1.txt ... OK! " ) ;
		bRet = TRUE;
	}
	else
	{
		Log::SaveLog( SERVER_LOGFILE, "Load ImpactSEData_v1.txt ... Failed! " ) ;
		bRet = FALSE;
	}
	return bRet;
	__LEAVE_FUNCTION
	return FALSE;
}
BOOL			TableInit::InitStandardImpactMgr()
{
	__ENTER_FUNCTION
	BOOL bRet = FALSE;
	if(TRUE == g_StandardImpactMgr.InitFromDB( FILE_STANDARD_IMPACT_DATA))
	{
		Log::SaveLog( SERVER_LOGFILE, "Load StandardImpact.txt ... OK! " ) ;
		bRet = TRUE;
	}
	else
	{
		Log::SaveLog( SERVER_LOGFILE, "Load StandardImpact.txt ... Failed! " ) ;
		bRet = FALSE;
	}
	return bRet;
	__LEAVE_FUNCTION
	return FALSE;
}
BOOL			TableInit::InitSpecialObjDataMgr()
{
	__ENTER_FUNCTION
	BOOL bRet = FALSE;
	if(TRUE == g_SpecialObjDataMgr.InitFromDB( FILE_SPECIAL_OBJ_DATA ))
	{
		Log::SaveLog( SERVER_LOGFILE, "Load SpecialObjData.txt ... OK! " ) ;
		bRet = TRUE;
	}
	else
	{
		Log::SaveLog( SERVER_LOGFILE, "Load SpecialObjData.txt ... Failed! " ) ;
		bRet = FALSE;
	}
	return bRet;
	__LEAVE_FUNCTION
	return FALSE;
}
//end skill
VOID	TableInit::InitAIScriptFile()
{
	__ENTER_FUNCTION
	g_AIFileDataMgr.Init( FILE_AI_SCRIPT, FALSE);

	memset((VOID*)(g_pAIScriptList), NULL, AISCRIPT_NUM);
	for (INT i = 0; i < AISCRIPT_NUM; ++i)
	{
		SFileData * pSFileData = g_AIFileDataMgr.GetFileData(i);
		if (!pSFileData)
		{// 已经读完（正常情况下已经读完）
			continue ;
		}

		AIScript* pAIScript = new AIScript();
		if (!pAIScript)
		{
			Assert(NULL && "TableInit::InitAIScriptFile...new AIScript Failed...");
			return ;
		}

		if(pSFileData)
		{
			CHAR  szName[_MAX_PATH];
			CHAR* filename = const_cast<CHAR*>(pSFileData->GetFileName());
			memset(szName,0,_MAX_PATH);
			GET_AI_SCRIPT_FULL_PATH( szName, filename);
		
			pAIScript->ParseScript(szName);
		}

		// 压入g_pAIScriptList数组中
		g_pAIScriptList[i] = pAIScript;
	}

	__LEAVE_FUNCTION
}

VOID TableInit::CleanUpAIScrpt()
{
	for (INT i = 0; i < AISCRIPT_NUM; ++i)
	{
		SAFE_DELETE(g_pAIScriptList[i]);
	}
}

//camp and stand
BOOL TableInit::InitCampAndStandDataMgr()
{
	__ENTER_FUNCTION
	BOOL bRet = FALSE;
	if(TRUE == g_CampAndStandDataMgr.InitFromDB( FILE_CAMP_AND_STAND ))
	{
		Log::SaveLog( SERVER_LOGFILE, "Load CampAndStand.txt ... OK! " ) ;
		bRet = TRUE;
	}
	else
	{
		Log::SaveLog( SERVER_LOGFILE, "Load CampAndStand.txt ... Failed! " ) ;
		bRet = FALSE;
	}
	return bRet;
	__LEAVE_FUNCTION
	return FALSE;
}
//end camp and stand
//Impact collection
BOOL TableInit::InitIdCollectionMgr()
{
	__ENTER_FUNCTION
	BOOL bRet = FALSE;
	if(TRUE == g_IDCollectionMgr.InitFromDB(FILE_ID_COLLECTION))
	{
		Log::SaveLog( SERVER_LOGFILE, "Load IDCollections.txt ... OK! " ) ;
		bRet = TRUE;
	}
	else
	{
		Log::SaveLog( SERVER_LOGFILE, "Load IDCollections.txt ... Failed! " ) ;
		bRet = FALSE;
	}
	return bRet;
	__LEAVE_FUNCTION
	return FALSE;
}

VOID TableInit::InitQuestionLib( )
{
__ENTER_FUNCTION

	enum	QuestionTBLData
	{
		TBIndex		=	0,
		TBContex	=	1,
		TBOption0	=	2,
		TBOption1	=	3,
		TBOption2	=	4,
		TBOption3	=	5,
		TBOption4	=	6,
		TBOption5	=	7,
		TBKey0		=	8,
		TBKey1		=	9,
		TBKey2		=	10,
		TBKey3		=	11,
		TBKey4		=	12,
		TBKey5		=	13,
	};
	
	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_QUESTION );


	INT iTableCount		=		ThirdFile.GetRecordsNum();
	INT iColumnCount	=		ThirdFile.GetFieldsNum();

	Assert(TBKey5<iColumnCount);
	Assert(iTableCount<=MAX_QUESTIONS) ;

	g_QuestionLib.m_Count		=		iTableCount;

	for(INT i = 0;i<iTableCount;i++)
	{
		g_QuestionLib.m_aQuestions[i].m_nIndex	=	ThirdFile.Search_Posistion(i,TBIndex)->iValue;
		
		strncpy(g_QuestionLib.m_aQuestions[i].m_szContex,ThirdFile.Search_Posistion(i, TBContex)->pString,QUES_CON_SIZE-1) ;
		if( g_QuestionLib.m_aQuestions[i].m_szContex[0]=='#' ) memset( g_QuestionLib.m_aQuestions[i].m_szContex, 0, QUES_CON_SIZE ) ;
		
		strncpy(g_QuestionLib.m_aQuestions[i].m_szOption0,ThirdFile.Search_Posistion(i, TBOption0)->pString,OPT_CON_SIZE-1) ;
		if( g_QuestionLib.m_aQuestions[i].m_szOption0[0]=='#' ) memset( g_QuestionLib.m_aQuestions[i].m_szOption0, 0, OPT_CON_SIZE ) ;
		
		strncpy(g_QuestionLib.m_aQuestions[i].m_szOption1,ThirdFile.Search_Posistion(i, TBOption1)->pString,OPT_CON_SIZE-1) ;
		if( g_QuestionLib.m_aQuestions[i].m_szOption1[0]=='#' ) memset( g_QuestionLib.m_aQuestions[i].m_szOption1, 0, OPT_CON_SIZE ) ;

		strncpy(g_QuestionLib.m_aQuestions[i].m_szOption2,ThirdFile.Search_Posistion(i, TBOption2)->pString,OPT_CON_SIZE-1) ;
		if( g_QuestionLib.m_aQuestions[i].m_szOption2[0]=='#' ) memset( g_QuestionLib.m_aQuestions[i].m_szOption2, 0, OPT_CON_SIZE ) ;

		strncpy(g_QuestionLib.m_aQuestions[i].m_szOption3,ThirdFile.Search_Posistion(i, TBOption3)->pString,OPT_CON_SIZE-1) ;
		if( g_QuestionLib.m_aQuestions[i].m_szOption3[0]=='#' ) memset( g_QuestionLib.m_aQuestions[i].m_szOption3, 0, OPT_CON_SIZE ) ;

		strncpy(g_QuestionLib.m_aQuestions[i].m_szOption4,ThirdFile.Search_Posistion(i, TBOption4)->pString,OPT_CON_SIZE-1) ;
		if( g_QuestionLib.m_aQuestions[i].m_szOption4[0]=='#' ) memset( g_QuestionLib.m_aQuestions[i].m_szOption4, 0, OPT_CON_SIZE ) ;

		strncpy(g_QuestionLib.m_aQuestions[i].m_szOption5,ThirdFile.Search_Posistion(i, TBOption5)->pString,OPT_CON_SIZE-1) ;
		if( g_QuestionLib.m_aQuestions[i].m_szOption5[0]=='#' ) memset( g_QuestionLib.m_aQuestions[i].m_szOption5, 0, OPT_CON_SIZE ) ;

		g_QuestionLib.m_aQuestions[i].m_Key0	=	ThirdFile.Search_Posistion(i,TBKey0)->iValue;
		g_QuestionLib.m_aQuestions[i].m_Key1	=	ThirdFile.Search_Posistion(i,TBKey1)->iValue;
		g_QuestionLib.m_aQuestions[i].m_Key2	=	ThirdFile.Search_Posistion(i,TBKey2)->iValue;
		g_QuestionLib.m_aQuestions[i].m_Key3	=	ThirdFile.Search_Posistion(i,TBKey3)->iValue;
		g_QuestionLib.m_aQuestions[i].m_Key4	=	ThirdFile.Search_Posistion(i,TBKey4)->iValue;
		g_QuestionLib.m_aQuestions[i].m_Key5	=	ThirdFile.Search_Posistion(i,TBKey5)->iValue;
	}

	Log::SaveLog( SERVER_LOGFILE, "Load Questions.txt ... OK! " );


__LEAVE_FUNCTION
}

VOID TableInit::InitIPRange( )
{
	__ENTER_FUNCTION

	Ini ini( FILE_IPRANGE ) ;

	INT i;
	g_IPRange.m_EnableCount = ini.ReadInt( "enableip", "count" ) ;
	for( i=0; i<g_IPRange.m_EnableCount; i++ )
	{
		CHAR szIP[32],szMask[32] ;
		sprintf( szIP, "ip%d", i ) ;
		sprintf( szMask, "mask%d", i ) ;
		ini.ReadText( "enableip", szIP, g_IPRange.m_aaIPEnable[i], IP_SIZE ) ;
		ini.ReadText( "enableip", szMask, g_IPRange.m_aaMaskEnable[i], IP_SIZE ) ;
	}

	g_IPRange.m_DisableCount = ini.ReadInt( "disableip", "count" ) ;
	for( i=0; i<g_IPRange.m_DisableCount; i++ )
	{
		CHAR szIP[32],szMask[32] ;
		sprintf( szIP, "ip%d", i ) ;
		sprintf( szMask, "mask%d", i ) ;
		ini.ReadText( "disableip", szIP, g_IPRange.m_aaIPDisable[i], IP_SIZE ) ;
		ini.ReadText( "disableip", szMask, g_IPRange.m_aaMaskDisable[i], IP_SIZE ) ;
	}

	Log::SaveLog( SERVER_LOGFILE, "Load IPRange.ini ... OK! " ) ;

	__LEAVE_FUNCTION

}

VOID	TableInit::InitLevelMoneyTable()
{
	__ENTER_FUNCTION

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_PLAYER_LEVEL_MONEY );

	INT iTableCount = ThirdFile.GetRecordsNum();

	Assert(iTableCount==MAX_PLAYER_EXP_LEVEL);

	for(INT i = 0;	i<MAX_PLAYER_EXP_LEVEL;i++)
	{
		g_LevelMoneyTbl.m_Table[i][0]	=	i;
		g_LevelMoneyTbl.m_Table[i][1]	=	ThirdFile.Search_Posistion(i,2)->iValue;
	}

	Log::SaveLog( SERVER_LOGFILE, "Load LvMaxMoney.txt ... OK! " ) ;

	__LEAVE_FUNCTION
}

VOID	TableInit::InitCitySceneTable()
{
	__ENTER_FUNCTION

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_CITY_SCENE );

	INT iTableCount = ThirdFile.GetRecordsNum();

	Assert(iTableCount<=MAX_SCENE);

	for(INT i = 0; i<iTableCount; i++)
	{
		g_CitySceneTbl.m_Table[i].m_PortID	=	ThirdFile.Search_Posistion(i, 0)->iValue;
		strncpy( g_CitySceneTbl.m_Table[i].m_szSceneFile, ThirdFile.Search_Posistion(i,1)->pString, sizeof( g_CitySceneTbl.m_Table[i].m_szSceneFile ) - 1 );
	}

	Log::SaveLog( SERVER_LOGFILE, "Load CityInfo.txt ... OK! " ) ;

	__LEAVE_FUNCTION
}
VOID	TableInit::InitChatConsume( )
{
	__ENTER_FUNCTION

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_CHAT_CONSUME );

	INT iTableCount = ThirdFile.GetRecordsNum();
	g_ChatConsumeTbl.m_Count = iTableCount ;

	Assert(iTableCount<=MAX_CHAT_TYPE_INTABLE);

	for(INT i = 0; i<iTableCount; i++)
	{
		g_ChatConsumeTbl.m_ChatComsumeType[i].m_ConsumeCount = 0 ;
		g_ChatConsumeTbl.m_ChatComsumeType[i].m_DelayTime = ThirdFile.Search_Posistion(i, 3)->iValue;
		g_ChatConsumeTbl.m_ChatComsumeType[i].m_ConsumeType[0] = ThirdFile.Search_Posistion(i, 4)->iValue;
		g_ChatConsumeTbl.m_ChatComsumeType[i].m_ConsumeValue[0] = ThirdFile.Search_Posistion(i, 5)->iValue;
		if( g_ChatConsumeTbl.m_ChatComsumeType[i].m_ConsumeType[0]!=CHAT_NEED_NONE ) g_ChatConsumeTbl.m_ChatComsumeType[i].m_ConsumeCount = 1 ;
		g_ChatConsumeTbl.m_ChatComsumeType[i].m_ConsumeType[1] = ThirdFile.Search_Posistion(i, 6)->iValue;
		g_ChatConsumeTbl.m_ChatComsumeType[i].m_ConsumeValue[1] = ThirdFile.Search_Posistion(i, 7)->iValue;
		if( g_ChatConsumeTbl.m_ChatComsumeType[i].m_ConsumeType[1]!=CHAT_NEED_NONE ) g_ChatConsumeTbl.m_ChatComsumeType[i].m_ConsumeCount = 2 ;
		g_ChatConsumeTbl.m_ChatComsumeType[i].m_ConsumeType[2] = ThirdFile.Search_Posistion(i, 8)->iValue;
		g_ChatConsumeTbl.m_ChatComsumeType[i].m_ConsumeValue[2] = ThirdFile.Search_Posistion(i, 9)->iValue;
		if( g_ChatConsumeTbl.m_ChatComsumeType[i].m_ConsumeType[2]!=CHAT_NEED_NONE ) g_ChatConsumeTbl.m_ChatComsumeType[i].m_ConsumeCount = 3 ;
		g_ChatConsumeTbl.m_ChatComsumeType[i].m_NeedLevel = ThirdFile.Search_Posistion(i, 10)->iValue;
	}

	Log::SaveLog( SERVER_LOGFILE, "Load ChatConfig.txt ... OK! " ) ;

	__LEAVE_FUNCTION
}

VOID	TableInit::InitHairStyleTable( )
{
__ENTER_FUNCTION

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_HAIRSTYLE );
	if( ret != TRUE )
	{
		Assert( FALSE && "Read Hair Style table failed, please check it." );
		return;
	}

	INT iTableCount = ThirdFile.GetRecordsNum();
	g_HairStyleTbl.m_uCount = iTableCount;

	Assert(iTableCount<=MAX_HAIR_STYLE_INTABLE);

	for(INT i = 0; i<iTableCount; i++)
	{
		g_HairStyleTbl.m_HairStyle[i].m_nIndex
			=	ThirdFile.Search_Posistion(i, _HAIR_STYLE_TYPE_::HAIRSTYLE_INDEX)->iValue;
		g_HairStyleTbl.m_HairStyle[i].m_nRaceID
			=	ThirdFile.Search_Posistion(i, _HAIR_STYLE_TYPE_::HAIRSTYLE_RACEID)->iValue;
		g_HairStyleTbl.m_HairStyle[i].m_uItemSerial
			=	ThirdFile.Search_Posistion(i, _HAIR_STYLE_TYPE_::HAIRSTYLE_ITEMSERIAL)->iValue;
		g_HairStyleTbl.m_HairStyle[i].m_nItemCostCount
			=	ThirdFile.Search_Posistion(i, _HAIR_STYLE_TYPE_::HAIRSTYLE_ITEMCOSTCOUNT)->iValue;
		g_HairStyleTbl.m_HairStyle[i].m_nUseScope
			=	ThirdFile.Search_Posistion(i, _HAIR_STYLE_TYPE_::HAIRSTYLE_USESCOPE)->iValue;
	}

	Log::SaveLog( SERVER_LOGFILE, "Load HairStyle.txt ... OK! " ) ;

__LEAVE_FUNCTION
}

VOID	TableInit::InitAIStateList()
{
__ENTER_FUNCTION
	g_StateList.InitStateList();
	Log::SaveLog( SERVER_LOGFILE, "Load InitAIStateList ... OK! " ) ;
__LEAVE_FUNCTION
}

VOID	TableInit::CleanUpAIStateList()
{
__ENTER_FUNCTION
	g_StateList.ReleaseState();
	Log::SaveLog( SERVER_LOGFILE, "Load CleanUpAIStateList ... OK! " ) ;
__LEAVE_FUNCTION
}

VOID TableInit::InitNPCPaoPaoIndexTable()
{
__ENTER_FUNCTION
	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_NPCPAOPAOINDEXTBL );

	INT iTableCount = ThirdFile.GetRecordsNum();

	Assert(iTableCount<=MAXTYPE_NUMBER);

	memset(g_NPCPaoPaoIndexTbl,-1, sizeof(_NPC_PAOPAO_INDEX_TBL)*MAXTYPE_NUMBER );

	INT nIndex = 0;
	for (INT i = 0; i < iTableCount; ++i)
	{
		nIndex = ThirdFile.Search_Posistion(i, 0)->iValue;
		g_NPCPaoPaoIndexTbl[nIndex].m_nBaseIndex = ThirdFile.Search_Posistion(i, 1)->iValue;
		g_NPCPaoPaoIndexTbl[nIndex].m_nIndexStep = ThirdFile.Search_Posistion(i, 2)->iValue;
	}
	Log::SaveLog( SERVER_LOGFILE, "Load NPCPaoPaoIndexTable.txt ... OK! " ) ;
__LEAVE_FUNCTION
}

VOID TableInit::InitGMTable( )
{
__ENTER_FUNCTION

	Ini ini( FILE_GMLIST ) ;

	_GM_TABLE tmp ;

	tmp.m_uCount = ini.ReadInt( "gm", "count" ) ;
	for( INT i=0; i<tmp.m_uCount; i++ )
	{
		CHAR szguid[64] ;
		sprintf( szguid, "guid%d", i ) ;
		tmp.m_aGUID[i] = (GUID_t)(ini.ReadInt( "gm", szguid )) ;
	}

	g_GMTbl = tmp ;

	Log::SaveLog( SERVER_LOGFILE, "Load GMList.txt ... OK! " ) ;

__LEAVE_FUNCTION
}

VOID			TableInit::InitMissionDataTable()
{
__ENTER_FUNCTION
	enum MissionDataTableIndex
	{
		DataIdx_MissionID,
		DataIdx_MissionClass,
		DataIdx_MissionSubClass,
		DataIdx_MissionType,
		DataIdx_MissionLevel,
		DataIdx_Level,
		DataIdx_MaxLeve,
		DataIdx_Camp,
		DataIdx_Identity,
		DataIdx_Item1ID,
		DataIdx_Item1Num,
		DataIdx_Item2ID,
		DataIdx_Item2Num,
		DataIdx_Item3ID,
		DataIdx_Item3Num,
		DataIdx_Item4ID,
		DataIdx_Item4Num,
		DataIdx_Item5ID,
		DataIdx_Item5Num,
		DataIdx_Item6ID,
		DataIdx_Item6Num,
		DataIdx_Item7ID,
		DataIdx_Item7Num,
		DataIdx_Monster1ID,
		DataIdx_Monster1Num,
		DataIdx_Monster2ID,
		DataIdx_Monster2Num,
		DataIdx_Monster3ID,
		DataIdx_Monster3Num,
		DataIdx_Monster4ID,
		DataIdx_Monster4Num,
		DataIdx_Money1,
		DataIdx_Money2,
		DataIdx_MenPai,
		DataIdx_GuildLevel,
		DataIdx_MissionKind,
		DataIdx_TotalTimes,
		DataIdx_DayTimes,
		DataIdx_TimeDist,
		DataIdx_MaxCycle,
		DataIdx_PositionTag,
		DataIdx_PreMission,
		DataIdx_AftMission,
	};	
	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_MISSION_DATA );

	INT iTableCount = ThirdFile.GetRecordsNum();
	INT iTableColumn = ThirdFile.GetFieldsNum();

	Assert(iTableColumn==43);
	Assert(iTableCount>0);

	g_MissionDataTable.m_MissionData = new _MISSION_DATA_t[iTableCount];
	g_MissionDataTable.m_nMissionDataCount = iTableCount;

	for (INT i = 0; i < iTableCount; ++i)
	{
		g_MissionDataTable.m_MissionData[i].nMissionID			= ThirdFile.Search_Posistion(i, DataIdx_MissionID)->iValue;
		g_MissionDataTable.m_MissionData[i].nMissionClass		= ThirdFile.Search_Posistion(i, DataIdx_MissionClass)->iValue;
		g_MissionDataTable.m_MissionData[i].nMissionSubClass	= ThirdFile.Search_Posistion(i, DataIdx_MissionSubClass)->iValue;
		g_MissionDataTable.m_MissionData[i].nMissionType		= ThirdFile.Search_Posistion(i, DataIdx_MissionType)->iValue;
		g_MissionDataTable.m_MissionData[i].nMissionLevel		= ThirdFile.Search_Posistion(i, DataIdx_MissionLevel)->iValue;
		g_MissionDataTable.m_MissionData[i].nLevel				= ThirdFile.Search_Posistion(i, DataIdx_Level)->iValue;
		g_MissionDataTable.m_MissionData[i].nMaxLeve			= ThirdFile.Search_Posistion(i, DataIdx_MaxLeve)->iValue;
		g_MissionDataTable.m_MissionData[i].nCamp				= ThirdFile.Search_Posistion(i, DataIdx_Camp)->iValue;
		g_MissionDataTable.m_MissionData[i].nIdentity			= ThirdFile.Search_Posistion(i, DataIdx_Identity)->iValue;
		g_MissionDataTable.m_MissionData[i].nItem1ID			= ThirdFile.Search_Posistion(i, DataIdx_Item1ID)->iValue;
		g_MissionDataTable.m_MissionData[i].nItem1Num			= ThirdFile.Search_Posistion(i, DataIdx_Item1Num)->iValue;
		g_MissionDataTable.m_MissionData[i].nItem2ID			= ThirdFile.Search_Posistion(i, DataIdx_Item2ID)->iValue;
		g_MissionDataTable.m_MissionData[i].nItem2Num			= ThirdFile.Search_Posistion(i, DataIdx_Item2Num)->iValue;
		g_MissionDataTable.m_MissionData[i].nItem3ID			= ThirdFile.Search_Posistion(i, DataIdx_Item3ID)->iValue;
		g_MissionDataTable.m_MissionData[i].nItem3Num			= ThirdFile.Search_Posistion(i, DataIdx_Item3Num)->iValue;
		g_MissionDataTable.m_MissionData[i].nItem4ID			= ThirdFile.Search_Posistion(i, DataIdx_Item4ID)->iValue;
		g_MissionDataTable.m_MissionData[i].nItem4Num			= ThirdFile.Search_Posistion(i, DataIdx_Item4Num)->iValue;
		g_MissionDataTable.m_MissionData[i].nItem5ID			= ThirdFile.Search_Posistion(i, DataIdx_Item5ID)->iValue;
		g_MissionDataTable.m_MissionData[i].nItem5Num			= ThirdFile.Search_Posistion(i, DataIdx_Item5Num)->iValue;
		g_MissionDataTable.m_MissionData[i].nItem6ID			= ThirdFile.Search_Posistion(i, DataIdx_Item6ID)->iValue;
		g_MissionDataTable.m_MissionData[i].nItem6Num			= ThirdFile.Search_Posistion(i, DataIdx_Item6Num)->iValue;
		g_MissionDataTable.m_MissionData[i].nItem7ID			= ThirdFile.Search_Posistion(i, DataIdx_Item7ID)->iValue;
		g_MissionDataTable.m_MissionData[i].nItem7Num			= ThirdFile.Search_Posistion(i, DataIdx_Item7Num)->iValue;
		g_MissionDataTable.m_MissionData[i].nMonster1ID			= ThirdFile.Search_Posistion(i, DataIdx_Monster1ID)->iValue;
		g_MissionDataTable.m_MissionData[i].nMonster1Num		= ThirdFile.Search_Posistion(i, DataIdx_Monster1Num)->iValue;
		g_MissionDataTable.m_MissionData[i].nMonster2ID			= ThirdFile.Search_Posistion(i, DataIdx_Monster2ID)->iValue;
		g_MissionDataTable.m_MissionData[i].nMonster2Num		= ThirdFile.Search_Posistion(i, DataIdx_Monster2Num)->iValue;
		g_MissionDataTable.m_MissionData[i].nMonster3ID			= ThirdFile.Search_Posistion(i, DataIdx_Monster3ID)->iValue;
		g_MissionDataTable.m_MissionData[i].nMonster3Num		= ThirdFile.Search_Posistion(i, DataIdx_Monster3Num)->iValue;
		g_MissionDataTable.m_MissionData[i].nMonster4ID			= ThirdFile.Search_Posistion(i, DataIdx_Monster4ID)->iValue;
		g_MissionDataTable.m_MissionData[i].nMonster4Num		= ThirdFile.Search_Posistion(i, DataIdx_Monster4Num)->iValue;
		g_MissionDataTable.m_MissionData[i].nMoney1				= ThirdFile.Search_Posistion(i, DataIdx_Money1)->iValue;
		g_MissionDataTable.m_MissionData[i].nMoney2				= ThirdFile.Search_Posistion(i, DataIdx_Money2)->iValue;
		g_MissionDataTable.m_MissionData[i].nMenPai				= ThirdFile.Search_Posistion(i, DataIdx_MenPai)->iValue;
		g_MissionDataTable.m_MissionData[i].nGuildLevel			= ThirdFile.Search_Posistion(i, DataIdx_GuildLevel)->iValue;
		g_MissionDataTable.m_MissionData[i].nMissionKind		= ThirdFile.Search_Posistion(i, DataIdx_MissionKind)->iValue;
		g_MissionDataTable.m_MissionData[i].nTotalTimes			= ThirdFile.Search_Posistion(i, DataIdx_TotalTimes)->iValue;
		g_MissionDataTable.m_MissionData[i].nDayTimes			= ThirdFile.Search_Posistion(i, DataIdx_DayTimes)->iValue;
		g_MissionDataTable.m_MissionData[i].nTimeDist			= ThirdFile.Search_Posistion(i, DataIdx_TimeDist)->iValue;
		g_MissionDataTable.m_MissionData[i].nMaxCycle			= ThirdFile.Search_Posistion(i, DataIdx_MaxCycle)->iValue;
		g_MissionDataTable.m_MissionData[i].nPositionTag		= ThirdFile.Search_Posistion(i, DataIdx_PositionTag)->iValue;
		g_MissionDataTable.m_MissionData[i].nPreMission			= ThirdFile.Search_Posistion(i, DataIdx_PreMission)->iValue;
		g_MissionDataTable.m_MissionData[i].nAftMission			= ThirdFile.Search_Posistion(i, DataIdx_AftMission)->iValue;
	}

	Log::SaveLog( SERVER_LOGFILE, "Load MissionData.txt ... OK! " ) ;
__LEAVE_FUNCTION
}

VOID			TableInit::CleanMissionData()
{
	__ENTER_FUNCTION

	SAFE_DELETE_ARRAY( g_MissionDataTable.m_MissionData );
	g_MissionDataTable.m_nMissionDataCount=0;

	__LEAVE_FUNCTION
}

INT CompareMissionDataTB(const VOID* pArg1,const VOID* pArg2)
{	
	_MISSION_DATA_t* pData1	=	(_MISSION_DATA_t*)pArg1;
	_MISSION_DATA_t* pData2	=	(_MISSION_DATA_t*)pArg2;

	if( pData1->nMissionID > pData2->nMissionID )
		return 1;
	else if( pData1->nMissionID < pData2->nMissionID )
		return -1;
	else
		return 0;
}

_MISSION_DATA_t*	_MISSION_DATA::Get(MissionID_t nMissionID)
{
	_MISSION_DATA_t	tb;

	tb.nMissionID	=	nMissionID;

	_MISSION_DATA_t* pResult = (_MISSION_DATA_t*)bsearch(&tb,
		m_MissionData,
		m_nMissionDataCount,
		sizeof(_MISSION_DATA_t),
		(INT (*)(const VOID*, const VOID*))CompareMissionDataTB);

	if(pResult)
		return pResult;
	else
	{
		Log::SaveLog( SERVER_LOGFILE, "Search _MISSION_DATA  MissionID:%d Get Errors", 
			nMissionID) ;
		Assert(FALSE);
	}
	return	0;

}

VOID			TableInit::InitMissionRewardTable()
{
__ENTER_FUNCTION
	enum
	{
		RewardIdx_RewardID,
		RewardIdx_Money,
		RewardIdx_Item1ID,
		RewardIdx_Item1Num,
		RewardIdx_Item2ID,
		RewardIdx_Item2Num,
		RewardIdx_Item3ID,
		RewardIdx_Item3Num,
		RewardIdx_Item4ID,
		RewardIdx_Item4Num,
		RewardIdx_Item5ID,
		RewardIdx_Item5Num,
		RewardIdx_Item6ID,
		RewardIdx_Item6Num,
		RewardIdx_Exp,
		RewardIdx_Level,
		RewardIdx_GuildExp1,
		RewardIdx_GuildExp2,
		RewardIdx_GuildExp3,
		RewardIdx_Buff1ID,
		RewardIdx_Buff2ID,
		RewardIdx_Buff3ID,
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_MISSION_REWARD );

	INT iTableCount = ThirdFile.GetRecordsNum();
	INT iTableColumn = ThirdFile.GetFieldsNum();

	Assert(iTableColumn==22);
	Assert(iTableCount>0);

	g_MissionRewardTable.m_MissionReward = new _MISSION_REWARD_t[iTableCount];
	g_MissionRewardTable.m_nMissionRewardCount = iTableCount;

	for (INT i = 0; i < iTableCount; ++i)
	{
		g_MissionRewardTable.m_MissionReward[i].nRewardID			= ThirdFile.Search_Posistion(i, RewardIdx_RewardID)->iValue;
		g_MissionRewardTable.m_MissionReward[i].nMoney				= ThirdFile.Search_Posistion(i, RewardIdx_Money)->iValue;
		g_MissionRewardTable.m_MissionReward[i].nItem1ID			= ThirdFile.Search_Posistion(i, RewardIdx_Item1ID)->iValue;
		g_MissionRewardTable.m_MissionReward[i].nItem1Num			= ThirdFile.Search_Posistion(i, RewardIdx_Item1Num)->iValue;
		g_MissionRewardTable.m_MissionReward[i].nItem2ID			= ThirdFile.Search_Posistion(i, RewardIdx_Item2ID)->iValue;
		g_MissionRewardTable.m_MissionReward[i].nItem2Num			= ThirdFile.Search_Posistion(i, RewardIdx_Item2Num)->iValue;
		g_MissionRewardTable.m_MissionReward[i].nItem3ID			= ThirdFile.Search_Posistion(i, RewardIdx_Item3ID)->iValue;
		g_MissionRewardTable.m_MissionReward[i].nItem3Num			= ThirdFile.Search_Posistion(i, RewardIdx_Item3Num)->iValue;
		g_MissionRewardTable.m_MissionReward[i].nItem4ID			= ThirdFile.Search_Posistion(i, RewardIdx_Item4ID)->iValue;
		g_MissionRewardTable.m_MissionReward[i].nItem4Num			= ThirdFile.Search_Posistion(i, RewardIdx_Item4Num)->iValue;
		g_MissionRewardTable.m_MissionReward[i].nItem5ID			= ThirdFile.Search_Posistion(i, RewardIdx_Item5ID)->iValue;
		g_MissionRewardTable.m_MissionReward[i].nItem5Num			= ThirdFile.Search_Posistion(i, RewardIdx_Item5Num)->iValue;
		g_MissionRewardTable.m_MissionReward[i].nItem6ID			= ThirdFile.Search_Posistion(i, RewardIdx_Item6ID)->iValue;
		g_MissionRewardTable.m_MissionReward[i].nItem6Num			= ThirdFile.Search_Posistion(i, RewardIdx_Item6Num)->iValue;
		g_MissionRewardTable.m_MissionReward[i].nExp				= ThirdFile.Search_Posistion(i, RewardIdx_Exp)->iValue;
		g_MissionRewardTable.m_MissionReward[i].nLevel				= ThirdFile.Search_Posistion(i, RewardIdx_Level)->iValue;
		g_MissionRewardTable.m_MissionReward[i].nGuildExp1			= ThirdFile.Search_Posistion(i, RewardIdx_GuildExp1)->iValue;
		g_MissionRewardTable.m_MissionReward[i].nGuildExp2			= ThirdFile.Search_Posistion(i, RewardIdx_GuildExp2)->iValue;
		g_MissionRewardTable.m_MissionReward[i].nGuildExp3			= ThirdFile.Search_Posistion(i, RewardIdx_GuildExp3)->iValue;
		g_MissionRewardTable.m_MissionReward[i].nBuff1ID			= ThirdFile.Search_Posistion(i, RewardIdx_Buff1ID)->iValue;
		g_MissionRewardTable.m_MissionReward[i].nBuff2ID			= ThirdFile.Search_Posistion(i, RewardIdx_Buff2ID)->iValue;
		g_MissionRewardTable.m_MissionReward[i].nBuff3ID			= ThirdFile.Search_Posistion(i, RewardIdx_Buff3ID)->iValue;
	}

	Log::SaveLog( SERVER_LOGFILE, "Load MissionReward.txt ... OK! " ) ;
__LEAVE_FUNCTION
}

VOID			TableInit::CleanMissionReward()
{
__ENTER_FUNCTION

	SAFE_DELETE_ARRAY( g_MissionRewardTable.m_MissionReward );
	g_MissionRewardTable.m_nMissionRewardCount=0;

__LEAVE_FUNCTION
}

INT CompareMissionRewardTB(const VOID* pArg1,const VOID* pArg2)
{	
	_MISSION_REWARD_t* pData1	=	(_MISSION_REWARD_t*)pArg1;
	_MISSION_REWARD_t* pData2	=	(_MISSION_REWARD_t*)pArg2;

	if( pData1->nRewardID > pData2->nRewardID )
		return 1;
	else if( pData1->nRewardID < pData2->nRewardID )
		return -1;
	else
		return 0;
}

_MISSION_REWARD_t*	_MISSION_REWARD::Get(SHORT nMissionRewardID)
{
	_MISSION_REWARD_t	tb;

	tb.nRewardID	=	nMissionRewardID;

	_MISSION_REWARD_t* pResult = (_MISSION_REWARD_t*)bsearch(&tb,
		m_MissionReward,
		m_nMissionRewardCount,
		sizeof(_MISSION_REWARD_t),
		(INT (*)(const VOID*, const VOID*))CompareMissionRewardTB);

	if(pResult)
		return pResult;
	else
	{
		Log::SaveLog( SERVER_LOGFILE, "Search _MISSION_REWARD  RewardID:%d Get Errors", 
			nMissionRewardID) ;
		Assert(FALSE);
	}
	return	0;
}

VOID			TableInit::InitMissionPunishTable()
{
__ENTER_FUNCTION

	enum
	{
		PunishIdx_PunishID,
		PunishIdx_Money,
		PunishIdx_Item1ID,
		PunishIdx_Item1Num,
		PunishIdx_Item2ID,
		PunishIdx_Item2Num,
		PunishIdx_Item3ID,
		PunishIdx_Item3Num,
		PunishIdx_Exp,
		PunishIdx_Level,
		PunishIdx_MissionTag,
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_MISSION_PUNISH );

	INT iTableCount = ThirdFile.GetRecordsNum();
	INT iTableColumn = ThirdFile.GetFieldsNum();

	Assert(iTableColumn==11);
	Assert(iTableCount>0);

	g_MissionPunishTable.m_MissionPunish = new _MISSION_PUNISH_t[iTableCount];
	g_MissionPunishTable.m_nMissionPunishCount = iTableCount;

	for (INT i = 0; i < iTableCount; ++i)
	{
		g_MissionPunishTable.m_MissionPunish[i].nPunishID		= ThirdFile.Search_Posistion(i, PunishIdx_PunishID)->iValue;
		g_MissionPunishTable.m_MissionPunish[i].nMoney			= ThirdFile.Search_Posistion(i, PunishIdx_Money)->iValue;
		g_MissionPunishTable.m_MissionPunish[i].nItem1ID		= ThirdFile.Search_Posistion(i, PunishIdx_Item1ID)->iValue;
		g_MissionPunishTable.m_MissionPunish[i].nItem1Num		= ThirdFile.Search_Posistion(i, PunishIdx_Item1Num)->iValue;
		g_MissionPunishTable.m_MissionPunish[i].nItem2ID		= ThirdFile.Search_Posistion(i, PunishIdx_Item2ID)->iValue;
		g_MissionPunishTable.m_MissionPunish[i].nItem2Num		= ThirdFile.Search_Posistion(i, PunishIdx_Item2Num)->iValue;
		g_MissionPunishTable.m_MissionPunish[i].nItem3ID		= ThirdFile.Search_Posistion(i, PunishIdx_Item3ID)->iValue;
		g_MissionPunishTable.m_MissionPunish[i].nItem3Num		= ThirdFile.Search_Posistion(i, PunishIdx_Item3Num)->iValue;
		g_MissionPunishTable.m_MissionPunish[i].nExp			= ThirdFile.Search_Posistion(i, PunishIdx_Exp)->iValue;
		g_MissionPunishTable.m_MissionPunish[i].nLevel			= ThirdFile.Search_Posistion(i, PunishIdx_Level)->iValue;
		g_MissionPunishTable.m_MissionPunish[i].nMissionTag		= ThirdFile.Search_Posistion(i, PunishIdx_MissionTag)->iValue;
	}

	Log::SaveLog( SERVER_LOGFILE, "Load MissionPunish.txt ... OK! " ) ;
__LEAVE_FUNCTION

}

VOID			TableInit::CleanMissionPunish()
{
__ENTER_FUNCTION

	SAFE_DELETE_ARRAY( g_MissionPunishTable.m_MissionPunish );
	g_MissionPunishTable.m_nMissionPunishCount=0;

__LEAVE_FUNCTION
}

INT CompareMissionPunishTB(const VOID* pArg1,const VOID* pArg2)
{	
	_MISSION_PUNISH_t* pData1	=	(_MISSION_PUNISH_t*)pArg1;
	_MISSION_PUNISH_t* pData2	=	(_MISSION_PUNISH_t*)pArg2;

	if( pData1->nPunishID > pData2->nPunishID )
		return 1;
	else if( pData1->nPunishID < pData2->nPunishID )
		return -1;
	else
		return 0;
}

_MISSION_PUNISH_t*	_MISSION_PUNISH::Get(SHORT nMissionPunishID)
{
	_MISSION_PUNISH_t	tb;

	tb.nPunishID	=	nMissionPunishID;

	_MISSION_PUNISH_t* pResult = (_MISSION_PUNISH_t*)bsearch(&tb,
		m_MissionPunish,
		m_nMissionPunishCount,
		sizeof(_MISSION_PUNISH_t),
		(INT (*)(const VOID*, const VOID*))CompareMissionPunishTB);

	if(pResult)
		return pResult;
	else
	{
		Log::SaveLog( SERVER_LOGFILE, "Search _MISSION_PUNISH  PunishID:%d Get Errors", 
			nMissionPunishID) ;
		Assert(FALSE);
	}
	return	0;
}

VOID			TableInit::InitMissionDialogTable()
{
__ENTER_FUNCTION

	enum
	{
		DialogIdx_DialogID,
		DialogIdx_MissionName,
		DialogIdx_MissionDesc,
		DialogIdx_MissionTarget,
		DialogIdx_MissionContinue,
		DialogIdx_MissionAbandon,
		DialogIdx_MissionSuccess,
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_MISSION_DIALOG );

	INT iTableCount = ThirdFile.GetRecordsNum();
	INT iTableColumn = ThirdFile.GetFieldsNum();

	Assert(iTableColumn==7);
	Assert(iTableCount>0);

	g_MissionDialogTable.m_MissionDialog = new _MISSION_DIALOG_t[iTableCount];
	g_MissionDialogTable.m_nMissionDialogCount = iTableCount;

	memset( g_MissionDialogTable.m_MissionDialog, 0, sizeof(_MISSION_DIALOG_t) * iTableCount );

	for (INT i = 0; i < iTableCount; ++i)
	{
		g_MissionDialogTable.m_MissionDialog[i].nDialogID		= ThirdFile.Search_Posistion(i, DialogIdx_DialogID)->iValue;
		strncpy( g_MissionDialogTable.m_MissionDialog[i].szMissionName, ThirdFile.Search_Posistion(i,DialogIdx_MissionName)->pString, sizeof( g_MissionDialogTable.m_MissionDialog[i].szMissionName ) - 1 );
		strncpy( g_MissionDialogTable.m_MissionDialog[i].szMissionDesc, ThirdFile.Search_Posistion(i,DialogIdx_MissionDesc)->pString, sizeof( g_MissionDialogTable.m_MissionDialog[i].szMissionDesc ) - 1 );
		strncpy( g_MissionDialogTable.m_MissionDialog[i].szMissionTarget, ThirdFile.Search_Posistion(i,DialogIdx_MissionTarget)->pString, sizeof( g_MissionDialogTable.m_MissionDialog[i].szMissionTarget ) - 1 );
		strncpy( g_MissionDialogTable.m_MissionDialog[i].szMissionContinue, ThirdFile.Search_Posistion(i,DialogIdx_MissionContinue)->pString, sizeof( g_MissionDialogTable.m_MissionDialog[i].szMissionContinue ) - 1 );
		strncpy( g_MissionDialogTable.m_MissionDialog[i].szMissionAbandon, ThirdFile.Search_Posistion(i,DialogIdx_MissionAbandon)->pString, sizeof( g_MissionDialogTable.m_MissionDialog[i].szMissionAbandon ) - 1 );
		strncpy( g_MissionDialogTable.m_MissionDialog[i].szMissionSuccess, ThirdFile.Search_Posistion(i,DialogIdx_MissionSuccess)->pString, sizeof( g_MissionDialogTable.m_MissionDialog[i].szMissionSuccess ) - 1 );
	}

	Log::SaveLog( SERVER_LOGFILE, "Load MissionDialog.txt ... OK! " ) ;
__LEAVE_FUNCTION
}

VOID			TableInit::CleanMissionDialog()
{
__ENTER_FUNCTION

	SAFE_DELETE_ARRAY( g_MissionDialogTable.m_MissionDialog );
	g_MissionDialogTable.m_nMissionDialogCount=0;

__LEAVE_FUNCTION

}

INT CompareMissionDialogTB(const VOID* pArg1,const VOID* pArg2)
{	
	_MISSION_DIALOG_t* pData1	=	(_MISSION_DIALOG_t*)pArg1;
	_MISSION_DIALOG_t* pData2	=	(_MISSION_DIALOG_t*)pArg2;

	if( pData1->nDialogID > pData2->nDialogID )
		return 1;
	else if( pData1->nDialogID < pData2->nDialogID )
		return -1;
	else
		return 0;
}

_MISSION_DIALOG_t*	_MISSION_DIALOG::Get(SHORT nMissionDialogID)
{
	_MISSION_DIALOG_t	tb;

	tb.nDialogID	=	nMissionDialogID;

	_MISSION_DIALOG_t* pResult = (_MISSION_DIALOG_t*)bsearch(&tb,
		m_MissionDialog,
		m_nMissionDialogCount,
		sizeof(_MISSION_DIALOG_t),
		(INT (*)(const VOID*, const VOID*))CompareMissionDialogTB);

	if(pResult)
		return pResult;
	else
	{
		Log::SaveLog( SERVER_LOGFILE, "Search _MISSION_DIALOG  PunishID:%d Get Errors", 
			nMissionDialogID) ;
		Assert(FALSE);
	}
	return	0;
}

VOID			TableInit::InitMissionListTable()
{
__ENTER_FUNCTION

	enum
	{
		ListIdx_ScriptID,
		ListIdx_MissionID,
		ListIdx_RewardID,
		ListIdx_PunishID,
		ListIdx_DialogID,
		ListIdx_PromulgatorScene,
		ListIdx_PromulgatorID,
		ListIdx_SubmitorScene,
		ListIdx_SubmitorID,
		ListIdx_DestScene,
		ListIdx_DestX,
		ListIdx_DestZ,
	};

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_MISSION_LIST );

	INT iTableCount = ThirdFile.GetRecordsNum();
	INT iTableColumn = ThirdFile.GetFieldsNum();

	Assert(iTableColumn==12);
	Assert(iTableCount>0);

	_MISSION_LIST_t MissionList;
	INT nPromulgator, nSubmitor;
	for (INT i = 0; i < iTableCount; ++i)
	{
		MissionList.nScriptID			= ThirdFile.Search_Posistion(i, ListIdx_ScriptID)->iValue;
		MissionList.nMissionID			= ThirdFile.Search_Posistion(i, ListIdx_MissionID)->iValue;
		MissionList.nReward				= ThirdFile.Search_Posistion(i, ListIdx_RewardID)->iValue;
		MissionList.nPunish				= ThirdFile.Search_Posistion(i, ListIdx_PunishID)->iValue;
		MissionList.nDialog				= ThirdFile.Search_Posistion(i, ListIdx_DialogID)->iValue;
		MissionList.nPromulgatorScene	= ThirdFile.Search_Posistion(i, ListIdx_PromulgatorScene)->iValue;
		MissionList.nPromulgatorID		= ThirdFile.Search_Posistion(i, ListIdx_PromulgatorID)->iValue;
		MissionList.nSubmitorScene		= ThirdFile.Search_Posistion(i, ListIdx_SubmitorScene)->iValue;
		MissionList.nSubmitorID			= ThirdFile.Search_Posistion(i, ListIdx_SubmitorID)->iValue;
		MissionList.nDestScene			= ThirdFile.Search_Posistion(i, ListIdx_DestScene)->iValue;
		MissionList.nDestX				= ThirdFile.Search_Posistion(i, ListIdx_DestX)->iValue;
		MissionList.nDestZ				= ThirdFile.Search_Posistion(i, ListIdx_DestZ)->iValue;

		nPromulgator = nSubmitor = 0;
		//if( MissionList.nPromulgatorID != INVALID_ID )--需求变更、暂时不需要
			nPromulgator = MissionList.nPromulgatorScene * 10000 + MissionList.nPromulgatorID;
		//if( MissionList.nSubmitorID != INVALID_ID )--需求变更、暂时不需要
			nSubmitor = MissionList.nSubmitorScene * 10000 + MissionList.nSubmitorID;

		if( nPromulgator == nSubmitor )
		{
			g_PromulgatorAndSubmitorIndex[nPromulgator].push_back( MissionList );
		}
		else
		{
			g_PromulgatorIndex[nPromulgator].push_back( MissionList );
			g_SubmitorIndex[nSubmitor].push_back( MissionList );
		}

		g_MissionIndexByScriptID[MissionList.nScriptID] = MissionList;
	}

	Log::SaveLog( SERVER_LOGFILE, "Load MissionList.txt ... OK! " ) ;
__LEAVE_FUNCTION
}

BOOL			TableInit::GetMissionListInfo( INT nMissionIndex, INT nScriptID, INT nIdent, _MISSION_LIST_t& one )
{
	BOOL bFind = FALSE;
	map<INT,vector<_MISSION_LIST_t> >::iterator it;
	if( nIdent == 3 )
	{
		it = g_PromulgatorAndSubmitorIndex.find( nMissionIndex );
		if( it != g_PromulgatorAndSubmitorIndex.end() )
			bFind = TRUE;
	}
	else if( nIdent == 1 )
	{
		it = g_PromulgatorIndex.find( nMissionIndex );
		if( it != g_PromulgatorIndex.end() )
			bFind = TRUE;
	}
	else if( nIdent == 2 )
	{
		it = g_SubmitorIndex.find( nMissionIndex );
		if( it != g_SubmitorIndex.end() )
			bFind = TRUE;
	}
	else
	{
		it = g_PromulgatorAndSubmitorIndex.find( nMissionIndex );
		if( it == g_PromulgatorAndSubmitorIndex.end() )
		{
			it = g_PromulgatorIndex.find( nMissionIndex );
			if( it == g_PromulgatorIndex.end() )
			{
				it = g_SubmitorIndex.find( nMissionIndex );
				if( it != g_SubmitorIndex.end() )
				{
					bFind = TRUE;
				}
			}
			else
			{
				bFind = TRUE;
			}
		}
		else
		{
			bFind = TRUE;
		}
	}

	if( bFind == FALSE ) return FALSE;

	bFind = FALSE;
	vector<_MISSION_LIST_t>& vList = it->second;
	vector<_MISSION_LIST_t>::iterator vit;
	for( vit = vList.begin(); vit != vList.end(); ++vit )
	{
		one = (_MISSION_LIST_t)(*vit);
		if( one.nScriptID == nScriptID )
		{
			bFind = TRUE;
			break;
		}
	}
	return( bFind );
}

BOOL			TableInit::GetMissionListInfo( INT nScriptID, _MISSION_LIST_t& one )
{
	map<INT,_MISSION_LIST_t>::iterator it;
	it = g_MissionIndexByScriptID.find( nScriptID );
	if( it != g_MissionIndexByScriptID.end() )
	{
		one = it->second;
		return TRUE;
	}

	return FALSE;
}


