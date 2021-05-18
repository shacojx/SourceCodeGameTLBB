#pragma once
#include "TDObjectSystem.h"
/*
|
|	游戏中的物品相关函数集	
|
*/
#define MAX_ITEM_CREATOR_NAME	13
class CObject_Item : public tObject_Item
{
	//-----------------------------------------------------
	//管理器
public:
	//从本地资源表中读取基本定义,生成一个ITEM
	static CObject_Item*	NewItem(UINT idTable);
	//释放Item(应该是Item最后一个被调用的函数)
	static VOID				DestroyItem(CObject_Item* pItem);
	//寻找物品
	static tObject_Item*	FindItem(UINT id);
	//不能在背包里使用的检查
	static BOOL				CheckUseInPackage(CObject_Item* pItem, STRING& szMsg);
	//-----------------------------------------------------
	//
public:
	//客户端ID
	virtual INT				GetID(VOID) const { return m_idClient; }
			VOID			ChangeClientID(VOID);

	//GUID
	virtual VOID				SetGUID(USHORT idWorld, USHORT idServer, UINT idSerial);
	virtual VOID				GetGUID(USHORT& idWorld, USHORT& idServer, UINT& idSerail) const;
	virtual unsigned __int64	GetGUID(VOID) const;


	//-------------------------------------------------------------------------------------------------------------------------------
	//-- super tool tip 使用
	//-- 
	// 得到玩家使用这个物品需要的等级
	virtual INT				GetNeedLevel();

	// 得到物品耐久度
	virtual INT				GetItemDur();

	// 得到物品最大耐久度
	virtual INT				GetItemMaxDur();

	// 得到物品的修理次数 
	virtual INT				GetItemRepairCount();

	// 得到物品的绑定信息 
	virtual INT				GetItemBindInfo();

	// 得到物品卖给npc的价格
	virtual INT				GetItemPrice();

	// 得到物品的制作人
	virtual LPCTSTR				GetManufacturer();

	// 得到白色属性
	virtual LPCTSTR			GetBaseWhiteAttrInfo();

	// 得到扩展蓝色属性
	LPCTSTR			GetExtBlueAttrInfo();



	//物品的类别
	virtual ITEM_CLASS		GetItemClass(VOID) const	= 0;

	// 物品重新分类以后， 需要 getitemClass ItemQuality 和GetItemType（）；才能确定。
	// 2006-3-30;
	virtual INT				GetItemTableQuality(VOID);
	virtual INT				GetItemTableType(VOID);


	//物品的名称
	virtual LPCTSTR			GetName(VOID) const			= 0;
	//物品解释
	virtual LPCTSTR			GetDesc(VOID) const			= 0;
	//图标名
	virtual LPCTSTR			GetIconName(VOID) const		= 0;
	//物品的详细编号，不是client用的index
	virtual INT				GetParticularID(VOID) const	{ return m_nParticularID; }
	//详细解释(可能需要服务器)
	virtual LPCTSTR			GetExtraDesc(VOID)			= 0;
	//设置详细解释
	virtual VOID			SetExtraInfo(const _ITEM * pItemInfo)= 0;
	//设置物品的制作者
	virtual VOID			SetManufacturer(const _ITEM * pItemInfo);
	//数量
	virtual VOID			SetNumber(INT nNum) { m_nNum = nNum; }
	virtual	INT				GetNumber(VOID) const { return m_nNum; }
	//获得物品的最大叠加数量
	virtual	INT				GetMaxOverLay(VOID) const	= 0;

	//最大数量,暂时用于有限商品得显示
	virtual VOID			SetMax(INT nNum){m_nMax = nNum;}
	virtual INT				GetMax(VOID) const{return m_nMax;}

	//归属
	virtual VOID			SetTypeOwner(ITEM_OWNER owner) { m_typeOwner = owner; }
	virtual ITEM_OWNER		GetTypeOwner(VOID) const { return m_typeOwner; }

	//所在容器的索引
	virtual INT				GetPosIndex(VOID) const { return m_nPosIndex; }
	virtual VOID			SetPosIndex(INT nPosIndex) { m_nPosIndex = nPosIndex; }

	//在资源表中的位置
	virtual INT				GetIdTable(VOID) const { return m_idTable; }
	
	//查询逻辑属性,一般用于脚本调用
	virtual STRING			GetAttributeValue(LPCTSTR szValueName);

	//克隆详细信息
	virtual VOID			Clone(const CObject_Item * pItemSource);

	//物品是否被锁定 =TURE锁定
	virtual BOOL			IsLocked(){return m_bLocked;}
	virtual VOID			SetLock(BOOL bLock){m_bLocked = bLock;}

	//物品规则验证
	virtual BOOL			Rule(INT ruletype);
	virtual STRING			RuleFailMessage(INT ruletype);

	//得到物品使用的目标类型
	virtual INT				GetItemTargetType();

	// 得到物品的类型描述2006-4-28
	virtual LPCTSTR			GetItemTableTypeDesc();


	// 物品在客户端的保存状态 2006－3－24
	enum SAVE_ITEM_STATUS
	{
		NO_MORE_INFO = 0,		// 没有详细信息。
		GET_MORE_INFO,			// 得到详细信息
	};

