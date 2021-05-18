#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact017.cpp
// 功能说明：效果--效果--免疫特定的效果
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "GameTable.h"
#include "StdImpact017.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		BOOL StdImpact017_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetRefixRate(rImp, 0);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL StdImpact017_T::RefixPowerByRate(OWN_IMPACT & rImp, INT nRate) const
		{
			__ENTER_FUNCTION
			nRate += GetRefixRate(rImp);
			SetRefixRate(rImp, nRate);
			__LEAVE_FUNCTION
			return TRUE;
		}
		INT StdImpact017_T::OnFiltrateImpact(OWN_IMPACT& rImp, Obj_Character& rMe, OWN_IMPACT& rImpactNeedCheck) const
		{
			__ENTER_FUNCTION
			INT nImmunoLevel = GetImmunoLevel(rImp);
			INT nRate = GetRefixRate(rImp);
			nRate += 100;
			nImmunoLevel = Float2Int((nImmunoLevel*nRate)/100.0f);
			IDCollection_T const* pCollection = NULL;
			for(INT nIdx=0; COLLECTION_COUNT>nIdx; ++nIdx)
			{
				INT nCollectionID = GetCollectionByIndex(rImp, nIdx);
				if(INVALID_ID != nCollectionID)
				{
					pCollection = g_IDCollectionMgr.GetInstanceByID(nCollectionID);
					if(NULL!=pCollection)
					{
						if(0<pCollection->GetCollectionSize())
						{
							if(nImmunoLevel>=Impact_GetLevel(rImpactNeedCheck))
							{
								if(TRUE==Impact_IsImpactInCollection(rImpactNeedCheck, nCollectionID))
								{
									return MissFlag_T::FLAG_IMMU; //免疫该效果
								}
							}
						}
					}
				}
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};

