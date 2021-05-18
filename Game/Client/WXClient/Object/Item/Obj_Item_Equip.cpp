#include "StdAfx.h"
#include "Obj_Item_Equip.h"
#include "GameStruct_Item.h"
#include "../../DataPool/GMDataPool.h"
#include "../../DBC/GMDataBase.h"
#include "TDDBC_Struct.h"
#include "TDException.h"
#include "../../Procedure/GameProcedure.h"

const char* g_szAttachAttName[] = 
{
	"equip_attr_maxhp",								//0   350
	"equip_attr_maxhp",								//1	  350
	"equip_attr_restorehp",							//2	  351
	"equip_attr_maxmp",								//3	  353
	"equip_attr_maxmp",								//4	  353
	"equip_attr_restoremp",							//5	  354
	"equip_attr_attack_cold",						//6	  356
	"equip_attr_resist_cold",						//7	  357
	"equip_attr_time_chihuan",						//8	  358
	"equip_attr_attack_fire",						//9	  359
	"equip_attr_resist_fire",						//10  360
	"equip_attr_time_ruohua",						//11  361
	"equip_attr_attack_light",						//12  362
	"equip_attr_resist_light",						//13  363
	"equip_attr_time_xuanyun",						//14  364
	"equip_attr_attack_poison",						//15  365
	"equip_attr_resist_poison",						//16  366
	"equip_attr_time_zhongdu",						//17  367
	"equip_attr_resist_all",						//18  368
	"equip_attr_attack_p",							//19  371
	"equip_attr_attack_p",							//20  371
	"equip_attr_attack_ep",							//21  372
	"equip_attr_defence_p",							//22  377
	"equip_attr_defence_p",							//23  377
	"equip_attr_defence_ep",						//24  378
	"equip_attr_immunity_p",						//25  383
	"equip_attr_attack_m",							//26  374
	"equip_attr_attack_m",							//27  374
	"equip_attr_attack_em",							//28  375
	"equip_attr_defence_m",							//29  380
	"equip_attr_defence_m",							//30  380
	"equip_attr_defence_em",						//31  381
	"equip_attr_immunity_m",						//32  384
	"equip_attr_attackspeed",						//33  386
	"equip_attr_cooldowm",							//34  387
	"equip_attr_hit",								//35  389
	"equip_attr_miss",								//36  390
	"equip_attr_2attack",							//37  391
	"equip_attr_nodefence",							//38  392
	"equip_attr_movespeed",							//39  388
	"equip_attr_damageret",							//40  393
	"equip_attr_damage2mp",							//41  394
	"equip_attr_str",								//42  395
	"equip_attr_spr",								//43  396
	"equip_attr_con",								//44  397
	"equip_attr_int",								//45  398
	"equip_attr_dex",								//46  399
	"equip_yuliu",									//47  499
	"equip_attr_all",								//48  401
	"equip_attr_hp_thieve",							//49  404
	"equip_attr_mop_thieve",						//50  405
	"equip_yuliu",									//51  499
	"equip_yuliu",									//52  499
	"equip_yuliu",									//53  499
	"equip_base_attack_p",							//54  330
	"equip_yuliu",									//55  499
	"equip_base_attack_m",							//56  332
	"equip_yuliu",									//57  499
	"equip_yuliu",									//58  499
	"equip_yuliu",									//69  499
	"equip_base_defence_p",							//60  334
	"equip_yuliu",									//61  499
	"equip_base_defence_m",							//62  336
	"equip_yuliu",									//63  499
	"equip_base_miss",								//64  338
	"equip_yuliu",									//65  499
};

// 55 - 66[1]
//const char* g_szGemAttrName[] = 
//{
//	"增加HP的上限",								//0		
//	"百分比增加HP的上限",						//1
//	"加快HP的回复速度",							//2
//	"增加MP的上限",								//3
//	"百分比增加MP的上限",						//4
//	"加快MP的回复速度",							//5
//	"冰攻击",									//6
//	"冰抵抗",									//7
//	"减少冰冻迟缓时间",							//8
//	"火攻击",									//9
//	"火抵抗",									//10
//	"减少火烧持续时间",							//11
//	"电攻击",									//12
//	"电抵抗",									//13
//	"减少电击眩晕时间",							//14
//	"毒攻击",									//15
//	"毒抵抗",									//16
//	"减少中毒时间",								//17
//	"按百分比抵消所有属性攻击",					//18
//	"物理攻击",									//19
//	"按百分比增加物理攻击",						//20
//	"对装备基础物理攻击百分比加成",				//21
//	"物理防御",									//22
//	"按百分比增加物理防御",						//23
//	"对装备基础物理防御百分比加成",				//24
//	"按百分比抵消物理伤害",						//25
//	"魔法攻击",									//26
//	"按百分比增加魔法攻击",						//27
//	"对装备基础魔法攻击百分比加成",				//28
//	"魔法防御",									//29
//	"按百分比增加魔法防御",						//30
//	"对装备基础魔法防御百分比加成"				//31
//	"按百分比抵消魔法伤害",						//32
//	"攻击速度(两次攻击间隔时间)",				//33
//	"魔法冷却速度",								//34
//	"命中",										//35
//	"闪避",										//36
//	"会心一击（双倍攻击）的百分比",				//37
//	"无视对方防御比率",							//38
//	"移动速度百分比",							//39
//	"伤害反射",									//40
//	"伤害由内力抵消",							//41
//	"力量",										//42
//	"灵气",										//43
//	"体制",										//44
//	"定力",										//45
//	"身法",										//46
//	"悟性",										//47
//	"增加所有的人物一级属性",					//48
//	"生命偷取",									//49
//	"内力偷取",									//50
//	"增加某个技能等级",							//51
//	"增加所有技能等级",							//52
//	"特殊技能发动概率",							//53
//	"基础物理攻击",								//54
//	"基础魔法攻击",								//55
//	"基础攻击速度",								//56
//	"基础物理防御",								//57
//	"基础魔法防御",								//58
//	"基础闪避",									//59
//											
//};//											
												
												
CObject_Item_Equip::CObject_Item_Equip(INT id)	
		: CObject_Item(id)						
{
	m_Quality = INVALID_EQUIP;
	m_pExtraDefine	= NULL;
	m_EquipAttrib = EQUIP_ATTRIB_IDENTIFY;
}


