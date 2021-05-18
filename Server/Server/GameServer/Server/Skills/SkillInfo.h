///////////////////////////////////////////////////////////////////////////////
// 文件名：SkillInfo.h
// 功能说明：角色身上技能实例
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef SKILLINFO_H
#define SKILLINFO_H

#include "Type.h"
#include "DataRecords.h"
#include "GameDefine2.h"
#include "OResultDef.h"
#include "GameStruct.h"
#include "DB_Struct.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;
	class SkillInfo_T
	{
	public:
		SkillInfo_T();
		SkillInfo_T(SkillInfo_T const& rSrc);
		~SkillInfo_T(){};
		VOID Init(VOID);
		SkillID_t GetSkillID(VOID) const {return m_nSkillID;};
		VOID SetSkillID(SkillID_t const nID) {m_nSkillID = nID;};
		
		INT GetAccuracy(VOID) const {return m_nAccuracy;};
		VOID SetAccuracy(INT const nID) {m_nAccuracy = nID;};
		INT GetCriticalRate(VOID) const {return m_nCriticalRate;};
		VOID SetCriticalRate(INT const nRate) {m_nCriticalRate = nRate;};

		INT GetDepleteRefixByValue(VOID) const {return m_nDepleteRefixByValue;}
		VOID SetDepleteRefixByValue(INT nValue) {m_nDepleteRefixByValue = nValue;}
		INT GetDepleteRefixByRate(VOID) const {return m_nDepleteRefixByRate;}
		VOID SetDepleteRefixByRate(INT nRate) {m_nDepleteRefixByRate = nRate;}
		INT GetPowerRefixByValue(VOID) const {return m_nPowerRefixByValue;}
		VOID SetPowerRefixByValue(INT nValue) {m_nPowerRefixByValue = nValue;}
		INT GetPowerRefixByRate(VOID) const {return m_nPowerRefixByRate;}
		VOID SetPowerRefixByRate(INT nRate) {m_nPowerRefixByRate = nRate;}
		INT GetTimeRefixByValue(VOID) const {return m_nTimeRefixByValue;}
		VOID SetTimeRefixByValue(INT nValue) {m_nTimeRefixByValue = nValue;}
		INT GetTimeRefixByRate(VOID) const {return m_nTimeRefixByRate;}
		VOID SetTimeRefixByRate(INT nRate) {m_nTimeRefixByRate = nRate;}
		
		Time_t GetCooldownTime(VOID) const {return m_nCooldownTime;};
		VOID SetCooldownTime(Time_t const nTime) {m_nCooldownTime = nTime;};
		Time_t GetChargeTime(VOID) const {return m_nChargeTime;};
		VOID SetChargeTime(Time_t const nTime) {m_nChargeTime = nTime;};
		Time_t GetChannelTime(VOID) const {return m_nChannelTime;};
		VOID SetChannelTime(Time_t const nTime) {m_nChannelTime = nTime;};

		MenPaiID_t GetMenPai(VOID) const;
		CHAR const* GetName(VOID) const;
		INT  GetStudyLevel(VOID) const;
		INT  GetStudyMoney(VOID) const;
		INT  GetSkillMaxLevel(VOID) const;
		BOOL GetMustUseWeaponFlag(VOID) const;
		BOOL GetDisableByFlag1(VOID) const;
		BOOL GetDisableByFlag2(VOID) const;
		BOOL GetDisableByFlag3(VOID) const;
		ID_t GetSkillClass(VOID) const;
		//INT  GetXinFaParam(VOID) const;
		INT  GetRangedSkillFlag(VOID) const;
		BOOL IsForceBreakPreSkill(VOID) const;
		INT  GetTargetMustInSpecialState(VOID) const;
		ID_t GetSkillType(VOID) const;
		CooldownID_t GetCooldownID(VOID) const;
		ID_t GetSelectType(VOID) const;
		ID_t GetOperateModeForPetSkill(VOID) const;
		ID_t GetPetRateOfSkill(VOID) const;
		ID_t GetTypeOfPetSkill(VOID) const;
		ID_t GetImpactID(VOID) const;
		ID_t GetTargetingLogic(VOID) const;
		Time_t GetPlayActionTime(VOID) const;
		FLOAT GetOptimalRangeMin(VOID) const;
		FLOAT GetOptimalRangeMax(VOID) const;
		ID_t GetStandFlag(VOID) const;
		ID_t GetTargetLogicByStand(VOID) const;
		ID_t GetTargetCheckByObjType(VOID) const;
		BOOL IsPartyOnly(VOID) const;
		INT GetChargesOrInterval(VOID) const;
		BOOL IsAutoShotSkill(VOID) const;
		BOOL UseNormalAttackRate(VOID) const;
		FLOAT GetRadius(VOID) const;
		FLOAT GetAngle(VOID) const;
		INT GetMaxTargetNumber(VOID) const;
		BOOL CanInterruptAutoShot(VOID) const;
		Time_t GetDelayTime(VOID) const;
		ID_t GetLogicID(VOID) const;
			
		ConDepTerm_T const* GetConDepTermByIndex(INT const nIdx) const;
		INT GetTargetLevel(VOID) const;
		Descriptor_T const* GetDescriptorByIndex(INT const nIdx) const;
		
		SkillInfo_T& operator=(SkillTemplateData_T const& rSrc);
		SkillInfo_T& operator=(SkillInstanceData_T const& rSrc);
		SkillInfo_T& operator=(SkillInfo_T const& rSrc);
	protected:
		SkillID_t GetInstanceID(VOID) const {return m_nInstanceID;};
		VOID SetInstanceID(SkillID_t const nID) {m_nInstanceID = nID;};
	private:
		SkillID_t m_nSkillID; //技能编号
		INT m_nInstanceID;		//实例编号
		INT m_nAccuracy;//命中率
		INT m_nCriticalRate;//会心率,或者叫暴几率
		INT m_nDepleteRefixByValue; //效果值修正
		INT m_nDepleteRefixByRate; //消耗%修正
		INT m_nPowerRefixByValue; //消耗值修正
		INT m_nPowerRefixByRate;//威力%修正
		INT m_nTimeRefixByValue; //效果或陷阱的时间值修正
		INT m_nTimeRefixByRate; //效果或陷阱的时间%修正
		Time_t m_nCooldownTime;//
		Time_t m_nChargeTime;
		Time_t m_nChannelTime;
	};
	Skill_Module::SkillTemplateData_T const* GetSkillTemplateByID(INT nSkill);
	Skill_Module::SkillInstanceData_T const* GetSkillInstanceByID(INT nInstance);

	class DamageList_T
	{
		public:
			DamageList_T()
			{
				memset((VOID*)m_aDamages, 0, sizeof(m_aDamages));
			}
			~DamageList_T(){};
			INT m_aDamages[DAMAGE_TYPE_NUMBER];
		protected:
		private:
	};
}

#endif //SKILLINFO_H
