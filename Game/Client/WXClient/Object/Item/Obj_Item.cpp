#include "StdAfx.h"
#include "Obj_Item.h"
#include "../../DBC/GMDataBase.h"
#include "TDDBC_Struct.h"
#include "TDException.h"

#include "Obj_Item_Equip.h"
#include "Obj_Item_Medicine.h"
#include "Obj_Item_Gem.h"
#include "Obj_Item_StoreMap.h"
#include "../../Procedure/GameProcedure.h"

//需要在ui上显示的属性
const char*  ITEMATTRIBUTE_NAME			= "name";			//物品名
const char*  ITEMATTRIBUTE_LEVEL		= "level";			//物品等级
const char*  ITEMATTRIBUTE_DAM			= "dam";			//损坏度
const char*  ITEMATTRIBUTE_DAMMAX		= "dam_max";		//损坏度最大值
const char*  ITEMATTRIBUTE_PRICE		= "price";			//价格
const char*  ITEMATTRIBUTE_REPAIRNUM	= "repair_num";		//修理次数
const char*  ITEMATTRIBUTE_WHITE_ATT	= "white_all";		//所有白色属性
const char*  ITEMATTRIBUTE_BLUE_ATT		= "blue_all";		//所有蓝色属性
const char*  ITEMATTRIBUTE_GREEN_ATT	= "green_all";		//所有绿色属性
const char*  ITEMATTRIBUTE_PRODUCER		= "producer";		//作者
const char*  ITEMATTRIBUTE_DESC			= "desc";			//详细解释

#define INITIAL_RULE(ppItem) \
	const _DBC_ITEM_RULE* pItemRule = (const _DBC_ITEM_RULE*)s_pItemRule->Search_Index_EQU((UINT)ppItem->nRule);\
	if(pItemRule == NULL) return NULL;\
	pNewItem->m_Rule.bCanDrop		=	pItemRule->bCanDrop;\
	pNewItem->m_Rule.bCanOverLay	=	pItemRule->bCanOverLay;\
	pNewItem->m_Rule.bCanQuick		=	pItemRule->bCanQuick;\
	pNewItem->m_Rule.bCanSale		=	pItemRule->bCanSale;\
	pNewItem->m_Rule.bCanExchange	=	pItemRule->bCanExchange;\
	pNewItem->m_Rule.bCanUse		=	pItemRule->bCanUse;\
	pNewItem->m_Rule.bPickBind		=	pItemRule->bPickBind;\
	pNewItem->m_Rule.bEquipBind		=	pItemRule->bEquipBind;\
	pNewItem->m_Rule.bUnique		=	pItemRule->bUnique;\

std::map< UINT, CObject_Item* > CObject_Item::s_mapAllItem;
CObject_Item::CObject_Item(UINT id)
{
	m_idClient			= id;
	m_ID.m_idUnion		= 0;

	m_typeOwner			= IO_UNKNOWN;
	m_idOwner			= INVALID_ID;
	m_nPosIndex			= 0;
	m_idTable			= INVALID_ID;
	m_nMax				= -1;

	m_bLocked			= FALSE;

	m_nItemSaveStatus   = NO_MORE_INFO;

	m_bByProtect		= FALSE;

	m_pExtraInfoItem	= NULL;

	memset(m_Creator,0, MAX_ITEM_CREATOR_NAME);
}

CObject_Item::~CObject_Item()
{
	if(m_pExtraInfoItem)
	{
		delete m_pExtraInfoItem;
		m_pExtraInfoItem = 0;
	}
}

