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

#include "OgreTagPoint.h"
#include "OgreMatrix4.h"
#include "OgreMatrix3.h"
#include "OgreEntity.h"
#include "OgreSceneNode.h"
#include "OgreSkeleton.h"
#include "OgreQuaternion.h"


namespace Ogre {

    //-----------------------------------------------------------------------------
    TagPoint::TagPoint(unsigned short handle, Skeleton* creator)
        : Bone(handle, creator)
        , mParentEntity(0)
        , mChildObject(0)
    {
    }
    //-----------------------------------------------------------------------------
    TagPoint::~TagPoint()
    {
    }
    //-----------------------------------------------------------------------------
    Entity *TagPoint::getParentEntity(void) const
    {
        return mParentEntity;
    }
    //-----------------------------------------------------------------------------
    void TagPoint::setParentEntity(Entity *pEntity)
    {
        mParentEntity = pEntity;
    }
    //-----------------------------------------------------------------------------
    void TagPoint::setChildObject(MovableObject *pObject)
    {
        mChildObject = pObject;
    }
    //-----------------------------------------------------------------------------
    const Matrix4& TagPoint::_getFullLocalTransform(void) const
    {
        return mFullLocalTransform;
    }
    //-----------------------------------------------------------------------------
    const Matrix4& TagPoint::getParentEntityTransform(void) const
    {

        return mParentEntity->_getParentNodeFullTransform();
    }
    //-----------------------------------------------------------------------------
    void TagPoint::needUpdate(bool forceParentUpdate)
    {
		Bone::needUpdate(forceParentUpdate);

        // We need to tell parent entities node
        if (mParentEntity)
        {
            Node* n = mParentEntity->getParentNode();
            if (n)
            {
                n->needUpdate();
            }

        }

    }
    //-----------------------------------------------------------------------------
    void TagPoint::_updateFromParent(void) const
    {
        // Call superclass
        Bone::_updateFromParent();

        // Save transform for local skeleton
        mFullLocalTransform.makeTransform(
            mDerivedPosition,
            mDerivedScale,
            mDerivedOrientation);

        // Include Entity transform
        if (mParentEntity)
        {
            Node* entityParentNode = mParentEntity->getParentNode();
            if (entityParentNode)
            {
                // Note: orientation/scale inheritance already take care with
                // _updateFromParent, don't do that with parent entity transform.

                // Combine orientation with that of parent entity
                const Quaternion& parentOrientation = entityParentNode->_getDerivedOrientation();
                mDerivedOrientation = parentOrientation * mDerivedOrientation;

                // Incorporate parent entity scale
                const Vector3& parentScale = entityParentNode->_getDerivedScale();
                mDerivedScale *= parentScale;

                // Change position vector based on parent entity's orientation & scale
                mDerivedPosition = parentOrientation * (parentScale * mDerivedPosition);

                // Add altered position vector to parent entity
                mDerivedPosition += entityParentNode->_getDerivedPosition();
            }
        }


    }
    //-----------------------------------------------------------------------------
    const LightList& TagPoint::getLights(void) const
    {
        return mParentEntity->queryLights();
    }

}
