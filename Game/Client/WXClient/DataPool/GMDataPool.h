/****************************************\
*										*
* 			   数据池					*
*										*
\****************************************/
#pragma once
#include "TDDataPool.h"
#include "GMDP_Struct_Pet.h"
#include "GameStruct_Script.h"
#include "GameStruct_Pet.h"
#include "GMDP_Struct_Guild.h"
#include "GMDP_Struct_RecordList.h"
#include "GMDP_Struct_PlayerShop.h"

class CObject;
class CObject_Character;
class CCharacterData;
class CObject_PlayerNPC;
enum HUMAN_EQUIP;
class tObject_Item;
struct _ITEM;
struct _BUFF_IMPACT_INFO;
struct X_PARAM;
struct X_SCRIPT;
struct EVENT;
class Relation;
class CMailPool;

class CDataPool : public tDataPool_
{
public:
	static void WINAPI	_Skill_OnUpdatePlayerAttrib(const EVENT* pEvent, UINT dwOwnerData);
	//---------------------------------------------
	//公共接口
	//---------------------------------------------
public:
	enum TYPE_PLAYER_SHOP
	{
		TYPE_PLAYER_SHOP_INVALID = 0,		//无效类别，还没有分配出去
		TYPE_PLAYER_SHOP_ITEM,				//物品店
		TYPE_PLAYER_SHOP_GEM,				//宝石店
		TYPE_PLAYER_SHOP_WEAPON,			//武器店
		TYPE_PLAYER_SHOP_ARMOR,				//护甲店
		TYPE_PLAYER_SHOP_MATERIAL,			//材料店
		TYPE_PLAYER_SHOP_PET,				//宠物店
	};

	//------------
	//角色属性创建/销毁
	//------------
	CCharacterData*			CharacterData_Create(CObject_Character* pCharacterData);
	void					CharacterData_Destroy(CObject_Character* pCharacterData);
	CCharacterData*			CharacterData_Get( INT nID );
	void					SetCharFirstLogin(BYTE bFirst);
	BYTE					GetCharFirstLogin() { return m_bCharFirstLogin; }


	//------------
	//冷却组
	//------------
	struct COOLDOWN_GROUP
	{
		INT		nTime;
		INT		nTotalTime;
		INT 	nTimeElapsed;
	};
	//取得冷却组
	virtual const COOLDOWN_GROUP*	CoolDownGroup_Get(INT nCoolDownID) const;
	//更新冷却组
	virtual VOID					CoolDownGroup_UpdateList(const Cooldown_T *pCoolDownList, INT nUpdateNum);
	//取得宠物冷却组
	virtual const COOLDOWN_GROUP*	PetSkillCoolDownGroup_Get(INT nCoolDownID,INT nPetNum) const;
	//更新宠物冷却组
	virtual VOID					PetSkillCoolDownGroup_UpdateList(const Cooldown_T *pCoolDownList, INT nUpdateNum,const PET_GUID_t nPet_GUID);


	//------------
	//公共冷却组
	//------------
	virtual const INT				CommonCoolDown_Get(VOID) const { return m_nCommonCoolDown; }
	virtual VOID					CommonCoolDown_Update(VOID) { m_nCommonCoolDown = COMMONCOOLDOWN_TIME;}
	
	//------------
	//任务时间
	//------------
	//取得任务时间
	virtual const INT				QuestTimeGroup_Get(INT nQuestTimeID) const;
	//设置任务时间
	virtual VOID					QuestTimeGroup_UpdateList(const INT nQuestTime, INT nUpdateNum);
	
	//------------
	//玩家装备系列
	//------------
	//清空
	virtual VOID			UserEquip_Clear(VOID);
	//设置装备
	virtual VOID			UserEquip_SetItem(HUMAN_EQUIP ptEquip, tObject_Item* pItem, BOOL bClearOld=TRUE);
	//取得装备
	virtual tObject_Item*	UserEquip_GetItem(HUMAN_EQUIP ptEquip);
	//套装是否组合成功,返回套装编号，不成功返回-1
	virtual INT				UserEquip_IsUnion(VOID);

	//------------
	//别人装备系列
	//------------
	//清空
	virtual VOID			OtherPlayerEquip_Clear(VOID);
	//设置装备
	virtual VOID			OtherPlayerEquip_SetItem(HUMAN_EQUIP ptEquip, tObject_Item* pItem, BOOL bClearOld=TRUE);
	//取得装备
	virtual tObject_Item*	OtherPlayerEquip_GetItem(HUMAN_EQUIP ptEquip);
	//套装是否组合成功,返回套装编号，不成功返回-1
	virtual INT				OtherPlayerEquip_IsUnion(VOID);

	//------------
	// 背包物品
	//------------
	//清空
	virtual VOID			UserBag_Clear(VOID);
	//设置物品
	virtual VOID			UserBag_SetItem(INT nBagIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);
	//重新设置物品属性
	virtual VOID			UserBag_SetItemInfo(INT nBagIndex, BOOL bEmpty, const _ITEM* pItem);
	//取得装备
	virtual tObject_Item*	UserBag_GetItem(INT nBagIndex);
	//通过GUID取得装备位置
	virtual INT				UserBag_GetItemIndexByGUID(__int64	 ItemGUID);
	//通过ID取得物品位置
	virtual INT				UserBag_GetItemIndexByID( INT nID );
	//请求装备详细信息
	virtual VOID			UserBag_AskExtraInfo(INT nBagIndex);
	//通过GUID取得装备的数量
	virtual INT				UserBag_CountItemByIDTable(INT	 ItemGUID);

	//------------
	// 打开的箱子
	//------------
	//清空
	virtual VOID			ItemBox_Clear(VOID);
	//设置箱子ID
	virtual VOID			ItemBox_SetID(INT id)		{ m_idItemBox = id; }
	//取得箱子ID
	virtual INT				ItemBox_GetID(VOID) const	{ return m_idItemBox; }
	//设置其中物品
	virtual VOID			ItemBox_SetItem(INT nBoxIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);
	//取得其中物品
	virtual tObject_Item*	ItemBox_GetItem(INT nBoxIndex);
	//按照ID取得物品
	virtual tObject_Item*	ItemBox_GetItem(WORD idWorld, WORD idServer, INT idSerial, INT& nIndex);
	//返回目前箱子中物品个数
	virtual INT				ItemBox_GetNumber(VOID);

	//获得一个物品在当前商店的实际收购价格
	virtual INT				ItemBox_GetCurNpcBuyPrice(INT nBagIndex);
	//------------
	// 商店
	//------------
	virtual VOID				Booth_Clear( VOID );
	virtual VOID				Booth_Sold_Clear( VOID );
	virtual VOID				Booth_SetItem(INT nBoothIndex, tObject_Item* pItem);
	virtual VOID				Booth_SetItemPrice(INT nBoothIndex, UINT nPrice);
	virtual VOID				Booth_SetSoldItem(INT nBoothIndex, tObject_Item* pItem);
	virtual VOID				Booth_SetSoldPrice(INT nSoldIndex, UINT uPrice);
	virtual VOID				Booth_SetNumber(INT nBoothNumber) { m_nBoothNumber = nBoothNumber; }
	virtual VOID				Booth_SetSoldNumber(INT nBoothNumber) { m_nBoothSoldNumber = nBoothNumber; }
	virtual tObject_Item*		Booth_GetItem(INT nBoothIndex);
	virtual UINT				Booth_GetItemPrice(INT nBoothIndex);
	virtual UINT				Booth_GetSoldPrice(INT nSoldIndex);
	virtual tObject_Item*		Booth_GetSoldItem(INT nBoothIndex);
	virtual	tObject_Item*		Booth_GetItemByID(INT IDtable);
	virtual INT					Booth_GetNumber(VOID) const { return m_nBoothNumber; }
	virtual INT					Booth_GetSoldNumber(VOID) const { return m_nBoothSoldNumber ; }
//	virtual	BOOL				Booth_IsCursorRepairing(VOID)	const {return	m_bIsRepairing;}
//	virtual	VOID				Booth_SetCursorRepairing(BOOL flag)	{m_bIsRepairing = flag;}
	virtual	BOOL				Booth_IsClose(VOID)	const {return	m_bBoothWindowIsDirty;}
	virtual VOID				Booth_Open(VOID){m_bBoothWindowIsDirty = FALSE;}
	virtual VOID				Booth_Close(VOID){m_bBoothWindowIsDirty = TRUE;}
	virtual VOID				Booth_SetBuyType(INT nBuyType){m_nBuyType = nBuyType;}
	virtual INT					Booth_GetBuyType(VOID){return m_nBuyType;}
	virtual VOID				Booth_SetRepairType(INT nRepairType){m_nRepairType = nRepairType;}
	virtual INT					Booth_GetRepairType(VOID){return m_nRepairType;}

	virtual VOID				Booth_SetRepairLevel(INT nRepairLevel){m_nRepairLevel = nRepairLevel;}
	virtual INT					Booth_GetRepairLevel(VOID){return m_nRepairLevel;}
	virtual VOID				Booth_SetBuyLevel(INT nBuyLevel){m_nBuyLevel = nBuyLevel;}
	virtual INT					Booth_GetBuyLevel(VOID){return m_nBuyLevel;}
	virtual VOID				Booth_SetRepairSpend(FLOAT nRepairSpend){m_nRepairSpend = nRepairSpend;}
	virtual FLOAT				Booth_GetRepairSpend(VOID){return m_nRepairSpend;}
	virtual VOID				Booth_SetRepairOkProb(FLOAT nRepairOkProb){m_nRepairOkProb = nRepairOkProb;}
	virtual FLOAT				Booth_GetRepairOkProb(VOID){return m_nRepairOkProb;}
	virtual VOID				Booth_SetScale(FLOAT fScale){m_fScale = fScale;}
	virtual FLOAT				Booth_GetScale(VOID){return m_fScale;}

	virtual VOID				Booth_SetShopNpcId(INT nShopNpcId){m_nShopNpcId = nShopNpcId;}
	virtual INT					Booth_GetShopNpcId(VOID){return m_nShopNpcId;}

	virtual VOID				Booth_SetShopUniqueId(UINT nShopUniqueId){m_nShopUniqueId = nShopUniqueId;}
	virtual UINT				Booth_GetShopUniqueId(VOID){return m_nShopUniqueId;}

	virtual BOOL				Booth_IsCanRepair(tObject_Item* pItem);
	virtual BOOL				Booth_IsCanBuy(tObject_Item* pItem);

	virtual VOID				Booth_SetCurrencyUnit(INT nCurrencyUnit){m_nCurrencyUnit = nCurrencyUnit;}
	virtual INT					Booth_GetCurrencyUnit(VOID){return m_nCurrencyUnit;}

	virtual VOID				Booth_SetSerialNum(INT nSerialNum){m_nSerialNum = nSerialNum;}
	virtual INT					Booth_GetSerialNum(VOID){return m_nSerialNum;}

