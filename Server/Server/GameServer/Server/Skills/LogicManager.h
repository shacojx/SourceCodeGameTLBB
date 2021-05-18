///////////////////////////////////////////////////////////////////////////////
// 文件名：LogicManager.h
// 功能说明：通用的逻辑管理器
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
/** \file LogicManager.h
*		\brief 逻辑管理器。用模板实现
*/
#ifndef LOGICMANAGER_H
#define LOGICMANAGER_H

#include "Type.h"
#include "ImpactLogics.h"
#include "TargetChecker.h"
#include "RelationshipChecker.h"
#include "SkillLogics.h"
#include "MenPais.h"
#include "SpecialObjLogics.h"

namespace Combat_Module
{
	template <typename T, INT nSize>
	class LogicManager_T
	{
		public:
			LogicManager_T();
			~LogicManager_T();
			BOOL Init(VOID);
			T const* GetLogicById(INT const nId);
			VOID RegisterLogic(INT nID, T* pLogic);
		private:
			BOOL m_bReady;
			T* m_apLogics[nSize];
	};
	template <typename T, INT nSize>
	LogicManager_T<T, nSize>::LogicManager_T()
	{
		__ENTER_FUNCTION
		memset((VOID*)m_apLogics, 0, sizeof(m_apLogics));
		m_bReady = Init();
		__LEAVE_FUNCTION				
	};
	
	template <typename T, INT nSize>
	inline BOOL LogicManager_T<T, nSize>::Init(VOID)
	{
		__ENTER_FUNCTION
		return FALSE;
		__LEAVE_FUNCTION
		return FALSE;
	};
	
	template <typename T, INT nSize>
	inline LogicManager_T<T, nSize>::~LogicManager_T()
	{
		__ENTER_FUNCTION
		m_bReady = FALSE;
		for(INT i=0;nSize>i;++i)
		{
			if(NULL != m_apLogics[i])
			{
				SAFE_DELETE(m_apLogics[i]);
			};
		};
		__LEAVE_FUNCTION		
	};
	
	template<typename T, INT nSize>
	inline T const* LogicManager_T<T, nSize>::GetLogicById(INT const nId)
	{
		__ENTER_FUNCTION
		if(0<=nId && nSize>nId && TRUE==m_bReady)
		{
			return m_apLogics[nId];
		}
		return NULL;
		__LEAVE_FUNCTION
		return NULL;
	}
	
	template<typename T, INT nSize>
	inline VOID LogicManager_T<T, nSize>::RegisterLogic(INT nID, T* pLogic)
	{
		Assert(m_apLogics[nID]==NULL);
		m_apLogics[nID]=pLogic;
	}
	#define REGISTER_LOGIC(LOGIC) {RegisterLogic(LOGIC::ID, (new (LOGIC)));}

