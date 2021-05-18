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
#include "OgreAnimationTrack.h"
#include "OgreAnimation.h"
#include "OgreKeyFrame.h"
#include "OgreNode.h"
#include "OgreLogManager.h"
#include "OgreHardwareBufferManager.h"
#include "OgreMesh.h"
#include "OgreException.h"
#include "OgrePoolAllocator.h"

namespace Ogre {

    namespace {
        // Locally key frame search helper
        struct KeyFrameTimeLess
        {
            bool operator() (const KeyFrame* kf, const KeyFrame* kf2) const
            {
                return kf->getTime() < kf2->getTime();
            }
        };
    }
    //---------------------------------------------------------------------
    //---------------------------------------------------------------------
    AnimationTrack::AnimationTrack(Animation* parent, unsigned short handle) :
		mParent(parent), mHandle(handle)
    {
    }
    //---------------------------------------------------------------------
    AnimationTrack::~AnimationTrack()
    {
        assert(mKeyFrames.empty());
    }
    //---------------------------------------------------------------------
    unsigned short AnimationTrack::getNumKeyFrames(void) const
    {
        return (unsigned short)mKeyFrames.size();
    }
    //---------------------------------------------------------------------
    KeyFrame* AnimationTrack::getKeyFrame(unsigned short index) const
    {
		// If you hit this assert, then the keyframe index is out of bounds
        assert( index < (ushort)mKeyFrames.size() );

        return mKeyFrames[index];
    }
    //---------------------------------------------------------------------
    Real AnimationTrack::getKeyFramesAtTime(const TimeIndex& timeIndex, KeyFrame** keyFrame1, KeyFrame** keyFrame2,
        unsigned short* firstKeyIndex) const
    {
        // Parametric time
        // t1 = time of previous keyframe
        // t2 = time of next keyframe
        Real t1, t2;

        Real timePos = timeIndex.getTimePos();

        // Find first keyframe after or on current time
        KeyFrameList::const_iterator i;
        if (timeIndex.hasKeyIndex())
        {
            // Global keyframe index available, map to local keyframe index directly.
            assert(timeIndex.getKeyIndex() < mKeyFrameIndexMap.size());
            i = mKeyFrames.begin() + mKeyFrameIndexMap[timeIndex.getKeyIndex()];
#ifdef _DEBUG
            KeyFrame timeKey(0, timePos);
            if (i != std::lower_bound(mKeyFrames.begin(), mKeyFrames.end(), &timeKey, KeyFrameTimeLess()))
            {
                OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR,
                    "Optimised key frame search failed",
                    "AnimationTrack::getKeyFramesAtTime");
            }
#endif
        }
        else
        {
            // Wrap time
            Real totalAnimationLength = mParent->getLength();
            assert(totalAnimationLength > 0.0f && "Invalid animation length!");

            while (timePos > totalAnimationLength && totalAnimationLength > 0.0f)
            {
                timePos -= totalAnimationLength;
            }

            // No global keyframe index, need to search with local keyframes.
            KeyFrame timeKey(0, timePos);
            i = std::lower_bound(mKeyFrames.begin(), mKeyFrames.end(), &timeKey, KeyFrameTimeLess());
        }

        if (i == mKeyFrames.end())
        {
            // There is no keyframe after this time, wrap back to first
            *keyFrame2 = mKeyFrames.front();
            t2 = mParent->getLength() + (*keyFrame2)->getTime();

            // Use last keyframe as previous keyframe
            --i;
        }
        else
        {
            *keyFrame2 = *i;
            t2 = (*keyFrame2)->getTime();

            // Find last keyframe before or on current time
            if (i != mKeyFrames.begin() && timePos < (*i)->getTime())
            {
                --i;
            }
        }

        // Fill index of the first key
        if (firstKeyIndex)
        {
            *firstKeyIndex = std::distance(mKeyFrames.begin(), i);
        }

        *keyFrame1 = *i;

        t1 = (*keyFrame1)->getTime();