	virtual VOID				Booth_SetbCanBuyMult(INT bBuyMulti){m_nBuyMulti = bBuyMulti;}
	virtual INT					Booth_GetbCanBuyMult(VOID){return m_nBuyMulti;}

	virtual VOID				Booth_SetCallBack(INT bCallBack){m_bCallBack = bCallBack;}
	virtual INT					Booth_GetCallBack(VOID){return m_bCallBack;}

	virtual VOID				Booth_SetShopType(INT nType){m_nShopType = nType;}
	virtual INT					Booth_GetShopType(VOID){return m_nShopType;}

	virtual INT					Booth_GetShopNpcCamp(VOID){return 1;}

	//------------------------------------------------------
	// 银行数据的访问
	//------------------------------------------------------
	virtual VOID				UserBank_Clear(VOID);
	virtual VOID				UserBank_SetItem(INT nBankIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);
	virtual tObject_Item*		UserBank_GetItem(INT nBankIndex);
	virtual	VOID				UserBank_SetBankEndIndex(INT endindex)	{m_CurBankEndIndex = endindex;}
	virtual	INT					UserBank_GetBankEndIndex(){	return m_CurBankEndIndex;}
	virtual	VOID				UserBank_SetBankMoney(INT Money){m_CurBankMoney = Money; }
	virtual	INT					UserBank_GetBankMoney(){return m_CurBankMoney;}
	virtual	VOID				UserBank_SetBankRMB(INT Money){m_CurBankRMB = Money; }
	virtual	INT					UserBank_GetBankRMB(){return m_CurBankRMB;}
	virtual	VOID				UserBank_SetItemExtraInfo(INT nBankIndex, BOOL bEmpty, const _ITEM* pItem);
	// 查询银行nIndex编号的租赁箱子是不是有空格
	virtual BOOL				UserBank_IsEmpty(INT nIndex);
	virtual VOID				UserBank_SetNpcId(INT nNpcId){m_nBankNpcID = nNpcId;}
	virtual INT					UserBank_GetNpcId(VOID){return m_nBankNpcID;}


	//------------
	// 关系系统部分
	//------------
	virtual Relation*			GetRelation() { return m_pRelation; }
	virtual CMailPool*			GetMailPool() { return m_pMailPool; }

	//------------------------------------------------------
	// 交易盒数据访问
	//------------------------------------------------------
	virtual VOID				MyExBox_Clear(VOID);
	virtual VOID				MyExBox_SetItem(INT nExBoxIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);
	virtual tObject_Item*		MyExBox_GetItem(INT nExBoxIndex);
	virtual	VOID				MyExBox_SetItemExtraInfo(INT nExBoxIndex, BOOL bEmpty, const _ITEM* pItem);
	virtual	VOID				MyExBox_SetMoney(INT Money){m_MyExchangeBox.m_Money	= Money; }
	virtual	INT					MyExBox_GetMoney(){return m_MyExchangeBox.m_Money;}
	virtual	VOID				MyExBox_SetLock(BOOL Setlock){m_MyExchangeBox.m_IsLocked	=	Setlock; }
	virtual	BOOL				MyExBox_GetLock(){return m_MyExchangeBox.m_IsLocked; }
	virtual	VOID				MyExBox_SetConfirm(BOOL SetConfirm){m_MyExchangeBox.m_CanConform	=	SetConfirm; }
	virtual	BOOL				MyExBox_GetConfirm(){return m_MyExchangeBox.m_CanConform; }
	virtual	INT					MyExBox_GetItemNum(){return (INT)m_MyExchangeBox.m_ItemList.size(); }

	INT							MyExBox_GetPetByGuid(PET_GUID_t PetGuid);
	VOID						MyExBox_SetPet(INT nIndex, SDATA_PET* pPetData, BOOL bClearOld = TRUE);
	SDATA_PET*					MyExBox_GetPet(INT nIndex);
	INT							MyExBox_GetCount(VOID);
	VOID						MyExBox_SetSkill(INT nIndex,INT nSkillIndex,const _OWN_SKILL *Skill);
	PET_SKILL*					MyExBox_GetSkill(INT nIndex,INT nSkillIndex);


	// 查询自己的交易框里头是否有空格
	virtual	BOOL				MyExBox_IsEmpty(VOID);


	virtual VOID				OtExBox_Clear(BYTE bFlagClearPet = TRUE);
	virtual VOID				OtExBox_SetItem(INT nExBoxIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);
	virtual tObject_Item*		OtExBox_GetItem(INT nExBoxIndex);
	virtual	VOID				OtExBox_SetItemExtraInfo(INT nExBoxIndex, BOOL bEmpty, const _ITEM* pItem);
	virtual	VOID				OtExBox_SetMoney(INT Money){m_OtherExchangeBox.m_Money	= Money; }
	virtual	INT					OtExBox_GetMoney(){return m_OtherExchangeBox.m_Money;}
	virtual	VOID				OtExBox_SetLock(BOOL Setlock){m_OtherExchangeBox.m_IsLocked	=	Setlock; }
	virtual	BOOL				OtExBox_GetLock(){return m_OtherExchangeBox.m_IsLocked; }
	virtual	VOID				OtExBox_SetConfirm(BOOL SetConfirm){m_OtherExchangeBox.m_CanConform	=	SetConfirm; }
	virtual	BOOL				OtExBox_GetConfirm(){return m_OtherExchangeBox.m_CanConform; }
	virtual	INT					OtExBox_GetItemNum(){return (INT)m_OtherExchangeBox.m_ItemList.size(); }

	INT							OtExBox_GetPetByGuid(PET_GUID_t PetGuid);
	VOID						OtExBox_SetPet(INT nIndex, SDATA_PET* pPetData, BOOL bClearOld = TRUE);
	SDATA_PET*					OtExBox_GetPet(INT nIndex);
	INT							OtExBox_GetCount(VOID);
	VOID						OtExBox_SetSkill(INT nIndex,INT nSkillIndex,const _OWN_SKILL *Skill);
	VOID						OtExBox_SetSkill(INT nIndex, INT nSkillIndex, PET_SKILL* pPetSkillData, BOOL bClearOld);
	PET_SKILL*					OtExBox_GetSkill(INT nIndex,INT nSkillIndex);

	virtual	VOID				SetExchangObjID(INT ObjID){m_CurExID = ObjID;}				//当前交易对方ID
	virtual	INT					GetExchangObjID()	{ return m_CurExID;}				//当前交易对方ID

	virtual	VOID				AddToAppList(INT appid);				//加入到申请人列表中
	virtual	INT					GetFromAppList();						//从申请人列表中取出一个
	virtual	BOOL				IsStillAnyAppInList();					//申请人列表是否空

/*
	//------------------------------------------------------
	// 宝石合成/镶嵌界面的数据
	//------------------------------------------------------
	virtual VOID				EnchasePage_Clear(VOID);
	virtual VOID				EnchasePage_SetItem(INT nEnchaseIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);
	virtual tObject_Item*		EnchasePage_GetItem(INT nEnchaseIndex);
*/
/*以后再加
	virtual VOID				AppendItem_Clear(VOID);					//清空附加物品的列表
	virtual VOID				AppendItem_SetItem(INT nIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);//设置附加物品的列表
	virtual tObject_Item*		AppendItem_GetItem(INT nIndex);			//取得附加物品的列表的第i个物品。
*/

	virtual VOID				MissionBox_Clear(VOID);					//清空递交任务物品的列表
	virtual VOID				MissionBox_SetItem(INT nMissionBoxIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);//设置递交任务物品的列表
	virtual tObject_Item*		MissionBox_GetItem(INT nMissionBoxIndex);//取得递交任务物品的列表

	virtual VOID				QuestLogBox_Clear(VOID);							//清空任务物品的列表
	virtual VOID				QuestLogBox_SetItem(INT nQuestIndex, INT nItemIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);//设置任务物品的列表
	virtual tObject_Item*		QuestLogBox_GetItem(INT nQuestIndex, INT nItemIndex);	//取得任务物品的列表

	VOID						Copy_To_TargetEquip(CObject* pObj){ m_pTargetEquip = pObj; }
	CObject*					GetTargetEquip(VOID) { return m_pTargetEquip;}
	//------------------------------------------------------
	// 摊位盒数据访问
	//------------------------------------------------------
	virtual VOID				MyStallBox_SetDefaultPage(INT nPage){m_MyStallBox.m_nDefaultPage = nPage;}
	virtual INT					MyStallBox_GetDefaultPage(){return m_MyStallBox.m_nDefaultPage;}
	virtual VOID				MyStallBox_SetPosTax(UINT unPosTax){m_MyStallBox.m_PosTax = unPosTax;}//设置摊位费
	virtual UINT				MyStallBox_GetPosTax(VOID){return m_MyStallBox.m_PosTax;}//获得摊位费
	virtual VOID				MyStallBox_SetTradeTax(UINT unTradeTax){m_MyStallBox.m_TradeTax = unTradeTax;}//设置交易税
	virtual UINT				MyStallBox_GetTradeTax(VOID){return m_MyStallBox.m_TradeTax;}//获得交易税

	virtual VOID				MyStallBox_Clear(VOID);					//清空自己摊位盒
	virtual VOID				MyStallBox_SetIsOpen(BOOL IsOpen){ m_MyStallBox.m_bIsOpen = IsOpen; }//设置摊主ID
	virtual BOOL				MyStallBox_GetIsOpen(){ return m_MyStallBox.m_bIsOpen; }//获得摊主ID
	virtual VOID				MyStallBox_SetObjID(UINT nObjID){ m_MyStallBox.m_ObjID = nObjID; }//设置摊主ID
	virtual UINT				MyStallBox_GetObjID(){ return m_MyStallBox.m_ObjID; }//获得摊主ID
	virtual VOID				MyStallBox_SetStallName(LPCTSTR szName) {m_MyStallBox.m_szStallName = szName;}//设置摊位名
	virtual LPCTSTR				MyStallBox_GetStallName(){ return m_MyStallBox.m_szStallName.c_str(); }//获得摊位名
	virtual VOID				MyStallBox_SetOwnerName(LPCTSTR szName) {m_MyStallBox.m_szOwnerName = szName;}//设置摊主名
	virtual LPCTSTR				MyStallBox_GetOwnerName(){ return m_MyStallBox.m_szOwnerName.c_str(); }//获得摊主名
	virtual VOID				MyStallBox_SetFirstPage(UINT nFirstPage) {m_MyStallBox.m_nFirstPage = nFirstPage;}//设置首页
	virtual UINT				MyStallBox_GetFirstPage(){ return m_MyStallBox.m_nFirstPage; }//获得首页
	virtual VOID				MyStallBox_SetItem(INT nStallBoxIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);//设置摊位中的物品
	virtual VOID				MyStallBox_SetItemPrice(INT nStallBoxIndex, UINT nPrice);//设置摊位物品价格
	virtual VOID				MyStallBox_SetItemSerial(INT nStallBoxIndex, UINT nSerial);//设置摊位物品序列号
	virtual tObject_Item*		MyStallBox_GetItem(INT nStallBoxIndex);//获得摊位中指定位置的物品
	virtual UINT				MyStallBox_GetItemPrice(INT nStallBoxIndex);//获得物品价格
	virtual UINT				MyStallBox_GetItemSerial(INT nStallBoxIndex);//获得物品序列号
	virtual	VOID				MyStallBox_SetItemExtraInfo(INT nStallBoxIndex, BOOL bEmpty, const _ITEM* pItem);//设置物品详细信息
	virtual	INT					MyStallBox_GetItemNum(){return (INT)m_MyStallBox.m_ItemList.size(); }//获得物品数量
	virtual	BOOL				MyStallBox_IsOpen(){return (INT)m_MyStallBox.m_bIsOpen; }//查询自己是否处于摆摊状态
	virtual VOID				MyStallBox_SetCurSelectItem(tObject_Item* pItem){m_CurSelectItem.pItem = pItem;}//当前选中的物品
	virtual tObject_Item*		MyStallBox_GetCurSelectItem(){return m_CurSelectItem.pItem;}//当前选中的物品
	virtual VOID				MyStallBox_SetSelectItemInPos(INT nSelectItemInPos){m_nSelectItemInPos = nSelectItemInPos;}//当前选中的物品所在的容器
	virtual INT					MyStallBox_GetSelectItemInPos(){return m_nSelectItemInPos;}//当前选中的物品所在的容器
	virtual	INT					MyStallBox_GetItemIndexByGUID(__int64 ItemGUID);//根据GUID查找物品位置
	virtual	UINT				MyStallBox_GetOwnerGUID() {return m_MyStallBox.m_OtGUID;}
	virtual	VOID				MyStallBox_SetOwnerGUID(UINT GUID){m_MyStallBox.m_OtGUID = GUID;}

