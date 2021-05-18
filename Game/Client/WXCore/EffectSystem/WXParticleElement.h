/********************************************************************
filename:   WXParticleElement.h

purpose:    a particle system effect element.
*********************************************************************/

#ifndef __PARTICLEELEMENT_H__
#define __PARTICLEELEMENT_H__

// fairy header
#include "WXEffectElement.h"
#include "Core/WXPrerequisites.h"

// ogre header
namespace Ogre	{
	class ParticleSystem;
}

namespace WX	{

	class ParticleElement : public EffectElement
	{
	public:

		// 参数命令
		class CmdParticleSystem : public Ogre::ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif 
        };       

		/////////////////////////////////////////////////////////////
		ParticleElement(const String &type, System *system);
		~ParticleElement();
		
		virtual bool createRenderInstance(void);
		virtual void destroyRenderInstance(void);
		virtual void updateRenderInstance( Real time, TransformInfo &info );

		virtual void destroyMe(void);

		void setParticleSystemName(const String &name);

		const String &getParticleSystemName(void) const	{	return mParticleSystemName;	}

		Ogre::ParticleSystem * getParticleSystem(void)	
		{
			return mParticleSystem;
		}

		void setParticleSystem(Ogre::ParticleSystem *system)
		{
			assert (system);
			mParticleSystem = system;
		}

		String mParticleSystemName;

		virtual void sleep(void);
		
		virtual void active(void);

		virtual bool existVisualElements(void);

		virtual void shutdown(void);

        virtual void setColour( const Ogre::ColourValue& colour );

        /// 设置visible flag
        virtual void setVisibleFlag(uint32 flags);

        /// 设置特效的显示或隐藏
        virtual void setVisible(bool visible);

	protected:

		bool _createParticleSystem(void);
		void _destroyParticleSystem(void);

        /// 设置colour affector中的颜色
        void _setAffectorColour(void);

	protected:

		Ogre::ParticleSystem *mParticleSystem;

		bool mIsEmitterEnabled;

        /// 保存粒子系统中原来的颜色，可能是颜色影响器中的原颜色，也可能是粒子系统的颜色
        typedef std::map<String, Ogre::ColourValue> ColourMap;
        ColourMap mColourMap;

        /// 需要在update的时候更改颜色
        bool mNeedUpdateColour;
        /// 当前设置的颜色
        Ogre::ColourValue mCurrentColour;

		////////////////////////////////////////////////////////////////
		static CmdParticleSystem msParticleSystemCmd;
	};
}

#endif