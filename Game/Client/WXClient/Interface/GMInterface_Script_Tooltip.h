/************************************\
*									*
*			Tooltips接口			*
*									*
\************************************/


namespace LuaPlus
{
	class LuaState;
	class LuaObject;
}
class CActionItem;

//
//脚本函数沙箱，这些函数应该由脚本调用
//
namespace SCRIPT_SANDBOX
{

class SuperTooltips
{
public:

	//查询Tooltip的Title
	//
	// 传入参数 
	//   
	// 传出参数
	//   0[String] - Title
	//
	INT		GetTitle(LuaPlus::LuaState* state);

	//查询Tooltip的Icon
	//
	// 传入参数 
	//   
	// 传出参数
	//   0[String] - Image(set:imageset image:imagename)
	//
	INT		GetIcon(LuaPlus::LuaState* state);

	//查询Tooltip的详细逻辑属性
	//
	// 传入参数 
	//   0[String] - AttributeName
	//   
	// 传出参数
	//   0[String] - AttributeValue
	//
	INT		GetAttributeValue(LuaPlus::LuaState* state);

	//查询Tooltip的描述说明
	//
	//
	//   
	// 传出参数
	//   0[String] - 描述字符串
	//
	INT		GetItemDesc(LuaPlus::LuaState* state);

	//查询物品定点传送参数

	INT GetItemTransferParam( LuaPlus::LuaState* state);
	INT GetItemIndex( LuaPlus::LuaState* state);
	//查询Tooltip的物品等级
	//
	//
	//   
	// 传出参数
	//   0[int] - -1: 这个物品没有这个属性
	//
	INT		GetItemLevel(LuaPlus::LuaState* state);

	//查询Tooltip的物品耐久
	//
	//
	//   
	// 传出参数两个
	//   0[int] - 当前耐久 -1: 这个物品没有这个属性
	//   1[int] - 最大耐久 -1: 这个物品没有这个属性
	//
	INT		GetItemDur(LuaPlus::LuaState* state);

	//查询Tooltip的物品的修理次数
	//
	//
	//   
	// 传出参数1个
	//   0[int] - 修理次数 -1: 这个物品没有这个属性
	//   
	//
	INT		GetItemRepairCount(LuaPlus::LuaState* state);

	//查询Tooltip的物品的绑定信息
	//
	//
	//   
	// 传出参数1个
	//   0[int] - 修理次数 -1: 这个物品没有这个属性
	//   
	//
	INT		GetItemBindInfo(LuaPlus::LuaState* state);

	//查询Tooltip的物品的卖给npc的价格
	//
	//
	//   
	// 传出参数1个
	//   0[int] - 修理次数 -1: 这个物品没有这个属性
	//   
	//
	INT		GetItemPrice(LuaPlus::LuaState* state);

	//查询Tooltip的物品的制造者
	//
	//
	//   
	// 传出参数1个
	//   0[int] - 修理次数 -1: 这个物品没有这个属性
	//   
	//
	INT		GetItemManufacturer(LuaPlus::LuaState* state);

	//查询Tooltip的物品的白色属性
	//
	//
	//   
	// 传出参数1个
	//   0[str] - "": 这个物品没有这个属性
	//   
	//
	INT		GetItemBaseWhiteAttrInfo(LuaPlus::LuaState* state);

	
	//查询Tooltip的物品的蓝色属性
	//
	//
	//   
	// 传出参数1个
	//   0[str] - "": 这个物品没有这个属性
	//   
	//
	INT		GetItemExtBlueAttrInfo(LuaPlus::LuaState* state);


	//查询Tooltip的物品的蓝色属性
	//
	//
	//   
	// 传出参数1个
	//   0[str] - "": 这个物品没有这个属性
	//   
	//
	INT		GetItemGreenAttrInfo(LuaPlus::LuaState* state);


	//查询Tooltip的物品的类型
	//
	//
	//   
	// 传出参数1个
	//   0[str] - "": 这个物品没有这个属性
	//   
	//
	INT	GetItemType(LuaPlus::LuaState* state);


	//查询Tooltip的装备的品质(白, 绿, 蓝)
	//
	//
	//   
	// 传出参数1个
	//   0[int] - 1 : 白
	//            2 : 绿
    //            3 : 蓝
	INT	GetItemEquipQuantity(LuaPlus::LuaState* state);


	//查询Tooltip的装备的类型属性
	//
	//
	//   
	// 传出参数1个
	//   0[int] - 物品类型
	INT	GetItemClassType(LuaPlus::LuaState* state);


	

	//
	// 得到最大修理次数
	// 传出参数1个
	// 0[int] - 最大修理次数
	INT	GetEquipMaxRepair(LuaPlus::LuaState* state);


	
	//查询Tooltip的装备的宝石信息
	//
	//
	//   
	// 传出参数15个
	//   0[int] - 宝石个数
	//   1[str] - 宝石1的图标字符串 "" - 表示无图标
	//	 2[str] - 宝石2的图标字符串 "" - 表示无图标
	//	 3[str] - 宝石2的图标字符串 "" - 表示无图标
	//	 4[str] - 宝石附加属性的描述
	INT	GetGemInEquipInfo(LuaPlus::LuaState* state);
	

