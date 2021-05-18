///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact009.h
// 功能说明：效果--传送角色到指定位置
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT009_H
#define STDIMPACT009_H

#include "Type.h"
#include "ImpactLogic.h"
#include "GameStruct_Impact.h"
#include "Obj_Character.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact009_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_009,
				};
				class OwnImpactParamIndex_T
				{
					public:
						enum
						{
							IDX_SCENE_ID = 0,
							IDX_POSITION_X,
							IDX_POSITION_Z,
						};
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_SCENE_ID = 0,
							IDX_POSITION_X,
							IDX_POSITION_Z,
						};
				};
				StdImpact009_T() {};
				~StdImpact009_T() {};
				BOOL IsOverTimed(VOID) const {return FALSE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;
				
				INT GetSceneID(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_SCENE_ID);}
				VOID SetSceneID(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_SCENE_ID, nValue);}

				INT GetPosition_X(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_POSITION_X);}
				VOID SetPosition_X(OWN_IMPACT& rImp, FLOAT fValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_POSITION_X, Float2Int(fValue*100));}

				INT GetPosition_Z(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_POSITION_Z);}
				VOID SetPosition_Z(OWN_IMPACT& rImp, FLOAT fValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_POSITION_Z, Float2Int(fValue*100));}
			public: //message handler
				VOID OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const;
			protected:
			private:
		} DI_Teleport_T;
	};
};
#endif //STDIMPACT009_H
