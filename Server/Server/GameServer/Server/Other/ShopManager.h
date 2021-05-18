#ifndef _SHOP_MANAGER_H_
#define _SHOP_MANAGER_H_
#include "Type.h"
#include "ItemManager.h"
#include "ItemBoxManager.h"
#include "Rand.h"

#define MAX_SHOP_PER_PERSON				10
//#define MAX_SOLDOUT_ITEM_PER_PERSON		5

class Obj_Human;
class Obj_Monster;

/*
_SHOP,一个商店
*/
struct _SHOP
{
	/*
	_MERCHANDISE_LIST,商店中的商品列表
	*/
	struct _MERCHANDISE_LIST
	{
		USHORT			m_ListCount;
		_ITEM_TYPE*		m_ListType;
		UINT*			m_ListTypeIndex;
		INT*			m_TypeCount;
		INT*			m_TypeMaxNum;
		FLOAT*			m_AppearRate;
		//INT*			m_pnPrice;				// 价格，当该商店货币单位不是 CU_MONEY 时生效
		BOOL*			m_pbForSale;			// 随机商店使用：待售商品
		INT				m_nCurrent;				// 运行时数据

		//
		_MERCHANDISE_LIST() 
		{
			m_ListCount = 0;
			m_nCurrent = 0;
			m_ListType	= NULL;
			m_ListTypeIndex = NULL;
			m_TypeCount = NULL;
			m_AppearRate = NULL;
			//m_pnPrice = NULL;
			m_pbForSale = NULL;
		};
		//
		_MERCHANDISE_LIST(BYTE num)
		{
			m_ListCount		= num;
			m_nCurrent		= 0;
			m_ListType		= new _ITEM_TYPE[m_ListCount];
			m_ListTypeIndex	= new UINT[m_ListCount];
			m_TypeCount		= new INT[m_ListCount];
			m_TypeMaxNum	= new INT[m_ListCount];
			m_AppearRate	= new FLOAT[m_ListCount];
			//m_pnPrice		= new INT[m_ListCount];
			m_pbForSale		= new BOOL[m_ListCount];
		}
		~_MERCHANDISE_LIST()
		{
			SAFE_DELETE_ARRAY(m_ListType)
			SAFE_DELETE_ARRAY(m_ListTypeIndex)
			SAFE_DELETE_ARRAY(m_TypeCount)
			SAFE_DELETE_ARRAY(m_TypeMaxNum)
			SAFE_DELETE_ARRAY(m_AppearRate)
			//SAFE_DELETE_ARRAY(m_pnPrice)
			SAFE_DELETE_ARRAY(m_pbForSale)
		}

		//
		VOID	AddType(_ITEM_TYPE it, INT Count, INT MaxCount, /*INT nPrice,*/ FLOAT Rate)
		{
			Assert(!it.isNull());
			Assert(m_nCurrent<m_ListCount);
			m_ListType[m_nCurrent]		=	it;
			m_ListTypeIndex[m_nCurrent]	=	it.ToSerial();
			m_TypeCount[m_nCurrent]		=	Count;
			m_TypeMaxNum[m_nCurrent]	=	MaxCount;
			//m_pnPrice[m_nCurrent]		=	nPrice;
			m_pbForSale[m_nCurrent]		=	FALSE;
			m_AppearRate[m_nCurrent++]	=	Rate;
		}
	};

	_SHOP()
	{
		m_ShopId = -1;
		m_bIsRandShop = FALSE;
		m_nCountForSell = 0;
		m_nCurrencyUnit = CU_MONEY;
		m_nRepairLevel=0;	
		m_nBuyLevel=0;	
		m_nRepairType=-1;	
		m_nBuyType=-1;		
		m_nRepairSpend=0;	
		m_nRepairOkProb=0;
		m_scale = 0.0f;
		m_ItemList = NULL;
		m_refreshTime = 0;
		m_IsDyShop = FALSE;
		m_uSerialNum = 0;
		m_bCanBuyBack = FALSE;
		m_bCanMultiBuy	=	FALSE;
		m_szShopName[0] = 0;
	}
	_SHOP(INT itemNum)
	{
		m_ShopId = -1;
		m_bIsRandShop = FALSE;
		m_nCountForSell = 0;
		m_nCurrencyUnit = CU_MONEY;
		m_nRepairLevel=0;
		m_nBuyLevel=0;	
		m_nRepairType=-1;	
		m_nBuyType=-1;		
		m_nRepairSpend=0;	
		m_nRepairOkProb=0;
		m_scale = 0.0f;
		m_ItemList = new _MERCHANDISE_LIST(itemNum);
		m_refreshTime = 0;
		m_IsDyShop = FALSE;
		m_uSerialNum = 0;
		m_bCanBuyBack = FALSE;
		m_bCanMultiBuy	=	FALSE;
		m_szShopName[0] = 0;
	}


