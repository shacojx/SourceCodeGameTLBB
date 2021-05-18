#pragma once
#include "TDNode.h"
/*
数据池，用来存储所有的外界需要知道的数据
*/

class tObject_Item;
struct _ITEM;
struct SCommand_DPC;
struct SClientSkill;
struct SClientLifeAbility;
//struct SClientXinFa;
enum HUMAN_EQUIP;
class tDataPool : public tNode
{
public:
	//--------------------------------------------------
	//角色属性
	//生活技能列表
	//virtual SClientLifeAbility*	UserData_GetLifeAbility(INT idAbility)			= 0;
	//心法列表
	//virtual SClientXinFa*		UserData_GetXinFa(INT idXinFa)					= 0;
	//技能冷却信息
//	virtual BOOL				UserData_GetSkillCoollInfo(INT nSkillID, INT& nCoolDownTime, INT& nCoolTimeNow) = 0;
/*
	//------------------------------------------------------
	//宝箱
	virtual VOID				ItemBox_Clear(VOID)			= 0;
	virtual VOID				ItemBox_SetID(INT id)		= 0;
	virtual INT					ItemBox_GetID(VOID)	const	= 0;
	virtual VOID				ItemBox_SetItem(INT nBoxIndex, tObject_Item* pItem, BOOL bClearOld=TRUE) = 0;
	virtual tObject_Item*		ItemBox_GetItem(INT nBoxIndex) = 0;
	virtual tObject_Item*		ItemBox_GetItem(WORD idWorld, WORD idServer, INT idSerial, INT& nIndex) = 0;
	
	//------------------------------------------------------
	// 主角装备
	virtual VOID				UserEquip_SetItem(HUMAN_EQUIP ptEquip, tObject_Item* pItem, BOOL bClearOld=TRUE)	= 0;
	virtual tObject_Item*		UserEquip_GetItem(HUMAN_EQUIP ptEquip)  = 0;

	//------------------------------------------------------
	// 背包物品
	virtual VOID				UserBag_SetItem(INT nBagIndex, tObject_Item* pItem, BOOL bClearOld=TRUE)	= 0;
	virtual tObject_Item*		UserBag_GetItem(INT nBagIndex) 	= 0;
	virtual VOID				UserBag_AskExtraInfo(INT nBagIndex)					= 0;
	virtual VOID				UserBag_SetItemExtraInfo(INT nBagIndex, BOOL bEmpty, const _ITEM* pItem)= 0;
	
	*/

	//------------------------------------------------------
	// 银行数据的访问
	//virtual VOID				UserBank_Clear(VOID)	=	0;
	//virtual VOID				UserBank_SetItem(INT nBankIndex, tObject_Item* pItem, BOOL bClearOld=TRUE)	=	0;
	//virtual tObject_Item*		UserBank_GetItem(INT nBankIndex)		=	0;
	//virtual	VOID				UserBank_SetBankEndIndex(INT endindex)	=	0;
	//virtual	INT					UserBank_GetBankEndIndex()	=	0;
	//virtual	VOID				UserBank_SetBankMoney(INT Money)	=	0;
	//virtual	INT					UserBank_GetBankMoney()	=	0;
	//virtual	VOID				UserBank_SetItemExtraInfo(INT nBankIndex, BOOL bEmpty, const _ITEM* pItem)	=	0;
	WX_DECLARE_DYNAMIC( tDataPool );
};
