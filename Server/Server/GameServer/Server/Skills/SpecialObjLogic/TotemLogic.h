///////////////////////////////////////////////////////////////////////////////
// 文件名：TotemLogic.h
// 功能说明：图腾类似对象的基础类，用于声明接口和实现默认操作
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#ifndef TOTEMLOGIC_H
#define TOTEMLOGIC_H
#include "Type.h"
#include "SpecialObjLogic.h"

namespace Combat_Module
{
	namespace Special_Obj_Module
	{
		class TotemLogic_T : public SpecialObjLogic_T
		{
			public:
				enum
				{
					TYPE = TOTEM_OBJ,
				};
				TotemLogic_T() {};
				~TotemLogic_T() {};
				virtual VOID ActiveNow(Obj_Special& rThisObj) const;
				virtual VOID FadeOutNow(Obj_Special& rThisObj) const;
				virtual VOID OnTick(Obj_Special& rThisObj) const;
				virtual BOOL IsScanedTargetValid(Obj_Special& rThis, Obj_Character& rScaned) const {return FALSE;};
				virtual BOOL IsScanedTargetValid(Obj_Special& rThis, Obj_Special& rScaned) const {return FALSE;};
			protected:
				VOID ScanTarget(VOID){};
				VOID EffectOnTarget(VOID){};
				VOID BroadCastActiveMsg(VOID){};
				VOID BroadCastFadeOutMsg(VOID){};
			private:
		};
	}
}
#endif //TOTEMLOGIC_H
