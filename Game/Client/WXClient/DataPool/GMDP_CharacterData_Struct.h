#pragma once

//========================================================
//
//			角色对象中完全由服务器决定的属性数据
//
//========================================================
#include "GameDefine.h"
#include "GameDefine2.h"
#include "TDDBC_Struct.h"
#include "GameStruct_Skill.h"

//=================================================================
//-->0. Character 基本属性
//=================================================================
struct SDATA_CHARACTER
{
	INT				m_nRaceID;					// 数据表中的ID						

	STRING			m_strName;					// 角色的名字，从服务器传来			
	STRING			m_strTitle;					//当前称号
	FLOAT			m_fHPPercent;				// 生命值百分比						
	FLOAT			m_fMPPercent;				// 魔法值百分比						
	FLOAT			m_fMoveSpeed;				// 移动的速度						
	INT				m_nCampID;					// 阵营ID							
	INT				m_nLevel;					// 等级								
	BOOL			m_bFightState;				// 战斗状态							
};

//=================================================================
//-->1. Character_NPC 对于NPC
//=================================================================
struct SDATA_NPC : public SDATA_CHARACTER
{

};

//=================================================================
//-->2. Character_PlayerOther 对于其他玩家
//=================================================================
struct _TITLE_
{
	enum
	{
		INVALID_TITLE = 0,
		ID_TITLE,
		STRING_TITLE,
	};
	BOOL		bFlag;							//表明此title是否是id
	BYTE		bType;							//表明此title的类型
	union
	{
		INT		ID;								//如果是id,这个就是表中id
		CHAR	szTitleData[MAX_CHARACTER_TITLE];	//如果不是id，这个就是title内容
	};
	_TITLE_()
	{
		bFlag		=	INVALID_TITLE;
		bType		=	_TITLE::NO_TITLE;	
		memset(szTitleData, 0, MAX_CHARACTER_TITLE);
	}
};
struct SDATA_PLAYER_OTHER : public SDATA_NPC
{
	INT				m_nMenPai;			// 门派ID									
										/*
										|	0、少林
										|	1、明教
										|	2、丐帮
										|	3、武当
										|	4、峨嵋
										|	5、星宿
										|	6、大理
										|	7、天山
										|	8、逍遥
										|	9、无门派
										*/

	//头发外形
	INT			m_nHairMeshID;			//-> DBC_CHARACTER_HAIR_GEO					
	INT			m_nHairMaterialID;		//-> DBC_CHARACTER_HAIR_MAT					

	//脸部外形
	INT			m_nFaceMeshID;			//-> DBC_CHARACTER_HEAD_GEO					
	INT			m_nFaceMaterialID;		//-> DBC_CHARACTER_HEAD_MAT					

	INT			m_nEquipVer;//角色的装备状态版本号，用于和服务器同步数据的依据		
	INT			m_nEquipmentID[HEQUIP_NUMBER];	//装备表,用于外形显示				

	BOOL		m_bTeamFlag; // 是否有队伍
	BOOL		m_bTeamLeaderFlag; // 是否是队长
	BOOL		m_bTeamFullFlag; // 是否是满队
	
	INT			m_nTitleNum;
	INT			m_nCurTitleIndex;
	_TITLE_		m_nTitleList[MAX_TITLE_SIZE];		//称号表							
};


//=================================================================
//-->3. Character_PlayerMySelf 对于玩家自己
//=================================================================

/*
	0)心法数据结构
*/
struct SCLIENT_XINFA
{
	const _DBC_SKILL_XINFA*		m_pDefine;		//心法表定义
	INT							m_nPosIndex;	//第几个心法 (UI同步)
	BOOL						m_bLeaned;		//是否已经学会
	INT							m_nLevel;		//心法等级
};
typedef std::map< INT, SCLIENT_XINFA >			SXINFA_MAP;

/*
	1) 技能数据结构
*/
struct SCLIENT_SKILL
{
	const _DBC_SKILL_DATA		*m_pDefine;		//技能表定义
	INT							m_nPosIndex;	//第几个技能 (UI同步)
	BOOL						m_bLeaned;		//是否已经学会
	Time_t						m_nActiveTime;  //激活时间
};
typedef std::map< INT, SCLIENT_SKILL >			SSKILL_MAP;


/*
	2) 客户端生活技能数据结构
*/
struct SCLIENT_LIFEABILITY
{
	const _DBC_LIFEABILITY_DEFINE *	m_pDefine;		//生活技能表中的定义
	INT								m_nPosIndex;	// 第几个技能 (UI同步)
	INT								m_nLevel;		// 技能等级
	INT								m_nExp;			// 技能熟练度
};
typedef std::map< INT, SCLIENT_LIFEABILITY >	SLIFEABILITY_MAP;

/*
	3) 客户端配方数据结构
*/
struct SCLIENT_PRESCR
{
	const _DBC_LIFEABILITY_ITEMCOMPOSE*		m_pDefine;		//在配方表中的定义
};
typedef std::map< INT, SCLIENT_PRESCR >			SPRESCR_MAP;

class tObject_Item;

struct SDATA_PLAYER_MYSELF : public SDATA_PLAYER_OTHER
{
	//-----------------------------------------------------
	INT				m_nHP;				// 生命点						
	INT				m_nMP;				// 魔法点						
	INT				m_nExp;				// 经验值						
	//INT			m_nMaxExp;			// 经验值上限					
	INT				m_nMoney;			// 游戏币数						

	//-----------------------------------------------------
	//一级战斗属性
	INT				m_nSTR;				// 外功							
	INT				m_nSPR;				// 内功							
	INT				m_nCON;				// 身法							
	INT				m_nINT;				// 体魄							
	INT				m_nDEX;				// 智慧							
	INT				m_nPoint_Remain;	// 剩余待分配点数				

	//-----------------------------------------------------
	//二级战斗属性
	INT				m_nAtt_Physics;		// 物理攻击力					
	INT				m_nAtt_Magic;		// 魔法攻击力					
	INT				m_nDef_Physics;		// 物理防御力					
	INT				m_nDef_Magic;		// 魔法防御力					
	INT				m_nMaxHP;			// 最大生命点					
	INT				m_nMaxMP;			// 最大魔法点					
	INT				m_nHP_ReSpeed;		// HP恢复速度  点/秒			
	INT				m_nMP_ReSpeed;		// MP恢复速度  点/秒			
	INT				m_nHit;				// 命中率						
	INT				m_nMiss;			// 闪避率						
	INT				m_nCritRate;		// 会心率						
	//FLOAT			m_fMoveSpeed;		// 移动速度(在基本属性中)					
	INT				m_nAttackSpeed;		// 攻击速度						
	INT				m_nAtt_Cold;		// 冰攻击						
	INT				m_nDef_Cold;		// 冰防御						
	INT				m_nAtt_Fire;		// 火攻击						
	INT				m_nDef_Fire;		// 火防御						
	INT				m_nAtt_Light;		// 电攻击						
	INT				m_nDef_Light;		// 电防御						
	INT				m_nAtt_Posion;		// 毒攻击						
	INT				m_nDef_Posion;		// 毒防御						

	//-----------------------------------------------------
	//其他属性
	SXINFA_MAP			m_theXinFa;			//心法
	SSKILL_MAP			m_theSkill;			//技能数据
	SLIFEABILITY_MAP	m_theLifeAbility;	//生活技能数据
	SPRESCR_MAP			m_theSprescr;		//配方数据
};