	~_SHOP()
	{
		SAFE_DELETE(m_ItemList)
	}

	UCHAR				GetSerialNum( ) { return m_uSerialNum; }
	VOID				UpdateSerialNum( ) { m_uSerialNum = (m_uSerialNum+1) % 255; }

	// 验证索引的合法性，[随机商店适用：将显示列表的索引转换成商店列表中的实际索引]
	INT					ConvertIndex( INT nIndex );

	// 设置随机商品列表
	//VOID				SetRandPriceItemList(RandPriceItemList_t* pItemList);

	// 产生随机价格
	BOOL				GenerateRandItemPrice();

	// 获得随机价格
	INT					GetRPItemValue(UINT uItemIdx, ValueType valuetype);

	//
	INT					m_ShopId;

	CHAR				m_szShopName[MAX_SHOP_NAME];

	INT					m_ShopType;		//0普通商店， 1元宝商店
	// 随机商店属性
	BOOL				m_bIsRandShop;
	INT					m_nCountForSell;			// 随机库中挑选出来出售的商品数量
	// 随机商店属性

	INT					m_nCurrencyUnit;			// 货币单位 enum CURRENCY_UNIT
	INT					m_refreshTime;

	// 以下只有货币单位是 CU_MONEY 时生效
	INT					m_nRepairLevel;				// 修理等级
	INT					m_nBuyLevel;				// 收购等级
	INT					m_nRepairType;				// 修理类型
	INT					m_nBuyType;					// 商店的收购类型
	FLOAT				m_nRepairSpend;				// 修理花费
	FLOAT				m_nRepairOkProb;			// 修理成功几率
	BOOL				m_bCanBuyBack;				// 是否能回购

	FLOAT				m_scale;
	// 以上只有货币单位是 CU_MONEY 时生效

	_MERCHANDISE_LIST*	m_ItemList;
	BOOL				m_IsDyShop;
	BOOL				m_bCanMultiBuy;				//是否能够购买多个商品

	UCHAR				m_uSerialNum;				// 随机商店流水号，记录以免因为玩家买了不同版本的商店的物品导致错误购买

	_100_PER_RANDOM_TABLE		m_Rand100;
	//RandPriceItemList_t			m_aRPItemList;	

};

/*
ShopMgr,负责数据的访问与管理
*/
class ShopMgr
{
public:
	friend class Obj_Human;
//initial+destory
public:
	ShopMgr();
	~ShopMgr();
public:
	virtual	BOOL		Init(){return TRUE;};
	virtual VOID		CleanUp();
	_SHOP*				GetShopByIndex(INT index){return &m_Shops[index];}
	_SHOP*				GetShopByID(INT id);
	INT					GetShopIndexByID(INT id);

//globle
public:
	static INT			ConvertItemType2Money(_ITEM_TYPE it);

//data
protected:
	_SHOP*				m_Shops;
	INT					m_Count;
};

/*
StaticShopManager,静态态的商店管理器,整个游戏世界只有一个静态商店管理器的实例，
用来保存所有不会变化的商店的信息．它同时也是所有动态商店管理器的生成商店的模版．
*/
class StaticShopManager	: public ShopMgr
{
public:
	StaticShopManager(){;}
	~StaticShopManager();
public:
	BOOL				Init();
	VOID				CleanUp();
private:
	BOOL				LoadShopsFromFile( CHAR* filename ) ;	
};

/*
DynamicShopManager,顾名思义,动态的商店管理器,
它的每个实例挂在每个NPC上，由NPC心跳触发Ｔｉｃｋ()
动态的商店可以随着时间刷新，来更改自己的局部数据
*/
class DynamicShopManager : public ShopMgr
{

public:
	DynamicShopManager(Obj_Monster* pboss);
	~DynamicShopManager();
public:
	INT					GetShopsNum()const{return m_nCurrent;}
public:
	BOOL				Init();
	VOID				CleanUp();
	INT					AddDynamicShop(_SHOP* pSource);
	BOOL				Tick(UINT uTime);

private:
	INT					m_nCurrent;
	CMyTimer*			m_aRefeshTimer;
	Obj_Monster*		m_pBoss;

};

extern StaticShopManager* g_pStaticShopManager;

#endif