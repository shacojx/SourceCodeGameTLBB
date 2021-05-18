/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2005 The OGRE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/
#include "OgreStableHeaders.h"

#include "OgreAnimationState.h"
#include "OgreException.h"
#include "OgrePoolAllocator.h"

namespace Ogre 
{

    OGRE_DECLARE_OBJECT_POOL(static, msAnimationStatePool, AnimationState);
	//---------------------------------------------------------------------
	AnimationState::AnimationState(AnimationStateSet* parent, const AnimationState &rhs)
        : mAnimationName(rhs.mAnimationName)
        , mParent(parent)
        , mTimePos(rhs.mTimePos)
        , mLength(rhs.mLength)
        , mWeight(rhs.mWeight)
        , mEnabled(rhs.mEnabled)
        , mLoop(rhs.mLoop)
	{
		mParent->_notifyDirty();
	}
	//---------------------------------------------------------------------
	AnimationState::~AnimationState()
	{
	}
    //---------------------------------------------------------------------
    AnimationState::AnimationState(const String& animName, 
		AnimationStateSet *parent, Real length)
        : mAnimationName(animName)
        , mParent(parent)
        , mTimePos(0)
        , mLength(length)
        , mWeight(1)
        , mEnabled(false)
        , mLoop(true)
    {
		mParent->_notifyDirty();
    }
    //---------------------------------------------------------------------
    const String& AnimationState::getAnimationName() const
    {
        return mAnimationName;
    }
    //---------------------------------------------------------------------
    Real AnimationState::getTimePosition(void) const
    {
        return mTimePos;
    }
    //---------------------------------------------------------------------
    void AnimationState::setTimePosition(Real timePos)
    {
		if (timePos != mTimePos)
		{
			mTimePos = timePos;
			if (mLoop)
			{
				// Wrap
				mTimePos = fmod(mTimePos, mLength);
				if(mTimePos < 0)
					mTimePos += mLength;     
			}
			else
			{
				// Clamp
				if(mTimePos < 0)
					mTimePos = 0;
				else if (mTimePos > mLength)
					mTimePos = mLength;
			}

            if (mEnabled)
                mParent->_notifyDirty();
        }

    }
    //---------------------------------------------------------------------
    Real AnimationState::getLength() const
    {
        return mLength;
    }
    //---------------------------------------------------------------------
    void AnimationState::setLength(Real len)
    {
        mLength = len;
    }
    //---------------------------------------------------------------------
    Real AnimationState::getWeight(void) const
    {
        return mWeight;
    }
    //---------------------------------------------------------------------
    void AnimationState::setWeight(Real weight)
    {
        mWeight = weight;

        if (mEnabled)
            mParent->_notifyDirty();
    }
    //---------------------------------------------------------------------
    void AnimationState::addTime(Real offset)
    {
        setTimePosition(mTimePos + offset);
    }
    //---------------------------------------------------------------------
    bool AnimationState::getEnabled(void) const
    {
        return mEnabled;
    }
    //---------------------------------------------------------------------
    void AnimationState::setEnabled(bool enabled)
    {
        mEnabled = enabled;
        mParent->_notifyAnimationStateEnabled(this, enabled);
    }
    //---------------------------------------------------------------------
    bool AnimationState::operator==(const AnimationState& rhs) const
    {
        if (mAnimationName == rhs.mAnimationName &&
            mEnabled == rhs.mEnabled &&
            mTimePos == rhs.mTimePos &&
            mWeight == rhs.mWeight &&
            mLength == rhs.mLength && 
            mLoop == rhs.mLoop)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    //---------------------------------------------------------------------
    bool AnimationState::operator!=(const AnimationState& rhs) const
    {
        return !(*this == rhs);
    }
    //---------------------------------------------------------------------
    void AnimationState::copyStateFrom(const AnimationState& animState)
    {
        mTimePos = animState.mTimePos;
        mLength = animState.mLength;
        mWeight = animState.mWeight;
        mEnabled = animState.mEnabled;
        mLoop = animState.mLoop;
		mParent->_notifyDirty();

    }
	//---------------------------------------------------------------------
	//---------------------------------------------------------------------
	AnimationStateSet::AnimationStateSet()
		: mDirtyFrameNumber(std::numeric_limits<unsigned long>::max())
	{
	}
	//---------------------------------------------------------------------
	AnimationStateSet::AnimationStateSet(const AnimationStateSet& rhs)
		: mDirtyFrameNumber(std::numeric_limits<unsigned long>::max())
	{
		for (AnimationStateMap::const_iterator i = rhs.mAnimationStates.begin();
			i != rhs.mAnimationStates.end(); ++i)
		{
			AnimationState* src = i->second;
			mAnimationStates[src->getAnimationName()] = 
				OGRE_POOL_OBJECT_NEW(msAnimationStatePool, AnimationState, (this, *src));
		}

        // Clone enabled animation state list
        for (EnabledAnimationStateList::const_iterator it = rhs.mEnabledAnimationStates.begin();
            it != rhs.mEnabledAnimationStates.end(); ++it)
        {
            const AnimationState* src = *it;
            mEnabledAnimationStates.push_back(getAnimationState(src->getAnimationName()));
        }
	}
	//---------------------------------------------------------------------
	AnimationStateSet::~AnimationStateSet()
	{
		// Destroy
		removeAllAnimationStates();
	}
	//---------------------------------------------------------------------
	void AnimationStateSet::removeAnimationState(const String& name)
	{
		AnimationStateMap::iterator i = mAnimationStates.find(name);
		if (i != mAnimationStates.end())
		{
            mEnabledAnimationStates.remove(i->second);

			OGRE_POOL_OBJECT_DELETE(msAnimationStatePool, i->second);
			mAnimationStates.erase(i);
		}
	}
	//---------------------------------------------------------------------
	void AnimationStateSet::removeAllAnimationStates(void)
	{
		for (AnimationStateMap::iterator i = mAnimationStates.begin();
			i != mAnimationStates.end(); ++i)
		{
			OGRE_POOL_OBJECT_DELETE(msAnimationStatePool, i->second);
		}
		mAnimationStates.clear();
        mEnabledAnimationStates.clear();

	}
	//---------------------------------------------------------------------
	AnimationState* AnimationStateSet::createAnimationState(const String& name,  
		Real timePos, Real length, Real weight, bool enabled)
	{
		AnimationState*& slot = mAnimationStates[name];
		if (slot)
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
				"State for animation named '" + name + "' already exists.", 
				"AnimationStateSet::createAnimationState");
		}

