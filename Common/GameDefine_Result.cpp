#include "stdafx.h"
// GameDefine_Result.cpp

#include "Type.h"
#include <map>
#include "GameDefine_Result.h"

struct _RESULT_TEXT
{
	OPERATE_RESULT	nCode;
	char			*pszText;
};

static _RESULT_TEXT s_aResultText[] =
{
	{ OR_OK,							"成功"								},
	{ OR_ERROR,							""									},
	{ OR_DIE,							"你已死亡"							},
	{ OR_INVALID_SKILL,					"无效技能"							},
	{ OR_TARGET_DIE,					"目标已死亡"						},
	{ OR_LACK_MANA,						"真气不足"							},
	{ OR_COOL_DOWNING,					"冷却时间未到"						},
	{ OR_NO_TARGET,						"没有目标"							},
	{ OR_INVALID_TARGET,				"无效目标"							},
	{ OR_OUT_RANGE,						"超出范围"							},
	{ OR_IN_RANGE,						"距离太近"							},
	{ OR_NO_PLATFORM,					"没有操作平台"						},
	{ OR_OUT_PLATFORM,					"距离目标太远"						},
	{ OR_NO_TOOL,						"没有操作工具"						},
	{ OR_STUFF_LACK,					"缺少材料"							},
	{ OR_BAG_OUT_OF_SPACE,				"背包缺少空间"						},
	{ OR_WARNING,						"未知警告"							},
	{ OR_BUSY,							"正在做其它事情"					},
	{ OR_MISSION_HAVE,					"已经拥有该任务"					},
	{ OR_MISSION_LIST_FULL,				"任务记录已满"						},
	{ OR_MISSION_NOT_FIND,				"没找到该任务"						},
	{ OR_EXP_LACK,						"熟练度不够"						},
	{ OR_CHAR_DO_NOT_KNOW_THIS_SKILL,	"角色还不会这种技能"				},
	{ OR_NO_SCRIPT,						"目标角色无脚本"					},
	{ OR_NOT_ENOUGH_HP,					"没有足够的生命值"					},
	{ OR_NOT_ENOUGH_RAGE,				"没有足够的怒气值"					},
	{ OR_NOT_ENOUGH_STRIKE_POINT,		"没有足够的连击点"					},
	{ OR_NOT_ENOUGH_ITEM,				"没有足够的道具"					},
	{ OR_NOT_ENOUGH_VIGOR,				"没有足够的活力"					},
	{ OR_NO_LEVEL,						"技能等级不够"						},
	{ OR_CANNOT_UPGRADE,				"宝石无法再升级"					},
	{ OR_FAILURE,						"操作失败"							},
	{ OR_GEM_CONFLICT,					"一件装备上不允许装备同类型的宝石"	},
	{ OR_NEED_IN_FURY_STANCE,			"需要在狂暴状态下使用"				},
	{ OR_INVALID_TARGET_POS,			"无效目标点"						},
	{ OR_GEM_SLOT_LACK,					"无法镶嵌更多宝石"					},
	{ OR_LIMIT_MOVE,					"无法移动"							},
	{ OR_LIMIT_USE_SKILL,				"无法使用技能"						},
	{ OR_INACTIVE_SKILL,				"使用尚未激活的技能"				},
	{ OR_TOO_MUCH_HP,					"生命值必须小于限定数值"			},
	{ OR_NEED_A_WEAPON,					"本技能需要使用一把武器"			},
	{ OR_NEED_HIGH_LEVEL_XINFA,			"你的等级不够"						},
	{ OR_NEED_LEARN_SKILL_FIRST,		"尚未学会这个技能"					},
	{ OR_NEED_MENPAI_FOR_LEVELUP,		"你必须加入一个门派才能继续升级"	},
	{ OR_U_CANNT_DO_THIS_RIGHT_NOW,		"你现在无法这样做"					},
	{ OR_ROBOT_TIMEOUT,					"挂机时间超时"						},
	{ OR_NEED_HAPPINESS,				"你的宠物快乐度不足60，不能出战，需要驯养"},
	{ OR_NEED_HIGH_LEVEL,				"你的等级不够"						},
	{ OR_CANNOT_GETEXP,					"你的宠物已无法得到经验"			},
	{ OR_NEED_LIFE,						"你的宠物寿命为0，无法再召唤，请为其增加寿命"},
	{ OR_EXP_FULL,						"您的经验已经到达上限"				},
	{ OR_TOO_MANY_TRAPS,				"无法设置更多的此类陷阱"			},
	
	{ OR_PET_PLACARD_ISSUE_FAILED,		"发布失败"							},
	{ OR_PET_PLACARD_NOT_APPOINT_PET,	"未指定宠物"						},
	{ OR_PET_PLACARD_ONLY_CAN_ISSUE_ONE,"对不起，同时只能发布一只宠"		},
	{ OR_PET_PLACARD_NEED_PET_TYPE,		"对不起，你的宠好像不是宝宝哦"		},
	{ OR_PET_PLACARD_NEED_PET_LEVEL,	"对不起，你的宠级别还不够哦"		},
	{ OR_PET_PLACARD_NEED_PET_HAPPINESS,"对不起，你的宠快乐值不够高"		},
	{ OR_PET_PLACARD_NEED_PET_LIFE,		"对不起，你的宠寿命不够"			},
	{ OR_NEED_IN_STEALTH_MODE,   		"需要在隐身状态下使用"				},
	{ OR_NOT_ENOUGH_ENERGY,				"没有足够的精力"					},
	{ OR_NEED_IN_SHIELD_MODE,			"需要在护体保护下"					},
	{ OR_PET_DIE,						"你的宠物已经死亡"					},
	{ OR_PET_HADRECALL_NEEDHAPPINESS,	"你的宠物快乐度不足60，已被收回"	},
	{ OR_PET_HADRECALL_NEEDLIFE,		"你的宠物寿命为0，已被收回"			},

	{ OR_CAN_NOT_MOVE_STALL_OPEN,   	"无法在摆摊中移动"					},

	{ OR_GEM_NOT_FIT_EQUIP,				"这种宝石不能镶嵌在这种装备上"		},
	{ OR_CANNOT_ASK_PETDETIAL,			"你无法察看该宠物的信息"			},
	{ OR_VARIANCEPET_CANNOT_RETURNTOCHILD, "变异宠不能进行还童"				},
	{ OR_COMBAT_CANNOT_RETURNTOCHILD,	"出战宠不能进行还童"				},
	{ OR_IMPASSABLE_ZONE,				"不可走区域"						},
	{ OR_NEED_SETMINORPASSWORD,			"请先设置二级密码再重复刚才的操作"	},
	{ OR_NEED_UNLOCKMINORPASSWORD,		"请先对二级密码进行解锁，再重复刚才的操作" },
	{ OR_PETINEXCHANGE_CANNOT_GOFIGHT,	"正在交易的宠物无法出战"			},
	{ OR_HASSPOUSE_CANNOT_RETURNTOCHILD,"已经有配偶的宠物不能还童"			},
	{ OR_CAN_NOT_FIND_SPECIFIC_ITEM,	"缺少指定物品"						},

};

class COResultTextManager
{
protected:
	typedef std::map< INT, const char* >	COResultTextMap;
	COResultTextMap			m_mapOResultText;

public:
	COResultTextManager()
	{
		INT i, nSize;
		nSize = sizeof( s_aResultText ) / sizeof( _RESULT_TEXT );
		for ( i = 0; i < nSize; i++ )
		{
			m_mapOResultText.insert( COResultTextMap::value_type( s_aResultText[i].nCode, s_aResultText[i].pszText ) );
		}
	}

	~COResultTextManager()
	{
		m_mapOResultText.erase(m_mapOResultText.begin(), m_mapOResultText.end());
	}

	LPCSTR GetOperateResultText( OPERATE_RESULT oResult )
	{
		static const char *szOR_ERROR			= "unkown error";

		COResultTextMap::iterator itFind;
		itFind = m_mapOResultText.find(oResult);
		if ( itFind != m_mapOResultText.end() )
		{
			return itFind->second;
		}
		else
		{
			return szOR_ERROR;
		}
	}

};

static COResultTextManager g_OResultTextManager;

const char *GetOResultText( OPERATE_RESULT code )
{
	return g_OResultTextManager.GetOperateResultText( code );
}
