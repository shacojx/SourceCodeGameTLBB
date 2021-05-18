/********************************************************************
    filename:   WXMeshInformer.cpp
    
    purpose:    Rotines for collect mesh information
*********************************************************************/

#include "WXMeshInformer.h"

#include <OgreMesh.h>
#include <OgreSubMesh.h>
#include <OgreEntity.h>
#include <OgreSubEntity.h>

namespace WX {
    using namespace Ogre;

    //-----------------------------------------------------------------------
    size_t
    MeshInformer::getVertices(Vector3* pDst,
                              const VertexData* vertexData)
    {
        const VertexElement* posElem = vertexData->vertexDeclaration->findElementBySemantic(VES_POSITION);
        HardwareVertexBufferSharedPtr vbuf = vertexData->vertexBufferBinding->getBuffer(posElem->getSource());
        size_t vertex_stride = vbuf->getVertexSize();
        size_t vertexCount = vertexData->vertexCount;

        void* pBase = vbuf->lock(
            vertex_stride * vertexData->vertexStart,
            vertex_stride * vertexCount,
            HardwareBuffer::HBL_READ_ONLY);

        uchar* pSrc; posElem->baseVertexPointerToElement(pBase, &pSrc);
        for (size_t i = 0; i < vertexCount; ++i, pSrc += vertex_stride, ++pDst)
        {
            const float* pFloat = reinterpret_cast<const float*>(pSrc);
            pDst->x = pFloat[0];
            pDst->y = pFloat[1];
            pDst->z = pFloat[2];
        }

        vbuf->unlock();

        return vertexCount;
    }
    //-----------------------------------------------------------------------
    template <typename DestType, typename SrcType>
    size_t
    MeshInformer::getTriangleIndices(DestType* pDest,
                                     const SrcType* pSrc, size_t count, size_t offset, int operationType)
    {
        size_t i;

        assert(count >= 3);
        switch (operationType)
        {
        case RenderOperation::OT_TRIANGLE_LIST:
            assert(count % 3 == 0);
            for (i = 0; i < count; ++i)
            {
                *pDest++ = *pSrc++ + offset;
            }
            return count;

        case RenderOperation::OT_TRIANGLE_STRIP:
            for (i = 0; i < count - 3; i += 2)
            {
                pDest[0] = pSrc[0] + offset;
                pDest[4] = pDest[1] = pSrc[1] + offset;
                pDest[3] = pDest[2] = pSrc[2] + offset;
                pDest[5] = pSrc[3] + offset;
                pDest += 6;
                pSrc += 2;
            }
            if (count & 1)
            {
                pDest[0] = pSrc[0] + offset;
                pDest[1] = pSrc[1] + offset;
                pDest[2] = pSrc[2] + offset;
            }
            return (count - 2) * 3;

        case RenderOperation::OT_TRIANGLE_FAN:
            for (i = 0; i < count - 2; ++i)
            {
                *pDest++ = pSrc[0]     + offset;
                *pDest++ = pSrc[i + 1] + offset;
                *pDest++ = pSrc[i + 2] + offset;
            }
            return (count - 2) * 3;

        default:
            assert(false);
            return 0;
        }
    }
    //-----------------------------------------------------------------------
    size_t
    MeshInformer::getTriangles(size_t* pDst,
                               const IndexData* indexData, size_t offset, int operationType)
    {
        size_t index_stride = indexData->indexBuffer->getIndexSize();
        size_t index_count = indexData->indexCount;

        const void* pBase = indexData->indexBuffer->lock(
            index_stride * indexData->indexStart,
            index_stride * index_count,
            HardwareBuffer::HBL_READ_ONLY);

        size_t result;
        if (indexData->indexBuffer->getType() == HardwareIndexBuffer::IT_16BIT)
        {
            const uint16* pSrc = static_cast<const uint16*>(pBase);
            result = getTriangleIndices(pDst, pSrc, index_count, offset, operationType);
        }
        else
        {
            const uint32* pSrc = static_cast<const uint32*>(pBase);
            result = getTriangleIndices(pDst, pSrc, index_count, offset, operationType);
        }

        indexData->indexBuffer->unlock();

        return result;
    }
    //-----------------------------------------------------------------------
    void
    MeshInformer::getMeshStatistics(const MeshPtr& mesh, size_t& numVertices, size_t& numIndices)
    {
        size_t numSubMeshes = mesh->getNumSubMeshes();
        bool added_shared_vertex = false;

        for (size_t i = 0; i < numSubMeshes; ++i)
        {
            SubMesh* subMesh = mesh->getSubMesh(i);

            if (subMesh->operationType == RenderOperation::OT_TRIANGLE_LIST ||
                subMesh->operationType == RenderOperation::OT_TRIANGLE_STRIP ||
                subMesh->operationType == RenderOperation::OT_TRIANGLE_FAN)
            {
                if (subMesh->useSharedVertices)
                {
                    if (!added_shared_vertex)
                    {
                        added_shared_vertex = true;
                        numVertices += subMesh->parent->sharedVertexData->vertexCount;
                    }
                }
                else
                {
                    numVertices += subMesh->vertexData->vertexCount;
                }

                assert(subMesh->indexData->indexCount >= 3);
                switch (subMesh->operationType)
                {
                case RenderOperation::OT_TRIANGLE_LIST:
                    assert(subMesh->indexData->indexCount % 3 == 0);
                    numIndices += subMesh->indexData->indexCount;
                    break;

                case RenderOperation::OT_TRIANGLE_STRIP:
                case RenderOperation::OT_TRIANGLE_FAN:
                    numIndices += (subMesh->indexData->indexCount - 2) * 3;
                    break;
                }
            }
        }
    }
    //-----------------------------------------------------------------------
    void
    MeshInformer::getMeshTriangles(const MeshPtr& mesh,
                                   std::vector<Vector3>& vertices,
                                   std::vector<size_t>& indices,
                                   std::vector<size_t>* indexOffsets)
    {
        size_t numVertices = 0, numIndices = 0;
        getMeshStatistics(mesh, numVertices, numIndices);

        size_t vertex_offset = vertices.size();
        size_t index_offset = indices.size();

        vertices.resize(vertex_offset + numVertices);
        indices.resize(index_offset + numIndices);

        Vector3* pVertices = &vertices[0];
        size_t* pIndices = &indices[0];
        size_t shared_vertex_offset = vertex_offset;
        bool added_shared_vertex = false;

        size_t numSubMeshes = mesh->getNumSubMeshes();
        for (size_t i = 0; i < numSubMeshes; ++i)
        {
            SubMesh* subMesh = mesh->getSubMesh(i);

            if (indexOffsets)
            {
                indexOffsets->push_back(index_offset);
            }

            if (subMesh->operationType == RenderOperation::OT_TRIANGLE_LIST ||
                subMesh->operationType == RenderOperation::OT_TRIANGLE_STRIP ||
                subMesh->operationType == RenderOperation::OT_TRIANGLE_FAN)
            {
                size_t current_vertex_offset;
                if (subMesh->useSharedVertices)
                {
                    if (!added_shared_vertex)
                    {
                        size_t vertexCount = getVertices(pVertices + vertex_offset, subMesh->parent->sharedVertexData);
                        shared_vertex_offset = vertex_offset;
                        vertex_offset += vertexCount;
                        added_shared_vertex = true;
                    }

                    current_vertex_offset = shared_vertex_offset;
                }
                else
                {
                    size_t vertexCount = getVertices(pVertices + vertex_offset, subMesh->vertexData);
                    current_vertex_offset = vertex_offset;
                    vertex_offset += vertexCount;
                }

                size_t index_count = getTriangles(pIndices + index_offset, subMesh->indexData, current_vertex_offset, subMesh->operationType);
                index_offset += index_count;
            }
        }
    }
    //-----------------------------------------------------------------------
    void
    MeshInformer::getEntityStatistics(const Entity* entity, size_t& numVertices, size_t& numIndices)
    {
        size_t numSubEntities = entity->getNumSubEntities();
        bool added_shared_vertex = false;

        for (size_t i = 0; i < numSubEntities; ++i)
        {
            SubEntity* subEntity = entity->getSubEntity(i);
            if (!subEntity->isVisible())
            {
                // Skip non-visible sub-entity
                continue;
            }

            SubMesh* subMesh = subEntity->getSubMesh();

            if (subMesh->operationType == RenderOperation::OT_TRIANGLE_LIST ||
                subMesh->operationType == RenderOperation::OT_TRIANGLE_STRIP ||
                subMesh->operationType == RenderOperation::OT_TRIANGLE_FAN)
            {
                if (subMesh->useSharedVertices)
                {
                    if (!added_shared_vertex)
                    {
                        added_shared_vertex = true;
                        numVertices += subMesh->parent->sharedVertexData->vertexCount;
                    }
                }
                else
                {
                    numVertices += subMesh->vertexData->vertexCount;
                }

                assert(subMesh->indexData->indexCount >= 3);
                switch (subMesh->operationType)
                {
                case RenderOperation::OT_TRIANGLE_LIST:
                    assert(subMesh->indexData->indexCount % 3 == 0);
                    numIndices += subMesh->indexData->indexCount;
                    break;

                case RenderOperation::OT_TRIANGLE_STRIP:
                case RenderOperation::OT_TRIANGLE_FAN:
                    numIndices += (subMesh->indexData->indexCount - 2) * 3;
                    break;
                }
            }
        }
    }
    //-----------------------------------------------------------------------
    void
    MeshInformer::getEntityTriangles(const Entity* entity,
                                     std::vector<Vector3>& vertices,
                                     std::vector<size_t>& indices,
                                     std::vector<size_t>* indexOffsets)
    {
        size_t numVertices = 0, numIndices = 0;
        getEntityStatistics(entity, numVertices, numIndices);

        size_t vertex_offset = vertices.size();
        size_t index_offset = indices.size();

        vertices.resize(vertex_offset + numVertices);
        indices.resize(index_offset + numIndices);

        // Software blended vertex data available only if animation enabled, and software animation are
        // used by internally engine, or user requested software animation.
		bool useBlended = entity->_isAnimated() &&
            (!entity->isHardwareAnimationEnabled() || entity->getSoftwareAnimationRequests() > 0);

        // Use skinned vertex data only if blended data available and skeleton animation enabled.
        bool useSkinned = useBlended && entity->_isSkeletonAnimated();

        Vector3* pVertices = &vertices[0];
        size_t* pIndices = &indices[0];
        size_t shared_vertex_offset = vertex_offset;
        bool added_shared_vertex = false;

        size_t numSubEntities = entity->getNumSubEntities();
        for (size_t i = 0; i < numSubEntities; ++i)
        {
            SubEntity* subEntity = entity->getSubEntity(i);
            if (!subEntity->isVisible())
            {
                // Skip non-visible sub-entity
                continue;
            }

            SubMesh* subMesh = subEntity->getSubMesh();

            if (indexOffsets)
            {
                indexOffsets->push_back(index_offset);
            }

            if (subMesh->operationType == RenderOperation::OT_TRIANGLE_LIST ||
                subMesh->operationType == RenderOperation::OT_TRIANGLE_STRIP ||
                subMesh->operationType == RenderOperation::OT_TRIANGLE_FAN)
            {
                size_t current_vertex_offset;
                if (subMesh->useSharedVertices)
                {
                    if (!added_shared_vertex)
                    {
                        size_t vertexCount = getVertices(pVertices + vertex_offset,
                            useSkinned ? entity->_getSkelAnimVertexData()
                                :
                            useBlended && subMesh->parent->getSharedVertexDataAnimationType() != VAT_NONE ? entity->_getSoftwareVertexAnimVertexData()
                                :
                            subMesh->parent->sharedVertexData);
                        shared_vertex_offset = vertex_offset;
                        vertex_offset += vertexCount;
                        added_shared_vertex = true;
                    }

                    current_vertex_offset = shared_vertex_offset;
                }
                else
                {
                    size_t vertexCount = getVertices(pVertices + vertex_offset,
                        useSkinned ? subEntity->_getSkelAnimVertexData()
                            :
                        useBlended && subMesh->getVertexAnimationType() != VAT_NONE ? subEntity->_getSoftwareVertexAnimVertexData()
                            :
                        subMesh->vertexData);
                    current_vertex_offset = vertex_offset;
                    vertex_offset += vertexCount;
                }

                size_t index_count = getTriangles(pIndices + index_offset, subMesh->indexData, current_vertex_offset, subMesh->operationType);
                index_offset += index_count;
            }
        }
    }

}
