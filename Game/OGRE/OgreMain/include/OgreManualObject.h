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

#ifndef __OgreManualObject_H__
#define __OgreManualObject_H__

#include "OgrePrerequisites.h"
#include "OgreMovableObject.h"
#include "OgreRenderable.h"
#include "OgreResourceGroupManager.h"


namespace Ogre
{
	/** Class providing a much simplified interface to generating manual
	 	objects with custom geometry.
	@remarks
		Building one-off geometry objects manually usually requires getting
		down and dirty with the vertex buffer and vertex declaration API, 
		which some people find a steep learning curve. This class gives you 
		a simpler interface specifically for the purpose of building a one-off
		3D object simply and quickly. Note that if you intend to instance your
		object you will still need to become familiar with the Mesh class. 
	@par
		This class draws heavily on the interface for OpenGL 
		immediate-mode (glBegin, glVertex, glNormal etc), since this
		is generally well-liked by people. There are a couple of differences
		in the results though - internally this class still builds hardware 
		buffers which can be re-used, so you can render the resulting object
		multiple times without re-issuing all the same commands again. 
		Secondly, the rendering is not immediate, it is still queued just like
		all OGRE objects. This makes this object more efficient than the 
		equivalent GL immediate-mode commands, so it's feasible to use it for
		large objects if you really want to.
	@par
		To construct some geometry with this object:
		  -# If you know roughly how many vertices (and indices, if you use them)
		     you're going to submit, call estimateVertexCount and estimateIndexCount.
			 This is not essential but will make the process more efficient by saving
			 memory reallocations.
		  -# Call begin() to begin entering data
		  -# For each vertex, call position(), normal(), textureCoord(), colour()
		     to define your vertex data. Note that each time you call position()
			 you start a new vertex. Note that the first vertex defines the 
			 components of the vertex - you can't add more after that. For example
			 if you didn't call normal() in the first vertex, you cannot call it
			 in any others. You ought to call the same combination of methods per
			 vertex.
		  -# If you want to define triangles (or lines/points) by indexing into the vertex list, 
			 you can call index() as many times as you need to define them.
			 If you don't do this, the class will assume you want triangles drawn
			 directly as defined by the vertex list, ie non-indexed geometry. Note
			 that stencil shadows are only supported on indexed geometry, and that
			 indexed geometry is a little faster; so you should try to use it.
		  -# Call end() to finish entering data.
		  -# Optionally repeat the begin-end cycle if you want more geometry 
		  	using different rendering operation types, or different materials
	    After calling end(), the class will organise the data for that section
		internally and make it ready to render with. Like any other 
		MovableObject you should attach the object to a SceneNode to make it 
		visible. Other aspects like the relative render order can be controlled
		using standard MovableObject methods like setRenderQueueGroup.
	@par
		Note that like all OGRE geometry, triangles should be specified in 
		anti-clockwise winding order (whether you're doing it with just
		vertices, or using indexes too). That is to say that the front of the
		face is the one where the vertices are listed in anti-clockwise order.
	*/
	class _OgreExport ManualObject : public MovableObject
	{
	public:
		ManualObject(const String& name);
		virtual ~ManualObject();

		/** Completely clear the contents of the object.
		@remarks
			This class is not designed for dynamic vertex data, since the 
			translation it has to perform is not suitable for frame-by-frame
			updates. However if you do want to modify the contents from time
			to time you can do so by clearing and re-specifying the data.
		*/
		virtual void clear(void);
		
		/** Estimate the number of vertices ahead of time.
		@remarks
			Calling this helps to avoid memory reallocation when you define
			vertices. 
		*/
		virtual void estimateVertexCount(size_t vcount);

		/** Estimate the number of vertices ahead of time.
		@remarks
			Calling this helps to avoid memory reallocation when you define
			indices. 
		*/
		virtual void estimateIndexCount(size_t icount);

		/** Start defining a part of the object.
		@remarks
			Each time you call this method, you start a new section of the
			object with its own material and potentially its own type of
			rendering operation (triangles, points or lines for example).
		@param materialName The name of the material to render this part of the
			object with.
		@param opType The type of operation to use to render. 
		*/
		virtual void begin(const String& materialName, 
			RenderOperation::OperationType opType = RenderOperation::OT_TRIANGLE_LIST);
		/** Add a vertex position, starting a new vertex at the same time. 
		@remarks A vertex position is slightly special among the other vertex data
			methods like normal() and textureCoord(), since calling it indicates
			the start of a new vertex. All other vertex data methods you call 
			after this are assumed to be adding more information (like normals or
			texture coordinates) to the last vertex started with position().
		*/
		virtual void position(const Vector3& pos);
		/// @copydoc ManualObject::position(const Vector3&)
		virtual void position(Real x, Real y, Real z);