	INT GetItem_IsProtected(LuaPlus::LuaState* state);

	//----------------------------------------------------------------------------------------------------------------------
	// 心法函数
	//
	
	// 得到心法等级
	// 传出参数1个
	// 0[int] - 当前学会的心法等级.
	//INT	GetXinfaLevel(LuaPlus::LuaState* state);


	//----------------------------------------------------------------------------------------------------------------------
	// 技能函数
	//

	// 得到技能等级
	// 传出参数1个
	// 0[int] - 当前学会的技能等级.
	INT	GetSkillLevel(LuaPlus::LuaState* state);

	// 得到是否学会技能
	// 传出参数1个
	// 0[int] - 当前学会的技能等级.
	INT	GetIsLearnedSkill(LuaPlus::LuaState* state);

	// 得到技能描述
	// 传出参数1个
	// 0[str] - 得到技能描述
	INT	GetSkillDesc(LuaPlus::LuaState* state);

	// 得到技能对应的心法等级
	//
	// 传出参数1个
	// 0[int] - 心法等级
	//INT	GetSkillXinfaLevel(LuaPlus::LuaState* state);




	//----------------------------------------------------------------------------------------------------------------------
	//
	// 宝石信息
	//

	// 得到宝石等级
	// 传出参数1个
	// 0[int] - 宝石等级
	INT	GetGemLevel(LuaPlus::LuaState* state);

	// 得到宝石属性
	// 传出参数1个
	// 0[str] - 宝石属性
	INT	GetGemArribInfo(LuaPlus::LuaState* state);


	//----------------------------------------------------------------------------------------------------------------------
	//
	// 得到物品的quality
	//

	// 传出参数1个
	// 0[int] - 药品或宝石品质
	INT	GetItemQuality(LuaPlus::LuaState* state);


	// 得到物品在表中的类型
	// 传出参数1个
	// 0[int] - 表中的类型。
	INT	GetItemTableType(LuaPlus::LuaState* state);


	
	// 得到配方需要的技能名字。
	// 传出参数1个
	// 0[int] - 表中的类型。
	INT	GetPrescrSkillName(LuaPlus::LuaState* state);

	// 得到配方需要的技能等级。
	// 传出参数1个
	// 0[int] - 表中的等级。
	INT	GetPrescrNeedSkillLevel(LuaPlus::LuaState* state);


	// 得到配方需要技能的当前等级。
	// 传出参数1个
	// 0[int] - 配方需要的当前等级。
	INT	GetPrescrNeedSkillCurLevel(LuaPlus::LuaState* state);


	// 向服务器请求item的详细信息
	// 
	// 
	INT	SendAskItemInfoMsg(LuaPlus::LuaState* state);

	
	// 得到蓝色装备是否鉴定
	// 
	// 
	INT	IsBlueEquipIdentify(LuaPlus::LuaState* state);

	//
	// npc商店是否打开。
	// 
	INT	IsNpcShopOpen(LuaPlus::LuaState* state);

	//是否存在
	INT IsPresent(LuaPlus::LuaState* state);

	//得到描述信息2006－4－48
	INT GetTypeDesc(LuaPlus::LuaState* state);

	//得到物品是否在背包中2006-5-10
	INT IsItemInPacket(LuaPlus::LuaState* state);

	//
	// 得到是否显示价格 2006-5-18
	//
	INT IsShowPrice(LuaPlus::LuaState* state);

	//
	// 得到显示价格 2006-5-18
	//
	INT GetShowPrice(LuaPlus::LuaState* state);

	//
	// 得到物品档次等级描述 2006-5-18
	//
	INT GetItemLevelDesc(LuaPlus::LuaState* state);

	// 
	// 得到定位点类型物品的参数
	INT GetAnchorValue( LuaPlus::LuaState* state);

	// 得到套装属性函数
	// 是否是套装
	INT IsItemSuit(  LuaPlus::LuaState* state );
	// 得到套装的数量
	INT GetItemSuitNumber( LuaPlus::LuaState* state );
	//// 判断自己是不是有第几件套装如果有就返回名字，没有就返回空字符串
	INT IsItemExist( LuaPlus::LuaState* state );
	// 得到第几件套装的名字,
	//INT GetSuitName( LuaPlus::LuaState* state );
	// 得到套装的属性
	INT GetSuitAtt( LuaPlus::LuaState* state );
	//
	// 得到银票的价值参数
	INT GetTwoMoneyValue(LuaPlus::LuaState* state);

	// 得到物品的显示颜色
	INT GetSysColour(LuaPlus::LuaState* state);
public:
	//由游戏逻辑设置激活中的ActionItem
	VOID	SetActionItem(CActionItem* pActionitem);
	CActionItem* GetActionItem(void) { return m_pActionItem; }

protected:

	INT			 m_nActionId;
	CActionItem* m_pActionItem;

public:
	SuperTooltips();
	virtual ~SuperTooltips();

	static SuperTooltips s_SupperTooltip;
	static LuaPlus::LuaObject* s_pMetaTable;
};




}