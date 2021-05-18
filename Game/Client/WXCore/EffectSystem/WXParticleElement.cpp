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

        // 真正地进行颜色的改变
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

			// 消除MeshParticleRenderer残留的粒子
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

		// 先不激活emitter，因为现在可能还没到start time，如果现在激活，会使粒子播放时间减少，应在到start time
		// 时才激活
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
                    // 中级特效，quota为原来的50%，特效不迭代
                    size_t newQuota = (size_t)(pTemplate->getParticleQuota() * 0.5);

                    if (newQuota == 0)
                        newQuota = 1;

                    mParticleSystem->setParticleQuota(newQuota);

                    mParticleSystem->setIterationInterval(0.0f);
                }
                else if (effectLevel == EL_LOW)
                {
                    // 中级特效，quota为原来的10%，特效不迭代
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
        // 改变colour fader影响器中的颜色
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

                        // 如果之前已经保存过原来的颜色，就直接获取
                        if (i != mColourMap.end())
                        {
                            originColour = i->second;
                        }
                        else
                        {
                            originColour = 
                                Ogre::StringConverter::parseColourValue( affector->getParameter(paraName) );

                            /// 保存原来的颜色
                            mColourMap.insert( ColourMap::value_type(paraName, originColour) );
                        }

                        /// 用原来的颜色与要设定的颜色进行相乘
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
                // 如果要重新显示,就把粒子重新挂接上去
                if (!mParticleSystem->getParentNode() && mBasicNode)
                {
                    mBasicNode->attachObject(mParticleSystem);
                }
            }
            else
            {
                // 如果要隐藏这个粒子系统,要先把它从父节点上解挂,这样,就不会调用粒子系统的update()
                if (mParticleSystem->getParentNode() &&
                    mParticleSystem->getParentNode() == mParticleSystem->getParentSceneNode())
                {
                    mParticleSystem->getParentSceneNode()->detachObject(mParticleSystem);
                }

                // 对于mesh粒子,要隐藏它生成的mesh particle
                Ogre::ParticleSystemRenderer *renderer = mParticleSystem->getRenderer();

                assert (renderer);

                if (renderer->getType() == "mesh")
                {
                    static_cast<Ogre::MeshParticleRenderer *>(renderer)->removeAllMeshParticle();
                }
            }
        }

        // 要先执行上面的detach和attach之后，才来设置scene node的visible，因为这时候particle system
        // 才挂在scene node上，set visible才会对它进行设置
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