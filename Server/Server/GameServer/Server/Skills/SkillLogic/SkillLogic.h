///////////////////////////////////////////////////////////////////////////////
// 文件名：SkillLogic.h
// 功能说明：技能的基础类，声明技能的公共接口和常用功能
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#ifndef SKILLLOGIC_H
#define SKILLLOGIC_H
#include "Type.h"
#include "Obj_Character.h"
#include "Obj_Human.h"
#include "SkillInfo.h"
#include "Skill_Core.h"
#include "GameStruct_Impact.h"
#include "MenPai.h"
#include "DataRecords.h"
#include "Impact_Core.h"

using namespace MenPai_Module;
namespace Combat_Module
{
	namespace Skill_Module
	{
		using namespace Combat_Module;
		enum
		{
			INVALID_SKILL_ID = INVALID_ID,
		};
		enum
		{
			MAX_MULTI_STRIKE_NUMBER = 5,//最大连击次数
		};
		class SkillLogic_T
		{
			public:
				enum
				{
					ID = INVALID_ID,
				};
				// public interface
				SkillLogic_T(){};
				~SkillLogic_T(){};
				//OnRefixItemPower();
				virtual BOOL SpecificOperationOnSkillStart(Obj_Character& rMe)const;
				virtual BOOL IsPassive(VOID) const;
				virtual BOOL OnInterrupt(Obj_Character& rMe) const;
				virtual BOOL OnCancel(Obj_Character& rMe) const;
				BOOL StartCharging(Obj_Character& rMe) const;
				BOOL StartChanneling(Obj_Character& rMe) const;
				BOOL StartLaunching(Obj_Character& rMe) const;
				BOOL IsWantedTarget(Obj_Character& rMe, Obj_Character& rTar, SkillInfo_T const& rSkillInfo) const;
				//Skill Refix interfaces
				VOID RefixPowerByRate(SkillInfo_T& rSkill, INT nRate) const;
				virtual BOOL CanBeRefixed(VOID)const {return FALSE;};
				VOID RefixDepleteByRate(SkillInfo_T& rSkill, INT nRate) const;
				BOOL IsConditionSatisfied(Obj_Character& rMe) const;
				virtual BOOL CancelSkillEffect(Obj_Character& rMe) const {return FALSE;};
				BOOL Action_ActivateOnceHandler(Obj_Character& rMe) const;
				BOOL Action_ActivateEachTickHandler(Obj_Character& rMe) const;
			public: //passive skill interface
				BOOL Refix_SkillInstance(SkillInfo_T& rThisPassiveSkill, SkillInfo_T& rTargetSkill) const{return TRUE;};
				BOOL Refix_ItemEffect(SkillInfo_T& rThisPassiveSkill, INT nSlotID, INT nItemType, INT nATTRID, SHORT& nAttrValue) const {return TRUE;};
				VOID BroadcastTargetListMessage(Obj_Character& rMe, OBJLIST& rTargets, HitFlagsForOBJLIST_T& rHitFlagsForObjList) const;
			public: //SkillInfo accessor
				INT GetSkillInfoDescriptorValueByIndex(SkillInfo_T const& rSkillInfo, INT nIndex) const;
				VOID SetSkillInfoDescriptorValueByIndex(SkillInfo_T& rSkillInfo, INT nIndex, INT nValue) const;
			protected: //interface for interval using
				BOOL ActivateOnce(Obj_Character& rMe) const;
				BOOL ActivateEachTick(Obj_Character& rMe) const;
				virtual BOOL EffectOnUnitEachTick(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag=FALSE) const;
				virtual BOOL EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag=FALSE) const;
				virtual BOOL TargetCheckForEachTick(Obj_Character& rMe) const;
				virtual BOOL TargetCheckForActivateOnce(Obj_Character& rMe) const;
				BOOL DepleteProcess(Obj_Character& rMe) const;
				// standard methods
				BOOL CommonConditionCheck(Obj_Character& rMe) const;
				virtual BOOL SpecificConditionCheck(Obj_Character& rMe) const {return TRUE;}
				BOOL CommonDeplete(Obj_Character& rMe) const;
				virtual BOOL SpecificDeplete(Obj_Character& rMe) const {return TRUE;}
				virtual BOOL IsSpecificTargetWanted(Obj_Character& rMe, Obj_Character& rTar, SkillInfo_T const& rSkillInfo) const {return TRUE;};
				BOOL CanelSkill(Obj_Character& rMe) const;
				Obj* GetTargetObj(Obj_Character& rMe) const;
				BOOL IsOutOfRange(Obj_Character& rMe) const;
				BOOL IsOutOfRange(Obj_Character& rMe, Obj_Character& rObj) const;
				BOOL IsOutOfRange(Obj_Character& rMe, WORLD_POS const& rPosTarget) const;
				VOID SetCooldown(Obj_Character& rMe, CooldownID_t nID, INT nCooldownTime) const;
				VOID SetAutoRepeatCooldown(Obj_Character& rMe, INT nCooldownTime) const;
				BOOL ScanUnitForTarget(Obj_Character& rMe, FLOAT fX,FLOAT fZ, OBJLIST& rTargets)const;
				BOOL HitThisTarget(Obj_Character& rMe, Obj_Character& rTar) const;
				BOOL CriticalHitThisTarget(Obj_Character & rMe, Obj_Character & rTar) const;
				BOOL CalculateTargetList(Obj_Character & rMe, OBJLIST& rTargets) const;
				// functional method
				BOOL IsTargetAlive(Obj_Character& rTar) const;
				//VOID StopSkill(Obj_Character& rMe) const;
				FLOAT DistanceSq(WORLD_POS const& posStart, WORLD_POS const& posEnd) const;
				BOOL IsHit(Obj_Character& rMe, Obj_Character& rTar, INT nAccuracy) const;
				BOOL IsCriticalHit(Obj_Character& rMe, INT nCriticalRate) const;
				Time_t RefixPlayActionTimeWithAttackRate(Time_t nPlayActionTime, INT nAttackRate) const;
				Time_t RefixCooldownTimeWithAttackRate(Time_t nCooldownTime, INT nAttackRate) const;
				BOOL RegisterImpactEvent(Obj_Character& rReceiver, Obj_Character& rSender, OWN_IMPACT& rImp, Time_t nDelayTime, BOOL bCriticalFlag) const;
				BOOL RegisterSkillMissEvent(Obj_Character& rReceiver, Obj_Character& rSender, SkillID_t nSkill, Time_t nDelayTime) const;
				BOOL RegisterBeSkillEvent(Obj_Character& rReceiver, Obj_Character& rSender, SkillID_t nSkill, Time_t nDelayTime) const;
				BOOL RegisterActiveObj(Obj& rObj, Obj_Character& rSender, Time_t nDelayTime) const;
			private:
				Time_t CalculateActionTime(Obj_Character& rMe) const;
				VOID CooldownProcess(Obj_Character& rMe) const;
		};
		SkillLogic_T const* Skill_GetLogic(SkillInfo_T const& rSkill);
		enum
		{
			NUMBER_OF_COMMON_SKILL = 20,
			NUMBER_OF_MENPAI_SKILL = 30,
			NUMBER_OF_ITEM_SKILL = 100,
			NUMBER_OF_PET_SKILL = 150,
			NUMBER_OF_MONSTER_SKILL = 100,
			NUMBER_OF_OTHER_SKILL = 10, //尽量不要用这个分类
			NUMBER_OF_ALL_SKILL = NUMBER_OF_COMMON_SKILL
								+ NUMBER_OF_MENPAI_SKILL * MenPai_T::NUMBER_OF_MENPAI
								+ NUMBER_OF_PET_SKILL
								+ NUMBER_OF_ITEM_SKILL
								+ NUMBER_OF_MONSTER_SKILL
								+ NUMBER_OF_OTHER_SKILL,
		};
		typedef enum
		{
			INVALID_SKILL_LOGIC_ID = INVALID_ID,
			//Base skill, start at 0
			IMPACTS_TO_TARGET = 0,
			IMPACTS_TO_TARGET_AND_SELF,
			TELEPORT_AND_IMPACTS,
			//...
			ARMOR_MASTERY =8,
			WEAPON_MASTERY =9,
			
			// Common Skill, start at 10
			COMMON_SKILL_001 = 10,
			COMMON_SKILL_002,
			COMMON_SKILL_003,
			//...
			// shao lin, start at 20
			SHAO_LIN_SKILL_001 = IMPACTS_TO_TARGET + NUMBER_OF_COMMON_SKILL, 
			SHAO_LIN_SKILL_002, 
			SHAO_LIN_SKILL_003, 
			SHAO_LIN_SKILL_004, 
			SHAO_LIN_SKILL_005, 
			SHAO_LIN_SKILL_006, 
			SHAO_LIN_SKILL_007, 
			SHAO_LIN_SKILL_008, 
			SHAO_LIN_SKILL_009, 
			SHAO_LIN_SKILL_010, 
			SHAO_LIN_SKILL_011, 
			SHAO_LIN_SKILL_012, 
			SHAO_LIN_SKILL_013, 
			SHAO_LIN_SKILL_014, 
			SHAO_LIN_SKILL_015, 
			SHAO_LIN_SKILL_016, 
			SHAO_LIN_SKILL_017, 
			SHAO_LIN_SKILL_018, 
			SHAO_LIN_SKILL_019, 
			//...
			//MING JIAO, start at 50
			MING_JIAO_SKILL_001 = SHAO_LIN_SKILL_001 + NUMBER_OF_MENPAI_SKILL,
			MING_JIAO_SKILL_002,
			MING_JIAO_SKILL_003,
			MING_JIAO_SKILL_004,
			MING_JIAO_SKILL_005,
			MING_JIAO_SKILL_006,
			MING_JIAO_SKILL_007,
			MING_JIAO_SKILL_008,
			MING_JIAO_SKILL_009,
			MING_JIAO_SKILL_010,
			MING_JIAO_SKILL_011,
			MING_JIAO_SKILL_012,
			MING_JIAO_SKILL_013,
			MING_JIAO_SKILL_014,
			MING_JIAO_SKILL_015,
			MING_JIAO_SKILL_016,
			MING_JIAO_SKILL_017,
			MING_JIAO_SKILL_018,
			MING_JIAO_SKILL_019,
			//...
			//Gai Bang, start at 80
			GAI_BANG_SKILL_001 = MING_JIAO_SKILL_001 + NUMBER_OF_MENPAI_SKILL,
			GAI_BANG_SKILL_002,
			GAI_BANG_SKILL_003,
			GAI_BANG_SKILL_004,
			GAI_BANG_SKILL_005,
			GAI_BANG_SKILL_006,
			GAI_BANG_SKILL_007,
			GAI_BANG_SKILL_008,
			GAI_BANG_SKILL_009,
			GAI_BANG_SKILL_010,
			GAI_BANG_SKILL_011,
			GAI_BANG_SKILL_012,
			GAI_BANG_SKILL_013,
			GAI_BANG_SKILL_014,
			GAI_BANG_SKILL_015,
			GAI_BANG_SKILL_016,
			GAI_BANG_SKILL_017,
			GAI_BANG_SKILL_018,
			GAI_BANG_SKILL_019,
			//...
			// wu dang, start at 110
			WU_DANG_SKILL_001 = GAI_BANG_SKILL_001 + NUMBER_OF_MENPAI_SKILL,
			WU_DANG_SKILL_002,
			WU_DANG_SKILL_003,
			WU_DANG_SKILL_004,
			WU_DANG_SKILL_005,
			WU_DANG_SKILL_006,
			WU_DANG_SKILL_007,
			WU_DANG_SKILL_008,
			WU_DANG_SKILL_009,
			WU_DANG_SKILL_010,
			WU_DANG_SKILL_011,
			WU_DANG_SKILL_012,
			WU_DANG_SKILL_013,
			WU_DANG_SKILL_014,
			WU_DANG_SKILL_015,
			WU_DANG_SKILL_016,
			WU_DANG_SKILL_017,
			WU_DANG_SKILL_018,
			WU_DANG_SKILL_019,
			//...
			//E Mei,start at 140
			E_MEI_SKILL_001 = WU_DANG_SKILL_001 + NUMBER_OF_MENPAI_SKILL,
			E_MEI_SKILL_002,
			E_MEI_SKILL_003,
			E_MEI_SKILL_004,
			E_MEI_SKILL_005,
			E_MEI_SKILL_006,
			E_MEI_SKILL_007,
			E_MEI_SKILL_008,
			E_MEI_SKILL_009,
			E_MEI_SKILL_010,
			E_MEI_SKILL_011,
			E_MEI_SKILL_012,
			E_MEI_SKILL_013,
			E_MEI_SKILL_014,
			E_MEI_SKILL_015,
			E_MEI_SKILL_016,
			E_MEI_SKILL_017,
			E_MEI_SKILL_018,
			E_MEI_SKILL_019,
			//...
			//Xing Xiu, start at 170
			XING_XIU_SKILL_001 = E_MEI_SKILL_001 + NUMBER_OF_MENPAI_SKILL,
			XING_XIU_SKILL_002,
			XING_XIU_SKILL_003,
			XING_XIU_SKILL_004,
			XING_XIU_SKILL_005,
			XING_XIU_SKILL_006,
			XING_XIU_SKILL_007,
			XING_XIU_SKILL_008,
			XING_XIU_SKILL_009,
			XING_XIU_SKILL_010,
			XING_XIU_SKILL_011,
			XING_XIU_SKILL_012,
			XING_XIU_SKILL_013,
			XING_XIU_SKILL_014,
			XING_XIU_SKILL_015,
			XING_XIU_SKILL_016,
			XING_XIU_SKILL_017,
			XING_XIU_SKILL_018,
			XING_XIU_SKILL_019,
			//...
			//DaLi, start at 200
			DA_LI_SKILL_001 = XING_XIU_SKILL_001 + NUMBER_OF_MENPAI_SKILL,
			DA_LI_SKILL_002,
			DA_LI_SKILL_003,
			DA_LI_SKILL_004,
			DA_LI_SKILL_005,
			DA_LI_SKILL_006,
			DA_LI_SKILL_007,
			DA_LI_SKILL_008,
			DA_LI_SKILL_009,
			DA_LI_SKILL_010,
			DA_LI_SKILL_011,
			DA_LI_SKILL_012,
			DA_LI_SKILL_013,
			DA_LI_SKILL_014,
			DA_LI_SKILL_015,
			DA_LI_SKILL_016,
			DA_LI_SKILL_017,
			DA_LI_SKILL_018,
			DA_LI_SKILL_019,
			//...
			//TianShan, start at 230
			TIAN_SHAN_SKILL_001 = DA_LI_SKILL_001 + NUMBER_OF_MENPAI_SKILL,
			TIAN_SHAN_SKILL_002,
			TIAN_SHAN_SKILL_003,
			TIAN_SHAN_SKILL_004,
			TIAN_SHAN_SKILL_005,
			TIAN_SHAN_SKILL_006,
			TIAN_SHAN_SKILL_007,
			TIAN_SHAN_SKILL_008,
			TIAN_SHAN_SKILL_009,
			TIAN_SHAN_SKILL_010,
			TIAN_SHAN_SKILL_011,
			TIAN_SHAN_SKILL_012,
			TIAN_SHAN_SKILL_013,
			TIAN_SHAN_SKILL_014,
			TIAN_SHAN_SKILL_015,
			TIAN_SHAN_SKILL_016,
			TIAN_SHAN_SKILL_017,
			TIAN_SHAN_SKILL_018,
			TIAN_SHAN_SKILL_019,
			//...
			//XiaoYao, start at 260
			XIAO_YAO_SKILL_001 = TIAN_SHAN_SKILL_001 + NUMBER_OF_MENPAI_SKILL,
			XIAO_YAO_SKILL_002,
			XIAO_YAO_SKILL_003,
			XIAO_YAO_SKILL_004,
			XIAO_YAO_SKILL_005,
			XIAO_YAO_SKILL_006,
			XIAO_YAO_SKILL_007,
			XIAO_YAO_SKILL_008,
			XIAO_YAO_SKILL_009,
			XIAO_YAO_SKILL_010,
			XIAO_YAO_SKILL_011,
			XIAO_YAO_SKILL_012,
			XIAO_YAO_SKILL_013,
			XIAO_YAO_SKILL_014,
			XIAO_YAO_SKILL_015,
			XIAO_YAO_SKILL_016,
			XIAO_YAO_SKILL_017,
			XIAO_YAO_SKILL_018,
			XIAO_YAO_SKILL_019,
			//...
			//Pet , start at 290
			PET_SKILL_001 = XIAO_YAO_SKILL_001 + NUMBER_OF_MENPAI_SKILL,
			PET_SKILL_002,
			PET_SKILL_003,
			PET_SKILL_004,
			PET_SKILL_005,
			PET_SKILL_006,
			PET_SKILL_007,
			PET_SKILL_008,
			PET_SKILL_009,
			PET_SKILL_010,
			PET_SKILL_011,
			PET_SKILL_012,
			PET_SKILL_013,
			PET_SKILL_014,
			PET_SKILL_015,
			PET_SKILL_016,
			PET_SKILL_017,
			PET_SKILL_018,
			PET_SKILL_019,
			PET_SKILL_020,
			PET_SKILL_021,
			PET_SKILL_022,
			//...
			//Item, start at 440
			ITEM_SKILL_001 = PET_SKILL_001 + NUMBER_OF_PET_SKILL,
			ITEM_SKILL_002,
			ITEM_SKILL_003,
			ITEM_SKILL_004,
			ITEM_SKILL_005,
			ITEM_SKILL_006,
			ITEM_SKILL_007,
			ITEM_SKILL_008,
			ITEM_SKILL_009,
			ITEM_SKILL_010,
			ITEM_SKILL_011,
			ITEM_SKILL_012,
			ITEM_SKILL_013,
			ITEM_SKILL_014,
			ITEM_SKILL_015,
			ITEM_SKILL_016,
			ITEM_SKILL_017,
			ITEM_SKILL_018,
			ITEM_SKILL_019,
			ITEM_SKILL_020,
			//...
			//Monster , start at 540
			MONSTER_SKILL_001 = ITEM_SKILL_001 + NUMBER_OF_ITEM_SKILL,
			MONSTER_SKILL_002,
			MONSTER_SKILL_003,
			MONSTER_SKILL_004,
			MONSTER_SKILL_005,
			MONSTER_SKILL_006,
			MONSTER_SKILL_007,
			MONSTER_SKILL_008,
			MONSTER_SKILL_009,
			MONSTER_SKILL_010,
			MONSTER_SKILL_011,
			MONSTER_SKILL_012,
			MONSTER_SKILL_013,
			MONSTER_SKILL_014,
			MONSTER_SKILL_015,
			MONSTER_SKILL_016,
			MONSTER_SKILL_017,
			MONSTER_SKILL_018,
			MONSTER_SKILL_019,
			MONSTER_SKILL_020,
			//...
			// OTHER, start 640
			OTHER_SKILL_001 = MONSTER_SKILL_001 + NUMBER_OF_MONSTER_SKILL,
			MAX_SKILL_LOGIC_ID = OTHER_SKILL_001 + NUMBER_OF_OTHER_SKILL  //最大的skill logic id
			//...
		} SkillLogicID_T;
		enum
		{
			COMMON_SKILL_LOGIC_ID_START 	= 	MELEE_ATTACK,
			COMMON_SKILL_LOGIC_ID_END 		= 	COMMON_SKILL_LOGIC_ID_START+NUMBER_OF_COMMON_SKILL-1,
			SHAO_LIN_SKILL_LOGIC_ID_START	= 	SHAO_LIN_SKILL_001,
			SHAO_LIN_SKILL_LOGIC_ID_END 	=	SHAO_LIN_SKILL_001 + NUMBER_OF_MENPAI_SKILL-1,
			MING_JIAO_SKILL_LOGIC_ID_START	= 	MING_JIAO_SKILL_001,
			MING_JIAO_SKILL_LOGIC_ID_END 	=	MING_JIAO_SKILL_001 + NUMBER_OF_MENPAI_SKILL-1,
			GAI_BANG_SKILL_LOGIC_ID_START	= 	GAI_BANG_SKILL_001,
			GAI_BANG_SKILL_LOGIC_ID_END 	=	GAI_BANG_SKILL_001 + NUMBER_OF_MENPAI_SKILL-1,
			WU_DANG_SKILL_LOGIC_ID_START	= 	WU_DANG_SKILL_001,
			WU_DANG_SKILL_LOGIC_ID_END 		=	WU_DANG_SKILL_001 + NUMBER_OF_MENPAI_SKILL-1,
			E_MEI_SKILL_LOGIC_ID_START		= 	E_MEI_SKILL_001,
			E_MEI_SKILL_LOGIC_ID_END 		=	E_MEI_SKILL_001 + NUMBER_OF_MENPAI_SKILL-1,
			XING_XIU_SKILL_LOGIC_ID_START	= 	XING_XIU_SKILL_001,
			XING_XIU_SKILL_LOGIC_ID_END 	=	XING_XIU_SKILL_001 + NUMBER_OF_MENPAI_SKILL-1,
			DA_LI_SKILL_LOGIC_ID_START		= 	DA_LI_SKILL_001,
			DA_LI_SKILL_LOGIC_ID_END 		=	DA_LI_SKILL_001 + NUMBER_OF_MENPAI_SKILL-1,
			TIAN_SHAN_SKILL_LOGIC_ID_START	= 	TIAN_SHAN_SKILL_001,
			TIAN_SHAN_SKILL_LOGIC_ID_END 	=	TIAN_SHAN_SKILL_001 + NUMBER_OF_MENPAI_SKILL-1,
			XIAO_YAO_SKILL_LOGIC_ID_START	= 	XIAO_YAO_SKILL_001,
			XIAO_YAO_SKILL_LOGIC_ID_END 	=	XIAO_YAO_SKILL_001 + NUMBER_OF_MENPAI_SKILL-1,
			PET_SKILL_LOGIC_ID_START 		= 	PET_SKILL_001,
			PET_SKILL_LOGIC_ID_END 			= 	PET_SKILL_001 + NUMBER_OF_PET_SKILL-1,
			ITEM_SKILL_LOGIC_ID_START 		= 	ITEM_SKILL_001,
			ITEM_SKILL_LOGIC_ID_END			= 	ITEM_SKILL_001 + NUMBER_OF_ITEM_SKILL-1,
			OTHER_SKILL_LOGIC_ID_START		=	OTHER_SKILL_001,
			OTHER_SKILL_LOGIC_ID_END		=	OTHER_SKILL_001 + NUMBER_OF_OTHER_SKILL,
		};
	}
}
#endif //SKILLLOGIC_H
