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
#include "OgreSkeleton.h"
#include "OgreBone.h"
#include "OgreAnimation.h"
#include "OgreAnimationState.h"
#include "OgreException.h"
#include "OgreLogManager.h"
#include "OgreSkeletonManager.h"
#include "OgreSkeletonSerializer.h"
#include "OgreStringConverter.h"
#include "OgrePoolAllocator.h"
// Just for logging
#include "OgreAnimationTrack.h"
#include "OgreKeyFrame.h"


namespace Ogre {

    OGRE_DECLARE_OBJECT_POOL(static, msBonePool, Bone);
    //---------------------------------------------------------------------
	Skeleton::Skeleton()
		: Resource(),
        mBlendState(ANIMBLEND_AVERAGE),
		mNextAutoHandle(0),
		mManualBonesDirtyCounter(0)
	{
	}
	//---------------------------------------------------------------------
    Skeleton::Skeleton(ResourceManager* creator, const String& name, ResourceHandle handle,
        const String& group, bool isManual, ManualResourceLoader* loader) 
        : Resource(creator, name, handle, group, isManual, loader), 
        mBlendState(ANIMBLEND_AVERAGE), mNextAutoHandle(0), mManualBonesDirtyCounter(0)
        // set animation blending to weighted, not cumulative
    {
        if (createParamDictionary("Skeleton"))
        {
            // no custom params
        }
    }
    //---------------------------------------------------------------------
    Skeleton::~Skeleton()
    {
        // have to call this here reather than in Resource destructor
        // since calling virtual methods in base destructors causes crash
        unload(); 
    }
    //---------------------------------------------------------------------
    void Skeleton::loadImpl(void)
    {
        SkeletonSerializer serializer;
		StringUtil::StrStreamType msg;
		msg << "Skeleton: Loading " << mName;
        LogManager::getSingleton().logMessage(msg.str());

        DataStreamPtr stream = 
            ResourceGroupManager::getSingleton().openResource(
				mName, mGroup, true, this);

        serializer.importSkeleton(stream, this);

		// Load any linked skeletons
		LinkedSkeletonAnimSourceList::iterator i;
		for (i = mLinkedSkeletonAnimSourceList.begin(); 
			i != mLinkedSkeletonAnimSourceList.end(); ++i)
		{
			i->pSkeleton = SkeletonManager::getSingleton().load(
				i->skeletonName, mGroup);
		}


    }
    //---------------------------------------------------------------------
    void Skeleton::unloadImpl(void)
    {
        // destroy bones
        BoneList::iterator i;
        for (i = mBoneList.begin(); i != mBoneList.end(); ++i)
        {
            OGRE_POOL_OBJECT_DELETE(msBonePool, *i);
        }
        mBoneList.clear();
        mBoneListByName.clear();
		mRootBones.clear();
        mManualBones.clear();
        mNextAutoHandle = 0;
        mManualBonesDirtyCounter = 0;

        // Destroy animations
        AnimationList::iterator ai;
        for (ai = mAnimationsList.begin(); ai != mAnimationsList.end(); ++ai)
        {
            delete ai->second;
        }
        mAnimationsList.clear();

        // Remove all linked skeletons
        mLinkedSkeletonAnimSourceList.clear();
    }
    //---------------------------------------------------------------------
    size_t Skeleton::calculateSize(void) const
    {
        size_t size = 0;

        size += sizeof(*this);
        size += sizeof(Bone) * mBoneList.size();
        size += sizeof(BoneList::value_type) * mBoneList.size();
        size += sizeof(BoneListByName::value_type) * mBoneListByName.size();
        size += sizeof(LinkedSkeletonAnimSourceList::value_type) * mLinkedSkeletonAnimSourceList.size();

        size += sizeof(AnimationList::value_type) * mAnimationsList.size();
        for (AnimationList::const_iterator ai = mAnimationsList.begin(); ai != mAnimationsList.end(); ++ai)
        {
            const Animation* anim = ai->second;
            size += sizeof(*anim);

            Animation::NodeTrackIterator nodeTrackIter = anim->getNodeTrackIterator();
            while (nodeTrackIter.hasMoreElements())
            {
                size += sizeof(Animation::NodeTrackList::value_type);
                const NodeAnimationTrack* nodeTrack = nodeTrackIter.getNext();
                size += sizeof(*nodeTrack);
                size += sizeof(TransformKeyFrame) * nodeTrack->getNumKeyFrames();
                // size for splines
                if (anim->getInterpolationMode() == Animation::IM_SPLINE)
                {
                    size += 2 * (sizeof(Vector3) + sizeof(Vector3) + sizeof(Quaternion)) * nodeTrack->getNumKeyFrames();
                }
            }

		    Animation::NumericTrackIterator numericTrackIter = anim->getNumericTrackIterator();
            while (numericTrackIter.hasMoreElements())
            {
                size += sizeof(Animation::NumericTrackList::value_type);
                const NumericAnimationTrack* numericTrack = numericTrackIter.getNext();
                size += sizeof(*numericTrack);
                size += sizeof(NumericKeyFrame) * numericTrack->getNumKeyFrames();
            }

            Animation::VertexTrackIterator vertexTrackIter = anim->getVertexTrackIterator();
            while (vertexTrackIter.hasMoreElements())
            {
                size += sizeof(Animation::VertexTrackList::value_type);
                const VertexAnimationTrack* vertexTrack = vertexTrackIter.getNext();
                size += sizeof(*vertexTrack);
                if (vertexTrack->getAnimationType() == VAT_POSE)
                    size += sizeof(VertexPoseKeyFrame) * vertexTrack->getNumKeyFrames();
                else
                    size += sizeof(VertexMorphKeyFrame) * vertexTrack->getNumKeyFrames();
            }
        }

        return size;
    }
    //---------------------------------------------------------------------
    Bone* Skeleton::createBone(void)
    {
        // use autohandle
        return createBone(mNextAutoHandle++);
    }
    //---------------------------------------------------------------------
    Bone* Skeleton::createBone(const String& name)
    {
        return createBone(name, mNextAutoHandle++);
    }
    //---------------------------------------------------------------------
    Bone* Skeleton::createBone(unsigned short handle)
    {
        if (handle >= OGRE_MAX_NUM_BONES)
        {
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Exceeded the maximum number of bones per skeleton.",
                "Skeleton::createBone");
        }
        // Check handle not used
        if (handle < mBoneList.size() && mBoneList[handle] != NULL)
        {
            OGRE_EXCEPT(
                Exception::ERR_DUPLICATE_ITEM,
                "A bone with the handle " + StringConverter::toString(handle) + " already exists",
                "Skeleton::createBone" );
        }
        Bone* ret = OGRE_POOL_OBJECT_NEW(msBonePool, Bone, (handle, this));
        assert(mBoneListByName.find(ret->getName()) == mBoneListByName.end());
        if (mBoneList.size() <= handle)
        {
            mBoneList.resize(handle+1);
        }
        mBoneList[handle] = ret;
        mBoneListByName[ret->getName()] = ret;
        return ret;

    }
    //---------------------------------------------------------------------
    Bone* Skeleton::createBone(const String& name, unsigned short handle)
    {
        if (handle >= OGRE_MAX_NUM_BONES)
        {
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Exceeded the maximum number of bones per skeleton.",
                "Skeleton::createBone");
        }
        // Check handle not used
        if (handle < mBoneList.size() && mBoneList[handle] != NULL)
        {
            OGRE_EXCEPT(
                Exception::ERR_DUPLICATE_ITEM,
                "A bone with the handle " + StringConverter::toString(handle) + " already exists",
                "Skeleton::createBone" );
        }
        // Check name not used
        Bone*& slot = mBoneListByName[name];
        if (slot)
        {
            OGRE_EXCEPT(
                Exception::ERR_DUPLICATE_ITEM,
                "A bone with the name " + name + " already exists",
                "Skeleton::createBone" );
        }
        Bone* ret = OGRE_POOL_OBJECT_NEW(msBonePool, Bone, (name, handle, this));
        if (mBoneList.size() <= handle)
        {
            mBoneList.resize(handle+1);
        }
        mBoneList[handle] = ret;
        slot = ret;
        return ret;
    }



    //---------------------------------------------------------------------
    Bone* Skeleton::getRootBone(void) const
    {
        if (mRootBones.empty())
        {
            deriveRootBone();
        }

        return mRootBones[0];
    }
    //---------------------------------------------------------------------
    void Skeleton::setAnimationState(const AnimationStateSet& animSet)
    {
        /* 
        Algorithm:
          1. Reset all bone positions
          2. Iterate per AnimationState, if enabled get Animation and call Animation::apply
        */

        // Reset bones
        reset();

        // Per enabled animation state
        ConstEnabledAnimationStateIterator stateIt = 
            animSet.getEnabledAnimationStateIterator();
        while (stateIt.hasMoreElements())
        {
            const AnimationState* animState = stateIt.getNext();
            const LinkedSkeletonAnimationSource* linked = 0;
            Animation* anim = _getAnimationImpl(animState->getAnimationName(), &linked);
            // tolerate state entries for animations we're not aware of
            if (anim)
            {
                if (linked)
                {
                    anim->apply(this, animState->getTimePosition(), animState->getWeight(), 
                        mBlendState == ANIMBLEND_CUMULATIVE, linked->scale);
                }
                else
                {
                    anim->apply(this, animState->getTimePosition(), animState->getWeight(), 
                        mBlendState == ANIMBLEND_CUMULATIVE);
                }
            }
        }


    }
    //---------------------------------------------------------------------
    void Skeleton::setBindingPose(void)
    {
        // Update the derived transforms
        _updateTransforms();


        BoneList::iterator i;
        for (i = mBoneList.begin(); i != mBoneList.end(); ++i)
        {            
            (*i)->setBindingPose();
        }
    }
    //---------------------------------------------------------------------
    void Skeleton::reset(bool resetManualBones)
    {
        BoneList::iterator i;
        for (i = mBoneList.begin(); i != mBoneList.end(); ++i)
        {
            if(!(*i)->isManuallyControlled() || resetManualBones)
                (*i)->reset();
        }
    }
    //---------------------------------------------------------------------
    Animation* Skeleton::createAnimation(const String& name, Real length)
    {
        // Check name not used
        if (mAnimationsList.find(name) != mAnimationsList.end())
        {
            OGRE_EXCEPT(
                Exception::ERR_DUPLICATE_ITEM,
                "An animation with the name " + name + " already exists",
                "Skeleton::createAnimation");
        }

        Animation* ret = new Animation(name, length);

        // Add to list
        mAnimationsList[name] = ret;

        return ret;

    }
	//---------------------------------------------------------------------
	Animation* Skeleton::getAnimation(const String& name, 
		const LinkedSkeletonAnimationSource** linker) const
	{
		Animation* ret = _getAnimationImpl(name, linker);
		if (!ret)
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "No animation entry found named " + name, 
				"Skeleton::getAnimation");
		}

		return ret;
	}
	//---------------------------------------------------------------------
	bool Skeleton::hasAnimation(const String& name)
	{
		return _getAnimationImpl(name) != 0;
	}
    //---------------------------------------------------------------------
    Animation* Skeleton::_getAnimationImpl(const String& name, 
		const LinkedSkeletonAnimationSource** linker) const
    {
		Animation* ret = 0;
        AnimationList::const_iterator i = mAnimationsList.find(name);

        if (i == mAnimationsList.end())
        {
			LinkedSkeletonAnimSourceList::const_iterator i;
			for (i = mLinkedSkeletonAnimSourceList.begin(); 
				i != mLinkedSkeletonAnimSourceList.end() && !ret; ++i)
			{
				if (!i->pSkeleton.isNull())
				{
					ret = i->pSkeleton->_getAnimationImpl(name);
					if (ret && linker)
					{
						*linker = &(*i);
					}

				}
			}

        }
		else
		{
			if (linker)
				*linker = 0;
			ret = i->second;
		}

		return ret;

    }
    //---------------------------------------------------------------------
    void Skeleton::removeAnimation(const String& name)
    {
        AnimationList::iterator i = mAnimationsList.find(name);

        if (i == mAnimationsList.end())
        {
            OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "No animation entry found named " + name, 
            "Skeleton::getAnimation");
        }

        delete i->second;

        mAnimationsList.erase(i);

    }
    //-----------------------------------------------------------------------
    void Skeleton::_initAnimationState(AnimationStateSet* animSet)
    {
        animSet->removeAllAnimationStates();
           
        AnimationList::iterator i;
        for (i = mAnimationsList.begin(); i != mAnimationsList.end(); ++i)
        {
            Animation* anim = i->second;
            // Create animation at time index 0, default params mean this has weight 1 and is disabled
            const String& animName = anim->getName();
            animSet->createAnimationState(animName, 0.0, anim->getLength());
        }

		// Also iterate over linked animation
		LinkedSkeletonAnimSourceList::iterator li;
		for (li = mLinkedSkeletonAnimSourceList.begin(); 
			li != mLinkedSkeletonAnimSourceList.end(); ++li)
		{
			if (!li->pSkeleton.isNull())
			{
				li->pSkeleton->_refreshAnimationState(animSet);
			}
		}

    }
	//-----------------------------------------------------------------------
	void Skeleton::_refreshAnimationState(AnimationStateSet* animSet)
	{
		// Merge in any new animations
		AnimationList::iterator i;
		for (i = mAnimationsList.begin(); i != mAnimationsList.end(); ++i)
		{
			Animation* anim = i->second;
			// Create animation at time index 0, default params mean this has weight 1 and is disabled
			const String& animName = anim->getName();
			if (!animSet->hasAnimationState(animName))
			{
				animSet->createAnimationState(animName, 0.0, anim->getLength());
			}
			else
			{
				// Update length incase changed
				AnimationState* animState = animSet->getAnimationState(animName);
				animState->setLength(anim->getLength());
				animState->setTimePosition(std::min(anim->getLength(), animState->getTimePosition()));
			}
		}
		// Also iterate over linked animation
		LinkedSkeletonAnimSourceList::iterator li;
		for (li = mLinkedSkeletonAnimSourceList.begin(); 
			li != mLinkedSkeletonAnimSourceList.end(); ++li)
		{
			if (!li->pSkeleton.isNull())
			{
				li->pSkeleton->_refreshAnimationState(animSet);
			}
		}
	}
	//-----------------------------------------------------------------------
	void Skeleton::_notifyManualBonesDirty(void)
	{
		++mManualBonesDirtyCounter;
	}
	//-----------------------------------------------------------------------
	void Skeleton::_notifyManualBoneStateChange(Bone* bone)
	{
		if (bone->isManuallyControlled())
			mManualBones.insert(bone);
		else
			mManualBones.erase(bone);
	}
    //-----------------------------------------------------------------------
    unsigned short Skeleton::getNumBones(void) const
    {
        return (unsigned short)mBoneList.size();
    }
    //-----------------------------------------------------------------------
    void Skeleton::_getBoneMatrices(Matrix4* pMatrices)
    {
        // Update derived transforms
        _updateTransforms();

        /*
            Calculating the bone matrices
            -----------------------------
            Now that we have the derived scaling factors, orientations & positions in the
            Bone nodes, we have to compute the Matrix4 to apply to the vertices of a mesh.
            Because any modification of a vertex has to be relative to the bone, we must
            first reverse transform by the Bone's original derived position/orientation/scale,
            then transform by the new derived position/orientation/scale.
            Also note we combine scale as equivalent axes, no shearing.
        */

        BoneList::const_iterator i, boneend;
        boneend = mBoneList.end();
        for (i = mBoneList.begin();i != boneend; ++i)
        {
            Bone* pBone = *i;
            pBone->_getOffsetTransform(*pMatrices);
            pMatrices++;
        }

    }
    //---------------------------------------------------------------------
    unsigned short Skeleton::getNumAnimations(void) const
    {
        return (unsigned short)mAnimationsList.size();
    }
    //---------------------------------------------------------------------
    Animation* Skeleton::getAnimation(unsigned short index) const
    {
		// If you hit this assert, then the index is out of bounds.
        assert( index < mAnimationsList.size() );

        AnimationList::const_iterator i = mAnimationsList.begin();

		std::advance(i, index);

        return i->second;
    }
    //---------------------------------------------------------------------
    Bone* Skeleton::getBone(unsigned short handle) const
    {
        assert(handle < mBoneList.size() && "Index out of bounds");
        return mBoneList[handle];
    }
    //---------------------------------------------------------------------
    Bone* Skeleton::getBone(const String& name) const
    {
        BoneListByName::const_iterator i = mBoneListByName.find(name);

        if (i == mBoneListByName.end())
        {
            OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "Bone named '" + name + "' not found.", 
                "Skeleton::getBone");
        }

        return i->second;

    }
    //---------------------------------------------------------------------
    void Skeleton::deriveRootBone(void) const
    {
        // Start at the first bone and work up
        if (mBoneList.empty())
        {
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Cannot derive root bone as this "
                "skeleton has no bones!", "Skeleton::deriveRootBone");
        }

        mRootBones.clear();

        Bone* currentBone;
        BoneList::const_iterator i;
        BoneList::const_iterator iend = mBoneList.end();
        for (i = mBoneList.begin(); i != iend; ++i)
        {
            currentBone = *i;
            if (currentBone->getParent() == 0)
            {
                // This is a root
                mRootBones.push_back(currentBone);
            }
        }
    }
    //---------------------------------------------------------------------
    void Skeleton::_dumpContents(const String& filename)
    {
        std::ofstream of;

        Quaternion q;
        Radian angle;
        Vector3 axis;
        of.open(filename.c_str());

        of << "-= Debug output of skeleton " << mName << " =-" << std::endl << std::endl;
        of << "== Bones ==" << std::endl;
        of << "Number of bones: " << (unsigned int)mBoneList.size() << std::endl;
        
        BoneList::iterator bi;
        for (bi = mBoneList.begin(); bi != mBoneList.end(); ++bi)
        {
            Bone* bone = *bi;

            of << "-- Bone " << bone->getHandle() << " --" << std::endl;
            of << "Position: " << bone->getPosition();
            q = bone->getOrientation();
            of << "Rotation: " << q;
            q.ToAngleAxis(angle, axis);
            of << " = " << angle.valueRadians() << " radians around axis " << axis << std::endl << std::endl;
        }

        of << "== Animations ==" << std::endl;
        of << "Number of animations: " << (unsigned int)mAnimationsList.size() << std::endl;

        AnimationList::iterator ai;
        for (ai = mAnimationsList.begin(); ai != mAnimationsList.end(); ++ai)
        {
            Animation* anim = ai->second;

            of << "-- Animation '" << anim->getName() << "' (length " << anim->getLength() << ") --" << std::endl;
            of << "Number of tracks: " << anim->getNumNodeTracks() << std::endl;

            int ti;
            for (ti = 0; ti < anim->getNumNodeTracks(); ++ti)
            {
                NodeAnimationTrack* track = anim->getNodeTrack(ti);
                of << "  -- AnimationTrack " << ti << " --" << std::endl;
                of << "  Affects bone: " << ((Bone*)track->getAssociatedNode())->getHandle() << std::endl;
                of << "  Number of keyframes: " << track->getNumKeyFrames() << std::endl;

                int ki;
                
                for (ki = 0; ki < track->getNumKeyFrames(); ++ki)
                {
                    TransformKeyFrame* key = track->getNodeKeyFrame(ki);
                    of << "    -- KeyFrame " << ki << " --" << std::endl;
                    of << "    Time index: " << key->getTime(); 
                    of << "    Translation: " << key->getTranslate() << std::endl;
                    q = key->getRotation();
                    of << "    Rotation: " << q;
                    q.ToAngleAxis(angle, axis);
                    of << " = " << angle.valueRadians() << " radians around axis " << axis << std::endl;
                }

            }



        }

    }
    //---------------------------------------------------------------------
	SkeletonAnimationBlendMode Skeleton::getBlendMode() const
    {
		return mBlendState;
	}
    //---------------------------------------------------------------------
	void Skeleton::setBlendMode(SkeletonAnimationBlendMode state) 
    {
		mBlendState = state;
	}
    //---------------------------------------------------------------------
    Skeleton::BoneIterator Skeleton::getRootBoneIterator(void)
    {
        if (mRootBones.empty())
        {
            deriveRootBone();
        }
        return BoneIterator(mRootBones.begin(), mRootBones.end());
    }
    //---------------------------------------------------------------------
    Skeleton::BoneIterator Skeleton::getBoneIterator(void)
    {
        return BoneIterator(mBoneList.begin(), mBoneList.end());
    }
    //---------------------------------------------------------------------
    void Skeleton::_updateTransforms(void)
    {
        BoneList::iterator i, iend;
        iend = mRootBones.end();
        for (i = mRootBones.begin(); i != iend; ++i)
        {
            (*i)->_update(true, false);
        }
    }
    //---------------------------------------------------------------------
	void Skeleton::optimiseAllAnimations(bool respectsCurrentModeOnly, Real toleranceFactor)
	{
        AnimationList::iterator ai;
        for (ai = mAnimationsList.begin(); ai != mAnimationsList.end(); ++ai)
        {
			ai->second->optimise(respectsCurrentModeOnly, toleranceFactor);
		}
	}
	//---------------------------------------------------------------------
	void Skeleton::addLinkedSkeletonAnimationSource(const String& skelName, 
		Real scale)
	{
		// Check not already linked
		LinkedSkeletonAnimSourceList::iterator i;
		for (i = mLinkedSkeletonAnimSourceList.begin(); 
			i != mLinkedSkeletonAnimSourceList.end(); ++i)
		{
			if (skelName == i->skeletonName)
				return; // don't bother
		}

		if (mIsLoaded)
		{
			// Load immediately
			SkeletonPtr skelPtr = 
				SkeletonManager::getSingleton().load(skelName, mGroup);
			mLinkedSkeletonAnimSourceList.push_back(
				LinkedSkeletonAnimationSource(skelName, scale, skelPtr));

		}
		else
		{
			// Load later
			mLinkedSkeletonAnimSourceList.push_back(
				LinkedSkeletonAnimationSource(skelName, scale));
		}

	}
	//---------------------------------------------------------------------
	void Skeleton::removeAllLinkedSkeletonAnimationSources(void)
	{
		mLinkedSkeletonAnimSourceList.clear();
	}
	//---------------------------------------------------------------------
	Skeleton::LinkedSkeletonAnimSourceIterator 
	Skeleton::getLinkedSkeletonAnimationSourceIterator(void) const
	{
		return LinkedSkeletonAnimSourceIterator(
			mLinkedSkeletonAnimSourceList.begin(), 
			mLinkedSkeletonAnimSourceList.end());
	}
	//---------------------------------------------------------------------
    struct DeltaTransform
    {
        Vector3 translate;
        Quaternion rotate;
        Vector3 scale;
        bool isIdentity;
    };
    void Skeleton::_mergeSkeletonAnimations(const Skeleton* src,
        const BoneHandleMap& boneHandleMap,
        const StringVector& animations)
    {
        ushort handle;

        ushort numSrcBones = src->getNumBones();
        ushort numDstBones = this->getNumBones();

        if (boneHandleMap.size() != numSrcBones)
        {
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
                "Number of bones in the bone handle map must equal to "
                "number of bones in the source skeleton.",
                "Skeleton::_mergeSkeletonAnimations");
        }

        bool existsMissingBone = false;

        // Check source skeleton structures compatible with ourself (that means
        // identically bones with identical handles, and with same hierarchy, but
        // not necessary to have same number of bones and bone names).
        for (handle = 0; handle < numSrcBones; ++handle)
        {
            const Bone* srcBone = src->getBone(handle);
            ushort dstHandle = boneHandleMap[handle];

            // Does it exists in target skeleton?
            if (dstHandle < numDstBones)
            {
                Bone* destBone = this->getBone(dstHandle);

                // Check both bones have identical parent, or both are root bone.
                const Bone* srcParent = static_cast<Bone*>(srcBone->getParent());
                Bone* destParent = static_cast<Bone*>(destBone->getParent());
                if ((srcParent || destParent) &&
                    (!srcParent || !destParent ||
                     boneHandleMap[srcParent->getHandle()] != destParent->getHandle()))
                {
                    OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
                        "Source skeleton incompatible with this skeleton: "
                        "difference hierarchy between bone '" + srcBone->getName() +
                        "' and '" + destBone->getName() + "'.",
                        "Skeleton::_mergeSkeletonAnimations");
                }
            }
            else
            {
                existsMissingBone = true;
            }
        }

        // Clone bones if need
        if (existsMissingBone)
        {
            // Create missing bones
            for (handle = 0; handle < numSrcBones; ++handle)
            {
                const Bone* srcBone = src->getBone(handle);
                ushort dstHandle = boneHandleMap[handle];

                // The bone is missing in target skeleton?
                if (dstHandle >= numDstBones)
                {
                    Bone* dstBone = this->createBone(srcBone->getName(), dstHandle);
                    // Sets initial transform
                    dstBone->setPosition(srcBone->getInitialPosition());
                    dstBone->setOrientation(srcBone->getInitialOrientation());
                    dstBone->setScale(srcBone->getInitialScale());
                    dstBone->setInitialState();
                }
            }

            // Link new bones to parent
            for (handle = 0; handle < numSrcBones; ++handle)
            {
                const Bone* srcBone = src->getBone(handle);
                ushort dstHandle = boneHandleMap[handle];

                // Is new bone?
                if (dstHandle >= numDstBones)
                {
                    const Bone* srcParent = static_cast<Bone*>(srcBone->getParent());
                    if (srcParent)
                    {
                        Bone* destParent = this->getBone(boneHandleMap[srcParent->getHandle()]);
                        Bone* dstBone = this->getBone(dstHandle);
                        destParent->addChild(dstBone);
                    }
                }
            }

            // Derive root bones in case it was changed
            this->deriveRootBone();

            // Reset binding pose for new bones
            this->reset(true);
            this->setBindingPose();
        }

        //
        // We need to adapt animations from source to target skeleton, but since source
        // and target skeleton bones bind transform might difference, so we need to alter
        // keyframes in source to suit to target skeleton.
        //
        // For any given animation time, formula:
        //
        //      LocalTransform = BindTransform * KeyFrame;
        //      DerivedTransform = ParentDerivedTransform * LocalTransform
        //
        // And all derived transforms should be keep identically after adapt to
        // target skeleton, Then:
        //
        //      DestDerivedTransform == SrcDerivedTransform
        //      DestParentDerivedTransform == SrcParentDerivedTransform
        // ==>
        //      DestLocalTransform = SrcLocalTransform
        // ==>
        //      DestBindTransform * DestKeyFrame = SrcBindTransform * SrcKeyFrame
        // ==>
        //      DestKeyFrame = inverse(DestBindTransform) * SrcBindTransform * SrcKeyFrame
        //
        // We define (inverse(DestBindTransform) * SrcBindTransform) as 'delta-transform' here.
        //

        // Calculate delta-transforms for all source bones.
        std::vector<DeltaTransform> deltaTransforms(numSrcBones);
        for (handle = 0; handle < numSrcBones; ++handle)
        {
            const Bone* srcBone = src->getBone(handle);
            DeltaTransform& deltaTransform = deltaTransforms[handle];
            ushort dstHandle = boneHandleMap[handle];

            if (dstHandle < numDstBones)
            {
                // Common bone, calculate delta-transform

                Bone* dstBone = this->getBone(dstHandle);

                deltaTransform.translate = srcBone->getInitialPosition() - dstBone->getInitialPosition();
                deltaTransform.rotate = dstBone->getInitialOrientation().Inverse() * srcBone->getInitialOrientation();
                deltaTransform.scale = srcBone->getInitialScale() / dstBone->getInitialScale();

                // Check whether or not delta-transform is identity
                const Real tolerance = 1e-3f;
                Vector3 axis;
                Radian angle;
                deltaTransform.rotate.ToAngleAxis(angle, axis);
                deltaTransform.isIdentity =
                    deltaTransform.translate.positionEquals(Vector3::ZERO, tolerance) &&
                    deltaTransform.scale.positionEquals(Vector3::UNIT_SCALE, tolerance) &&
                    Math::RealEqual(angle.valueRadians(), 0.0f, tolerance);
            }
            else
            {
                // New bone, the delta-transform is identity

                deltaTransform.translate = Vector3::ZERO;
                deltaTransform.rotate = Quaternion::IDENTITY;
                deltaTransform.scale = Vector3::UNIT_SCALE;
                deltaTransform.isIdentity = true;
            }
        }

        // Now copy animations

        ushort numAnimations;
        if (animations.empty())
            numAnimations = src->getNumAnimations();
        else
            numAnimations = static_cast<ushort>(animations.size());
        for (ushort i = 0; i < numAnimations; ++i)
        {
            const Animation* srcAnimation;
            if (animations.empty())
            {
                // Get animation of source skeleton by the given index
                srcAnimation = src->getAnimation(i);
            }
            else
            {
                // Get animation of source skeleton by the given name
                const LinkedSkeletonAnimationSource* linker;
                srcAnimation = src->_getAnimationImpl(animations[i], &linker);
                if (!srcAnimation || linker)
                {
                    OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
                        "No animation entry found named " + animations[i],
                        "Skeleton::_mergeSkeletonAnimations");
                }
            }

            // Create target animation
            Animation* dstAnimation = this->createAnimation(srcAnimation->getName(), srcAnimation->getLength());

            // Copy interpolation modes
            dstAnimation->setInterpolationMode(srcAnimation->getInterpolationMode());
            dstAnimation->setRotationInterpolationMode(srcAnimation->getRotationInterpolationMode());

            // Copy track for each bone
            for (handle = 0; handle < numSrcBones; ++handle)
            {
                const DeltaTransform& deltaTransform = deltaTransforms[handle];
                ushort dstHandle = boneHandleMap[handle];

                if (srcAnimation->hasNodeTrack(handle))
                {
                    // Clone track from source animation

                    const NodeAnimationTrack* srcTrack = srcAnimation->getNodeTrack(handle);
                    NodeAnimationTrack* dstTrack = dstAnimation->createNodeTrack(dstHandle, this->getBone(dstHandle));
                    dstTrack->setUseShortestRotationPath(srcTrack->getUseShortestRotationPath());

                    ushort numKeyFrames = srcTrack->getNumKeyFrames();
                    for (ushort k = 0; k < numKeyFrames; ++k)
                    {
                        const TransformKeyFrame* srcKeyFrame = srcTrack->getNodeKeyFrame(k);
                        TransformKeyFrame* dstKeyFrame = dstTrack->createNodeKeyFrame(srcKeyFrame->getTime());

                        // Adjust keyframes to match target binding pose
                        if (deltaTransform.isIdentity)
                        {
                            dstKeyFrame->setTranslate(srcKeyFrame->getTranslate());
                            dstKeyFrame->setRotation(srcKeyFrame->getRotation());
                            dstKeyFrame->setScale(srcKeyFrame->getScale());
                        }
                        else
                        {
                            dstKeyFrame->setTranslate(deltaTransform.translate + srcKeyFrame->getTranslate());
                            dstKeyFrame->setRotation(deltaTransform.rotate * srcKeyFrame->getRotation());
                            dstKeyFrame->setScale(deltaTransform.scale * srcKeyFrame->getScale());
                        }
                    }
                }
                else if (!deltaTransform.isIdentity)
                {
                    // Create 'static' track for this bone

                    NodeAnimationTrack* dstTrack = dstAnimation->createNodeTrack(dstHandle, this->getBone(dstHandle));
                    TransformKeyFrame* dstKeyFrame;

                    dstKeyFrame = dstTrack->createNodeKeyFrame(0);
                    dstKeyFrame->setTranslate(deltaTransform.translate);
                    dstKeyFrame->setRotation(deltaTransform.rotate);
                    dstKeyFrame->setScale(deltaTransform.scale);

                    dstKeyFrame = dstTrack->createNodeKeyFrame(dstAnimation->getLength());
                    dstKeyFrame->setTranslate(deltaTransform.translate);
                    dstKeyFrame->setRotation(deltaTransform.rotate);
                    dstKeyFrame->setScale(deltaTransform.scale);
                }
            }
        }
    }
    //---------------------------------------------------------------------
    void Skeleton::_buildMapBoneByHandle(const Skeleton* src,
        BoneHandleMap& boneHandleMap) const
    {
        ushort numSrcBones = src->getNumBones();
        boneHandleMap.resize(numSrcBones);

        for (ushort handle = 0; handle < numSrcBones; ++handle)
        {
            boneHandleMap[handle] = handle;
        }
    }
    //---------------------------------------------------------------------
    void Skeleton::_buildMapBoneByName(const Skeleton* src,
        BoneHandleMap& boneHandleMap) const
    {
        ushort numSrcBones = src->getNumBones();
        boneHandleMap.resize(numSrcBones);

        ushort newBoneHandle = this->getNumBones();
        for (ushort handle = 0; handle < numSrcBones; ++handle)
        {
            const Bone* srcBone = src->getBone(handle);
            BoneListByName::const_iterator i = this->mBoneListByName.find(srcBone->getName());
            if (i == mBoneListByName.end())
                boneHandleMap[handle] = newBoneHandle++;
            else
                boneHandleMap[handle] = i->second->getHandle();
        }
    }

}

