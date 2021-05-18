/********************************************************************
    filename:   WXCollisionModel.cpp
    
    purpose:    Managed to store opcode model
*********************************************************************/

#include "WXCollisionModel.h"
#include "WXMeshInformer.h"
#include "WXOpcode.h"

#include <OgreException.h>

namespace WX {

    class CollisionModel::Imp
    {
        friend class CollisionModel;

        Opcode::Model mModel;
        Opcode::MeshInterface mMeshInterface;
    };

    CollisionModel::CollisionModel(void)
        : mImp()
        , mVertices()
        , mFaceNormals()
        , mIndices()
        , mIndexOffsets()
        , mMemoryUsage()
    {
    }

    CollisionModel::~CollisionModel()
    {
        delete mImp;
    }

    const Opcode::Model& CollisionModel::getOpcodeModel(void) const
    {
        assert(mImp);
        return mImp->mModel;
    }

    void
    CollisionModel::addMesh(const Ogre::MeshPtr& mesh)
    {
        MeshInformer::getMeshTriangles(mesh, mVertices, mIndices, &mIndexOffsets);
    }

    void
    CollisionModel::addEntity(const Ogre::Entity* entity)
    {
        MeshInformer::getEntityTriangles(entity, mVertices, mIndices, &mIndexOffsets);
    }

    void
    CollisionModel::reset(void)
    {
        destroy();

        mVertices.swap(std::vector<Ogre::Vector3>());
        mFaceNormals.swap(std::vector<Ogre::Vector4>());
        mIndices.swap(std::vector<size_t>());
        mIndexOffsets.swap(std::vector<size_t>());

        mMemoryUsage = 0;
    }

    void
    CollisionModel::build(bool dynamic)
    {
        destroy();

        mImp = new Imp;

        mImp->mMeshInterface.SetNbVertices(mVertices.size());
        mImp->mMeshInterface.SetNbTriangles(mIndices.size() / 3);
        mImp->mMeshInterface.SetPointers(
            reinterpret_cast<const IceMaths::IndexedTriangle*>(&mIndices[0]),
            reinterpret_cast<const IceMaths::Point*>(&mVertices[0]));

        // Make sure the mesh interface is valid
        if(!mImp->mMeshInterface.IsValid())
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                "Opcode MeshInterface is invalid.",
                "CollisionModel::build");
        }

        // Setup the Opcode model settings
        Opcode::OPCODECREATE buildInfo;
        buildInfo.mIMesh            = &mImp->mMeshInterface;
        buildInfo.mSettings.mLimit  = 1;
        buildInfo.mSettings.mRules  = Opcode::SPLIT_BALANCED | Opcode::SPLIT_GEOM_CENTER;
        buildInfo.mNoLeaf           = true;
        buildInfo.mQuantized        = dynamic ? false : true;
#ifdef __MESHMERIZER_H__
        buildInfo.mCollisionHull    = true;
#endif // __MESHMERIZER_H__
        buildInfo.mKeepOriginal     = false;
        buildInfo.mCanRemap         = false;

        // Create the Opcode model
        if (!mImp->mModel.Build(buildInfo))
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                "Can not build Opcode Model.",
                "CollisionModel::build");
        }

        // Compute triangle face normals
        size_t numTriangles = mIndices.size() / 3;
        mFaceNormals.reserve(numTriangles);
        for (size_t i = 0; i < numTriangles; ++i)
        {
            const Ogre::Vector3& v0 = mVertices[mIndices[i*3 + 0]];
            const Ogre::Vector3& v1 = mVertices[mIndices[i*3 + 1]];
            const Ogre::Vector3& v2 = mVertices[mIndices[i*3 + 2]];

            mFaceNormals.push_back(
                Ogre::Math::calculateFaceNormalWithoutNormalize(v0, v1, v2));
        }

        // Calculate used memory
        mMemoryUsage = mImp->mModel.GetUsedBytes() +
            mVertices.size() * sizeof(mVertices[0]) +
            mFaceNormals.size() * sizeof(mFaceNormals[0]) +
            mIndices.size() * sizeof(mIndices[0]) +
            mIndexOffsets.size() * sizeof(mIndexOffsets[0]);
    }

    void
    CollisionModel::destroy(void)
    {
        delete mImp;
        mImp = 0;
    }

}
