/********************************************************************
    filename:   WXMeshInformer.h
    
    purpose:    Rotines for collect mesh information
*********************************************************************/

#ifndef __FAIRYMESHINFORMER_H__
#define __FAIRYMESHINFORMER_H__

#include "WXPrerequisites.h"

namespace WX
{
    class MeshInformer
    {
    public:
        static size_t getVertices(Ogre::Vector3* pDst,
                                  const Ogre::VertexData* vertexData);

        template <typename DestType, typename SrcType>
        static size_t getTriangleIndices(DestType* pDest,
                                         const SrcType* pSrc, size_t count, size_t offset, int operationType);

        static size_t getTriangles(size_t* pDst,
                                   const Ogre::IndexData* indexData, size_t offset, int operationType);

        static void getMeshStatistics(const Ogre::MeshPtr& mesh,
                                      size_t& numVertices, size_t& numIndices);

        static void getMeshTriangles(const Ogre::MeshPtr& mesh,
                                     std::vector<Ogre::Vector3>& vertices,
                                     std::vector<size_t>& indices,
                                     std::vector<size_t>* indexOffsets);

        static void getEntityStatistics(const Ogre::Entity* entity,
                                        size_t& numVertices, size_t& numIndices);

        static void getEntityTriangles(const Ogre::Entity* entity,
                                       std::vector<Ogre::Vector3>& vertices,
                                       std::vector<size_t>& indices,
                                       std::vector<size_t>* indexOffsets);
    };
}

#endif 
