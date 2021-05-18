/********************************************************************
filename:   WXEffect.h

purpose:    a effect contains a lot of effect elements, the info of effect will
			be keep in the .effect files.
*********************************************************************/

#ifndef __FAIRYEFFECT_H__
#define __FAIRYEFFECT_H__

// fairy header
#include "Core/WXPrerequisites.h"
#include "WXEffectBasic.h"

#include <OgreStringInterface.h>

namespace WX	{

	class EffectElement;	

	class Effect : public Ogre::StringInterface, public EffectBasic
	{
	public:
		typedef std::vector<EffectElement *> ElementList;
		typedef ElementList::iterator ElementIterator;

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

		//////////////////////////////////////////////////////////////////////////
		Effect(const String &templateName, System *system);
		virtual ~Effect();

		const String& getTemplateName(void) const	{	return mTemplateName;	}
		void setTemplateName( const String &name )	{	mTemplateName = name;	}

		void dump(std::ofstream &of) const;		

		/// update the effect and its elements, will be called by a controller.
		void updateEffect(Real value);
		
		/** Assignment operator for copying.
		@remarks
		This operator deep copies all info of the effect.
		*/
        Effect& operator=(const Effect& rhs);

		virtual void setTransformInfo( const TransformInfo &info );

		void setExtraTransformInfos( const TransformInfos &infos );

		void reborn(void);

		/** add a child element */
		void addElement( EffectElement *element );
		void addElement( const String &type );

		void removeAllElements(void);

		/// remove the element,not destroy,just sleep
		void removeElement( EffectElement *element );

		/// 把effect主节点从它的父节点上卸下来，以便在之后可以挂上别的父节点
		void removeParentSceneNode(void);

		/// destroy the child element
		ElementIterator destroyElement( ElementIterator i );
		void destroyElement( EffectElement *element );
		void destroyElement( unsigned short index );

		void destroyAllElements(void);

		/**	get the number of child elements */
		unsigned short getNumElements(void) const
		{
			return static_cast<unsigned short>( mElementList.size() );
		}
	
		EffectElement *getElement( unsigned short index );

		Ogre::SceneNode* createSceneNode(void);
		void createSceneNode( Ogre::SceneNode *parentNode );

		/**	set the life time of this element
		@param time second
		*/
		void setLifeTime(Real time)	{	mLifeTime = time;	}
		/**	get the born time of this element
		@param time second
		*/
		Real getLifeTime(void) const	{	return mLifeTime;	}

		bool isAlive(void)	{ return mAliveEffect;	}

		virtual bool existVisualElements(void);

		static void setDefaultIterativeInterval(Real time)
		{
			msDefaultIterativeInterval = time;
		}

		static Real getDefaultIterativeInterval(void)
		{
			return msDefaultIterativeInterval;
		}

		void setIterativeInterval(Real time)
		{
			mIterativeInterval = time;
		}

		Real getIterativeInterval(void)
		{
			return mIterativeInterval;
		}

		void shutdown(void);

        virtual void setVisible(bool visible);

        /** 设置特效上各种元素的颜色
        @remarks 会遍历所有的元素，改变各种元素的颜色
        */
        virtual void setColour( const Ogre::ColourValue& colour );

        /// 设置visible flag
        virtual void setVisibleFlag(uint32 flags);

        void setRemoveParentNode(bool removeParentNode)
        {
            mRemoveParentNode = removeParentNode;
        }
        bool getRemoveParentNode(void)
        {
            return mRemoveParentNode;
        }

        void setDelayRemove(bool delayRemove)
        {
            mDelayRemove = delayRemove;
        }
        bool getDelayRemove(void)
        {
            return mDelayRemove;
        }

        /// 特效是否在删除时回收
        bool mPutIntoFreePool;

	protected:

		bool initDefaults(void);

		void _updateSelf(Real time);

		void _updateElements(Real time);

	protected:

		String mTemplateName;

		TransformInfo mTransformInfo;

		/// child elements
		ElementList mElementList;

		/// the life time of this element, is zero by default, if it is zero, the element will live forever.
		Real mLifeTime;

		/// current life time of this element.
		Real mCurrentLifeTime;

		bool mAliveEffect;

		Real mIterateRemainTime;
		Real mIterativeInterval;

        bool mNeedSetVisibleFlags;
        uint32 mVisibleFlags;

        bool mRemoveParentNode;

        bool mDelayRemove;

		static Real msDefaultIterativeInterval;

		////////////////////////////////////////////////////////////////
		static CmdLifeTime msLifeTimeCmd;
	};
}

#endif