#include "StdAfx.h"
#include "Obj_Item_Gem.h"
#include "TDException.h"
#include "../../Procedure/GameProcedure.h"


const char* g_szGemAttName[] = 
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
//
//
//
//const char* g_szGemAttName[] = 
//{
//	"增加HP的上限",						   //0 				
//	"百分比增加HP的上限",				   //1
//	"加快HP的回复速度",					   //2
//	"增加MP的上限",						   //3
//	"百分比增加MP的上限",				   //4
//	"加快MP的回复速度",					   //5
//	"冰攻击",							   //6
//	"冰抵抗",							   //7
//	"减少冰冻迟缓时间",					   //8
//	"火攻击",							   //9
//	"火抵抗",							   //10
//	"减少火烧持续时间",					   //11
//	"电攻击",							   //12
//	"电抵抗",							   //13
//	"减少电击眩晕时间",					   //14
//	"毒攻击",							   //15
//	"毒抵抗",							   //16
//	"减少中毒时间",						   //17
//	"按百分比抵消所有属性攻击",			   //18
//	"物理攻击",							   //19
//	"按百分比增加物理攻击",				   //20
//	"对装备基础物理攻击百分比加成",		   //21
//	"物理防御",							   //22
//	"按百分比增加物理防御",				   //23
//	"对装备基础物理防御百分比加成",		   //24
//	"按百分比抵消物理伤害",				   //25
//	"魔法攻击",							   //26
//	"按百分比增加魔法攻击",				   //27
//	"对装备基础魔法攻击百分比加成",		   //28
//	"魔法防御",							   //29
//	"按百分比增加魔法防御",				   //30
//	"对装备基础魔法防御百分比加成",		   //31
//	"按百分比抵消魔法伤害",				   //32
//	"攻击速度(两次攻击间隔时间)",		   //33
//	"魔法冷却速度",						   //34
//	"命中",								   //35
//	"闪避",								   //36
//	"会心一击（双倍攻击）的百分比",		   //37
//	"无视对方防御比率",					   //38
//	"移动速度百分比",					   //39
//	"伤害反射",							   //40
//	"伤害由内力抵消",					   //41
//	"力量",								   //42
//	"灵气",								   //43
//	"体制",								   //44
//	"定力",								   //45
//	"身法",								   //46
//	"悟性",								   //47
//	"增加所有的人物一级属性",			   //48
//	"生命偷取",							   //49
//	"内力偷取",							   //50
//	"增加某个技能等级",					   //51
//	"增加所有技能等级",					   //52
//	"特殊技能发动概率",					   //53
//	"基础物理攻击",						   //54
//	"基础魔法攻击",						   //55
//	"基础攻击速度",						   //56
//	"基础物理防御",						   //57
//	"基础魔法防御",						   //58
//	"基础闪避",							   //59
//};//	

CObject_Item_Gem::CObject_Item_Gem(INT id)   
		: CObject_Item(id)				  
{
	m_theBaseDef = NULL;
//	m_HostOfAppend = NULL;
}

CObject_Item_Gem::~CObject_Item_Gem()
{
}

VOID CObject_Item_Gem::AsGem(const _DBC_ITEM_GEM* pGemDefine)
{
	TDAssert(pGemDefine);

	m_theBaseDef = pGemDefine;
	m_nParticularID = ( ( ( ( ( m_theBaseDef->nClass * 100 ) + m_theBaseDef->nQuality ) * 100 ) + m_theBaseDef->nType ) * 1000 ) + m_theBaseDef->nGemIndex;


}

LPCTSTR  CObject_Item_Gem::GetIconName(VOID) const
{
	return m_theBaseDef->szIcon;
}

//设置详细解释
VOID CObject_Item_Gem::SetExtraInfo(const _ITEM * pItemInfo)
{
	TDAssert(pItemInfo);

	//调用基类函数，保存_ITEM结构
	CObject_Item::SetExtraInfo(pItemInfo);

	SetNumber(pItemInfo->GetItemCount());
	SetManufacturer(pItemInfo);
}

LPCTSTR  CObject_Item_Gem::GetExtraDesc(VOID)
{
	//CHAR szTemp[1024];

	//_snprintf(szTemp, 1024, 
	//	"ID=%d\n"
	//	"名称: %s\n"
	//	"-------------------\n"
	//	"%s",
	//	m_ID.m_idOrg.m_uSerial, 
	//	m_theBaseDef->szName,
	//	m_theBaseDef->szDesc);//

	m_strExtraDesc = m_theBaseDef->szDesc;
	return m_strExtraDesc.c_str();
}

VOID	CObject_Item_Gem::Clone(const CObject_Item * pItemSource)
{
	SetNumber(((CObject_Item_Gem*)pItemSource)->GetNumber());
	CObject_Item::Clone(pItemSource);
}

// 得到宝石的等级
INT		CObject_Item_Gem::GetGemLevel()
{
	if(m_theBaseDef)
	{
		return m_theBaseDef->nQuality;
	}

	return 0;

}

// 得到物品卖给npc的价格
INT		CObject_Item_Gem::GetItemPrice()
{
	if(m_theBaseDef)
	{
		return m_theBaseDef->nPrice;
	}

	return 0;
}

// 得到宝石属性信息
LPCTSTR	CObject_Item_Gem::GetGemArribInfo()
{
	
	static TCHAR szAttribInfo[1024];
	TCHAR	szInfo[512];

	memset(szAttribInfo, 0, sizeof(szAttribInfo));
	STRING strAttrib = _T("");

	STRING strTemp = "";
		
	if(m_theBaseDef)
	{
		for(int i = 0; i < 60; i++)
		{
			memset(szInfo, 0, sizeof(szInfo));
			if(-1 != m_theBaseDef->nAttr[i])
			{
				strTemp = NOCOLORMSGFUNC(g_szGemAttName[i]);
				_stprintf(szInfo, _T("%s+%d\n"), strTemp.c_str(), m_theBaseDef->nAttr[i]);
				//_stprintf(szInfo, _T("%s:%d\n"), g_szGemAttName[i], m_theBaseDef->nAttr[i]);
				strAttrib += szInfo;
			}
		}

		_stprintf(szAttribInfo, _T("%s"), strAttrib.c_str());
		return szAttribInfo;
	}

	return szAttribInfo;


}

// 2006-3-30;
// 得到quality信息
INT	CObject_Item_Gem::GetItemTableQuality(VOID)
{
	return m_theBaseDef->nQuality;
}

// 2006-3-30;
// 得到type信息
INT	CObject_Item_Gem::GetItemTableType(VOID)
{
	return m_theBaseDef->nType;
}


// 得到物品的类型描述2006-4-28
LPCTSTR	CObject_Item_Gem::GetItemTableTypeDesc()
{
	return m_theBaseDef->szTypeDesc;
}
