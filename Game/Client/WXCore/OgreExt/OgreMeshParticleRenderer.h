/********************************************************************
filename:   OgreMeshParticleRenderer.h

purpose:    Specialisation of ParticleSystemRenderer to render particles using 
			mesh. 
*********************************************************************/

#ifndef __MeshParticleRenderer_H__
#define __MeshParticleRenderer_H__

#include "OgrePrerequisites.h"
#include "OgreParticleSystemRenderer.h"
#include "OgreMaterial.h"

namespace Ogre	{

	class MeshParticleVisualData;
	class Node;

	/** The type of billboard to use. */
	enum MeshOrientationType
	{
		/// origin mesh orientation
		MOT_ORIENTED_ORIGIN,
		/// oriented speed direction
		MOT_ORIENTED_SELF,
	};

	class MeshParticleRenderer : public ParticleSystemRenderer
	{
	public:

		/** Command object for mesh name (see ParamCommand).*/
		class _OgrePrivate CmdMeshName : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};

		/** Command object for orientation type (see ParamCommand).*/
		class _OgrePrivate CmdOrientationType : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};

		typedef std::vector<MeshParticleVisualData *> MeshParticleVisualDataPool;

		//////////////////////////////////////////////////////////////////////////
		MeshParticleRenderer();
		~MeshParticleRenderer();

		/// @copydoc ParticleSystemRenderer::getType
		virtual const String& getType(void) const;
		/// @copydoc ParticleSystemRenderer::_updateRenderQueue
		virtual void _updateRenderQueue(RenderQueue* queue, 
			std::list<Particle*>& currentParticles, bool cullIndividually);
		/// @copydoc ParticleSystemRenderer::_setMaterial
		virtual void _setMaterial(MaterialPtr& mat);
		/// @copydoc ParticleSystemRenderer::_notifyCurrentCamera
		virtual void _notifyCurrentCamera(Camera* cam);
		/// @copydoc ParticleSystemRenderer::_notifyParticleRotated
		virtual void _notifyParticleRotated(void);
		/// @copydoc ParticleSystemRenderer::_notifyParticleResized
		virtual void _notifyParticleResized(void);
		/// @copydoc ParticleSystemRenderer::_notifyParticleQuota
		virtual void _notifyParticleQuota(size_t quota);
		/// @copydoc ParticleSystemRenderer::_notifyAttached
		virtual void _notifyAttached(Node* parent, bool isTagPoint = false);
		/// @copydoc ParticleSystemRenderer::_notifyDefaultDimensions
		virtual void _notifyDefaultDimensions(Real width, Real height);
		/// @copydoc ParticleSystemRenderer::setRenderQueueGroup
		virtual void setRenderQueueGroup(uint8 queueID);
		/// @copydoc ParticleSystemRenderer::setKeepParticlesInLocalSpace
		virtual void setKeepParticlesInLocalSpace(bool keepLocal);
        /// @copydoc ParticleSystemRenderer::_getSortMode
        virtual Ogre::SortMode _getSortMode(void) const;
		/// @copydoc ParticleSystemRenderer::_createVisualData
		virtual ParticleVisualData* _createVisualData(void);
		/// @copydoc ParticleSystemRenderer::_destroyVisualData
		virtual void _destroyVisualData(ParticleVisualData* vis);
        /** Optional callback notified when particles moved */
        virtual void _notifyParticleMoved(std::list<Particle*>& currentParticles);
        /** Optional callback notified when particle expired */
        virtual void _notifyParticleExpired(Particle* particle);

		/// sets the name of the mesh that particle used.
		void setMeshName(const String& name);
		const String& getMeshName(void) const	{	return mMeshName;	}

		/// set the material of the mesh
		void setMaterialName( const String& name );

		/// sets the mesh orientation type
		void setMeshOrientationType(MeshOrientationType mot);
		MeshOrientationType getMeshOrientationType(void) const;

		/// remove all visible mesh particle
		void removeAllMeshParticle(void);

	protected:

		/// destroy all the mesh particle in the renderer
		void _freeParticleMeshVisualDatas(void);

	protected:

		/// the name of the mesh.
		String mMeshName;
		/// Name of the material to use.
        String mMaterialName;
        /// Pointer to the material to use.
        MaterialPtr mpMaterial;

		/// whether the mesh in move in world space.
		bool mWorldSpace;

		/// whether the mesh will be rotated.
		bool mMeshRotation;

		/// whether the size of the mesh will be changed.
		bool mMeshSizeChanged;

		/// the parent node of the particle system attached.
		Node *mParticleSystemNode;

		Real mDefaultWidth;
		Real mDefaultHeight;

		MeshOrientationType mMeshOrientationType;

		MeshParticleVisualDataPool mMeshParticleVisualDataPool;

        size_t mNextPoolIndex;

	protected:

		static CmdMeshName msMeshNameCmd;
		static CmdOrientationType msOrientationTypeCmd;
	};

	/** Factory class for MeshParticleRenderer */
	class MeshParticleRendererFactory : public ParticleSystemRendererFactory
	{
	public:
		/// @copydoc FactoryObj::getType
		virtual const String& getType() const;
		/// @copydoc FactoryObj::createInstance
		virtual ParticleSystemRenderer* createInstance( const String& name );    
		/// @copydoc FactoryObj::destroyInstance
		virtual void destroyInstance( ParticleSystemRenderer* inst);    
	};
}

#endif