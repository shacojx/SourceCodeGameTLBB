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
#include "OgreLight.h"
#include "OgreEdgeListBuilder.h"
#include "OgreOptimisedUtil.h"

namespace Ogre {
    const LightList& ShadowRenderable::getLights(void) const 
    {
        // return empty
        static LightList ll;
        return ll;
    }
    // ------------------------------------------------------------------------
    void ShadowCaster::updateEdgeListLightFacing(EdgeData* edgeData, 
        const Vector4& lightPos)
    {
        edgeData->updateTriangleLightFacing(lightPos);
    }
    // ------------------------------------------------------------------------
    void ShadowCaster::generateShadowVolume(EdgeData* edgeData, 
        const HardwareIndexBufferSharedPtr& indexBuffer, const Light* light,
        ShadowRenderableList& shadowRenderables, unsigned long flags)
    {
        // Edge groups should be 1:1 with shadow renderables
        assert(edgeData->edgeGroups.size() == shadowRenderables.size());

        EdgeData::EdgeGroupList::const_iterator egi, egiend;
        ShadowRenderableList::const_iterator si;

        Light::LightTypes lightType = light->getType();

        // Lock index buffer for writing
        unsigned short* pIdx = static_cast<unsigned short*>(
            indexBuffer->lock(HardwareBuffer::HBL_DISCARD));
        size_t numIndices = 0;

        // Iterate over the groups and form renderables for each based on their
        // lightFacing
        si = shadowRenderables.begin();
        egiend = edgeData->edgeGroups.end();
        for (egi = edgeData->edgeGroups.begin(); egi != egiend; ++egi, ++si)
        {
            const EdgeData::EdgeGroup& eg = *egi;
            // Initialise the index start for this shadow renderable
            IndexData* indexData = (*si)->getRenderOperationForUpdate()->indexData;
            indexData->indexStart = numIndices;
            // original number of verts (without extruded copy)
            size_t originalVertexCount = eg.vertexData->vertexCount;
            bool  firstDarkCapTri = true;
            unsigned short darkCapStart;

            EdgeData::EdgeList::const_iterator i, iend;
            iend = eg.edges.end();
            for (i = eg.edges.begin(); i != iend; ++i)
            {
                const EdgeData::Edge& edge = *i;

                // Silhouette edge, when two tris has opposite light facing, or
                // degenerate edge where only tri 1 is valid and the tri light facing
                char lightFacing = edgeData->triangleLightFacings[edge.triIndex[0]];
                if ((edge.degenerate && lightFacing) ||
                    (!edge.degenerate && (lightFacing != edgeData->triangleLightFacings[edge.triIndex[1]])))
                {
                    size_t v0 = edge.vertIndex[0];
                    size_t v1 = edge.vertIndex[1];
                    if (!lightFacing)
                    {
                        // Inverse edge indexes when t1 is light away
                        std::swap(v0, v1);
                    }

                    /* Note edge(v0, v1) run anticlockwise along the edge from
                    the light facing tri so to point shadow volume tris outward,
                    light cap indexes have to be backwards

                    We emit 2 tris if light is a point light, 1 if light 
                    is directional, because directional lights cause all
                    points to converge to a single point at infinity.

                    First side tri = near1, near0, far0
                    Second tri = far0, far1, near1

                    'far' indexes are 'near' index + originalVertexCount
                    because 'far' verts are in the second half of the 
                    buffer
                    */
                    *pIdx++ = v1;
                    *pIdx++ = v0;
                    *pIdx++ = v0 + originalVertexCount;
                    numIndices += 3;

                    // Are we extruding to infinity?
                    if (!(lightType == Light::LT_DIRECTIONAL &&
                        flags & SRF_EXTRUDE_TO_INFINITY))
                    {
                        // additional tri to make quad
                        *pIdx++ = v0 + originalVertexCount;
                        *pIdx++ = v1 + originalVertexCount;
                        *pIdx++ = v1;
                        numIndices += 3;
                    }

                    // Do dark cap tri
                    // Use McGuire et al method, a triangle fan covering all silhouette
                    // edges and one point (taken from the initial tri)
                    if (flags & SRF_INCLUDE_DARK_CAP)
                    {
                        if (firstDarkCapTri)
                        {
                            darkCapStart = v0 + originalVertexCount;
                            firstDarkCapTri = false;
                        }
                        else
                        {
                            *pIdx++ = darkCapStart;
                            *pIdx++ = v1 + originalVertexCount;
                            *pIdx++ = v0 + originalVertexCount;
                            numIndices += 3;
                        }

                    }
                }

            }

            // Do light cap
            if (flags & SRF_INCLUDE_LIGHT_CAP) 
            {
                // separate light cap?
                if ((*si)->isLightCapSeparate())
                {
                    // update index count for this shadow renderable
                    indexData->indexCount = numIndices - indexData->indexStart;

                    // get light cap index data for update
                    indexData = (*si)->getLightCapRenderable()->getRenderOperationForUpdate()->indexData;
                    // start indexes after the current total
                    indexData->indexStart = numIndices;
                }

                // Iterate over the triangles which are using this vertex set
                EdgeData::TriangleList::const_iterator ti, tiend;
                EdgeData::TriangleLightFacingList::const_iterator lfi;
                ti = edgeData->triangles.begin() + eg.triStart;
                tiend = ti + eg.triCount;
                lfi = edgeData->triangleLightFacings.begin() + eg.triStart;
                for ( ; ti != tiend; ++ti, ++lfi)
                {
                    const EdgeData::Triangle& t = *ti;
                    assert(t.vertexSet == eg.vertexSet);
                    // Check it's light facing
                    if (*lfi)
                    {
                        *pIdx++ = t.vertIndex[0];
                        *pIdx++ = t.vertIndex[1];
                        *pIdx++ = t.vertIndex[2];
                        numIndices += 3;
                    }
                }

            }

            // update index count for current index data (either this shadow renderable or its light cap)
            indexData->indexCount = numIndices - indexData->indexStart;

        }


        // Unlock index buffer
        indexBuffer->unlock();

		// In debug mode, check we didn't overrun the index buffer
		assert(numIndices <= indexBuffer->getNumIndexes() &&
            "Index buffer overrun while generating shadow volume!! "
			"You must increase the size of the shadow index buffer.");

    }
    // ------------------------------------------------------------------------
    void ShadowCaster::extrudeVertices(
        const HardwareVertexBufferSharedPtr& vertexBuffer, 
        size_t originalVertexCount, const Vector4& light, Real extrudeDist)
    {
        assert (vertexBuffer->getVertexSize() == sizeof(float) * 3
            && "Position buffer should contain only positions!");

        // Extrude the first area of the buffer into the second area
        // Lock the entire buffer for writing, even though we'll only be
        // updating the latter because you can't have 2 locks on the same
        // buffer
        float* pSrc = static_cast<float*>(
            vertexBuffer->lock(HardwareBuffer::HBL_NORMAL));

        // TODO: We should add extra (ununsed) vertices ensure source and
        // destination buffer have same alignment for slight performance gain.
        float* pDest = pSrc + originalVertexCount * 3;

        OptimisedUtil::getImplementation()->extrudeVertices(
            light, extrudeDist,
            pSrc, pDest, originalVertexCount);

        vertexBuffer->unlock();

    }
    // ------------------------------------------------------------------------
    void ShadowCaster::extrudeBounds(AxisAlignedBox& box, const Vector4& light, Real extrudeDist) const
    {
        Vector3 extrusionDir;

        if (light.w == 0)
        {
            // Parallel projection guarantees min/max relationship remains the same
            extrusionDir.x = -light.x;
            extrusionDir.y = -light.y;
            extrusionDir.z = -light.z;
            extrusionDir.normalise();
            extrusionDir *= extrudeDist;
            box.setExtents(box.getMinimum() + extrusionDir, 
                box.getMaximum() + extrusionDir);
        }
        else
        {
            Vector3 vmin, vmax;
            Vector3 corner, tmp;

#define __EXTRUDE_POINT()                               \
            {                                           \
                extrusionDir.x = corner.x - light.x;    \
                extrusionDir.y = corner.y - light.y;    \
                extrusionDir.z = corner.z - light.z;    \
                extrusionDir.normalise();               \
                extrusionDir *= extrudeDist;            \
                tmp = corner + extrusionDir;            \
            }
            corner = box.getMinimum(); __EXTRUDE_POINT(); vmin = vmax = tmp;
            corner.x = box.getMaximum().x; __EXTRUDE_POINT(); vmin.makeFloor(tmp); vmax.makeCeil(tmp);
            corner.y = box.getMaximum().y; __EXTRUDE_POINT(); vmin.makeFloor(tmp); vmax.makeCeil(tmp);
            corner.x = box.getMinimum().x; __EXTRUDE_POINT(); vmin.makeFloor(tmp); vmax.makeCeil(tmp);
            corner.z = box.getMaximum().z; __EXTRUDE_POINT(); vmin.makeFloor(tmp); vmax.makeCeil(tmp);
            corner.x = box.getMaximum().x; __EXTRUDE_POINT(); vmin.makeFloor(tmp); vmax.makeCeil(tmp);
            corner.y = box.getMinimum().y; __EXTRUDE_POINT(); vmin.makeFloor(tmp); vmax.makeCeil(tmp);
            corner.x = box.getMinimum().x; __EXTRUDE_POINT(); vmin.makeFloor(tmp); vmax.makeCeil(tmp);
#undef __EXTRUDE_POINT


            box.setExtents(vmin, vmax);

        }

    }
    // ------------------------------------------------------------------------
    Real ShadowCaster::getExtrusionDistance(const Vector3& objectPos, const Light* light) const
    {
        Vector3 diff = objectPos - light->getDerivedPosition();
        return light->getAttenuationRange() - diff.length();
    }

}
