/********************************************************************
filename:   FairyEffectKeyframe.h
created:    2005/10/5
author:     Weiliang Xie (feiyurainy@163.com)

purpose:    keep the infos get from .path file
*********************************************************************/

#ifndef __EffectKeyframe_H__
#define __EffectKeyframe_H__

// fairy header
#include "Core/FairyPrerequisites.h"

// ogre header
#include <OgreVector3.h>
#include <OgreScriptLoader.h>
#include <OgreStringVector.h>
#include <OgreDataStream.h>

namespace Fairy	{

	class EffectKeyFrame
	{
	public:
		EffectKeyFrame( Ogre::Vector3 pos = Ogre::Vector3::ZERO, 
			Ogre::Quaternion ori = Ogre::Quaternion::IDENTITY,
			Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE, 
			Real time = 0.0f, Real realTime = 0.0f )	:
		mPosition(pos), mOrientation(ori), mScale(scale), mTime(time), mRealTime(realTime)
		{
		}		

		void setPosition(Ogre::Vector3 &pos)	{	mPosition = pos;	}
		void setOrientation(Ogre::Quaternion &ori)	{	mOrientation = ori;	}
		void setScale(Ogre::Vector3 &scale)	{	mScale = scale;	}

		Real getRealTime(void)	{	return mRealTime;	}
		void setRealTime(Real realTime)	{	mRealTime = realTime;	}

		void setTime(Real time)
		{	
			mTime = time;

			setRealTime( mTime / 4800.0f );
		}
		Real getTime(void)	{	return mTime;	}
		
		Ogre::Vector3 & getPosition(void)	{	return mPosition;	}
		Ogre::Quaternion & getOrientation(void)	{	return mOrientation;	}
		Ogre::Vector3 & getScale(void)	{	return mScale;	}

	protected:

		Ogre::Vector3 mPosition;
		Ogre::Quaternion mOrientation;
		Ogre::Vector3 mScale;
		Real mTime;
		Real mRealTime;
	};

	typedef std::vector<EffectKeyFrame *> KeyFrameArray;

	class KeyFrames
	{
	public:
		KeyFrames();
		~KeyFrames();

		void addKeyFrame( EffectKeyFrame *keyFrame );

		Real getAnimationLength() const	{	return mAnimationLength;	}

		size_t size(void)	{	return mKeyFrames.size();	}

		EffectKeyFrame *getKeyFrame( size_t index )
		{
			assert (index < mKeyFrames.size() && index >= 0 );
			return mKeyFrames[index];
		}

	protected:
		KeyFrameArray mKeyFrames;
		Real mAnimationLength;
	};
}

#endif