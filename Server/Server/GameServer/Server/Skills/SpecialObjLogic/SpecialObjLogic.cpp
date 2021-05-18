#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：SpecialObjLogic.cpp
// 功能说明：特殊对象的基础类，用于声明接口和实现默认操作
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#include "SpecialObjLogic.h"
#include "LogicManager.h"

namespace Combat_Module
{
	namespace Special_Obj_Module
	{
		SpecialObjLogic_T const* ObjSpecial_GetLogic(Obj_Special& rObj)
		{
		__ENTER_FUNCTION
			if(INVALID_ID==rObj.GetDataID())
			{
				return NULL;
			}
			SpecialObjData_T const* pData = rObj.GetDataRecord();
			if(NULL==pData)
			{
				return NULL;
			}
			ID_t nLogicID = pData->GetLogicID();
			if(INVALID_ID==nLogicID)
			{
				return NULL;
			}

			return g_SpecialObjLogicList.GetLogicById(nLogicID);
		__LEAVE_FUNCTION
			return NULL;
		}
		INT SpecialObjLogic_T::GetDescriptorValueByIndex(Obj_Special& rThis, INT nIdx) const
		{
		__ENTER_FUNCTION
			if(INVALID_ID==rThis.GetDataID())
			{
				AssertEx(FALSE, "[SpecialObjLogic_T::GetDescriptorValueByIndex]: Can't find the specific data for this obj.");
				return 0;
			}
			SpecialObjData_T const* pData = rThis.GetDataRecord();
			if(NULL==pData)
			{
				AssertEx(FALSE, "[SpecialObjLogic_T::GetDescriptorValueByIndex]: Can't find the specific data for this obj.");
				return 0;
			}
			if(0>nIdx || DESCRIPTOR_NUMBER<=nIdx)
			{
				AssertEx(FALSE, "[SpecialObjLogic_T::GetDescriptorValueByIndex]: Illegal Index Found.");
				return 0;
			}
			Descriptor_T const * pDescriptor = pData->GetDescriptorByIndex(nIdx);
			if(NULL==pDescriptor)
			{
				AssertEx(FALSE, "[SpecialObjLogic_T::GetDescriptorValueByIndex]: Illegal Index Found.");
				return 0;				
			}
			return pDescriptor->GetValue();
		__LEAVE_FUNCTION
			return 0;
		}
	};
};