	virtual INT					MyStallBox_GetnSelectPetOnStall(){return m_nSelectPetOnStall;}//当前选中的物品所在的容器
	virtual	VOID				MyStallBox_SetnSelectPetOnStall(INT nSelectPetOnStall){m_nSelectPetOnStall = nSelectPetOnStall;};//根据GUID查找物品位置
	virtual	BOOL				MyStallBox_IsHaveEmpty(INT nType);

	//摆摊宠物
	INT							MyStallBox_GetPetByGuid(PET_GUID_t PetGuid);
	VOID						MyStallBox_SetPet(INT nIndex, SDATA_PET* pPetData, BOOL bClearOld = TRUE);
	VOID						MyStallBox_SetPetPrice(INT nIndex, INT nPetPrice);
	VOID						MyStallBox_SetPetSerial(INT nIndex, INT nSerial);
	INT							MyStallBox_GetPetPrice(INT nIndex);
	INT							MyStallBox_GetPetSerial(INT nIndex);
	SDATA_PET*					MyStallBox_GetPet(INT nIndex);
	INT							MyStallBox_GetCount(VOID);
	VOID						MyStallBox_SetSkill(INT nIndex,INT nSkillIndex,const _OWN_SKILL *Skill);
	PET_SKILL*					MyStallBox_GetSkill(INT nIndex,INT nSkillIndex);
	VOID						MyStallBox_GetSelectpetGuid(PET_GUID_t &CurSelectpetGuid)
	{
								CurSelectpetGuid.m_uHighSection = m_CurSelectpetGuid.m_uHighSection;
								CurSelectpetGuid.m_uLowSection  = m_CurSelectpetGuid.m_uLowSection;
	};           
	VOID						MyStallBox_SetSelectpetGuid(PET_GUID_t CurSelectpetGuid)
	{
								m_CurSelectpetGuid.m_uHighSection = CurSelectpetGuid.m_uHighSection;
								m_CurSelectpetGuid.m_uLowSection  = CurSelectpetGuid.m_uLowSection;
	}
	virtual	BOOL				MyStallBox_IsPetOnStall(PET_GUID_t PetGuid);

	virtual	VOID				MyStallMsg_ClearUp(VOID);
	virtual	VOID				MyStallMsg_SetSerial( UINT nSerial) {m_MyStallBox.m_BBS.m_nSerial = nSerial;}
	virtual	INT					MyStallMsg_SetIDByIndex(UINT nIndex, UINT nID);
	virtual	INT					MyStallMsg_SetTimeByIndex(UINT	nIndex, BYTE nHour, BYTE nMin);
	virtual	INT					MyStallMsg_SetHasReplyByIndex(UINT	nIndex, BYTE bhasReply);
	virtual	INT					MyStallMsg_SetReTimeByIndex(UINT nIndex, BYTE nHour, BYTE nMin);
	virtual	VOID				MyStallMsg_SetTitle(LPCTSTR szMsg);
	virtual	INT					MyStallMsg_SetAuthorNameByIndex(UINT	nIndex, LPCTSTR szMsg);
	virtual	INT					MyStallMsg_SetProposedMessageByIndex(UINT	nIndex, LPCTSTR szMsg);
	virtual	INT					MyStallMsg_SetReplyMessageByIndex(UINT nIndex, LPCTSTR szMsg);
	virtual	UINT				MyStallMsg_GetSerial() {return m_MyStallBox.m_BBS.m_nSerial;}
	virtual	INT					MyStallMsg_GetIDByIndex(UINT nIndex);
	virtual	BYTE				MyStallMsg_GetHasReplyByIndex(UINT nIndex);
	virtual	LPCTSTR				MyStallMsg_GetTitle();
	virtual	LPCTSTR				MyStallMsg_GetAuthorNameByIndex(UINT nIndex);
	virtual	LPCTSTR				MyStallMsg_GetProposedMessageByIndex(UINT nIndex );
	virtual	LPCTSTR				MyStallMsg_GetReplyMessageByIndex(UINT nIndex);
	virtual	INT					MyStallMsg_GetMessageNum();
	virtual	VOID				MyStallMsg_SetMessageNum(INT nNum);
	virtual	INT					MyStallMsg_GetHourByIndex(UINT	nIndex);
	virtual	INT					MyStallMsg_GetMinByIndex(UINT	nIndex);
	virtual	INT					MyStallMsg_GetReHourByIndex(UINT	nIndex);
	virtual	INT					MyStallMsg_GetReMinByIndex(UINT	nIndex);


	virtual VOID				OtStallBox_SetDefaultPage(INT nPage){m_OtStallBox.m_nDefaultPage = nPage;}
	virtual INT					OtStallBox_GetDefaultPage(){return m_OtStallBox.m_nDefaultPage;}
	virtual VOID				OtStallBox_SetTradeTax(UINT unTradeTax){m_OtStallBox.m_TradeTax = unTradeTax;}//设置交易税
	virtual UINT				OtStallBox_GetTradeTax(VOID){return m_OtStallBox.m_TradeTax;}//获得交易税

	virtual VOID				OtStallBox_Clear(BYTE bFlagClearPet = TRUE);//清空对方摊位盒
	virtual VOID				OtStallBox_SetIsOpen(BOOL IsOpen){ m_OtStallBox.m_bIsOpen = IsOpen; }//设置摊主ID
	virtual BOOL				OtStallBox_GetIsOpen(){ return m_OtStallBox.m_bIsOpen; }//获得摊主ID
	virtual VOID				OtStallBox_SetObjID(UINT nObjID){ m_OtStallBox.m_ObjID = nObjID; }//设置摊主ID
	virtual UINT				OtStallBox_GetObjID(){ return m_OtStallBox.m_ObjID; }//获得摊主ID
	virtual VOID				OtStallBox_SetStallName(LPCTSTR szName) {m_OtStallBox.m_szStallName = szName;}//设置摊位名
	virtual LPCTSTR				OtStallBox_GetStallName(){ return m_OtStallBox.m_szStallName.c_str(); }//获得摊位名
	virtual VOID				OtStallBox_SetOwnerName(LPCTSTR szName) {m_OtStallBox.m_szOwnerName = szName;}//设置摊主名
	virtual LPCTSTR				OtStallBox_GetOwnerName(){ return m_OtStallBox.m_szOwnerName.c_str(); }//获得摊主名
	virtual VOID				OtStallBox_SetFirstPage(UINT nFirstPage) {m_OtStallBox.m_nFirstPage = nFirstPage;}//设置摊位首页
	virtual UINT				OtStallBox_GetFirstPage(){ return m_OtStallBox.m_nFirstPage; }//获得摊位首页
	virtual VOID				OtStallBox_SetItem(INT nExBoxIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);//设置摊位物品
	virtual VOID				OtStallBox_SetItemPrice(INT nStallBoxIndex, UINT nPrice);//设置物品价格
	virtual VOID				OtStallBox_SetItemSerial(INT nStallBoxIndex, UINT nSerial);//设置物品序列号
	virtual tObject_Item*		OtStallBox_GetItem(INT nExBoxIndex);//获得制定位置的物品
	virtual UINT				OtStallBox_GetItemPrice(INT nStallBoxIndex);//获得物品价格
	virtual UINT				OtStallBox_GetItemSerial(INT nStallBoxIndex);//获得物品序列号
	virtual	VOID				OtStallBox_SetItemExtraInfo(INT nExBoxIndex, BOOL bEmpty, const _ITEM* pItem);//设置物品详细信息
	virtual	INT					OtStallBox_GetItemNum(){return (INT)m_MyStallBox.m_ItemList.size(); }//获得摊位中的物品数量
	virtual	INT					OtStallBox_GetItemIndexByGUID(__int64 ItemGUID);//根据GUID查找物品位置

	virtual	UINT				OtStallBox_GetOwnerGUID() {return m_OtStallBox.m_OtGUID;}
	virtual	VOID				OtStallBox_SetOwnerGUID(UINT GUID){m_OtStallBox.m_OtGUID = GUID;}

	//摆摊宠物
	INT							OtStallBox_GetPetByGuid(PET_GUID_t PetGuid);
	VOID						OtStallBox_SetPet(INT nIndex, SDATA_PET* pPetData, BOOL bClearOld = TRUE);
	VOID						OtStallBox_SetPetPrice(INT nIndex, INT nPetPrice);
	VOID						OtStallBox_SetPetSerial(INT nIndex, INT nSerial);
	INT							OtStallBox_GetPetPrice(INT nIndex);
	INT							OtStallBox_GetPetSerial(INT nIndex);
	SDATA_PET*					OtStallBox_GetPet(INT nIndex);
	INT							OtStallBox_GetCount(VOID);
	VOID						OtStallBox_SetSkill(INT nIndex,INT nSkillIndex,const _OWN_SKILL *Skill);
	PET_SKILL*					OtStallBox_GetSkill(INT nIndex,INT nSkillIndex);

