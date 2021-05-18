#include "WXTerrainProjector.h"
#include "Terrain.h"
#include "WXUtils.h"

#include <OgreMaterialManager.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>

#if OGRE_COMPILER == OGRE_COMPILER_MSVC
// disable deprecation warnings while compiling deprecated class/function
#pragma warning(disable : 4996)
#endif

namespace WX {

#define VERTEX_SOURCE           0

    class TerrainProjector::PatchRenderable : public Ogre::Renderable
    {
    protected:
        TerrainProjector* mParent;

    public:
        Ogre::RenderOperation mRenderOp;
        Ogre::HardwareVertexBufferSharedPtr mVertexBuffer;

    public:
        PatchRenderable::PatchRenderable(TerrainProjector* parent)
            : Renderable()
            , mParent(parent)
            , mRenderOp()
            , mVertexBuffer()
        {
            mRenderOp.vertexData = new Ogre::VertexData;
            mRenderOp.useIndexes = false;

            Ogre::VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;

            size_t offset;

            // position and normal
            offset = 0;
            decl->addElement(VERTEX_SOURCE, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
            offset += 3 * sizeof(float);
            decl->addElement(VERTEX_SOURCE, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
            offset += 3 * sizeof(float);
        }
        //-----------------------------------------------------------------------
        PatchRenderable::~PatchRenderable()
        {
            delete mRenderOp.vertexData;
            delete mRenderOp.indexData;
        }
        //-----------------------------------------------------------------------
        /** Overridden - see Renderable. */
        const Ogre::MaterialPtr& getMaterial(void) const
        {
            return mParent->mPatchMaterial;
        }
        //-----------------------------------------------------------------------
        /** Overridden - see Renderable. */
        void getRenderOperation(Ogre::RenderOperation& op)
        {
            op = mRenderOp;
        }
        //-----------------------------------------------------------------------
        /** Overridden - see Renderable. */
        void getWorldTransforms(Ogre::Matrix4* xform) const
        {
            *xform = Ogre::Matrix4::IDENTITY;
        }
        //-----------------------------------------------------------------------
        /** Overridden - see Renderable. */
        const Ogre::Quaternion& getWorldOrientation(void) const
        {
            return Ogre::Quaternion::IDENTITY;
        }
        //-----------------------------------------------------------------------
        /** Overridden - see Renderable. */
        const Ogre::Vector3& getWorldPosition(void) const
        {
            return Ogre::Vector3::ZERO;
        }
        //-----------------------------------------------------------------------
        /** Overridden - see Renderable. */
        Ogre::Real getSquaredViewDepth(const Ogre::Camera* camera) const
        {
            Ogre::Node* n = mParent->getParentNode();
            assert(n);
            return n->getSquaredViewDepth(camera);
        }
        //-----------------------------------------------------------------------
        /** Overridden - see Renderable. */
        const Ogre::LightList& getLights(void) const
        {
            return mParent->queryLights();
        }
    };

    //////////////////////////////////////////////////////////////////////////

    const String TerrainProjector::msMovableType = "TerrainProjector";
    uint TerrainProjector::ms_uGenNameCount = 0;

    //-----------------------------------------------------------------------
    TerrainProjector::TerrainProjector(Terrain* terrain)
        : Ogre::Frustum()
        , mTerrain(terrain)
        , mPatchAreaLimit(256)
        , mPatchRenderable()
        , mPatchMaterial()
        , mPatchMaterialName()
        , mPatchMaterialDirty()
        , mShowFrustum()
    {
        assert(terrain);

        mPatchRenderable = new PatchRenderable(this);

        // Overriden Frustum default
        mVisible = true;

        // Disable cast shadows by default
        mCastShadows = false;

        // Generate name
        Ogre::StringUtil::StrStreamType str;
        str << msMovableType << ms_uGenNameCount++;
        mName = str.str();
    }
    //-----------------------------------------------------------------------
    TerrainProjector::~TerrainProjector()
    {
        if (mPatchRenderable)
        {
            if (!mPatchMaterial.isNull())
            {
                Ogre::MaterialManager::getSingleton().remove(mPatchMaterial->getHandle());
                mPatchMaterial.setNull();
            }

            delete mPatchRenderable;
        }
    }
    //-----------------------------------------------------------------------
    void TerrainProjector::setShowFrustum(bool showFrustum)
    {
        mShowFrustum = showFrustum;
    }
    //-----------------------------------------------------------------------
    bool TerrainProjector::getShowFrustum(void) const
    {
        return mShowFrustum;
    }
    //-----------------------------------------------------------------------
    void TerrainProjector::setPatchAreaLimit(int limit)
    {
        assert(limit > 0);
        mPatchAreaLimit = limit;
    }
    //-----------------------------------------------------------------------
    int TerrainProjector::getPatchAreaLimit(void) const
    {
        return mPatchAreaLimit;
    }
    //-----------------------------------------------------------------------
    void TerrainProjector::setPatchMaterial(const String& materialName)
    {
        mPatchMaterialName = materialName;
        mPatchMaterialDirty = true;
    }
    //-----------------------------------------------------------------------
    const String& TerrainProjector::getPatchMaterialName(void)
    {
        return mPatchMaterialName;
    }
    //-----------------------------------------------------------------------
    const String& TerrainProjector::getMovableType(void) const
    {
        return msMovableType;
    }
    //-----------------------------------------------------------------------
    void TerrainProjector::_notifyCurrentCamera(Ogre::Camera* camera)
    {
        // Nothing to do now
        Frustum::_notifyCurrentCamera(camera);
    }
    //-----------------------------------------------------------------------
    bool TerrainProjector::_updateMaterial(void)
    {
        if (mPatchMaterialDirty)
        {
            if (!mPatchMaterial.isNull())
            {
                Ogre::MaterialManager::getSingleton().remove(mPatchMaterial->getHandle());
                mPatchMaterial.setNull();
            }

            if (!mPatchMaterialName.empty())
            {
                mPatchMaterial = createProjectiveMaterial(mPatchMaterialName, this);
            }

            mPatchMaterialDirty = false;
        }

        return !mPatchMaterial.isNull();
    }
    //-----------------------------------------------------------------------
    bool TerrainProjector::_updateRenderable(void)
    {
        if (!mPatchRenderable || !_updateMaterial())
            return false;

        std::vector<Ogre::Vector3> positions;
        std::vector<Ogre::Vector3> normals;
        TerrainData* data = mTerrain->getData();
        assert(data);

        size_t numVertices = data->getIntersectsTriangles(this, mPatchAreaLimit, true, positions, &normals);
        if (numVertices <= 0)
            return false;

        // Check vertex buffer fit
        if (mPatchRenderable->mVertexBuffer.isNull() ||
            mPatchRenderable->mVertexBuffer->getNumVertices() < numVertices)
        {
            // Release vertex buffer first
            mPatchRenderable->mRenderOp.vertexData->vertexBufferBinding->unsetAllBindings();
            mPatchRenderable->mVertexBuffer.setNull();
            // Allocate vertex buffer
            mPatchRenderable->mVertexBuffer =
			    Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
				    6 * sizeof(float),
				    numVertices,
				    Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE);
            mPatchRenderable->mRenderOp.vertexData->vertexBufferBinding->setBinding(VERTEX_SOURCE, mPatchRenderable->mVertexBuffer);
        }

        // Fill up vertex buffer
        float* pFloat = static_cast<float*>(mPatchRenderable->mVertexBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD));
        std::vector<Ogre::Vector3>::const_iterator itPosition = positions.begin();
        std::vector<Ogre::Vector3>::const_iterator itNormal = normals.begin();
        for (size_t i = 0; i < numVertices; ++i, ++itPosition, ++itNormal)
        {
            *pFloat++ = itPosition->x;
            *pFloat++ = itPosition->y;
            *pFloat++ = itPosition->z;
            *pFloat++ = itNormal->x;
            *pFloat++ = itNormal->y;
            *pFloat++ = itNormal->z;
        }
        mPatchRenderable->mVertexBuffer->unlock();
        mPatchRenderable->mRenderOp.vertexData->vertexCount = numVertices;

        return true;
    }
    //-----------------------------------------------------------------------
    void TerrainProjector::_updateRenderQueue(Ogre::RenderQueue* queue)
    {
        if (mShowFrustum)
        {
            if (mRenderQueueIDSet)
                queue->addRenderable(this, mRenderQueueID);
            else
                queue->addRenderable(this);
        }

        if (_updateRenderable())
        {
            Ogre::uint8 groupId;
            if (mRenderQueueIDSet)
            {
                groupId = mRenderQueueID;
            }
            else
            {
                // Default to the render queue that the world geometry associated with.
                groupId = getParentSceneNode()->getCreator()->getWorldGeometryRenderQueue();
            }
            // Rendered later ensure the projective texture would be override
            // the basic geometry even if depth check is enabled.
            queue->addRenderable(mPatchRenderable, groupId, queue->getDefaultRenderablePriority()+10);
        }
    }

}