CObject_Item* CObject_Item::NewItem(UINT idTable)
{
	CObject_Item* pNewItem = NULL;

	//打开物品规则表
	DBC_DEFINEHANDLE(s_pItemRule, DBC_ITEM_RULE);

	//-------------------------------------------------------
	//根据资源id打开资源定义
	if(idTable >= 10100000 && idTable <= 10199999)			// 白色装备
	{
		//打开数据表
		DBC_DEFINEHANDLE(s_pItem_WhiteEquip, DBC_ITEM_EQUIP_WHITE);
		//搜索纪录
		const _DBC_ITEM_EQUIP_WHITE* pWhiteEquip = (const _DBC_ITEM_EQUIP_WHITE*)s_pItem_WhiteEquip->Search_Index_EQU((UINT)idTable);
		if(!pWhiteEquip) return NULL;
	
		pNewItem = new CObject_Item_Equip(_CreateID());
		((CObject_Item_Equip*)pNewItem)->AsWhiteEquip(pWhiteEquip);

		//物品规则
		INITIAL_RULE(pWhiteEquip);
	}
	else if(idTable >= 10400000 && idTable <= 10499999)	// 绿色装备
	{
		//打开数据表
		DBC_DEFINEHANDLE(s_pItem_GreenEquip, DBC_ITEM_EQUIP_GREEN);
		//搜索纪录
		const _DBC_ITEM_EQUIP_GREEN* pGreenEquip = (const _DBC_ITEM_EQUIP_GREEN*)s_pItem_GreenEquip->Search_Index_EQU((UINT)idTable);
		if(!pGreenEquip) return NULL;
	
		pNewItem = new CObject_Item_Equip(_CreateID());
		((CObject_Item_Equip*)pNewItem)->AsGreenEquip(pGreenEquip);

		//物品规则
		INITIAL_RULE(pGreenEquip);
	}
	else if(idTable >= 10200000 && idTable <= 10299999)	// 蓝色装备
	{
		//打开数据表
		DBC_DEFINEHANDLE(s_pItem_BlueEquip, DBC_ITEM_EQUIP_BLUE);
		//搜索纪录
		const _DBC_ITEM_EQUIP_BLUE* pBlueEquip = (const _DBC_ITEM_EQUIP_BLUE*)s_pItem_BlueEquip->Search_Index_EQU((UINT)idTable);
		if(!pBlueEquip) return NULL;
	
		pNewItem = new CObject_Item_Equip(_CreateID());
		((CObject_Item_Equip*)pNewItem)->AsBlueEquip(pBlueEquip);

		//物品规则
		INITIAL_RULE(pBlueEquip);
	}
	else if(idTable >= 20000000 && idTable <= 49999999)	// 原材料 + 食品 + 药瓶
	{
		//打开数据表
		DBC_DEFINEHANDLE(s_pItem_Medic, DBC_ITEM_MEDIC);
		//搜索纪录
		const _DBC_ITEM_MEDIC* pMedic = (const _DBC_ITEM_MEDIC*)s_pItem_Medic->Search_Index_EQU((UINT)idTable);
		if(!pMedic) return NULL;

		pNewItem = new CObject_Item_Medicine(_CreateID());
		((CObject_Item_Medicine*)pNewItem)->AsMedicine(pMedic);
		
		//物品规则
		INITIAL_RULE(pMedic);
	}
	else if(idTable >= 50000000 && idTable <= 59999999)	// 宝石
	{
		//打开数据表
		DBC_DEFINEHANDLE(s_pItem_Gem, DBC_ITEM_GEM);
		//搜索纪录
		const _DBC_ITEM_GEM* pGem = (const _DBC_ITEM_GEM*)s_pItem_Gem->Search_Index_EQU((UINT)idTable);
		if(!pGem) return NULL;

		pNewItem = new CObject_Item_Gem(_CreateID());
		((CObject_Item_Gem*)pNewItem)->AsGem(pGem);

		//物品规则
		INITIAL_RULE(pGem);
	}
	else if(idTable >= 60000000 && idTable <= 69999999)	// 地图
	{
		//打开数据表
		DBC_DEFINEHANDLE(s_pItem_StoreMap, DBC_ITEM_STOREMAP);
		//搜索纪录
		const _DBC_ITEM_STOREMAP* pStoreMap = (const _DBC_ITEM_STOREMAP*)s_pItem_StoreMap->Search_Index_EQU((UINT)idTable);
		if(!pStoreMap) return NULL;

		pNewItem = new CObject_Item_StoreMap(_CreateID());
		((CObject_Item_StoreMap*)pNewItem)->AsStoreMap(pStoreMap);

		//物品规则
		INITIAL_RULE(pStoreMap);
	}
	else
	{
		return NULL;
		TDThrow("Invalid ItemID:%d", idTable);
	}

	pNewItem->m_idTable = idTable;
	s_mapAllItem[pNewItem->GetID()] = pNewItem;

	return pNewItem;
}

VOID CObject_Item::DestroyItem(CObject_Item* pItem)
{
	if(pItem)
	{
		std::map< UINT, CObject_Item* >::iterator it = s_mapAllItem.find(pItem->GetID());
		if(it != s_mapAllItem.end()) s_mapAllItem.erase(it);
		if(pItem->m_pExtraInfoItem) 
		{
			delete pItem->m_pExtraInfoItem;
			pItem->m_pExtraInfoItem = 0;
		}
		delete pItem;
	}
	return;
	//不能再调用其他代码
	//...
}

