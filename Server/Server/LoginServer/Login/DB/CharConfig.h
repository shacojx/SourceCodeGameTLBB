#ifndef _CHAR_CONFIG_H_
#define _CHAR_CONFIG_H_
#include "Type.h"

struct FULLUSERDATA;

struct START_SCENE //出生地点
{
	INT		nSceneNum ; //场景号码
	FLOAT	fStartX ; //出生地点
	FLOAT	fStartZ ;
};

struct SKILL_DATA
{
	INT nSkillID;
	INT nMenPai;
};

struct ATTR_DATA
{
	INT str1;
	INT spr1;
	INT con1;
	INT int1;
	INT dex1;
};

struct BASE_VALUE_TBL
{
	//属性影响因素数量*门派数量
	INT		m_Table[AINFOTYPE_NUMBER][MATTRIBUTE_NUMBER] ;

	BASE_VALUE_TBL( )
	{
		memset( m_Table, 0, sizeof(INT)*AINFOTYPE_NUMBER*MATTRIBUTE_NUMBER ) ;
	};
	INT		Get( uint AttrInfo, uint MenPai ){
		Assert( AttrInfo<AINFOTYPE_NUMBER && MenPai<MATTRIBUTE_NUMBER ) ;
		return m_Table[AttrInfo][MenPai] ;
	};
};

class CharConfig
{
public:

	CharConfig();
	~CharConfig();
	BOOL		Init();
	BOOL		InitCharAttr(FULLUSERDATA* pData);	//在Login 上初始化角色基本信息

public:

	VOID InitDefaultCharData();

	VOID InitSkillData();

	VOID InitLevel1Attr();

	VOID InitBaseValueTable();

	FULLUSERDATA*	m_pUserData; //角色数据

	START_SCENE*	m_StartScene; //场景、阵营数据

	SKILL_DATA*		m_SkillData; //技能数据
	INT				m_SkillData_Count;

	ATTR_DATA		m_Level1Attr[MATTRIBUTE_NUMBER]; //等级1的属性数据

	BASE_VALUE_TBL	m_BaseValueTbl;
};


extern CharConfig g_CharConfig;

#endif