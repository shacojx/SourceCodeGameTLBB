#include "AddLightObjectPlugin.h"
#include "SceneManipulator.h"

#include "Core/WXLightObject.h"
#include "Core/WXObjectProxy.h"
#include "Core/WXSceneInfo.h"
#include "Core/TerrainData.h"

#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreCamera.h>

namespace WX {

    class AddLightObjectPlugin::Indicator
    {
    public:
        Indicator(const ObjectPtr &object, Ogre::SceneManager *sceneMgr, WX::SceneManipulator *sceneManipulator)
			: mProxy(NULL), mOldPos(Ogre::Vector3::ZERO)
		{
            assert(sceneMgr && sceneManipulator);

			mProxy = new ObjectProxy(object);

            mSceneManipulator = sceneManipulator;

            mCurrentLightEntity = NULL;

            // 根据光源位置来定节点位置
            Ogre::Vector3 pos = VariantCast<Ogre::Vector3>(object->getProperty("position"));
            mIndicatorSceneNode = sceneManipulator->getIndicatorRootSceneNode()->createChildSceneNode(pos);
       //     mIndicatorSceneNode->scale(2.0,15.0,2.0);

            //  创建三个entity分别来代表三种光源的模型
            mPointLightEntity = sceneMgr->createEntity(mIndicatorSceneNode->getName()+"point","pointLight.mesh");
            mDirectionalLightEntity = sceneMgr->createEntity(mIndicatorSceneNode->getName()+"directional","directionalLight.mesh");
            mDirectionalLightEntity->setQueryFlags(0);

            mSpotLightEntity = sceneMgr->createEntity(mIndicatorSceneNode->getName()+"spotlight","spotLight.mesh");

            mPointLightEntity->setUserObject(mProxy);
            mDirectionalLightEntity->setUserObject(mProxy);
            mSpotLightEntity->setUserObject(mProxy);

            // 根据光源类型来挂接模型
            setIndicatorModel(mProxy->getObject()->getPropertyAsString("type"));

            // 根据光源方向来决定scenenode方向
            Ogre::Vector3 &dir = VariantCast<Ogre::Vector3>(object->getProperty("direction"));
            setDirection(dir);

            showIndicator(false);
        }

        ~Indicator()
        {
            if (mIndicatorSceneNode)
            {
                mIndicatorSceneNode->destroy();
            }
            if (mPointLightEntity)
            {
                mPointLightEntity->destroy();
            }
            if (mDirectionalLightEntity)
            {
                mDirectionalLightEntity->destroy();
            }
            if (mSpotLightEntity)
            {
                mSpotLightEntity->destroy();
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

            assert( mCurrentLightEntity && mDirectionalLightEntity );

            // 如果是方向光
            if ( mCurrentLightEntity == mDirectionalLightEntity )
            {
                mIndicatorSceneNode->setVisible(show);
                mIndicatorSceneNode->showBoundingBox(false);

                // 获取到摄像机与地形的交点
                Ogre::Vector3 pos;
                Ogre::Ray cameraRay( mSceneManipulator->getCamera()->getPosition(), mSceneManipulator->getCamera()->getDirection() );
                bool hit = mSceneManipulator->getTerrainIntersects(cameraRay, pos);

                if (hit)
                {
                    // 在地形上的一米处出现光源指示器
                    float height = mSceneManipulator->getTerrainData()->getHeightAt(pos.x, pos.z) + 100.0f;
                    mIndicatorSceneNode->setPosition(pos.x, height, pos.z);
                }
            }
            else
            {
                mIndicatorSceneNode->showBoundingBox(show);
            }
        }

        void setPosition( const Ogre::Vector3 &pos )
        {
            assert(mIndicatorSceneNode);
			mOldPos = pos;
            if ( mCurrentLightEntity != mDirectionalLightEntity )
                mIndicatorSceneNode->setPosition(pos);
        }

        void setDirection( const Ogre::Vector3 &dir )
        {
            assert(mIndicatorSceneNode);

            // 分别计算出节点三个轴上的方向
            Ogre::Vector3 yAdjustVec = -dir;
            yAdjustVec.normalise();
           
            Ogre::Vector3 xVec = mIndicatorSceneNode->getOrientation().zAxis().crossProduct( yAdjustVec );
            xVec.normalise();

            Ogre::Vector3 zVec = xVec.crossProduct( yAdjustVec );
            zVec.normalise();        
            mIndicatorSceneNode->setOrientation(Ogre::Quaternion( xVec, yAdjustVec, zVec ));     
        }

        void setIndicatorModel( const Ogre::String &lightType )
        {
            assert( !lightType.empty() );

            assert ( mIndicatorSceneNode );
            assert ( mPointLightEntity );
            assert ( mDirectionalLightEntity );
            assert ( mSpotLightEntity );

            mIndicatorSceneNode->detachAllObjects();

            if ( lightType == "point" )
            {
              //  mIndicatorSceneNode->attachObject(mPointLightEntity);
                mCurrentLightEntity = mPointLightEntity;
				if ( mOldPos != Ogre::Vector3::ZERO )
					mIndicatorSceneNode->setPosition(mOldPos);
            }	
            else if ( lightType == "directional")
            {
             //   mIndicatorSceneNode->attachObject(mDirectionalLightEntity);
				// 因为方向光本身没有位置属性，不过指示器会出现在地形中心，会改变scene node的位置
				// 所以这里先把旧位置保存下来
                mOldPos = mIndicatorSceneNode->getPosition();
                mCurrentLightEntity = mDirectionalLightEntity;
            //    mIndicatorSceneNode->setPosition(0, 0 ,0);

                // 获取到地形中心点的高度
                float height = mSceneManipulator->getTerrainData()->getHeightAt(0,0);
                mIndicatorSceneNode->setPosition(0, height ,0);
            }
            else if ( lightType == "spotlight" )
            {
             //   mIndicatorSceneNode->attachObject(mSpotLightEntity);
                mCurrentLightEntity = mSpotLightEntity;
				if ( mOldPos != Ogre::Vector3::ZERO )
					mIndicatorSceneNode->setPosition(mOldPos);
            }

            mIndicatorSceneNode->attachObject(mCurrentLightEntity);
        }

    protected:

        Ogre::Entity *mPointLightEntity;
        Ogre::Entity *mDirectionalLightEntity;
        Ogre::Entity *mSpotLightEntity;

        Ogre::Entity *mCurrentLightEntity;

        Ogre::SceneNode *mIndicatorSceneNode;

        WX::SceneManipulator *mSceneManipulator;
		ObjectProxy* mProxy;

		Ogre::Vector3 mOldPos;
    };

