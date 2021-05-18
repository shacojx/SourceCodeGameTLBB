#pragma once
#include "Obj_Item.h"
#include "GameDefine2.h"
#include "Obj_Item_Equip.h"
#include "TDDBC_Struct.h"
#include "../ObjectDef.h"
#include "GameStruct_Item.h"

/*
|
|	装备类物品
|
*/
class CObject_Item_Equip : public CObject_Item
{
public:
	//得到物品的类别
	virtual ITEM_CLASS		GetItemClass(VOID) const;
	//物品的名称
	virtual LPCTSTR			GetName(VOID) const;
	//物品解释
	virtual LPCTSTR			GetDesc(VOID) const;
	//图标名
	virtual LPCTSTR			GetIconName(VOID) const;
	//详细解释(可能需要服务器)
	virtual LPCTSTR			GetExtraDesc(VOID);
	//设置详细解释
	virtual VOID			SetExtraInfo(const _ITEM * pItemInfo);
	//得到装备点
	HUMAN_EQUIP				GetItemType(VOID) const;
	//得到显示数据ID
	virtual INT				GetVisualID(VOID)	const;
	//查询逻辑属性,一般用于脚本调用
	virtual STRING			GetAttributeValue(LPCTSTR szValueName);
	//克隆详细信息
	virtual VOID			Clone(const CObject_Item * pItemSource);
	//得到套装编号
	INT						GetSetID(VOID) const;
	//得到武器类型(如果是非武器，返回WEAPON_TYPE_NONE)
	ENUM_WEAPON_TYPE		GetWeaponType(VOID) const;
	//设置装备当前耐久度对于一个已经生成的装备，只有这个值会被改变
	VOID					SetDur(INT Dur);
	//获得当前耐久度
	INT						GetDur(VOID) const;
	//获得最大耐久度
	INT						GetMaxDur(VOID) const;
	
	//克隆详细信息
	VOID					ClonetExtraInfo(CObject_Item_Equip * pItemSource);
	//获得详细信息
	VOID*					GetExtraInfo() const{return m_pExtraDefine;};
	//获得物品的最大叠加数量
	virtual	INT				GetMaxOverLay(VOID) const	{return 1;};

	// 获得装备的修理价格
	virtual INT				GetRepairPrice();

	//--------------------------------------------------------------------------------------------------------------
	//-- supertooltip 使用
	//

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
	virtual LPCTSTR			GetManufacturer();

	// 得到白色属性
	virtual LPCTSTR			GetBaseWhiteAttrInfo();


	// 得到物品的类型描述2006-4-28
	virtual LPCTSTR			GetItemTableTypeDesc();

	// 格式化属性字符串
	void SetAttri(const _ITEM_ATTR& att, char* const pszBuf);

	// 得到扩展蓝色属性
	LPCTSTR			GetExtBlueAttrInfo();

	// 得到扩展绿色属性
	LPCTSTR			GetGreenAttrInfo();

	// 得到套装属性
	LPCTSTR			GetSuitAttrInfo();

	// 得到宝石的个数.
	INT				GetGemMaxCount(VOID) const;

	// 得到宝石的图标
	VOID			GetGemIcon(INT iGemIndex, STRING& strIconName);

	// 得到宝石的附加功能
	VOID			GetGemExtAttr(INT iGemIndex, STRING& strGemAttr);

	//获得最大修理次数
	INT		GetMaxRepair(VOID) const;

	//获得使用职业
	INT		GetNeedJob(VOID) const;

/*
	CObject_Item*			GetAppendItem(INT nIndex) const;

	VOID					SetAppendItem(CObject_Item* pItem,INT nIndex);
*/


public:
	typedef std::vector< _ITEM_ATTR >		EQUIP_ATTRIBUTES;
	typedef std::vector< _ITEM_GEMINFO >	EQUIP_ATTACH_GEM;

public:
	CObject_Item_Equip(INT id);
	virtual ~CObject_Item_Equip();

	//生成一件白色装备
	VOID		AsWhiteEquip(const _DBC_ITEM_EQUIP_WHITE* pDefine);
	//生成一件绿色装备
	VOID		AsGreenEquip(const _DBC_ITEM_EQUIP_GREEN* pDefine); 
	//生成一件蓝色装备
	VOID		AsBlueEquip(const _DBC_ITEM_EQUIP_BLUE* pDefine);

