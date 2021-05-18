#include "stdafx.h"

#include "WorldTable.h"
#include "Log.h"
#include "Ini.h"
#include "FileDef.h"
#include "TLBB_DBC.h"

using namespace DBC;
_GUILD_MEMBER_ACCESS	g_GuildMemberAccessTbl ;
_DBC_CITY_DEFINE		g_CityBuildingTbl;
TableInit				g_TableInit ;
SKILL_DATA*				g_SkillData; //技能数据
INT						g_SkillData_Count=0;
CampAndStandDataMgr_T	g_CampAndStandDataMgr;

TableInit::TableInit( )
{
}

TableInit::~TableInit( )
{
	SAFE_DELETE(g_SkillData);
	g_SkillData_Count = 0;
}

BOOL TableInit::Init( )
{
__ENTER_FUNCTION
	Init_GuildMemberAccess( ) ;
	Init_CityBuilding();
	Init_SkillData();
	Init_CampAndStandDataMgr();
__LEAVE_FUNCTION

	return TRUE ;
}

VOID TableInit::Init_GuildMemberAccess( )
{
__ENTER_FUNCTION

	Ini ini( FILE_GUILD_MEMBER_ACCESS ) ;

	g_GuildMemberAccessTbl.m_MemberTypeCount = ini.ReadInt( "info", "membertypecount" ) ;
	Assert( g_GuildMemberAccessTbl.m_MemberTypeCount<MAX_MEMBER_TYPES ) ;

	for( INT i=0; i<g_GuildMemberAccessTbl.m_MemberTypeCount; i++ )
	{
		for( INT j=0; j<GUILD_AUTHORITY_NUMBER; j++ )
		{
			CHAR szSection[256], szName[256] ;
			sprintf( szSection, "member_type%d", i ) ;
			sprintf( szName, "access_type%d", j ) ;
			g_GuildMemberAccessTbl.m_Table[j][i] = ini.ReadInt( szSection, szName ) ;
		}
	}

	Log::SaveLog( SERVER_LOGFILE, "Load GuildMemberAccess.ini ... OK! " ) ;

__LEAVE_FUNCTION
}

VOID TableInit::Init_CityBuilding( )
{
__ENTER_FUNCTION


	DBCFile DBFile(0);
	BOOL ret = DBFile.OpenFromTXT( FILE_CITY_BUILDING );
	INT iTableCount = DBFile.GetRecordsNum();

	Assert(iTableCount<=MAX_CITY_BUILDING);

	INT nIndex = 0;
	for (INT i = 0; i < iTableCount; ++i)
	{
		nIndex = DBFile.Search_Posistion(i, 0)->iValue;
		g_CityBuildingTbl.m_Table[nIndex].nID			= nIndex;
		g_CityBuildingTbl.m_Table[nIndex].nCityID		= DBFile.Search_Posistion(i, 1)->iValue;
		g_CityBuildingTbl.m_Table[nIndex++].nBuildingID	= DBFile.Search_Posistion(i, 2)->iValue;
	}

	g_CityBuildingTbl.m_Num = iTableCount;

__LEAVE_FUNCTION
}

VOID TableInit::Init_SkillData()
{
	__ENTER_FUNCTION

	DBCFile ThirdFile(0);
	BOOL ret = ThirdFile.OpenFromTXT( FILE_SKILL_TEMPLATE );

	Assert(ret);

	INT iTableCount			=	ThirdFile.GetRecordsNum();
	INT iColumnCount		=	ThirdFile.GetFieldsNum();

	INT iCount = 0;
	for( INT i = 0; i < iTableCount; i ++ )
	{
		INT iMenPai = ThirdFile.Search_Posistion(i, 2)->iValue;
		INT iLevel  = ThirdFile.Search_Posistion(i, 5)->iValue;
		INT iClass  = ThirdFile.Search_Posistion(i, 21)->iValue;
		if( iClass == 0 )
		{
			if( iMenPai == MATTRIBUTE_SHAOLIN ||
				iMenPai == MATTRIBUTE_WUDANG ||
				iMenPai == MATTRIBUTE_EMEI ||
				iMenPai == INVALID_ID )
			{
				if( iLevel < 0 )
				{
					iCount++;
				}
			}
		}
	}
	Assert( iCount > 0 );

	g_SkillData = new SKILL_DATA[iCount];
	g_SkillData_Count = iCount;

	iCount = 0;
	for( INT i = 0; i < iTableCount; i ++ )
	{
		INT iMenPai = ThirdFile.Search_Posistion(i, 2)->iValue;
		INT iLevel  = ThirdFile.Search_Posistion(i, 5)->iValue;
		INT iClass  = ThirdFile.Search_Posistion(i, 21)->iValue;
		if( iClass == 0 )
		{
			if( iMenPai == MATTRIBUTE_SHAOLIN ||
				iMenPai == MATTRIBUTE_WUDANG ||
				iMenPai == MATTRIBUTE_EMEI ||
				iMenPai == INVALID_ID )
			{
				if( iLevel < 0 )
				{
					g_SkillData[iCount].nSkillID = ThirdFile.Search_Posistion(i, 0)->iValue;
					g_SkillData[iCount].nMenPai  = ThirdFile.Search_Posistion(i, 2)->iValue;
					iCount ++;
				}
			}
		}
	}

	__LEAVE_FUNCTION
}

VOID TableInit::Init_CampAndStandDataMgr()
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
	Assert(bRet);
	__LEAVE_FUNCTION
}