CObject_Item_Equip::~CObject_Item_Equip()
{
	if(m_pExtraDefine)
	{
		delete m_pExtraDefine;
	}
	m_pExtraDefine = NULL;
}

ITEM_CLASS CObject_Item_Equip::GetItemClass(VOID) const 
{ 
	switch(m_Quality)
	{
	case WHITE_EQUIP:
		return (ITEM_CLASS)m_theBaseDef.pDefineAsWhite->nClass;

	case GREEN_EQUIP:
		return (ITEM_CLASS)m_theBaseDef.pDefineAsGreen->nClass;

	case BLUE_EQUIP:
		return (ITEM_CLASS)m_theBaseDef.pDefineAsBlue->nClass;

	default:
		TDAssert(FALSE);
	}
	return ICLASS_EQUIP; 
}

//生成一件白色装备
VOID CObject_Item_Equip::AsWhiteEquip(const _DBC_ITEM_EQUIP_WHITE* pDefine)
{
	m_Quality = WHITE_EQUIP;
	m_theBaseDef.pDefineAsWhite = pDefine;
	m_nParticularID = ( ( ( ( ( m_theBaseDef.pDefineAsWhite->nClass * 100 ) + m_theBaseDef.pDefineAsWhite->nQuality ) * 100 ) + m_theBaseDef.pDefineAsWhite->nType ) * 1000 ) + m_theBaseDef.pDefineAsWhite->nIndex;

	if(!m_pExtraDefine)
		m_pExtraDefine = new EXTRA_DEFINE;

	m_pExtraDefine->m_CurDurPoint = pDefine->nBMaxDur;
	m_pExtraDefine->m_MaxDurPoint = pDefine->nBMaxDur;
	m_pExtraDefine->m_nLevelNeed  = pDefine->nLevelRequire;
	m_pExtraDefine->m_nRepairFailureTimes  = pDefine->nBMaxRepair;
	m_pExtraDefine->m_nEquipQulity= CObject_Item_Equip::WHITE_EQUIP;
	m_pExtraDefine->m_nSellPrice  = pDefine->nBasePrice;
	
	// 暂时无绑定信息以后需要添加
	//m_pExtraDefine->m_bLocked  = FALSE;

	_ITEM_ATTR	attr;
	_ITEM_VALUE	attrValue;
	m_pExtraDefine->m_vEquipAttributes.clear();
	for(INT i=0; i<66; i++)
	{
		if(-1 != pDefine->NOTCARE[i] )
		{
			attrValue.m_Value = (SHORT)pDefine->NOTCARE[i];
			attr.m_AttrType = i;
			attr.m_Value    = attrValue;
			m_pExtraDefine->m_vEquipAttributes.push_back(attr);
		}
	}

	//for(INT i=0; i<pItemInfo->GetEquipData()->m_StoneCount; i++)
	//{
	//	m_pExtraDefine->m_vEquipAttachGem.push_back(pItemInfo->GetEquipData()->m_pGemInfo[i]);
	//}//

}

//生成一件绿色装备
VOID CObject_Item_Equip::AsGreenEquip(const _DBC_ITEM_EQUIP_GREEN* pDefine)
{
	m_Quality = GREEN_EQUIP;
	m_theBaseDef.pDefineAsGreen = pDefine;
	m_nParticularID = ( ( ( ( ( m_theBaseDef.pDefineAsGreen->nClass * 100 ) + m_theBaseDef.pDefineAsGreen->nQuality ) * 100 ) + m_theBaseDef.pDefineAsGreen->nType ) * 1000 ) + m_theBaseDef.pDefineAsGreen->nIndex;

	if(!m_pExtraDefine)
		m_pExtraDefine = new EXTRA_DEFINE;

	m_pExtraDefine->m_CurDurPoint = pDefine->nBMaxDur;
	m_pExtraDefine->m_MaxDurPoint = pDefine->nBMaxDur;
	m_pExtraDefine->m_nLevelNeed  = pDefine->nLevelRequire;
	m_pExtraDefine->m_nRepairFailureTimes  = pDefine->nBMaxRepair;
	m_pExtraDefine->m_nEquipQulity= CObject_Item_Equip::GREEN_EQUIP;
	m_pExtraDefine->m_nSellPrice  = pDefine->nBasePrice;
	
	// 暂时无绑定信息以后需要添加
	//m_pExtraDefine->m_bLocked  = FALSE;

	
    //for(INT i=0; i<pItemInfo->GetEquipData()->m_AttrCount; i++)
	//{
	//	m_pExtraDefine->m_vEquipAttributes.push_back(pItemInfo->GetEquipData()->m_pAttr[i]);
	//}

	//for(INT i=0; i<pItemInfo->GetEquipData()->m_StoneCount; i++)
	//{
	//	m_pExtraDefine->m_vEquipAttachGem.push_back(pItemInfo->GetEquipData()->m_pGemInfo[i]);
	//}

	_ITEM_ATTR	attr;
	_ITEM_VALUE	attrValue;
	m_pExtraDefine->m_vEquipAttributes.clear();
	for(INT i=0; i<66; i++)
	{
		if(-1 != pDefine->NOTCARE[i] )
		{
			attrValue.m_Value = (SHORT)pDefine->NOTCARE[i];
			attr.m_AttrType = i;
			attr.m_Value    = attrValue;
			m_pExtraDefine->m_vEquipAttributes.push_back(attr);
		}
	}

}

//生成一件蓝色装备
VOID CObject_Item_Equip::AsBlueEquip(const _DBC_ITEM_EQUIP_BLUE* pDefine)
{
	m_Quality = BLUE_EQUIP;
	m_theBaseDef.pDefineAsBlue = pDefine;
	m_nParticularID = ( ( ( ( ( m_theBaseDef.pDefineAsBlue->nClass * 100 ) + m_theBaseDef.pDefineAsBlue->nQuality ) * 100 ) + m_theBaseDef.pDefineAsBlue->nType ) * 1000 ) + m_theBaseDef.pDefineAsBlue->nIndex;

	if(!m_pExtraDefine)
		m_pExtraDefine = new EXTRA_DEFINE;

	m_pExtraDefine->m_CurDurPoint = pDefine->nBMaxDur;
	m_pExtraDefine->m_MaxDurPoint = pDefine->nBMaxDur;
	m_pExtraDefine->m_nLevelNeed  = pDefine->nLevelRequire;
	m_pExtraDefine->m_nRepairFailureTimes  = pDefine->nBMaxRepair;
	m_pExtraDefine->m_nEquipQulity= CObject_Item_Equip::BLUE_EQUIP;
	m_pExtraDefine->m_nSellPrice  = pDefine->nBasePrice;
	
	m_pExtraDefine->m_vEquipAttributes.clear();
	_ITEM_ATTR	attr;
	_ITEM_VALUE	attrValue;
	for(INT i=53; i<66; i++)
	{
		if(-1 != pDefine->NOTCARE[i] )
		{
			attrValue.m_Value = (SHORT)pDefine->NOTCARE[i];
			attr.m_AttrType = i;
			attr.m_Value    = attrValue;
			m_pExtraDefine->m_vEquipAttributes.push_back(attr);
		}
	}

	

}