	using namespace Combat_Module::Impact_Module;
	template<>
	inline BOOL LogicManager_T<ImpactLogic_T, NUMBER_OF_IMPACT_LOGIC_IDS>::Init(VOID)
	{
		__ENTER_FUNCTION
		REGISTER_LOGIC(StdImpact000_T);
		REGISTER_LOGIC(StdImpact001_T);
		REGISTER_LOGIC(StdImpact002_T);
		REGISTER_LOGIC(StdImpact003_T);
		REGISTER_LOGIC(StdImpact004_T);
		REGISTER_LOGIC(StdImpact005_T);
		REGISTER_LOGIC(StdImpact006_T);
		REGISTER_LOGIC(StdImpact007_T);
		REGISTER_LOGIC(StdImpact008_T);
		REGISTER_LOGIC(StdImpact009_T);
		REGISTER_LOGIC(StdImpact010_T);
		REGISTER_LOGIC(StdImpact011_T);
		REGISTER_LOGIC(StdImpact012_T);
		REGISTER_LOGIC(StdImpact013_T);
		REGISTER_LOGIC(StdImpact014_T);
		REGISTER_LOGIC(StdImpact015_T);
		REGISTER_LOGIC(StdImpact016_T);
		REGISTER_LOGIC(StdImpact017_T);
		REGISTER_LOGIC(StdImpact018_T);
		REGISTER_LOGIC(StdImpact019_T);
		REGISTER_LOGIC(StdImpact020_T);
		REGISTER_LOGIC(StdImpact021_T);
		REGISTER_LOGIC(StdImpact022_T);
		REGISTER_LOGIC(StdImpact023_T);
		REGISTER_LOGIC(StdImpact024_T);
		REGISTER_LOGIC(StdImpact025_T);
		REGISTER_LOGIC(StdImpact026_T);
		REGISTER_LOGIC(StdImpact027_T);
		REGISTER_LOGIC(StdImpact028_T);
		REGISTER_LOGIC(StdImpact029_T);
		REGISTER_LOGIC(StdImpact030_T);
		REGISTER_LOGIC(StdImpact031_T);
		REGISTER_LOGIC(StdImpact032_T);
		REGISTER_LOGIC(StdImpact033_T);
		REGISTER_LOGIC(StdImpact034_T);
		REGISTER_LOGIC(StdImpact035_T);
		REGISTER_LOGIC(StdImpact036_T);
		REGISTER_LOGIC(StdImpact037_T);
		REGISTER_LOGIC(StdImpact038_T);
		REGISTER_LOGIC(StdImpact039_T);
		REGISTER_LOGIC(StdImpact040_T);
		REGISTER_LOGIC(StdImpact041_T);
		REGISTER_LOGIC(StdImpact042_T);
		REGISTER_LOGIC(StdImpact043_T);
		REGISTER_LOGIC(StdImpact044_T);
		REGISTER_LOGIC(StdImpact045_T);
		REGISTER_LOGIC(StdImpact046_T);
		REGISTER_LOGIC(StdImpact047_T);
		REGISTER_LOGIC(StdImpact048_T);
		REGISTER_LOGIC(StdImpact049_T);
		REGISTER_LOGIC(StdImpact050_T);
		REGISTER_LOGIC(StdImpact051_T);
		REGISTER_LOGIC(StdImpact052_T);
		REGISTER_LOGIC(StdImpact053_T);
		REGISTER_LOGIC(StdImpact054_T);
		REGISTER_LOGIC(StdImpact055_T);
		REGISTER_LOGIC(StdImpact056_T);
		REGISTER_LOGIC(StdImpact057_T);
		/*
		REGISTER_LOGIC(StdImpact058_T);
		REGISTER_LOGIC(StdImpact059_T);
		REGISTER_LOGIC(StdImpact060_T);
		REGISTER_LOGIC(StdImpact061_T);
		REGISTER_LOGIC(StdImpact062_T);
		REGISTER_LOGIC(StdImpact063_T);
		REGISTER_LOGIC(StdImpact064_T);
		REGISTER_LOGIC(StdImpact065_T);
		REGISTER_LOGIC(StdImpact066_T);
		REGISTER_LOGIC(StdImpact067_T);
		REGISTER_LOGIC(StdImpact068_T);
		REGISTER_LOGIC(StdImpact069_T);
		REGISTER_LOGIC(StdImpact070_T);
		REGISTER_LOGIC(StdImpact071_T);
		REGISTER_LOGIC(StdImpact072_T);
		REGISTER_LOGIC(StdImpact073_T);
		REGISTER_LOGIC(StdImpact074_T);
		REGISTER_LOGIC(StdImpact075_T);
		REGISTER_LOGIC(StdImpact076_T);
		REGISTER_LOGIC(StdImpact077_T);
		REGISTER_LOGIC(StdImpact078_T);
		REGISTER_LOGIC(StdImpact079_T);
		*/
		m_bReady = TRUE;
		return TRUE;
		__LEAVE_FUNCTION		
		return FALSE;
	};

	using namespace Combat_Module::Special_Obj_Module;
	template<>
	inline BOOL LogicManager_T<SpecialObjLogic_T, SpecialObjLogic_T::MAX_ID>::Init(VOID)
	{
		__ENTER_FUNCTION
		REGISTER_LOGIC(TrapLogic_T);
		m_bReady = TRUE;
		return TRUE;
		__LEAVE_FUNCTION		
		return FALSE;
	};