        if (t1 == t2)
        {
            // Same KeyFrame (only one)
            return 0.0;
        }
        else
        {
            return (timePos - t1) / (t2 - t1);
        }
    }
    //---------------------------------------------------------------------
    void AnimationTrack::_estimateNumKeyFrames(unsigned short numKeyFrames)
    {
        // reserve space for key frame list
        mKeyFrames.reserve(numKeyFrames);
    }
    //---------------------------------------------------------------------
    KeyFrame* AnimationTrack::_createKeyFrameUnsorted(Real timePos)
    {
        KeyFrame* kf = createKeyFrameImpl(timePos);

        mKeyFrames.push_back(kf);

        // Didn't notify any thing for speed, callee is supposed to
        // call _sortKeyFrames after it has done.

        return kf;
    }
    //---------------------------------------------------------------------
    void AnimationTrack::_sortKeyFrames(void)
    {
        std::stable_sort(mKeyFrames.begin(), mKeyFrames.end(), KeyFrameTimeLess());

        _keyFrameDataChanged();
        mParent->_keyFrameListChanged();
    }
    //---------------------------------------------------------------------
    KeyFrame* AnimationTrack::createKeyFrame(Real timePos)
    {
        KeyFrame* kf = createKeyFrameImpl(timePos);

        // Insert just before upper bound
        KeyFrameList::iterator i =
            std::upper_bound(mKeyFrames.begin(), mKeyFrames.end(), kf, KeyFrameTimeLess());
        mKeyFrames.insert(i, kf);

        _keyFrameDataChanged();
        mParent->_keyFrameListChanged();

        return kf;

    }
    //---------------------------------------------------------------------
    void AnimationTrack::removeKeyFrame(unsigned short index)
    {
		// If you hit this assert, then the keyframe index is out of bounds
        assert( index < (ushort)mKeyFrames.size() );

        KeyFrameList::iterator i = mKeyFrames.begin();

        i += index;

        destroyKeyFrameImpl(*i);

        mKeyFrames.erase(i);

        _keyFrameDataChanged();
        mParent->_keyFrameListChanged();


    }
    //---------------------------------------------------------------------
    void AnimationTrack::removeAllKeyFrames(void)
    {
        KeyFrameList::iterator i = mKeyFrames.begin();

        for (; i != mKeyFrames.end(); ++i)
        {
            destroyKeyFrameImpl(*i);
        }

        mKeyFrames.clear();

        _keyFrameDataChanged();
        mParent->_keyFrameListChanged();
    }
    //---------------------------------------------------------------------
    void AnimationTrack::_collectKeyFrameTimes(std::vector<Real>& keyFrameTimes)
    {
        for (KeyFrameList::const_iterator i = mKeyFrames.begin(); i != mKeyFrames.end(); ++i)
        {
            Real timePos = (*i)->getTime();

            std::vector<Real>::iterator it =
                std::lower_bound(keyFrameTimes.begin(), keyFrameTimes.end(), timePos);
            if (it == keyFrameTimes.end() || *it != timePos)
            {
                keyFrameTimes.insert(it, timePos);
            }
        }
    }
    //---------------------------------------------------------------------
    void AnimationTrack::_buildKeyFrameIndexMap(const std::vector<Real>& keyFrameTimes)
    {
        // Pre-allocate memory
        mKeyFrameIndexMap.resize(keyFrameTimes.size() + 1);

        size_t i = 0, j = 0;
        while (j <= keyFrameTimes.size())
        {
            mKeyFrameIndexMap[j] = static_cast<ushort>(i);
            while (i < mKeyFrames.size() && mKeyFrames[i]->getTime() <= keyFrameTimes[j])
                ++i;
            ++j;
        }
    }
	//---------------------------------------------------------------------
	void AnimationTrack::populateClone(AnimationTrack* clone) const
	{
		for (KeyFrameList::const_iterator i = mKeyFrames.begin();
			i != mKeyFrames.end(); ++i)
		{
			KeyFrame* clonekf = (*i)->_clone(clone);
			clone->mKeyFrames.push_back(clonekf);
		}
	}
	//---------------------------------------------------------------------
	//---------------------------------------------------------------------
	// Numeric specialisations
	//---------------------------------------------------------------------
    OGRE_DECLARE_OBJECT_POOL(static, msNumericKeyFramePool, NumericKeyFrame);
    //---------------------------------------------------------------------
	NumericAnimationTrack::NumericAnimationTrack(Animation* parent,
		unsigned short handle)
		: AnimationTrack(parent, handle)
	{
	}
	//---------------------------------------------------------------------
	NumericAnimationTrack::NumericAnimationTrack(Animation* parent,
		unsigned short handle, AnimableValuePtr& target)
		:AnimationTrack(parent, handle), mTargetAnim(target)
	{
	}
    //---------------------------------------------------------------------
    NumericAnimationTrack::~NumericAnimationTrack()
    {
        // have to call this here reather than in AnimationTrack destructor
        // since calling virtual methods in base destructors causes crash
        removeAllKeyFrames();
    }
	//---------------------------------------------------------------------
	const AnimableValuePtr& NumericAnimationTrack::getAssociatedAnimable(void) const
	{
		return mTargetAnim;
	}
	//---------------------------------------------------------------------
	void NumericAnimationTrack::setAssociatedAnimable(const AnimableValuePtr& val)
	{
		mTargetAnim = val;
	}
	//---------------------------------------------------------------------
	KeyFrame* NumericAnimationTrack::createKeyFrameImpl(Real time)
	{
		return OGRE_POOL_OBJECT_NEW(msNumericKeyFramePool, NumericKeyFrame, (this, time));
	}
	//---------------------------------------------------------------------
	void NumericAnimationTrack::destroyKeyFrameImpl(KeyFrame* kf)
	{
		OGRE_POOL_OBJECT_DELETE(msNumericKeyFramePool, static_cast<NumericKeyFrame*>(kf));
	}
	//---------------------------------------------------------------------
	void NumericAnimationTrack::getInterpolatedKeyFrame(const TimeIndex& timeIndex,
		KeyFrame* kf) const
	{
		NumericKeyFrame* kret = static_cast<NumericKeyFrame*>(kf);

        // Keyframe pointers
		KeyFrame *kBase1, *kBase2;
        NumericKeyFrame *k1, *k2;
        unsigned short firstKeyIndex;

        Real t = this->getKeyFramesAtTime(timeIndex, &kBase1, &kBase2, &firstKeyIndex);
		k1 = static_cast<NumericKeyFrame*>(kBase1);
		k2 = static_cast<NumericKeyFrame*>(kBase2);

        if (t == 0.0)
        {
            // Just use k1
            kret->setValue(k1->getValue());
        }
        else
        {
            // Interpolate by t
			AnyNumeric diff = k2->getValue() - k1->getValue();
			kret->setValue(k1->getValue() + diff * t);
        }
	}
	//---------------------------------------------------------------------
	void NumericAnimationTrack::apply(const TimeIndex& timeIndex, Real weight, bool accumulate,
		Real scale)
	{
		applyToAnimable(mTargetAnim, timeIndex, weight, scale);
	}
	//---------------------------------------------------------------------
	void NumericAnimationTrack::applyToAnimable(const AnimableValuePtr& anim, const TimeIndex& timeIndex,
		Real weight, Real scale)
	{
		// Nothing to do if no keyframes or zero weight, scale
		if (mKeyFrames.empty() || !weight || !scale)
			return;

		NumericKeyFrame kf(0, timeIndex.getTimePos());
		getInterpolatedKeyFrame(timeIndex, &kf);
		// add to existing. Weights are not relative, but treated as
		// absolute multipliers for the animation
		AnyNumeric val = kf.getValue() * (weight * scale);

		anim->applyDeltaValue(val);

	}
	//--------------------------------------------------------------------------
	NumericKeyFrame* NumericAnimationTrack::_createNumericKeyFrameUnsorted(Real timePos)
	{
		return static_cast<NumericKeyFrame*>(_createKeyFrameUnsorted(timePos));
	}
	//--------------------------------------------------------------------------
	NumericKeyFrame* NumericAnimationTrack::createNumericKeyFrame(Real timePos)
	{
		return static_cast<NumericKeyFrame*>(createKeyFrame(timePos));
	}
	//--------------------------------------------------------------------------
	NumericKeyFrame* NumericAnimationTrack::getNumericKeyFrame(unsigned short index) const
	{
		return static_cast<NumericKeyFrame*>(getKeyFrame(index));
	}
    //---------------------------------------------------------------------
	NumericAnimationTrack* NumericAnimationTrack::_clone(Animation* newParent) const
	{
		NumericAnimationTrack* newTrack = 
			newParent->createNumericTrack(mHandle);
		newTrack->mTargetAnim = mTargetAnim;
		populateClone(newTrack);
		return newTrack;
	}
    //---------------------------------------------------------------------
	//---------------------------------------------------------------------
	// Node specialisations
	//---------------------------------------------------------------------
    OGRE_DECLARE_OBJECT_POOL(static, msTransformKeyFramePool, TransformKeyFrame);
    //---------------------------------------------------------------------
	NodeAnimationTrack::NodeAnimationTrack(Animation* parent, unsigned short handle)
		: AnimationTrack(parent, handle), mTargetNode(0)
        , mSplines(0), mSplineBuildNeeded(false)
        , mUseShortestRotationPath(true)
	{
	}
	//---------------------------------------------------------------------
	NodeAnimationTrack::NodeAnimationTrack(Animation* parent, unsigned short handle,
		Node* targetNode)
		: AnimationTrack(parent, handle), mTargetNode(targetNode)
        , mSplines(0), mSplineBuildNeeded(false)
        , mUseShortestRotationPath(true)
	{
	}
    //---------------------------------------------------------------------
    NodeAnimationTrack::~NodeAnimationTrack()
    {
        delete mSplines;

        // have to call this here reather than in AnimationTrack destructor
        // since calling virtual methods in base destructors causes crash
        removeAllKeyFrames();
    }
	//---------------------------------------------------------------------
    void NodeAnimationTrack::getInterpolatedKeyFrame(const TimeIndex& timeIndex, KeyFrame* kf) const
    {
		TransformKeyFrame* kret = static_cast<TransformKeyFrame*>(kf);

        // Keyframe pointers
		KeyFrame *kBase1, *kBase2;
        TransformKeyFrame *k1, *k2;
        unsigned short firstKeyIndex;

        Real t = this->getKeyFramesAtTime(timeIndex, &kBase1, &kBase2, &firstKeyIndex);
		k1 = static_cast<TransformKeyFrame*>(kBase1);
		k2 = static_cast<TransformKeyFrame*>(kBase2);

        if (t == 0.0)
        {
            // Just use k1
            kret->setRotation(k1->getRotation());
            kret->setTranslate(k1->getTranslate());
            kret->setScale(k1->getScale());
        }
        else
        {
            // Interpolate by t
            Animation::InterpolationMode im = mParent->getInterpolationMode();
            Animation::RotationInterpolationMode rim =
                mParent->getRotationInterpolationMode();
            Vector3 base;
            switch(im)
            {
            case Animation::IM_LINEAR:
                // Interpolate linearly
                // Rotation
                // Interpolate to nearest rotation if mUseShortestRotationPath set
                if (rim == Animation::RIM_LINEAR)
                {
                    kret->setRotation( Quaternion::nlerp(t, k1->getRotation(),
                        k2->getRotation(), mUseShortestRotationPath) );
                }
                else //if (rim == Animation::RIM_SPHERICAL)
                {
                    kret->setRotation( Quaternion::Slerp(t, k1->getRotation(),
					    k2->getRotation(), mUseShortestRotationPath) );
                }

                // Translation
                base = k1->getTranslate();
                kret->setTranslate( base + ((k2->getTranslate() - base) * t) );

                // Scale
                base = k1->getScale();
                kret->setScale( base + ((k2->getScale() - base) * t) );
                break;

            case Animation::IM_SPLINE:
                // Spline interpolation

                // Build splines if required
                if (mSplineBuildNeeded)
                {
                    buildInterpolationSplines();
                }

                // Rotation, take mUseShortestRotationPath into account
                kret->setRotation( mSplines->rotationSpline.interpolate(firstKeyIndex, t,
					mUseShortestRotationPath) );

                // Translation
                kret->setTranslate( mSplines->positionSpline.interpolate(firstKeyIndex, t) );

                // Scale
                kret->setScale( mSplines->scaleSpline.interpolate(firstKeyIndex, t) );

                break;
            }

        }
    }
    //---------------------------------------------------------------------
    void NodeAnimationTrack::apply(const TimeIndex& timeIndex, Real weight, bool accumulate,
		Real scale)
    {
        applyToNode(mTargetNode, timeIndex, weight, accumulate, scale);

    }
    //---------------------------------------------------------------------
    Node* NodeAnimationTrack::getAssociatedNode(void) const
    {
        return mTargetNode;
    }
    //---------------------------------------------------------------------
    void NodeAnimationTrack::setAssociatedNode(Node* node)
    {
        mTargetNode = node;
    }
    //---------------------------------------------------------------------
    void NodeAnimationTrack::applyToNode(Node* node, const TimeIndex& timeIndex, Real weight,
		bool accumulate, Real scl)
    {
		// Nothing to do if no keyframes or zero weight
		if (mKeyFrames.empty() || !weight)
			return;

        TransformKeyFrame kf(0, timeIndex.getTimePos());
		getInterpolatedKeyFrame(timeIndex, &kf);
		if (accumulate)
        {
            // add to existing. Weights are not relative, but treated as absolute multipliers for the animation
            Vector3 translate = kf.getTranslate() * weight * scl;
			node->translate(translate);

			// interpolate between no-rotation and full rotation, to point 'weight', so 0 = no rotate, 1 = full
            Quaternion rotate;
            Animation::RotationInterpolationMode rim =
                mParent->getRotationInterpolationMode();
            if (rim == Animation::RIM_LINEAR)
            {
                rotate = Quaternion::nlerp(weight, Quaternion::IDENTITY, kf.getRotation());
            }
            else //if (rim == Animation::RIM_SPHERICAL)
            {
                rotate = Quaternion::Slerp(weight, Quaternion::IDENTITY, kf.getRotation());
            }
			node->rotate(rotate);

			Vector3 scale = kf.getScale();
			// Not sure how to modify scale for cumulative anims... leave it alone
			//scale = ((Vector3::UNIT_SCALE - kf.getScale()) * weight) + Vector3::UNIT_SCALE;
			if (scl != 1.0f && scale != Vector3::UNIT_SCALE)
			{
				scale = Vector3::UNIT_SCALE + (scale - Vector3::UNIT_SCALE) * scl;
			}
			node->scale(scale);
		}
        else
        {
			// apply using weighted transform method
			Vector3 scale = kf.getScale();
			if (scl != 1.0f && scale != Vector3::UNIT_SCALE)
			{
				scale = Vector3::UNIT_SCALE + (scale - Vector3::UNIT_SCALE) * scl;
			}
			node->_weightedTransform(weight, kf.getTranslate() * scl, kf.getRotation(),
				scale);
		}

        /*
        // DEBUG
        if (!mMainWindow)
        {
            mMainWindow = Root::getSingleton().getRenderWindow("OGRE Render Window");
        }
        String msg = "Time: ";
        msg << timePos;
        mMainWindow->setDebugText(msg);
        */

        //node->rotate(kf.getRotation() * weight);
        //node->translate(kf.getTranslate() * weight);




    }
    //---------------------------------------------------------------------
    void NodeAnimationTrack::buildInterpolationSplines(void) const
    {
        // Allocate splines if not exists
        if (!mSplines)
        {
            mSplines = new Splines;
        }

        // Cache to register for optimisation
        Splines* splines = mSplines;

        // Don't calc automatically, do it on request at the end
        splines->positionSpline.setAutoCalculate(false);
        splines->rotationSpline.setAutoCalculate(false);
        splines->scaleSpline.setAutoCalculate(false);

        splines->positionSpline.clear();
        splines->rotationSpline.clear();
        splines->scaleSpline.clear();

        KeyFrameList::const_iterator i, iend;
        iend = mKeyFrames.end(); // precall to avoid overhead
        for (i = mKeyFrames.begin(); i != iend; ++i)
        {
			TransformKeyFrame* kf = static_cast<TransformKeyFrame*>(*i);
            splines->positionSpline.addPoint(kf->getTranslate());
            splines->rotationSpline.addPoint(kf->getRotation());
            splines->scaleSpline.addPoint(kf->getScale());
        }

        splines->positionSpline.recalcTangents();
        splines->rotationSpline.recalcTangents();
        splines->scaleSpline.recalcTangents();


        mSplineBuildNeeded = false;
    }

    //---------------------------------------------------------------------
	void NodeAnimationTrack::setUseShortestRotationPath(bool useShortestPath)
	{
		mUseShortestRotationPath = useShortestPath ;
	}

    //---------------------------------------------------------------------
	bool NodeAnimationTrack::getUseShortestRotationPath() const
	{
		return mUseShortestRotationPath ;
	}
    //---------------------------------------------------------------------
    void NodeAnimationTrack::_keyFrameDataChanged(void) const
    {
        mSplineBuildNeeded = true;
    }
    //---------------------------------------------------------------------
	bool NodeAnimationTrack::hasNonZeroKeyFrames(void) const
	{
        KeyFrameList::const_iterator i = mKeyFrames.begin();
        for (; i != mKeyFrames.end(); ++i)
        {
			// look for keyframes which have any component which is non-zero
			// Since exporters can be a little inaccurate sometimes we use a
			// tolerance value rather than looking for nothing
			TransformKeyFrame* kf = static_cast<TransformKeyFrame*>(*i);
			Vector3 trans = kf->getTranslate();
			Vector3 scale = kf->getScale();
			Vector3 axis;
			Radian angle;
			kf->getRotation().ToAngleAxis(angle, axis);
			Real tolerance = 1e-3f;
			if (!trans.positionEquals(Vector3::ZERO, tolerance) ||
				!scale.positionEquals(Vector3::UNIT_SCALE, tolerance) ||
				!Math::RealEqual(angle.valueRadians(), 0.0f, tolerance))
			{
				return true;
			}

		}

		return false;
	}
    //---------------------------------------------------------------------
    static bool _isLinearCorrlelation(const TransformKeyFrame* kf0,
        const TransformKeyFrame* kf, const TransformKeyFrame* kf1,
        Real toleranceFactor,
        Animation::RotationInterpolationMode rim, bool shortestRotationPath)
    {
        Real t0 = kf0->getTime();
        Real t1 = kf1->getTime();
        if (t0 == t1)   // Just in case
            return true;

        // Calculate factor that used to interpolate between kf0 and kf1
        // for the middle keyframe
        Real f = (kf->getTime() - t0) / (t1 - t0);

        // Calculate squared error factor which dependent on both interpolation and tolerance factor
        Real squaredErrorFactor = Math::Sqr((0.5f - Math::Abs(f - 0.5f)) * toleranceFactor);

        // Check translate
        const Vector3& trans = kf->getTranslate();
        const Vector3& trans0 = kf0->getTranslate();
        const Vector3& trans1 = kf1->getTranslate();
        Vector3 transDiff = trans1 - trans0;
        Vector3 interpolatedTrans = trans0 + f * transDiff;
        Real transTolerance = transDiff.squaredLength() * squaredErrorFactor;
        Real transError = (interpolatedTrans - trans).squaredLength();
        if (transError > transTolerance)
        {
            return false;
        }

        // Check rotation
        const Quaternion& rot = kf->getRotation();
        const Quaternion& rot0 = kf0->getRotation();
        const Quaternion& rot1 = kf1->getRotation();
        Quaternion interpolatedRot = rim == Animation::RIM_LINEAR ?
            Quaternion::nlerp(f, rot0, rot1, shortestRotationPath) :
            Quaternion::Slerp(f, rot0, rot1, shortestRotationPath);
        Real rotTolerance = Math::Sqr(Math::ACos(rot0.Dot(rot1)).valueRadians()) * squaredErrorFactor;
        Real rotError = Math::Sqr(Math::ACos(interpolatedRot.Dot(rot)).valueRadians());
        if (rotError > rotTolerance)
        {
            return false;
        }

        // Check scale
        const Vector3& scale = kf->getScale();
        const Vector3& scale0 = kf0->getScale();
        const Vector3& scale1 = kf1->getScale();
        Vector3 scaleDiff = scale1 - scale0;
        Vector3 interpolatedScale = scale0 + f * scaleDiff;
        Real scaleTolerance = scaleDiff.squaredLength() * squaredErrorFactor;
        Real scaleError = (interpolatedScale - scale).squaredLength();
        if (scaleError > scaleTolerance)
        {
            return false;
        }

        // All attributes are linear correlation
        return true;
    }
	void NodeAnimationTrack::optimise(bool respectsCurrentModeOnly, Real toleranceFactor)
	{
		std::list<unsigned short> removeList;

        static bool newOptimiseAlgo = true;
if (newOptimiseAlgo) {
        // Calculate number of end keyframes to preserve (not includes boundary keys),
        // and the rotation interpolation mode that used to evaluating rotation.
        size_t numEndKeysToPreserve;
        Animation::RotationInterpolationMode rim;
        if (respectsCurrentModeOnly)
        {
            rim = mParent->getRotationInterpolationMode();
            if (mParent->getInterpolationMode() == Animation::IM_LINEAR)
            {
                numEndKeysToPreserve = 1;
            }
            else
            {
                // Spline interpolation need 2 at each end to preserve tangents.
                numEndKeysToPreserve = 2;
            }
        }
        else
        {
            // Assume spherical rotation interpolation mode
            rim = Animation::RIM_SPHERICAL;
            // Assume spline interpolation
            numEndKeysToPreserve = 2;
        }

        size_t first = 0;
        while (first + 2 * numEndKeysToPreserve < mKeyFrames.size())
        {
            const TransformKeyFrame* kf0 = static_cast<const TransformKeyFrame*>(mKeyFrames[first]);

            // Search the maximise last keyframe, which all keyframes between
            // first and last are eliminable
            size_t last = first + 2 * numEndKeysToPreserve;
            for (; last < mKeyFrames.size(); ++last)
            {
                const TransformKeyFrame* kf1 = static_cast<const TransformKeyFrame*>(mKeyFrames[last]);

                // Check whether or not all keyframes between first and last are eliminable
                bool eliminable = true;
                for (size_t i = first + 1; i < last; ++i)
                {
                    const TransformKeyFrame* kf = static_cast<const TransformKeyFrame*>(mKeyFrames[i]);

                    // The keyframe is eliminable only if it can interpolated by other keyframes,
                    // for now supports linear corrlelation only.
                    if (!_isLinearCorrlelation(kf0, kf, kf1, toleranceFactor, rim, mUseShortestRotationPath))
                    {
                        eliminable = false;
                        break;
                    }
                }
                if (!eliminable)
                {
                    break;
                }
            }

            // Do we have keyframes to remove?
            if (first + 2 * numEndKeysToPreserve < last)
            {
                // Skip preserve keyframes
                first += numEndKeysToPreserve;

                // Remove the eliminable 'middle' keyframes
                do
                {
                    removeList.push_back(first);
                } while (++first < last - numEndKeysToPreserve);

                // Continue process the keyframes that just following eliminted keyframe
            }
            else
            {
                // Process next keyframe
                ++first;
            }
        }
} else {
		// Eliminate duplicate keyframes from 2nd to penultimate keyframe
		// NB only eliminate middle keys from sequences of 5+ identical keyframes
		// since we need to preserve the boundary keys in place, and we need
		// 2 at each end to preserve tangents for spline interpolation
		Vector3 lasttrans;
		Vector3 lastscale;
		Quaternion lastorientation;
        KeyFrameList::iterator i = mKeyFrames.begin();
		Radian quatTolerance(1e-3f);
		unsigned short k = 0;
		ushort dupKfCount = 0;
        for (; i != mKeyFrames.end(); ++i, ++k)
        {
			TransformKeyFrame* kf = static_cast<TransformKeyFrame*>(*i);
			Vector3 newtrans = kf->getTranslate();
			Vector3 newscale = kf->getScale();
			Quaternion neworientation = kf->getRotation();
			// Ignore first keyframe; now include the last keyframe as we eliminate
			// only k-2 in a group of 5 to ensure we only eliminate middle keys
			if (i != mKeyFrames.begin() &&
				newtrans.positionEquals(lasttrans) &&
				newscale.positionEquals(lastscale) &&
				neworientation.equals(lastorientation, quatTolerance))
			{
				++dupKfCount;

				// 4 indicates this is the 5th duplicate keyframe
				if (dupKfCount == 4)
				{
					// remove the 'middle' keyframe
					removeList.push_back(k-2);
					--dupKfCount;
				}
			}
			else
			{
				// reset
				dupKfCount = 0;
				lasttrans = newtrans;
				lastscale = newscale;
				lastorientation = neworientation;
			}
		}
}

		// Now remove keyframes, in reverse order to avoid index revocation
		std::list<unsigned short>::reverse_iterator r = removeList.rbegin();
		for (; r!= removeList.rend(); ++r)
		{
			removeKeyFrame(*r);
		}


	}
	//--------------------------------------------------------------------------
	KeyFrame* NodeAnimationTrack::createKeyFrameImpl(Real time)
	{
		return OGRE_POOL_OBJECT_NEW(msTransformKeyFramePool, TransformKeyFrame, (this, time));
	}
	//---------------------------------------------------------------------
	void NodeAnimationTrack::destroyKeyFrameImpl(KeyFrame* kf)
	{
		OGRE_POOL_OBJECT_DELETE(msTransformKeyFramePool, static_cast<TransformKeyFrame*>(kf));
	}
	//--------------------------------------------------------------------------
	TransformKeyFrame* NodeAnimationTrack::_createNodeKeyFrameUnsorted(Real timePos)
	{
		return static_cast<TransformKeyFrame*>(_createKeyFrameUnsorted(timePos));
	}
	//--------------------------------------------------------------------------
	TransformKeyFrame* NodeAnimationTrack::createNodeKeyFrame(Real timePos)
	{
		return static_cast<TransformKeyFrame*>(createKeyFrame(timePos));
	}
	//--------------------------------------------------------------------------
	TransformKeyFrame* NodeAnimationTrack::getNodeKeyFrame(unsigned short index) const
	{
		return static_cast<TransformKeyFrame*>(getKeyFrame(index));
	}
    //---------------------------------------------------------------------
	NodeAnimationTrack* NodeAnimationTrack::_clone(Animation* newParent) const
	{
		NodeAnimationTrack* newTrack = 
			newParent->createNodeTrack(mHandle, mTargetNode);
		newTrack->mUseShortestRotationPath = mUseShortestRotationPath;
		populateClone(newTrack);
		return newTrack;
	}	
	//--------------------------------------------------------------------------
	//---------------------------------------------------------------------
	// Vertex specialisations
	//---------------------------------------------------------------------
    OGRE_DECLARE_OBJECT_POOL(static, msVertexMorphKeyFramePool, VertexMorphKeyFrame);
    OGRE_DECLARE_OBJECT_POOL(static, msVertexPoseKeyFramePool, VertexPoseKeyFrame);
    //---------------------------------------------------------------------
	VertexAnimationTrack::VertexAnimationTrack(Animation* parent,
		unsigned short handle, VertexAnimationType animType)
		: AnimationTrack(parent, handle), mAnimationType(animType)
	{
	}
	//--------------------------------------------------------------------------
	VertexAnimationTrack::VertexAnimationTrack(Animation* parent, unsigned short handle,
		VertexAnimationType animType, VertexData* targetData, TargetMode target)
		: AnimationTrack(parent, handle), mAnimationType(animType),
		mTargetVertexData(targetData), mTargetMode(target)
	{
	}
    //---------------------------------------------------------------------
    VertexAnimationTrack::~VertexAnimationTrack()
    {
        // have to call this here reather than in AnimationTrack destructor
        // since calling virtual methods in base destructors causes crash
        removeAllKeyFrames();
    }
	//--------------------------------------------------------------------------
	VertexMorphKeyFrame* VertexAnimationTrack::_createVertexMorphKeyFrameUnsorted(Real timePos)
	{
		if (mAnimationType != VAT_MORPH)
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"Morph keyframes can only be created on vertex tracks of type morph.",
				"VertexAnimationTrack::_createVertexMorphKeyFrameUnsorted");
		}
		return static_cast<VertexMorphKeyFrame*>(_createKeyFrameUnsorted(timePos));
	}
	//--------------------------------------------------------------------------
	VertexPoseKeyFrame* VertexAnimationTrack::_createVertexPoseKeyFrameUnsorted(Real timePos)
	{
		if (mAnimationType != VAT_POSE)
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"Pose keyframes can only be created on vertex tracks of type pose.",
				"VertexAnimationTrack::createVertexPoseKeyFrame");
		}
		return static_cast<VertexPoseKeyFrame*>(_createKeyFrameUnsorted(timePos));
	}
	//--------------------------------------------------------------------------
	VertexMorphKeyFrame* VertexAnimationTrack::createVertexMorphKeyFrame(Real timePos)
	{
		if (mAnimationType != VAT_MORPH)
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"Morph keyframes can only be created on vertex tracks of type morph.",
				"VertexAnimationTrack::createVertexMorphKeyFrame");
		}
		return static_cast<VertexMorphKeyFrame*>(createKeyFrame(timePos));
	}
	//--------------------------------------------------------------------------
	VertexPoseKeyFrame* VertexAnimationTrack::createVertexPoseKeyFrame(Real timePos)
	{
		if (mAnimationType != VAT_POSE)
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"Pose keyframes can only be created on vertex tracks of type pose.",
				"VertexAnimationTrack::createVertexPoseKeyFrame");
		}
		return static_cast<VertexPoseKeyFrame*>(createKeyFrame(timePos));
	}
	//--------------------------------------------------------------------------
	void VertexAnimationTrack::apply(const TimeIndex& timeIndex, Real weight, bool accumulate,
		Real scale)
	{
		applyToVertexData(mTargetVertexData, timeIndex, weight);
	}
	//--------------------------------------------------------------------------
	void VertexAnimationTrack::applyToVertexData(VertexData* data,
		const TimeIndex& timeIndex, Real weight, const PoseList* poseList)
	{
		// Nothing to do if no keyframes
		if (mKeyFrames.empty())
			return;

		// Get keyframes
		KeyFrame *kf1, *kf2;
		Real t = getKeyFramesAtTime(timeIndex, &kf1, &kf2);

		if (mAnimationType == VAT_MORPH)
		{
			VertexMorphKeyFrame* vkf1 = static_cast<VertexMorphKeyFrame*>(kf1);
			VertexMorphKeyFrame* vkf2 = static_cast<VertexMorphKeyFrame*>(kf2);

			if (mTargetMode == TM_HARDWARE)
			{
				// If target mode is hardware, need to bind our 2 keyframe buffers,
				// one to main pos, one to morph target texcoord
				assert(!data->hwAnimationDataList.empty() &&
					"Haven't set up hardware vertex animation elements!");

				// no use for TempBlendedBufferInfo here btw
				// NB we assume that position buffer is unshared
				// VertexDeclaration::getAutoOrganisedDeclaration should see to that
				const VertexElement* posElem =
					data->vertexDeclaration->findElementBySemantic(VES_POSITION);
				// Set keyframe1 data as original position
				data->vertexBufferBinding->setBinding(
					posElem->getSource(), vkf1->getVertexBuffer());
				// Set keyframe2 data as derived
				data->vertexBufferBinding->setBinding(
					data->hwAnimationDataList[0].targetVertexElement->getSource(),
					vkf2->getVertexBuffer());
				// save T for use later
				data->hwAnimationDataList[0].parametric = t;

			}
			else
			{
				// If target mode is software, need to software interpolate each vertex

				Mesh::softwareVertexMorph(
					t, vkf1->getVertexBuffer(), vkf2->getVertexBuffer(), data);
			}
		}
		else
		{
			// Pose

			VertexPoseKeyFrame* vkf1 = static_cast<VertexPoseKeyFrame*>(kf1);
			VertexPoseKeyFrame* vkf2 = static_cast<VertexPoseKeyFrame*>(kf2);

			// For each pose reference in key 1, we need to locate the entry in
			// key 2 and interpolate the influence
			const VertexPoseKeyFrame::PoseRefList& poseList1 = vkf1->getPoseReferences();
			const VertexPoseKeyFrame::PoseRefList& poseList2 = vkf2->getPoseReferences();
			for (VertexPoseKeyFrame::PoseRefList::const_iterator p1 = poseList1.begin();
				p1 != poseList1.end(); ++p1)
			{
				Real startInfluence = p1->influence;
				Real endInfluence = 0;
				// Search for entry in keyframe 2 list (if not there, will be 0)
				for (VertexPoseKeyFrame::PoseRefList::const_iterator p2 = poseList2.begin();
					p2 != poseList2.end(); ++p2)
				{
					if (p1->poseIndex == p2->poseIndex)
					{
						endInfluence = p2->influence;
						break;
					}
				}
				// Interpolate influence
				Real influence = startInfluence + t*(endInfluence - startInfluence);
				// Scale by animation weight
				influence = weight * influence;
				// Get pose
				assert (p1->poseIndex <= poseList->size());
				Pose* pose = (*poseList)[p1->poseIndex];
				// apply
				applyPoseToVertexData(pose, data, influence);
			}
			// Now deal with any poses in key 2 which are not in key 1
			for (VertexPoseKeyFrame::PoseRefList::const_iterator p2 = poseList2.begin();
				p2 != poseList2.end(); ++p2)
			{
				bool found = false;
				for (VertexPoseKeyFrame::PoseRefList::const_iterator p1 = poseList1.begin();
					p1 != poseList1.end(); ++p1)
				{
					if (p1->poseIndex == p2->poseIndex)
					{
						found = true;
						break;
					}
				}
				if (!found)
				{
					// Need to apply this pose too, scaled from 0 start
					Real influence = t * p2->influence;
					// Scale by animation weight
					influence = weight * influence;
					// Get pose
					assert (p2->poseIndex <= poseList->size());
					const Pose* pose = (*poseList)[p2->poseIndex];
					// apply
					applyPoseToVertexData(pose, data, influence);
				}
			} // key 2 iteration
		} // morph or pose animation
	}
	//-----------------------------------------------------------------------------
	void VertexAnimationTrack::applyPoseToVertexData(const Pose* pose,
		VertexData* data, Real influence)
	{
		if (mTargetMode == TM_HARDWARE)
		{
			// Hardware
			// If target mode is hardware, need to bind our pose buffer
			// to a target texcoord
			assert(!data->hwAnimationDataList.empty() &&
				"Haven't set up hardware vertex animation elements!");
			// no use for TempBlendedBufferInfo here btw
			// Set pose target as required
			size_t hwIndex = data->hwAnimDataItemsUsed++;
			// If we try to use too many poses, ignore extras
			if (hwIndex < data->hwAnimationDataList.size())
			{
				VertexData::HardwareAnimationData& animData = data->hwAnimationDataList[hwIndex];
				data->vertexBufferBinding->setBinding(
					animData.targetVertexElement->getSource(),
					pose->_getHardwareVertexBuffer(data->vertexCount));
				// save final influence in parametric
				animData.parametric = influence;

			}

		}
		else
		{
			// Software
			Mesh::softwareVertexPoseBlend(influence, pose->getVertexOffsets(), data);
		}

	}
	//--------------------------------------------------------------------------
	VertexMorphKeyFrame* VertexAnimationTrack::getVertexMorphKeyFrame(unsigned short index) const
	{
		if (mAnimationType != VAT_MORPH)
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"Morph keyframes can only be created on vertex tracks of type morph.",
				"VertexAnimationTrack::getVertexMorphKeyFrame");
		}

		return static_cast<VertexMorphKeyFrame*>(getKeyFrame(index));
	}
	//--------------------------------------------------------------------------
	VertexPoseKeyFrame* VertexAnimationTrack::getVertexPoseKeyFrame(unsigned short index) const
	{
		if (mAnimationType != VAT_POSE)
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"Pose keyframes can only be created on vertex tracks of type pose.",
				"VertexAnimationTrack::getVertexPoseKeyFrame");
		}

		return static_cast<VertexPoseKeyFrame*>(getKeyFrame(index));
	}
	//--------------------------------------------------------------------------
	KeyFrame* VertexAnimationTrack::createKeyFrameImpl(Real time)
	{
		switch(mAnimationType)
		{
		default:
		case VAT_MORPH:
            return OGRE_POOL_OBJECT_NEW(msVertexMorphKeyFramePool, VertexMorphKeyFrame, (this, time));
		case VAT_POSE:
			return OGRE_POOL_OBJECT_NEW(msVertexPoseKeyFramePool, VertexPoseKeyFrame, (this, time));
		};

	}
	//---------------------------------------------------------------------
	void VertexAnimationTrack::destroyKeyFrameImpl(KeyFrame* kf)
	{
		switch(mAnimationType)
		{
		default:
		case VAT_MORPH:
		    OGRE_POOL_OBJECT_DELETE(msVertexMorphKeyFramePool, static_cast<VertexMorphKeyFrame*>(kf));
            break;
        case VAT_POSE:
            OGRE_POOL_OBJECT_DELETE(msVertexPoseKeyFramePool, static_cast<VertexPoseKeyFrame*>(kf));
            break;
        }

	}
	//---------------------------------------------------------------------
	bool VertexAnimationTrack::hasNonZeroKeyFrames(void) const
	{
		if (mAnimationType == VAT_MORPH)
		{
			return !mKeyFrames.empty();
		}
		else
		{

			KeyFrameList::const_iterator i = mKeyFrames.begin();
			for (; i != mKeyFrames.end(); ++i)
			{
				// look for keyframes which have a pose influence which is non-zero
				const VertexPoseKeyFrame* kf = static_cast<const VertexPoseKeyFrame*>(*i);
				VertexPoseKeyFrame::ConstPoseRefIterator poseIt
					= kf->getPoseReferenceIterator();
				while (poseIt.hasMoreElements())
				{
					const VertexPoseKeyFrame::PoseRef& poseRef = poseIt.getNext();
					if (poseRef.influence > 0.0f)
						return true;
				}

			}

			return false;
		}
	}
	//---------------------------------------------------------------------
	void VertexAnimationTrack::optimise(bool respectsCurrentModeOnly, Real toleranceFactor)
	{
		// TODO - remove sequences of duplicate pose references?


	}
    //---------------------------------------------------------------------
	VertexAnimationTrack* VertexAnimationTrack::_clone(Animation* newParent) const
	{
		VertexAnimationTrack* newTrack = 
			newParent->createVertexTrack(mHandle, mAnimationType);
		newTrack->mTargetMode = mTargetMode;
		populateClone(newTrack);
		return newTrack;
	}	


}

