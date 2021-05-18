#ifndef _LOGDEFINE_H_
#define _LOGDEFINE_H_
#include "..\..\Common\Type.h"
#include "..\..\Common\DB_Struct.h"

//物品操作类型
enum	ITEM_OP_TYPE
{
	ITEM_OP_UNKNOW					=	0,		//未知物品拾取
	ITEM_CREATE_FROM_MONSTER		=	10,		//怪物掉落，生成到掉落包
	ITEM_CREATE_SCRIPT_TO_BOX		=	20,		//由脚本创建到掉落包，一般是生长点类型
	ITEM_PICK_FROM_MONSTER			=	30,		//掉落物品拾取,指到角色身上的创建
	ITEM_PICK_FROM_GROWPOINT		=	31,		//采集拾取,指到角色身上的创建
	ITEM_CREATE_FROM_SCRIPT			=	40,		//脚本给与物品
	ITEM_CREATE_FROM_ABILITY_SCRIPT	=	50,		//生活技能相关脚本给与
	ITEM_CREATE_GM					=	60,		//GM命令
	ITEM_CREATE_ABILITY_COMPOUD		=	70,		//宝石合成
	ITEM_CREATE_REMOVE_GEM			=	80,		//宝石卸载的重新生成
	ITEM_CREATE_FROM_SHOP			=   90,		//商店的创建
	ITEM_CREATE_TILED				=	100,	//创建后与原背包物品合并
	ITEM_CREATE_SPLIT				=   110,	//切分物品时物品的创建
	ITEM_EXCHANGE_TOOTHER			=   210,	//物品交易,
	ITEM_STALL_BOX_EXCHANGE			=	211,	//摊位交易
	ITEM_PLAYERSHOP_EXCHANGE		=	212,	//玩家商店
	ITEM_PLAYERSHOP_UP				=	213,	//玩家商店上架
	ITEM_PLAYERSHOP_DOWN			=	214,	//玩家商店下架
	ITEM_SPLIT_BAG_DEST				=	220,	//物品背包拆分目的
	ITEM_SPLIT_BAG_SRC				=	221,	//物品背包拆分源头
	ITEM_SPLIT_BANK_DEST			=	222,	//银行拆分
	ITEM_SPLIT_BANK_SRC				=	223,	//银行拆分
	ITEM_TILED_BAG_DEST				=	224,	//背包物品合并目的
	ITEM_TILED_BAG_SRC				=	225,	//背包物品合并源头
	ITEM_TILED_BANK					=	226,	//银行物品合并


	ITEM_REPAIR						=	310,	//物品修理,
	ITEM_USEGEM_LOST				=	410,	//宝石合成丢失
	ITEM_DISCARD					=	411,	//玩家主动丢弃
	ITEM_GEM_COMPOUND_LOST			=	412,	//宝石合成的物品消耗
	ITEM_ABILITY_COMPOUND_LOST		=	413,	//生活技能物品消耗
	ITEM_SCRIPT_DELETE				=	414,	//从脚本删除物品
	ITEM_CHANGE_HAIRMODEL_LOST		=	415,	//改变发型消耗
	ITEM_DIE_LOST					=	416,	//死亡惩罚
	ITEM_INLAY_LOST					=	417,	//物品镶嵌损失
	ITEM_MACHINE_LOST				=	418,	//机械制造损失
	ITEM_DISCARD_EQUIP				=	419,	//玩家主动丢弃已装备上的物品
	ITEM_DISCARD_BANK				=	420,	//玩家主动丢弃银行装备
	ITEM_USE						=	421,	//玩家主动使用
	ITEM_USE_IDENT_LOST				=	425,	//卷轴使用消耗
	ITEM_BOX_RECYCLE				=	500,	//ItemBox回收
	
};