	using namespace Combat_Module::Skill_Module;
	template<>
		inline BOOL LogicManager_T<SkillLogic_T, MAX_SKILL_LOGIC_ID>::Init(VOID)
	{
		__ENTER_FUNCTION
		//Base Skill
		REGISTER_LOGIC(ImpactsToTarget_T);
		REGISTER_LOGIC(ImpactsToTargetAndSelf_T);
		REGISTER_LOGIC(TeleportAndImpacts_T);
		
		//Passive Skill		
		REGISTER_LOGIC(ArmorMastery_T);
		REGISTER_LOGIC(WeaponMastery_T);

		//Common skill
		REGISTER_LOGIC(CommonSkill001_T);
		REGISTER_LOGIC(CommonSkill002_T);
		REGISTER_LOGIC(CommonSkill003_T);
		
		// Shao Lin
		//Ming Jiao
		REGISTER_LOGIC(MingJiaoSkill001_T);
		REGISTER_LOGIC(MingJiaoSkill002_T);
		
		//GaiBang
		REGISTER_LOGIC(GaiBangSkill001_T);
		//WuDang
		REGISTER_LOGIC(WuDangSkill001_T);

		//EMei
		REGISTER_LOGIC(EMeiSkill001_T);

		//XingXiu
		
		//DaLi
		REGISTER_LOGIC(DaLiSkill001_T);
		//TianShan
		REGISTER_LOGIC(TianShanSkill001_T);
		//XiaoYao
		REGISTER_LOGIC(XiaoYaoSkill001_T);
		REGISTER_LOGIC(XiaoYaoSkill002_T);

		//Pet Skill
		REGISTER_LOGIC(PetSkill001_T);
		m_bReady = TRUE;
		return TRUE;
		__LEAVE_FUNCTION		
		return FALSE;
	};
	using namespace MenPai_Module;
	template<>
		inline BOOL LogicManager_T<MenPai_T, MenPai_T::NUMBER_OF_MENPAI>::Init(VOID)
	{
		__ENTER_FUNCTION
		//...
		REGISTER_LOGIC(DaLi_T);
		REGISTER_LOGIC(EMei_T);
		REGISTER_LOGIC(GaiBang_T);
		REGISTER_LOGIC(MingJiao_T);
		REGISTER_LOGIC(ShaoLin_T);
		REGISTER_LOGIC(TianShan_T);
		REGISTER_LOGIC(WuDang_T);
		REGISTER_LOGIC(XiaoYao_T);
		REGISTER_LOGIC(XingXiu_T);
		REGISTER_LOGIC(WuMenPai_T);
		m_bReady = TRUE;
		return TRUE;
		__LEAVE_FUNCTION		
		return FALSE;
	};
}
//template instances
namespace MenPai_Module
{
	//g_MenPaiLogicList
	using namespace Combat_Module;
	typedef class LogicManager_T<MenPai_T, MenPai_T::NUMBER_OF_MENPAI> MenPaiLogicManager_T;
	extern MenPaiLogicManager_T g_MenPaiLogicList;
};
namespace Combat_Module
{
	namespace Impact_Module
	{
		//g_ImpactLogicList
		typedef class LogicManager_T<ImpactLogic_T, NUMBER_OF_IMPACT_LOGIC_IDS> ImpactLogicManager_T;
		extern ImpactLogicManager_T g_ImpactLogicList;
	};
	namespace Special_Obj_Module
	{
		//g_SpecialObjLogicList
		typedef class LogicManager_T<SpecialObjLogic_T, SpecialObjLogic_T::MAX_ID> SpecialObjLogicManager_T;
		extern SpecialObjLogicManager_T g_SpecialObjLogicList;
	};
	namespace Skill_Module
	{
		//g_SkillLogicList
		typedef class LogicManager_T<SkillLogic_T, MAX_SKILL_LOGIC_ID> SkillLogicManager_T;
		extern SkillLogicManager_T g_SkillLogicList;
	};
};
#endif //LOGICMANAGER_H
