#include "stdafx.h"
#include "ShopManager.h"

//public
#include "Obj_Human.h"
#include "Player.h"
#include "Obj_Monster.h"

#include "TLBB_DBC.h"//DBC
#include "ItemHelper.h"//TSerialHelper
#include "ItemTable.h"//_ITEM_TYPE
#include "TimeManager.h"//g_pTimeManager
#include "GCShopUpdateMerchandiseList.h"//GCShopUpdateMerchandiseList
#include "FileDef.h"

//globle
using namespace DBC;
StaticShopManager* g_pStaticShopManager =	NULL;

//macro
#define	SHOP_ITEM_PROPERTY_NUM			3
#define	SHOP_ID							0
#define SHOP_NAME						SHOP_ID+1
#define SHOP_TYPE						SHOP_NAME+1
#define	SHOP_REPAIR_LEVEL				SHOP_TYPE+1
#define	SHOP_BUY_LEVEL					SHOP_REPAIR_LEVEL+1
#define	SHOP_REPAIR_TYPE				SHOP_BUY_LEVEL+1
#define	SHOP_BUY_TYPE					SHOP_REPAIR_TYPE+1
#define	SHOP_REPAIR_SPEND				SHOP_BUY_TYPE+1
#define	SHOP_REPAIR_OKPROB				SHOP_REPAIR_SPEND+1
#define	SHOP_SCALE						SHOP_REPAIR_OKPROB+1
#define	SHOP_REFRESH_TIME				SHOP_SCALE+1
#define	SHOP_ITEM_NUM					SHOP_REFRESH_TIME+1
#define	SHOP_ITEM_PROPERTY_BEGIN		SHOP_ITEM_NUM+1
#define	NEW_AND_COPY_ARRAY(PDEST, PSOURCE, NUM, TYPE)\
	PDEST = new TYPE[NUM];\
	memcpy((CHAR*)PDEST, (CHAR*)PSOURCE, NUM*sizeof(TYPE));\

ShopMgr::ShopMgr()
{
	__ENTER_FUNCTION
	m_Count = 0;
	m_Shops = NULL;
	__LEAVE_FUNCTION
}
ShopMgr::~ShopMgr()
{
	__ENTER_FUNCTION
	CleanUp();
	__LEAVE_FUNCTION
}

VOID	ShopMgr::CleanUp( )
{
	__ENTER_FUNCTION
	SAFE_DELETE_ARRAY(m_Shops)
	__LEAVE_FUNCTION
}

//直接从ItemBoxManager::ConvertItemType2Index抄过来
INT		ShopMgr::ConvertItemType2Money(_ITEM_TYPE it)
{
	__ENTER_FUNCTION

	Assert(it.isNull() == FALSE);
	switch(it.m_Class)
	{
	case ICLASS_EQUIP:
		{
			switch(it.m_Quality) 
			{
			case EQUALITY_NORMAL:
				{
					COMMON_EQUIP_TB* pGET	=	g_ItemTable.GetWhiteItemTB(it.ToSerial());
					Assert(pGET);
					return pGET->m_BasePrice;
				}
				break;
			case EQUALITY_BLUE:
				{
					BLUE_EQUIP_TB* pGET =	g_ItemTable.GetBlueItemTB(it.ToSerial());
					Assert(pGET);
					return pGET->m_BasePrice;
				}
				break;
			case EQUALITY_YELLOW:
				{

				}
				break;
			case EQUALITY_GREEN:
				{
					GREEN_EQUIP_TB* pGET =	g_ItemTable.GetGreenItemTB(it.ToSerial());
					Assert(pGET);
					return pGET->m_BasePrice;
				}
				break;
			default:
				{
					Assert(FALSE);
					return FALSE;
				}
			}
			Assert(FALSE);
		}
		break;
	case ICLASS_MATERIAL: 
	case ICLASS_COMITEM:
		{
			COMMITEM_INFO_TB*	pGET		=		g_ItemTable.GetCommItemInfoTB(it.ToSerial());
			Assert(pGET);
			return pGET->m_nBasePrice;
		}
		break;
	case ICLASS_TASKITEM: 
		{
			return 1;
		}
		break;
	case ICLASS_GEM:
		{
			GEMINFO_TB*	pGET		=		g_ItemTable.GetGemInfoTB(it.ToSerial());
			Assert(pGET);
			return pGET->m_nPrice;
		}
		break;
	case ICLASS_STOREMAP:
		{
			STORE_MAP_INFO_TB*	pGET		=		g_ItemTable.GetStoreMapTB(it.ToSerial());
			Assert(pGET);
			return	pGET->m_nBasePrice;
			break;
		}
	case ICLASS_TALISMAN: 
		Assert(FALSE);
		break;
	case ICLASS_GUILDITEM: 
		Assert(FALSE);
		break;
	default:
		Assert(FALSE);
		break;
	}
	Assert(FALSE);

	__LEAVE_FUNCTION
	return -1;
}

