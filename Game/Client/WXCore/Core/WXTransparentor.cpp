/********************************************************************
    filename:   WXTransparentor.cpp
    
    purpose:    Dealing with auto transparency adjustment.
*********************************************************************/

#include "WXTransparentor.h"
#include "WXMaterialInstance.h"

#include <OgreEntity.h>
#include <OgreSubEntity.h>
#include <OgreTechnique.h>
#include <OgrePass.h>

namespace WX {

    //////////////////////////////////////////////////////////////////////////
    bool applyTransparency(const Ogre::MaterialPtr& origin, const Ogre::MaterialPtr& derived, Real transparency)
    {
        using namespace Ogre;

        assert(0 <= transparency && transparency <= 1);

        Real opacity = 1 - transparency;
        bool applied = false;

        Material::TechniqueIterator oti = origin->getTechniqueIterator();
        Material::TechniqueIterator dti = derived->getTechniqueIterator();
        while (oti.hasMoreElements())
        {
            Technique* ot = oti.getNext();
            Technique* dt = dti.getNext();
            if (ot->getNumPasses() <= 0)
                continue;

            if (ot->getNumPasses() == dt->getNumPasses())
            {
                Pass* oldPass = ot->getPass(0);
                if (oldPass->getDepthCheckEnabled() &&
                    oldPass->getDepthWriteEnabled() &&
                    oldPass->getColourWriteEnabled())
                {
                    applied = true;

                    // Insert depth buffer setup pass
                    Pass* newPass = dt->createPass();
                    dt->movePass(newPass->getIndex(), 0);

                    // Copy from refer pass
                    *newPass = *oldPass;
                    if (oldPass->getAlphaRejectFunction() == CMPF_ALWAYS_PASS &&
                        !oldPass->isProgrammable())
                    {
                        // Remove unnecessary texture units
                        newPass->removeAllTextureUnitStates();
                    }

                    // Disable colour write
                    newPass->setColourWriteEnabled(false);

                    // Enable fake alpha blending, but don't modify frame buffer
                    newPass->setSceneBlending(SBF_ZERO, SBF_ONE);
                }
            }
            else if (ot->getNumPasses()+1 != dt->getNumPasses())
            {
                OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS,
                    "Not support derived material",
                    "applyTransparency");
            }

            Technique::PassIterator opi = ot->getPassIterator();
            Technique::PassIterator dpi = dt->getPassIterator();

            if (ot->getNumPasses() < dt->getNumPasses())
            {
                // Special case for first depth buffer setup pass.
                Pass* op = opi.peekNext();
                Pass* dp = dpi.getNext();
                /* Seems not work
                if (op->getAlphaRejectFunction() != CMPF_ALWAYS_PASS)
                {
                    const ColourValue& diffuse = op->getDiffuse();
                    dp->setDiffuse(diffuse.r, diffuse.g, diffuse.b, diffuse.a * opacity);
                }
                */
            }

            while (opi.hasMoreElements())
            {
                Pass* op = opi.getNext();
                Pass* dp = dpi.getNext();
                SceneBlendFactor bfSrc = op->getSourceBlendFactor();
                SceneBlendFactor bfDst = op->getDestBlendFactor();
                if (bfSrc == SBF_ONE && bfDst == SBF_ZERO ||
                    bfSrc == SBF_SOURCE_ALPHA && bfDst == SBF_ONE_MINUS_SOURCE_ALPHA)
                {
                    applied = true;

                    dp->setDepthWriteEnabled(false);
                    dp->setSceneBlending(SBF_SOURCE_ALPHA, SBF_ONE_MINUS_SOURCE_ALPHA);
                    const ColourValue& diffuse = op->getDiffuse();
                    dp->setDiffuse(diffuse.r, diffuse.g, diffuse.b, diffuse.a * opacity);
                    switch (op->getAlphaRejectFunction())
                    {
                    case CMPF_ALWAYS_FAIL:
                    case CMPF_ALWAYS_PASS:
                        break;
                    case CMPF_LESS:
                    case CMPF_LESS_EQUAL:
                    case CMPF_EQUAL:
                    case CMPF_NOT_EQUAL:
                    case CMPF_GREATER_EQUAL:
                    case CMPF_GREATER:
                        dp->setAlphaRejectValue(static_cast<uchar>(op->getAlphaRejectValue() * opacity));
                        break;
                    }
                }
                else if (bfSrc == SBF_ONE && bfDst == SBF_ONE)
                {
                    applied = true;

                    // Disable depth write if exists extra depth buffer setup pass
                    if (ot->getNumPasses() < dt->getNumPasses())
                        dp->setDepthWriteEnabled(false);

                    const ColourValue& diffuse = op->getDiffuse();
                    dp->setDiffuse(diffuse.r * opacity, diffuse.g * opacity, diffuse.b * opacity, diffuse.a);
                    dp->setAmbient(op->getAmbient() * opacity);
                    dp->setSpecular(op->getSpecular() * opacity);
                    dp->setSelfIllumination(op->getSelfIllumination() * opacity);
                }
            }
        }

