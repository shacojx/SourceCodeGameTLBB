
/////////////////////////////////////////////////////////////////////////////////
//文件名：WorldTable.h
//功能描述：世界数据服务器所用到的资源表格
//修改记录：
//
/////////////////////////////////////////////////////////////////////////////////

#ifndef __WROLDTABLE_H__
#define __WROLDTABLE_H__

#include "Type.h"
#include "../Common/Combat/CampAndStand.h"

//帮派，城市 【职位】－【权限】对照表
#define MAX_MEMBER_TYPES 100
struct _GUILD_MEMBER_ACCESS
{
	INT		m_MemberTypeCount ;
	INT		m_Table[MAX_MEMBER_TYPES][GUILD_AUTHORITY_NUMBER] ;

	_GUILD_MEMBER_ACCESS( )
	{
		m_MemberTypeCount = 0 ;
		memset( m_Table, 0, sizeof(INT)*MAX_MEMBER_TYPES*GUILD_AUTHORITY_NUMBER ) ;
	};
	INT		Get( uint membertype, uint accesstype ){
		Assert( membertype<MAX_MEMBER_TYPES && accesstype<GUILD_AUTHORITY_NUMBER ) ;
		return m_Table[membertype][accesstype] ;
	};
};
extern _GUILD_MEMBER_ACCESS g_GuildMemberAccessTbl ;

struct _DBC_CITY_BUILDING
{
	INT		nID;					//id
	INT		nCityID;				//城市id(DBC_CITY_DEINFE)
	INT		nBuildingType;			//建筑类型
	INT		nBuildingID;			//建筑物id(DBC_BUILDING_DEINFE)
	INT		nInitialLevel;			//建筑物的初始级别
	INT		nLevelRequire1;			//升到1级所需条件
	INT		nLevelRequire2;			//升到2级所需条件
	INT		nLevelRequire3;			//升到3级所需条件
	INT		nLevelRequire4;			//升到4级所需条件
	INT		nLevelRequire5;			//升到4级所需条件
	INT		nResearchItem0;			//0级所能研究的配方
	INT		nResearchItem1;			//1级所能研究的配方
	INT		nResearchItem2;			//2级所能研究的配方
	INT		nResearchItem3;			//3级所能研究的配方
	INT		nResearchItem4;			//4级所能研究的配方
};

struct _DBC_CITY_DEFINE
{
	INT					m_Num;
	_DBC_CITY_BUILDING	m_Table[MAX_CITY_BUILDING];

	_DBC_CITY_DEFINE()
	{
		m_Num = 0;
		memset(m_Table,0,sizeof(_DBC_CITY_BUILDING)*MAX_CITY_BUILDING);
	}
};

extern _DBC_CITY_DEFINE g_CityBuildingTbl;

struct SKILL_DATA
{
	INT nSkillID;
	INT nMenPai;
};
extern SKILL_DATA*		g_SkillData; //技能数据
extern INT				g_SkillData_Count;
extern CampAndStandDataMgr_T g_CampAndStandDataMgr;

class TableInit
{
public :
	TableInit() ;
	~TableInit() ;

	BOOL			Init( ) ;

protected :
	VOID			Init_GuildMemberAccess() ;
	VOID			Init_CityBuilding() ;
	VOID			Init_SkillData();
	VOID			Init_CampAndStandDataMgr();

};
extern TableInit g_TableInit ;


#endif
