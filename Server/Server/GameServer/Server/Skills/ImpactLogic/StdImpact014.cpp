#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact014.cpp
// 功能说明：效果--昏迷,沉默，定身，无敌状态
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact014.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		BOOL StdImpact014_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetRefixRate(rImp, 0);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact014_T::MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const
		{
			__ENTER_FUNCTION
			if(0 <= GetCanAction1Refix(rImp))
			{
				rMe.MarkCanAction1DirtyFlag();
			}
			if(0 <= GetCanAction2Refix(rImp))
			{
				rMe.MarkCanAction2DirtyFlag();
			}
			if(0 <= GetCanMoveRefix(rImp))
			{
				rMe.MarkCanMoveDirtyFlag();
			}
			if(0 <= GetUnbreakableRefix(rImp))
			{
				rMe.MarkUnbreakableDirtyFlag();
			}
			if(0 != GetMoveSpeedRefix(rImp))
			{
				rMe.MarkMoveSpeedRefixDirtyFlag();
			}
			if(0 != GetStealthLevelRefix(rImp))
			{
				rMe.MarkStealthLevelRefixDirtyFlag();
			}
			if(0 != GetDetectLevelRefix(rImp))
			{
				rMe.MarkDetectLevelRefixDirtyFlag();
			}
			if(0 <= GetModelRefix(rImp))
			{
				rMe.MarkModelIDRefixDirtyFlag();
			}
			if(0 <= GetMountRefix(rImp))
			{
				rMe.MarkMountIDRefixDirtyFlag();
			}
			__LEAVE_FUNCTION			
		}
		BOOL StdImpact014_T::GetBoolAttrRefix(OWN_IMPACT& rImp, Obj_Character& rMe, CharBoolAttrs_T::Index_T nIdx, BOOL& rBoolAttrRefix) const
		{
			__ENTER_FUNCTION
			switch (nIdx)
			{
				case CharBoolAttrs_T::ATTR_CAN_ACTION1:
					{
						if(0<=GetCanAction1Refix(rImp))
						{
							rBoolAttrRefix = 1==GetCanAction1Refix(rImp);
							return TRUE;
						}
					}
					break;
				case CharBoolAttrs_T::ATTR_CAN_ACTION2:
					{
						if(0<=GetCanAction2Refix(rImp))
						{
							rBoolAttrRefix = 1==GetCanAction2Refix(rImp);
							return TRUE;
						}
					}
					break;
				case CharBoolAttrs_T::ATTR_CAN_MOVE:
					{
						if(0<=GetCanMoveRefix(rImp))
						{
							rBoolAttrRefix = 1==GetCanMoveRefix(rImp);
							return TRUE;
						}
					}
					break;				
				case CharBoolAttrs_T::ATTR_UNBREAKABLE:
					{
						if(0<=GetUnbreakableRefix(rImp))
						{
							rBoolAttrRefix = 0<GetUnbreakableRefix(rImp);
							return TRUE;
						}
					}
					break;				
				default:
					break;
			}
			__LEAVE_FUNCTION						
			return FALSE;
		}
		BOOL StdImpact014_T::GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const
		{
			__ENTER_FUNCTION
			INT nRefixRate = GetRefixRate(rImp);
			nRefixRate += 100;
			INT nValue = 0;
			switch (nIdx)
			{
				case CharIntAttrRefixs_T::REFIX_MOVE_SPEED:
					{
						if(0!=GetMoveSpeedRefix(rImp))
						{
							nValue = Float2Int((GetMoveSpeedRefix(rImp)*nRefixRate)/100.0f);
							nValue = Float2Int((rMe.GetBaseMoveSpeed()*nValue)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_STEALTH_LEVEL:
					{
						if(0!=GetStealthLevelRefix(rImp))
						{
							rIntAttrRefix += GetStealthLevelRefix(rImp);
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_DETECT_LEVEL:
					{
						if(0!=GetDetectLevelRefix(rImp))
						{
							rIntAttrRefix += GetDetectLevelRefix(rImp);
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_MODEL_ID:
					{
						if(INVALID_ID!=GetModelRefix(rImp))
						{
							rIntAttrRefix = GetModelRefix(rImp);
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_MOUNT_ID:
					{
						if(INVALID_ID!=GetMountRefix(rImp))
						{
							rIntAttrRefix = GetMountRefix(rImp);
							return TRUE;
						}
					}
					break;
				default:
					break;
			}
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL StdImpact014_T::RefixPowerByRate(OWN_IMPACT & rImp, INT nRate) const
		{
			__ENTER_FUNCTION
			nRate += GetRefixRate(rImp);
			SetRefixRate(rImp, nRate);
			__LEAVE_FUNCTION
			return TRUE;
		}
	};
};



