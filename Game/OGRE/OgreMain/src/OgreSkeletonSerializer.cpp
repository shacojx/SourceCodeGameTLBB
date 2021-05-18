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

#include "OgreSkeletonFileFormat.h"
#include "OgreSkeletonSerializer.h"
#include "OgreSkeleton.h"
#include "OgreAnimation.h"
#include "OgreAnimationTrack.h"
#include "OgreKeyFrame.h"
#include "OgreBone.h"
#include "OgreString.h"
#include "OgreDataStream.h"
#include "OgreLogManager.h"




namespace Ogre {
    /// stream overhead = ID + size
    const long STREAM_OVERHEAD_SIZE = sizeof(uint16) + sizeof(uint32);
    //---------------------------------------------------------------------
    SkeletonSerializer::SkeletonSerializer()
    {
        // Version number
        // NB changed to include bone names in 1.1
        mVersion = "[Serializer_v1.10]";
    }
    //---------------------------------------------------------------------
    SkeletonSerializer::~SkeletonSerializer()
    {
    }
    //---------------------------------------------------------------------
    void SkeletonSerializer::exportSkeleton(const Skeleton* pSkeleton, 
		const String& filename, Endian endianMode)
    {
		// Decide on endian mode
		determineEndianness(endianMode);

        String msg;
        mpfFile = fopen(filename.c_str(), "wb");
		if (!mpfFile)
		{
			OGRE_EXCEPT(Exception::ERR_CANNOT_WRITE_TO_FILE,
				"Unable to open file " + filename + " for writing",
				"SkeletonSerializer::exportSkeleton");
		}

        writeFileHeader();

        // Write main skeleton data
        LogManager::getSingleton().logMessage("Exporting bones..");
        writeSkeleton(pSkeleton);
        LogManager::getSingleton().logMessage("Bones exported.");

        // Write all animations
        unsigned short numAnims = pSkeleton->getNumAnimations();
        msg = "Exporting animations, count=";
		StringUtil::StrStreamType num;
		num << numAnims;
        msg += num.str();
        LogManager::getSingleton().logMessage(msg);
        for (unsigned short i = 0; i < numAnims; ++i)
        {
            Animation* pAnim = pSkeleton->getAnimation(i);
            msg = "Exporting animation: " + pAnim->getName();
            LogManager::getSingleton().logMessage(msg);
            writeAnimation(pSkeleton, pAnim);
            LogManager::getSingleton().logMessage("Animation exported.");

        }

		// Write links
		Skeleton::LinkedSkeletonAnimSourceIterator linkIt = 
			pSkeleton->getLinkedSkeletonAnimationSourceIterator();
		while(linkIt.hasMoreElements())
		{
			const LinkedSkeletonAnimationSource& link = linkIt.getNext();
			writeSkeletonAnimationLink(pSkeleton, link);
		}

        fclose(mpfFile);

    }
    //---------------------------------------------------------------------
    void SkeletonSerializer::importSkeleton(DataStreamPtr& stream, Skeleton* pSkel)
    {
		// Determine endianness (must be the first thing we do!)
		determineEndianness(stream);

		// Check header
        readFileHeader(stream);

        unsigned short streamID;
        while(!stream->eof())
        {
            streamID = readChunk(stream);
            switch (streamID)
            {
            case SKELETON_BONE:
                readBone(stream, pSkel);
                break;
            case SKELETON_BONE_PARENT:
                readBoneParent(stream, pSkel);
                break;
            case SKELETON_ANIMATION:
                readAnimation(stream, pSkel);
				break;
			case SKELETON_ANIMATION_LINK:
				readSkeletonAnimationLink(stream, pSkel);
				break;
            }
        }

        // Assume bones are stored in the binding pose
        pSkel->setBindingPose();


    }
    //---------------------------------------------------------------------
    void SkeletonSerializer::writeSkeleton(const Skeleton* pSkel)
    {
        // Write each bone
        unsigned short numBones = pSkel->getNumBones();
        unsigned short i;
        for (i = 0; i < numBones; ++i)
        {
            Bone* pBone = pSkel->getBone(i);
            writeBone(pSkel, pBone);
        }
        // Write parents
        for (i = 0; i < numBones; ++i)
        {
            Bone* pBone = pSkel->getBone(i);
            unsigned short handle = pBone->getHandle();
            Bone* pParent = (Bone*)pBone->getParent(); 
            if (pParent != NULL) 
            {
                writeBoneParent(pSkel, handle, pParent->getHandle());             
            }
        }
    }
    //---------------------------------------------------------------------
    void SkeletonSerializer::writeBone(const Skeleton* pSkel, const Bone* pBone)
    {
        writeChunkHeader(SKELETON_BONE, calcBoneSize(pSkel, pBone));

        unsigned short handle = pBone->getHandle();
        // char* name
        writeString(pBone->getName());
        // unsigned short handle            : handle of the bone, should be contiguous & start at 0
        writeShorts(&handle, 1);
        // Vector3 position                 : position of this bone relative to parent 
        writeObject(pBone->getPosition());
        // Quaternion orientation           : orientation of this bone relative to parent 
        writeObject(pBone->getOrientation());
        // Vector3 scale                    : scale of this bone relative to parent 
        if (pBone->getScale() != Vector3::UNIT_SCALE)
        {
            writeObject(pBone->getScale());
        }
    }
    //---------------------------------------------------------------------
    void SkeletonSerializer::writeBoneParent(const Skeleton* pSkel, 
        unsigned short boneId, unsigned short parentId)
    {
        writeChunkHeader(SKELETON_BONE_PARENT, calcBoneParentSize(pSkel));

        // unsigned short handle             : child bone
        writeShorts(&boneId, 1);
        // unsigned short parentHandle   : parent bone
        writeShorts(&parentId, 1);

    }
    //---------------------------------------------------------------------
    void SkeletonSerializer::writeAnimation(const Skeleton* pSkel, 
        const Animation* anim)
    {
        writeChunkHeader(SKELETON_ANIMATION, calcAnimationSize(pSkel, anim));

        // char* name                       : Name of the animation
        writeString(anim->getName());
        // float length                      : Length of the animation in seconds
        float len = anim->getLength();
        writeFloats(&len, 1);

        // Write all tracks
        Animation::NodeTrackIterator trackIt = anim->getNodeTrackIterator();
        while(trackIt.hasMoreElements())
        {
            writeAnimationTrack(pSkel, trackIt.getNext());
        }

    }
    //---------------------------------------------------------------------
    void SkeletonSerializer::writeAnimationTrack(const Skeleton* pSkel, 
        const NodeAnimationTrack* track)
    {
        writeChunkHeader(SKELETON_ANIMATION_TRACK, calcAnimationTrackSize(pSkel, track));

        // unsigned short boneIndex     : Index of bone to apply to
        Bone* bone = (Bone*)track->getAssociatedNode();
        unsigned short boneid = bone->getHandle();
        writeShorts(&boneid, 1);

#ifdef OGRE_SKELETON_SERIALIZER_OLD_VERSION
        // Write all keyframes
        for (unsigned short i = 0; i < track->getNumKeyFrames(); ++i)
        {
            writeKeyFrame(pSkel, track->getNodeKeyFrame(i));
        }
#else
        writeKeyFrameList(pSkel, track);
#endif

    }
    //---------------------------------------------------------------------
    void SkeletonSerializer::writeKeyFrame(const Skeleton* pSkel, 
        const TransformKeyFrame* key)
    {

        writeChunkHeader(SKELETON_ANIMATION_TRACK_KEYFRAME, 
            calcKeyFrameSize(pSkel, key));

        // float time                    : The time position (seconds)
        float time = key->getTime();
        writeFloats(&time, 1);
        // Quaternion rotate            : Rotation to apply at this keyframe
        writeObject(key->getRotation());
        // Vector3 translate            : Translation to apply at this keyframe
        writeObject(key->getTranslate());
        // Vector3 scale                : Scale to apply at this keyframe
        if (key->getScale() != Vector3::UNIT_SCALE)
        {
            writeObject(key->getScale());
        }
    }
    //---------------------------------------------------------------------
    size_t SkeletonSerializer::calcBoneSize(const Skeleton* pSkel, 
        const Bone* pBone)
    {
        size_t size = STREAM_OVERHEAD_SIZE;

        // handle
        size += sizeof(unsigned short);

        // position
        size += sizeof(float) * 3;

        // orientation
        size += sizeof(float) * 4;

        // scale
        if (pBone->getScale() != Vector3::UNIT_SCALE)
        {
            size += sizeof(float) * 3;
        }

        return size;
    }
    //---------------------------------------------------------------------
    size_t SkeletonSerializer::calcBoneSizeWithoutScale(const Skeleton* pSkel, 
        const Bone* pBone)
    {
        size_t size = STREAM_OVERHEAD_SIZE;

        // handle
        size += sizeof(unsigned short);

        // position
        size += sizeof(float) * 3;

        // orientation
        size += sizeof(float) * 4;

        return size;
    }
    //---------------------------------------------------------------------
    size_t SkeletonSerializer::calcBoneParentSize(const Skeleton* pSkel)
    {
        size_t size = STREAM_OVERHEAD_SIZE;

        // handle
        size += sizeof(unsigned short);

        // parent handle
        size += sizeof(unsigned short);

        return size;
    }
    //---------------------------------------------------------------------
    size_t SkeletonSerializer::calcAnimationSize(const Skeleton* pSkel, 
        const Animation* pAnim)
    {
        size_t size = STREAM_OVERHEAD_SIZE;

        // Name, including terminator
        size += pAnim->getName().length() + 1;
        // length
        size += sizeof(float);

        // Nested animation tracks
		Animation::NodeTrackIterator trackIt = pAnim->getNodeTrackIterator();
		while(trackIt.hasMoreElements())
		{
            size += calcAnimationTrackSize(pSkel, trackIt.getNext());
        }

        return size;
    }
    //---------------------------------------------------------------------
    size_t SkeletonSerializer::calcAnimationTrackSize(const Skeleton* pSkel, 
        const NodeAnimationTrack* pTrack)
    {
        size_t size = STREAM_OVERHEAD_SIZE;

        // unsigned short boneIndex     : Index of bone to apply to
        size += sizeof(unsigned short);

#ifdef OGRE_SKELETON_SERIALIZER_OLD_VERSION
        // Nested keyframes
        for (unsigned short i = 0; i < pTrack->getNumKeyFrames(); ++i)
        {
            size += calcKeyFrameSize(pSkel, pTrack->getNodeKeyFrame(i));
        }
#else
        size += calcKeyFrameListSize(pSkel, pTrack, calcKeyFrameListFlags(pTrack));
#endif

        return size;
    }
    //---------------------------------------------------------------------
    size_t SkeletonSerializer::calcKeyFrameSize(const Skeleton* pSkel, 
        const TransformKeyFrame* pKey)
    {
        size_t size = STREAM_OVERHEAD_SIZE;

        // float time                    : The time position (seconds)
        size += sizeof(float);
        // Quaternion rotate            : Rotation to apply at this keyframe
        size += sizeof(float) * 4;
        // Vector3 translate            : Translation to apply at this keyframe
        size += sizeof(float) * 3;
        // Vector3 scale                : Scale to apply at this keyframe
        if (pKey->getScale() != Vector3::UNIT_SCALE)
        {
            size += sizeof(float) * 3;
        }

        return size;
    }
    //---------------------------------------------------------------------
    size_t SkeletonSerializer::calcKeyFrameSizeWithoutScale(const Skeleton* pSkel, 
        const TransformKeyFrame* pKey)
    {
        size_t size = STREAM_OVERHEAD_SIZE;

        // float time                    : The time position (seconds)
        size += sizeof(float);
        // Quaternion rotate            : Rotation to apply at this keyframe
        size += sizeof(float) * 4;
        // Vector3 translate            : Translation to apply at this keyframe
        size += sizeof(float) * 3;

        return size;
    }
    //---------------------------------------------------------------------
    void SkeletonSerializer::readBone(DataStreamPtr& stream, Skeleton* pSkel)
    {
        // char* name
        String name = readString(stream);
        // unsigned short handle            : handle of the bone, should be contiguous & start at 0
        unsigned short handle;
        readShorts(stream, &handle, 1);

        // Create new bone
        Bone* pBone = pSkel->createBone(name, handle);

        float tmp[7];
        readFloats(stream, tmp, 7);

        // Vector3 position                 : position of this bone relative to parent 
        pBone->setPosition(Vector3(tmp[0], tmp[1], tmp[2]));
        // Quaternion orientation           : orientation of this bone relative to parent 
        pBone->setOrientation(Quaternion(tmp[6], tmp[3], tmp[4], tmp[5]));

        // Do we have scale?
        if (mCurrentstreamLen > calcBoneSizeWithoutScale(pSkel, pBone))
        {
            Vector3 scale;
            readObject(stream, scale);
            pBone->setScale(scale);
        }
    }
    //---------------------------------------------------------------------
    void SkeletonSerializer::readBoneParent(DataStreamPtr& stream, Skeleton* pSkel)
    {
        // All bones have been created by this point
        Bone *child, *parent;
        unsigned short tmp[2];

        // unsigned short handle            : child bone
        // unsigned short parentHandle      : parent bone
        readShorts(stream, tmp, 2);

        // Find bones
        parent = pSkel->getBone(tmp[1]);
        child = pSkel->getBone(tmp[0]);

        // attach
        parent->addChild(child);

    }
    //---------------------------------------------------------------------
    void SkeletonSerializer::readAnimation(DataStreamPtr& stream, Skeleton* pSkel)
    {
        // char* name                       : Name of the animation
        String name;
        name = readString(stream);
        // float length                      : Length of the animation in seconds
        float len;
        readFloats(stream, &len, 1);

        Animation *pAnim = pSkel->createAnimation(name, len);

        // Read all tracks
        if (!stream->eof())
        {
            unsigned short streamID = readChunk(stream);
            while(streamID == SKELETON_ANIMATION_TRACK && !stream->eof())
            {
                readAnimationTrack(stream, pAnim, pSkel);

                if (!stream->eof())
                {
                    // Get next stream
                    streamID = readChunk(stream);
                }
            }
            if (!stream->eof())
            {
                // Backpedal back to start of this stream if we've found a non-track
                stream->skip(-STREAM_OVERHEAD_SIZE);
            }

        }



    }
    //---------------------------------------------------------------------
    void SkeletonSerializer::readAnimationTrack(DataStreamPtr& stream, Animation* anim, 
        Skeleton* pSkel)
    {
        // unsigned short boneIndex     : Index of bone to apply to
        unsigned short boneHandle;
        readShorts(stream, &boneHandle, 1);

        // Find bone
        Bone *targetBone = pSkel->getBone(boneHandle);

        // Create track
        NodeAnimationTrack* pTrack = anim->createNodeTrack(boneHandle, targetBone);

        // Keep looking for nested keyframes
        if (!stream->eof())
        {
            unsigned short streamID = readChunk(stream);
            while (!stream->eof())
            {
                if (streamID == SKELETON_ANIMATION_TRACK_KEYFRAME)
                    readKeyFrame(stream, pTrack, pSkel);
                else if (streamID == SKELETON_ANIMATION_TRACK_KEYFRAME_LIST)
                    readKeyFrameList(stream, pTrack, pSkel);
                else
                    break;

                if (!stream->eof())
                {
                    // Get next stream
                    streamID = readChunk(stream);
                }
            }
            if (!stream->eof())
            {
                // Backpedal back to start of this stream if we've found a non-keyframe
                stream->skip(-STREAM_OVERHEAD_SIZE);
            }

            // Sort the keyframes
            pTrack->_sortKeyFrames();

        }


    }
    //---------------------------------------------------------------------
    void SkeletonSerializer::readKeyFrame(DataStreamPtr& stream, NodeAnimationTrack* track, 
        Skeleton* pSkel)
    {
        float tmp[8];
        readFloats(stream, tmp, 8);

        // float time                   : The time position (seconds)
        TransformKeyFrame *kf = track->_createNodeKeyFrameUnsorted(tmp[0]);

        // Quaternion rotate            : Rotation to apply at this keyframe
        kf->setRotation(Quaternion(tmp[4], tmp[1], tmp[2], tmp[3]));
        // Vector3 translate            : Translation to apply at this keyframe
        kf->setTranslate(Vector3(tmp[5], tmp[6], tmp[7]));

        // Do we have scale?
        if (mCurrentstreamLen > calcKeyFrameSizeWithoutScale(pSkel, kf))
        {
            Vector3 scale;
            readObject(stream, scale);
            kf->setScale(scale);
        }
    }
    //---------------------------------------------------------------------
#define KF_ROTATION     (1<<0)
#define KF_TRANSLATE    (1<<1)
#define KF_SCALE        (1<<2)
#define KF_ALL          (KF_ROTATION | KF_TRANSLATE | KF_SCALE)
    //---------------------------------------------------------------------
    void SkeletonSerializer::writeKeyFrameList(const Skeleton* pSkel, const NodeAnimationTrack* track)
    {
        unsigned short keyFrameFlags = calcKeyFrameListFlags(track);

        writeChunkHeader(SKELETON_ANIMATION_TRACK_KEYFRAME_LIST, 
            calcKeyFrameListSize(pSkel, track, keyFrameFlags));

        // unsigned short numKeyFrames
        unsigned short numKeyFrames = track->getNumKeyFrames();
        writeShorts(&numKeyFrames, 1);

        // unsigned short keyFrameFlags
        writeShorts(&keyFrameFlags, 1);

        // Write all keyframes
        for (unsigned short i = 0; i < numKeyFrames; ++i)
        {
            const TransformKeyFrame* kf = track->getNodeKeyFrame(i);

            // float time                   : The time position (seconds)
            float time = kf->getTime();
            writeFloats(&time, 1);
            // Quaternion rotate            : Rotation to apply at this keyframe    - exists only if keyFrameFlags bit 0 is non-zero
            if (keyFrameFlags & KF_ROTATION)
                writeObject(kf->getRotation());
            // Vector3 translate            : Translation to apply at this keyframe - exists only if keyFrameFlags bit 1 is non-zero
            if (keyFrameFlags & KF_TRANSLATE)
                writeObject(kf->getTranslate());
            // Vector3 scale                : Scale to apply at this keyframe       - exists only if keyFrameFlags bit 2 is non-zero
            if (keyFrameFlags & KF_SCALE)
                writeObject(kf->getScale());
        }
    }
    //---------------------------------------------------------------------
    unsigned short SkeletonSerializer::calcKeyFrameListFlags(const NodeAnimationTrack* track)
    {
        unsigned short keyFrameFlags = 0;

        unsigned short numKeyFrames = track->getNumKeyFrames();
        for (unsigned short i = 0; i < numKeyFrames && keyFrameFlags != KF_ALL; ++i)
        {
            const TransformKeyFrame* kf = track->getNodeKeyFrame(i);
            if (!(keyFrameFlags & KF_ROTATION) && kf->getRotation() != Quaternion::IDENTITY)
                keyFrameFlags |= KF_ROTATION;
            if (!(keyFrameFlags & KF_TRANSLATE) && kf->getTranslate() != Vector3::ZERO)
                keyFrameFlags |= KF_TRANSLATE;
            if (!(keyFrameFlags & KF_SCALE) && kf->getScale() != Vector3::UNIT_SCALE)
                keyFrameFlags |= KF_SCALE;
        }

        return keyFrameFlags;
    }
    //---------------------------------------------------------------------
    size_t SkeletonSerializer::calcKeyFrameListSize(const Skeleton* pSkel,
        const NodeAnimationTrack* track, unsigned short keyFrameFlags)
    {
		size_t size = STREAM_OVERHEAD_SIZE;

        // unsigned short numKeyFrames
		size += sizeof(unsigned short);

        // KeyFrame* keyFrameList
        size += track->getNumKeyFrames() * (
            + sizeof(float) * 1                                         // float time
            + ((keyFrameFlags & KF_ROTATION ) ? sizeof(float) * 4 : 0)  // Quaternion rotate
            + ((keyFrameFlags & KF_TRANSLATE) ? sizeof(float) * 3 : 0)  // Vector3 translate
            + ((keyFrameFlags & KF_SCALE    ) ? sizeof(float) * 3 : 0)  // Vector3 scale
            );

		return size;
    }
    //---------------------------------------------------------------------
    void SkeletonSerializer::readKeyFrameList(DataStreamPtr& stream, NodeAnimationTrack* track, 
        Skeleton* pSkel)
    {
        // unsigned short numKeyFrames
        // unsigned short keyFrameFlags
        unsigned short tmp[2];
        readShorts(stream, tmp, 2);
        unsigned short numKeyFrames = tmp[0];
        unsigned short keyFrameFlags = tmp[1];

        // Per-allocate memory for keyframes
        track->_estimateNumKeyFrames(numKeyFrames + track->getNumKeyFrames());

        // KeyFrame* keyFrameList
        static std::vector<float> buffer;
        size_t numFloats = numKeyFrames * (
            + 1                                         // float time
            + ((keyFrameFlags & KF_ROTATION ) ? 4 : 0)  // Quaternion rotate
            + ((keyFrameFlags & KF_TRANSLATE) ? 3 : 0)  // Vector3 translate
            + ((keyFrameFlags & KF_SCALE    ) ? 3 : 0)  // Vector3 scale
            );
        buffer.resize(numFloats);
        readFloats(stream, &buffer.front(), numFloats);

        const float* data = &buffer.front();
        unsigned short i;
        TransformKeyFrame *kf;
        switch (keyFrameFlags & KF_ALL)
        {
        default:
        case 0:
            for (i = 0; i < numKeyFrames; ++i)
            {
                // float time                   : The time position (seconds)
                kf = track->_createNodeKeyFrameUnsorted(data[i * 1 + 0]);
            }
            break;

        case KF_ROTATION:
            for (i = 0; i < numKeyFrames; ++i)
            {
                // float time                   : The time position (seconds)
                kf = track->_createNodeKeyFrameUnsorted(data[i * 5 + 0]);

                // Quaternion rotate            : Rotation to apply at this keyframe
                kf->setRotation(Quaternion(data[i * 5 + 4], data[i * 5 + 1], data[i * 5 + 2], data[i * 5 + 3]));
            }
            break;

        case KF_TRANSLATE:
            for (i = 0; i < numKeyFrames; ++i)
            {
                // float time                   : The time position (seconds)
                kf = track->_createNodeKeyFrameUnsorted(data[i * 4 + 0]);

                // Vector3 translate            : Translation to apply at this keyframe
                kf->setTranslate(Vector3(data[i * 4 + 1], data[i * 4 + 2], data[i * 4 + 3]));
            }
            break;

        case KF_SCALE:
            for (i = 0; i < numKeyFrames; ++i)
            {
                // float time                   : The time position (seconds)
                kf = track->_createNodeKeyFrameUnsorted(data[i * 4 + 0]);

                // Vector3 scale                : Scale to apply at this keyframe
                kf->setScale(Vector3(data[i * 4 + 1], data[i * 4 + 2], data[i * 4 + 3]));
            }
            break;

        case KF_ROTATION | KF_TRANSLATE:
            for (i = 0; i < numKeyFrames; ++i)
            {
                // float time                   : The time position (seconds)
                kf = track->_createNodeKeyFrameUnsorted(data[i * 8 + 0]);

                // Quaternion rotate            : Rotation to apply at this keyframe
                kf->setRotation(Quaternion(data[i * 8 + 4], data[i * 8 + 1], data[i * 8 + 2], data[i * 8 + 3]));
                // Vector3 translate            : Translation to apply at this keyframe
                kf->setTranslate(Vector3(data[i * 8 + 5], data[i * 8 + 6], data[i * 8 + 7]));
            }
            break;

        case KF_ROTATION | KF_SCALE:
            for (i = 0; i < numKeyFrames; ++i)
            {
                // float time                   : The time position (seconds)
                kf = track->_createNodeKeyFrameUnsorted(data[i * 8 + 0]);

                // Quaternion rotate            : Rotation to apply at this keyframe
                kf->setRotation(Quaternion(data[i * 8 + 4], data[i * 8 + 1], data[i * 8 + 2], data[i * 8 + 3]));
                // Vector3 scale                : Scale to apply at this keyframe
                kf->setScale(Vector3(data[i * 8 + 5], data[i * 8 + 6], data[i * 8 + 7]));
            }
            break;

        case KF_TRANSLATE | KF_SCALE:
            for (i = 0; i < numKeyFrames; ++i)
            {
                // float time                   : The time position (seconds)
                kf = track->_createNodeKeyFrameUnsorted(data[i * 7 + 0]);

                // Vector3 translate            : Translation to apply at this keyframe
                kf->setTranslate(Vector3(data[i * 7 + 1], data[i * 7 + 2], data[i * 7 + 3]));
                // Vector3 scale                : Scale to apply at this keyframe
                kf->setScale(Vector3(data[i * 7 + 4], data[i * 7 + 5], data[i * 7 + 6]));
            }
            break;

        case KF_ROTATION | KF_TRANSLATE | KF_SCALE:
            for (i = 0; i < numKeyFrames; ++i)
            {
                // float time                   : The time position (seconds)
                kf = track->_createNodeKeyFrameUnsorted(data[i * 11 + 0]);

                // Quaternion rotate            : Rotation to apply at this keyframe
                kf->setRotation(Quaternion(data[i * 11 + 4], data[i * 11 + 1], data[i * 11 + 2], data[i * 11 + 3]));
                // Vector3 translate            : Translation to apply at this keyframe
                kf->setTranslate(Vector3(data[i * 11 + 5], data[i * 11 + 6], data[i * 11 + 7]));
                // Vector3 scale                : Scale to apply at this keyframe
                kf->setScale(Vector3(data[i * 11 + 8], data[i * 11 + 9], data[i * 11 + 10]));
            }
            break;
        }
    }
	//---------------------------------------------------------------------
	void SkeletonSerializer::writeSkeletonAnimationLink(const Skeleton* pSkel, 
		const LinkedSkeletonAnimationSource& link)
	{
		writeChunkHeader(SKELETON_ANIMATION_LINK, 
			calcSkeletonAnimationLinkSize(pSkel, link));

		// char* skeletonName
		writeString(link.skeletonName);
		// float scale
		writeFloats(&(link.scale), 1);

	}
    //---------------------------------------------------------------------
	size_t SkeletonSerializer::calcSkeletonAnimationLinkSize(const Skeleton* pSkel, 
		const LinkedSkeletonAnimationSource& link)
	{
		size_t size = STREAM_OVERHEAD_SIZE;

		// char* skeletonName
		size += link.skeletonName.length() + 1;
		// float scale
		size += sizeof(float);

		return size;

	}
	//---------------------------------------------------------------------
	void SkeletonSerializer::readSkeletonAnimationLink(DataStreamPtr& stream, 
		Skeleton* pSkel)
	{
		// char* skeletonName
		String skelName = readString(stream);
		// float scale
		float scale;
		readFloats(stream, &scale, 1);

		pSkel->addLinkedSkeletonAnimationSource(skelName, scale);

	}



}