    AddLightObjectPlugin::AddLightObjectPlugin(WX::SceneManipulator* sceneManipulator)
    {
        assert(sceneManipulator);
    
        mSceneManipulator = sceneManipulator;
        mSceneManipulator->addSceneListener(this);
    }

    AddLightObjectPlugin::~AddLightObjectPlugin()
    {
        mSceneManipulator->removeSceneListener(this);

        clearIndicators();
    }

    //////////////////////////////////////////////////////////////////////////
    void
    AddLightObjectPlugin::onSceneReset(void)
    {
        clearIndicators();

        typedef WX::SceneInfo::Objects Objects;
        const WX::SceneInfo* sceneInfo = mSceneManipulator->getSceneInfo();
        const Objects& objects = sceneInfo->getObjects();
        for (Objects::const_iterator it = objects.begin(); it != objects.end(); ++it)
        {
            Ogre::String type = (*it)->getType();

            // 判断，如果是能处理的类型（LightObject），就处理
            if ( type == "Light" )
            {
                LightObject *lightObject = static_cast<LightObject *> ((*it).get());
                Indicator *indicator = new Indicator( *it, mSceneManipulator->getSceneManager(),
                    mSceneManipulator );

                std::pair<Indicators::iterator, bool> inserted =
                    mIndicators.insert(Indicators::value_type(*it, indicator));

                assert(inserted.second);
            }
        }
    }

    void 
    AddLightObjectPlugin::onAddObject(const ObjectPtr& object)
    {
        Ogre::String type = object->getType();

        // 判断，如果是能处理的类型（LightObject），就处理
        if ( type == "Light" )
        {
            LightObject *lightObject = static_cast<LightObject *> (object.get());
            Indicator *indicator = new Indicator(object,mSceneManipulator->getSceneManager(),
                mSceneManipulator );

            std::pair<Indicators::iterator, bool> inserted =
                mIndicators.insert(Indicators::value_type(object, indicator));

            assert(inserted.second);
        }
    }

    void 
    AddLightObjectPlugin::onRemoveObject(const ObjectPtr& object)
    {
        Ogre::String type = object->getType();

        // 判断，如果是能处理的类型（LightObject），就处理
        if ( type == "Light" )
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
        AddLightObjectPlugin::onRenameObject(const ObjectPtr& object, const String& oldName)
    {
    }*/

    void 
    AddLightObjectPlugin::onSelectObject(const ObjectPtr& object)
    {
        Ogre::String type = object->getType();

        // 判断，如果是能处理的类型（LightObject），就处理
        if ( type == "Light" )
        {
            Indicators::iterator i = mIndicators.find(object);

            if ( i != mIndicators.end() )
            {
                i->second->showIndicator(true);
            }
        }

    }

    void 
    AddLightObjectPlugin::onDeselectObject(const ObjectPtr& object)
    {
        Ogre::String type = object->getType();

        // 判断，如果是能处理的类型（LightObject），就处理
        if ( type == "Light" )
        {
            Indicators::iterator i = mIndicators.find(object);

            if ( i != mIndicators.end() )
            {
                i->second->showIndicator(false);
            }
        }
    }

    void 
    AddLightObjectPlugin::onDeselectAllObjects(void)
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
    AddLightObjectPlugin::onObjectPropertyChanged(const ObjectPtr& object, const String& name)
    {
        Ogre::String type = object->getType();

        // 判断，如果是能处理的类型（LightObject），就处理
        if ( type == "Light" )
        {
            Indicators::iterator i = mIndicators.find(object);

            if ( i != mIndicators.end() )
            {
                if ( name == "position" )
                   i->second->setPosition( VariantCast<Ogre::Vector3>(object->getProperty("position")) );
                else if ( name == "type" )
                {
                    Ogre::String lightType = object->getPropertyAsString("type");
                    i->second->setIndicatorModel(lightType);
                }
                else if ( name == "direction" )
                {
                    i->second->setDirection( VariantCast<Ogre::Vector3>(object->getProperty("direction")) );
                }
            }
        }

    }

    void 
	AddLightObjectPlugin::clearIndicators(void)
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
	}
}