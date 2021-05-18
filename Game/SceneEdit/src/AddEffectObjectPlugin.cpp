#include "AddEffectObjectPlugin.h"
#include "SceneManipulator.h"
#include "Core/WXEffectObject.h"
#include "Core/WXObjectProxy.h"
#include "Core/WXSceneInfo.h"
#include "Core/WXUtils.h"
#include "EffectSystem/WXEffect.h"

// ogre header
#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

namespace WX	{

	class AddEffectObjectPlugin::Indicator
    {
    public:
        Indicator(const ObjectPtr &object, SceneManipulator *sceneManipulator)
			: mProxy(NULL)
        {
            assert(sceneManipulator);

			mProxy = new ObjectProxy(object);

            // 根据光源位置来定节点位置
            Ogre::Vector3 pos = VariantCast<Ogre::Vector3>(object->getProperty("position"));
            mIndicatorSceneNode = sceneManipulator->getIndicatorRootSceneNode()->createChildSceneNode(pos);

            Real radius = 5;
            int rings = 16;
            int segments = 16;

            Ogre::MeshPtr sphereMesh = createCommonSphere(mIndicatorSceneNode->getName(), radius, rings, segments);
            Ogre::MaterialPtr material = createPureColourMaterial(
                Ogre::ColourValue(1, 0, 0, 0.75)   );

            mIndicatorEntity = sceneManipulator->getSceneManager()->createEntity(mIndicatorSceneNode->getName(), sphereMesh->getName());
            mIndicatorEntity->setNormaliseNormals(true);
            mIndicatorEntity->setMaterialName(material->getName());
            mIndicatorEntity->setUserObject(mProxy);

            mIndicatorSceneNode->attachObject(mIndicatorEntity);

            // 选择时不考虑粒子系统的包围盒，用的是指示器的包围盒
    //        EffectObject *effectObject = static_cast<EffectObject *> (object.get());
     //       effectObject->getParticleSystem()->setQueryFlags(0);

            // 根据光源类型来挂接模型
            showIndicator(false);
        }

        ~Indicator()
        {
            if (mIndicatorSceneNode)
            {
                mIndicatorSceneNode->destroy();
            }
            if (mIndicatorEntity)
            {
                mIndicatorEntity->destroy();
            }

			if (mProxy)
			{
				delete mProxy;
				mProxy = NULL;
			}
        }

        void showIndicator( bool show )
        {
            assert(mIndicatorSceneNode);           
            
            mIndicatorSceneNode->showBoundingBox(show);
        }

        void setPosition( const Ogre::Vector3 &pos )
        {
            assert(mIndicatorSceneNode);

            mIndicatorSceneNode->setPosition(pos);
        }    
       
    protected:

        Ogre::Entity *mIndicatorEntity;

        Ogre::SceneNode *mIndicatorSceneNode;

		ObjectProxy* mProxy;
    };

    AddEffectObjectPlugin::AddEffectObjectPlugin(WX::SceneManipulator* sceneManipulator)
    {
        assert(sceneManipulator);

        mSceneManipulator = sceneManipulator;
        mSceneManipulator->addSceneListener(this);
    }

    AddEffectObjectPlugin::~AddEffectObjectPlugin()
    {
        mSceneManipulator->removeSceneListener(this);

        for (Indicators::iterator i = mIndicators.begin(); i != mIndicators.end(); ++i )
        {
            if ( i->second )
            {
                delete i->second;
                i->second = NULL;
            }
        }
		mIndicators.clear();
    }

    //////////////////////////////////////////////////////////////////////////
    void
        AddEffectObjectPlugin::onSceneReset(void)
    {
        for (Indicators::iterator i = mIndicators.begin(); i != mIndicators.end(); ++i )
        {
            if ( i->second )
            {
                delete i->second;
                i->second = NULL;
            }
        }
		mIndicators.clear();

        typedef WX::SceneInfo::Objects Objects;
        const WX::SceneInfo* sceneInfo = mSceneManipulator->getSceneInfo();
        const Objects& objects = sceneInfo->getObjects();
        for (Objects::const_iterator it = objects.begin(); it != objects.end(); ++it)
        {
            Ogre::String type = (*it)->getType();

            // 判断，如果是能处理的类型（ParticleSystemObject），就处理
            if ( type == "Effect" )
            {
                EffectObject *effectObject = static_cast<EffectObject *> ((*it).get());
                Indicator *indicator = new Indicator( *it, mSceneManipulator);

                std::pair<Indicators::iterator, bool> inserted =
                    mIndicators.insert(Indicators::value_type(*it, indicator));

                assert(inserted.second);
            }
        }
    }

    void 
        AddEffectObjectPlugin::onAddObject(const ObjectPtr& object)
    {
        Ogre::String type = object->getType();

        // 判断，如果是能处理的类型（ParticleSystemObject），就处理
        if ( type == "Effect" )
        {
            EffectObject *effectObject = static_cast<EffectObject *> (object.get());
            Indicator *indicator = new Indicator(object, mSceneManipulator);

            std::pair<Indicators::iterator, bool> inserted =
                mIndicators.insert(Indicators::value_type(object, indicator));

            assert(inserted.second);
        }
    }

    void 
        AddEffectObjectPlugin::onRemoveObject(const ObjectPtr& object)
    {
        Ogre::String type = object->getType();

        // 判断，如果是能处理的类型（ParticleSystemObject），就处理
        if ( type == "Effect" )
        {
            Indicators::iterator i = mIndicators.find(object);

            if ( i != mIndicators.end() )
            {
                delete i->second;
                i->second = NULL;
                mIndicators.erase(i);                
            }
        }
    }

    /*   void 
    AddEffectObjectPlugin::onRenameObject(const ObjectPtr& object, const String& oldName)
    {
    }*/

    void 
        AddEffectObjectPlugin::onSelectObject(const ObjectPtr& object)
    {
        Ogre::String type = object->getType();

        // 判断，如果是能处理的类型（ParticleSystemObject），就处理
        if ( type == "Effect" )
        {
            Indicators::iterator i = mIndicators.find(object);

            if ( i != mIndicators.end() )
            {
                i->second->showIndicator(true);
            }
        }

    }

    void 
        AddEffectObjectPlugin::onDeselectObject(const ObjectPtr& object)
    {
        Ogre::String type = object->getType();

        // 判断，如果是能处理的类型（ParticleSystemObject），就处理
        if ( type == "Effect" )
        {
            Indicators::iterator i = mIndicators.find(object);

            if ( i != mIndicators.end() )
            {
                i->second->showIndicator(false);
            }
        }
    }

    void 
        AddEffectObjectPlugin::onDeselectAllObjects(void)
    { 
        for (Indicators::iterator i = mIndicators.begin(); i != mIndicators.end(); ++i )
        {
            if ( i->second )
            {
                i->second->showIndicator(false);
            }
        }        
    }

    void 
        AddEffectObjectPlugin::onObjectPropertyChanged(const ObjectPtr& object, const String& name)
    {
        Ogre::String type = object->getType();

        // 判断，如果是能处理的类型（ParticleSystemObject），就处理
        if ( type == "Effect" )
        {
            Indicators::iterator i = mIndicators.find(object);

            if ( i != mIndicators.end() )
            {
                if ( name == "position" )
                    i->second->setPosition( VariantCast<Ogre::Vector3>(object->getProperty("position")) );                
            }
        }
    }
}