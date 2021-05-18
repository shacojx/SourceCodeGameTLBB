///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact051.h
// 功能说明：效果--分身效果
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT051_H
#define STDIMPACT051_H

#include "ImpactLogic.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact051_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_051,
				};
				class OwnImpactParamIndex_T
				{
					public:
						enum
						{
							IDX_SUMMONER_ID=0,
							IDX_SHADOW_GUARD_ID,
						};
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_RESERVED,
						};
				};
				StdImpact051_T() {};
				~StdImpact051_T() {};
				BOOL IsOverTimed(VOID) const {return TRUE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;

				INT GetSummonerID(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_SUMMONER_ID);};
				VOID SetSummonerID(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_SUMMONER_ID, nValue);};

				INT GetShadowGuardID(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_SHADOW_GUARD_ID);};
				VOID SetShadowGuardID(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_SHADOW_GUARD_ID, nValue);};

			public: //message handler
				VOID OnFadeOut(OWN_IMPACT& rImp, Obj_Character& rMe) const;
			protected:
				BOOL SpecialHeartBeatCheck(OWN_IMPACT& rImp, Obj_Character& rMe) const;
			private:
		}SOT_ShadowGuard_T;
	};
};
#endif //STDIMPACT051_H