	//设置物品保存状态 2006－3－24
	void SetItemSaveStatus(SAVE_ITEM_STATUS nSaveStatus = NO_MORE_INFO);

	//物品是否有详细信息2006－3－30
	BOOL CObject_Item::IsItemHasDetailInfo();

	virtual BOOL			GetByProtect(){return m_bByProtect;}
	virtual VOID			SetByProtect(BOOL bByProtect){m_bByProtect = bByProtect;}
	
	// 得到物品档次描述信息
	virtual INT	GetItemLevelDesc(){	return 1;}

	virtual _ITEM* GetItemExtraInfoItem() const{return m_pExtraInfoItem;}

	virtual INT	GetItemTransferString(STRING& strResult) const;
	virtual VOID SetExtraInfoFromString(LPCTSTR strIn, INT nLen);
	virtual LPCTSTR			GetSysColour(){return ConvertSysColour(-1);}		//默认返回白色
	virtual INT				GetNeedJob(VOID) const	{ return -1; }
protected:
	LPCTSTR ConvertSysColour(INT nColor);
public:
	// 为物品传输系统使用的字符串转换函数 [8/7/2006]
	static BOOL	Bin2Str_Transfer(const CHAR* pIn,UINT InLength,CHAR* pOut);
	static BOOL	Str2Bin_Transfer(const CHAR* pIn,UINT InLength,CHAR* pOut,UINT OutLimit,UINT& OutLength);
protected:
	INT			m_idClient;		//客户端ID;

	ITEM_OWNER		m_typeOwner;	//归属的类型
	INT				m_idOwner;		//所属的Object的ID，有可能是玩家的ServerID，或者是宝箱的ServerID
	INT				m_nPosIndex;	//所在的位置编号，针对处于包裹，箱子，窗口等需要用索引定位的情况
	INT				m_nNum;			//数量
	INT				m_nMax;			//最大数量暂时用于有限商品的显示

	SAVE_ITEM_STATUS	m_nItemSaveStatus;	//客户端物品的保存状态。 
											//
											// 0 ： 无详细信息
											// 1 ： 得到详细信息。 


	//-----------------------------------------------------
	//在资源表中的定义ID
	// 20000000 - 49999999		普通道具：包括材料、药品、卷轴、任务物品等
	// 50000000 - 59999999		宝石
	// 60000000 - 69999999		藏宝图
	// 10200000 - 10299999		蓝色装备
	// 10400000 - 10499999		绿色装备
	// 10100000 - 10199999		白色装备
	INT			m_idTable;			

	//物品的详细编号 = ((nClass*100+nQuality)*100+nType)*1000+nIndex
	INT			m_nParticularID;

	//该物品是否正在被某个操作锁定，只影响客户端的操作，这个属性不会直接映射给服务器
	BOOL		m_bLocked;

	//是否二级密码保护
	BOOL		m_bByProtect;	

	//制作者名称
	CHAR		m_Creator[MAX_ITEM_CREATOR_NAME];

	//额外信息，用来生成字符串使用
	_ITEM*		m_pExtraInfoItem;

	//物品规则
public:
	struct Rule_t
	{
		BOOL		bCanDrop;		// 是否可丢弃
		BOOL		bCanOverLay;	// 是否可重叠
		BOOL		bCanQuick;		// 是否可放入快捷栏
		BOOL		bCanSale;		// 是否可以出售给NPC商店
		BOOL		bCanExchange;	// 是否可以交易
		BOOL		bCanUse;		// 是否可以使用
		BOOL		bPickBind;		// 是否是拾取邦定
		BOOL		bEquipBind;		// 是否是装备邦定
		BOOL		bUnique;		// 是否是唯一

		Rule_t()
		{
			bCanDrop	=	TRUE;		
			bCanOverLay	=	TRUE;		
			bCanQuick	=	TRUE;		
			bCanSale	=	TRUE;		
			bCanExchange=	TRUE;	
			bCanUse		=	TRUE;	
			bPickBind	=	TRUE;		
			bEquipBind	=	TRUE;		
			bUnique		=	TRUE;		
		}
	};

	Rule_t			m_Rule;

protected:
	//产生唯一物品ID
	static UINT	_CreateID(VOID);
	static std::map< UINT, CObject_Item* > s_mapAllItem;

protected:
	CObject_Item(UINT id);
	virtual ~CObject_Item();
};

//需要在ui上显示的属性
extern const char*  ITEMATTRIBUTE_NAME;				//物品名
extern const char*  ITEMATTRIBUTE_LEVEL;			//物品等级
extern const char*  ITEMATTRIBUTE_DAM;				//损坏度
extern const char*  ITEMATTRIBUTE_DAMMAX;			//损坏度最大值
extern const char*  ITEMATTRIBUTE_PRICE;			//价格
extern const char*  ITEMATTRIBUTE_REPAIRNUM;		//修理失败次数
extern const char*  ITEMATTRIBUTE_WHITE_ATT;		//所有白色属性
extern const char*  ITEMATTRIBUTE_BLUE_ATT;			//所有蓝色属性
extern const char*  ITEMATTRIBUTE_GREEN_ATT;		//所有绿色属性
extern const char*  ITEMATTRIBUTE_PRODUCER;			//作者
extern const char*  ITEMATTRIBUTE_DESC;				//详细解释
