/****************************************\
*										*
* 			  操作管理器-物品			*
*										*
\****************************************/

#pragma once
#include "GMActionSystem.h"

class CActionItem_Item : public CActionItem
{
public:
	enum TOOLTOPS_VIEW_PRICE_TYPE
	{
		TOOLTOPS_VIEW_PRICE_NO = 0,
		TOOLTOPS_VIEW_PRICE_SELL,
		TOOLTOPS_VIEW_PRICE_REPAIR,
		TOOLTOPS_VIEW_PRICE_CALLBACK,
		TOOLTOPS_VIEW_LOOT_SILVER,				//银票类型的价格，城市商店获得的银票的价格显示
	};

	//得到操作类型
	virtual ACTION_OPTYPE	GetType(VOID) const		{ return AOT_ITEM;	}
	//类型字符串
	virtual	LPCTSTR			GetType_String(VOID);
	//得到定义ID
	virtual INT				GetDefineID(VOID);
	//得到数量
	virtual INT				GetNum(VOID);
	//得到内部数据
	virtual VOID*			GetImpl(VOID)			{ return (VOID*)GetItemImpl();	}
	//得到解释
	virtual LPCTSTR			GetDesc(VOID);
	//得到冷却状组ID
	virtual INT				GetCoolDownID(VOID);


	//----------------------------------------------------------------------------------------------------------
	//-- super tool tip 使用
	//--

	// 得到使用物品的等级
	virtual INT				GetItemLevel();
	// 得到物品的耐久
	virtual INT				GetItemDur();
	// 得到物品的最大耐久
	virtual INT				GetItemMaxDur();
	// 得到物品的修理次数
	virtual INT				GetItemRepairCount();
	// 得到物品的绑定信息 
	virtual INT				GetItemBindInfo();
	// 得到物品卖给npc的价格
	virtual INT				GetItemPrice();
	// 得到物品的制作人
	virtual LPCTSTR			GetManufacturer();
	// 得到白色属性
	virtual LPCTSTR			GetBaseWhiteAttrInfo();
	// 得到扩展蓝色属性
	virtual LPCTSTR			GetExtBlueAttrInfo();
	// 得到action所引用的item 的类型
	virtual INT				GetItemType();
	// 得到绿色属性
	LPCTSTR	GetGreenAttrInfo();
	// 得到类型描述
	virtual LPCTSTR			GetTypeDesc();
	// 是否需要在ToolTip上显示价格
	virtual INT				IsViewToolTopsPrice();
	// 获得在ToolTip上显示的价格
	virtual INT		 		GetViewToolTopsPrice();
	
	


	//得到所在容器的索引
	//	箱子中的物品	- 在箱子中的位置
	//	货架上的物品	- 货架上位置
	//
	virtual INT				GetPosIndex(VOID);
	//是否能够自动继续进行
	virtual BOOL			AutoKeepOn(VOID)		{ return FALSE; }
	//激活动作
	virtual VOID			DoAction(VOID);
	//激活动作(副操作)
	virtual VOID			DoSubAction(VOID);
	//是否有效
	virtual BOOL			IsValidate(VOID);
	//刷新
	virtual VOID			Update(tActionReference* pRef);
	//检查冷却是否结束
	virtual BOOL			CoolDownIsOver(VOID);
	//拖动结束
	virtual VOID			NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName);
	//查询逻辑属性
	virtual STRING			GetAttributeValue(LPCTSTR szAttributeName);

	//获得银行租赁箱的开始索引值
	virtual INT				GetCurBankRentBegin(VOID);


public:

	// 得到装备的属性(白, 绿, 蓝)
	INT GetEquipQuantity();

	// 得到装备上的宝石个数
	INT GetGemInEquipCount();

	// 得到装备的最大耐久
	INT GetItemMaxRepair();

	// 得到装备上的宝石的图标
	VOID GetGemIcon(INT iGemIndex, STRING& strIconName);

	// 得到装备上的宝石描述
	VOID GetGemInEquipDesc(STRING& strIconName);

	// 得到属于第几个任务
	INT	GetQuestIndex(VOID)	{return m_Quest_Number;}

	// 得到装备的鉴定信息
	INT GetEquipIdentify();



public:

	//-----------------------------------------------------------------------------------------------------------------------
	//
	// 宝石信息
	//

	// 得到宝石等级
	INT GetGemLevel();

	// 得到宝石属性信息
	LPCTSTR	GetGemArribInfo();


	//-----------------------------------------------------------------------------------------------------------------------
	//
	// 原料和药品
	//

	// 得到原料或药品的质量
	INT GetItemQuality();
	// 得到消耗品在表中的类型
	INT GetMedicineItemTableType();
	// 得到配方需要的技能名称。
	LPCTSTR GetPrescrNeedSkillName();
	// 得到配方需要的技能等级。
	int GetPrescrNeedSkillLevel();
	// 得到配方需要技能的当前等级。
	int GetPrescrNeedSkillCurLevel();

	// 这个技能是否已经学会
	bool IsPrescrStudy();
	// 得到物品档次等级描述 2006-5-18
	virtual INT GetItemLevelDesc();

	// 使用鉴定卷轴2006－3－30
	VOID UseIdentify();

	// 准备使用鉴定卷轴2006-3-30
	VOID PrepareUseIdentify();

	// 取消鉴定状态2006－4－15
	VOID CancelIdentify();

public:
	CActionItem_Item(INT nID);
	virtual ~CActionItem_Item();

	// 
	VOID	Update_Item(tObject_Item* pItem);

	// 发送请求详细数据消息
	void SendAskItemInfoMsg();

	//得到物品
	tObject_Item*			GetItemImpl(VOID);
	

protected:
	//用于物品操作时，物品ID(指向tObject_Item)
	INT		m_idItemImpl;

	//属于第几个任务
	INT		m_Quest_Number;

private:

	// 把装备从身上卸下
	VOID	UnUseEquip();
	//DoAction_Packet
	VOID	DoAction_Packet(VOID);
	//销毁物品的操作
	VOID	DestoryItem(LPCSTR szSourceName);

};
