/****************************************\
*										*
* 			   数据池数据结构			*
*					-角色数据			*
*										*
\****************************************/
#pragma once

//========================================================
//
//			角色对象中完全由服务器决定的属性数据
//
//========================================================
#include "GameDefine.h"
#include "GameDefine2.h"
#include "TDDBC_Struct.h"
#include "DB_Struct.h"
#include "GameStruct_Skill.h"

#include "GMDP_Struct_Skill.h"
#include "GMDP_Struct_Title.h"
#include "GMDP_Struct_Pet.h"

//=================================================================
//-->0. Character 基本属性
//=================================================================
struct SDATA_CHARACTER
{
	INT				m_nRaceID;					// 数据表中的ID						

	INT				m_nPortraitID;				// 头像ID		
	STRING			m_strName;					// 角色的名字，从服务器传来			
	STRING			m_strTitle;					//当前称号
	BYTE			m_TitleType;				//当前称号类型
	FLOAT			m_fHPPercent;				// 生命值百分比						
	FLOAT			m_fMPPercent;				// 魔法值百分比						
	INT				m_nRage;					// 怒气
	FLOAT			m_fMoveSpeed;				// 移动的速度						
	_CAMP_DATA		m_CampData;					// 阵营ID	
	INT				m_nOwnerID;					// 所有者ID
	GUID_t			m_OccupantGUID;				// 占有者(是谁打的，死了算谁的)
	INT				m_nRelative;				// 相对关系，这个人是否可被攻击，是否主动攻击
	INT				m_nModelID;					// 模型ID
	INT				m_nMountID;					// 坐骑ID
	INT				m_nLevel;					// 等级								
	BOOL			m_bFightState;				// 战斗状态							
	INT				m_nStealthLevel;			// 隐身级别
	BOOL			m_bSit;						// 坐下状态
	INT				m_nAIType;					// AI类型
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
struct SDATA_PLAYER_OTHER : public SDATA_NPC
{
	INT				m_nMenPai;					// 门派ID									
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
	INT			m_nHairMeshID;					//-> DBC_CHARACTER_HAIR_GEO					
	//脸部外形
	INT			m_nFaceMeshID;					//-> DBC_CHARACTER_HEAD_GEO					
	UINT		m_uHairColor;					//头发颜色
	INT			m_nEquipVer;					//角色的装备状态版本号，用于和服务器同步数据的依据		
	INT			m_nEquipmentID[HEQUIP_NUMBER];	//装备表;用于外形显示				
	INT			m_nEquipmentGemID[HEQUIP_NUMBER];	//装备宝石表;用于外形显示				

	BOOL		m_bTeamFlag;					// 是否有队伍
	BOOL		m_bTeamLeaderFlag;				// 是否是队长
	BOOL		m_bTeamFullFlag;				// 是否是满队
	BOOL		m_bTeamFollowFlag;				// 是否组队跟随
	INT			m_nTeamFollowListCount;			// 跟随列表里的人数
	ObjID_t		m_aTeamFollowList[MAX_TEAM_MEMBER];	// 跟随列表
	
	INT			m_nTitleNum;
	INT			m_nCurTitleIndex;
	_TITLE_		m_nTitleList[MAX_TITLE_SIZE];	//称号表
	BOOL		m_bIsInStall;					//是否摆摊中
	STRING		m_strStallName;					//摊位名			
};


//=================================================================
//-->3. Character_PlayerMySelf 对于玩家自己
//=================================================================

class tObject_Item;
struct SDATA_PLAYER_MYSELF : public SDATA_PLAYER_OTHER
{
	//-----------------------------------------------------
	INT				m_nHP;				// 生命点						
	INT				m_nMP;				// 魔法点						
	INT				m_nExp;				// 经验值						
	//INT			m_nMaxExp;			// 经验值上限(根据等级查表获得)					
	INT				m_nMoney;			// 游戏币数	
	INT				m_nStrikePoint;		// 连技点
	INT				m_nRMB;				//m_nVigor;			// 活力值
	//INT				m_nMaxVigor;		// 活力值上限
	INT				m_nDoubleExpTime_Num;			// 双倍经验时间和倍数
	//INT				m_nMaxEnergy;		// 精力值上限
	//INT				m_nGoodBadValue;	// 善恶值
	PET_GUID_t		m_guidCurrentPet;	// 当前宠物GUID

	//-----------------------------------------------------
	INT				m_nGuild;			// 帮派ID 

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
	INT				m_nAtt_Physics;			// 物理攻击力					
	INT				m_nAtt_Magic;			// 魔法攻击力					
	INT				m_nDef_Physics;			// 物理防御力					
	INT				m_nDef_Magic;			// 魔法防御力					
	INT				m_nMaxHP;				// 最大生命点					
	INT				m_nMaxMP;				// 最大魔法点					
	INT				m_nHP_ReSpeed;			// HP恢复速度  点/秒			
	INT				m_nMP_ReSpeed;			// MP恢复速度  点/秒			
	INT				m_nHit;					// 命中率						
	INT				m_nMiss;				// 闪避率						
	INT				m_nCritRate;			// 会心率						
	//FLOAT			m_fMoveSpeed;			// 移动速度(在基本属性中)					
	INT				m_nAttackSpeed;			// 攻击速度						
	INT				m_nAtt_Cold;			// 冰攻击						
	INT				m_nDef_Cold;			// 冰防御						
	INT				m_nAtt_Fire;			// 火攻击
	INT				m_nDef_Fire;			// 火防御
	INT				m_nAtt_Light;			// 电攻击
	INT				m_nDef_Light;			// 电防御
	INT				m_nAtt_Posion;			// 毒攻击
	INT				m_nDef_Posion;			// 毒防御

	BOOL			m_bLimitMove;			//是否限制不能移动
	BOOL			m_bCanActionFlag1;		//技能受限标记1,用于昏迷催眠
	BOOL			m_bCanActionFlag2;		//技能受限标记2,用于沉默
	//BOOL			m_bLimitUseSkill;		//是否限制不能施法
	//BOOL			m_bLimitHandle;			//是否限制不能进行一切操作

	BOOL			m_bIsMinorPwdSetup;		// 是否已设置二级密码
	BOOL			m_bIsMinorPwdUnlocked;	// 是否已经解锁二级密码

	//-----------------------------------------------------
	//其他属性
	SKILLCLASS_MAP		m_theSkillClass;	//技能系
	SSKILL_MAP			m_theSkill;			//技能数据
	SLIFEABILITY_MAP	m_theLifeAbility;	//生活技能数据
	SPRESCR_MAP			m_theSprescr;		//配方数据

	//宠物数据
//	SDATA_PET			m_thePet;			//宠物
};