        if (applied)
        {
            // Must re-compile
            derived->compile();
        }

        return applied;
    }
    //////////////////////////////////////////////////////////////////////////
    // class EntityTransparentor
    //////////////////////////////////////////////////////////////////////////
    size_t EntityTransparentor::addMaterial(const Ogre::MaterialPtr& material)
    {
        size_t index = 0;
        MaterialInstanceList::const_iterator it, itend;
        itend = mMaterialInstances.end();
        for (it = mMaterialInstances.begin(); it != itend; ++it, ++index)
        {
            MaterialInstance* materialInstance = *it;
            if (materialInstance->getOrigin() == material)
                return index;
        }

        mMaterialInstances.push_back(new MaterialInstance(material));
        return index;
    }
    //////////////////////////////////////////////////////////////////////////
    EntityTransparentor::EntityTransparentor(void)
        : mEntity()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    EntityTransparentor::EntityTransparentor(Ogre::Entity* entity)
        : mEntity()
    {
        init(entity);
    }
    //////////////////////////////////////////////////////////////////////////
    EntityTransparentor::~EntityTransparentor()
    {
        clear();
    }
    //////////////////////////////////////////////////////////////////////////
    void EntityTransparentor::clear(void)
    {
        MaterialInstanceList::const_iterator it, itend;
        itend = mMaterialInstances.end();
        for (it = mMaterialInstances.begin(); it != itend; ++it)
        {
            MaterialInstance* materialInstance = *it;
            delete materialInstance;
        }
        mMaterialInstances.clear();
        mSubEntityMaterialIndices.clear();
    }
    //////////////////////////////////////////////////////////////////////////
    void EntityTransparentor::init(Ogre::Entity* entity)
    {
        clear();

        mEntity = entity;

        size_t numSubEntities = mEntity->getNumSubEntities();
        mSubEntityMaterialIndices.resize(numSubEntities);
        for (uint i = 0; i < numSubEntities; ++i)
        {
            Ogre::SubEntity* subEntity = mEntity->getSubEntity(i);
            const Ogre::MaterialPtr& material = subEntity->getMaterial();
            size_t index = addMaterial(material);
            mSubEntityMaterialIndices[i] = index;
        }
    }
    //////////////////////////////////////////////////////////////////////////
    void EntityTransparentor::apply(Real transparency)
    {
        assert(mEntity);

        size_t numSubEntities = mEntity->getNumSubEntities();
        if (transparency >= 1)
        {
            // Complete transparency, just as invisible
            mEntity->setVisible(false);
        }
        else if (transparency <= 0)
        {
            // Complete opaque, just as origin
            mEntity->setVisible(true);
            for (uint i = 0; i < numSubEntities; ++i)
            {
                Ogre::SubEntity* subEntity = mEntity->getSubEntity(i);
                subEntity->setMaterialName(mMaterialInstances[mSubEntityMaterialIndices[i]]->getOrigin()->getName());
            }
        }
        else
        {
            // Derive transparency material first
            MaterialInstanceList::const_iterator it, itend;
            itend = mMaterialInstances.end();
            for (it = mMaterialInstances.begin(); it != itend; ++it)
            {
                MaterialInstance* materialInstance = *it;
                applyTransparency(materialInstance->getOrigin(), materialInstance->getDerived(), transparency);
            }

            // Applies derived transparency material to sub-entities.
            mEntity->setVisible(true);
            for (uint i = 0; i < numSubEntities; ++i)
            {
                Ogre::SubEntity* subEntity = mEntity->getSubEntity(i);
                subEntity->setMaterialName(mMaterialInstances[mSubEntityMaterialIndices[i]]->getDerived()->getName());
            }
        }
    }

}