HUMAN_EQUIP	CObject_Item_Equip::GetItemType(VOID) const
{
	switch(m_Quality)
	{
	case WHITE_EQUIP:
		return (HUMAN_EQUIP)(m_theBaseDef.pDefineAsWhite->nEquipPoint);

	case GREEN_EQUIP:
		return (HUMAN_EQUIP)(m_theBaseDef.pDefineAsGreen->nEquipPoint);

	case BLUE_EQUIP:
		return (HUMAN_EQUIP)(m_theBaseDef.pDefineAsBlue->nEquipPoint);

	case INVALID_EQUIP:
	default:
		return (HUMAN_EQUIP)-1;
	}
}

INT CObject_Item_Equip::GetVisualID(VOID) const
{
	switch(m_Quality)
	{
	case WHITE_EQUIP:
		return (HUMAN_EQUIP)(m_theBaseDef.pDefineAsWhite->nVisualID);

	case GREEN_EQUIP:
		return (HUMAN_EQUIP)(m_theBaseDef.pDefineAsGreen->nVisualID);

	case BLUE_EQUIP:
		return (HUMAN_EQUIP)(m_theBaseDef.pDefineAsBlue->nVisualID);

	case INVALID_EQUIP:
	default:
		return -1;
	}
}

ENUM_WEAPON_TYPE CObject_Item_Equip::GetWeaponType(VOID) const
{
	INT nDefine = -1;
	switch(m_Quality)
	{
	case WHITE_EQUIP:
		nDefine = (INT)(m_theBaseDef.pDefineAsWhite->nType);
		break;

	case GREEN_EQUIP:
		nDefine = (INT)(m_theBaseDef.pDefineAsGreen->nType);
		break;

	case BLUE_EQUIP:
		nDefine = (INT)(m_theBaseDef.pDefineAsBlue->nType);
		break;

	case INVALID_EQUIP:
	default:
		nDefine =  -1;
		break;
	}

	//转化为ENUM_WEAPON_TYPE
	switch(nDefine)
	{
	case WTYPE_DAO:		return WEAPON_TYPE_LONG;
	case WTYPE_QIANG:	return WEAPON_TYPE_SPEAR;
	case WTYPE_1DUAN:	return WEAPON_TYPE_SHORT;
	case WTYPE_2DUAN:	return WEAPON_TYPE_DSHORT;
	case WTYPE_SHAN:	return WEAPON_TYPE_FAN;
	case WTYPE_HUAN:	return WEAPON_TYPE_RING;
	default:			return WEAPON_TYPE_NONE;

	}
}

LPCTSTR CObject_Item_Equip::GetName(VOID) const
{
	switch(m_Quality)
	{
	case WHITE_EQUIP:
		return (m_theBaseDef.pDefineAsWhite->szName);

	case GREEN_EQUIP:
		return (m_theBaseDef.pDefineAsGreen->szName);

	case BLUE_EQUIP:
		return (m_theBaseDef.pDefineAsBlue->szName);

	case INVALID_EQUIP:
	default:
		return NULL;
	}
}

LPCTSTR CObject_Item_Equip::GetIconName(VOID) const
{
	switch(m_Quality)
	{
	case WHITE_EQUIP:
		return (m_theBaseDef.pDefineAsWhite->szIcon);

	case GREEN_EQUIP:
		return (m_theBaseDef.pDefineAsGreen->szIcon);

	case BLUE_EQUIP:
		return (m_theBaseDef.pDefineAsBlue->szIcon);

	case INVALID_EQUIP:
	default:
		return NULL;
	}
}

LPCTSTR CObject_Item_Equip::GetDesc(VOID) const
{
	switch(m_Quality)
	{
	case WHITE_EQUIP:
		return (m_theBaseDef.pDefineAsWhite->szDesc);

	case GREEN_EQUIP:
		return (m_theBaseDef.pDefineAsGreen->szDesc);

	case BLUE_EQUIP:
		return (m_theBaseDef.pDefineAsBlue->szDesc);

	case INVALID_EQUIP:
	default:
		return NULL;
	}
}

INT CObject_Item_Equip::GetGemMaxCount(VOID) const
{
	//switch(m_Quality)
	//{
	//case WHITE_EQUIP:
	//	return (m_theBaseDef.pDefineAsWhite->nBMaxGem);

	//case GREEN_EQUIP:
	//	return (m_theBaseDef.pDefineAsGreen->nBMaxGem);

	//case BLUE_EQUIP:
	//	return (m_theBaseDef.pDefineAsBlue->nBMaxGem);

	//case INVALID_EQUIP:
	//default:
	//	return NULL;
	//}//

	INT iMaxEnableGemCount = m_pExtraDefine->m_EnableGemCount;
	return iMaxEnableGemCount;

}

//克隆详细信息
VOID CObject_Item_Equip::Clone(const CObject_Item * pItemSource)
{
	ClonetExtraInfo((CObject_Item_Equip*)pItemSource);
	CObject_Item::Clone(pItemSource);
}

