/********************************************************************
filename:   FairyParticleElement.cpp

purpose:    a particle system effect element.
*********************************************************************/

#include "WXParticleElement.h"
#include "WXEffectManager.h"

// ogre header
#include <OgreParticleSystem.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreStringConverter.h>
#include <OgreParticleEmitter.h>
#include <OgreParticleAffector.h>
#include <OgreParticleSystemManager.h>

#include "OgreExt/OgreMeshParticleRenderer.h"

namespace WX	{

	ParticleElement::CmdParticleSystem ParticleElement::msParticleSystemCmd;

	ParticleElement::ParticleElement(const String &type, System *system) :
	EffectElement(type, system),
	mParticleSystem(NULL),
	mParticleSystemName(""),
	mIsEmitterEnabled(false),
    mNeedUpdateColour(false),
    mCurrentColour(Ogre::ColourValue::White)
	{
		if ( initDefaults(mElementType) )
		{
			Ogre::ParamDictionary* dict = getParamDictionary();

        // Custom params      
			dict->addParameter(Ogre::ParameterDef("ParticleSystem", 
			  "the name of particle system.",
				Ogre::PT_STRING),&msParticleSystemCmd);      
		}
	}
	//---------------------------------------------------------------------
	ParticleElement::~ParticleElement()
	{
		destroyRenderInstance();
	}	
	//---------------------------------------------------------------------
	bool ParticleElement::createRenderInstance(void)
	{
		return _createParticleSystem();
	}
	//---------------------------------------------------------------------
	void ParticleElement::destroyRenderInstance(void)
	{
		_destroyParticleSystem();

		EffectElement::destroyRenderInstance();
	}
	//---------------------------------------------------------------------
	void ParticleElement::updateRenderInstance( Real time, TransformInfo &info )
	{
		if ( false == mIsEmitterEnabled )
		{
			if (mParticleSystem)
			{
				for ( unsigned short i = 0; i < mParticleSystem->getNumEmitters(); ++i )
				{
					Ogre::ParticleEmitter* emitter = mParticleSystem->getEmitter(i);

					if (emitter && false == emitter->getEnabled())
						emitter->setEnabled(true);
				}
			}

			mIsEmitterEnabled = true;
		}

        // ????????????????????
        if (mNeedUpdateColour)
        {
            _setAffectorColour();
            mNeedUpdateColour = false;
        }
	} 
	//---------------------------------------------------------------------
	void ParticleElement::destroyMe(void)
	{
		EffectElement::destroyMe();
	//	if (mParticleSystem)
	//		mParticleSystem->removeAllEmitters();
	}
	//---------------------------------------------------------------------
	void ParticleElement::sleep(void)
	{
		if (mParticleSystem)
		{
            // detach from parent first
            if (mParticleSystem->getParentNode() &&
                mParticleSystem->getParentNode() == mParticleSystem->getParentSceneNode())
            {
                mParticleSystem->getParentSceneNode()->detachObject(mParticleSystem);
            }

			for ( unsigned short i = 0; i < mParticleSystem->getNumEmitters(); ++i )
			{
				Ogre::ParticleEmitter* emitter = mParticleSystem->getEmitter(i);

				if (emitter && emitter->getEnabled())
					emitter->setEnabled(false);
			}

			mIsEmitterEnabled = false;

			mParticleSystem->clear();

			// ????MeshParticleRenderer??????????
			Ogre::ParticleSystemRenderer *renderer = mParticleSystem->getRenderer();

			assert (renderer);

			if (renderer->getType() == "mesh")
			{
				static_cast<Ogre::MeshParticleRenderer *>(renderer)->removeAllMeshParticle();
			}
		}

        mCurrentColour = Ogre::ColourValue::White;
        mNeedUpdateColour = false;

		EffectElement::sleep();		
	}
	//---------------------------------------------------------------------
	void ParticleElement::active(void)
	{
        // attach to basic scene node in case if wake up from sleeping
        if (mParticleSystem && !mParticleSystem->getParentNode() && mBasicNode)
        {
            mBasicNode->attachObject(mParticleSystem);
        }

		EffectElement::active();

		// ????????emitter????????????????????start time????????????????????????????????????????????start time
		// ????????
		/*if (mParticleSystem)
		{
			for ( unsigned short i = 0; i < mParticleSystem->getNumEmitters(); ++i )
			{
				mParticleSystem->getEmitter(i)->setEnabled(true);
			}
		}*/
	}	
	//---------------------------------------------------------------------
	void ParticleElement::setParticleSystemName(const String &name)
	{	
		mParticleSystemName = name;

		_destroyParticleSystem();

		mIsRenderInstanceCreated = false;
	}		
	//---------------------------------------------------------------------
	bool ParticleElement::_createParticleSystem(void)
	{
		assert( mParticleSystem == NULL );
		//assert( false == mParticleSystemName.empty() );

		if ( false == mParticleSystemName.empty() )
		{
			Ogre::SceneNode* parent = mBasicNode;

			if (parent)
			{
                mParticleSystem = parent->getCreator()->createParticleSystem( parent->getName(), mParticleSystemName );
                assert(mParticleSystem);

                Ogre::ParticleSystem* pTemplate = Ogre::ParticleSystemManager::getSingleton().getTemplate( mParticleSystemName );

                EffectManager& effectMgr = EffectManager::getSingleton();

                EffectLevel effectLevel = effectMgr.getEffectLevel();

                if (effectLevel == EL_MID)
                {
                    // ??????????quota????????50%????????????
                    size_t newQuota = (size_t)(pTemplate->getParticleQuota() * 0.5);

                    if (newQuota == 0)
                        newQuota = 1;

                    mParticleSystem->setParticleQuota(newQuota);

                    mParticleSystem->setIterationInterval(0.0f);
                }
                else if (effectLevel == EL_LOW)
                {
                    // ??????????quota????????10%????????????
                    size_t newQuota = (size_t)(pTemplate->getParticleQuota() * 0.1);

                    if (newQuota == 0)
                        newQuota = 1;

                    mParticleSystem->setParticleQuota(newQuota);

                    mParticleSystem->setIterationInterval(0.0f);
                }

                mParticleSystem->setRenderQueueGroup(Ogre::RENDER_QUEUE_8);

                mParticleSystem->setCastShadows(false);

                mParticleSystem->setVisible(mVisible);

				parent->attachObject(mParticleSystem);

				return true;
			}
		}       

		return false;
	}
	//---------------------------------------------------------------------
	void ParticleElement::_destroyParticleSystem(void)
	{
		if (mParticleSystem)
		{
			mParticleSystem->destroy();
			mParticleSystem = NULL;
		} 
	}
	//---------------------------------------------------------------------
	bool ParticleElement::existVisualElements(void)
	{
		if (mParticleSystem)
		{
			return (mParticleSystem->getNumParticles() > 0);
		}
		else
			return false;
	}
	//---------------------------------------------------------------------
	void ParticleElement::shutdown(void)
	{
		if (mParticleSystem)
		{
			for ( unsigned short i = 0; i < mParticleSystem->getNumEmitters(); ++i )
			{
				Ogre::ParticleEmitter* emitter = mParticleSystem->getEmitter(i);

				if (emitter && emitter->getEnabled())
					emitter->setEnabled(false);
			}
		}
	}
    //---------------------------------------------------------------------
    void ParticleElement::setColour( const Ogre::ColourValue& colour )
    {
        mCurrentColour = colour;
        mNeedUpdateColour = true;        
    }
    //---------------------------------------------------------------------
    void ParticleElement::_setAffectorColour(void)
    {
        // ????colour fader??????????????
        if (mParticleSystem)
        {
            for ( unsigned short i = 0; i < mParticleSystem->getNumAffectors(); ++i )
            {
                Ogre::ParticleAffector* affector = mParticleSystem->getAffector(i);

                assert (affector);
                if (affector->getType() == "ColourFading")
                {
                    for ( unsigned short num = 0; num < 6; ++num )
                    {
                        String paraName = "colour" + Ogre::StringConverter::toString(num);

                        ColourMap::iterator i = mColourMap.find(paraName);

                        Ogre::ColourValue originColour;

                        // ????????????????????????????????????????
                        if (i != mColourMap.end())
                        {
                            originColour = i->second;
                        }
                        else
                        {
                            originColour = 
                                Ogre::StringConverter::parseColourValue( affector->getParameter(paraName) );

                            /// ??????????????
                            mColourMap.insert( ColourMap::value_type(paraName, originColour) );
                        }

                        /// ??????????????????????????????????
                        affector->setParameter( paraName, 
                            Ogre::StringConverter::toString(originColour * mCurrentColour) );
                    }

                }
            }
        }
    }
    //-----------------------------------------------------------------------
    void ParticleElement::setVisibleFlag(uint32 flags)
    {
        if (mParticleSystem)
            mParticleSystem->setVisibilityFlags(flags);
    }
    //-----------------------------------------------------------------------
    void ParticleElement::setVisible(bool visible)
    {
        if (mParticleSystem)
        {
            if (visible)
            {
                // ??????????????,????????????????????
                if (!mParticleSystem->getParentNode() && mBasicNode)
                {
                    mBasicNode->attachObject(mParticleSystem);
                }
            }
            else
            {
                // ??????????????????????,??????????????????????,????,????????????????????update()
                if (mParticleSystem->getParentNode() &&
                    mParticleSystem->getParentNode() == mParticleSystem->getParentSceneNode())
                {
                    mParticleSystem->getParentSceneNode()->detachObject(mParticleSystem);
                }

                // ????mesh????,??????????????mesh particle
                Ogre::ParticleSystemRenderer *renderer = mParticleSystem->getRenderer();

                assert (renderer);

                if (renderer->getType() == "mesh")
                {
                    static_cast<Ogre::MeshParticleRenderer *>(renderer)->removeAllMeshParticle();
                }
            }
        }

        // ??????????????detach??attach??????????????scene node??visible????????????particle system
        // ??????scene node????set visible????????????????
        EffectBasic::setVisible(visible);        
    }
    ///////////////////////////////////////////
	String ParticleElement::CmdParticleSystem::doGet(const void* target) const
    {
        return static_cast<const ParticleElement*>(target)->getParticleSystemName();
    }
    void ParticleElement::CmdParticleSystem::doSet(void* target, const String& val)
    {
        static_cast<ParticleElement *>(target)->setParticleSystemName(val);
    }  
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void ParticleElement::CmdParticleSystem::doCopy(void* target, const void* source)
    {
        static_cast<ParticleElement *>(target)->setParticleSystemName(
            static_cast<const ParticleElement*>(source)->getParticleSystemName() );
    }
#endif
}