//钱币操作类型
enum	MONEY_OP_TYPE
{
	MONEY_OP_UNKNOW						=	0,		//未知
	MONEY_BANK_SAVE						=	10,		//银行存
	MONEY_BANK_WITHDRAW					=	20,		//银行取
	MONEY_BANK_SAVERMB					=	21,		//
	MONEY_BANK_WITHDRAWRMB				=	22,		//
	MONEY_EXCHANGE_INCOME				=	30,		//货币交易收入
	MONEY_EXCHANGE_OUTCOME				=	40,		//货币交易支出
	MONEY_EXCHANGE_STALL_INCOME			=	50,		//货币通过摊位交易收入
	MONEY_EXCHANGE_STALL_OUTCOME		=	60,		//货币通过摊位交易支出
	MONEY_EXCHANGE_STALL_TAX			=	61,		//货币通过摊位税收支出
	MONEY_EXCHANGE_PLAYERSHOP_OUTCOME	=	80,		//货币通过玩家商店交易货物支出,钱入玩家商店
	MONEY_BUY_PLAYERSHOP_OUTCOME		=	81,		//货币通过交易玩家商店支出
	MONEY_CREATE_PLAYERSHOP_OUTCOME		=	82,		//货币通过创建玩家商店支出
	MONEY_PLAYERSHOP_SAVE				=	90,		//玩家商店存
	MONEY_PLAYERSHOP_WITHDRAW			=	100,	//玩家商店取
	MONEY_SHOP_COST						=	110,	//商店购买支出
	MONEY_SHOP_SELL						=	111,	//商店出售收入
	MONEY_SHOP_REPAIR					=	120,	//商店修理支出
	MONEY_LEARN_ABILITY_COST			=	121,	//学习生活技能支出	
	MONEY_LEARN_XINFA_COST				=	122,	//学习生活心法支出	
	MONEY_SCRIPT_COST					=	130,	//脚本调用金钱消耗
	MONEY_SCRIPT_INCOME					=	131,	//脚本调用金钱收入
	MONEY_DIE_COST						=	132,	//死亡金钱惩罚
	MONEY_PET_COST						=	133,	//宠物金钱消耗
	MONEY_GM_MODIFY						=	200,	//GM 修改
	MONEY_SHOP_RMB_COST					=	300,	//元宝消费


};
//宠物操作类型
enum	PET_OP_TYPE
{
	PET_OP_UNKNOW				=	0,		//宠物未知操作
	PET_OP_CATCH				=	10,		//宠物捕捉
	PET_OP_SCRIPT_CREATE		=	11,		//脚本创建宠物
	PET_OP_SCRIPT_DELETE		=	12,		//脚本删除宠物
	PET_OP_GENERA				=	20,		//宠物繁殖
	PET_OP_EXCHANGE_OTHER		=	30,		//宠物交易获得
	PET_OP_STALL_EXCHANGE_GET	=	40,//摆摊交易获得
	PET_OP_PLAYERSHOP_EXCHANGE_GET	=	50,		//玩家商店交易获得
	PET_OP_PLAYERSHOP_UP		=	51,		//玩家商店上架
	PET_OP_PLAYERSHOP_DOWN		=	52,		//玩家商店下架
	PET_OP_GM_CREATE			=	60,		//GM 修改添加
	PET_OP_GM_LOST				=	61,		//GM 修改销毁
};

//技能操作类型
enum	SKILL_OP_TYPE
{
	SKILL_OP_UNKNOW						=	0,		//未知技能操作
	SKILL_OP_SCRIPT_STUDY				=	10,		//技能学习
	SKILL_OP_LEVELUP					=	20,		//技能升级
	SKILL_OP_SCRIPT_ABANDON				=	30,		//技能放弃
	SKILL_OP_SCRIPT_CHANGEMENPAI		=	31,		//脚本调用ChangeMenpai
	SKILL_OP_GM_MODIFY					=	40,		//通过GM命令 学习
	
};