//设置详细解释
VOID CObject_Item_Equip::SetExtraInfo(const _ITEM * pItemInfo)
{
	if(!m_pExtraDefine)
		m_pExtraDefine = new EXTRA_DEFINE;

	m_pExtraDefine->m_CurDurPoint = pItemInfo->GetEquipData()->m_CurDurPoint;
	m_pExtraDefine->m_MaxDurPoint = pItemInfo->GetEquipData()->m_MaxDurPoint;
	m_pExtraDefine->m_nLevelNeed  = pItemInfo->GetEquipData()->m_NeedLevel;
//	m_pExtraDefine->m_nFrogLevel  = pItemInfo->GetEquipData()->m_Level;
	m_pExtraDefine->m_nRepairFailureTimes  = pItemInfo->GetEquipData()->m_FaileTimes;
	m_pExtraDefine->m_nSellPrice  = pItemInfo->GetEquipData()->m_BasePrice;

	// 可以镶嵌的宝石个数
	m_pExtraDefine->m_EnableGemCount = pItemInfo->GetEquipData()->m_GemMax;

	AxTrace(1, 0, "%d", pItemInfo->m_Equip.m_GemMax);
	
	// 得到鉴定信息。
	if(pItemInfo->GetItemIdent())
	{
		// 已经鉴定
		m_EquipAttrib = CObject_Item_Equip::EQUIP_ATTRIB_IDENTIFY;
	}
	else
	{
		// 未鉴定
		m_EquipAttrib = CObject_Item_Equip::EQUIP_ATTRIB_UNIDENTIFY;
	}

	// 得到绑定信息。
	if(pItemInfo->GetItemBind())
	{
		m_pExtraDefine->M_nEquipBindInfo = BINDED;
	}
	else
	{
		if(Rule(RULE_PICKBIND))
			m_pExtraDefine->M_nEquipBindInfo = GETUP_BIND;
		else if(Rule(RULE_EQUIPBIND))
			m_pExtraDefine->M_nEquipBindInfo = EQUIP_BIND;

	}

	const INT BLUE_ATT_START	= 0;
	const INT BLUE_ATT_END		= 53;

	// 清空蓝装备信息
	m_pExtraDefine->m_vBlueEquipAttributes.clear();
	for(INT i=0; i<pItemInfo->GetEquipData()->m_AttrCount; i++)
	{
		m_pExtraDefine->m_vBlueEquipAttributes.push_back(pItemInfo->GetEquipData()->m_pAttr[i]);

		if(	i < (INT)m_pExtraDefine->m_vBlueEquipAttributes.size() - (INT)m_pExtraDefine->m_vEquipAttachGem.size() &&
			pItemInfo->GetEquipData()->m_pAttr[i].m_AttrType >= BLUE_ATT_START && 
			pItemInfo->GetEquipData()->m_pAttr[i].m_AttrType <= BLUE_ATT_END)
		{
			AxTrace(3, 0, "%d", i);
			
			CHAR szAtt[MAX_PATH] = {0};

			SetAttri(pItemInfo->GetEquipData()->m_pAttr[i], szAtt);
			AxTrace(0, 1, "%s", szAtt);
		}
////////////////////////
	}

	// 清空宝石信息
	m_pExtraDefine->m_vEquipAttachGem.clear();
	for(INT i=0; i<pItemInfo->GetEquipData()->m_StoneCount; i++)
	{
		m_pExtraDefine->m_vEquipAttachGem.push_back(pItemInfo->GetEquipData()->m_pGemInfo[i]);
	}

	SetNumber(pItemInfo->GetItemCount());
	SetManufacturer(pItemInfo);
}

//克隆额外信息
VOID CObject_Item_Equip::ClonetExtraInfo( CObject_Item_Equip * pItemSource)
{
	if(!m_pExtraDefine)
		m_pExtraDefine = new EXTRA_DEFINE;

	EXTRA_DEFINE* pNewExtraDefine = (EXTRA_DEFINE*)(pItemSource->GetExtraInfo());

	m_pExtraDefine->m_CurDurPoint = pNewExtraDefine->m_CurDurPoint;
	m_pExtraDefine->m_MaxDurPoint = pNewExtraDefine->m_MaxDurPoint;
	m_pExtraDefine->m_nLevelNeed  = pNewExtraDefine->m_nLevelNeed;
	//	m_pExtraDefine->m_nFrogLevel  = pItemInfo->GetEquipData()->m_Level;
	m_pExtraDefine->m_nRepairFailureTimes  = pNewExtraDefine->m_nRepairFailureTimes;
	m_pExtraDefine->m_nSellPrice  = pNewExtraDefine->m_nSellPrice;

	m_pExtraDefine->M_nEquipBindInfo = pNewExtraDefine->M_nEquipBindInfo;

	for(UINT i=0; i<pNewExtraDefine->m_vEquipAttachGem.size(); i++)
	{
		m_pExtraDefine->m_vEquipAttachGem.push_back(pNewExtraDefine->m_vEquipAttachGem[i]);
	}

	for(UINT i=0; i<pNewExtraDefine->m_vBlueEquipAttributes.size(); i++)
	{
		m_pExtraDefine->m_vBlueEquipAttributes.push_back(pNewExtraDefine->m_vBlueEquipAttributes[i]);
	}

	SetNumber(pItemSource->GetNumber());

	m_Quality = (EQUIP_QUALITY)(pItemSource->GetEquipQuantity());
	m_theBaseDef = pItemSource->GetEquipBaseDefine();

	// 装备是否鉴定过属性。2006－4－10。
	m_EquipAttrib = pItemSource->m_EquipAttrib;
}

//得到套装编号
INT CObject_Item_Equip::GetSetID(VOID) const
{
	switch(m_Quality)
	{
	case WHITE_EQUIP:
		return (m_theBaseDef.pDefineAsWhite->nSetID);

	case GREEN_EQUIP:
		return (m_theBaseDef.pDefineAsGreen->nSetID);

	case BLUE_EQUIP:
		return (m_theBaseDef.pDefineAsBlue->nSetID);

	case INVALID_EQUIP:
	default:
		return -1;
	}
}

LPCTSTR CObject_Item_Equip::GetExtraDesc(VOID)
{
	if(!m_pExtraDefine) 
	{
		return NULL;
	}

	switch(m_pExtraDefine->m_nEquipQulity)
	{
	case WHITE_EQUIP:
		{
			// 得到白装的描述信息
			if(m_theBaseDef.pDefineAsWhite)
			{
				m_strExtraDesc = m_theBaseDef.pDefineAsWhite->szDesc;
			}
			else
			{
				m_strExtraDesc = "";
			}
			break;
		}
	case GREEN_EQUIP:
		{
			// 得到绿装的描述信息
			if(m_theBaseDef.pDefineAsGreen)
			{
				m_strExtraDesc = m_theBaseDef.pDefineAsGreen->szDesc;
			}
			else
			{
				m_strExtraDesc = "";
			}
			break;
		}

	case BLUE_EQUIP:
		{
			// 得到兰装的描述信息.
			if(m_theBaseDef.pDefineAsBlue)
			{
				m_strExtraDesc = m_theBaseDef.pDefineAsBlue->szDesc;
			}
			else
			{
				m_strExtraDesc = "";
			}
			break;
		}
	}
	
	return m_strExtraDesc.c_str();
}

