#include "StdAfx.h"
#include "GMDataBase.h"
#include "TDException.h"
#include "TDUtil.h"
#include "TLBB_DBC.h"
#include "../Global.h"
#include "../Procedure/GameProcedure.h"
#include "TDResourceProvider.h"
#include "../WxRender/ResourceProvider.h"

//============================================================================
CDataBase::CDataBase(INT id)
{
	m_pFileDBC = new DBC::DBCFile(id);
}

CDataBase::~CDataBase()
{
	delete m_pFileDBC;
}

const tDataBase::DBC_FIELD* CDataBase::Search_Index_EQU(INT iIndexValue) const 
{
	return (DBC_FIELD*)m_pFileDBC->Search_Index_EQU(iIndexValue);
}

const tDataBase::DBC_FIELD* CDataBase::Search_LineNum_EQU(INT iLineNum) const
{
	return  (DBC_FIELD*)m_pFileDBC->Search_Posistion(iLineNum, 0); 
}

//查找某列等于指定值的第一行
const tDataBase::DBC_FIELD* CDataBase::Search_First_Column_Equ(INT nColumnNum, const DBC_FIELD& value) const
{
	return (DBC_FIELD*)m_pFileDBC->Search_First_Column_Equ(nColumnNum, value.iValue);
}

//取得ID
INT CDataBase::GetID(VOID) const
{
	return m_pFileDBC->GetID(); 
}

//取得列数
UINT CDataBase::GetFieldsNum(VOID)const
{ 
	return m_pFileDBC->GetFieldsNum(); 
}

//取得记录的条数
UINT CDataBase::GetRecordsNum(VOID)const
{ 
	return m_pFileDBC->GetRecordsNum(); 
}

//直接打开一个dbc文件
BOOL CDataBase::Util_OpenDBCFromTxt(LPCTSTR szFileName, DBC::DBCFile* pDBCFile)
{
	TDAssert(szFileName && pDBCFile);

	char* lpAddress = NULL;
	DWORD dwSize = CGameProcedure::s_pResourceProvider->loadResource(szFileName, lpAddress, "General");
	if( dwSize > 0 )
	{
		BOOL bRet = pDBCFile->OpenFromMemory( lpAddress, lpAddress + dwSize + 1);
		CGameProcedure::s_pResourceProvider->unloadResource( lpAddress, dwSize );
		return bRet;
	}
	return FALSE;
}

BOOL CDataBase::OpenFromTXT(LPCTSTR szFileName)
{
	return Util_OpenDBCFromTxt(szFileName, m_pFileDBC);
}

//============================================================================
WX_IMPLEMENT_DYNAMIC(CDataBaseSystem, GETCLASS(tDataBaseSystem));
CDataBaseSystem* CDataBaseSystem::s_pMe = NULL;

CDataBaseSystem::CDataBaseSystem()
{
	s_pMe = this;
}

CDataBaseSystem::~CDataBaseSystem()
{
}

VOID CDataBaseSystem::Initial(VOID*)
{
	OpenAllDataBase();
}

VOID CDataBaseSystem::Release(VOID)
{
	CloseAllDataBase();
}

