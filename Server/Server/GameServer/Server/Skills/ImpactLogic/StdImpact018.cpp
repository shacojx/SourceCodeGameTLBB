#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact018.cpp
// 功能说明：效果--效果--免疫特定的效果
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "GameTable.h"
#include "StdImpact018.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		BOOL StdImpact018_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetRefixRate(rImp, 0);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL StdImpact018_T::RefixPowerByRate(OWN_IMPACT & rImp, INT nRate) const
		{
			__ENTER_FUNCTION
			nRate += GetRefixRate(rImp);
			SetRefixRate(rImp, nRate);
			__LEAVE_FUNCTION
			return TRUE;
		}

		VOID StdImpact018_T::OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			if(FALSE==rMe.IsAlive() || FALSE==rMe.IsActiveObj())
			{
				return;
			}
			INT nDispelLevel = GetDispelLevel(rImp);
			INT nDispelCount = GetDispelCount(rImp);
			INT nRate = GetRefixRate(rImp);
			INT nDispeled = 0;
			nRate += 100;
			nDispelCount = Float2Int((nDispelCount*nRate)/100.0f); //强化只影响驱散数目
			IDCollection_T const* pCollection = NULL;
			for(INT nIdx=0; COLLECTION_NUMBER>nIdx; ++nIdx)
			{
				INT nCollectionID = GetCollectionByIndex(rImp, nIdx);
				if(INVALID_ID != nCollectionID)
				{
					pCollection = g_IDCollectionMgr.GetInstanceByID(nCollectionID);
					if(NULL!=pCollection)
					{
						nDispeled = rMe.Impact_DispelImpactInSpecificCollection(nCollectionID, nDispelLevel, nDispelCount);
						if(0<nDispelCount)
						{	//驱散有限个数
							if(nDispeled>=nDispelCount)
							{
								break; //驱散名额已满
							}
						}
					}
				}
			}
			__LEAVE_FUNCTION
		}
	};
};