//设置装备当前耐久度对于一个已经生成的装备，只有这个值会被改变
VOID	CObject_Item_Equip::SetDur(INT Dur)
{
	m_pExtraDefine->m_CurDurPoint = Dur;
}
//获得当前耐久度
INT		CObject_Item_Equip::GetDur(VOID) const
{
	return	m_pExtraDefine->m_CurDurPoint;
}
//获得最大耐久度
INT		CObject_Item_Equip::GetMaxDur(VOID) const
{
	return	m_pExtraDefine->m_MaxDurPoint;
}

//获得最大修理次数
INT		CObject_Item_Equip::GetMaxRepair(VOID) const
{
	if(m_Quality == WHITE_EQUIP)
	{
		return	m_theBaseDef.pDefineAsBlue->nBMaxRepair;
	}
	else if(m_Quality == GREEN_EQUIP)
	{
		return m_theBaseDef.pDefineAsGreen->nBMaxRepair;
	}
	else if(m_Quality == BLUE_EQUIP)
	{
		return m_theBaseDef.pDefineAsBlue->nBMaxRepair;
	}

	return 0;
}


VOID CObject_Item_Equip::GetAllGreenAttribute(STRING& strValue)
{
	const INT GREEN_ATT_START	= 0;
	const INT GREEN_ATT_END		= 53;
	if(!m_pExtraDefine)
	{
		strValue = "";
	}
	
	CHAR szAttachAtt[1024] = {0};
	for(INT i=0; i<(INT)m_pExtraDefine->m_vEquipAttributes.size(); i++)
	{
		const _ITEM_ATTR& att = m_pExtraDefine->m_vEquipAttributes[i];
	
		if((att.m_AttrType >= 0)&&(att.m_AttrType <= 53))
		{
			CHAR szAtt[MAX_PATH] = {0};
			SetAttri(att, szAtt);
			strncat(szAttachAtt, szAtt, 1024);
		}
	}

	strValue = szAttachAtt;
}


VOID CObject_Item_Equip::GetAllWhiteAttribute(STRING& strValue)
{
	const INT WRITE_ATT_START	= 54;
	const INT WRITE_ATT_END		= 65;
	if(!m_pExtraDefine)
	{
		strValue = "";
	}
	
	CHAR szAttachAtt[1024] = {0};
	for(INT i=0; i<(INT)m_pExtraDefine->m_vEquipAttributes.size(); i++)
	{
		const _ITEM_ATTR& att = m_pExtraDefine->m_vEquipAttributes[i];
		if((att.m_AttrType >= 54)&&(att.m_AttrType <= 65))
		{
			CHAR szAtt[MAX_PATH] = {0};
			SetAttri(att, szAtt);
			strncat(szAttachAtt, szAtt, 1024);
		}
	}

	strValue = szAttachAtt;
}

VOID CObject_Item_Equip::GetAllBlueAttribute(STRING& strValue)
{
	if(!m_pExtraDefine) strValue = "";
	
	const INT BLUE_ATT_START	= 0;
	const INT BLUE_ATT_END		= 53;

	CHAR szAttachAtt[1024] = {0};
	INT gem_attr_count = 0;
	/*
	for(INT iGemIndex=0; iGemIndex < (INT)m_pExtraDefine->m_vEquipAttachGem.size(); iGemIndex++)
	{
		//打开宝石数据表
		DBC_DEFINEHANDLE(s_pItem_Gem, DBC_ITEM_GEM);
		//搜索纪录
		UINT iGemTableIndex = -1;
		iGemTableIndex = m_pExtraDefine->m_vEquipAttachGem[iGemIndex].m_GemType;
		const _DBC_ITEM_GEM* pGem = (const _DBC_ITEM_GEM*)s_pItem_Gem->Search_Index_EQU(iGemTableIndex);
		for(INT i = 0;i < 60; i++)
		{
			if(	pGem->nAttr[i] > -1 )
			{
				gem_attr_count++;
			}
		}
	}
	*/
/*
	for(INT i=0; i<(INT)m_pExtraDefine->m_vBlueEquipAttributes.size(); i++)
	{
		const _ITEM_ATTR& att = m_pExtraDefine->m_vBlueEquipAttributes[i];

		CHAR szAtt[MAX_PATH] = {0};

//		INT find = 0;
		if(att.m_AttrType >= BLUE_ATT_START && att.m_AttrType <= BLUE_ATT_END)
		{
			//_snprintf(szAtt, MAX_PATH, "#{%s}%d\n", 
			//	g_szAttachAttName[att.m_AttrType], att.m_Value);//

			for(INT iGemIndex=0; iGemIndex < (INT)m_pExtraDefine->m_vEquipAttachGem.size(); iGemIndex++)
			{
				//打开宝石数据表
				DBC_DEFINEHANDLE(s_pItem_Gem, DBC_ITEM_GEM);
				//搜索纪录
				UINT iGemTableIndex = -1;
				iGemTableIndex = m_pExtraDefine->m_vEquipAttachGem[iGemIndex].m_GemType;
				const _DBC_ITEM_GEM* pGem = (const _DBC_ITEM_GEM*)s_pItem_Gem->Search_Index_EQU(iGemTableIndex);

				if(	att.m_AttrType < 60 &&
					pGem->nAttr[att.m_AttrType] > -1 && 
					pGem->nAttr[att.m_AttrType] == att.m_Value.m_Value)
				{
					find = 1;
					break;
				}
			}
			if(!find)
			SetAttri(att, szAtt);
			
		}

		if(!find)
		strncat(szAttachAtt, szAtt, 1024);
		}
		strValue = szAttachAtt;
	}//
*/
	for(INT i=0; i<(INT)m_pExtraDefine->m_vBlueEquipAttributes.size() - (INT)m_pExtraDefine->m_vEquipAttachGem.size(); i++)
	{
		const _ITEM_ATTR& att = m_pExtraDefine->m_vBlueEquipAttributes[i];

		CHAR szAtt[MAX_PATH] = {0};
		if(att.m_AttrType >= BLUE_ATT_START && att.m_AttrType <= BLUE_ATT_END)
		{
			//_snprintf(szAtt, MAX_PATH, "#{%s}%d\n", 
			//	g_szAttachAttName[att.m_AttrType], att.m_Value);//
			SetAttri(att, szAtt);
			
		}

		strncat(szAttachAtt, szAtt, 1024);
	}//

	strValue = szAttachAtt;
}