BOOL CObject_Item::CheckUseInPackage(CObject_Item* pItem, STRING& szMsg)
{
	struct ID_RANGE{
		INT		idBegin;
		INT		idEnd;
	};

	static ID_RANGE invalidRange[]={
		{ITEM_PET_SKILL_STUDY_BEGIN,	ITEM_PET_SKILL_STUDY_END},			//技能书
		{ITEM_PET_RETURN_BABAY_BEGIN-1, ITEM_PET_RETURN_BABAY_END+1},		//还童
		{ITEM_PET_ADD_LIFE_BEGIN,		ITEM_PET_ADD_LIFE_END},				//延长寿命
	};

    if(!pItem) return FALSE;
	for(INT i=0; i<sizeof(invalidRange)/sizeof(ID_RANGE); ++i)
	{
		if(pItem->GetIdTable() > invalidRange[i].idBegin && pItem->GetIdTable() < invalidRange[i].idEnd)
		{
			szMsg = pItem->RuleFailMessage(RULE_USE);
			return FALSE;
		}
	}

	return TRUE;
}

VOID  CObject_Item::ChangeClientID(VOID)
{ 
	s_mapAllItem.erase(m_idClient);
	m_idClient = _CreateID(); 
	s_mapAllItem[m_idClient] = this;
}

UINT CObject_Item::_CreateID(VOID)
{
	static UINT s_ID = 1;
	return s_ID++;
}

tObject_Item* CObject_Item::FindItem(UINT id)
{
	std::map< UINT, CObject_Item* >::iterator it = s_mapAllItem.find(id);
	if(it == s_mapAllItem.end()) return NULL;
	return it->second;
}

//=============================================================================

VOID CObject_Item::SetGUID(USHORT idWorld, USHORT idServer, UINT idSerial)
{
	m_ID.m_idOrg.m_idWorld	= idWorld;
	m_ID.m_idOrg.m_idServer	= idServer;
	m_ID.m_idOrg.m_uSerial	= idSerial;
}

VOID CObject_Item::GetGUID(USHORT& idWorld, USHORT& idServer, UINT& idSerail ) const
{
	idWorld		= m_ID.m_idOrg.m_idWorld;
	idServer	= m_ID.m_idOrg.m_idServer;
	idSerail	= m_ID.m_idOrg.m_uSerial;
}

unsigned __int64 CObject_Item::GetGUID(VOID) const
{
	return m_ID.m_idUnion;
}

STRING CObject_Item::GetAttributeValue(LPCTSTR szValueName)
{
	if(!szValueName) return "";

	if(_stricmp(szValueName, ITEMATTRIBUTE_NAME) == 0)
	{
		return GetName();
	}
	else
	{
		return "";
	}
}

VOID	CObject_Item::Clone(const CObject_Item* pItemSource)
{
	//克隆GUID
	USHORT idWorld		= 0;
	USHORT idServer		= 0;
	UINT idSerail		= 0;
	pItemSource->GetGUID(idWorld, idServer, idSerail);
	SetGUID(idWorld, idServer, idSerail);
	//克隆制造者
	LPCTSTR pManufacturer = ((CObject_Item*)pItemSource)->GetManufacturer();
	strncpy(m_Creator, pManufacturer, MAX_ITEM_CREATOR_NAME);

	//克隆物品的额外信息
	if(pItemSource->GetItemExtraInfoItem())
	{
		if(m_pExtraInfoItem) 
		{
			delete m_pExtraInfoItem;
		}
		m_pExtraInfoItem = new _ITEM;
		memcpy(m_pExtraInfoItem, pItemSource->m_pExtraInfoItem, sizeof(_ITEM));
	}
}


//物品规则验证
BOOL	CObject_Item::Rule(INT ruletype)
{
	switch(ruletype)
	{
	case RULE_DROP:
		{
			return m_Rule.bCanDrop;
		}
		break;
	case RULE_OVERLAY:
		{
			return m_Rule.bCanOverLay;
		}
		break;

	case RULE_QUICK:
		{
			return m_Rule.bCanQuick;
		}
		break;

	case RULE_SALE:
		{
			return m_Rule.bCanSale;
		}
		break;

	case RULE_EXCHANGE:
		{
			return m_Rule.bCanExchange;
		}
		break;

	case RULE_USE:
		{
			return m_Rule.bCanUse;
		}
		break;

	case RULE_PICKBIND:
		{
			return m_Rule.bPickBind;
		}
		break;

	case RULE_EQUIPBIND:
		{
			return m_Rule.bEquipBind;
		}
		break;

	case RULE_UNIQUE:
		{
			return m_Rule.bUnique;
		}
		break;
	default:
		TDAssert(0);
		break;
	}
	return TRUE;
}