	virtual	VOID				OtStallMsg_ClearUp(VOID);
	virtual	VOID				OtStallMsg_SetSerial( UINT nSerial) {m_OtStallBox.m_BBS.m_nSerial = nSerial;}
	virtual	INT					OtStallMsg_SetIDByIndex(UINT nIndex, UINT nID);
	virtual	INT					OtStallMsg_SetTimeByIndex(UINT	nIndex, BYTE nHour, BYTE nMin);
	virtual	INT					OtStallMsg_SetHasReplyByIndex(UINT	nIndex, BYTE bhasReply);
	virtual	INT					OtStallMsg_SetReTimeByIndex(UINT nIndex, BYTE nHour, BYTE nMin);
	virtual	VOID				OtStallMsg_SetTitle(LPCTSTR szMsg);
	virtual	INT					OtStallMsg_SetAuthorNameByIndex(UINT	nIndex, LPCTSTR szMsg);
	virtual	INT					OtStallMsg_SetProposedMessageByIndex(UINT	nIndex, LPCTSTR szMsg);
	virtual	INT					OtStallMsg_SetReplyMessageByIndex(UINT nIndex, LPCTSTR szMsg);
	virtual	UINT				OtStallMsg_GetSerial() {return m_OtStallBox.m_BBS.m_nSerial;}
	virtual	INT					OtStallMsg_GetIDByIndex(UINT nIndex);
	virtual	BYTE				OtStallMsg_GetHasReplyByIndex(UINT nIndex);
	virtual	LPCTSTR				OtStallMsg_GetTitle();
	virtual	LPCTSTR				OtStallMsg_GetAuthorNameByIndex(UINT nIndex);
	virtual	LPCTSTR				OtStallMsg_GetProposedMessageByIndex(UINT nIndex );
	virtual	LPCTSTR				OtStallMsg_GetReplyMessageByIndex(UINT nIndex);
	virtual	INT					OtStallMsg_GetHourByIndex(UINT	nIndex);
	virtual	INT					OtStallMsg_GetMinByIndex(UINT	nIndex);
	virtual	INT					OtStallMsg_GetReHourByIndex(UINT	nIndex);
	virtual	INT					OtStallMsg_GetReMinByIndex(UINT	nIndex);
	virtual	INT					OtStallMsg_GetMessageNum();
	virtual	VOID				OtStallMsg_SetMessageNum(INT nNum);									   
	//------------					   
	//学习（升级）门派心法相关
	//------------
	//门派心法教师的ID
	//VOID						StudyXinfa_SetMenpaiInfo(INT idXinfaTeacher){m_idXinfaTeacher = idXinfaTeacher;}
	//INT							StudyXinfa_GetMenpaiInfo(VOID){return m_idXinfaTeacher;}
	//VOID						StudyXinfa_SetXinfaTeacherId(INT nXinfaTeacherId){m_nXinfaTeacherId = nXinfaTeacherId;}
	//INT							StudyXinfa_GetXinfaTeacherId(VOID){return m_nXinfaTeacherId;}
	//向服务器发送加入门派的事件
	VOID					SendStudySkillEvent(INT nSkillIDandLEVEL,INT NPCID,INT HaveUPLevelSkill);
	INT						MStudySkill(INT nSkillID,INT nSkillLEVEL);

	//------------
	//生活技能教师
	//------------
	INT							StudyAbility_GetID(VOID) { return m_nStudyAbility_ID; }
	VOID						StudyAbility_SetID(INT nAbilityID) { m_nStudyAbility_ID = nAbilityID; }

	INT							StudyAbility_GetNeedMoney(VOID) { return m_uStudyAbility_NeedMoney; }
	VOID						StudyAbility_SetNeedMoney(INT uNeedMoney) { m_uStudyAbility_NeedMoney = uNeedMoney; }

	INT							StudyAbility_GetNeedExp(VOID) { return m_uStudyAbility_NeedExp; }
	VOID						StudyAbility_SetNeedExp(INT uNeedExp) { m_uStudyAbility_NeedExp = uNeedExp; }

	INT							StudyAbility_GetSkillExp(VOID) { return m_uStudyAbility_SkillExp; }
	VOID						StudyAbility_SetSkillExp(INT uSkillExp) { m_uStudyAbility_SkillExp = uSkillExp; }

	INT							StudyAbility_GetLevelLimit(VOID) { return m_nStudyAbility_LevelLimit;}
	VOID						StudyAbility_SetLevelLimit(INT nStudyAbility_LevelLimit) { m_nStudyAbility_LevelLimit = nStudyAbility_LevelLimit;}

	INT							StudyAbility_GetScriptId(VOID) { return m_uStudyAbility_ScriptId;}
	VOID						StudyAbility_SetScriptId(INT uStudyAbility_ScriptId) { m_uStudyAbility_ScriptId = uStudyAbility_ScriptId;}

	INT							StudyAbility_GetNpcId(VOID) { return m_StudyAbility_NpcId;}
	VOID						StudyAbility_SetNpcId(INT StudyAbility_NpcId) { m_StudyAbility_NpcId = StudyAbility_NpcId;}

	INT							Pet_GetPetByGuid(PET_GUID_t PetGuid);
	SDATA_PET*					Pet_GetPet(INT nIndex);
	SDATA_PET*					Pet_GetPet(PET_GUID_t guidPet);
	VOID						Pet_ClearPet(INT nIndex);
	INT							Pet_GetCount(VOID);
	INT							Pet_GetPetFoodType(PET_GUID_t guidPet);											//获得宠物的食物类型
	VOID						Pet_GetPetFoodTypeMinMax(INT nFoodType, INT* idMin = NULL, INT* idMax = NULL);	//获得宠物相应食物类型得idTable限制
	tObject_Item*				Pet_GetLowestLevel_Food_From_Package(PET_GUID_t pg, INT *idxPackage = NULL);	//从背包获得喂养所需最低级的食物
	tObject_Item*				Pet_GetLowestLevel_Dome_From_Package(PET_GUID_t pg, INT *idxPackage = NULL);	//从背包获得驯养所需最低级的食物

	VOID						Pet_CopyToTarget(SDATA_PET* pSourcePet);

	SDATA_PET*					TargetPet_GetPet(VOID) { return &m_TargetPet; }
	VOID						TargetPet_Clear(VOID) { m_TargetPet.m_nIsPresent = -1; m_TargetPet.CleanUp();}
	VOID						TargetPet_SetPetModel(INT nRaceID);
	PET_SKILL*					TargetPet_GetSkill(INT nSkillIndex);
	VOID						TargetPet_SetSkill(INT nSkillIndex, const _OWN_SKILL *Skill);
	VOID						TargetPet_SetSkill(INT nSkillIndex, PET_SKILL* pPetSkillData, BOOL bClearOld = TRUE);

	VOID						PetStudySkill_SetPetModel(INT nRaceID);

	VOID						Pet_SetSkill(INT nIndex, INT nSkillIndex, const _OWN_SKILL *Skill);
	VOID						Pet_SetSkill(INT nIndex, INT nSkillIndex, PET_SKILL* pPetSkillData, BOOL bClearOld = TRUE);
	PET_SKILL*					Pet_GetSkill(INT nIndex,INT nSkillIndex);
	VOID						Pet_SetPetModel(INT nIndex);
	BOOL						Pet_SendUseSkillMessage( SkillID_t idSkill, ObjID_t idTargetObj, FLOAT fTargetX, FLOAT fTargetZ );

	//------------
	// 技能的BUFF数据
	//------------
	INT							BuffImpact_GetCount( VOID )const{ return (INT)(m_mapBuffImpact.size()); }
	const _BUFF_IMPACT_INFO*	BuffImpact_Get( INT nSN )const;
	const _BUFF_IMPACT_INFO*	BuffImpact_GetByIndex( INT nIndex )const;
	const _BUFF_IMPACT_INFO*	BuffImpact_GetByID( INT nID )const;
	BOOL						BuffImpact_Add( const _BUFF_IMPACT_INFO *pBuffImpact );
	VOID						BuffImpact_Remove( INT nSN );
	VOID						BuffImpact_RemoveByIndex( INT nIndex );
	VOID						BuffImpact_RemoveAll( VOID );
	const CHAR*					BuffImpact_GetToolTips( INT nSN )const;
	const CHAR*					BuffImpact_GetToolTipsByIndex( INT nIndex )const;
	BOOL						BuffImpact_Dispel( INT nSN );
	BOOL						BuffImpact_DispelByIndex( INT nIndex );
	INT							BuffImpact_GetTime( INT nIndex );
	INT							BuffImpact_GetTimeByIndex( INT nIndex );


	//当前需要销毁物品
	INT							DisCard_GetItemConta(){return m_CurDisCardItem.m_nContainer;}
	INT							DisCard_GetItemPos(){return m_CurDisCardItem.m_nPosition;}
	VOID						DisCard_SetItemConta(INT nContainer){m_CurDisCardItem.m_nContainer = nContainer;}
	VOID						DisCard_SetItemPos(INT nPosition){m_CurDisCardItem.m_nPosition = nPosition;}


	X_PARAM						m_X_PARAM;
	VOID						X_PARAM_Set(X_PARAM *pBuf,INT nUIIndex);
	X_PARAM*					X_PARAM_Get( VOID ){return &m_X_PARAM;};

	X_SCRIPT					m_X_SCRIPT;
	X_SCRIPT*					X_XCRIPT_Get( VOID ){return &m_X_SCRIPT;};


	//当前正在拆分的物品
	INT							Split_GetItemConta(){return m_CurSplitItem.m_nContainer;}
	INT							Split_GetItemPos(){return m_CurSplitItem.m_nPosition;}
	INT							Split_GetSum(){return m_CurSplitItem.m_nSum;}
	INT							Split_GetNumber(){return m_CurSplitItem.m_nNumber;}
	VOID						Split_SetItemConta(INT nContainer){m_CurSplitItem.m_nContainer = nContainer;}
	VOID						Split_SetItemPos(INT nPosition){m_CurSplitItem.m_nPosition = nPosition;}
	VOID						Split_SetSum(INT nSum){m_CurSplitItem.m_nSum = nSum;}
	VOID						Split_SetNumber(INT nNumber){m_CurSplitItem.m_nNumber = nNumber;}

	//好友名单和黑名单
	VOID						Relation_ClearFriend();	//清空好友列表
	VOID						Relation_ClearBlack();	//清空黑名单

	UINT						Relation_GetFriendNum()	{return m_uFriendCount;}
	UINT						Relation_GetBlackNum()	{return m_uBlackCount;}
	VOID						Relation_SetFriendNum(UINT uFriendCount){m_uFriendCount = uFriendCount;}
	VOID						Relation_SetBlackNum(UINT uBlackCount)	{m_uBlackCount = uBlackCount;}

	VOID						Relation_AddFriend(INT index, _OWN_RELATION* pRelation);	//添加一个朋友
	VOID						Relation_AddBlack(INT index, _OWN_RELATION* pRelation);		//添加一个黑名单

	VOID						Relation_DelFriend();	//删除一个朋友
	VOID						Relation_DelBlack();	//删除一个黑名单

	BOOL						Relation_GetFriend(INT index, _OWN_RELATION* pRelation);
	BOOL						Relation_GetBlack(INT index, _OWN_RELATION* pRelation);
	//------------------------------------------------------
	// 玩家商店数据访问
	//------------------------------------------------------
	BOOL						PlayerShop_GetMeIsOwner(){return m_bMeIsOwner;}
	VOID						PlayerShop_SetMeIsOwner(BOOL bOwner){m_bMeIsOwner = bOwner;}