INT CObject_Item_Equip::GetGemCount()
{
	if(NULL == m_pExtraDefine)
	{
		return 0;
	}
	
	INT iGemCount = (INT)m_pExtraDefine->m_vEquipAttachGem.size();
	return iGemCount;
}

// 得到宝石的图标
VOID CObject_Item_Equip::GetGemIcon(INT iGemIndex, STRING& strIconName)
{
	if(NULL == m_pExtraDefine)
	{
		strIconName = _T("");
		return;
	}
	
	INT iGemCount = (INT)m_pExtraDefine->m_vEquipAttachGem.size();
	if(iGemIndex >= iGemCount)
	{
		strIconName = _T("");
		return;
	}

	//打开宝石数据表
	DBC_DEFINEHANDLE(s_pItem_Gem, DBC_ITEM_GEM);
	//搜索纪录
	UINT iGemTableIndex = 0;
	iGemTableIndex = m_pExtraDefine->m_vEquipAttachGem[iGemIndex].m_GemType;
	const _DBC_ITEM_GEM* pGem = (const _DBC_ITEM_GEM*)s_pItem_Gem->Search_Index_EQU(iGemTableIndex);
	
	if(!pGem)
	{
		strIconName = _T("");
		return;
	}

	strIconName = pGem->szIcon;
	return;
	
}

// 得到宝石的附加功能
VOID CObject_Item_Equip::GetGemExtAttr(INT iGemIndex, STRING& strGemAttr)
{
	//打开宝石数据表
	DBC_DEFINEHANDLE(s_pItem_Gem, DBC_ITEM_GEM);
	//搜索纪录
	UINT iGemTableIndex = -1;
	iGemTableIndex = m_pExtraDefine->m_vEquipAttachGem[iGemIndex].m_GemType;
	const _DBC_ITEM_GEM* pGem = (const _DBC_ITEM_GEM*)s_pItem_Gem->Search_Index_EQU(iGemTableIndex);


	TCHAR bufInfo[1024]={0};
	for(int i = 0; i < 60; i++)
	{
		if(pGem->nAttr[i] > -1)
		{
			memset(bufInfo, 0, sizeof(bufInfo));
			_stprintf(bufInfo, "#{%s} +%d\n", g_szAttachAttName[i], pGem->nAttr[i]);
			strGemAttr += bufInfo;
		}
	}
}



STRING CObject_Item_Equip::GetAttributeValue(LPCTSTR szValueName)
{
	if(!szValueName) return "";

	CHAR szTemp[1024];

	if(_stricmp(szValueName, ITEMATTRIBUTE_LEVEL) == 0)
	{
		if(m_pExtraDefine)
		{
			_snprintf(szTemp, 1024, "%d", m_pExtraDefine->m_nLevelNeed);
			return szTemp;
		}
	}
	else if(_stricmp(szValueName, ITEMATTRIBUTE_DAM) == 0)
	{
		if(m_pExtraDefine)
		{
			_snprintf(szTemp, 1024, "%d", m_pExtraDefine->m_CurDurPoint);
			return szTemp;
		}
	}
	else if(_stricmp(szValueName, ITEMATTRIBUTE_DAMMAX) == 0)
	{
		if(m_pExtraDefine)
		{
			_snprintf(szTemp, 1024, "%d", m_pExtraDefine->m_MaxDurPoint);
			return szTemp;
		}
	}
	else if(_stricmp(szValueName, ITEMATTRIBUTE_PRICE) == 0)
	{
		if(m_pExtraDefine)
		{
			_snprintf(szTemp, 1024, "%d", m_pExtraDefine->m_nSellPrice);
			return szTemp;
		}
	}
	else if(_stricmp(szValueName, ITEMATTRIBUTE_REPAIRNUM) == 0)
	{
		if(m_pExtraDefine)
		{
			_snprintf(szTemp, 1024, "%d", m_pExtraDefine->m_nRepairFailureTimes);
			return szTemp;
		}
	}
	else if(_stricmp(szValueName, ITEMATTRIBUTE_WHITE_ATT) == 0)
	{
		if(m_pExtraDefine)
		{
			STRING strValue;
			GetAllWhiteAttribute(strValue);
			return strValue;
		}
	}
	else if(_stricmp(szValueName, ITEMATTRIBUTE_BLUE_ATT) == 0)
	{
		if(m_pExtraDefine)
		{
			STRING strValue;
			GetAllBlueAttribute(strValue);
			return strValue;
		}
	}


	return CObject_Item::GetAttributeValue(szValueName);
}

// 得到玩家使用这个物品需要的等级
INT	CObject_Item_Equip::GetNeedLevel()
{
	//EXTRA_DEFINE* pInfo = (EXTRA_DEFINE*)GetExtraInfo();
	//if(pInfo)
	//{
	//	return pInfo->m_nLevelNeed;
	//}//

	
	switch(m_Quality)
	{
	case WHITE_EQUIP:
		return m_theBaseDef.pDefineAsWhite->nLevelRequire;

	case GREEN_EQUIP:
		return m_theBaseDef.pDefineAsGreen->nLevelRequire;

	case BLUE_EQUIP:
		return m_theBaseDef.pDefineAsBlue->nLevelRequire;

	default:
		return 0;
	}
	return 0;
}

INT CObject_Item_Equip::GetNeedJob(VOID) const
{
	switch(m_Quality)
	{
	case WHITE_EQUIP:
		return m_theBaseDef.pDefineAsWhite->nJob;

	case GREEN_EQUIP:
		return m_theBaseDef.pDefineAsGreen->nJob;

	case BLUE_EQUIP:
		return m_theBaseDef.pDefineAsBlue->nJob;

	default:
		return -1;
	}
	return -1;
}

// 得到物品耐久度
INT	CObject_Item_Equip::GetItemDur()
{
	
	return GetDur();
	
}

// 得到物品最大耐久度
INT	CObject_Item_Equip::GetItemMaxDur()
{
	
	return GetMaxDur();
	
}

// 得到物品的修理次数 
INT	CObject_Item_Equip::GetItemRepairCount()
{
	return m_pExtraDefine->m_nRepairFailureTimes;
}

