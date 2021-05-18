/********************************************************************
filename:   FairyEffectKeyframe.cpp
created:    2005/10/5
author:     Weiliang Xie (feiyurainy@163.com)

purpose:    keep the infos get from .path file
*********************************************************************/

#include "FairyEffectKeyframe.h"

// ogre header
#include <OgreResourceGroupManager.h>
#include <OgreException.h>
#include <OgreLogManager.h>
#include <OgreStringConverter.h>

namespace Fairy	{

	//////////////////////////////////////////////////////
	KeyFrames::KeyFrames() :
	mAnimationLength(0.0f)
	{
		mKeyFrames.clear();
	}

	KeyFrames::~KeyFrames()
	{
		for ( size_t i = 0; i < mKeyFrames.size(); ++i )
		{
			delete mKeyFrames[i];
			mKeyFrames[i] = NULL;
		}
		mKeyFrames.clear();
	}

	void KeyFrames::addKeyFrame( EffectKeyFrame *keyFrame )
	{
		assert (keyFrame);
		mKeyFrames.push_back( keyFrame );

		mAnimationLength = keyFrame->getRealTime();
	}
}