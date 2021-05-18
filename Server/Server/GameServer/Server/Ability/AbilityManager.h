/********************************************************************************
 *	文件名：	AbilityManager.h
 *	全路径：	d:\Prj\Server\Server\Ability\AbilityManager.h
 *	创建时间：	
 *
 *	功能说明：	用于管理所有生活技能实例、配方表以及经验表
 *	修改记录：	2005 年 11 月 6 日	16:52，调整了一下结构
*********************************************************************************/

#ifndef __ABILITYMANAGER_H__
#define __ABILITYMANAGER_H__

#include "Type.h"
#include "GameDefine.h"

#define MAX_ABILITY						MAX_CHAR_ABILITY_NUM

class Ability;
class Player;
struct _AbilityExpTable;
struct _ABILITY_PRESCRIPTION_TBL;

class AbilityManager
{
public:
	AbilityManager();
	~AbilityManager();

	enum AbilityClass
	{
		ABILITY_CLASS_INVALID			= -1,	// 非法
		ABILITY_CLASS_NOUSE				= 0,	// 占位技能
		ABILITY_CLASS_COOKING,					// 烹饪
		ABILITY_CLASS_PHARMACY,					// 制药
		ABILITY_CLASS_INLAY,					// 镶嵌
		ABILITY_CLASS_FOUNDRY,					// 铸造
		ABILITY_CLASS_TAILOR,					// 缝纫
		ABILITY_CLASS_ARTWORK,					// 工艺
		ABILITY_CLASS_GATHERMINE,				// 采矿
		ABILITY_CLASS_GATHERMEDIC,				// 采药
		ABILITY_CLASS_FISH,						// 钓鱼
		ABILITY_CLASS_PLANT,					// 种植
		ABILITY_CLASS_SHAOLINDRUG,				// 少林制药（开光）
		ABILITY_CLASS_HOLYFIRE,					// 明教（圣火术）
		ABILITY_CLASS_BREWING,					// 丐帮（酿酒）
		ABILITY_CLASS_THICKICE,					// 天山（玄冰术）
		ABILITY_CLASS_INSECTCULTURING,			// 大理（制蛊）
		ABILITY_CLASS_POISON,					// 星宿（制毒）
		ABILITY_CLASS_INCANTATION,				// 峨嵋（制符）
		ABILITY_CLASS_ALCHEMY,					// 武当（炼丹）
		ABILITY_CLASS_THAUMATURGY,				// 逍遥（奇门遁甲）
		ABILITY_CLASS_ENGINEERING,				// 工程学
		ABILITY_CLASS_FIGHTGHOST,				// 驱鬼
		ABILITY_CLASS_SEARCHTSTORE,				// 挖宝
		ABILITY_CLASS_TRADE,					// 跑商
		ABILITY_CLASS_HAGGLE,					// 杀价
		ABILITY_CLASS_EXCESSPROFIT,				// 热卖
		ABILITY_CLASS_PROCESSING,				// 处理用到的进度条
		ABILITY_CLASS_PHARMACOLOGY,				// 药理
		ABILITY_CLASS_REGIMEN,					// 养生法
		ABILITY_CLASS_BUDDHOLOGY,				// 佛法
		ABILITY_CLASS_FIREMAKING,				// 采火术
		ABILITY_CLASS_BEGSKILL,					// 莲花落
		ABILITY_CLASS_ICEMAKING,				// 采冰术
		ABILITY_CLASS_VENATIONFORMULA,			// 经脉百诀
		ABILITY_CLASS_INSECTENTICING,			// 引虫术
		ABILITY_CLASS_MENTALTELEPATHY,			// 灵心术
		ABILITY_CLASS_TAOISM,					// 道法
		ABILITY_CLASS_BODYBUILDING,				// 六艺风骨

		ABILITY_CLASS_SIZE,
	};

	VOID								CleanUp();

	BOOL								Init();

	// 获得
	Ability*							GetAbility( AbilityID_t aid )
	{
		INT idx;
		idx = (INT)aid;

		if( idx<=ABILITY_CLASS_INVALID || idx>=ABILITY_CLASS_SIZE )
		{
			Assert(idx>ABILITY_CLASS_INVALID && idx<ABILITY_CLASS_SIZE);
			return NULL;
		}

		return m_pAbility[idx];
	}

	BYTE								MaxAbilityNum() { return (BYTE)ABILITY_CLASS_SIZE; }

	BOOL								InitAbility( const CHAR* filename );
	BOOL								InitPrescriptionList( const CHAR* filename );
	BOOL								InitAbilityExpTable( const CHAR* filename );

	_AbilityExpTable*					GetAbilityExpTbl()
	{
		return m_pAbilityExpTbl;
	}

	INT									GetAbilityExpTblRowCount()
	{
		return m_AbilityExpTblCount;
	}

	const _ABILITY_PRESCRIPTION_TBL*	GetPrescriptionList( )
	{
		return m_pPrescriptionList;
	}
private:
	Ability*							m_pAbility[MAX_ABILITY];
	_AbilityExpTable*					m_pAbilityExpTbl;
	INT									m_AbilityExpTblCount;
	_ABILITY_PRESCRIPTION_TBL*			m_pPrescriptionList;
};

extern AbilityManager*					g_pAbilityManager;

#endif // __ABILITYMANAGER_H__
