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
#include "OgreAnimation.h"
#include "OgreKeyFrame.h"
#include "OgreException.h"
#include "OgreSkeleton.h"
#include "OgreBone.h"
#include "OgreEntity.h"
#include "OgreSubEntity.h"
#include "OgreMesh.h"
#include "OgreSubMesh.h"
#include "OgreStringConverter.h"

namespace Ogre {

    Animation::InterpolationMode Animation::msDefaultInterpolationMode = Animation::IM_LINEAR;
    Animation::RotationInterpolationMode 
        Animation::msDefaultRotationInterpolationMode = Animation::RIM_LINEAR;
    OGRE_DECLARE_OBJECT_POOL(static, msNodeAnimationTrackPool, NodeAnimationTrack);
    OGRE_DECLARE_OBJECT_POOL(static, msNumericAnimationTrackPool, NumericAnimationTrack);
    OGRE_DECLARE_OBJECT_POOL(static, msVertexAnimationTrackPool, VertexAnimationTrack);
    //---------------------------------------------------------------------
    Animation::Animation(const String& name, Real length)
        : mName(name)
        , mLength(length)
        , mInterpolationMode(msDefaultInterpolationMode)
        , mRotationInterpolationMode(msDefaultRotationInterpolationMode)
        , mKeyFrameTimesDirty(false)
    {
    }
    //---------------------------------------------------------------------
    Animation::~Animation()
    {
        destroyAllTracks();
    }
    //---------------------------------------------------------------------
    Real Animation::getLength(void) const
    {
        return mLength;
    }
    //---------------------------------------------------------------------
    NodeAnimationTrack* Animation::createNodeTrack(unsigned short handle)
    {
        NodeAnimationTrack*& slot = mNodeTrackList[handle];
        if (slot)
        {
            OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
                "Node track with the specified handle " +
                StringConverter::toString(handle) + " already exists",
                "Animation::createNodeTrack");
        }

        NodeAnimationTrack* ret =
            OGRE_POOL_OBJECT_NEW(msNodeAnimationTrackPool, NodeAnimationTrack, (this, handle));

