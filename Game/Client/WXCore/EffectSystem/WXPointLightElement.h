/********************************************************************
filename:   WXPointLightElement.h

purpose:    点光源特效元素，对Ogre的Light进行封装，可以使发出的特效对
			周围的物体进行影响，如发出的火球可以照亮地面
*********************************************************************/

#ifndef _POINTLIGHTELEMENT_H_
#define _POINTLIGHTELEMENT_H_

// fairy header
#include "WXEffectElement.h"
#include "Core/WXPrerequisites.h"

#include <OgreColourValue.h>

namespace Ogre	{
	class Light;
}

namespace WX	{

	class PointLightElement : public EffectElement
	{
	public:

        class CmdDiffuse : public Ogre::ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif
        };

        class CmdSpecular : public Ogre::ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif      
        };

        class CmdRange : public Ogre::ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif      
        };

        class CmdAttenuationConstant : public Ogre::ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif      
        };

        class CmdAttenuationLinear : public Ogre::ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif      
        };

        class CmdAttenuationQuadric : public Ogre::ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif      
        };

		/////////////////////////////////////////////////////////////
		PointLightElement(const String &type, System *system);
		~PointLightElement();

		virtual bool createRenderInstance(void);
		virtual void destroyRenderInstance(void);

        void setDiffuse(const Ogre::ColourValue& diffuse);
        Ogre::ColourValue getDiffuse(void) const
        {
            return mDiffuse;
        }

        void setSpecular(const Ogre::ColourValue& specular);
        Ogre::ColourValue getSpecular(void) const
        {
            return mSpecular;
        }

        void setRange(Real range);
        Real getRange(void) const
        {
            return mRange;
        }

        void setAttenuationConstant(Real constant);
        Real getAttenuationConstant(void) const
        {
            return mAttenuationConstant;
        }

        void setAttenuationLinear(Real linear);
        Real getAttenuationLinear(void) const
        {
            return mAttenuationLinear;
        }

        void setAttenuationQuadric(Real quadric);
        Real getAttenuationQuadric(void) const
        {
            return mAttenuationQuadric;
        }

        /// 设置visible flag
        virtual void setVisibleFlag(uint32 flags);

	protected:

		bool _createLight(void);
		void _destroyLight(void);

	protected:

		Ogre::Light* mLight;

        Ogre::ColourValue mDiffuse;
        Ogre::ColourValue mSpecular;

        Real mRange;
        Real mAttenuationConstant;
        Real mAttenuationLinear;
        Real mAttenuationQuadric;

		////////////////////////////////////////////////////////////////
        static CmdDiffuse  msDiffuseCmd;
        static CmdSpecular msSpecularCmd;
        static CmdRange msRangeCmd;
        static CmdAttenuationConstant msAttenuationConstantCmd;
        static CmdAttenuationLinear msAttenuationLinearCmd;
        static CmdAttenuationQuadric msAttenuationQuadricCmd;
	};
}

#endif