_SHOP*	ShopMgr::GetShopByID(INT id)
{
	__ENTER_FUNCTION
	for(INT i = 0; i<m_Count; i++)
	{
		if(m_Shops[i].m_ShopId == id)
		{
			return &m_Shops[i];
		}
	}
	return	NULL;
	__LEAVE_FUNCTION
	return	NULL;
}

INT		ShopMgr::GetShopIndexByID(INT id)
{
	__ENTER_FUNCTION
	for(INT i = 0; i<m_Count; i++)
	{
		if(m_Shops[i].m_ShopId == id)
		{
			return i;
		}
	}
	return	-1;
	__LEAVE_FUNCTION
	return	-1;
}

/*
StaticShopManager
*/
StaticShopManager::~StaticShopManager()
{
	__ENTER_FUNCTION
	CleanUp();
	__LEAVE_FUNCTION
}
BOOL	StaticShopManager::Init()
{
	return LoadShopsFromFile( FILE_SHOP );
}

VOID	StaticShopManager::CleanUp()
{
	__ENTER_FUNCTION
	SAFE_DELETE_ARRAY(m_Shops)
	__LEAVE_FUNCTION
}

BOOL	StaticShopManager::LoadShopsFromFile( CHAR* filename )
{
	__ENTER_FUNCTION

	UINT d = sizeof(_ITEM);
	DBCFile ShopFile(0);
	BOOL ret = ShopFile.OpenFromTXT(filename);
	if( !ret )
		return FALSE;

	INT iTableCount		= ShopFile.GetRecordsNum();
	INT iTableColumn	= ShopFile.GetFieldsNum();

	m_Count		=	iTableCount;
	m_Shops		=	new _SHOP[m_Count];
	INT itemnum = 0;
	INT i,j,k;
	INT itemTypeSn;
	_ITEM_TYPE	itemType;
	INT			PerItemNum	= 0;
	INT			MaxItemNum	= 0;
	FLOAT		PerRate		= 0.0;

	for(i =0;i<iTableCount;i++)
	{
		m_Shops[i].m_ShopId			= 		ShopFile.Search_Posistion(i,SHOP_ID)->iValue;
		strncpy( m_Shops[i].m_szShopName, ShopFile.Search_Posistion(i,SHOP_NAME)->pString, MAX_SHOP_NAME-2 );
		m_Shops[i].m_ShopType		=		ShopFile.Search_Posistion(i,SHOP_TYPE)->iValue;
		m_Shops[i].m_nRepairLevel	=		ShopFile.Search_Posistion(i,SHOP_REPAIR_LEVEL)->iValue;
		m_Shops[i].m_nBuyLevel		=		ShopFile.Search_Posistion(i,SHOP_BUY_LEVEL)->iValue;
		m_Shops[i].m_nRepairType	=		ShopFile.Search_Posistion(i,SHOP_REPAIR_TYPE)->iValue;
		m_Shops[i].m_nBuyType		=		ShopFile.Search_Posistion(i,SHOP_BUY_TYPE)->iValue;
		m_Shops[i].m_nRepairSpend	=		ShopFile.Search_Posistion(i,SHOP_REPAIR_SPEND)->fValue;
		m_Shops[i].m_nRepairOkProb	=		ShopFile.Search_Posistion(i,SHOP_REPAIR_OKPROB)->fValue;
		m_Shops[i].m_scale			=		ShopFile.Search_Posistion(i,SHOP_SCALE)->fValue;
		m_Shops[i].m_refreshTime	=		ShopFile.Search_Posistion(i,SHOP_REFRESH_TIME)->iValue;
		itemnum						=		ShopFile.Search_Posistion(i,SHOP_ITEM_NUM)->iValue;

		//分析实际有的数据
		INT nNum = 0;
		for(k=0; k<itemnum*SHOP_ITEM_PROPERTY_NUM; k++)
		{
			itemTypeSn	= ShopFile.Search_Posistion(i,SHOP_ITEM_PROPERTY_BEGIN+k)->iValue;
			if(itemTypeSn == 0)
			{
				break;
			}
			nNum ++;
			k = k+2;
		}
		itemnum = nNum;

		m_Shops[i].m_ItemList		=		new _SHOP::_MERCHANDISE_LIST(itemnum);

		for(j = 0; j<itemnum*SHOP_ITEM_PROPERTY_NUM; j++)
		{
			//Type
			itemTypeSn	= ShopFile.Search_Posistion(i,SHOP_ITEM_PROPERTY_BEGIN+j)->iValue;
			TSerialHelper help(itemTypeSn);
			itemType	= help.GetItemTypeStruct();

			//GroupNum 0~100
			PerItemNum  = ShopFile.Search_Posistion(i,SHOP_ITEM_PROPERTY_BEGIN+(++j))->iValue;
			if(PerItemNum<0) PerItemNum = 1;
			if(PerItemNum>100) PerItemNum = 100;

			//MaxNum -1代表无限，>0代表有限商品上限，不可以填0,<100
			MaxItemNum	= ShopFile.Search_Posistion(i,SHOP_ITEM_PROPERTY_BEGIN+(++j))->iValue;
			if(MaxItemNum == 0)	MaxItemNum = -1;
			if(PerItemNum>100) PerItemNum = 100;

			//Rate 0.0~1.0
			PerRate = 1.0;

			//ADD TO STRUCTURE
			m_Shops[i].m_ItemList->AddType(itemType, PerItemNum, MaxItemNum, PerRate);

		}
	}
	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;

}

