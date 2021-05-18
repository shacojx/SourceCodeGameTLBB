/********************************************************************
	创建日期:	2005年11月2日
	创建时间:	16:54
	文件名称:	ItemRuler.h
	文件路径:	d:\Prj\Server\Server\Item\ItemRuler.h
	
	文件功能： 物品系统的规则类,比如创建规则\掉落规则\使用规则等\拾取规则
	修改纪录：
	
*********************************************************************/

#ifndef _ITEM_RULER_H_
#define _ITEM_RULER_H_


#include "ItemTable.h"
#include "Obj_ItemBox.h"
#include "ItemHelper.h"


class ItemBoxRuler
{
public:

	//ItemBox创建规则

	VOID		CreateItemFromMonsterDrop(INT iMonsterValue,
										 MONSTER_DROPBOXS& mDrop,
										 INT iPlayerLvl,
										 INT iMonsterLvl,
										 FLOAT fControlValue,
										 ItemBoxContaner&	OutBox,
										 FLOAT fWallow);





};




class ItemCreateRuler
{

public:

	static	BOOL		CreateEquipAttrib(ITEM_LOG_PARAM*	pLogParam,_ITEM_TYPE itemType,INT iPlayerLvl,_ITEM& it,INT& quality);
	static	BOOL		CreateGreenEquipAttrib(_ITEM_TYPE itemType,INT iPlayerLvl,_ITEM& it);
	static	BOOL		CreateCommonEquipAttrib(_ITEM_TYPE itemType,INT iPlayerLvl,_ITEM& it);
	static	BOOL		CreateBlueEquipAttrib(ITEM_LOG_PARAM* pLogParam,_ITEM_TYPE itemType,INT iPlayerLvl,_ITEM& it,INT& quality);
	static	BOOL		CreateGemAttrib(_ITEM_TYPE itemType,_ITEM& it);
	static	BOOL		CreateGemAttrib(UINT TableIndex,_ITEM& it);
	static	BOOL		CreateMedicAttrib(_ITEM_TYPE itemType,_ITEM& it);
	static	BOOL		CreateMaterialAttrib(_ITEM_TYPE itemType,_ITEM& it);
	static	BOOL		CreateQuestItemAttrib(_ITEM_TYPE itemType,_ITEM& it);
	static	BOOL		CreateStoreMapAttrib(_ITEM_TYPE itemType,_ITEM& it);
	static	INT			CaculateQualityIndex(double fRandValue,ITEM_QUALITY_TB* pTB);

};






/*怪物Exp分配计算类
 *	
 */
class MonsterExpCaculateRuler
{
public:

	uint				CaculateBaseExp(INT iMonsterLevel,INT iKillerLevel,uint MonsterBaseExp);
};


/*
 *		怪物物品掉落
 *		以及固定分配
 */

class Obj_Monster;
class Obj_Human;

class MonsterDropRuler
{
public:

	static	BOX_DISTRIBUTE_RULER	
						GetMonsterDropRuler(Obj_Monster*	pMonster);

	static	BOOL		CaculateCommOwnerList(Obj_Monster*	pMonster);
	static	BOOL		CaculateBossOwnerList(Obj_Monster*	pMonster);

	static	BOOL		CaculateCommDropRuler(Obj_Monster*	pMonster);

	static	BOOL		CaculateBossDropRuler(Obj_Monster*	pMonster, BOX_DISTRIBUTE_RULER DropRuler);

};

/* 物品规则检查类
 *	
 */

class ItemRulerCheck
{
public:
	/* 规则检查方法
	 *	
	 *		参数描叙    
	 *			ITEM_RULER_LIST ruler			规则编号
	 *		
	 *			const CHAR& iIndex				规则ID
	 *
	 *		返回
	 *
	 *			BOOL							0 表示没有通过  1 表示通过该规则
	 *
	 */
	static BOOL	CheckRuler(ITEM_RULER_LIST ruler,const CHAR& iRuler)
	{

		__ENTER_FUNCTION

		INT iRulerID = iRuler;
		switch(ruler)
		{
		case IRL_DISCARD:
			{
				return g_ItemTable.GetRuleValueByID(iRulerID)->CanDiscard();
			}
			break;
		case IRL_TILE:
			{
				return g_ItemTable.GetRuleValueByID(iRulerID)->CanTile();
			}
			break;
		case IRL_SHORTCUT:
			{
				return g_ItemTable.GetRuleValueByID(iRulerID)->CanShortCut();
			}
			break;
		case IRL_CANSELL:
			{
				return g_ItemTable.GetRuleValueByID(iRulerID)->CanSell();
			}
			break;
		case IRL_CANEXCHANGE:
			{
				return g_ItemTable.GetRuleValueByID(iRulerID)->CanExchange();
			}
			break;
		case IRL_CANUSE:
			{
				return	g_ItemTable.GetRuleValueByID(iRulerID)->CanUse();
			}
			break;
		case IRL_PICKBIND:
			{
				return g_ItemTable.GetRuleValueByID(iRulerID)->isPickBind();
			}
			break;
		case IRL_EQUIPBIND:
			{
				return g_ItemTable.GetRuleValueByID(iRulerID)->isEquipBind();
			}
			break;
		case IRL_UNIQUE:
			{
				return g_ItemTable.GetRuleValueByID(iRulerID)->isUnique();
			}
			break;
		default:
			{
				Assert(FALSE);
			}
			break;
		}
		return FALSE;

	__LEAVE_FUNCTION

		return FALSE ;
	
	}



	/*
	 *		检查方法
	 *		
	 *		参数描叙    
	 *			ITEM_RULER_LIST ruler			规则编号
	 *		
	 *			const CHAR& iIndex				规则ID
	 *		
	 *		返回
	 *		
	 *			BOOL							0 表示没有通过  1 表示通过该规则
	 *	
	 *	
	 *	
	 *
	 */

	static	BOOL	CheckTypeRuler(ITEM_RULER_LIST ruler,const UINT uItemIndex);
	
};







#endif