		/** Add a vertex normal to the current vertex.
		@remarks
			Vertex normals are most often used for dynamic lighting, and 
			their components should be normalised.
		*/
		virtual void normal(const Vector3& norm);
		/// @copydoc ManualObject::normal(const Vector3&)
		virtual void normal(Real x, Real y, Real z);

		/** Add a texture coordinate to the current vertex.
		@remarks
			You can call this method multiple times between position() calls
			to add multiple texture coordinates to a vertex. Each one can have
			between 1 and 3 dimensions, depending on your needs, although 2 is
			most common. There are several versions of this method for the 
			variations in number of dimensions.
		*/
		virtual void textureCoord(Real u);
		/// @copydoc ManualObject::textureCoord(Real)
		virtual void textureCoord(Real u, Real v);
		/// @copydoc ManualObject::textureCoord(Real)
		virtual void textureCoord(Real u, Real v, Real w);
		/// @copydoc ManualObject::textureCoord(Real)
		virtual void textureCoord(const Vector2& uv);
		/// @copydoc ManualObject::textureCoord(Real)
		virtual void textureCoord(const Vector3& uvw);

		/** Add a vertex colour to a vertex.
		*/
		virtual void colour(const ColourValue& col);
		/** Add a vertex colour to a vertex.
		@param r,g,b,a Colour components expressed as floating point numbers from 0-1
		*/
		virtual void colour(Real r, Real g, Real b, Real a = 1.0f);

		/** Add a vertex index to construct faces / lines / points via indexing
			rather than just by a simple list of vertices. 
		@remarks
			You will have to call this 3 times for each face for a triangle list, 
			or use the alternative 3-parameter version. Other operation types
			require different numbers of indexes, @see RenderOperation::OperationType.
		@note
			32-bit indexes are not supported on all cards which is why this 
			class only allows 16-bit indexes, for simplicity and ease of use.
		@param idx A vertex index from 0 to 65535. 
		*/
		virtual void index(uint16 idx);
		/** Add a set of 3 vertex indices to construct a triangle; this is a
			shortcut to calling index() 3 times. It is only valid for triangle 
			lists.
		@note
			32-bit indexes are not supported on all cards which is why this 
			class only allows 16-bit indexes, for simplicity and ease of use.
		@param i1, i2, i3 3 vertex indices from 0 to 65535 defining a face. 
		*/
		virtual void triangle(uint16 i1, uint16 i2, uint16 i3);
		/** Add a set of 4 vertex indices to construct a quad (out of 2 
			triangles); this is a shortcut to calling index() 6 times, 
			or triangle() twice. It's only valid for triangle list operations.
		@note
			32-bit indexes are not supported on all cards which is why this 
			class only allows 16-bit indexes, for simplicity and ease of use.
		@param i1, i2, i3 3 vertex indices from 0 to 65535 defining a face. 
		*/
		virtual void quad(uint16 i1, uint16 i2, uint16 i3, uint16 i4);

		/** Finish defining the object and compile the final renderable version. */
		virtual void end(void);