        slot = ret;
        return ret;
    }
    //---------------------------------------------------------------------
    NodeAnimationTrack* Animation::createNodeTrack(unsigned short handle, Node* node)
    {
        NodeAnimationTrack* ret = createNodeTrack(handle);

        ret->setAssociatedNode(node);

        return ret;
    }
    //---------------------------------------------------------------------
    unsigned short Animation::getNumNodeTracks(void) const
    {
        return (unsigned short)mNodeTrackList.size();
    }
	//---------------------------------------------------------------------
	bool Animation::hasNodeTrack(unsigned short handle) const
	{
		return (mNodeTrackList.find(handle) != mNodeTrackList.end());
	}
    //---------------------------------------------------------------------
    NodeAnimationTrack* Animation::getNodeTrack(unsigned short handle) const
    {
        NodeTrackList::const_iterator i = mNodeTrackList.find(handle);

        if (i == mNodeTrackList.end())
        {
            OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
                "Cannot find node track with the specified handle " +
                StringConverter::toString(handle),
                "Animation::getNodeTrack");
        }

        return i->second;

    }
    //---------------------------------------------------------------------
    void Animation::destroyNodeTrack(unsigned short handle)
    {
        NodeTrackList::iterator i = mNodeTrackList.find(handle);

		if (i != mNodeTrackList.end())
		{
			OGRE_POOL_OBJECT_DELETE(msNodeAnimationTrackPool, i->second);
			mNodeTrackList.erase(i);
            _keyFrameListChanged();
		}
    }
    //---------------------------------------------------------------------
    void Animation::destroyAllNodeTracks(void)
    {
        NodeTrackList::iterator i;
        for (i = mNodeTrackList.begin(); i != mNodeTrackList.end(); ++i)
        {
            OGRE_POOL_OBJECT_DELETE(msNodeAnimationTrackPool, i->second);
        }
        mNodeTrackList.clear();
        _keyFrameListChanged();
    }
	//---------------------------------------------------------------------
	NumericAnimationTrack* Animation::createNumericTrack(unsigned short handle)
	{
        NumericAnimationTrack*& slot = mNumericTrackList[handle];
        if (slot)
        {
            OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
                "Numeric track with the specified handle " +
                StringConverter::toString(handle) + " already exists",
                "Animation::createNumericTrack");
        }

		NumericAnimationTrack* ret =
            OGRE_POOL_OBJECT_NEW(msNumericAnimationTrackPool, NumericAnimationTrack, (this, handle));

		slot = ret;
		return ret;
	}
	//---------------------------------------------------------------------
	NumericAnimationTrack* Animation::createNumericTrack(unsigned short handle, 
		const AnimableValuePtr& anim)
	{
		NumericAnimationTrack* ret = createNumericTrack(handle);

		ret->setAssociatedAnimable(anim);

		return ret;
	}
	//---------------------------------------------------------------------
	unsigned short Animation::getNumNumericTracks(void) const
	{
		return (unsigned short)mNumericTrackList.size();
	}
	//---------------------------------------------------------------------
	bool Animation::hasNumericTrack(unsigned short handle) const
	{
		return (mNumericTrackList.find(handle) != mNumericTrackList.end());
	}
	//---------------------------------------------------------------------
	NumericAnimationTrack* Animation::getNumericTrack(unsigned short handle) const
	{
		NumericTrackList::const_iterator i = mNumericTrackList.find(handle);

		if (i == mNumericTrackList.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"Cannot find numeric track with the specified handle " +
                StringConverter::toString(handle),
				"Animation::getNumericTrack");
		}

		return i->second;

	}
	//---------------------------------------------------------------------
	void Animation::destroyNumericTrack(unsigned short handle)
	{
		NumericTrackList::iterator i = mNumericTrackList.find(handle);

		if (i != mNumericTrackList.end())
		{
			OGRE_POOL_OBJECT_DELETE(msNumericAnimationTrackPool, i->second);
			mNumericTrackList.erase(i);
            _keyFrameListChanged();
		}
	}
	//---------------------------------------------------------------------
	void Animation::destroyAllNumericTracks(void)
	{
		NumericTrackList::iterator i;
		for (i = mNumericTrackList.begin(); i != mNumericTrackList.end(); ++i)
		{
			OGRE_POOL_OBJECT_DELETE(msNumericAnimationTrackPool, i->second);
		}
		mNumericTrackList.clear();
        _keyFrameListChanged();
	}
	//---------------------------------------------------------------------
	VertexAnimationTrack* Animation::createVertexTrack(unsigned short handle, 
		VertexAnimationType animType)
	{
        VertexAnimationTrack*& slot = mVertexTrackList[handle];
        if (slot)
        {
            OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM, 
                "Vertex track with the specified handle " +
                StringConverter::toString(handle) + " already exists",
                "Animation::createVertexTrack");
        }

		VertexAnimationTrack* ret =
            OGRE_POOL_OBJECT_NEW(msVertexAnimationTrackPool, VertexAnimationTrack, (this, handle, animType));

		slot = ret;
		return ret;

	}
	//---------------------------------------------------------------------
	VertexAnimationTrack* Animation::createVertexTrack(unsigned short handle, 
		VertexData* data, VertexAnimationType animType)
	{
		VertexAnimationTrack* ret = createVertexTrack(handle, animType);

		ret->setAssociatedVertexData(data);

		return ret;
	}
	//---------------------------------------------------------------------
	unsigned short Animation::getNumVertexTracks(void) const
	{
		return (unsigned short)mVertexTrackList.size();
	}
	//---------------------------------------------------------------------
	bool Animation::hasVertexTrack(unsigned short handle) const
	{
		return (mVertexTrackList.find(handle) != mVertexTrackList.end());
	}
	//---------------------------------------------------------------------
	VertexAnimationTrack* Animation::getVertexTrack(unsigned short handle) const
	{
		VertexTrackList::const_iterator i = mVertexTrackList.find(handle);

		if (i == mVertexTrackList.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, 
				"Cannot find vertex track with the specified handle " +
                StringConverter::toString(handle),
				"Animation::getVertexTrack");
		}

		return i->second;

	}
	//---------------------------------------------------------------------
	void Animation::destroyVertexTrack(unsigned short handle)
	{
		VertexTrackList::iterator i = mVertexTrackList.find(handle);

		if (i != mVertexTrackList.end())
		{
			OGRE_POOL_OBJECT_DELETE(msVertexAnimationTrackPool, i->second);
			mVertexTrackList.erase(i);
            _keyFrameListChanged();
		}
	}
	//---------------------------------------------------------------------
	void Animation::destroyAllVertexTracks(void)
	{
		VertexTrackList::iterator i;
		for (i = mVertexTrackList.begin(); i != mVertexTrackList.end(); ++i)
		{
			OGRE_POOL_OBJECT_DELETE(msVertexAnimationTrackPool, i->second);
		}
		mVertexTrackList.clear();
        _keyFrameListChanged();
	}
	//---------------------------------------------------------------------
	void Animation::destroyAllTracks(void)
	{
		destroyAllNodeTracks();
		destroyAllNumericTracks();
		destroyAllVertexTracks();
	}
    //---------------------------------------------------------------------
    const String& Animation::getName(void) const
    {
        return mName;
    }
    //---------------------------------------------------------------------
	void Animation::apply(Real timePos, Real weight, bool accumulate, Real scale)
    {
        // Calculate time index for fast keyframe search
        TimeIndex timeIndex = _getTimeIndex(timePos);

        NodeTrackList::iterator i;
        for (i = mNodeTrackList.begin(); i != mNodeTrackList.end(); ++i)
        {
            i->second->apply(timeIndex, weight, accumulate, scale);
        }
		NumericTrackList::iterator j;
		for (j = mNumericTrackList.begin(); j != mNumericTrackList.end(); ++j)
		{
			j->second->apply(timeIndex, weight, accumulate, scale);
		}
		VertexTrackList::iterator k;
		for (k = mVertexTrackList.begin(); k != mVertexTrackList.end(); ++k)
		{
			k->second->apply(timeIndex, weight, accumulate, scale);
		}

    }
    //---------------------------------------------------------------------
    void Animation::apply(Skeleton* skel, Real timePos, Real weight, 
		bool accumulate, Real scale)
    {
        // Calculate time index for fast keyframe search
        TimeIndex timeIndex = _getTimeIndex(timePos);

        NodeTrackList::iterator i;
        for (i = mNodeTrackList.begin(); i != mNodeTrackList.end(); ++i)
        {
            // get bone to apply to 
            Bone* b = skel->getBone(i->first);
            i->second->applyToNode(b, timeIndex, weight, accumulate, scale);
        }


    }
	//---------------------------------------------------------------------
	void Animation::apply(Entity* entity, Real timePos, Real weight, 
		bool software, bool hardware)
	{
        // Calculate time index for fast keyframe search
        TimeIndex timeIndex = _getTimeIndex(timePos);

		VertexTrackList::iterator i;
		for (i = mVertexTrackList.begin(); i != mVertexTrackList.end(); ++i)
		{
			unsigned short handle = i->first;
			VertexAnimationTrack* track = i->second;

			VertexData* swVertexData;
			VertexData* hwVertexData;
			VertexData* origVertexData;
			bool firstAnim = false;
			if (handle == 0)
			{
				// shared vertex data
				firstAnim = !entity->_getBuffersMarkedForAnimation();
				swVertexData = entity->_getSoftwareVertexAnimVertexData();
				hwVertexData = entity->_getHardwareVertexAnimVertexData();
				origVertexData = entity->getMesh()->sharedVertexData;
				entity->_markBuffersUsedForAnimation();
			}
			else
			{
				// sub entity vertex data (-1)
				SubEntity* s = entity->getSubEntity(handle - 1);
				firstAnim = !s->_getBuffersMarkedForAnimation();
				swVertexData = s->_getSoftwareVertexAnimVertexData();
				hwVertexData = s->_getHardwareVertexAnimVertexData();
				origVertexData = s->getSubMesh()->vertexData;
				s->_markBuffersUsedForAnimation();
			}
			// Apply to both hardware and software, if requested
			if (software)
			{
				if (firstAnim && track->getAnimationType() == VAT_POSE)
				{
					// First time through for a piece of pose animated vertex data
					// We need to copy the original position values to the temp accumulator
					const VertexElement* origelem = 
						origVertexData->vertexDeclaration->findElementBySemantic(VES_POSITION);
					const VertexElement* destelem = 
						swVertexData->vertexDeclaration->findElementBySemantic(VES_POSITION);
					HardwareVertexBufferSharedPtr origBuffer = 
						origVertexData->vertexBufferBinding->getBuffer(origelem->getSource());
					HardwareVertexBufferSharedPtr destBuffer = 
						swVertexData->vertexBufferBinding->getBuffer(destelem->getSource());
					destBuffer->copyData(*origBuffer.get(), 0, 0, destBuffer->getSizeInBytes(), true);
				}
				track->setTargetMode(VertexAnimationTrack::TM_SOFTWARE);
				track->applyToVertexData(swVertexData, timeIndex, weight, 
					&(entity->getMesh()->getPoseList()));
			}
			if (hardware)
			{
				track->setTargetMode(VertexAnimationTrack::TM_HARDWARE);
				track->applyToVertexData(hwVertexData, timeIndex, weight, 
					&(entity->getMesh()->getPoseList()));
			}
		}

	}
    //---------------------------------------------------------------------
    void Animation::setInterpolationMode(InterpolationMode im)
    {
        mInterpolationMode = im;
    }
    //---------------------------------------------------------------------
    Animation::InterpolationMode Animation::getInterpolationMode(void) const
    {
        return mInterpolationMode;
    }
    //---------------------------------------------------------------------
    void Animation::setDefaultInterpolationMode(InterpolationMode im)
    {
        msDefaultInterpolationMode = im;
    }
    //---------------------------------------------------------------------
    Animation::InterpolationMode Animation::getDefaultInterpolationMode(void)
    {
        return msDefaultInterpolationMode;
    }
    //---------------------------------------------------------------------
    const Animation::NodeTrackList& Animation::_getNodeTrackList(void) const
    {
        return mNodeTrackList;

    }
	//---------------------------------------------------------------------
	const Animation::NumericTrackList& Animation::_getNumericTrackList(void) const
	{
		return mNumericTrackList;
	}
    //---------------------------------------------------------------------
    const Animation::VertexTrackList& Animation::_getVertexTrackList(void) const
    {
        return mVertexTrackList;
    }
    //---------------------------------------------------------------------
    void Animation::setRotationInterpolationMode(RotationInterpolationMode im)
    {
        mRotationInterpolationMode = im;
    }
    //---------------------------------------------------------------------
    Animation::RotationInterpolationMode Animation::getRotationInterpolationMode(void) const
    {
        return mRotationInterpolationMode;
    }
    //---------------------------------------------------------------------
    void Animation::setDefaultRotationInterpolationMode(RotationInterpolationMode im)
    {
        msDefaultRotationInterpolationMode = im;
    }
    //---------------------------------------------------------------------
    Animation::RotationInterpolationMode Animation::getDefaultRotationInterpolationMode(void)
    {
        return msDefaultRotationInterpolationMode;
    }
    //---------------------------------------------------------------------
	void Animation::optimise(bool respectsCurrentModeOnly, Real toleranceFactor)
	{
		optimiseNodeTracks(respectsCurrentModeOnly, toleranceFactor);
		optimiseVertexTracks(respectsCurrentModeOnly, toleranceFactor);
		
	}
	//-----------------------------------------------------------------------
	void Animation::optimiseNodeTracks(bool respectsCurrentModeOnly, Real toleranceFactor)
	{
		// Iterate over the node tracks and identify those with no useful keyframes
		std::list<unsigned short> tracksToDestroy;
		NodeTrackList::iterator i;
		for (i = mNodeTrackList.begin(); i != mNodeTrackList.end(); ++i)
		{
			NodeAnimationTrack* track = i->second;
			if (!track->hasNonZeroKeyFrames())
			{
				// mark the entire track for destruction
				tracksToDestroy.push_back(i->first);
			}
			else
			{
				track->optimise(respectsCurrentModeOnly, toleranceFactor);
			}

		}

		// Now destroy the tracks we marked for death
		for(std::list<unsigned short>::iterator h = tracksToDestroy.begin();
			h != tracksToDestroy.end(); ++h)
		{
			destroyNodeTrack(*h);
		}
	}
	//-----------------------------------------------------------------------
	void Animation::optimiseVertexTracks(bool respectsCurrentModeOnly, Real toleranceFactor)
	{
		// Iterate over the node tracks and identify those with no useful keyframes
		std::list<unsigned short> tracksToDestroy;
		VertexTrackList::iterator i;
		for (i = mVertexTrackList.begin(); i != mVertexTrackList.end(); ++i)
		{
			VertexAnimationTrack* track = i->second;
			if (!track->hasNonZeroKeyFrames())
			{
				// mark the entire track for destruction
				tracksToDestroy.push_back(i->first);
			}
			else
			{
				track->optimise(respectsCurrentModeOnly, toleranceFactor);
			}

		}

		// Now destroy the tracks we marked for death
		for(std::list<unsigned short>::iterator h = tracksToDestroy.begin();
			h != tracksToDestroy.end(); ++h)
		{
			destroyVertexTrack(*h);
		}

	}
	//-----------------------------------------------------------------------
	Animation* Animation::clone(const String& newName) const
	{
		Animation* newAnim = new Animation(newName, mLength);
        newAnim->mInterpolationMode = mInterpolationMode;
        newAnim->mRotationInterpolationMode = mRotationInterpolationMode;
		
		// Clone all tracks
		for (NodeTrackList::const_iterator i = mNodeTrackList.begin();
			i != mNodeTrackList.end(); ++i)
		{
			newAnim->mNodeTrackList[i->second->getHandle()] = i->second->_clone(newAnim);
		}
		for (NumericTrackList::const_iterator i = mNumericTrackList.begin();
			i != mNumericTrackList.end(); ++i)
		{
			newAnim->mNumericTrackList[i->second->getHandle()] = i->second->_clone(newAnim);
		}
		for (VertexTrackList::const_iterator i = mVertexTrackList.begin();
			i != mVertexTrackList.end(); ++i)
		{
			newAnim->mVertexTrackList[i->second->getHandle()] = i->second->_clone(newAnim);
		}

        newAnim->_keyFrameListChanged();
		return newAnim;

	}
    //-----------------------------------------------------------------------
    TimeIndex Animation::_getTimeIndex(Real timePos) const
    {
        // Uncomment following statement for work as previous
        //return timePos;

        // Build keyframe time list on demand
        if (mKeyFrameTimesDirty)
        {
            buildKeyFrameTimeList();
        }

        // Wrap time
        Real totalAnimationLength = mLength;
        assert(totalAnimationLength > 0.0f && "Invalid animation length!");

        while (timePos > totalAnimationLength && totalAnimationLength > 0.0f)
        {
            timePos -= totalAnimationLength;
        }

        // Search for global index
        KeyFrameTimeList::iterator it =
            std::lower_bound(mKeyFrameTimes.begin(), mKeyFrameTimes.end(), timePos);

        return TimeIndex(timePos, std::distance(mKeyFrameTimes.begin(), it));
    }
    //-----------------------------------------------------------------------
    void Animation::buildKeyFrameTimeList(void) const
    {
        NodeTrackList::const_iterator i;
		NumericTrackList::const_iterator j;
		VertexTrackList::const_iterator k;

        // Clear old keyframe times
        mKeyFrameTimes.clear();

        // Collect all keyframe times from each track
        for (i = mNodeTrackList.begin(); i != mNodeTrackList.end(); ++i)
        {
            i->second->_collectKeyFrameTimes(mKeyFrameTimes);
        }
		for (j = mNumericTrackList.begin(); j != mNumericTrackList.end(); ++j)
		{
            j->second->_collectKeyFrameTimes(mKeyFrameTimes);
		}
		for (k = mVertexTrackList.begin(); k != mVertexTrackList.end(); ++k)
		{
            k->second->_collectKeyFrameTimes(mKeyFrameTimes);
		}

        // Build global index to local index map for each track
        for (i = mNodeTrackList.begin(); i != mNodeTrackList.end(); ++i)
        {
            i->second->_buildKeyFrameIndexMap(mKeyFrameTimes);
        }
		for (j = mNumericTrackList.begin(); j != mNumericTrackList.end(); ++j)
		{
            j->second->_buildKeyFrameIndexMap(mKeyFrameTimes);
		}
		for (k = mVertexTrackList.begin(); k != mVertexTrackList.end(); ++k)
		{
            k->second->_buildKeyFrameIndexMap(mKeyFrameTimes);
		}

        // Reset dirty flag
        mKeyFrameTimesDirty = false;
    }

}