STRING	CObject_Item::RuleFailMessage(INT ruletype)
{
	STRING strTemp = "";

	switch(ruletype)
	{
	case RULE_DROP:
		{
			return "此物品不可以丢弃";
		}
		break;
	case RULE_OVERLAY:
		{
			return "此物品不可以叠加";
		}
		break;

	case RULE_QUICK:
		{
			return "此物品不可以放入快捷栏";
		}
		break;

	case RULE_SALE:
		{
			return "此物品不可以卖出";
		}
		break;

	case RULE_EXCHANGE:
		{
			return "此物品不可以交易";
		}
		break;

	case RULE_USE:
		{
			return "此物品不可以使用";
		}
		break;

	case RULE_PICKBIND:
		{
			return "此物品已绑定";
		}
		break;

	case RULE_EQUIPBIND:
		{
			return "此物品已绑定";
		}
		break;

	case RULE_UNIQUE:
		{
			return "";
		}
		break;
	default:
		TDAssert(0);
		break;
	}
	return "";
}

// 得到玩家使用这个物品需要的等级
INT	CObject_Item::GetNeedLevel()
{
	// 表示没有这个属性
	return -1;
}

// 得到物品耐久度
INT	CObject_Item::GetItemDur()
{
	
	return -1;
}

// 得到物品最大耐久度
INT	CObject_Item::GetItemMaxDur()
{

	return -1;
}

// 得到物品的修理次数 
INT	CObject_Item::GetItemRepairCount()
{
	// -1 表示无修理信息
	return -1;
}

// 得到物品的绑定信息 
INT	CObject_Item::GetItemBindInfo()
{
	// -1 无绑定信息
	return -1;
}

// 得到物品卖给npc的价格
INT	CObject_Item::GetItemPrice()
{
	// -1 无价格信息
	return -1;
}

// 得到物品的制作人
LPCTSTR	CObject_Item::GetManufacturer()
{
	return _T("");
}

// 得到白色属性
LPCTSTR	CObject_Item::GetBaseWhiteAttrInfo()
{
	return _T("");
}

// 得到扩展蓝色属性
LPCTSTR	CObject_Item::GetExtBlueAttrInfo()
{
	return _T("");
}

INT		CObject_Item::GetItemTargetType()
{
	return ITEM_TARGET_TYPE_INVALID;
}

//设置物品保存状态 2006－3－24
void CObject_Item::SetItemSaveStatus(SAVE_ITEM_STATUS nSaveStatus)
{
	m_nItemSaveStatus = nSaveStatus;
}

//设置物品保存状态 2006－3－30
BOOL CObject_Item::IsItemHasDetailInfo()
{
	if(GET_MORE_INFO == m_nItemSaveStatus)
	{
		return FALSE;
	}

	return TRUE;
}

INT	CObject_Item::GetItemTableQuality(VOID)
{
	return 0;
}

INT	CObject_Item::GetItemTableType(VOID)
{
	return 0;
}

// 得到物品的类型描述2006-4-28
LPCTSTR	CObject_Item::GetItemTableTypeDesc()
{
	return "无类型描述";
}

VOID	CObject_Item::SetManufacturer(const _ITEM * pItemInfo)
{
	if(pItemInfo && pItemInfo->GetCreatorVar())
	{
		strncpy(m_Creator, pItemInfo->m_Creator, MAX_ITEM_CREATOR_NAME);
	}
}

VOID	CObject_Item::SetExtraInfo(const _ITEM * pItemInfo)
{
	if(!m_pExtraInfoItem)
	{
		m_pExtraInfoItem = new _ITEM;
	}

	if(pItemInfo)
	{
		memcpy(m_pExtraInfoItem, pItemInfo, sizeof(_ITEM));
	}
}

