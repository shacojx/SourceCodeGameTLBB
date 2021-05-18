
#ifndef __GAMEDEFINE_RESULT_H__
#define __GAMEDEFINE_RESULT_H__


// 操作返回(为了区分与WINDOWS的HRESULT不同，所以不用HRESULT来做返回值)
#define OR_SUCCEEDED(x)		(((OPERATE_RESULT)(x)) >= 0)
#define OR_FAILED(x)		(((OPERATE_RESULT)(x)) < 0)
enum OPERATE_RESULT
{
	OR_OK								= 0,	// 成功
	OR_ERROR							= -1,	// 未知错误
	OR_DIE								= -2,	// 你已死亡
	OR_INVALID_SKILL					= -3,	// 无效技能
	OR_TARGET_DIE						= -4,	// 目标已死亡
	OR_LACK_MANA						= -5,	// MANA不足
	OR_COOL_DOWNING						= -6,	// 冷确时间未到
	OR_NO_TARGET						= -7,	// 没有目标
	OR_INVALID_TARGET					= -8,	// 无效目标
	OR_OUT_RANGE						= -9,	// 超出范围
	OR_IN_RANGE							= -10,	// 距离太近
	OR_NO_PLATFORM						= -11,	// 没有操作平台
	OR_OUT_PLATFORM						= -12,	// 离操作平台太远
	OR_NO_TOOL							= -13,	// 没有操作工具
	OR_STUFF_LACK						= -14,	// 缺少材料
	OR_BAG_OUT_OF_SPACE					= -15,	// 背包缺少空间
	OR_WARNING							= -16,	// 理论上这些错误将被客户端过滤，所以如果出现，可能来自于一些外挂
	OR_BUSY								= -17,	// 正在做其它事情
	OR_MISSION_HAVE						= -18,	// 已经拥有该任务
	OR_MISSION_LIST_FULL				= -19,	// 任务记录已满
	OR_MISSION_NOT_FIND					= -20,	// 没找到该任务
	OR_EXP_LACK							= -21,	// 熟练度不够
	OR_CHAR_DO_NOT_KNOW_THIS_SKILL		= -22,	// 角色还不会这种技能
	OR_NO_SCRIPT						= -23,	// 目标角色无脚本
	OR_NOT_ENOUGH_HP					= -24,	// 没有足够的生命值
	OR_NOT_ENOUGH_RAGE					= -25,	// 没有足够的怒气值
	OR_NOT_ENOUGH_STRIKE_POINT			= -26,	// 没有足够的连击点
	OR_NOT_ENOUGH_ITEM					= -27,	// 没有足够的道具
	OR_NOT_ENOUGH_VIGOR					= -28,	// 没有足够的活力
	OR_NO_LEVEL							= -29, 	// 技能等级不够
	OR_CANNOT_UPGRADE					= -30,	// 宝石无法再升级
	OR_FAILURE							= -31,	// 操作失败
	OR_GEM_CONFLICT						= -32,	// 一件装备上不允许装备同类型的宝石
	OR_NEED_IN_FURY_STANCE				= -33,	// 需要在狂暴状态下使用
	OR_INVALID_TARGET_POS				= -34,	// 无效目标点
	OR_GEM_SLOT_LACK					= -35,	// 缺乏宝石插口
	OR_LIMIT_MOVE						= -36,	// 无法移动
	OR_LIMIT_USE_SKILL					= -37,	// 无法使用技能
	OR_INACTIVE_SKILL					= -38,	// 使用尚未激活的技能
	OR_TOO_MUCH_HP						= -39,	// HP大于限定数值
	OR_NEED_A_WEAPON					= -40,	// 需要一把武器
	OR_NEED_HIGH_LEVEL_XINFA			= -41,	// 等级不够
	OR_NEED_LEARN_SKILL_FIRST			= -42,	// 尚未学会此技能
	OR_NEED_MENPAI_FOR_LEVELUP			= -43,	// 你必须拜师进入一个门派才能继续升级
	OR_U_CANNT_DO_THIS_RIGHT_NOW		= -44, 	// 你现在无法这样做
	OR_ROBOT_TIMEOUT					= -45,  // 挂机时间超时
	OR_NEED_HAPPINESS					= -46,  // 你的宠物快乐度不足60，不能出战，需要驯养
	OR_NEED_HIGH_LEVEL					= -47,	// 你的等级不够
	OR_CANNOT_GETEXP					= -48,	// 你的宠物已无法得到经验
	OR_NEED_LIFE						= -49,  // 你的宠物寿命为0，无法再召唤，请为其增加寿命
	OR_EXP_FULL							= -50,  // 您的经验已经到达上限
	OR_TOO_MANY_TRAPS					= -51, 	// 无法设置更多的此类陷阱
	OR_PET_PLACARD_ISSUE_FAILED			= -52,	// 发布失败
	OR_PET_PLACARD_NOT_APPOINT_PET		= -53,	// 未指定宠物
	OR_PET_PLACARD_ONLY_CAN_ISSUE_ONE	= -54,	// 对不起，同时只能发布一只宠
	OR_PET_PLACARD_NEED_PET_TYPE		= -55,	// 对不起，你的宠好像不是宝宝哦
	OR_PET_PLACARD_NEED_PET_LEVEL		= -56,	// 对不起，你的宠级别还不够哦
	OR_PET_PLACARD_NEED_PET_HAPPINESS	= -57,	// 对不起，你的宠快乐值不够高
	OR_PET_PLACARD_NEED_PET_LIFE		= -58,	// 对不起，你的宠寿命不够
	OR_NEED_IN_STEALTH_MODE				= -59,	// 需要在隐身状态下使用
	OR_NOT_ENOUGH_ENERGY				= -60,	// 对不起，你的精力不足
	OR_CAN_NOT_MOVE_STALL_OPEN			= -61,	// 无法在摆摊中移动
	OR_NEED_IN_SHIELD_MODE				= -62,	// 需要在护体保护下
	OR_PET_DIE							= -63,	// 你的宠物已经死亡
	OR_PET_HADRECALL_NEEDHAPPINESS		= -64,	// 你的宠物快乐度不足60，已被收回
	OR_PET_HADRECALL_NEEDLIFE			= -65,	// 你的宠物寿命为0，已被收回
	OR_GEM_NOT_FIT_EQUIP				= -66,	// 这种宝石不能镶嵌在这种装备上
	OR_CANNOT_ASK_PETDETIAL				= -67,	// 你无法察看该宠物的信息
	OR_VARIANCEPET_CANNOT_RETURNTOCHILD = -68,  // 变异宠不能进行还童
	OR_COMBAT_CANNOT_RETURNTOCHILD		= -69,  // 出战宠不能进行还童
	OR_IMPASSABLE_ZONE					= -70,	// 不可走区域
	OR_NEED_SETMINORPASSWORD			= -71,	// 需要设置 二级密码
	OR_NEED_UNLOCKMINORPASSWORD			= -72,	// 需要解锁 二级密码
	OR_PETINEXCHANGE_CANNOT_GOFIGHT		= -73,	// 正在交易的宠物无法出战
	OR_HASSPOUSE_CANNOT_RETURNTOCHILD	= -74,  // 已经有配偶的宠物不能还童
	OR_CAN_NOT_FIND_SPECIFIC_ITEM		= -75,	// 缺少指定物品
	OR_CUT_PATHROUTE					= -1000,// 不是错误，只是截短路径
	
	//////////////////////////////////////////////////////////////
	// 请勿忘了更改 GameDefine_Result.cpp 中有 s_aResultText 内容

};
typedef OPERATE_RESULT ORESULT;


const char *GetOResultText( OPERATE_RESULT code );

#endif
