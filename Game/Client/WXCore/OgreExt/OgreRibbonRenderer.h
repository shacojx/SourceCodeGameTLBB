/********************************************************************
filename:   OgreRibbonRenderer.h

purpose:    ribbon particle renderer
*********************************************************************/
#ifndef __RibbonRenderer_H__
#define __RibbonRenderer_H__

#include <OgreParticleSystemRenderer.h>

namespace Ogre
{
    class RibbonSet;

    class RibbonRenderer : public ParticleSystemRenderer
    {
    public:

        class _OgrePrivate CmdRibbonHeadAlpha : public ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
        };

        class _OgrePrivate CmdRibbonTailAlpha : public ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
        };

        class _OgrePrivate CmdRibbonHeadWidthScale : public ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
        };

        class _OgrePrivate CmdRibbonTailWidthScale : public ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
        };

        class _OgrePrivate CmdRibbonElementCount : public ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
        };

        RibbonRenderer();
        ~RibbonRenderer();

        void setRibbonHeadAlpha(Real alpha);
        Real getRibbonHeadAlpha(void) const;

        void setRibbonTailAlpha(Real alpha);
        Real getRibbonTailAlpha(void) const;

        void setRibbonHeadWidthScale(Real width);
        Real getRibbonHeadWidthScale(void) const;

        void setRibbonTailWidthScale(Real width);
        Real getRibbonTailWidthScale(void) const;

        void setRibbonElementCount(size_t count);
        size_t getRibbonElementCount(void) const;

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
        /** Optional callback notified when particle expired */
        virtual void _notifyParticleExpired(Particle* particle);
        /** Optional callback notified when particles moved */
        virtual void _notifyParticleMoved(std::list<Particle*>& currentParticles);

    protected:

        RibbonSet* mRibbonSet;

        /// is the ribbon in world space
        bool mWorldSpace;

        /// 用于标识当前的空闲ribbon下标
        size_t mNextRibbonIndex;

        static CmdRibbonHeadAlpha msRibbonHeadAlphaCmd;
        static CmdRibbonTailAlpha msRibbonTailAlphaCmd;
        static CmdRibbonHeadWidthScale msRibbonHeadWidthScaleCmd;
        static CmdRibbonTailWidthScale msRibbonTailWidthScaleCmd;
        static CmdRibbonElementCount msRibbonElementCountCmd;
    };

    /** Factory class for MeshParticleRenderer */
    class RibbonRendererFactory : public ParticleSystemRendererFactory
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

#endif // __RibbonRenderer_H__