/*
DynamicShopManager
*/
DynamicShopManager::DynamicShopManager(Obj_Monster* pboss)
{
	__ENTER_FUNCTION
	m_pBoss			=	pboss;
	m_Count			=	MAX_SHOP_PER_PERSON;
	m_Shops			=	new _SHOP[m_Count];
	m_aRefeshTimer	=	new CMyTimer[m_Count];
	memset(m_aRefeshTimer,0, m_Count*sizeof(CMyTimer));
	__LEAVE_FUNCTION
}

DynamicShopManager::~DynamicShopManager()
{
	__ENTER_FUNCTION
	CleanUp();
	__LEAVE_FUNCTION
}

BOOL	DynamicShopManager::Init()
{
	m_nCurrent		=	0;
    return	(m_Shops!= NULL)? TRUE:FALSE;
}

VOID	DynamicShopManager::CleanUp()
{
	SAFE_DELETE_ARRAY(m_Shops)
	SAFE_DELETE_ARRAY(m_aRefeshTimer)
	return;
}

INT		DynamicShopManager::AddDynamicShop(_SHOP* pSource)
{
	__ENTER_FUNCTION

	if(m_nCurrent > MAX_SHOP_PER_PERSON)
		return -1;

	for(INT i = 0;i<m_nCurrent; i++)
	{
		if(m_Shops[i].m_ShopId == pSource->m_ShopId)
		{//表里已经有了
			return -1;
		}
	}

	INT	itemnum;

	_SHOP&	ShopRef = m_Shops[m_nCurrent];

	ShopRef.m_ShopId					= 		pSource->m_ShopId;
	ShopRef.m_scale						=		pSource->m_scale;
	ShopRef.m_refreshTime				=		pSource->m_refreshTime;
	itemnum								=		pSource->m_ItemList->m_ListCount;
	ShopRef.m_ItemList					=		new _SHOP::_MERCHANDISE_LIST;
	ShopRef.m_ItemList->m_nCurrent		=		pSource->m_ItemList->m_nCurrent;
	ShopRef.m_ItemList->m_ListCount		=		itemnum;
	ShopRef.m_ShopType					=		pSource->m_ShopType;
	ShopRef.m_bIsRandShop				=		pSource->m_bIsRandShop;
	ShopRef.m_nCountForSell				=		pSource->m_nCountForSell;
	ShopRef.m_nRepairLevel				=		pSource->m_nRepairLevel;
	ShopRef.m_nBuyLevel					=		pSource->m_nBuyLevel;
	ShopRef.m_nRepairType				=		pSource->m_nRepairType;
	ShopRef.m_nBuyType					=		pSource->m_nBuyType;
	ShopRef.m_nRepairSpend				=		pSource->m_nRepairSpend;
	ShopRef.m_nRepairOkProb				=		pSource->m_nRepairOkProb;
	ShopRef.m_bCanBuyBack				=		pSource->m_bCanBuyBack;
	ShopRef.m_bCanMultiBuy				=		pSource->m_bCanMultiBuy;
	ShopRef.m_uSerialNum				=		pSource->m_uSerialNum;
	ShopRef.m_Rand100					=		pSource->m_Rand100;
	strncpy( ShopRef.m_szShopName, pSource->m_szShopName, MAX_SHOP_NAME );
	
	//copycopycopy!!!!!,这些数据要保存在本地，供每个商人自己改变
	NEW_AND_COPY_ARRAY(ShopRef.m_ItemList->m_TypeMaxNum, pSource->m_ItemList->m_TypeMaxNum, itemnum, INT)
	
	//其他数据全部共享静态表中的数据，这些应该全部是只读的，程序启动时由静态商店管理器初始化，
	//系统运行起来后谁都不准改！！
	ShopRef.m_ItemList->m_ListType		=		pSource->m_ItemList->m_ListType;
	ShopRef.m_ItemList->m_ListTypeIndex	=		pSource->m_ItemList->m_ListTypeIndex;
	ShopRef.m_ItemList->m_TypeCount		=		pSource->m_ItemList->m_TypeCount;
	ShopRef.m_ItemList->m_AppearRate	=		pSource->m_ItemList->m_AppearRate;

	//全部操作完成，标识这个商店为动态表中的商店，即，可以被商人自己修改
	ShopRef.m_IsDyShop					=		TRUE;

	//启动计时器
	if(ShopRef.m_refreshTime >0)
		m_aRefeshTimer[m_nCurrent].BeginTimer(ShopRef.m_refreshTime, g_pTimeManager->CurrentTime());
	return m_nCurrent++;
	__LEAVE_FUNCTION
	return -1;
}

BOOL	DynamicShopManager::Tick(UINT uTime)
{
	__ENTER_FUNCTION

	if(!m_pBoss)
		return FALSE;
	Scene* pCurScene = m_pBoss->getScene();
	for(INT i = 0; i< m_nCurrent; i++)
	{
		if(m_Shops[i].m_refreshTime <= 0)
			continue;
		if(m_aRefeshTimer[i].CountingTimer(uTime))
		{//refresh
			INT k = 0;
			//GCShopUpdateMerchandiseList::_MERCHANDISE_ITEM	MerchandiseList[MAX_BOOTH_NUMBER];
			for (INT j = 0; j<m_Shops[i].m_ItemList->m_ListCount;j++)
			{//用静态表中的数据刷新动态表的数据
				INT& LocalMaxNum	= m_Shops[i].m_ItemList->m_TypeMaxNum[j] ; 
				INT& GlobleMaxNum	= g_pStaticShopManager->GetShopByID(m_Shops[i].m_ShopId)->m_ItemList->m_TypeMaxNum[j];
				if(LocalMaxNum != GlobleMaxNum)
				{//改变了，填充消息
					LocalMaxNum = GlobleMaxNum;
				}
			}
		}
	}
	return	TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}
