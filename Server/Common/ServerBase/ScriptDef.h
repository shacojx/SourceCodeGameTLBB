// ScriptDef.h
//
/////////////////////////////////////////////////////////

#ifndef __SCRIPTDEF_H__
#define __SCRIPTDEF_H__

#define ABILITY_LOGIC_SCRIPT			701601
#define GEM_COMPOUND_SCRIPT				701602

#define	DEF_EVENT_ENTRY_FUNC_NAME		("OnDefaultEvent") //脚本进入函数
#define	DEF_ON_KILL_OBJECT_FUNC_NAME	("OnKillObject")
#define	DEF_ON_ITEM_CHANGED_FUNC_NAME	("OnItemChanged")
#define	DEF_ON_PET_CHANGED_FUNC_NAME	("OnPetChanged")
#define	DEF_ON_ENTER_AREA_FUNC_NAME		("OnEnterArea")
#define	DEF_ON_LEAVE_AREA_FUNC_NAME		("OnLeaveArea")
#define DEF_EVENT_ON_TIMER				("OnTimer")

#define DEF_MISSION_ACCEPT				("OnMissionAccept") //接受任务
#define DEF_MISSION_ABANDON				("OnAbandon") //放弃任务
#define DEF_MISSION_REFUSE				("OnMissionRefuse") // 拒绝接受任务
#define DEF_MISSION_SUBMIT				("OnMissionSubmit") //任务完成后，提交任务
#define DEF_MISSION_CHECK				("OnMissionCheck") //任务完成条件检查
#define DEF_MISSION_CONTINUE			("OnMissionContinue") //任务没完成，继续

// 以下三项供普通合成配方使用
// 技能使用检查函数
#define DEF_ABILITY_CHECK				("AbilityCheck")
// 合成结束，进行相关消耗
#define DEF_ABILITY_CONSUME				("AbilityConsume")
// 合成成功，产出产品
#define DEF_ABILITY_PRODUCE				("AbilityProduce")

// 合成结束时的成功率计算
#define DEF_ABILITY_SUCCESSFUL_CHECK	("CheckForResult")
// 配方合成结束时的熟练度增长
#define DEF_ABILITY_GAIN_EXPERIENCE		("GainExperience")

// 合成成功时计算产品的品质（Quality）
#define DEF_ABILITY_CALC_QUALITY		("CalcQuality")

// 以下是宝石部分
#define DEF_GEM_COUNT_CHECK				("TooManyGems")
#define DEF_IS_GEM_FIT_EQUIP			("IsGemFitEquip")
#define DEF_GEM_CONFLICT_CHECK			("IsGemConflict")
#define DEF_GEM_EMBED_PROCESS			("EmbedProc")

#define DEF_GEM_COMPOUND				("GemCompound")

#endif