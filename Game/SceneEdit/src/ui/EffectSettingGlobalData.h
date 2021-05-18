/********************************************************************
filename:   EffectSettingGlobalData.h

purpose:    save the global infomation when editing the effect and skill.
*********************************************************************/

#ifndef __EffectSettingGlobalData_H__
#define __EffectSettingGlobalData_H__

#include <OgrePrerequisites.h>

enum CurrentEditEffectType
{
	CEFT_NONE,
	CEFT_PARTICLE,
	CEFT_RIBBON,
	CEFT_EFFECT_BEAM,
	CEFT_EFFECT_PROJECTOR,
    CEFT_EFFECT_BILLBOARDSET,
};

namespace WX {
	class Skill;
	class Effect;
}

class EffectSettingGlobalData
{
public:

	EffectSettingGlobalData() :
	  mCurrentSkill(NULL), mCurrentParticleSystem(NULL), mCurrentParticleTemplateName(""),
		  mCurrentEditEffectType(CEFT_NONE), mCurrentRibbonIndex(0), mCurrentEffect(NULL),
		  mOriginProjectorMaterial(""),
		  mCurrentEffectElementIndex(0)
	  {
	  }

	  WX::Skill *mCurrentSkill;
	  unsigned short mCurrentRibbonIndex;
	  Ogre::ParticleSystem *mCurrentParticleSystem;
	  Ogre::String mCurrentParticleTemplateName;

	  WX::Effect *mCurrentEffect;
	  unsigned short mCurrentEffectElementIndex;

	  Ogre::String mOriginProjectorMaterial;

	  CurrentEditEffectType mCurrentEditEffectType;
}; 

extern EffectSettingGlobalData gEffectSettingGlobalData;

#endif