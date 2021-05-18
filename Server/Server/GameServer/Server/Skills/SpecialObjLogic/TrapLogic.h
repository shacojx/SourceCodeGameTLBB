///////////////////////////////////////////////////////////////////////////////
// 文件名：TrapLogic.h
// 功能说明：陷阱类似对象的基础类，用于声明接口和实现默认操作
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#ifndef TRAPLOGIC_H
#define TRAPLOGIC_H
#include "Type.h"
#include "SpecialObjLogic.h"
#include "Obj_Character.h"
namespace Combat_Module
{
	namespace Special_Obj_Module
	{
		class TrapLogic_T : public SpecialObjLogic_T
		{
			public:
				enum
				{
					TYPE = TRAP_OBJ,
				};
				enum
				{
					ID = TRAP_LOGIC_ID,
				};
				enum
				{
					MASK_ACTIVATE_WHEN_FADE_OUT = 1,
					MASK_TRIGGER_BY_CHARACTER = 2,
					MASK_TRIGGER_BY_TRAP =4,
				};
				class DescriptorIndex_T
				{
					public:
						enum
						{
							IDX_IMPACT0 = 0,
							IDX_IMPACT1,
							IDX_IMPACT2_ACTIVATE_ODDS,
							IDX_IMPACT2,
						};
				};
				// public interface
				TrapLogic_T(){};
				~TrapLogic_T(){};
				virtual INT	GetType(VOID) const {return TYPE;};
				virtual VOID ForceActivate(Obj_Special& rThis) const;
				virtual VOID ForceFadeOut(Obj_Special& rThis) const;
				VOID OnTick(Obj_Special& rThis) const;
				VOID OnTimeOver(Obj_Special& rThis) const;
				BOOL TriggerCheck(Obj_Special& rMe) const;
				//需要子类实现
				virtual BOOL CanTriggerBySpecificObj(Obj_Special& rMe, Obj_Character& rTar) const;
				virtual BOOL CanTriggerBySpecificObj(Obj_Special& rMe, Obj_Special& rOther) const;
				virtual BOOL IsScanedTargetValid(Obj_Special& rThis, Obj_Character& rScaned) const;
				virtual BOOL IsScanedTargetValid(Obj_Special& rThis, Obj_Special& rScaned) const;
			protected: //interface for interval using 
				INT	GetImpact0(Obj_Special& rThis) const;
				INT	GetImpact1(Obj_Special& rThis) const;
				INT	GetImpact2(Obj_Special& rThis) const;
				INT	GetImpact2ActivateOdds(Obj_Special& rThis) const;
				BOOL CanTriggerByTrap(Obj_Special& rThis) const;
				BOOL ActivateWhenFadeOut(Obj_Special& rThis) const;
				BOOL CanTriggerByCharacter(Obj_Special& rThis) const;
				BOOL EffectOnTrap(Obj_Special& rThis, Obj_Special& rTar) const;
				//需要子类实现
				virtual BOOL EffectOnChar(Obj_Special& rThis, Obj_Character& rTar) const;
			private:
				VOID Activate(Obj_Special& rMe) const;
		};
	}
}

#endif //TRAPLOGIC_H