	INT							PlayerShop_GetNpcID(){return m_nPSNpcID;}
	VOID						PlayerShop_SetNpcID(INT nPSNpcID){m_nPSNpcID = nPSNpcID;}

	INT							PlayerShop_GetShopNum(){return m_nShopNum;}
	VOID						PlayerShop_SetShopNum(INT nShopNum){m_nShopNum = nShopNum;}

	FLOAT						PlayerShop_GetCommercialFactor(){return m_CommerFactor;}
	VOID						PlayerShop_SetCommercialFactor(FLOAT fComFactor){ m_CommerFactor = fComFactor;}

	UINT						PlayerShop_GetCost(){return m_PlayerShopApplyInfo.m_Cost;}
	VOID						PlayerShop_SetCost(UINT uCost){m_PlayerShopApplyInfo.m_Cost = uCost;}

	BYTE						PlayerShop_GetType(){return m_PlayerShopApplyInfo.m_Type;}
	VOID						PlayerShop_SetType(BYTE nType){m_PlayerShopApplyInfo.m_Type = nType;}

	LPCTSTR						PlayerShop_GetShopNameByIndex(UINT uIndex);
	VOID						PlayerShop_SetShopNameByIndex(UINT uIndex, LPCTSTR pszShopName);

	LPCTSTR						PlayerShop_GetShopDescByIndex(UINT uIndex);
	VOID						PlayerShop_SetShopDescByIndex(UINT uIndex, LPCTSTR pszShopDesc);

	LPCTSTR						PlayerShop_GetShopFoundedTimeByIndex(UINT uIndex);
	VOID						PlayerShop_SetShopFoundedTimeByIndex(UINT uIndex, LPCTSTR pszTime);

	BYTE						PlayerShop_GetStallNumOpened(UINT uIndex);
	VOID						PlayerShop_SetStallNumOpened(UINT uIndex, BYTE nStallNumOpened);

	BYTE						PlayerShop_GetStallNumOnSale(UINT uIndex);
	VOID						PlayerShop_SetStallNumOnSale(UINT uIndex, BYTE nStallNumOnSale);

	_PLAYERSHOP_GUID			PlayerShop_GetIDByIndex(UINT uIndex);
	VOID						PlayerShop_SetIDByIndex(UINT uIndex, _PLAYERSHOP_GUID nId);

	BYTE						PlayerShop_GetIsFavorByIndex(UINT uIndex);
	VOID						PlayerShop_SetIsFavorByIndex(UINT uIndex, BYTE bIsFavor);

	INT							PlayerShop_GetStallEnableByIndex(BOOL bIsMine, UINT nStallIndex);
	VOID						PlayerShop_SetStallEnableByIndex(BOOL bIsMine, UINT nStallIndex, BYTE bIsEnable);

	BYTE						PlayerShop_GetTypeByIndex(UINT uIndex);
	VOID						PlayerShop_SetTypeByIndex(UINT uIndex, BYTE nType);

	LPCTSTR						PlayerShop_GetOwnerNameByIndex(UINT uIndex);
	VOID						PlayerShop_SetOwnerNameByIndex(UINT uIndex, LPCTSTR pszOwnerName);

	UINT						PlayerShop_GetOwnerGuidByIndex(UINT uIndex);
	VOID						PlayerShop_SetOwnerGuidByIndex(UINT uIndex, UINT uOwnerGuid);

	VOID						PlayerShop_CleanUp(BOOL bIsMine);//清空玩家商店

	VOID						PlayerShop_SetShopID(BOOL bIsMine, _PLAYERSHOP_GUID nShopID);//设置商店ID
	_PLAYERSHOP_GUID			PlayerShop_GetShopID(BOOL bIsMine);//获得商店ID

	VOID						PlayerShop_SetShopSerial(BOOL bIsMine, BYTE nShopSerial);//设置商店序列号
	BYTE						PlayerShop_GetShopSerial(BOOL bIsMine);//获得商店序列号

	VOID						PlayerShop_SetItem(BOOL bIsMine, INT nStallIndex, INT nBoxIndex, tObject_Item* pItem, BOOL bClearOld=TRUE);//设置摊位物品
	tObject_Item*				PlayerShop_GetItem(BOOL bIsMine, INT nStallIndex, INT nBoxIndex);//获得制定位置的物品

	VOID						PlayerShop_SetItemPrice(BOOL bIsMine, INT nStallIndex, INT nBoxIndex, UINT nPrice);//设置物品价格
	UINT						PlayerShop_GetItemPrice(BOOL bIsMine, INT nStallIndex, INT nBoxIndex);//获得物品价格

	VOID						PlayerShop_SetItemSerial(BOOL bIsMine, INT nStallIndex, INT nBoxIndex, UINT nSerial);//设置物品序列号
	UINT						PlayerShop_GetItemSerial(BOOL bIsMine, INT nStallIndex, INT nBoxIndex);//获得物品序列号

	VOID						PlayerShop_SetItemOnSale(BOOL bIsMine, INT nStallIndex, INT nBoxIndex, BYTE bIsEnable);//此物体是否上架
	BYTE						PlayerShop_GetItemOnSale(BOOL bIsMine, INT nStallIndex, INT nBoxIndex);//此物体是否上架

	VOID						PlayerShop_SetBaseMoney(BOOL bIsMine, UINT uBaseMoney);//设置商店本金
	UINT						PlayerShop_GetBaseMoney(BOOL bIsMine);//设置商店本金

	VOID						PlayerShop_SetProfitMoney(BOOL bIsMine, UINT uProfitPrice);//设置商店盈利金
	UINT						PlayerShop_GetProfitMoney(BOOL bIsMine);//设置商店盈利金

	//只有自己的店可以看到
	INT							PlayerShop_GetPartnerNum(){return m_nPartnerNum;}
	VOID						PlayerShop_SetPartnerNum(INT nPartnerNum){m_nPartnerNum = nPartnerNum;}

	VOID						PlayerShop_SetPartnerByIndex(UINT uIndex, GUID_t Guid, LPCTSTR szName);//设置指定位置的合作伙伴
	PlayerShopPartners_t* 		PlayerShop_GetPartnerByIndex(UINT uIndex);//获得指定位置的合作伙伴

	VOID						PlayerShop_SetShopType(BOOL bIsMine, INT nShopType);//设置商店类型
	UINT						PlayerShop_GetShopType(BOOL bIsMine);//设置商店类型

	VOID						PlayerShop_SetIsSaleOut(BOOL bIsMine, BOOL bIsSaleOut);//设置商店此商店是否是出售状态
	BOOL						PlayerShop_GetIsSaleOut(BOOL bIsMine);//设置商店此商店是否是出售状态

	VOID						PlayerShop_SetSaleOutPrice(BOOL bIsMine, UINT uSaleOutPrice);//设置商店此商店是否是出售状态
	UINT						PlayerShop_GetSaleOutPrice(BOOL bIsMine);//获得商店的盘出价

	VOID						PlayerShop_SetOwnerGuid(BOOL bIsMine, UINT uOwnerGuid);//设置店主guid
	UINT						PlayerShop_GetOwnerGuid(BOOL bIsMine);//获得店主guid

	VOID						PlayerShop_SetExRecNum(BOOL bIsMine, UINT uExRecNum);//设置交易记录数量
	UINT						PlayerShop_GetExRecNum(BOOL bIsMine);//获得交易记录数量

	VOID						PlayerShop_SetMaRecNum(BOOL bIsMine, UINT uMaRecNum);//设置管理记录数量
	UINT						PlayerShop_GetMaRecNum(BOOL bIsMine);//获得管理记录数量

	VOID						PlayerShop_SetOpenStallNum(BOOL bIsMine, UINT uOpenStallNum);//设置已经拥有的柜台数
	UINT						PlayerShop_GetOpenStalldNum(BOOL bIsMine);//获得已经拥有的柜台数

	VOID						PlayerShop_SetShopSubType(BOOL bIsMine, UINT uShopSubType);//设置商店的类型
	UINT						PlayerShop_GetShopSubType(BOOL bIsMine);//获得商店的类型

	VOID						PlayerShop_SetOwnerName(BOOL bIsMine, LPCTSTR pszOwnerName);//设置店主名
	LPCTSTR						PlayerShop_GetOwnerName(BOOL bIsMine);//获得店主名

	VOID						PlayerShop_SetShopName(BOOL bIsMine, LPCTSTR pszShopName);//设置商店名
	LPCTSTR						PlayerShop_GetShopName(BOOL bIsMine);//获得商店名

	VOID						PlayerShop_SetShopDesc(BOOL bIsMine, LPCTSTR pszShopDesc);//设置商店描述
	LPCTSTR						PlayerShop_GetShopDesc(BOOL bIsMine);//获得商店名

	VOID						PlayerShop_SetItemExtraInfo(BOOL bIsMine, INT nStallIndex, INT nBoxIndex, BOOL bEmpty, const _ITEM* pItem);//设置物品详细信息
	INT							PlayerShop_GetItemNum(BOOL bIsMine, INT nStallIndex);//获得摊位中的物品数量
	INT							PlayerShop_GetItemIndexByGUID(BOOL bIsMine, INT nStallIndex, __int64 ItemGUID);//根据GUID查找物品位置

	SDATA_PET*					PlayerShop_GetPet(BOOL bIsMine, INT nStallIndex, INT nIndex);

	INT							PlayerShop_GetPetByGuid(BOOL bIsMine, INT nStallIndex, PET_GUID_t PetGuid);//根据GUID在玩家商店指定柜台中查找物品
	VOID						PlayerShop_SetPet(BOOL bIsMine, INT nStallIndex, INT nIndex, SDATA_PET* pPetData, BOOL bClearOld = TRUE);//在指定柜台中设置宠物
	VOID						PlayerShop_SetPetSerial(BOOL bIsMine, INT nStallIndex, INT nIndex, INT nSerial);//设置指定柜台的格子序列号
	INT							PlayerShop_GetPetSerial(BOOL bIsMine, INT nStallIndex, INT nIndex);//获得指定柜台的格子序列号
	VOID						PlayerShop_SetPetPrice(BOOL bIsMine, INT nStallIndex, INT nIndex, INT nPetPrice);
	INT							PlayerShop_GetPetPrice(BOOL bIsMine, INT nStallIndex, INT nIndex);
	VOID						PlayerShop_SetSkill(BOOL bIsMine, INT nStallIndex, INT nIndex, INT nSkillIndex, PET_SKILL* pPetSkillData, BOOL bClearOld = TRUE);	
	VOID						PlayerShop_SetSkill(BOOL bIsMine, INT nStallIndex, INT nIndex,INT nSkillIndex, const _OWN_SKILL *Skill);
	PET_SKILL*					PlayerShop_GetSkill(BOOL bIsMine, INT nStallIndex, INT nIndex,INT nSkillIndex);

	//清除
	VOID						PlayerShop_CleanObjItem(BOOL bIsMine);

