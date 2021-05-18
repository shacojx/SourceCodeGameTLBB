/********************************************************************
filename:   WXLogicModelManager.h

purpose:    用于解析，创建并管理LogicModel。
*********************************************************************/

#ifndef _FAIRYLOGICMODELMANAGER_H_
#define _FAIRYLOGICMODELMANAGER_H_

#include <OgreSingleton.h>
#include <OgreScriptLoader.h>
#include <OgreStringVector.h>
#include <OgreDataStream.h>
#include <OgreIteratorWrappers.h>
#include <OgreResourceGroupManager.h>
#include <OgreAnimation.h>
#include <OgreMaterial.h>

// fairy header
#include "Core/WXPrerequisites.h"

namespace Ogre  {

    class CollisionManager;
    class UserCollider;
};

class TiXmlNode;

namespace WX	{

	class System;
    class TerrainData;
	class LogicModel;
    class TerrainCollider;        

	class LogicModelManager : public Ogre::Singleton<LogicModelManager>, Ogre::ScriptLoader
	{
    public:

        typedef std::map<Ogre::String, LogicModel *> LogicModelMap;

        /// 骨骼与动作之间的关联map，key为动作名称，value为包含这个动作的骨骼文件名称
        typedef std::map<String, String> AnimationSkeletonLinks;
        /// key为logic model的名称，如 女主角.obj
        typedef std::map<String, AnimationSkeletonLinks*> AnimationSkeletonLinksMap;

        typedef unsigned long MaterialHandle;
        typedef std::list<MaterialHandle> MaterialPool;

        struct MaterialCache
        {
            MaterialCache(const Ogre::MaterialPtr& mat, MaterialPool::iterator it)
                : mMaterial(mat)
                , mPoolIterator(it)
            {
            }

            Ogre::MaterialPtr mMaterial;
            MaterialPool::iterator mPoolIterator;
        };

        typedef std::map<MaterialHandle, MaterialCache> MaterialCaches;

	public:

		LogicModelManager( WX::System *system );
		~LogicModelManager();

		/// @copydoc ScriptLoader::getScriptPatterns
		const Ogre::StringVector& getScriptPatterns(void) const;
		/// @copydoc ScriptLoader::parseScript
		void parseScript(Ogre::DataStreamPtr& stream, const Ogre::String& groupName);
		/// @copydoc ScriptLoader::getLoadingOrder
		Ogre::Real getLoadingOrder(void) const;

		static LogicModelManager& getSingleton(void);       
		static LogicModelManager* getSingletonPtr(void);	

		LogicModel* createLogicModel( const String& modelName );

		LogicModel* createLogicModel( const String& name, const String& modelName );

		void destroyLogicModel(const String& name);
		void destroyLogicModel(LogicModel* model);

        /// 删除所有已经创建出来的LogicModel
        void destroyAllLogicModels(void);

        /// 删除所有LogicModel template
        void destroyAllLogicModelTemplates(void);

		const LogicModel * getTemplateByName(const String& modelName) const;

		LogicModel * getModelByName(const String& modelName);	

		void updateAllLogicModel(Real time);

        /// 设置全局的是否可以投影
        void setShadowCastable(bool castable);
        bool getShadowCastable(void)
        {
            return mShadowCastable;
        }

        /// 设置全局的行走特效开关
        void setEnableWalkingEffect(bool enable);
        bool getEnableWalkingEffect(void)
        {
            return mEnableWalkingEffect;
        }

        /** 读取指定obj的资源
        @param modelName 要读取的obj名称
        @param groupName 在哪个资源组中读取资源，如果没有提供，就自动检测资源组
        @remarks 这个操作并不会真正地创建Logic Model的实例，只是把
                 obj所用到的mesh，贴图，skeleton等资源都读取进来
        */
        void loadModelResource(const String& modelName, 
            const String& groupName = Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);

        Ogre::CollisionManager* getCollisionManager(void);

        Ogre::UserCollider* getTerrainCollider(void);

        /** 获取指定动作所在的骨骼名称
        @param modelName 模型名称
        @param animName 所指定的动作名称
        */
        const String& getSkeletonFromAnimationName(const String& modelName, const String& animName);

        /// 获取所指定的模型的动作名称迭代器
        typedef Ogre::ConstMapIterator<AnimationSkeletonLinks> AnimationSkeletonLinksIterator;
        AnimationSkeletonLinksIterator getAnimationSkeletonLinksIterator(const String& modelName);

        /// model模板迭代器
        typedef Ogre::MapIterator<LogicModelMap> LogicModelTemplateIterator;
        LogicModelTemplateIterator getLogicModelTemplateIterator(void)
        {
            return LogicModelTemplateIterator(
                mLogicModelTemplateMap.begin(), mLogicModelTemplateMap.end());
        }

        void _clearAnimationSkeletonLinks(void);

        void setAnimationInterpolationMode(Ogre::Animation::InterpolationMode mode)
        {
            mInterpolationMode = mode;
        }
        Ogre::Animation::InterpolationMode getAnimationInterpolationMode(void)
        {
            return mInterpolationMode;
        }

        void addToMaterialPool(MaterialHandle handle);

        Ogre::Entity* _createSkeletalEntity(const Ogre::SkeletonPtr& skeleton);
        void _destroySkeletalEntity(Ogre::Entity* entity);
        void _cleanupSkeletalEntityCache(void);

	protected:

		void _parseFailed( LogicModel *model, const char *errorDesc );

		void _createController(void);

		void _destroyController(void);

        /// 删除公用的terrain collider
        void _destroyTerrainCollider(void);

        /// 删除collider manager
        void _destroyColliderManager(void);

        AnimationSkeletonLinks* _createAnimationSkeletonLinksForModel(const Ogre::String& modelName);

        /// 解析单个obj的定义
        void _parseSingleObject(TiXmlNode* rootElement, const Ogre::String& objectName);

        void _removeMaterialPool(void);

	protected:

        AnimationSkeletonLinksMap mAnimationSkeletonLinksMap;

		Ogre::StringVector mScriptPatterns;

		WX::System *mSystem;

		LogicModelMap mLogicModelTemplateMap;
		LogicModelMap mLogicModelMap;

		ulong mAutoNamedModel;

		Ogre::Controller<Real>* mController;

        /// 是否可以投影
        bool mShadowCastable;

        /// 是否打开行走特效
        bool mEnableWalkingEffect;

        Ogre::CollisionManager* mCollisionManager;
        Ogre::UserCollider* mTerrainCollider;

        Ogre::Animation::InterpolationMode mInterpolationMode;

        MaterialPool mMaterialPool;
        MaterialCaches mMaterialCaches;

        typedef std::multimap<Ogre::Skeleton*, Ogre::Entity*> SkeletalEntityCache;
        SkeletalEntityCache mSkeletalEntityCache;
	};

};


#endif