INT CObject_Item::GetItemTransferString(STRING& strResult) const
{
	INT nLen = 0;

	CHAR buf[MAX_ITEM_STRING_LENGTH+1] = {0};
	UINT uValue;

	//idTable
	uValue = (UINT)m_idTable;
	memcpy(buf, &uValue, sizeof(UINT));
	nLen += sizeof(UINT);

	//Creator
	memcpy(buf+nLen, m_Creator, strlen(m_Creator));
	nLen += (INT)strlen(m_Creator);

	TDAssert(nLen <= MAX_ITEM_STRING_LENGTH);

	//创建完成，需要在发送前进行必要的转换
	CHAR strBuf[MAX_ITEM_STRING_LENGTH+1] = {0};
	Bin2Str_Transfer(buf, nLen, strBuf);

	//保存转化的结果
	CHAR strLen[5] = {0};
	_snprintf(strLen, 4, "%03di", strlen(strBuf)+1);

	nLen = (INT)(strlen(strLen)+strlen(strBuf));

	strResult = strLen;		//描述字串的长度(固定为3)
	strResult += strBuf;	//描述字串的内容

	return nLen;
}

VOID CObject_Item::SetExtraInfoFromString(LPCTSTR strOrginal, INT nOrginalSize)
{
	if(!strOrginal || 0 == nOrginalSize) return;

	INT nLen = 0;
	UINT nSize = 0;

	//描述字串的长度(skip)
	CHAR strIn[MAX_ITEM_STRING_LENGTH+1] = {0};
	Str2Bin_Transfer(strOrginal+4, nOrginalSize-4, strIn,MAX_ITEM_STRING_LENGTH, nSize);

	//idTable(skip)
	nLen += sizeof(UINT);

	SetNumber(1);
	//Creator
	if(nLen != nSize) //说明后面的内容是创建者信息
	{
		memcpy(m_Creator, strIn+nLen, strlen(strIn+nLen));
		nLen += (INT)strlen(strIn+nLen);
	}

	//TDAssert(nLen == nSize);
}

LPCTSTR CObject_Item::ConvertSysColour(INT nColor)
{
	DBC_DEFINEHANDLE(s_pSystemColorDBC, DBC_SYSTEM_COLOR);
	const _DBC_SYSTEM_COLOR* pSysColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(nColor);

	static STRING strColor;
	if(pSysColor)
	{
		//字体颜色
		if(strlen(pSysColor->strFontColor) == 6)
		{
			strColor += "#c";
			strColor += pSysColor->strFontColor;
		}

		//扩边颜色
		if(strlen(pSysColor->strExtenColor) == 6)
		{
			strColor += "#e";
			strColor += pSysColor->strExtenColor;
		}
	}
	else
	{
		strColor = "#cFFFFFF";	//缺省白色
	}

	return strColor.c_str();
}

BOOL CObject_Item::Bin2Str_Transfer(const CHAR* pIn,UINT InLength,CHAR* pOut)
{
	if(InLength==0)
	{
		return FALSE;
	}
	UINT iOut = 0;
	for(UINT i = 0 ;i<InLength;i++)
	{
		switch(pIn[i]) 
		{
		case '\0':
			{
				pOut[iOut] = '\\';
				iOut++;
				pOut[iOut] = '0';
				iOut++;
			}
			break;
		case '\\':
			{
				pOut[iOut] = '\\';
				iOut++;
				pOut[iOut] = '\\';
				iOut++;
			}
			break;
		default:
			{
				pOut[iOut] = pIn[i];
				iOut++;
			}
			break;
		}

	}
	return TRUE;
}

BOOL CObject_Item::Str2Bin_Transfer(const CHAR* pIn,UINT InLength,CHAR* pOut,UINT OutLimit,UINT& OutLength)
{
	if(InLength==0)
	{
		return FALSE;
	}

	UINT iOut = 0;
	UINT i;
	for( i = 0 ;i<InLength-1;)
	{
		if((pIn[i]=='\\') && (pIn[i+1]=='0'))
		{
			pOut[iOut]	=	'\0';
			iOut++;
			i+=2;
			continue;
		}
		else if((pIn[i]=='\\') && (pIn[i+1]=='\\'))
		{
			pOut[iOut] = '\\';
			iOut++;
			i+=2;
			continue;
		}
		else if(pIn[i]=='\0')
		{
			break;
		}
		pOut[iOut] = pIn[i];
		iOut++;
		i++;
		if(iOut>=OutLimit)
			break;
	}

	//保留InLength-1位置的数据。
	if(iOut < OutLimit && i == InLength-1)
	{
		pOut[iOut] = pIn[i];
		iOut++;
		i++;
	}

	OutLength = iOut;
	return TRUE;
}
