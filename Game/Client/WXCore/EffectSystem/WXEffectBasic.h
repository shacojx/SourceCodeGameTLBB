/********************************************************************
filename:   FairyEffectBasic.h

purpose:    the basic of effect and effect element.
*********************************************************************/

#ifndef __EFFECTBASIC_H__
#define __EFFECTBASIC_H__

// fairy header
#include "Core/WXPrerequisites.h"

// ogre header
#include <OgreVector3.h>
#include <OgreColourValue.h>

namespace Ogre	{
	class SceneNode;
}

namespace WX	{
	class EffectManager;
	class System;
}

namespace WX	{

	/// the transform info of this effect, this will set the position and orienetation of 
	/// the effect every frame
	struct TransformInfo
	{
		TransformInfo( Ogre::Vector3 pos = Ogre::Vector3::ZERO,
			Ogre::Quaternion rotation = Ogre::Quaternion::IDENTITY ) :
		mPosition(pos), mRotation(rotation)
		{
		}

		Ogre::Vector3 mPosition;
		Ogre::Quaternion mRotation;
	};

	typedef std::vector<TransformInfo> TransformInfos;

	class EffectBasic
	{
	public:

		/**
			constructor
		@param system the fairy system, so we can get the info of the scene.
		*/
		EffectBasic( System *system );
		virtual ~EffectBasic();		

		void setSceneNode( Ogre::SceneNode *node )
		{
			assert (node);
			mBasicNode = node;
		}

		/**	delete the scene node of this effect */
		void deleteSceneNode(void);	

		Ogre::SceneNode *getBasicNode(void)	{	return mBasicNode;	}

		/** dump the scene node info ,for debug */
		void dumpSceneNode(Ogre::SceneNode *node, bool create = true) const;

		/** 该特效是否仍可见（例如粒子系统还在往外发射）
		*/
		virtual bool existVisualElements(void)	{	return false;	}

		/// 设置位移信息或旋转信息
		virtual void setTransformInfo( const TransformInfo &info ) = 0;

        /// 设置特效的显示或隐藏
        virtual void setVisible(bool visible);

        virtual void setColour( const Ogre::ColourValue& colour ) { }

        /// 设置visible flag
        virtual void setVisibleFlag(uint32 flags) = 0;

	protected:

		/// the scene node of the effect
		Ogre::SceneNode *mBasicNode;

		System *mSystem;

        bool mVisible;
	};
}
#endif