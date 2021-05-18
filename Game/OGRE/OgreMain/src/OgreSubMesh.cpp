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
#include "OgreSubMesh.h"

#include "OgreMesh.h"
#include "OgreException.h"
#include "OgreMeshManager.h"
#include "OgreMaterialManager.h"
#include "OgreStringConverter.h"

namespace Ogre {
    //-----------------------------------------------------------------------
    SubMesh::SubMesh()
        : useSharedVertices(true)
        , operationType(RenderOperation::OT_TRIANGLE_LIST)
        , vertexData(0)
        , mMatInitialised(false)
        , mBoneAssignmentsOutOfDate(false)
		, mVertexAnimationType(VAT_NONE)
    {
		indexData = new IndexData();
    }
    //-----------------------------------------------------------------------
    SubMesh::~SubMesh()
    {
        delete vertexData;
		delete indexData;

		removeLodLevels();
    }

    //-----------------------------------------------------------------------
    void SubMesh::setMaterialName(const String& name)
    {
        mMaterialName = name;
        mMatInitialised = true;
    }
    //-----------------------------------------------------------------------
    const String& SubMesh::getMaterialName() const
    {
        return mMaterialName;
    }
    //-----------------------------------------------------------------------
    bool SubMesh::isMatInitialised(void) const
    {
        return mMatInitialised;

    }
    //-----------------------------------------------------------------------
    void SubMesh::_getRenderOperation(RenderOperation& ro, ushort lodIndex)
    {

		// SubMeshes always use indexes
        ro.useIndexes = true;
		if (lodIndex > 0 && static_cast< size_t >( lodIndex - 1 ) < mLodFaceList.size())
		{
			// lodIndex - 1 because we don't store full detail version in mLodFaceList
			ro.indexData = mLodFaceList[lodIndex-1];
        }
        else
        {
    		ro.indexData = indexData;
        }
		ro.operationType = operationType;
		ro.vertexData = useSharedVertices? parent->sharedVertexData : vertexData;

    }
    //-----------------------------------------------------------------------
    void SubMesh::addBoneAssignment(const VertexBoneAssignment& vertBoneAssign)
    {
        if (useSharedVertices)
        {
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "This SubMesh uses shared geometry,  you "
                "must assign bones to the Mesh, not the SubMesh", "SubMesh.addBoneAssignment");
        }
        mBoneAssignments.insert(
            VertexBoneAssignmentList::value_type(vertBoneAssign.vertexIndex, vertBoneAssign));
        mBoneAssignmentsOutOfDate = true;
    }
    //-----------------------------------------------------------------------
    void SubMesh::clearBoneAssignments(void)
    {
        mBoneAssignments.clear();
        mBoneAssignmentsOutOfDate = true;
    }

    //-----------------------------------------------------------------------
    void SubMesh::_compileBoneAssignments(void)
    {
        unsigned short maxBones =
            parent->_rationaliseBoneAssignments(vertexData->vertexCount, mBoneAssignments);

        if (maxBones != 0)
        {
            parent->compileBoneAssignments(mBoneAssignments, maxBones, 
                blendIndexToBoneIndexMap, vertexData);
        }

        mBoneAssignmentsOutOfDate = false;
    }
    //---------------------------------------------------------------------
    SubMesh::BoneAssignmentIterator SubMesh::getBoneAssignmentIterator(void)
    {
        return BoneAssignmentIterator(mBoneAssignments.begin(),
            mBoneAssignments.end());
    }
    //---------------------------------------------------------------------
    SubMesh::AliasTextureIterator SubMesh::getAliasTextureIterator(void) const
    {
        return AliasTextureIterator(mTextureAliases.begin(),
            mTextureAliases.end());
    }
    //---------------------------------------------------------------------
    void SubMesh::addTextureAlias(const String& aliasName, const String& textureName)
    {
        mTextureAliases[aliasName] = textureName;
    }
    //---------------------------------------------------------------------
    void SubMesh::removeTextureAlias(const String& aliasName)
    {
        mTextureAliases.erase(aliasName);
    }
    //---------------------------------------------------------------------
    void SubMesh::removeAllTextureAliases(void)
    {
        mTextureAliases.clear();
    }
    //---------------------------------------------------------------------
    bool SubMesh::updateMaterialUsingTextureAliases(void)
    {
        bool newMaterialCreated = false;
        // if submesh has texture aliases
        // ask the material manager if the current summesh material exists
        if (hasTextureAliases() && MaterialManager::getSingleton().resourceExists(mMaterialName))
        {
            // get the current submesh material
            MaterialPtr material = MaterialManager::getSingleton().getByName( mMaterialName );
            // get test result for if change will occur when the texture aliases are applied
            if (material->applyTextureAliases(mTextureAliases, false))
            {
                // material textures will be changed so copy material,
                // new material name is old material name + index
                // check with material manager and find a unique name
                size_t index = 0;
                String newMaterialName = mMaterialName + "_" + StringConverter::toString(index);
                while (MaterialManager::getSingleton().resourceExists(newMaterialName))
                {
                    // increment index for next name
                    newMaterialName = mMaterialName + "_" + StringConverter::toString(++index);
                }

                Ogre::MaterialPtr newMaterial = Ogre::MaterialManager::getSingleton().create(
                    newMaterialName, material->getGroup());
                // copy parent material details to new material
                material->copyDetailsTo(newMaterial);
                // apply texture aliases to new material
                newMaterial->applyTextureAliases(mTextureAliases);
                // place new material name in submesh
                setMaterialName(newMaterialName);
                newMaterialCreated = true;
            }
        }

        return newMaterialCreated;
    }
    //---------------------------------------------------------------------
    void SubMesh::removeLodLevels(void)
    {
        ProgressiveMesh::LODFaceList::iterator lodi, lodend;
		lodend = mLodFaceList.end();
		for (lodi = mLodFaceList.begin(); lodi != lodend; ++lodi)
		{
			delete *lodi;
		}

        mLodFaceList.clear();

    }
	//---------------------------------------------------------------------
	VertexAnimationType SubMesh::getVertexAnimationType(void) const
	{
		if(parent->_getAnimationTypesDirty())
		{
			parent->_determineAnimationTypes();
		}
		return mVertexAnimationType;
	}


}