//心法操作类型
//enum	XINFA_OP_TYPE
//{
//	XINFA_OP_UNKNOW						=	0,		//未知技能操作
//	XINFA_OP_SCRIPT_STUDY				=	10,		//心法学习
//	XINFA_OP_LEVELUP					=	20,		//心法升级
//	XINFA_OP_SCRIPT_ABANDON				=	30,		//心法放弃
//	XINFA_OP_SCRIPT_CHANGEMENPAI		=	31,		//脚本调用ChangeMenpai
//	XINFA_OP_GM_MODIFY					=	40,		//通过GM命令 学习
//	XINFA_OP_GM_MODIFY_ALL				=	41,		//通过GM命令学习所有
//	
//};

//心法操作类型
enum	ABILITY_OP_TYPE
{
	ABILITY_OP_UNKNOW						=	0,		//未知技能操作
	ABILITY_OP_SCRIPT_STUDY					=	10,		//生活技能学习
	ABILITY_OP_LEVELUP						=	20,		//生活技能升级
	ABILITY_OP_SCRIPT_ABANDON				=	30,		//生活技能放弃
	ABILITY_OP_GM_MODIFY					=	40,		//通过GM命令 学习
	ABILITY_OP_GM_MODIFY_ALL				=	41,		//通过GM命令学习所有

};

//任务操作类型
enum	MISSION_OP_TYPE
{
	MISSION_OP_UNKNOW						=	0,	//未知任务操作
	MISSION_OP_ADDMISSION					=	10,	//增加任务
	MISSION_OP_DELMISSION					=	20,	//删除任务（有可能是完成后删除）
	MISSION_OP_PARAM						=	30,	//任务参数变化
	MISSION_OP_DONEFLAG						=	40,	//任务完成标志变化
	MISSION_OP_DATA							=	50, //任务全局数据变化
};


//Log格式
/*
 *		IOL:World号,Server号,物品序号,角色GUID,对方GUID,操作类型,最后背包位置,场景号,
		相关NPC,XPOS,ZPOS,相关商店号(如果是玩家之间填-1),耐久度，镶嵌孔个数
		属性类型1，属性1。。。属性类型4，属性4
 */
static CHAR*	g_ItemLogFormat =
				"IOL:%d,%d,%d,%d,%d,%X,%X,%d,%d,%d,%d,%d,%.2f,%.2f,%d,%u,%u,%u,%u,%u,%u,%u,%u,%u";

struct ITEM_LOG_PARAM
{
	_ITEM_GUID		ItemGuid;									//物品编号
	UINT			ItemType;									//物品全局编号
	GUID_t			CharGUID;									//自身角色,物品所有者
	GUID_t			TargetGUID;									//目标角色
	ITEM_OP_TYPE	OpType;										//操作类型
	ITEM_OP_TYPE	SecOpType;									//转向操作
	BYTE			BagPos;										//背包位置
	BYTE			Count;										//变化个数
	SceneID_t		SceneID;									//场景编号
	INT				NpcType;									//npc 类型,掉落时填怪物类型
	FLOAT			XPos;										//事件发生位置
	FLOAT			ZPos;										
	INT				ShopGUID;									//相关商店号
	BYTE			GemMax;			    						//最大宝石个数(蓝装必须要)
	_ITEM_ATTR		ItemAttr[MAX_BLUE_ITEM_EXT_ATTR];			//蓝色装备属性

	ITEM_LOG_PARAM()
	{
			CleanUp();
	}

	VOID	CleanUp()
	{
		CharGUID	= -1;
		TargetGUID  = -1;
		OpType		= ITEM_OP_UNKNOW;
		SecOpType   = ITEM_OP_UNKNOW;
		BagPos		= -1;
		Count		=  1;
		SceneID		= -1;
		NpcType		= -1;
		XPos		= 0.0f;
		ZPos		= 0.0f;
		ShopGUID	=-1;
		GemMax		= 0;
		for(INT i=0;i<MAX_BLUE_ITEM_EXT_ATTR;i++)
			ItemAttr[i].CleanUp();
	}

	
};

VOID		SaveItemLog(ITEM_LOG_PARAM* pLogParam);

//Log 格式
/*
*
MOL:参与角色，对象角色，操作类型，货币数量，场景，x位置,z位置
*/

