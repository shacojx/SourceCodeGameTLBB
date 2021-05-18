/********************************************************************
filename:   WXEffectElement.h

purpose:    a effect contains a lot of effect elements,these elements may be particle
			system, or a mesh, or other things.
*********************************************************************/

#ifndef __EFFECTELEMENT_H__
#define __EFFECTELEMENT_H__

// ogre header
#include <OgreStringInterface.h>

// fairy header
#include "Core/WXPrerequisites.h"
#include "WXEffectBasic.h"
#include "WXEffect.h"

namespace WX	{
	class System;
}

namespace WX	{

	class EffectElement : public Ogre::StringInterface, public EffectBasic
	{
	public:

		class CmdLifeTime : public Ogre::ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif
        };
        class CmdStartTime : public Ogre::ParamCommand
		{
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif      
        };   
		class CmdRelPos : public Ogre::ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif      
        };		
        class CmdRelOrientation : public Ogre::ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif      
        };

	public:
		/////////////////////////////////////////////////////////////
		EffectElement(const String &type, System *system);
		virtual ~EffectElement();

		const String &getType(void) const 	{	return mElementType;	}

		/// dump the info of this element.
		void dump(std::ofstream &of) const;

		virtual bool createRenderInstance(void) = 0;

		virtual void destroyRenderInstance(void)	{	mIsRenderInstanceCreated = false;	}

		virtual void updateRenderInstance( Real time, TransformInfo &info ) {	}

		virtual void setTransformInfo( const TransformInfo &info );

		/// update this element, will be called by effect.
		void update( Real time, TransformInfo &info );

		///	set the relative position of the parent effect
		void setRelPos( const Ogre::Vector3 &pos );

		///	get the relative position of the parent effect
		const Ogre::Vector3& getRelPos(void) const	{	return mRelPos;	}

        /// set the relative orientation of the parent effect
        void setRelOrientation( const Ogre::Quaternion& orientation );
        /// get the relative orientation of the parent effect
        const Ogre::Quaternion& getRelOrientation(void) const
        {
            return mRelOrientation;
        }

		/**	set the born time of this element
		@param time second
		*/
		virtual void setStartTime(Real time)	{	mStartTime = time;	}
		/**	get the born time of this element
		@param time second
		*/
		Real getStartTime(void)	const {	return mStartTime;	}
		
		/**	set the life time of this element
		@param time second
		*/
		void setLifeTime(Real time)	{	mLifeTime = time;	}
		/**	get the born time of this element
		@param time second
		*/
		Real getLifeTime(void) const	{	return mLifeTime;	}

		/** let this element sleep.
		@remark when the element reach its life time, it will be not deleted, it will just
			    be sleep(disappear in scene)
		*/
		virtual void sleep(void);

		/** the opposite operation of sleep.
		*/
		virtual void active(void);

		bool isActive(void)	{	return mIsActive;	}

		/**	set the effect parent */
		void setParent( Effect *parent )	
		{	
			assert (parent);
			mEffectParent = parent;
		}
		/**	get the effect parent */
		Effect * getParent(void)	{	return mEffectParent;	}
		
		/**	create the scene node of the element
		@param parentNode the parent scene node of the node of element
		*/
		void createSceneNode( Ogre::SceneNode *parentNode );

		virtual void destroyMe(void);
		bool isDestroy(void)	{	return mIsDestroy;	}

		virtual void updateExtraTransformInfos( const TransformInfos &infos )	{	}

		virtual void shutdown(void)	{	}

	protected:

        bool initDefaults(const String &mType);

	protected:
	
	    String mElementType;		

		/// the render instance is created?(maybe we don't need this member data)
		bool mIsRenderInstanceCreated;

		/// the life time of this element, is zero by default, if it is zero, the element will live forever.
		Real mLifeTime;

		/// the born time of this element.
		Real mStartTime;

		/// current life time of this element.
		Real mCurrentLifeTime;

		/// current born time of this element, if this equal to mStartTime, the element will be born.
		Real mCurrentStartTime;

		/// the position relative to its parent scene node.
		Ogre::Vector3 mRelPos;

        /// the orientation relative to its parent scene node.
        Ogre::Quaternion mRelOrientation;

		bool mIsActive;

		/// the parent effect
		Effect *mEffectParent;

		bool mIsDestroy;

		////////////////////////////////////////////////////////////////
		static CmdStartTime msStartTimeCmd;
		static CmdLifeTime msLifeTimeCmd;
		static CmdRelPos msRelPosCmd;
        static CmdRelOrientation msRelOrientationCmd;
	};
}

#endif