VOID CDataBaseSystem::OpenAllDataBase(VOID)
{
	//所有需要打开的数据库定义
	struct _DATABASE_DEFINE
	{
		INT			idDatabase;
		const CHAR*		pFileName;
	};

	_DATABASE_DEFINE  s_dbToLoad[] = 
	{
		{ DBC_CHARACTER_RACE,			"CharRace.txt"			},
		{ DBC_CHARACTER_HAIR_GEO,		"CharHairGeo.txt"		},
		{ DBC_CHARACTER_HEAD_GEO,		"CharHeadGeo.txt"		},
		{ DBC_CHARACTER_ACTION_SET,		"CharActionSet.txt"		},
		{ DBC_CHARACTER_MODEL,			"CharModel.txt"			},
		{ DBC_CHARACTER_MOUNT,			"CharMount.txt"			},
		{ DBC_MISSION_LIST,				"MissionList.txt"		},
		{ DBC_MISSION_DATA,				"MissionData.txt"		},
		{ DBC_MISSION_REWARD,			"MissionReward.txt"		},
		{ DBC_MISSION_PUNISH,			"MissionPunish.txt"		},
		{ DBC_MISSION_DIALOG,			"MissionDialog.txt"		},
		{ DBC_CHARACTER_EFFECT,			"CharEffect.txt"		},
		{ DBC_CHARACTER_EXPLEVEL,		"PlayerExpLevel.txt"	},
		{ DBC_CHARACTER_FACE,			"CharacterFace.txt"		},	// 头像信息
		{ DBC_SYSTEM_COLOR,				"SystemColour.txt"		},
		{ DBC_CREATURE_ATT,				"MonsterAttrExTable.txt"},
		{ DBC_PET_EX_ATT,				"PetAttrTable.txt"		},
		{ DBC_CREATURE_SOUND,			"CreatureSound.txt"		},
		{ DBC_ITEM_VISUAL_CHAR,			"ItemVisualChar.txt"	},
		{ DBC_ITEM_VISUAL_LOCATOR,		"ItemVisualLocator.txt"	},
		{ DBC_ITEM_EQUIP_WHITE,			"WhiteEquipBase.txt"	},
		{ DBC_ITEM_EQUIP_GREEN,			"GreenEquipBase.txt"	},
		{ DBC_ITEM_EQUIP_BLUE,			"BlueEquipBase.txt"		},
		{ DBC_ITEM_MEDIC,				"CommonItem.txt"		},
		{ DBC_ITEM_GEM,					"GemInfo.txt"			},
		{ DBC_ITEM_STOREMAP,			"StoreMapItem.txt"		},
		{ DBC_ITEM_SETATT,				"EquipSetAttr.txt"		},
		{ DBC_SOUND_INFO,				"SoundInfo.txt"			},
		{ DBC_EFFECT_DEFINE,			"EffectDefine.txt"		},
		{ DBC_SKILL_XINFA,				"XinFa_V1.txt"			},
		{ DBC_SKILL_ADDIMPACT,			"SkillAddImpact.txt"	},
		{ DBC_BUFF_IMPACT,				"ImpactSEData_V1.txt"	},
		{ DBC_DIRECTLY_IMPACT,			"ImpactDirectly.txt"	},
		{ DBC_BULLET_DATA,				"Bullet.txt"			},
		{ DBC_SKILL_DATA,				"SkillTemplate_V1.txt"	},
		{ DBC_SKILL_DEPLETE,			"SkillDeplete.txt"		},
		{ DBC_SPECIAL_OBJ_DATA,			"SpecialObjData.txt"	},
		{ DBC_SCENE_DEFINE,				"SceneDefine.txt"		},
		{ DBC_SCENE_POS_DEFINE,			"ScenePosDefine.txt"	},
		{ DBC_UI_LAYOUTDEFINE,			"Interface.txt"			},
		{ DBC_LIFEABILITY_GROWPOINT,	"GrowPoint.txt"			},
		{ DBC_LIFEABILITY_DEFINE,		"Ability.txt"			},
		{ DBC_LIFEABILITY_ITEMCOMPOSE,	"ItemCompound.txt"		},
		{ DBC_LIFEABILITY_PLATFORM,		"Platform.txt"			},
		{ DBC_CODE_CONVERT,				"CodeConvert.txt"		},
		//{ DBC_XINFA_STUDYSPEND,			"XinFaStudySpend_V1.txt"},
		{ DBC_TITLE_NAME,				"CharTitle.txt"			},
		{ DBC_SKILLDATA_V1_DEPLETE,		"SkillData_V1.txt"		},	// 读取skillData_V1.txt;
		{ DBC_MISSION_KIND,				"MissionKind.txt"		},
		{ DBC_STRING_DICT,				"StrDictionary.txt"		},	//字典
		{ DBC_ITEM_RULE,				"ItemRule.txt"			},	//物品规则
		{ DBC_TALK_FILTER,				"StrFilter.txt"			},	//过滤词汇文件
		{ DBC_LV_MAXMONEY,				"LvMaxMoney.txt"		},	//等级金钱对应表
		{ DBC_TALK_ACT,					"ChatMood.txt"			},	//聊天动作命令文件
		{ DBC_TALK_CONFIG,				"ChatConfig.txt"		},	//聊天配置文件
		{ DBC_MONSTER_PAOPAO,			"MonsterPaoPao.txt"		},	//怪物聊天泡泡
		{ DBC_HAIR_STYLE,				"HairStyle.txt"			},	//发型消耗表
		{ DBC_BUILDING_DEINFE,			"BuildingDefine.txt"	},	//城市建筑物定义表
		{ DBC_CITY_BUILDING,			"CityBuilding.txt"		},	//城市建筑物表
		{ DBC_CAMP_AND_STAND,			"CampAndStand.txt"		},	//阵营配置表
		{ DBC_FULLCMP_FILTER,			"FullCompareFilter.txt"	},	//完全匹配过滤表
		{ DBC_PET_LEVELUP,				"PetLevelUpTable.txt"	},	//宠物升级所需经验表
	};

	//打开所有数据库
	INT nNum = sizeof(s_dbToLoad)/sizeof(_DATABASE_DEFINE);
	for(INT i=0; i<nNum; i++)
	{
		CDataBase* pDataBase = new CDataBase(s_dbToLoad[i].idDatabase);

		if(!(pDataBase->OpenFromTXT (s_dbToLoad[i].pFileName)))
		{
			TDThrow("Open %s Error!", s_dbToLoad[i].pFileName);
		}
		if(m_mapDataBaseBuf.find(s_dbToLoad[i].idDatabase) != m_mapDataBaseBuf.end())
		{
			TDThrow("DBC ID Multi define(%s)", s_dbToLoad[i].pFileName);
		}
		m_mapDataBaseBuf.insert(std::make_pair(s_dbToLoad[i].idDatabase, pDataBase));
	}
}

VOID CDataBaseSystem::CloseAllDataBase(VOID)
{
	std::map< INT, CDataBase* >::iterator it;
	for(it = m_mapDataBaseBuf.begin(); it!=m_mapDataBaseBuf.end(); it++)
	{
		delete (CDataBase*)(it->second);
		it->second = NULL;
	}

	m_mapDataBaseBuf.clear();
}

const tDataBase* CDataBaseSystem::GetDataBase(INT idDataBase) const
{
	std::map< INT, CDataBase* >::const_iterator it = m_mapDataBaseBuf.find(idDataBase);
	return (const tDataBase*)it->second;
}

LPCTSTR TransTitleStr(LPCTSTR szTitle)
{
	if(szTitle[0] == '#')
	{//这个字符串是id需要读表转换
		STRINGTOID(szTitle, TBIndex);
		if(TBIndex>0)
		{
			DBC_DEFINEHANDLE(pTitleDBC, DBC_TITLE_NAME);
			const _DBC_TITLE_NAME* pTitle = (const _DBC_TITLE_NAME*)pTitleDBC->Search_LineNum_EQU(TBIndex);
			return pTitle->szTitle;
		}
		return "";
	}
	return szTitle;	
}