static CHAR*	g_MoneyLogFormat = "MOL:%X,%X,%d,%d,%d,%.2f,%.2f";

struct  MONEY_LOG_PARAM
{
	GUID_t			CharGUID;									//自身角色,money所有者
	GUID_t			TargetGUID;									//目标角色
	MONEY_OP_TYPE	OPType;										//操作类型
	INT				Count;										//数量
	SceneID_t		SceneID;									//场景编号
	FLOAT			XPos;										//事件发生位置
	FLOAT			ZPos;

	MONEY_LOG_PARAM()
	{

	}

	VOID	CleanUp()
	{
		CharGUID	= INVALID_ID;									
		TargetGUID	= INVALID_ID;									
		OPType		= MONEY_OP_UNKNOW;										
		Count		=	0;										
		SceneID		=	-1;									
		XPos		=	0.0f;										
		ZPos		=	0.0f;	
	}
};

VOID		SaveMoneyLog(MONEY_LOG_PARAM* pLogParam);

/*
*	POL:参与角色，对象角色，操作类型，场景，x位置,z位置	,宠物编号
*/
static CHAR*	g_PetLogFormat = "POL:%X,%X,%d,%d,%.2f,%.2f,%u,%u,%d";
struct  PET_LOG_PARAM
{
	GUID_t			CharGUID;									//自身角色,Pet所有者
	GUID_t			TargetGUID;									//目标角色
	PET_OP_TYPE		OPType;										//操作类型
	SceneID_t		SceneID;									//场景编号
	FLOAT			XPos;										//事件发生位置
	FLOAT			ZPos;
	PET_GUID_t		PetGUID;									//宠物GUID
	INT				DataID;										//宠物类型
	
	PET_LOG_PARAM()
	{
		CleanUp();
	}

	VOID	CleanUp()
	{
		CharGUID	= INVALID_ID;									
		TargetGUID	= INVALID_ID;									
		OPType		= PET_OP_UNKNOW;										
		SceneID		=	-1;									
		XPos		=	0.0f;										
		ZPos		=	0.0f;
		PetGUID.m_uHighSection = 0;
		PetGUID.m_uLowSection  = 0;
		DataID		=	-1;
	}
};

VOID		SavePetLog(PET_LOG_PARAM* pLogParam);



/*
*	SOL:参与角色，操作类型，场景，x位置,z位置,技能编号
*/
static CHAR*	g_SkillLogFormat = 
"SOL:%X,%d,%d,%.2f,%.2f,%u,%d,%d";

struct SKILL_LOG_PARAM
{
	GUID_t			CharGUID;									//自身角色,Skill所有者
	SKILL_OP_TYPE	OPType;										//操作类型
	SceneID_t		SceneID;									//场景编号
	FLOAT			XPos;										//事件发生位置
	FLOAT			ZPos;
	SkillID_t		SkillID;									//技能号
	INT				Level;										//级别
	INT				Count;										//技能
	SKILL_LOG_PARAM()
	{
		CleanUp();
	}

	VOID	CleanUp()
	{
		CharGUID	= INVALID_ID;									
		OPType		= SKILL_OP_UNKNOW;										
		SceneID		=	-1;									
		XPos		=	0.0f;										
		ZPos		=	0.0f;
		SkillID		=	-1;
		Level		=	-1;
		Count		=	0;
	}
};

VOID	SaveSkillLog(SKILL_LOG_PARAM* pLogParam);


