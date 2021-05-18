// DataPoolCommandDef.h
//
//////////////////////////////////////////////////////

#pragma once

// 无效的
#define DPC_NONE					(0xFFFF)


// 刷新技能
// D0	:	对象ObjID
// D1	:	更改的数量
// P2	:	所更技能属性列表
//#define DPC_UPDATE_SKILL_ATTRIB		(3)

// 刷新心法
// D0	:	对象ObjID
// D1	:	更改的数量
// P2	:	所更心法属性列表
//#define DPC_UPDATE_XINFA_ATTRIB		(4)

// 刷新装备
// D0	:	对象ObjID
// D1	:	更改装备的mask
// P2	:	所更装备属性列表
//#define DPC_UPDATE_EQUIP_ATTRIB		(5)

// 脚本指令
// D0	:	script command id
// P1	:	param list
#define DPC_SCRIPT_COMMAND			(6)

// 任务列表
// D0	:	对象ObjID
// D1	:	所更改任务的标志
// P2	:	所更改任务BUF
// B3	:	是否刷新了任务完成的标志
// P4	:	任务完成的标志BUF
#define DPC_UPDATE_MISSION_LIST		(7)

// 增加一个任务
// P0	:	指向_OWN_MISSION的指针
#define DPC_UPDATE_MISSION_ADD		(8)

// 更改一个任务参数
// P0	:	指向_OWN_MISSION的指针
#define DPC_UPDATE_MISSION_MODIFY	(9)

// 删除一个任务
// D0	:	任务的ID(MissionID_t)
#define DPC_UPDATE_MISSION_REMOVE	(10)

// 刷新可以拾取的任务物品列表
// D0	:	数目
// P1	:	列表数据
#define DPC_UPDATE_CAN_PICK_MISSION_ITEM_LIST	(11)

// 加入一个可以拾取的任务物品
// D0	:	物品ID
#define DPC_ADD_CAN_PICK_MISSION_ITEM			(12)

// 删除一个可以拾取的任务物品
// D0	:	物品ID
#define DPC_REMOVE_CAN_PICK_MISSION_ITEM		(13)

// 修改任务描述信息
#define	DPC_UPDATE_MISSION_DESC					(18)

//生活技能刷新
// D0	:	对象ObjID
// D1	:	更改的数量
// P2	:	所更技能属性列表
//#define DPC_UPDATE_LIFE_ABILITY		(14)

//生活技能配方刷新
// P0	:	配方数据
//#define DPC_UPDATE_LIFE_PRESCR		(15)

//冷却时间
// D0	:	更新了多少个有效的COOL DOWN时间
// P1	:	时间列表
//#define DPC_UPDATE_COOL_DOWN_LIST	(16)

//组队信息更新
#define DPC_UPDATE_TEAM_OR_GROUP	(17)