		AnimationState* newState =
            OGRE_POOL_OBJECT_NEW(msAnimationStatePool, AnimationState, (name, this, length));
		slot = newState;

        newState->setTimePosition(timePos);
        newState->setWeight(weight);
        if (enabled)
            newState->setEnabled(enabled);

		return newState;

	}
	//---------------------------------------------------------------------
	AnimationState* AnimationStateSet::getAnimationState(const String& name) const
	{
		AnimationStateMap::const_iterator i = mAnimationStates.find(name);
		if (i == mAnimationStates.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"No state found for animation named '" + name + "'", 
				"AnimationStateSet::getAnimationState");
		}
		return i->second;
	}
	//---------------------------------------------------------------------
	bool AnimationStateSet::hasAnimationState(const String& name) const
	{
		return mAnimationStates.find(name) != mAnimationStates.end();
	}
	//---------------------------------------------------------------------
	AnimationStateIterator AnimationStateSet::getAnimationStateIterator(void)
	{
		return AnimationStateIterator(
			mAnimationStates.begin(), mAnimationStates.end());
	}
	//---------------------------------------------------------------------
	ConstAnimationStateIterator AnimationStateSet::getAnimationStateIterator(void) const
	{
		return ConstAnimationStateIterator(
			mAnimationStates.begin(), mAnimationStates.end());
	}
	//---------------------------------------------------------------------
	void AnimationStateSet::copyMatchingState(AnimationStateSet* target) const
	{
        AnimationStateMap::iterator i, iend;
        iend = target->mAnimationStates.end();
        for (i = target->mAnimationStates.begin(); i != iend; ++i) {
            AnimationStateMap::const_iterator iother = mAnimationStates.find(i->first);
            if (iother == mAnimationStates.end()) {
                OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "No animation entry found named " + i->first, 
                    "AnimationStateSet::copyMatchingState");
            } else {
                i->second->copyStateFrom(*(iother->second));
            }
        }

        // Copy matching enabled animation state list
        target->mEnabledAnimationStates.clear();

        EnabledAnimationStateList::const_iterator it, itend;
        itend = mEnabledAnimationStates.end();
        for (it = mEnabledAnimationStates.begin(); it != itend; ++it)
        {
            const AnimationState* src = *it;
            AnimationStateMap::const_iterator itarget = target->mAnimationStates.find(src->getAnimationName());
            if (itarget != target->mAnimationStates.end())
            {
                target->mEnabledAnimationStates.push_back(itarget->second);
            }
        }

        target->mDirtyFrameNumber = mDirtyFrameNumber;
    }
    //---------------------------------------------------------------------
    void AnimationStateSet::_notifyDirty(void)
    {
        ++mDirtyFrameNumber;
    }
    //---------------------------------------------------------------------
    void AnimationStateSet::_notifyAnimationStateEnabled(AnimationState* target, bool enabled)
    {
        // Remove from enabled animation state list first
        mEnabledAnimationStates.remove(target);

        // Add to enabled animation state list if need
        if (enabled)
        {
            mEnabledAnimationStates.push_back(target);
        }

        // Set the dirty frame number
        _notifyDirty();
    }
    //---------------------------------------------------------------------
    ConstEnabledAnimationStateIterator AnimationStateSet::getEnabledAnimationStateIterator(void) const
    {
        return ConstEnabledAnimationStateIterator(
            mEnabledAnimationStates.begin(), mEnabledAnimationStates.end());
    }
	//---------------------------------------------------------------------
	//---------------------------------------------------------------------
	Real AnimationStateControllerValue::getValue(void) const
	{
		return mTargetAnimationState->getTimePosition() / mTargetAnimationState->getLength();
	}
	//---------------------------------------------------------------------
	void AnimationStateControllerValue::setValue(Real value)
	{
		mTargetAnimationState->setTimePosition(value * mTargetAnimationState->getLength());
	}


}