/*
*	XOL:参与角色，操作类型，场景，x位置,z位置,技能编号
*/
//static CHAR*	g_XinFaLogFormat = 
//"XOL:%X,%d,%d,%.2f,%.2f,%u,%d,%d";
//
//struct XINFA_LOG_PARAM
//{
//	GUID_t			CharGUID;									//自身角色,Skill所有者
//	XINFA_OP_TYPE	OPType;										//操作类型
//	SceneID_t		SceneID;									//场景编号
//	FLOAT			XPos;										//事件发生位置
//	FLOAT			ZPos;
//	ID_t			XinFaID_Nouse;									//技能号
//	INT				Level;										//级别
//	INT				Count;
//	XINFA_LOG_PARAM()
//	{
//		CleanUp();
//	}
//
//	VOID	CleanUp()
//	{
//		CharGUID	=	INVALID_ID;									
//		OPType		=	XINFA_OP_UNKNOW;										
//		SceneID		=	-1;									
//		XPos		=	0.0f;										
//		ZPos		=	0.0f;
//		XinFaID_Nouse	=	-1;
//		Level		=	-1;
//		Count		=	0;
//	}
//};
//
//VOID	SaveXinFaLog(XINFA_LOG_PARAM* pLogParam);


/*
*	XOL:参与角色，操作类型，场景，x位置,z位置,技能编号
*/
static CHAR*	g_AbilityLogFormat = 
"AOL:%X,%d,%d,%.2f,%.2f,%u,%d,%d";

struct ABILITY_LOG_PARAM
{
	GUID_t			CharGUID;									//自身角色,Skill所有者
	ABILITY_OP_TYPE	OPType;										//操作类型
	SceneID_t		SceneID;									//场景编号
	FLOAT			XPos;										//事件发生位置
	FLOAT			ZPos;
	AbilityID_t		AbilityID;									//技能号
	INT				Level;										//级别
	INT				Count;
	ABILITY_LOG_PARAM()
	{
		CleanUp();
	}

	VOID	CleanUp()
	{
		CharGUID	=	INVALID_ID;									
		OPType		=	ABILITY_OP_UNKNOW;										
		SceneID		=	-1;									
		XPos		=	0.0f;										
		ZPos		=	0.0f;
		AbilityID		=	-1;
		Level		=	-1;
		Count		=	0;
	}
};

VOID	SaveAbilityLog(ABILITY_LOG_PARAM* pLogParam);

/*
*	MIL:角色GUID，操作类型，场景，x位置,z位置，任务编号，任务脚本，任务标志
*		参数0，参数1，参数2，参数3，参数4，参数5，参数6，参数7，
*		完成标志索引，完成标志值，全局任务数据索引，全局任务数据值
*/
static CHAR*	g_MissionLogFormat = 
"MIL:%X,%d,%d,%.2f,%.2f,%d,%d,%d,%u,%u,%u,%u,%u,%u,%u,%u,%d,%d,%d,%d";

struct MISSION_LOG_PARAM
{
	GUID_t			CharGUID;									//自身角色,Skill所有者
	MISSION_OP_TYPE	OPType;										//操作类型
	SceneID_t		SceneID;									//场景编号
	FLOAT			XPos;										//事件发生位置
	FLOAT			ZPos;
	MissionID_t		idMission;
	ScriptID_t		idScript;
	BYTE			yFlags;
	UINT			aParam[MAX_MISSION_PARAM_NUM];
	INT				nDoneFlagIndex ;
	INT				nDoneFlagValue ;
	INT				nDataIndex ;
	INT				nDataValue ;

	MISSION_LOG_PARAM( )
	{
		CleanUp( ) ;
	};
	VOID CleanUp( )
	{
		CharGUID		=INVALID_ID;				//自身角色,Skill所有者
		OPType			=MISSION_OP_UNKNOW;			//操作类型
		SceneID			=INVALID_ID;				//场景编号
		XPos			=0.0f;						//事件发生位置
		ZPos			=0.0f;
		idMission		=INVALID_ID;
		idScript		=INVALID_ID;
		yFlags			=0;
		memset(aParam,0,sizeof(UINT)*MAX_MISSION_PARAM_NUM);
		nDoneFlagIndex	=INVALID_INDEX ;
		nDoneFlagValue	=0 ;
		nDataIndex		=INVALID_INDEX ;
		nDataValue		=0 ;
	}
};

VOID	SaveMissionLog(MISSION_LOG_PARAM* pLogParam);

#endif