	//玩家商店上选中的物品
	VOID						PlayerShop_MyClearSelect(){m_MyPsSelectItem.m_nConIndex = -1;m_MyPsSelectItem.m_nPosition = -1;}
	VOID						PlayerShop_OtClearSelect(){m_OtPsSelectItem.m_nConIndex = -1;m_OtPsSelectItem.m_nPosition = -1;}

	INT							PlayerShop_GetMySelectConTa(){return m_MyPsSelectItem.m_nConIndex;}
	INT							PlayerShop_GetMySelectPos(){return m_MyPsSelectItem.m_nPosition;}
	INT							PlayerShop_GetOtSelectConTa(){return m_OtPsSelectItem.m_nConIndex;}
	INT							PlayerShop_GetOtSelectPos(){return m_OtPsSelectItem.m_nPosition;}

	VOID						PlayerShop_SetMySelectConTa(INT nConIndex) {m_MyPsSelectItem.m_nConIndex = nConIndex;}
	VOID						PlayerShop_SetMySelectPos(INT nPosition){m_MyPsSelectItem.m_nPosition = nPosition;}
	VOID						PlayerShop_SetOtSelectConTa(INT nConIndex) {m_OtPsSelectItem.m_nConIndex = nConIndex;}
	VOID						PlayerShop_SetOtSelectPos(INT nPosition){m_OtPsSelectItem.m_nPosition = nPosition;}

	//---------------------------------------------
	//帮派列表
	//---------------------------------------------
	VOID						Guild_ClearAllInfo();//清空所有工会信息
	GuildInfo_t*				Guild_GetInfoByIndex(INT nIndex);//通过索引获得工会信息
	VOID						Guild_SetInfoByIndex(INT nIndex, GuildInfo_t* pGuildInfo);//通过索引设置工会信息

	INT							Guild_GetInfoNum(){return m_GuildNum;}//获得工会数量
	VOID						Guild_SetInfoNum(INT iGuildNum){m_GuildNum = iGuildNum;}//设置工会数量
	
	//---------------------------------------------
	//帮派成员列表
	//---------------------------------------------
	VOID						Guild_ClearMemInfo();//清空所有帮众信息
	GuildMemberInfo_t*			Guild_GetMemInfoByIndex(INT nIndex);//通过索引获得帮众信息
	VOID						Guild_SetMemInfoByIndex(INT nIndex, GuildMemberInfo_t* pMemberInfo);//通过索引设置帮众信息

	INT							Guild_GetMemInfoNum(){return m_GuildMemNum;}//获得帮众信息
	VOID						Guild_SetMemInfoNum(INT iMemNum){m_GuildMemNum = iMemNum;}//设置帮众信息

	INT							Guild_GetMaxMemNum(){return m_GuildMaxMemNum;}//获得帮众的最大数量
	VOID						Guild_SetMaxMemNum(INT iMaxMemNum){m_GuildMaxMemNum = iMaxMemNum;}//设置帮众的最大数量

	LPCTSTR						Guild_GetDesc(){return m_GuildDesc.c_str();}//获得帮派宗旨
	VOID						Guild_SetDesc(LPCTSTR pGuildDesc){m_GuildDesc = pGuildDesc;}//设置帮派宗旨

	LPCTSTR						Guild_GetName(){return m_GuildName.c_str();}//获得帮派宗旨
	VOID						Guild_SetName(LPCTSTR pGuildName){m_GuildName = pGuildName;}//设置帮派宗旨

	BYTE						Guild_GetCurAccess(){return m_uAccess;}//获得当前人物权限
	VOID						Guild_SetCurAccess(BYTE uAccess){m_uAccess = uAccess;}//设置当前人物权限

	BYTE						Guild_GetCurPosition(){return m_uPosition;}//获得当前人物职位
	VOID						Guild_SetCurPosition(BYTE uPosition){m_uPosition = uPosition;}//设置当前人物职位


	//---------------------------------------------
	//帮派的详细信息
	//---------------------------------------------
	VOID						Guild_ClearDetailInfo();//清空帮派的详细信息
	GuildDetailInfo_t*			Guild_GetDetailInfo();//获得帮派的详细信息
	VOID						Guild_SetDetailInfo(GuildDetailInfo_t* pDetailInfo);//设置帮派的详细信息


	//---------------------------------------------
	//帮派可任命职位
	//---------------------------------------------
	VOID						Guild_ClearAppointInfo();//清空帮派可任命职位
	GuildAppointPos_t*			Guild_GetAppointInfoByIndex(INT nIndex);//获得帮派可任命职位
	VOID						Guild_SetAppointInfoByIndex( INT nIndex, GuildAppointPos_t* pDetailInfo);//设置帮派可任命职位


	//宠物门派技能学习数据访问
	VOID						PetSkillStudy_MenPaiList_AddSkill(INT skillId);
	VOID						PetSkillStudy_MenPaiList_Clear();
	PET_SKILL*					PetSkillStudy_MenPaiList_Get(INT idx);	//idx 是 m_petSkillStudyList 的索引
	PET_SKILL*					PetSkillStudy_MenPaiList_Get_BySkillId(INT sdx); //sdx 是 PET_SKILL中的 m_pDefine->m_nID


	//---------------------------------------------
	//宠物公告板
	//---------------------------------------------
	INT							PetPlacard_GetItemCount(VOID)const;
	const _PET_PLACARD_ITEM		*PetPlacard_GetItem(INT nIndex)const;
	VOID						PetPlacard_AddItem(const _PET_PLACARD_ITEM *pItem);
	VOID						PetPlacard_CleanUp();

	//---------------------------------------------
	//玩家资料
	//---------------------------------------------
	//virtual INT					PI_GetMyInfoType()	{return m_MyPrivateInfo.m_nInfoType;}
	//virtual INT					PI_GetMyAge()		{return m_MyPrivateInfo.m_nAge;}
	//virtual INT					PI_GetMySex()		{return m_MyPrivateInfo.m_nSex;}
	//virtual INT					PI_GetMyBloodType()	{return m_MyPrivateInfo.m_nBloodType;}
	//virtual INT					PI_GetMyYearAnimal(){return m_MyPrivateInfo.m_nYearAnimal;}
	//virtual INT					PI_GetMyConsella()	{return m_MyPrivateInfo.m_nConsella;}
	//virtual INT					PI_GetMyProvince()	{return m_MyPrivateInfo.m_nProvince;}

	//virtual LPCTSTR				PI_GetMyJobInfo()	{return m_MyPrivateInfo.m_szJobInfo.c_str();}
	//virtual LPCTSTR				PI_GetMySchoolInfo(){return m_MyPrivateInfo.m_szSchoolInfo.c_str();}
	//virtual LPCTSTR				PI_GetMyCity()		{return m_MyPrivateInfo.m_szCity.c_str();}
	//virtual LPCTSTR				PI_GetMyEmailInfo()	{return m_MyPrivateInfo.m_szEmailInfo.c_str();}
	//virtual LPCTSTR				PI_GetMyLuckWord()	{return m_MyPrivateInfo.m_szLuckWord.c_str();}

	//virtual INT					PI_GetOtInfoType()	{return m_OtPrivateInfo.m_nInfoType;}
	//virtual INT					PI_GetOtAge()		{return m_OtPrivateInfo.m_nAge;}
	//virtual INT					PI_GetOtSex()		{return m_OtPrivateInfo.m_nSex;}
	//virtual INT					PI_GetOtBloodType()	{return m_OtPrivateInfo.m_nBloodType;}
	//virtual INT					PI_GetOtYearAnimal(){return m_OtPrivateInfo.m_nYearAnimal;}
	//virtual INT					PI_GetOtConsella()	{return m_OtPrivateInfo.m_nConsella;}
	//virtual INT					PI_GetOtProvince()	{return m_OtPrivateInfo.m_nProvince;}

	//virtual LPCTSTR				PI_GetOtName()		{return m_OtPrivateInfo.m_szName.c_str();}
	//virtual LPCTSTR				PI_GetOtJobInfo()	{return m_OtPrivateInfo.m_szJobInfo.c_str();}
	//virtual LPCTSTR				PI_GetOtSchoolInfo(){return m_OtPrivateInfo.m_szSchoolInfo.c_str();}
	//virtual LPCTSTR				PI_GetOtCity()		{return m_OtPrivateInfo.m_szCity.c_str();}
	//virtual LPCTSTR				PI_GetOtEmailInfo()	{return m_OtPrivateInfo.m_szEmailInfo.c_str();}
	//virtual LPCTSTR				PI_GetOtLuckWord()	{return m_OtPrivateInfo.m_szLuckWord.c_str();}

	//virtual VOID				PI_SetMyInfoType(int nInfoType)		{m_MyPrivateInfo.m_nInfoType = nInfoType;}
	//virtual VOID				PI_SetMyAge(int nAge)				{m_MyPrivateInfo.m_nAge = nAge;}
	//virtual VOID				PI_SetMySex(int nSex)				{m_MyPrivateInfo.m_nSex = nSex;}
	//virtual VOID				PI_SetMyBloodType(int nBloodType)	{m_MyPrivateInfo.m_nBloodType = nBloodType;}
	//virtual VOID				PI_SetMyYearAnimal(int nYearAnimal)	{m_MyPrivateInfo.m_nYearAnimal = nYearAnimal;}
	//virtual VOID				PI_SetMyConsella(int nConsella)		{m_MyPrivateInfo.m_nConsella = nConsella;}
	//virtual VOID				PI_SetMyProvince(int nProvince)		{m_MyPrivateInfo.m_nProvince = nProvince;}

	//virtual VOID				PI_SetMyJobInfo(LPCTSTR szJobInfo)		{m_MyPrivateInfo.m_szJobInfo = szJobInfo;}
	//virtual VOID				PI_SetMySchoolInfo(LPCTSTR szSchoolInfo){m_MyPrivateInfo.m_szSchoolInfo = szSchoolInfo;}
	//virtual VOID				PI_SetMyCity(LPCTSTR szCity)			{m_MyPrivateInfo.m_szCity = szCity;}
	//virtual VOID				PI_SetMyEmailInfo(LPCTSTR szEmailInfo)	{m_MyPrivateInfo.m_szEmailInfo = szEmailInfo;}
	//virtual VOID				PI_SetMyLuckWord(LPCTSTR szLuckWord)	{m_MyPrivateInfo.m_szLuckWord = szLuckWord;}

	//virtual VOID				PI_SetOtInfoType(int nInfoType)		{m_OtPrivateInfo.m_nInfoType = nInfoType;}
	//virtual VOID				PI_SetOtAge(int nAge)				{m_OtPrivateInfo.m_nAge = nAge;}
	//virtual VOID				PI_SetOtSex(int nSex)				{m_OtPrivateInfo.m_nSex = nSex;}
	//virtual VOID				PI_SetOtBloodType(int nBloodType)	{m_OtPrivateInfo.m_nBloodType = nBloodType;}
	//virtual VOID				PI_SetOtYearAnimal(int nYearAnimal)	{m_OtPrivateInfo.m_nYearAnimal = nYearAnimal;}
	//virtual VOID				PI_SetOtConsella(int nConsella)		{m_OtPrivateInfo.m_nConsella = nConsella;}
	//virtual VOID				PI_SetOtProvince(int nProvince)		{m_OtPrivateInfo.m_nProvince = nProvince;}

