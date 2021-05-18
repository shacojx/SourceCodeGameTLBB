///////////////////////////////////////////////////////////////////////////////
// 文件名：ConditionAndDepleteCore.h
// 功能说明：技能的基础类，声明技能的公共接口和常用功能
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#ifndef CONDITIONANDDEPLETECORE_H
#define CONDITIONANDDEPLETECORE_H

#include "Obj_Character.h"
#include "DataRecords.h"

using namespace Combat_Module::Skill_Module;

namespace Combat_Module
{
	typedef enum _ConditionAndDepleteID
	{
		//C表示条件
		//D表示消耗
		CD_INVALID = INVALID_ID,
		C_UNIT_MUST_HAVE_IMPACT = 0,
		C_UNIT_HP_MUST_LESS_THAN_BY_RATE,
		C_TARGET_MUST_HAVE_IMPACT,
		C_TARGET_LEVEL_MUST_LESS_THAN_BY_VALUE,
		CD_MANA_BY_VALUE,
		CD_MANA_BY_RATE,
		CD_RAGE_BY_RATE,
		CD_STRIKE_POINT_BY_SEGMENT,
		D_ALL_RAGE,
		D_CANCEL_SPECIFIC_IMPACT,
		C_TARGET_MUST_BE_MY_SPOUSE,
		CD_HP_BY_RATE,
		C_HP_BY_RATE,
	} ConditionAndDepleteID_T;
	
	typedef class CDCore_T
	{
		public:
			// public interface
			CDCore_T(){};
			~CDCore_T(){};
			//OnRefixItemPower();
			BOOL ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const;
			BOOL Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const;
		protected: //interface for interval using
		private:
			class ConDepLogic_T
			{
				public:
					enum
					{
						ID = CD_INVALID,
					};
					ConDepLogic_T(){};
					~ConDepLogic_T(){};
					virtual BOOL ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const{return TRUE;};
					virtual BOOL Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const{return TRUE;};					
				protected:
				private:
			};
			class CD_ManaByValue_T : public ConDepLogic_T
			{
				public:
					enum
					{
						ID = CD_MANA_BY_VALUE,
					};
					CD_ManaByValue_T(){};
					~CD_ManaByValue_T(){};
					BOOL ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const;
					BOOL Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const;					
				protected:
				private:
			};
			class CD_ManaByRate_T : public ConDepLogic_T
			{
				public:
					enum
					{
						ID = CD_MANA_BY_RATE,
					};
					CD_ManaByRate_T(){};
					~CD_ManaByRate_T(){};
					BOOL ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const;
					BOOL Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const;					
				protected:
				private:
			};
			class CD_RageByRate_T : public ConDepLogic_T
			{
				public:
					enum
					{
						ID = CD_RAGE_BY_RATE,
					};
					CD_RageByRate_T(){}
					~CD_RageByRate_T(){}
					BOOL ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const;
					BOOL Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const;					
				protected:
				private:
			};
			class CD_StrikePointBySegment_T : public ConDepLogic_T
			{
				public:
					enum
					{
						ID = CD_STRIKE_POINT_BY_SEGMENT,
					};
					CD_StrikePointBySegment_T(){}
					~CD_StrikePointBySegment_T(){}
					BOOL ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const;
					BOOL Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const;					
				protected:
				private:
			};
			class D_AllRage_T : public ConDepLogic_T
			{
				public:
					enum
					{
						ID = D_ALL_RAGE,
					};
					D_AllRage_T(){};
					~D_AllRage_T(){};
					BOOL ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const;
					BOOL Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const;					
				protected:
				private:
			};
			class C_UnitHaveImpact_T : public ConDepLogic_T
			{
				public:
					enum
					{
						ID = C_UNIT_MUST_HAVE_IMPACT,
					};
					C_UnitHaveImpact_T(){}
					~C_UnitHaveImpact_T(){}
					BOOL ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const;
					BOOL Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const;					
				protected:
				private:
			};
			class C_UnitHpLessThanByRate_T : public ConDepLogic_T
			{
				public:
					enum
					{
						ID = C_UNIT_HP_MUST_LESS_THAN_BY_RATE,
					};
					C_UnitHpLessThanByRate_T(){}
					~C_UnitHpLessThanByRate_T(){}
					BOOL ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const;
					BOOL Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const;					
				protected:
				private:
			};
			class C_TargetMustHaveImpact_T : public ConDepLogic_T
			{
				public:
					enum
					{
						ID = C_TARGET_MUST_HAVE_IMPACT,
					};
					C_TargetMustHaveImpact_T(){}
					~C_TargetMustHaveImpact_T(){}
					BOOL ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const;
					BOOL Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const;					
				protected:
				private:
			};
			class D_CancelSpecialImpact_T : public ConDepLogic_T
			{
				public:
					enum
					{
						ID = D_CANCEL_SPECIFIC_IMPACT,
					};
					D_CancelSpecialImpact_T(){}
					~D_CancelSpecialImpact_T(){}
					BOOL ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const;
					BOOL Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const;					
				protected:
				private:
			};
			class C_TargetMustBeMySpouse_T : public ConDepLogic_T
			{
				public:
					enum
					{
						ID = C_TARGET_MUST_BE_MY_SPOUSE,
					};
					C_TargetMustBeMySpouse_T(){}
					~C_TargetMustBeMySpouse_T(){}
					BOOL ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const;
					BOOL Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const;					
				protected:
				private:
			};
			class CD_HpByRate_T : public ConDepLogic_T
			{
				public:
					enum
					{
						ID = CD_HP_BY_RATE,
					};
					CD_HpByRate_T(){}
					~CD_HpByRate_T(){}
					BOOL ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const;
					BOOL Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const;					
				protected:
				private:
			};
			class C_HpByRate_T : public ConDepLogic_T
			{
				public:
					enum
					{
						ID = C_HP_BY_RATE,
					};
					C_HpByRate_T(){}
					~C_HpByRate_T(){}
					BOOL ConditionCheck(Obj_Character& rMe, ConDepTerm_T const& rData) const;
					BOOL Deplete(Obj_Character& rMe, ConDepTerm_T const& rData) const;					
				protected:
				private:
			};
		private:
			CD_ManaByValue_T            m_CD_ManaByValue;
			CD_ManaByRate_T             m_CD_ManaByRate;
			CD_RageByRate_T             m_CD_RageByRate;
			CD_StrikePointBySegment_T   m_CD_StrikePointBySegment;
			D_AllRage_T                 m_D_AllRage;
			C_UnitHaveImpact_T          m_C_UnitHaveImpact;
			C_TargetMustHaveImpact_T    m_C_TargetMustHaveImpact;
			C_UnitHpLessThanByRate_T   	m_C_UnitHpLessThanByRate;
			D_CancelSpecialImpact_T		m_D_CancelSpecialImpact;
			C_TargetMustBeMySpouse_T	m_C_TargetMustBeMySpouse;
			CD_HpByRate_T				m_CD_HpByRate_T;
			C_HpByRate_T				m_C_HpByRate_T;
	} ConditionAndDepleteCore_T;
	extern ConditionAndDepleteCore_T g_ConditionAndDepleteCore;
}
#endif //CONDITIONANDDEPLETECORE_H