		/** Convert this object to a Mesh. 
		@remarks
			After you've finished building this object, you may convert it to 
			a Mesh if you want in order to be able to create many instances of
			it in the world (via Entity). This is optional, since this instance
			can be directly attached to a SceneNode itself, but of course only
			one instance of it can exist that way. 
		@note Only objects which use indexed geometry may be converted to a mesh.
		@param meshName The name to give the mesh
		@param groupName The resource group to create the mesh in
		*/
		virtual MeshPtr convertToMesh(const String& meshName, 
			const String& groupName = ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		// MovableObject overrides

		/** @copydoc MovableObject::getMovableType. */
		const String& getMovableType(void) const;
		/** @copydoc MovableObject::getBoundingBox. */
		const AxisAlignedBox& getBoundingBox(void) const;
		/** @copydoc MovableObject::getBoundingRadius. */
		Real getBoundingRadius(void) const;
		/** @copydoc MovableObject::_updateRenderQueue. */
		void _updateRenderQueue(RenderQueue* queue);
		/** Implement this method to enable stencil shadows. */
		EdgeData* getEdgeList(void);
		/** Implement this method to enable stencil shadows. */
		ShadowRenderableListIterator getShadowVolumeRenderableIterator(
			ShadowTechnique shadowTechnique, const Light* light, 
			HardwareIndexBufferSharedPtr* indexBuffer, 
			bool extrudeVertices, Real extrusionDist, unsigned long flags = 0);


		/// Built, renderable section of geometry
		class _OgreExport ManualObjectSection : public Renderable
		{
		protected:
			ManualObject* mParent;
			String mMaterialName;
			mutable MaterialPtr mMaterial;
			RenderOperation mRenderOperation;
			
		public:
			ManualObjectSection(ManualObject* parent, const String& materialName,
				RenderOperation::OperationType opType);
			virtual ~ManualObjectSection();
			
			/// Retrieve render operation for manipulation
			RenderOperation* getRenderOperation(void);
			/// Retrieve the material name in use
			const String& getMaterialName(void) const { return mMaterialName; }
			
			// Renderable overrides
			/** @copydoc Renderable::getMaterial. */
			const MaterialPtr& getMaterial(void) const;
			/** @copydoc Renderable::getRenderOperation. */
			void getRenderOperation(RenderOperation& op);
			/** @copydoc Renderable::getWorldTransforms. */
			void getWorldTransforms(Matrix4* xform) const;
			/** @copydoc Renderable::getWorldOrientation. */
			const Quaternion& getWorldOrientation(void) const;
			/** @copydoc Renderable::getWorldPosition. */
			const Vector3& getWorldPosition(void) const;
			/** @copydoc Renderable::getSquaredViewDepth. */
			Real getSquaredViewDepth(const Ogre::Camera *) const;
			/** @copydoc Renderable::getLights. */
			const LightList &getLights(void) const;
					
		};
		/** Nested class to allow shadows. */
		class _OgreExport ManualObjectSectionShadowRenderable : public ShadowRenderable
		{
		protected:
			ManualObject* mParent;
			// Shared link to position buffer
			HardwareVertexBufferSharedPtr mPositionBuffer;
			// Shared link to w-coord buffer (optional)
			HardwareVertexBufferSharedPtr mWBuffer;

		public:
			ManualObjectSectionShadowRenderable(ManualObject* parent, 
				HardwareIndexBufferSharedPtr* indexBuffer, const VertexData* vertexData, 
				bool createSeparateLightCap, bool isLightCap = false);
			~ManualObjectSectionShadowRenderable();
			/// Overridden from ShadowRenderable
			void getWorldTransforms(Matrix4* xform) const;
			/// Overridden from ShadowRenderable
			const Quaternion& getWorldOrientation(void) const;
			/// Overridden from ShadowRenderable
			const Vector3& getWorldPosition(void) const;
			const HardwareVertexBufferSharedPtr& getPositionBuffer(void) const { return mPositionBuffer; }
			const HardwareVertexBufferSharedPtr& getWBuffer(void) const { return mWBuffer; }

		};

		typedef std::vector<ManualObjectSection*> SectionList;
		
	protected:
		/// List of subsections
		SectionList mSectionList;
		/// Current section
		ManualObjectSection* mCurrentSection;
		/// Temporary vertex structure
		struct TempVertex
		{
			Vector3 position;
			Vector3 normal;
			Vector3 texCoord[OGRE_MAX_TEXTURE_COORD_SETS];
			ushort texCoordDims[OGRE_MAX_TEXTURE_COORD_SETS];
			ColourValue colour;
		};
		/// Temp storage
		TempVertex mTempVertex;
		/// First vertex indicator
		bool mFirstVertex;
		/// Temp vertex data to copy?
		bool mTempVertexPending;
		/// System-memory buffer whilst we establish the size required
		char* mTempVertexBuffer;
		/// System memory allocation size, in bytes
		size_t mTempVertexSize;
		/// System-memory buffer whilst we establish the size required
		uint16* mTempIndexBuffer;
		/// System memory allocation size, in bytes
		size_t mTempIndexSize;
		/// Current declaration vertex size
		size_t mDeclSize;
		/// Current texture coordinate
		ushort mTexCoordIndex;
		/// Bounding box
		AxisAlignedBox mAABB;
		/// Bounding sphere
		Real mRadius;
		/// Any indexed geoemtry on any sections?
		bool mAnyIndexed;
		/// Edge list, used if stencil shadow casting is enabled 
		EdgeData* mEdgeList;
		/// List of shadow renderables
		ShadowRenderableList mShadowRenderables;


		/// Delete temp buffers and reset init counts
		virtual void resetTempAreas(void);
		/// Resize the temp vertex buffer?
		virtual void resizeTempVertexBufferIfNeeded(size_t numVerts);
		/// Resize the temp index buffer?
		virtual void resizeTempIndexBufferIfNeeded(size_t numInds);

		/// Copy current temp vertex into buffer
		virtual void copyTempVertexToBuffer(void);

	};


	/** Factory object for creating ManualObject instances */
	class _OgreExport ManualObjectFactory : public MovableObjectFactory
	{
	protected:
		MovableObject* createInstanceImpl( const String& name, const NameValuePairList* params);
	public:
		ManualObjectFactory() {}
		~ManualObjectFactory() {}

		static String FACTORY_TYPE_NAME;

		const String& getType(void) const;
		void destroyInstance( MovableObject* obj);  

	};
}

#endif