	//virtual VOID				PI_SetOtName(LPCTSTR szName)			{m_OtPrivateInfo.m_szName = szName;}
	//virtual VOID				PI_SetOtJobInfo(LPCTSTR szJobInfo)		{m_OtPrivateInfo.m_szJobInfo = szJobInfo;}
	//virtual VOID				PI_SetOtSchoolInfo(LPCTSTR szSchoolInfo){m_OtPrivateInfo.m_szSchoolInfo = szSchoolInfo;}
	//virtual VOID				PI_SetOtCity(LPCTSTR szCity)			{m_OtPrivateInfo.m_szCity = szCity;}
	//virtual VOID				PI_SetOtEmailInfo(LPCTSTR szEmailInfo)	{m_OtPrivateInfo.m_szEmailInfo = szEmailInfo;}
	//virtual VOID				PI_SetOtLuckWord(LPCTSTR szLuckWord)	{m_OtPrivateInfo.m_szLuckWord = szLuckWord;}

	//virtual UINT				PI_GetMyCharGUID()	{return m_MyPrivateInfo.m_unCharGUID;}
	//virtual UINT				PI_GetOtCharGUID()	{return m_OtPrivateInfo.m_unCharGUID;}

	//virtual VOID				PI_SetMyCharGUID(UINT unCharGUID) {m_MyPrivateInfo.m_unCharGUID = unCharGUID;}
	//virtual VOID				PI_SetOtCharGUID(UINT unCharGUID) {m_OtPrivateInfo.m_unCharGUID = unCharGUID;}

	//virtual VOID				PI_SetObjId(INT nObjId){m_nObjId = nObjId;}
	//virtual INT					PI_GetObjId(){return m_nObjId;}
	
	RecordList_t*				GetRecordList(){return &m_RecordList;}


	//------------------	---------------------------
	//数据存储			
	//------------------	---------------------------
protected:					
	//------------		
	//角色属性池
	//------------
	//所有角色的逻辑数据
	std::map< INT, CCharacterData* >	m_mapAllCharacterData;

	//------------
	//冷却时间组
	//------------
	std::vector< COOLDOWN_GROUP >		m_vCoolDownGroup;

	std::vector< COOLDOWN_GROUP >		m_vPetSkillCoolDownGroup;
	//------------
	//公共冷却
	//------------
	INT									m_nCommonCoolDown;
	//------------
	//任务剩余时间组
	//------------
	std::vector< INT >					m_vQuestTimeGroup;
	//------------
	//玩家自己装备
	//------------
	//玩家装备实例数组
	std::vector< tObject_Item* >		m_vUserEquip;

	//------------
	//别人装备
	//------------
	//别人装备实例数组
	std::vector< tObject_Item* >		m_vOtherPlayerEquip;

	//------------
	//任务列表界面
	//------------
	//玩家任务的所有物品
	struct  QUESTLOG_BOX
	{
		std::vector<tObject_Item*>		m_ItemList;						//标示交易栏中自己的物品
		VOID CleanUp();
	};
	std::vector< QUESTLOG_BOX >			m_vQuestLogItem;

	//------------
	//查看对方角色属性界面
	//------------
	//对方的Obj

	CObject*							m_pTargetEquip;

	//------------
	//主角背包列表
	//------------
	//玩家背包装备实例数组
	std::vector< tObject_Item* >		m_vUserBag;
	//详细请求队列
	std::set< INT >						m_setAskBagExtra;

	//------------
	//当前打开的箱子
	//------------
	//箱子ID
	INT									m_idItemBox;
	//箱子物品实例数组
	std::vector< tObject_Item* >		m_vItemBox;


	//------------
	//当前打开的商人
	//------------
	INT									m_nBoothNumber;
	INT									m_nBoothSoldNumber;
	std::vector< tObject_Item* >		m_listBooth;
	std::vector< tObject_Item* >		m_listBoothSold;
	std::vector< UINT >					m_listSoldPrice;
	BOOL								m_bBoothWindowIsDirty;
//	BOOL								m_bIsRepairing;
	std::vector< UINT >					m_nlistPrice;
	INT									m_nRepairLevel;		// 修理等级
	INT									m_nBuyLevel;		// 收购等级
	INT									m_nRepairType;		// 修理类型
	INT									m_nBuyType;			// 商店的收购类型
	FLOAT								m_nRepairSpend;		// 修理花费
	FLOAT								m_nRepairOkProb;	// 修理成功几率
	FLOAT								m_fScale;			// 价格系数
	INT									m_nShopType;		// 商店类型

	INT									m_nCurrencyUnit;	// 
	INT									m_nSerialNum;		
	INT									m_nBuyMulti;		
	BOOL								m_bCallBack;

	// 当前Shop的NpcId
	INT									m_nShopNpcId;		// 

	// 当前Shop的NpcId
	UINT								m_nShopUniqueId;	// 

	//------------
	//当前打开的银行
	//------------
	//主角银行物品列表
	std::vector< tObject_Item* >		m_listUserBank;
	//当前银行中已经打开的位置
	INT									m_CurBankEndIndex;
	//当前银行中钱数
	INT									m_CurBankMoney;
	//当前银行元宝数
	INT									m_CurBankRMB;
	//当前银行NPC的ID
	INT									m_nBankNpcID;

	//------------
	// 关系系统部分
	//------------
	Relation*							m_pRelation;
	CMailPool*							m_pMailPool;

	//------------
	//交易盒
	//------------
	struct  CLIENT_EXCHANGE_BOX
	{
		BOOL							m_IsLocked;						//标示界面上lock选项是否钩上
		BOOL							m_CanConform;					//标示是否显示确定按钮
		UINT							m_Money;						//标示交易的金钱
		std::vector<tObject_Item*>		m_ItemList;						//标示交易栏中自己的物品
		std::vector< SDATA_PET* >		m_Petlist;						//标示交易栏中自己的宠物

		VOID CleanUp();
	};
	CLIENT_EXCHANGE_BOX					m_MyExchangeBox;				//自己的交易盒
	CLIENT_EXCHANGE_BOX					m_OtherExchangeBox;				//对方的交易盒
	INT									m_CurExID;
	std::vector< INT >					m_listApplicantsID;				//申请交易序列
	SHORT								m_pOut;							//从队列中取出的位置指针
	SHORT								m_pIn;							//加入队列的位置指针


	//宝石合成/镶嵌界面数组
//	std::vector< tObject_Item* >		m_vPageEnchase;

	// 装备上附加物品的数组，以后再加
//	std::vector< tObject_Item* >		m_vAppendItem;					// 附加的物品。

	//------------
	//任务盒
	//------------
	std::vector<tObject_Item*>			m_MissionItemList;				//递交任务物品界面

	//------------
	// BBS上的留言
	//------------
	struct MessageEntry_t
	{
		STRING	AuthorName; 
		UINT	nID;
		BYTE	nHour;
		BYTE	nMin;
		BOOL	bHasReply;
		STRING	szMessage;
		BYTE	nReHour;
		BYTE	nReMin;
		STRING	szReplyMessage;

		MessageEntry_t()
		{
			AuthorName		=	"";		
			nID				=	0;
			nHour			=	0;
			nMin			=	0;
			bHasReply		=	0;
			szMessage		=	"";
			nReHour			=	0;
			nReMin			=	0;
			szReplyMessage	=	"";
		}
	};

	struct BBS_t
	{
		BBS_t()
		{
			CleanUp();
		}
		VOID	CleanUp();

		STRING							m_szTitle;
		UINT							m_nSerial;
		INT								m_nMessageNum;
		std::vector< MessageEntry_t >	m_MessageList;
		
	};

	//------------
	//摊位盒
	//------------
	enum STALL_DEFAULT_PAGE
	{
		ITEM_PAGE = 0,
		PET_PAGE,
	};

	struct STALL_ITEM
	{
		STALL_ITEM()
		{
			pItem	= NULL;
			nPrice	= 0;
			nSerial = 0;
			nbIsEnable	=	0;
		}
		tObject_Item*					pItem;							//物品指针
		UINT							nPrice;							//价格
        UINT							nSerial;						//序列号
		BYTE							nbIsEnable;						//这个物品是否上架
	};

	struct STALL_PET
	{
		STALL_PET()
		{
			pPet	= NULL;
			nPrice	= 0;
			nSerial = 0;
			nbIsEnable	=	TRUE;
		}
		SDATA_PET*						pPet;							//宠物指针
		UINT							nPrice;							//价格
		UINT							nSerial;						//序列号
		BYTE							nbIsEnable;						//这个物品是否上架
	};

	struct CLIENT_STALL_BOX
	{
		INT								m_nDefaultPage;					//缺省的页面
		UINT							m_ObjID;
		BOOL							m_bIsOpen;						//自己摊位是否已经打开（是否处于交易状态）
		UINT							m_PosTax;						//摊位费
		UINT							m_TradeTax;						//每次交易税收
		STRING							m_szStallName;					//标示摊位名
		STRING							m_szOwnerName;					//标示摊主名
		UINT							m_nFirstPage;					//起始页
		std::vector< STALL_ITEM >	 	m_ItemList;						//标示摊位中的所有物品
		std::vector< STALL_PET >	 	m_PetList;						//标示摊位中的所有宠物
		BBS_t							m_BBS;							//摊位留言板
		UINT							m_OtGUID;
		VOID							CleanUp();		
	};

	CLIENT_STALL_BOX					m_MyStallBox;					//自己的摊位盒
	CLIENT_STALL_BOX					m_OtStallBox;					//对方的摊位盒
	STALL_ITEM							m_CurSelectItem;				//玩家选中的物品
	INT									m_nSelectItemInPos;				//选中的这个物品所在的容器
	PET_GUID_t							m_CurSelectpetGuid;				//选中等待上架的宠物的GUID
	INT									m_nSelectPetOnStall;			//在摊位上被选中的宠物编号


	//------------
	//玩家商店相关
	//------------
	PLAYERSHOP_APPLY_INFO				m_PlayerShopApplyInfo;			//申请商店信息

	std::vector< PLAYERSHOP_INFO >	 	m_PlayerShopList;				//商店列表
	INT									m_nShopNum;

	struct PLAYERSHOP_STALL_BOX											//标示摊位中的所有物品
	{
		INT								m_bIsEnable;
		std::vector< STALL_ITEM >	 	m_ItemList;
		std::vector< STALL_PET >	 	m_PetList;
		PLAYERSHOP_STALL_BOX()
		{
			m_bIsEnable = FALSE;
			STALL_ITEM	ItemInit;
			m_ItemList.resize(STALL_BOX_SIZE, ItemInit);
			STALL_PET	PetInit;
			m_PetList.resize(STALL_BOX_SIZE, PetInit);
		}
		VOID CleanUp()
		{
			m_bIsEnable = FALSE;
			for(INT i =0; i<STALL_BOX_SIZE; i++)
			{
				STALL_ITEM	ItemInit;
				m_ItemList[i] = ItemInit;

				STALL_PET	PetInit;
				m_PetList[i] = PetInit;
			}
		}
	};

