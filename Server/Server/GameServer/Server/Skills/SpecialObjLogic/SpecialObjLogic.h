///////////////////////////////////////////////////////////////////////////////
// 文件名：SpecialObjLogic.h
// 功能说明：特殊对象的基础类，用于声明接口和实现默认操作
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#ifndef SPECIALOBJLOGIC_H
#define SPECIALOBJLOGIC_H
#include "Type.h"
#include "GameStruct_Impact.h"
#include "Obj_Character.h"
#include "Obj_Special.h"

namespace Combat_Module
{
	namespace Special_Obj_Module
	{
		enum
		{
			SPECIAL_OBJ_TYPE = INVALID_ID,
			TRAP_OBJ,
			TOTEM_OBJ,
		};
		class SpecialObjLogic_T
		{
			public:
				enum
				{
					TYPE = SPECIAL_OBJ_TYPE,
					ID = INVALID_ID,
				};
				enum
				{
					DESCRIPTOR_NUMBER = 16,
				};
				enum
				{
					INVALID_LOGIC_ID = INVALID_ID,
					//ID start
					TRAP_LOGIC_ID = 0,
					//ID end
					MAX_ID,
				} SpecialObjLogicID_T;
				SpecialObjLogic_T(){};
				~SpecialObjLogic_T(){};
				virtual INT	GetType(VOID) const {return TYPE;};
				virtual VOID OnTick(Obj_Special& rThisObj) const {};
				virtual VOID OnTimeOver(Obj_Special& rThis) const {};
				virtual BOOL IsScanedTargetValid(Obj_Special& rThis, Obj_Character& rScaned) const {return FALSE;};
				virtual BOOL IsScanedTargetValid(Obj_Special& rThis, Obj_Special& rScaned) const {return FALSE;};
				
			protected:
				INT GetDescriptorValueByIndex(Obj_Special& rThis, INT nIdx) const;
			private:
				
		};
		SpecialObjLogic_T const* ObjSpecial_GetLogic(Obj_Special& rObj);
	}
}
#endif //SPECIALOBJLOGIC_H