	//得到所有白色属性
	VOID		GetAllWhiteAttribute(STRING& strValue);
	//得到所有蓝色属性
	VOID		GetAllBlueAttribute(STRING& strValue);
	//得到所有绿色属性. 
	VOID		GetAllGreenAttribute(STRING& strValue);
	//得到所有套装属性. 
	VOID		GetAllSuitAttribute(STRING& strValue);

	//得到镶嵌的宝石个数.
	INT			GetGemCount();
	// 得到物品表中quality一列2006－3－30
	virtual INT	GetItemTableQuality(VOID);

	// 得到物品表中type一列2006－3－30
	virtual INT	GetItemTableType(VOID);

public:
	//--------------------------------------------------------------
	enum EQUIP_QUALITY
	{
		INVALID_EQUIP = 0,

		WHITE_EQUIP,
		GREEN_EQUIP,
		BLUE_EQUIP,
		PURPLE_EQUIP,
	};

	//基本属性定义
	union BASE_DEFINE
	{
		const _DBC_ITEM_EQUIP_WHITE*	pDefineAsWhite;
		const _DBC_ITEM_EQUIP_GREEN*	pDefineAsGreen;
		const _DBC_ITEM_EQUIP_BLUE*		pDefineAsBlue;
	};

	enum EQUIP_BIND_INFO
	{
		FREE_BIND = 0,		// 无绑定限制
		BINDED,				// 已经绑定
		GETUP_BIND,			// 拾取绑定
		EQUIP_BIND,			// 装备绑定
	};
	//扩展属性定义
	struct EXTRA_DEFINE
	{
		INT					m_CurDurPoint;				//当前耐久值
		INT					m_MaxDurPoint;				//最大耐久值
		EQUIP_ATTRIBUTES	m_vEquipAttributes;			//装备属性(白色和绿色共有)
		EQUIP_ATTRIBUTES	m_vBlueEquipAttributes;		//装备属性(蓝色装备属性)
		EQUIP_ATTACH_GEM	m_vEquipAttachGem;			//附加的宝石
		INT					m_nLevelNeed;				//需求等级
//		INT					m_nFrogLevel;				//锻造等级
		INT					m_nRepairFailureTimes;		//修理失败次数
		INT					m_nEquipQulity;				//装备品质
		INT					m_nSellPrice;				//售出价格
		//BOOL				m_bLocked;					//是否锁定
		EQUIP_BIND_INFO		M_nEquipBindInfo;			// 装备是否绑定
		INT					m_EnableGemCount;			// 可以镶嵌的宝石个数2006－4－30
		EXTRA_DEFINE()
		{
			m_CurDurPoint = 0;				
			m_MaxDurPoint = 0;				
			m_nLevelNeed  = 0;				
			m_nRepairFailureTimes = 0;		
			m_nEquipQulity = 0;				
			m_nSellPrice   = 0;				
			M_nEquipBindInfo = FREE_BIND;	
			m_EnableGemCount = 0;
		}
													
	};
public:
	// 得到装备基本定义
	BASE_DEFINE	GetEquipBaseDefine() { return m_theBaseDef;}
	// 得到装备的属性(白, 绿, 蓝)
	EQUIP_QUALITY	GetEquipQuantity();

public:

	enum EQUIP_ATTRIB
	{
		EQUIP_ATTRIB_UNIDENTIFY,	// 装备未鉴定。
		EQUIP_ATTRIB_IDENTIFY,		// 装备已鉴定。
	};

	EQUIP_ATTRIB	m_EquipAttrib;
	EQUIP_ATTRIB	GetEquipAttrib();

	// 是否是武器
	BOOL IsWeapon();

	// 是否是防俱
	BOOL IsDefence();

	// 是否是装饰物
	BOOL IsAppend();

protected:
	//品质
	EQUIP_QUALITY		m_Quality;				

	//--------------------------------------------------------------
	//基本属性
	BASE_DEFINE			m_theBaseDef;			//基本属性定义(数据表)

	//----------------------------------------------------------------
	//扩展属性(需要服务器传输)
	EXTRA_DEFINE*		m_pExtraDefine;
	STRING				m_strExtraDesc;

//	std::vector< CObject_Item* >	m_vAppendItem;
};