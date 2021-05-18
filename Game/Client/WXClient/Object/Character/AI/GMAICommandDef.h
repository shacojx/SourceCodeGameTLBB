// AICommandDef.h
//
//////////////////////////////////////////////////////

#pragma once

// 无效的
#define AIC_NONE					(0xFFFF)	// 无效ID

// 寻路移动
// F0	:	目标点X坐标
// F1	:	目标点Z坐标
#define AIC_MOVE					(1)

// 使用技能
// D0	:	技能ID
// D1	:	目标ID
// F2	:	目标点X坐标
// F3	:	目标点Z坐标
// F4	:	方向
#define AIC_USE_SKILL				(10)

// 对某个Tripper点进行操作(箱子，矿物...)
// D0	:	目标ID
#define AIC_TRIPPER_ACTIVE			(20)

// 缺省事件
// D0	:	目标ID
#define AIC_DEFAULT_EVENT			(30)

// 使用某项合成技能(厨艺，制造...)
// D0	:	目标ID
#define AIC_COMPOSE_ITEM			(40)

// 跳跃
#define AIC_JUMP					(60)

// 跟随
#define AIC_FOLLOW					(70)

//挂机
// D0	:	1为开始挂机
#define AIC_GUAJI					(80)

