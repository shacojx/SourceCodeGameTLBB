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
#include "OgreManualObject.h"
#include "OgreException.h"
#include "OgreMaterialManager.h"
#include "OgreSceneNode.h"
#include "OgreRoot.h"
#include "OgreRenderSystem.h"
#include "OgreHardwareBufferManager.h"
#include "OgreEdgeListBuilder.h"
#include "OgreMeshManager.h"
#include "OgreMesh.h"
#include "OgreSubMesh.h"

namespace Ogre {

#define TEMP_INITIAL_SIZE 50
#define TEMP_VERTEXSIZE_GUESS sizeof(float) * 12
#define TEMP_INITIAL_VERTEX_SIZE TEMP_VERTEXSIZE_GUESS * TEMP_INITIAL_SIZE
#define TEMP_INITIAL_INDEX_SIZE sizeof(uint16) * TEMP_INITIAL_SIZE
	//-----------------------------------------------------------------------------
	ManualObject::ManualObject(const String& name)
		: MovableObject(name),
		  mCurrentSection(0), mFirstVertex(true),
		  mTempVertexPending(false),
		  mTempVertexBuffer(0), mTempVertexSize(TEMP_INITIAL_VERTEX_SIZE),
		  mTempIndexBuffer(0), mTempIndexSize(TEMP_INITIAL_INDEX_SIZE),
		  mDeclSize(0), mTexCoordIndex(0), mRadius(0), mAnyIndexed(false),
		  mEdgeList(0)
	{
	}
	//-----------------------------------------------------------------------------
	ManualObject::~ManualObject()
	{
		clear();
	}
	//-----------------------------------------------------------------------------
	void ManualObject::clear(void)
	{
		resetTempAreas();
		for (SectionList::iterator i = mSectionList.begin(); i != mSectionList.end(); ++i)
		{
			delete *i;
		}
		mSectionList.clear();
		mRadius = 0;
		mAABB.setNull();
		delete mEdgeList;
		mEdgeList = 0;
		mAnyIndexed = false;
		for (ShadowRenderableList::iterator s = mShadowRenderables.begin();
			s != mShadowRenderables.end(); ++s)
		{
			delete *s;
		}
		mShadowRenderables.clear();


	}
	//-----------------------------------------------------------------------------
	void ManualObject::resetTempAreas(void)
	{
		delete [] mTempVertexBuffer;
		delete [] mTempIndexBuffer;
		mTempVertexBuffer = 0;
		mTempIndexBuffer = 0;
		mTempVertexSize = TEMP_INITIAL_VERTEX_SIZE;
		mTempIndexSize = TEMP_INITIAL_INDEX_SIZE;
	}
	//-----------------------------------------------------------------------------
	void ManualObject::resizeTempVertexBufferIfNeeded(size_t numVerts)
	{
		// Calculate byte size
		// Use decl if we know it by now, otherwise default size to pos/norm/texcoord*2
		size_t newSize;
		if (!mFirstVertex)
		{
			newSize = mDeclSize * numVerts;
		}
		else
		{
			// estimate - size checks will deal for subsequent verts
			newSize = TEMP_VERTEXSIZE_GUESS * numVerts;
		}
		if (newSize > mTempVertexSize || !mTempVertexBuffer)
		{
			if (!mTempVertexBuffer)
			{
				// init
				newSize = mTempVertexSize;
			}
			else
			{
				// increase to at least double current
				newSize = std::max(newSize, mTempVertexSize*2);
			}
			// copy old data
			char* tmp = mTempVertexBuffer;
			mTempVertexBuffer = new char[newSize];
			if (tmp)
			{
				memcpy(mTempVertexBuffer, tmp, mTempVertexSize);
				// delete old buffer
				delete [] tmp;
			}
			mTempVertexSize = newSize;
		}
	}
	//-----------------------------------------------------------------------------
	void ManualObject::resizeTempIndexBufferIfNeeded(size_t numInds)
	{
		size_t newSize = numInds * sizeof(uint16);
		if (newSize > mTempIndexSize || !mTempIndexBuffer)
		{
			if (!mTempIndexBuffer)
			{
				// init
				newSize = mTempIndexSize;
			}
			else
			{
				// increase to at least double current
				newSize = std::max(newSize, mTempIndexSize*2);
			}
			numInds = newSize / sizeof(uint16);
			uint16* tmp = mTempIndexBuffer;
			mTempIndexBuffer = new uint16[numInds];
			if (tmp)
			{
				memcpy(mTempIndexBuffer, tmp, mTempIndexSize);
				delete [] tmp;
			}
			mTempIndexSize = newSize;
		}

	}
	//-----------------------------------------------------------------------------
	void ManualObject::estimateVertexCount(size_t vcount)
	{
		resizeTempVertexBufferIfNeeded(vcount);
	}
	//-----------------------------------------------------------------------------
	void ManualObject::estimateIndexCount(size_t icount)
	{
		resizeTempIndexBufferIfNeeded(icount);
	}
	//-----------------------------------------------------------------------------
	void ManualObject::begin(const String& materialName,
		RenderOperation::OperationType opType)
	{
		if (mCurrentSection)
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"You cannot call begin() again until after you call end()",
				"ManualObject::begin");
		}
		mCurrentSection = new ManualObjectSection(this, materialName, opType);
		mSectionList.push_back(mCurrentSection);
		mFirstVertex = true;
		mDeclSize = 0;
		mTexCoordIndex = 0;
	}
	//-----------------------------------------------------------------------------
	void ManualObject::position(const Vector3& pos)
	{
		position(pos.x, pos.y, pos.z);
	}
	//-----------------------------------------------------------------------------
	void ManualObject::position(Real x, Real y, Real z)
	{
		if (!mCurrentSection)
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"You must call begin() before this method",
				"ManualObject::position");
		}
		if (mTempVertexPending)
		{
			// bake current vertex
			copyTempVertexToBuffer();
			mFirstVertex = false;
		}

		if (mFirstVertex)
		{
			// defining declaration
			mCurrentSection->getRenderOperation()->vertexData->vertexDeclaration
				->addElement(0, mDeclSize, VET_FLOAT3, VES_POSITION);
			mDeclSize += VertexElement::getTypeSize(VET_FLOAT3);
		}

		mTempVertex.position.x = x;
		mTempVertex.position.y = y;
		mTempVertex.position.z = z;

		// update bounds
		mAABB.merge(mTempVertex.position);
		mRadius = std::max(mRadius, mTempVertex.position.length());

		// reset current texture coord
		mTexCoordIndex = 0;

		mTempVertexPending = true;
	}
	//-----------------------------------------------------------------------------
	void ManualObject::normal(const Vector3& norm)
	{
		normal(norm.x, norm.y, norm.z);
	}
	//-----------------------------------------------------------------------------
	void ManualObject::normal(Real x, Real y, Real z)
	{
		if (!mCurrentSection)
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"You must call begin() before this method",
				"ManualObject::normal");
		}
		if (mFirstVertex)
		{
			// defining declaration
			mCurrentSection->getRenderOperation()->vertexData->vertexDeclaration
				->addElement(0, mDeclSize, VET_FLOAT3, VES_NORMAL);
			mDeclSize += VertexElement::getTypeSize(VET_FLOAT3);
		}
		mTempVertex.normal.x = x;
		mTempVertex.normal.y = y;
		mTempVertex.normal.z = z;
	}
	//-----------------------------------------------------------------------------
	void ManualObject::textureCoord(Real u)
	{
		if (!mCurrentSection)
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"You must call begin() before this method",
				"ManualObject::textureCoord");
		}
		if (mFirstVertex)
		{
			// defining declaration
			mCurrentSection->getRenderOperation()->vertexData->vertexDeclaration
				->addElement(0, mDeclSize, VET_FLOAT1, VES_TEXTURE_COORDINATES, mTexCoordIndex);
			mDeclSize += VertexElement::getTypeSize(VET_FLOAT1);
		}
		mTempVertex.texCoordDims[mTexCoordIndex] = 1;
		mTempVertex.texCoord[mTexCoordIndex].x = u;

		++mTexCoordIndex;

	}
	//-----------------------------------------------------------------------------
	void ManualObject::textureCoord(Real u, Real v)
	{
		if (!mCurrentSection)
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"You must call begin() before this method",
				"ManualObject::textureCoord");
		}
		if (mFirstVertex)
		{
			// defining declaration
			mCurrentSection->getRenderOperation()->vertexData->vertexDeclaration
				->addElement(0, mDeclSize, VET_FLOAT2, VES_TEXTURE_COORDINATES, mTexCoordIndex);
			mDeclSize += VertexElement::getTypeSize(VET_FLOAT2);
		}
		mTempVertex.texCoordDims[mTexCoordIndex] = 2;
		mTempVertex.texCoord[mTexCoordIndex].x = u;
		mTempVertex.texCoord[mTexCoordIndex].y = v;

		++mTexCoordIndex;
	}
	//-----------------------------------------------------------------------------
	void ManualObject::textureCoord(Real u, Real v, Real w)
	{
		if (!mCurrentSection)
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"You must call begin() before this method",
				"ManualObject::textureCoord");
		}
		if (mFirstVertex)
		{
			// defining declaration
			mCurrentSection->getRenderOperation()->vertexData->vertexDeclaration
				->addElement(0, mDeclSize, VET_FLOAT3, VES_TEXTURE_COORDINATES, mTexCoordIndex);
			mDeclSize += VertexElement::getTypeSize(VET_FLOAT3);
		}
		mTempVertex.texCoordDims[mTexCoordIndex] = 3;
		mTempVertex.texCoord[mTexCoordIndex].x = u;
		mTempVertex.texCoord[mTexCoordIndex].y = v;
		mTempVertex.texCoord[mTexCoordIndex].z = w;

		++mTexCoordIndex;
	}
	//-----------------------------------------------------------------------------
	void ManualObject::textureCoord(const Vector2& uv)
	{
		textureCoord(uv.x, uv.y);
	}
	//-----------------------------------------------------------------------------
	void ManualObject::textureCoord(const Vector3& uvw)
	{
		textureCoord(uvw.x, uvw.y, uvw.z);
	}
	//-----------------------------------------------------------------------------
	void ManualObject::colour(const ColourValue& col)
	{
		colour(col.r, col.g, col.b, col.a);
	}
	//-----------------------------------------------------------------------------
	void ManualObject::colour(Real r, Real g, Real b, Real a)
	{
		if (!mCurrentSection)
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"You must call begin() before this method",
				"ManualObject::colour");
		}
		if (mFirstVertex)
		{
			// defining declaration
			mCurrentSection->getRenderOperation()->vertexData->vertexDeclaration
				->addElement(0, mDeclSize, VET_COLOUR, VES_DIFFUSE);
			mDeclSize += VertexElement::getTypeSize(VET_COLOUR);
		}
		mTempVertex.colour.r = r;
		mTempVertex.colour.g = g;
		mTempVertex.colour.b = b;
		mTempVertex.colour.a = a;

	}
	//-----------------------------------------------------------------------------
	void ManualObject::index(uint16 idx)
	{
		if (!mCurrentSection)
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"You must call begin() before this method",
				"ManualObject::index");
		}
		mAnyIndexed = true;
		// make sure we have index data
		RenderOperation* rop = mCurrentSection->getRenderOperation();
		if (!rop->indexData)
		{
			rop->indexData = new IndexData();
			rop->indexData->indexCount = 0;
			rop->useIndexes = true;
		}
		resizeTempIndexBufferIfNeeded(++rop->indexData->indexCount);

		mTempIndexBuffer[rop->indexData->indexCount - 1] = idx;
	}
	//-----------------------------------------------------------------------------
	void ManualObject::triangle(uint16 i1, uint16 i2, uint16 i3)
	{
		if (!mCurrentSection)
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"You must call begin() before this method",
				"ManualObject::index");
		}
		if (mCurrentSection->getRenderOperation()->operationType !=
			RenderOperation::OT_TRIANGLE_LIST)
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"This method is only valid on triangle lists",
				"ManualObject::index");
		}

		index(i1);
		index(i2);
		index(i3);
	}
	//-----------------------------------------------------------------------------
	void ManualObject::quad(uint16 i1, uint16 i2, uint16 i3, uint16 i4)
	{
		// first tri
		triangle(i1, i2, i3);
		// second tri
		triangle(i3, i4, i1);
	}
	//-----------------------------------------------------------------------------
	void ManualObject::copyTempVertexToBuffer(void)
	{
		mTempVertexPending = false;
		RenderOperation* rop = mCurrentSection->getRenderOperation();
		if (rop->vertexData->vertexCount == 0)
		{
			// first vertex, autoorganise decl
			VertexDeclaration* oldDcl = rop->vertexData->vertexDeclaration;
			rop->vertexData->vertexDeclaration =
				oldDcl->getAutoOrganisedDeclaration(false, false);
			HardwareBufferManager::getSingleton().destroyVertexDeclaration(oldDcl);
		}
		resizeTempVertexBufferIfNeeded(++rop->vertexData->vertexCount);

		// get base pointer
		char* pBase = mTempVertexBuffer + (mDeclSize * (rop->vertexData->vertexCount-1));
		const VertexDeclaration::VertexElementList& elemList =
			rop->vertexData->vertexDeclaration->getElements();
		for (VertexDeclaration::VertexElementList::const_iterator i = elemList.begin();
			i != elemList.end(); ++i)
		{
			float* pFloat;
			RGBA* pRGBA;
			const VertexElement& elem = *i;
			switch(elem.getType())
			{
			case VET_FLOAT1:
			case VET_FLOAT2:
			case VET_FLOAT3:
				elem.baseVertexPointerToElement(pBase, &pFloat);
				break;
			case VET_COLOUR:
			case VET_COLOUR_ABGR:
			case VET_COLOUR_ARGB:
				elem.baseVertexPointerToElement(pBase, &pRGBA);
				break;
			default:
				// nop ?
				break;
			};


			RenderSystem* rs;
			unsigned short dims;
			switch(elem.getSemantic())
			{
			case VES_POSITION:
				*pFloat++ = mTempVertex.position.x;
				*pFloat++ = mTempVertex.position.y;
				*pFloat++ = mTempVertex.position.z;
				break;
			case VES_NORMAL:
				*pFloat++ = mTempVertex.normal.x;
				*pFloat++ = mTempVertex.normal.y;
				*pFloat++ = mTempVertex.normal.z;
				break;
			case VES_TEXTURE_COORDINATES:
				dims = VertexElement::getTypeCount(elem.getType());
				for (ushort t = 0; t < dims; ++t)
					*pFloat++ = mTempVertex.texCoord[elem.getIndex()].val[t];
				break;
			case VES_DIFFUSE:
				rs = Root::getSingleton().getRenderSystem();
				if (rs)
					rs->convertColourValue(mTempVertex.colour, pRGBA++);
				else
					*pRGBA++ = mTempVertex.colour.getAsRGBA(); // pick one!
				break;
			default:
				// nop ?
				break;
			};

		}

	}
	//-----------------------------------------------------------------------------
	void ManualObject::end(void)
	{
		if (!mCurrentSection)
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"You cannot call end() until after you call begin()",
				"ManualObject::end");
		}
		if (mTempVertexPending)
		{
			// bake current vertex
			copyTempVertexToBuffer();
		}

		// Bake the real buffers
		RenderOperation* rop = mCurrentSection->getRenderOperation();
		if (rop->vertexData->vertexCount == 0)
		{
			// You're wasting my time sonny
			// Has already been added to section list, so remove
			mSectionList.pop_back();
			delete mCurrentSection;
		}
		else
		{
			HardwareVertexBufferSharedPtr vbuf =
				HardwareBufferManager::getSingleton().createVertexBuffer(
					mDeclSize,
					rop->vertexData->vertexCount,
					HardwareBuffer::HBU_STATIC_WRITE_ONLY);
			rop->vertexData->vertexBufferBinding->setBinding(0, vbuf);
			vbuf->writeData(0, vbuf->getSizeInBytes(), mTempVertexBuffer, true);

			if(rop->useIndexes)
			{
				rop->indexData->indexBuffer =
					HardwareBufferManager::getSingleton().createIndexBuffer(
						HardwareIndexBuffer::IT_16BIT,
						rop->indexData->indexCount,
						HardwareBuffer::HBU_STATIC_WRITE_ONLY);
				rop->indexData->indexBuffer->writeData(
					0, rop->indexData->indexBuffer->getSizeInBytes(),
					mTempIndexBuffer, true);
			}
		}

		mCurrentSection = 0;
		resetTempAreas();

		// Tell parent if present
		if (mParentNode)
		{
			mParentNode->needUpdate();
		}

	}
	//-----------------------------------------------------------------------------
	MeshPtr ManualObject::convertToMesh(const String& meshName, const String& groupName)
	{
		if (mCurrentSection)
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"You cannot call convertToMesh() whilst you are in the middle of "
				"defining the object; call end() first.",
				"ManualObject::convertToMesh");
		}
		if (mSectionList.empty())
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
				"No data defined to convert to a mesh.",
				"ManualObject::convertToMesh");
		}
		for (SectionList::iterator i = mSectionList.begin(); i != mSectionList.end(); ++i)
		{
			ManualObjectSection* sec = *i;
			if (!sec->getRenderOperation()->useIndexes)
			{
				OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
					"Only indexed geometry may be converted to a mesh.",
					"ManualObject::convertToMesh");
			}
		}
		MeshPtr m = MeshManager::getSingleton().createManual(meshName, groupName);

		for (SectionList::iterator i = mSectionList.begin(); i != mSectionList.end(); ++i)
		{
			ManualObjectSection* sec = *i;
			RenderOperation* rop = sec->getRenderOperation();
			SubMesh* sm = m->createSubMesh();
			sm->useSharedVertices = false;
			sm->operationType = rop->operationType;
			sm->setMaterialName(sec->getMaterialName());
			// Copy vertex data; replicate buffers too
			sm->vertexData = rop->vertexData->clone(true);
			// Copy index data; replicate buffers too; delete the default, old one to avoid memory leaks
			delete sm->indexData;
			sm->indexData = rop->indexData->clone(true);
		}
        // update bounds
		m->_setBounds(mAABB);
		m->_setBoundingSphereRadius(mRadius);

		m->load();

		return m;


	}
	//-----------------------------------------------------------------------------
	const String& ManualObject::getMovableType(void) const
	{
		return ManualObjectFactory::FACTORY_TYPE_NAME;
	}
	//-----------------------------------------------------------------------------
	const AxisAlignedBox& ManualObject::getBoundingBox(void) const
	{
		return mAABB;
	}
	//-----------------------------------------------------------------------------
	Real ManualObject::getBoundingRadius(void) const
	{
		return mRadius;
	}
	//-----------------------------------------------------------------------------
	void ManualObject::_updateRenderQueue(RenderQueue* queue)
	{
		for (SectionList::iterator i = mSectionList.begin(); i != mSectionList.end(); ++i)
		{
			if (mRenderQueueIDSet)
				queue->addRenderable(*i, mRenderQueueID);
			else
				queue->addRenderable(*i);
		}
	}
	//-----------------------------------------------------------------------------
	EdgeData* ManualObject::getEdgeList(void)
	{
		// Build on demand
		if (!mEdgeList && mAnyIndexed)
		{
			EdgeListBuilder eb;
			size_t vertexSet = 0;
			for (SectionList::iterator i = mSectionList.begin(); i != mSectionList.end(); ++i)
			{
				RenderOperation* rop = (*i)->getRenderOperation();
				// Only indexed geometry supported for stencil shadows
				if (rop->useIndexes)
				{
					eb.addVertexData(rop->vertexData);
					eb.addIndexData(rop->indexData, vertexSet++);
				}
			}

			mEdgeList = eb.build();

		}
		return mEdgeList;
	}
	//-----------------------------------------------------------------------------
	ShadowCaster::ShadowRenderableListIterator
	ManualObject::getShadowVolumeRenderableIterator(
		ShadowTechnique shadowTechnique, const Light* light,
		HardwareIndexBufferSharedPtr* indexBuffer,
		bool extrude, Real extrusionDistance, unsigned long flags)
	{
		assert(indexBuffer && "Only external index buffers are supported right now");
		assert((*indexBuffer)->getType() == HardwareIndexBuffer::IT_16BIT &&
			"Only 16-bit indexes supported for now");

        EdgeData* edgeList = getEdgeList();
        if (!edgeList)
        {
            return ShadowRenderableListIterator(
                mShadowRenderables.begin(), mShadowRenderables.end());
        }

		// Calculate the object space light details
		Vector4 lightPos = light->getAs4DVector();
		Matrix4 world2Obj = mParentNode->_getFullTransform().inverseAffine();
		lightPos = world2Obj.transformAffine(lightPos);


		// Init shadow renderable list if required (only allow indexed)
		bool init = mShadowRenderables.empty() && mAnyIndexed;

		EdgeData::EdgeGroupList::iterator egi;
		ShadowRenderableList::iterator si, siend;
		ManualObjectSectionShadowRenderable* esr = 0;
		SectionList::iterator seci;
		if (init)
			mShadowRenderables.resize(edgeList->edgeGroups.size());

		siend = mShadowRenderables.end();
		egi = edgeList->edgeGroups.begin();
		seci = mSectionList.begin();
		for (si = mShadowRenderables.begin(); si != siend; ++seci)
		{
            // Skip non-indexed geometry
            if (!(*seci)->getRenderOperation()->useIndexes)
            {
                continue;
            }

			if (init)
			{
				// Create a new renderable, create a separate light cap if
				// we're using a vertex program (either for this model, or
				// for extruding the shadow volume) since otherwise we can
				// get depth-fighting on the light cap
				MaterialPtr mat = (*seci)->getMaterial();
				mat->load();
				bool vertexProgram = false;
				Technique* t = mat->getBestTechnique();
				for (int p = 0; p < t->getNumPasses(); ++p)
				{
					Pass* pass = t->getPass(p);
					if (pass->hasVertexProgram())
					{
						vertexProgram = true;
						break;
					}
				}
				*si = new ManualObjectSectionShadowRenderable(this, indexBuffer,
					egi->vertexData, vertexProgram || !extrude);
			}
			// Get shadow renderable
			esr = static_cast<ManualObjectSectionShadowRenderable*>(*si);
			const HardwareVertexBufferSharedPtr& esrPositionBuffer = esr->getPositionBuffer();
			// Extrude vertices in software if required
			if (extrude)
			{
				extrudeVertices(esrPositionBuffer,
					egi->vertexData->vertexCount,
					lightPos, extrusionDistance);

			}

            ++si;
            ++egi;
		}
		// Calc triangle light facing
		updateEdgeListLightFacing(edgeList, lightPos);

		// Generate indexes and update renderables
		generateShadowVolume(edgeList, *indexBuffer, light,
			mShadowRenderables, flags);


		return ShadowRenderableListIterator(
			mShadowRenderables.begin(), mShadowRenderables.end());


	}
	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	ManualObject::ManualObjectSection::ManualObjectSection(ManualObject* parent,
		const String& materialName,	RenderOperation::OperationType opType)
		: mParent(parent), mMaterialName(materialName)
	{
		mRenderOperation.operationType = opType;
		// default to no indexes unless we're told
		mRenderOperation.useIndexes = false;
		mRenderOperation.vertexData = new VertexData();
		mRenderOperation.vertexData->vertexCount = 0;

	}
	//-----------------------------------------------------------------------------
	ManualObject::ManualObjectSection::~ManualObjectSection()
	{
		delete mRenderOperation.vertexData;
		delete mRenderOperation.indexData; // ok to delete 0
	}
	//-----------------------------------------------------------------------------
	RenderOperation* ManualObject::ManualObjectSection::getRenderOperation(void)
	{
		return &mRenderOperation;
	}
	//-----------------------------------------------------------------------------
	const MaterialPtr& ManualObject::ManualObjectSection::getMaterial(void) const
	{
		if (mMaterial.isNull())
		{
			// Load from default group. If user wants to use alternate groups,
			// they can define it and preload
			mMaterial = MaterialManager::getSingleton().load(mMaterialName,
				ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		}
		return mMaterial;
	}
	//-----------------------------------------------------------------------------
	void ManualObject::ManualObjectSection::getRenderOperation(RenderOperation& op)
	{
		// direct copy
		op = mRenderOperation;
	}
	//-----------------------------------------------------------------------------
	void ManualObject::ManualObjectSection::getWorldTransforms(Matrix4* xform) const
	{
		xform[0] = mParent->_getParentNodeFullTransform();
	}
	//-----------------------------------------------------------------------------
	const Quaternion& ManualObject::ManualObjectSection::getWorldOrientation(void) const
	{
		return mParent->getParentNode()->_getDerivedOrientation();
	}
	//-----------------------------------------------------------------------------
	const Vector3& ManualObject::ManualObjectSection::getWorldPosition(void) const
	{
		return mParent->getParentNode()->_getDerivedPosition();
	}
	//-----------------------------------------------------------------------------
	Real ManualObject::ManualObjectSection::getSquaredViewDepth(const Ogre::Camera *cam) const
	{
		Node* n = mParent->getParentNode();
		assert(n);
		return n->getSquaredViewDepth(cam);
	}
	//-----------------------------------------------------------------------------
	const LightList& ManualObject::ManualObjectSection::getLights(void) const
	{
		return mParent->queryLights();
	}
	//-----------------------------------------------------------------------------
	//--------------------------------------------------------------------------
	ManualObject::ManualObjectSectionShadowRenderable::ManualObjectSectionShadowRenderable(
		ManualObject* parent, HardwareIndexBufferSharedPtr* indexBuffer,
		const VertexData* vertexData, bool createSeparateLightCap,
		bool isLightCap)
		: mParent(parent)
	{
		// Initialise render op
		mRenderOp.indexData = new IndexData();
		mRenderOp.indexData->indexBuffer = *indexBuffer;
		mRenderOp.indexData->indexStart = 0;
		// index start and count are sorted out later

		// Create vertex data which just references position component (and 2 component)
		mRenderOp.vertexData = new VertexData();
		// Map in position data
		mRenderOp.vertexData->vertexDeclaration->addElement(0,0,VET_FLOAT3, VES_POSITION);
		ushort origPosBind =
			vertexData->vertexDeclaration->findElementBySemantic(VES_POSITION)->getSource();
		mPositionBuffer = vertexData->vertexBufferBinding->getBuffer(origPosBind);
		mRenderOp.vertexData->vertexBufferBinding->setBinding(0, mPositionBuffer);
		// Map in w-coord buffer (if present)
		if(!vertexData->hardwareShadowVolWBuffer.isNull())
		{
			mRenderOp.vertexData->vertexDeclaration->addElement(1,0,VET_FLOAT1, VES_TEXTURE_COORDINATES, 0);
			mWBuffer = vertexData->hardwareShadowVolWBuffer;
			mRenderOp.vertexData->vertexBufferBinding->setBinding(1, mWBuffer);
		}
		// Use same vertex start as input
		mRenderOp.vertexData->vertexStart = vertexData->vertexStart;

		if (isLightCap)
		{
			// Use original vertex count, no extrusion
			mRenderOp.vertexData->vertexCount = vertexData->vertexCount;
		}
		else
		{
			// Vertex count must take into account the doubling of the buffer,
			// because second half of the buffer is the extruded copy
			mRenderOp.vertexData->vertexCount =
				vertexData->vertexCount * 2;
			if (createSeparateLightCap)
			{
				// Create child light cap
				mLightCap = new ManualObjectSectionShadowRenderable(parent,
					indexBuffer, vertexData, false, true);
			}
		}
	}
	//--------------------------------------------------------------------------
	ManualObject::ManualObjectSectionShadowRenderable::~ManualObjectSectionShadowRenderable()
	{
		delete mRenderOp.indexData;
		delete mRenderOp.vertexData;
	}
	//--------------------------------------------------------------------------
	void ManualObject::ManualObjectSectionShadowRenderable::getWorldTransforms(
		Matrix4* xform) const
	{
		// pretransformed
		*xform = mParent->_getParentNodeFullTransform();
	}
	//--------------------------------------------------------------------------
	const Quaternion&
		ManualObject::ManualObjectSectionShadowRenderable::getWorldOrientation(void) const
	{
		return mParent->getParentNode()->_getDerivedOrientation();
	}
	//--------------------------------------------------------------------------
	const Vector3&
		ManualObject::ManualObjectSectionShadowRenderable::getWorldPosition(void) const
	{
		return mParent->getParentNode()->_getDerivedPosition();
	}
	//-----------------------------------------------------------------------------
	//-----------------------------------------------------------------------------
	String ManualObjectFactory::FACTORY_TYPE_NAME = "ManualObject";
	//-----------------------------------------------------------------------------
	const String& ManualObjectFactory::getType(void) const
	{
		return FACTORY_TYPE_NAME;
	}
	//-----------------------------------------------------------------------------
	MovableObject* ManualObjectFactory::createInstanceImpl(
		const String& name, const NameValuePairList* params)
	{
		return new ManualObject(name);
	}
	//-----------------------------------------------------------------------------
	void ManualObjectFactory::destroyInstance( MovableObject* obj)
	{
		delete obj;
	}



}