// 得到物品的绑定信息 
INT	CObject_Item_Equip::GetItemBindInfo()
{
	// -1 无绑定信息
	if(m_pExtraDefine)
	{
		return m_pExtraDefine->M_nEquipBindInfo;
	}
	else
	{
		return -1;
	}
}

// 得到物品卖给npc的价格
INT	CObject_Item_Equip::GetItemPrice()
{

	EXTRA_DEFINE* pInfo = (EXTRA_DEFINE*)GetExtraInfo();
	if(pInfo)
	{
		return pInfo->m_nSellPrice;
	}
	return -1;
}

// 得到物品的制作人
LPCTSTR	CObject_Item_Equip::GetManufacturer()
{
	return _T("");//GetManufacturer();
}

// 得到白色属性
LPCTSTR	CObject_Item_Equip::GetBaseWhiteAttrInfo()
{
	STRING strWhiteValue("");
	GetAllWhiteAttribute(strWhiteValue);

	if(_T("") == strWhiteValue)
	{
		return _T("");
	}

	static TCHAR bufInfo[256];
	memset(bufInfo, 0, sizeof(bufInfo));
	_stprintf(bufInfo, _T("%s"), strWhiteValue.c_str());
	return bufInfo;
}

// 得到扩展蓝色属性
LPCTSTR	CObject_Item_Equip::GetExtBlueAttrInfo()
{
	STRING strValue("");
	GetAllBlueAttribute(strValue);

	if(_T("") == strValue)
	{
		return _T("");
	}

	static TCHAR bufInfo[1024];
	memset(bufInfo, 0, sizeof(bufInfo));
	_stprintf(bufInfo, _T("%s"), strValue.c_str());
	return bufInfo;
}
//得到所有套装属性. 
VOID		CObject_Item_Equip::GetAllSuitAttribute(STRING& strValue)
{
	const INT SUIT_ATT_START	= 0;
	const INT SUIT_ATT_END		= 53;
	if(!m_pExtraDefine)
	{
		strValue = "";
		return;
	}
	int nID = -1;
	int nType = GetEquipQuantity();
	switch( nType )
	{
	case PURPLE_EQUIP:
	case GREEN_EQUIP:
		{
			nID = GetEquipBaseDefine().pDefineAsGreen->nSetID;
			break;
		}
	case BLUE_EQUIP:
		{
			nID = GetEquipBaseDefine().pDefineAsBlue->nSetID;
		}
		break;
	case WHITE_EQUIP:
		nID = GetEquipBaseDefine().pDefineAsWhite->nSetID;
		break;
	default:
		strValue = "";
		return ;
	}

	CHAR szAttachAtt[1024] = {0};
	DBC_DEFINEHANDLE( s_pItem_SuitEquip, DBC_ITEM_SETATT );
	for(INT i=0; i<(INT)s_pItem_SuitEquip->GetRecordsNum(); i++)
	{
		const _DBC_ITEM_SETATT* pSuitEquip = (const _DBC_ITEM_SETATT*)s_pItem_SuitEquip->Search_LineNum_EQU((UINT)i);

		if( pSuitEquip )
		{
			if( pSuitEquip->nID == nID )
			{
				for( int j = 0; j < 61; j ++ )
				{
					if( pSuitEquip->nAtt[ j ] == -1 )
						continue;
					_ITEM_ATTR att;
					att.m_AttrType = j;
					att.m_Value.m_Value = pSuitEquip->nAtt[ j ];
					CHAR szAtt[MAX_PATH] = {0};
					SetAttri(att, szAtt);
					strncat(szAttachAtt, szAtt, 1024);
				}
				break;
			}
		}
	}

	strValue = szAttachAtt;
}
LPCTSTR			CObject_Item_Equip::GetSuitAttrInfo()
{
	STRING strValue("");
	GetAllSuitAttribute(strValue);

	if(_T("") == strValue)
	{
		return _T("");
	}

	static TCHAR bufInfo[1024];
	memset(bufInfo, 0, sizeof(bufInfo));
	_stprintf(bufInfo, _T("%s"), strValue.c_str());
	return bufInfo;
}

// 得到扩展绿色属性
LPCTSTR	CObject_Item_Equip::GetGreenAttrInfo()
{
	STRING strValue("");
	GetAllGreenAttribute(strValue);

	if(_T("") == strValue)
	{
		return _T("");
	}

	static TCHAR bufInfo[1024];
	memset(bufInfo, 0, sizeof(bufInfo));
	_stprintf(bufInfo, _T("%s"), strValue.c_str());
	return bufInfo;
}

// 得到装备的属性(白, 绿, 蓝)
	// 得到装备的属性(白, 绿, 蓝)
CObject_Item_Equip::EQUIP_QUALITY CObject_Item_Equip::GetEquipQuantity()
{
	return m_Quality;
}

// 格式化属性字符串
void CObject_Item_Equip::SetAttri(const _ITEM_ATTR& att, char* const pszBuf)
{
	switch(att.m_AttrType)
		{
		case 1:
		//case 2:
		case 4:
		case 20:
		case 21:
		case 23:
		case 24:
		case 27:
		case 28:
		case 30:
		case 31:
		case 33:
		case 34:
		case 39:
			{
				if(att.m_Value.m_Value > 0)
				{
					_snprintf(
								pszBuf, 
								MAX_PATH, 
								"#{%s}+%d%%\n", 
								g_szAttachAttName[att.m_AttrType], 
								att.m_Value.m_Value
								);
				}
				else
				{
					_snprintf(
								pszBuf, 
								MAX_PATH, 
								"#{%s}-%d%%\n", 
								g_szAttachAttName[att.m_AttrType], 
								att.m_Value.m_Value
								);
				}
				break;
			}
		case 38:
			{
				_snprintf(
							pszBuf, 
							MAX_PATH, 
							"%d%%#{%s}\n", 
							att.m_Value.m_Value,
							g_szAttachAttName[att.m_AttrType]
							);
				break;
			}
		case 40:
		case 41:
		case 49:
		case 50:
			{
			
				_snprintf(
							pszBuf, 
							MAX_PATH, 
							"#{%s}%d%%\n", 
							g_szAttachAttName[att.m_AttrType], 
							att.m_Value.m_Value
							);
			
				break;
			}

		case 8:
		case 11:
		case 14:
		case 17:
		case 25:
		case 32:
			{

				if(att.m_Value.m_Value > 0)
				{
					_snprintf(
								pszBuf, 
								MAX_PATH, 
								"#{%s}-%d%%\n", 
								g_szAttachAttName[att.m_AttrType], 
								att.m_Value.m_Value
								);
				}
				else
				{
					_snprintf(
								pszBuf, 
								MAX_PATH, 
								"#{%s}+%d%%\n", 
								g_szAttachAttName[att.m_AttrType], 
								att.m_Value.m_Value
								);
				}
				break;

			}
	
		default:
			{
				if(att.m_Value.m_Value > 0)
				{
					_snprintf(
								pszBuf, 
								MAX_PATH, 
								"#{%s}+%d\n", 
								g_szAttachAttName[att.m_AttrType], 
								att.m_Value.m_Value
								);
				}
				else
				{
					_snprintf(
								pszBuf, 
								MAX_PATH, 
								"#{%s}-%d\n", 
								g_szAttachAttName[att.m_AttrType], 
								att.m_Value.m_Value
								);
				}
				break;
			}

		}
}