	enum SHOP_TYPE
	{
		NO_SHOP = 0,
		ITEM_SHOP,
		PET_SHOP,
	};

	struct PLAYERSHOP_BOX												//标示商店中的所有柜台
	{
		std::vector< PLAYERSHOP_STALL_BOX >	m_StallList;
		UINT								m_BaseMoney;
		UINT								m_ProfitMoney;
		_PLAYERSHOP_GUID					m_ShopID;
		STRING								m_szShopName;				//标示商店名
		STRING								m_szShopDesc;				//标示商店描述
		STRING								m_szOwnerName;				//标示店主名
		UINT								m_OwnerGuid;				//店主GUID
		BYTE								m_bIsSaleOut;				//此商店已被盘出
		UINT								m_uSaleOutPrice;			//商店的盘出价
		BYTE								m_Serial;					//商店的序列号
		INT									m_nShopType;				//商店的类型
		BYTE								m_ExRecListNum;				//交易记录数量
		BYTE								m_MaRecListNum;				//管理记录数量
		BYTE								m_StallOpenNum;				//已经拥有的柜台数
		BYTE								m_ShopSubType;				//商店子类型
		std::vector< PlayerShopPartners_t > m_PartnerList;				//合伙人列表
		
		PLAYERSHOP_BOX()
		{
			m_BaseMoney		= 0;
			m_ProfitMoney	= 0;
			m_szShopName	=	"";
			m_szOwnerName	=	"";
			m_szShopDesc	=	"";
			m_OwnerGuid		=	0;
			m_bIsSaleOut	=	FALSE;
			m_uSaleOutPrice	=	0;
			m_Serial		=	0;
			m_ExRecListNum	=	0;
			m_MaRecListNum	=	0;
			m_StallOpenNum	=	0;
			PLAYERSHOP_STALL_BOX StallInit;
			m_StallList.resize(MAX_STALL_NUM_PER_SHOP, StallInit);
			PlayerShopPartners_t PartnerInit;
			m_PartnerList.resize(MAX_PARTNER_PER_SHOP, PartnerInit);
			m_nShopType		= NO_SHOP;
			m_ShopSubType	=	0;
		}
		VOID CleanUp()
		{
			m_BaseMoney		= 0;
			m_ProfitMoney	= 0;
			m_szShopName	=	"";
			m_szOwnerName	=	"";
			m_OwnerGuid		=	0;
			m_bIsSaleOut	=	FALSE;
			m_uSaleOutPrice	=	0;
			m_Serial		=	0;
			m_ExRecListNum	=	0;
			m_MaRecListNum	=	0;
			m_StallOpenNum	=	0;
			m_ShopID.Reset();
			m_nShopType		=  NO_SHOP;
			m_ShopSubType	=	0;
			for(INT i =0; i<MAX_STALL_NUM_PER_SHOP; i++)
			{
				m_StallList[i].CleanUp();
			}
			for(INT i =0; i<MAX_PARTNER_PER_SHOP; i++)
			{
				m_PartnerList[i].CleanUp();
			}
		}
	};

	PLAYERSHOP_BOX						m_MyPlayerShop;					//自己的玩家商店
	PLAYERSHOP_BOX						m_OtPlayerShop;					//其他人的商店
	FLOAT								m_CommerFactor;					//商业指数
	BOOL								m_bMeIsOwner;					//标志自己是不是店主
	INT									m_nPartnerNum;					//合作伙伴人数

	struct SELECT_ITEM
	{
		INT								m_nConIndex;
		INT								m_nPosition;
		VOID							CleanUp()
		{
			m_nConIndex = -1;
			m_nPosition = -1;
		}
	};
	SELECT_ITEM							m_MyPsSelectItem;				//自己的商店界面上选中的物品
	SELECT_ITEM							m_OtPsSelectItem;				//别人的商店界面上选中的物品

	//当前玩家商店NPC的ID
	INT									m_nPSNpcID;


	//客户端显示的帮派列表
	GuildInfo_t							m_GuildList[MAX_GUILD_SIZE];
	INT									m_GuildNum;

	//帮众列表
	GuildMemberInfo_t					m_GuildMemList[USER_ARRAY_SIZE];
	INT									m_GuildMaxMemNum;
	INT									m_GuildMemNum;
	STRING								m_GuildDesc;
	STRING								m_GuildName;
	BYTE								m_uPosition;
	BYTE								m_uAccess;

	//帮派的详细信息
	GuildDetailInfo_t					m_GuildDetailInfo;

	//可任命帮派的职位信息
	GuildAppointPos_t					m_GuildPosAvail[GUILD_POSITION_SIZE];

	// 需要销毁的物品的一个临时记忆区
	struct DISCARD_ITEM
	{
		INT								m_nContainer;
		INT								m_nPosition;
		VOID							CleanUp()
		{
			m_nContainer = -1;
			m_nPosition = -1;
		}
	};
	DISCARD_ITEM						m_CurDisCardItem;

	// 拆分信息
	struct SPLIT_ITEM
	{
		INT								m_nContainer;
		INT								m_nPosition;
		INT								m_nSum;			//总个数
		INT								m_nNumber;		//分出来的个数
		VOID							CleanUp()
		{
			m_nSum		 = 0;
			m_nNumber	 = 0;
			m_nContainer = -1;
			m_nPosition	 = -1;
		}
	};
	SPLIT_ITEM							m_CurSplitItem;

	//玩家资料
	//struct PRIVATE_INFO
	//{
	//	STRING		m_szName;				//在CleanUp中不要清除
	//	INT			m_nInfoType;			//保密程度
	//	UINT		m_unCharGUID;			//角色编号
	//	INT			m_nAge;					//年龄
	//	INT			m_nSex;					//性别
	//	STRING		m_szJobInfo;			//职业信息		[MAX_JOBINFO_LENGTH]
	//	STRING		m_szSchoolInfo;			//毕业院校		[MAX_SCHOOLINFO_LENGTH]
	//	INT			m_nBloodType;			//血型
	//	INT			m_nYearAnimal;			//生肖
	//	INT			m_nConsella;			//星座
	//	INT			m_nProvince;			//省份
	//	STRING		m_szCity;				//城市			[MAX_CITY_LENGTH]
	//	STRING		m_szEmailInfo;			//邮件			[MAX_EMAIL_LENGTH]
	//	STRING		m_szLuckWord;			//心情寄语		[MAX_LUCKWORD_LENGTH]

	//	VOID		CleanUp()
	//	{
	//		m_nInfoType		= 0;
	//		m_unCharGUID	= 0;
	//		m_nAge			= 0;
	//		m_nSex			= 0;
	//		m_szJobInfo		= "";
	//		m_szSchoolInfo	= "";
	//		m_nBloodType	= 0;
	//		m_nYearAnimal	= 0;
	//		m_nConsella		= 0;
	//		m_nProvince		= 0;
	//		m_szCity		= "";
	//		m_szEmailInfo	= "";
	//		m_szLuckWord	= "";
	//	}
	//};
	//PRIVATE_INFO	m_MyPrivateInfo;
	//PRIVATE_INFO	m_OtPrivateInfo;
	INT				m_nObjId;

	//------------
	// 交易记录，或者操作记录
	//------------
	RecordList_t						m_RecordList;

	//------------
	// 门派心法老师
	//------------
	//心法老师的心法ID
	INT									m_idXinfaTeacher;
	INT									m_nXinfaTeacherId;

	//------------
	// 生活技能老师的相关内容
	//------------
	//所教的生活技能的ID
	INT									m_nStudyAbility_ID;
	//需要花费的金钱
	UINT								m_uStudyAbility_NeedMoney;	
	//需要消耗的经验值
	UINT								m_uStudyAbility_NeedExp;	
	//技能需要达到的点数（学习中不会消耗）
	UINT								m_uStudyAbility_SkillExp;
	//学习这个生活技能需要的等级
	INT									m_nStudyAbility_LevelLimit;
	//学习这个技能的服务器脚本号
	UINT								m_uStudyAbility_ScriptId;
	//学习生活技能NpcServer ID
	UINT								m_StudyAbility_NpcId;

	std::vector< SDATA_PET >			m_listPet;

	SDATA_PET							m_TargetPet;

	//用于Pet UI显示的宠物对象
	CObject_PlayerNPC*					m_pPetAvatar;

	//用于TargetPet UI显示的宠物对象
	CObject_PlayerNPC*					m_pTargetPetAvatar;

	//用于PetStudySkill UI显示的宠物对象
	CObject_PlayerNPC*					m_pPetStudySkillAvatar;


	//------------
	// 技能的BUFF数据
	//------------
	typedef std::map< INT, _BUFF_IMPACT_INFO* >	CBuffImpactMap;
	CBuffImpactMap						m_mapBuffImpact;
	INT									m_nBuffImpactTime;



	UINT								m_uFriendCount;		// 好友数
	UINT								m_uBlackCount;		// 黑名单数
	_FRIEND_INFO						m_FriendInfo[RELATION_BLACKNAME_OFFSET - RELATION_FRIEND_OFFSET];
	_BLACKNAME_INFO						m_BlackNameInfo[MAX_RELATION_SIZE - RELATION_BLACKNAME_OFFSET];

	//------------
	// 宠物门派技能学习
	//------------
	std::vector<PET_SKILL>				m_petSkillStudyList;				//服务器传过来的技能列表，只在学习宠物门派技能的时候使用


	//------------
	// 宠物公告板
	//------------
	INT									m_nPetPlacardItemCount;								// 当前条数
	_PET_PLACARD_ITEM					m_aPetPlacardItems[MAX_PETPLACARD_LIST_ITEM_NUM];	// 宠物公告板数据

	BYTE								m_bCharFirstLogin;
public:

	//---------------------------------------------------------------------------------------------------------------------------------
	// 鉴定相关
	//----------------------------------------------------------------------------------------------------------------------------------
	// 锁定鉴定轴2006-4-15
	void Identify_Lock(INT iBagPos);

	// 取消鉴定轴锁定2006－4－15
	void Identify_UnLock();

	// 当前鉴定卷轴的位置
	INT	m_nIdentifyPosition;
	// 使用卷轴的背包索引位置
	int m_iIdentifyInBagIndex;
	//鉴定卷轴等级。
	int m_iIdentifyLevel;

	//---------------------------------------------
	//核心继承
	//---------------------------------------------
public:
	CDataPool();
	virtual ~CDataPool();
	static CDataPool*	GetMe(VOID) { return s_pMe; }

public:
	virtual VOID		Initial(VOID*);
	virtual VOID		Release(VOID);
	virtual VOID		Tick(VOID);

protected:
	static CDataPool*	s_pMe;

	WX_DECLARE_DYNAMIC( CDataPool );
};