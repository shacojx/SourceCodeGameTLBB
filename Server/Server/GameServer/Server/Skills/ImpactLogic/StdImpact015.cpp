#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact015.cpp
// 功能说明：效果--State: 修正力量、耐力、灵性、定力、敏捷
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact015.h"
#include "Obj_Human.h"
#include "Obj_Pet.h"
namespace Combat_Module
{
	namespace Impact_Module
	{
		BOOL StdImpact015_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetRefixRate(rImp, 0);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact015_T::MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const
		{
			__ENTER_FUNCTION
			if(Obj::OBJ_TYPE_HUMAN==rMe.GetObjType()||Obj::OBJ_TYPE_PET==rMe.GetObjType())
			{
				if(0!=GetStrRefix(rImp))
				{
					rMe.MarkStrRefixDirtyFlag();
				}
				if(0!=GetConRefix(rImp))
				{
					rMe.MarkConRefixDirtyFlag();
				}
				if(0!=GetSprRefix(rImp))
				{
					rMe.MarkSprRefixDirtyFlag();
				}
				if(0!=GetIntRefix(rImp))
				{
					rMe.MarkIntRefixDirtyFlag();
				}
				if(0!=GetDexRefix(rImp))
				{
					rMe.MarkDexRefixDirtyFlag();
				}
			}
			__LEAVE_FUNCTION
		}
		BOOL StdImpact015_T::GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const
		{
			__ENTER_FUNCTION
			if(Obj::OBJ_TYPE_HUMAN==rMe.GetObjType()||Obj::OBJ_TYPE_PET==rMe.GetObjType())
			{
				INT nRefixRate = GetRefixRate(rImp);
				nRefixRate += 100;
				INT nValue = 0;
				switch (nIdx)
				{
					case CharIntAttrRefixs_T::REFIX_STR:
						{
							if(0!=GetStrRefix(rImp))
							{
								nValue = Float2Int((GetStrRefix(rImp)*nRefixRate)/100.0f);
								rIntAttrRefix += nValue;
								return TRUE;
							}
						}
						break;
					case CharIntAttrRefixs_T::REFIX_CON:
						{
							if(0!=GetConRefix(rImp))
							{
								nValue = Float2Int((GetConRefix(rImp)*nRefixRate)/100.0f);
								rIntAttrRefix += nValue;
								return TRUE;
							}
						}
						break;
					case CharIntAttrRefixs_T::REFIX_SPR:
						{
							if(0!=GetSprRefix(rImp))
							{
								nValue = Float2Int((GetSprRefix(rImp)*nRefixRate)/100.0f);
								rIntAttrRefix += nValue;
								return TRUE;
							}
						}
						break;
					case CharIntAttrRefixs_T::REFIX_INT:
						{
							if(0!=GetIntRefix(rImp))
							{
								nValue = Float2Int((GetIntRefix(rImp)*nRefixRate)/100.0f);
								rIntAttrRefix += nValue;
								return TRUE;
							}
						}
						break;
					case CharIntAttrRefixs_T::REFIX_DEX:
						{
							if(0!=GetDexRefix(rImp))
							{
								nValue = Float2Int((GetDexRefix(rImp)*nRefixRate)/100.0f);
								rIntAttrRefix += nValue;
								return TRUE;
							}
						}
						break;
					default:
						break;
				}
			}
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL StdImpact015_T::RefixPowerByRate(OWN_IMPACT & rImp, INT nRate) const
		{
			__ENTER_FUNCTION
			nRate += GetRefixRate(rImp);
			SetRefixRate(rImp, nRate);
			__LEAVE_FUNCTION
			return TRUE;
		}
	};
};