CObject_Item_Equip::EQUIP_ATTRIB	CObject_Item_Equip::GetEquipAttrib()
{
	if(GetEquipQuantity() != BLUE_EQUIP) return EQUIP_ATTRIB_IDENTIFY;

	return m_EquipAttrib;
}

// 得到物品表中quality一列2006－3－30
INT	CObject_Item_Equip::GetItemTableQuality(VOID)
{
	switch(m_Quality)
	{
	case WHITE_EQUIP:
		return m_theBaseDef.pDefineAsWhite->nQuality;

	case GREEN_EQUIP:
		return m_theBaseDef.pDefineAsGreen->nQuality;

	case BLUE_EQUIP:
		return m_theBaseDef.pDefineAsBlue->nQuality;

	default:
		return 0;
	}

	return 0; 
}

// 得到物品表中type一列2006－3－30
INT	CObject_Item_Equip::GetItemTableType(VOID)
{
	switch(m_Quality)
	{
	case WHITE_EQUIP:
		return m_theBaseDef.pDefineAsWhite->nType;

	case GREEN_EQUIP:
		return m_theBaseDef.pDefineAsGreen->nType;

	case BLUE_EQUIP:
		return m_theBaseDef.pDefineAsBlue->nType;

	default:
		return 0;
	}

	return 0; 
}

// 是否是武器
BOOL CObject_Item_Equip::IsWeapon()
{
	INT  iType = GetItemTableType();

	if((iType > 5)||(iType < 0))
	{
		return FALSE;
	}

	return TRUE;
}

	// 是否是防俱
BOOL CObject_Item_Equip::IsDefence()
{
	INT  iType = GetItemTableType();

	if((iType > 13)||(iType < 10))
	{
		return FALSE;
	}

	return TRUE;
}

	// 是否是装饰物
BOOL CObject_Item_Equip::IsAppend()
{
	INT  iType = GetItemTableType();

	if((iType > 22)||(iType < 20))
	{
		return FALSE;
	}

	return TRUE;
}

// 得到物品的类型描述2006-4-28
LPCTSTR	CObject_Item_Equip::GetItemTableTypeDesc()
{
	switch(m_Quality)
	{
	case WHITE_EQUIP:
		return m_theBaseDef.pDefineAsWhite->szTypeDesc;

	case GREEN_EQUIP:
		return m_theBaseDef.pDefineAsGreen->szTypeDesc;

	case BLUE_EQUIP:
		return m_theBaseDef.pDefineAsBlue->szTypeDesc;

	default:
		return "无类型描述";
	}

	return "无类型描述";
}

// 获得装备的修理价格 (只关系耐久度，没有考虑修理者的系数)
INT CObject_Item_Equip::GetRepairPrice()
{
	/*  新的修理费用公式
	修理费用计算：
	V：物品的标准价格，查询道具表
	L：最大耐久
	x：当前耐久，0<=x<=L
	y：修理费用
	公式：
	y=a*(1-x/L)*V，a为参数暂定a=1，建议开放
	*/

	//INT V = 0;
	//INT L = 0;
	//INT x = 0;

	//switch(m_Quality)
	//{
	//case WHITE_EQUIP:
	//	V = (INT)(m_theBaseDef.pDefineAsWhite->nBasePrice);
	//	break;
	//case GREEN_EQUIP:
	//case PURPLE_EQUIP:
	//	V = (INT)(m_theBaseDef.pDefineAsGreen->nBasePrice);
	//	break;
	//case BLUE_EQUIP:
	//	V = (INT)(m_theBaseDef.pDefineAsBlue->nBasePrice);
	//	break;
	//case INVALID_EQUIP:
	//default:
	//	V =  0;
	//	break;
	//}
	//L = this->GetItemMaxDur();
	//x = this->GetItemDur();
	//if( L==0 )
	//	return 0;
	//return (INT)((1 - (FLOAT)x/(FLOAT)L) * V) ;

	// --------------------------------------------------------------------------------------------
	//  2006,8,8 更换为使用最新的修理价格计算方式，需要根据装备的等级×相应的修理价格系数

/*	INT	nMax = this->GetItemMaxDur();
	INT nCur = this->GetItemDur();

	INT nNeedLevel = GetNeedLevel();

	if(nNeedLevel <= 0 || nNeedLevel > 100 || nMax == nCur)
	{
		return 0;
	}

	INT nIndex = 1 + (INT)(nNeedLevel / 10);

	// 从表中读取相应的价格系数
	DBC_DEFINEHANDLE(s_pRepairPriceDBC, DBC_ITEM_REPAIR_RATE);

	const _DBC_ITEM_REPAIR_RATE* pDef = (const _DBC_ITEM_REPAIR_RATE*)s_pRepairPriceDBC->Search_LineNum_EQU(nIndex-1);

	INT nMoneySpent = 0;

	if(HEQUIP_WEAPON == GetItemType())		// 武器
	{
		nMoneySpent = (INT)( (nMax-nCur) * 160 * pDef->fWeaponRepair );
		if(nNeedLevel>=40)
		{
			nMoneySpent += 1125+(nIndex-1)*225;
		}
	}
	else									// 防具
	{
		nMoneySpent = (INT)( (nMax-nCur) * 160 * pDef->fDefanceRepair );
		if(nNeedLevel>=40)
		{
			nMoneySpent += 1125+(nIndex-1)*225;
		}
	}
	return nMoneySpent;*/

	return 1;
}
