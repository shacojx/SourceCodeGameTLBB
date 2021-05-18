/********************************************************************
filename:   WXLogicModel.cpp

purpose:    LogicModel类是用于替换原来DObject的，其主要作用是渲染层与
客户端的一个接合层，利用渲染层，实现客户端对场景中
动态物体的一些需求（如换装，挂特效等），尽量是客户端可以很
方便地实现各种效果，而不需调用渲染层的代码。
*********************************************************************/

#include "WXLogicModel.h"

#include "Core/WXSystem.h"
#include "Core/WXUtils.h"
#include "Core/TerrainTypeInfos.h"
#include "Core/WXResources.h"
#include "Core/WXFlags.h"

#include "EffectSystem/WXEffectManager.h"
#include "EffectSystem/WXSkill.h"
#include "EffectSystem/WXAnimationRibbon.h"
#include "EffectSystem/WXAnimationEffectInfo.h"
#include "EffectSystem/WXAnimationSound.h"
#include "EffectSystem/WXProjectorElement.h"
#include "EffectSystem/WXAnimationSceneLightInfo.h"
#include "EffectSystem/WXMainSceneLight.h"

#include <OgreLogManager.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>
#include <OgreStringConverter.h>
#include <OgreSkeletonInstance.h>
#include <OgreBone.h>
#include <OgreSubEntity.h>
#include <OgreSubMesh.h>
#include <OgreMaterialManager.h>
#include <OgreCamera.h>
#include <OgreAxisAlignedBox.h>
#include <OgreGpuProgramManager.h>
#include <OgreAnimation.h>
#include <OgreSkeletonManager.h>

#include <ScorpioFabric.h>
#include <ScorpioCollision.h>

/// 如果define EDITOR，在execute中就会执行编辑器所需要的行为（根据滑条调整动作位置等）
//#define EDITOR
#undef  EDITOR

namespace WX	{

    OnPlaySound LogicModel::mOnPlaySound = NULL;
	OnStopSound LogicModel::mOnStopSound = NULL;
    GetHeightInWorld LogicModel::mGetHeightInWorld = NULL;

	String LogicModel::MESH_HINT = "Mesh";
	String LogicModel::MATERIAL_HINT = "Material";
	String LogicModel::SLMT_CREATE_HINT = "SlotCreate";

	String LogicModel::MOVEMENT_TYPE_ATTRIBUTE = "MovementType";
    String LogicModel::DEFAULT_ANIM_NAME = "DefaultAnimation";
    String LogicModel::BOUNDING_BOX = "BoundingBox";
    String LogicModel::SCALE_FACTOR = "Scale";

    String LogicModel::HAIR_MESH_NAME = "HairMesh";

    //////////////////////////////////////////////////////////////////////////
    //-----------------------------------------------------------------------
    const Ogre::ColourValue& _getColourFromMaterial(const Ogre::MaterialPtr& mat)
    {
        assert (false == mat.isNull());

        const Ogre::ColourValue realZero(0.0,0.0,0.0,1.0);

        for (unsigned short techIt = 0; techIt < mat->getNumTechniques(); ++techIt)
        {
            Ogre::Technique* technique = mat->getTechnique(techIt);

            for (unsigned short passIt = 0; passIt < technique->getNumPasses(); ++passIt)
            {
                Ogre::Pass* pass = technique->getPass(passIt);

                const Ogre::ColourValue& passColour = pass->getSelfIllumination();

                if (passColour != realZero &&
                    passColour != Ogre::ColourValue::ZERO)
                {
                    // 返回第一个不为0的自发光颜色
                    return passColour;
                }
            }
        }

        // 默认返回的是黑色
        return Ogre::ColourValue::Black;
    }

    LogicModel::ModelMaterialInstance::ModelMaterialInstance(const Ogre::MaterialPtr& origin) :
    mOrigin(), mDerived()
    {
        // Generate material name
        Ogre::String newName;

        // Avoid name collision
        do {
            newName = origin->getName() + Ogre::StringConverter::toString(Ogre::Math::UnitRandom());
        } while (Ogre::MaterialManager::getSingleton().resourceExists(newName));

        Ogre::AliasTextureNamePairList nameList;
        bool depthWrite = false;
        bool setName = false;
        bool alphaTest = false;

        bool colourOpAdd = false;

        Ogre::Material::TechniqueIterator dti = origin->getTechniqueIterator();
        while (dti.hasMoreElements())
        {
            Ogre::Technique* dt = dti.getNext();
            Ogre::Technique::PassIterator dpi = dt->getPassIterator();
            while (dpi.hasMoreElements())
            {
                Ogre::Pass* dp = dpi.getNext();

                if (dp->getDepthWriteEnabled())
                    depthWrite = true;

                if (dp->getAlphaRejectFunction() != Ogre::CMPF_ALWAYS_PASS)
                    alphaTest = true;

                Ogre::Pass::TextureUnitStateIterator dti = dp->getTextureUnitStateIterator();

                while (dti.hasMoreElements())
                {
                    Ogre::TextureUnitState* dt = dti.getNext();

                    if (dt->getTextureType() == Ogre::TEX_TYPE_2D && !setName)
                    {                        
                        nameList.insert(Ogre::AliasTextureNamePairList::value_type("<baseTexture>", dt->getTextureName()));

                        setName = true;
                    }

                    // 判断原材质是否用到了add叠加方法
                    if (dt->getColourBlendMode().operation == Ogre::LBX_ADD)
                        colourOpAdd = true;
                }                
            }
        }

        mDerived = Ogre::MaterialManager::getSingleton().create(newName, origin->getGroup());

        // 再clone一个originMat来作为mOrigin，这样，entity中的原材质就永远不会被改动
        do {
            newName = origin->getName() + Ogre::StringConverter::toString(Ogre::Math::UnitRandom());
        } while (Ogre::MaterialManager::getSingleton().resourceExists(newName));

        mOrigin = origin->clone(newName);

        // Clone material
        Ogre::MaterialPtr baseTransparencyMat;

        if (depthWrite && alphaTest)
            baseTransparencyMat = Ogre::MaterialManager::getSingleton().getByName("Transparent/AlphaTemplate");
        else
            baseTransparencyMat = Ogre::MaterialManager::getSingleton().getByName("Transparent/NoAlphaTemplate");

        if (baseTransparencyMat.isNull())
        {
            Ogre::LogManager::getSingleton().logMessage("can't find the transparent material! "
                "LogicModel::ModelMaterialInstance::ModelMaterialInstance");

            baseTransparencyMat = Ogre::MaterialManager::getSingleton().getByName("BaseWhite");

            if (baseTransparencyMat.isNull())
            {
                // 所找的effect不存在
                OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
                    "can't find the BaseWhite material!",
                    "LogicModel::ModelMaterialInstance::ModelMaterialInstance");
            }
        }

        baseTransparencyMat->copyDetailsTo(mDerived);

        mDerived->applyTextureAliases(nameList);

        // 由于Transparent/AlphaTemplate材质中可能会disable lighting，所以这里应该把lighting打开
        mDerived->setLightingEnabled(true);

        mDerived->setSelfIllumination( _getColourFromMaterial(origin) );

        // 头发材质的特殊设置
        if (colourOpAdd)
        {
            mDerived->getTechnique(0)->getPass(1)->getTextureUnitState(0)->setColourOperation(Ogre::LBO_ADD);
            mDerived->setDiffuse(Ogre::ColourValue::Black);
            mDerived->setAmbient(Ogre::ColourValue::Black);
        }

        mDerived->compile();
    }
    //-----------------------------------------------------------------------
    LogicModel::ModelMaterialInstance::~ModelMaterialInstance()
    {
        if (!mDerived.isNull())
        {
            Ogre::MaterialManager *mm = Ogre::MaterialManager::getSingletonPtr();
            if (mm)
            {
                mm->remove(mDerived->getHandle());
            }
            mDerived.setNull();
        }

        if (!mOrigin.isNull())
        {
            Ogre::MaterialManager *mm = Ogre::MaterialManager::getSingletonPtr();
            if (mm)
            {
                mm->remove(mOrigin->getHandle());
            }
            mOrigin.setNull();
        }
    }
    //////////////////////////////////////////////////////////////////////////

    size_t LogicModel::EntityTransparentor::addMaterial(const Ogre::MaterialPtr& material)
    {
        size_t index = 0;
        MaterialInstanceList::const_iterator it, itend;
        itend = mMaterialInstances.end();
        for (it = mMaterialInstances.begin(); it != itend; ++it, ++index)
        {
            ModelMaterialInstance* materialInstance = *it;
            if (materialInstance->mOrigin->getName() == material->getName())
                return index;
        }

        mMaterialInstances.push_back(new ModelMaterialInstance(material));
        return index;
    }
    //-----------------------------------------------------------------------
    LogicModel::EntityTransparentor::EntityTransparentor(void)
        : mEntity(NULL)
    {
    }
    //-----------------------------------------------------------------------
    LogicModel::EntityTransparentor::EntityTransparentor(Ogre::Entity* entity)
        : mEntity(NULL)
    {
        init(entity);
    }
    //-----------------------------------------------------------------------
    LogicModel::EntityTransparentor::~EntityTransparentor()
    {
        clear();
    }
    //-----------------------------------------------------------------------
    void LogicModel::EntityTransparentor::clear(void)
    {
        MaterialInstanceList::const_iterator it, itend;
        itend = mMaterialInstances.end();
        for (it = mMaterialInstances.begin(); it != itend; ++it)
        {
            ModelMaterialInstance* materialInstance = *it;
            delete materialInstance;
        }
        mMaterialInstances.clear();
        mSubEntityMaterialIndices.clear();
    }
    //-----------------------------------------------------------------------
    void LogicModel::EntityTransparentor::init(Ogre::Entity* entity)
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
    //-----------------------------------------------------------------------
    void LogicModel::EntityTransparentor::apply(Real transparency)
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
                subEntity->setMaterialName(mMaterialInstances[mSubEntityMaterialIndices[i]]->mOrigin->getName());
            }
        }
        else
        {
            // Derive transparency material first
            MaterialInstanceList::const_iterator it, itend;
            itend = mMaterialInstances.end();
            for (it = mMaterialInstances.begin(); it != itend; ++it)
            {
                ModelMaterialInstance* materialInstance = *it;
                applyTransparency(materialInstance->mOrigin, materialInstance->mDerived, transparency);
            }

            // Applies derived transparency material to sub-entities.
            mEntity->setVisible(true);
            for (uint i = 0; i < numSubEntities; ++i)
            {
                Ogre::SubEntity* subEntity = mEntity->getSubEntity(i);
                subEntity->setMaterialName(mMaterialInstances[mSubEntityMaterialIndices[i]]->mDerived->getName());
            }
        }
    }
    //-----------------------------------------------------------------------
    bool LogicModel::EntityTransparentor::applyTransparency(const Ogre::MaterialPtr& origin, const Ogre::MaterialPtr& derived, Real transparency)
    {
        using namespace Ogre;

        assert(0 <= transparency && transparency <= 1);

        Real opacity = 1 - transparency;
        bool applied = false;

        Material::TechniqueIterator dti = derived->getTechniqueIterator();
        while (dti.hasMoreElements())
        {
            Technique* dt = dti.getNext();
            Technique::PassIterator dpi = dt->getPassIterator();
            while (dpi.hasMoreElements())
            {
                Pass* dp = dpi.getNext();
                SceneBlendFactor bfSrc = dp->getSourceBlendFactor();
                SceneBlendFactor bfDst = dp->getDestBlendFactor();
                if (bfSrc == SBF_SOURCE_ALPHA && bfDst == SBF_ONE_MINUS_SOURCE_ALPHA)
                {
                    applied = true;
                    
                    const ColourValue& diffuse = dp->getDiffuse();
                    dp->setDiffuse(diffuse.r, diffuse.g, diffuse.b, opacity);
                    switch (dp->getAlphaRejectFunction())
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
                        dp->setAlphaRejectValue(static_cast<uchar>(128 * opacity));
                        break;
                    }
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
    //-----------------------------------------------------------------------
    LogicModel::FabricTransparentor::FabricTransparentor(Ogre::Fabric* fabric)
        : mFabric(fabric)
        , mMaterialInstance(NULL)
    {
        init();
    }
    //-----------------------------------------------------------------------
    LogicModel::FabricTransparentor::~FabricTransparentor()
    {
        clear();
    }
    //-----------------------------------------------------------------------
    void LogicModel::FabricTransparentor::clear(void)
    {
        delete mMaterialInstance;
    }
    //-----------------------------------------------------------------------
    void LogicModel::FabricTransparentor::init(void)
    {
        clear();

        mMaterialInstance = new ModelMaterialInstance(Ogre::MaterialManager::getSingleton().getByName(mFabric->getMaterialName()));
    }
    //-----------------------------------------------------------------------
    void LogicModel::FabricTransparentor::apply(Real transparency)
    {
        assert(mFabric);

        if (transparency >= 1)
        {
            // Complete transparency, just as invisible
            mFabric->setVisible(false);
        }
        else if (transparency <= 0)
        {
            // Complete opaque, just as origin
            mFabric->setVisible(true);
            mFabric->setMaterialName(mMaterialInstance->mOrigin->getName());
        }
        else
        {
            applyTransparency(mMaterialInstance->mOrigin, mMaterialInstance->mDerived, transparency);

            // Applies derived transparency material to sub-entities.
            mFabric->setVisible(true);
            mFabric->setMaterialName(mMaterialInstance->mDerived->getName());
        }
    }
    //-----------------------------------------------------------------------
    bool LogicModel::FabricTransparentor::applyTransparency(const Ogre::MaterialPtr& origin, const Ogre::MaterialPtr& derived, Real transparency)
    {
        using namespace Ogre;

        assert(0 <= transparency && transparency <= 1);

        Real opacity = 1 - transparency;
        bool applied = false;

        Material::TechniqueIterator dti = derived->getTechniqueIterator();
        while (dti.hasMoreElements())
        {
            Technique* dt = dti.getNext();
            Technique::PassIterator dpi = dt->getPassIterator();
            while (dpi.hasMoreElements())
            {
                Pass* dp = dpi.getNext();
                SceneBlendFactor bfSrc = dp->getSourceBlendFactor();
                SceneBlendFactor bfDst = dp->getDestBlendFactor();
                if (bfSrc == SBF_SOURCE_ALPHA && bfDst == SBF_ONE_MINUS_SOURCE_ALPHA)
                {
                    applied = true;

                    const ColourValue& diffuse = dp->getDiffuse();
                    dp->setDiffuse(diffuse.r, diffuse.g, diffuse.b, opacity);
                    switch (dp->getAlphaRejectFunction())
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
                        dp->setAlphaRejectValue(static_cast<uchar>(128 * opacity));
                        break;
                    }
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
    
	LogicModel::LogicModel( const String& name, System *system ) :
	mName(name),
	mSystem(system),
	mModelMainNode(NULL),
    mParentNode(NULL),
	mSkeletonEntity(NULL),
	mCreatedEntityCount(0),
	mSkeleton(NULL),
    mFullBoundingBox(NULL),
	mIsPrepareBoundBox(false),
	mCurrentAnimationState(NULL),
	mFadeOutAnimationState(NULL),
	mGlobalAnimationRate(1.0f),
	mFadeOutDelay(0.0f),
	mCurrentAnimationName(""),
	mAnimationLoop(false),
	mEffectHandleCount(0),
	mCurrentSkill(NULL),
	mOnAnimationFinish(NULL),
	mOnSkillBreakTime(NULL),
	mOnSkillHitTime(NULL),
    mOnSkillShakeTime(NULL),
    mOnGetEffectName(NULL),
    mCallbackFuncInfo(0),
	mAnimEditing(false),
	mAnimTimePos(0.0f),
	mChildModel(false),
	mMainNodeNeedUpdate(false),
	mMovementType(MT_NORMAL),
    mModelPosition(Ogre::Vector3::ZERO),
    mModelOrientation(Ogre::Quaternion::IDENTITY),
    mCurrentTransparency(0.0f),
    mDestTransparency(0.0f),
    mNeedUpdateTransparency(false),
    mTransparencyTime(0.0f),
    mDefaultAnimationName(""),
    mHairColour(Ogre::ColourValue::Black),
    mShadowCastable(false),
    mAttachParent(NULL),
    mSelected(false),
    mExternalBoundingBox(NULL),
    mExternalBoundingBoxValue(""),
    mExternalScaleFactor(Ogre::Vector3::UNIT_SCALE),
    mColliderSet(NULL),
    mTerrainHeight(Ogre::Math::POS_INFINITY),
    mVisibleFlag(OVF_DEFAULT),
    mEnableWalkingEffect(true),
    mIsActive(true),
    mVisible(true),
    mFrontBackOrientation(Ogre::Quaternion::IDENTITY),
    mLeftRightOrientation(Ogre::Quaternion::IDENTITY)
	{
		mUserAny = Ogre::Any();

		mAttribMap.clear();
		mEntityMap.clear();
		mLocatorMap.clear();
		mEffectMap.clear();
		mAttachedModelOriginSceneNodes.clear();
        mModelProjectors.clear();
        mShadowUncastableMeshs.clear();
        mMaterialMap.clear();
        mFabricInfoMap.clear();
        mFabricColliderMap.clear();
        mCreatedEffectList.clear();
	}
	//-----------------------------------------------------------------------
	LogicModel::~LogicModel()
	{
        if (mExternalBoundingBox)
            delete mExternalBoundingBox;

        if (mFullBoundingBox)
            delete mFullBoundingBox;

        removeAllFabrics();

        _destroyColliders();

        // 先删除挂在自己身上的model（如武器等）
        mLocatorMap.clear();

        mShadowUncastableMeshs.clear();

		mAttribMap.clear();

        _delAllProjector();

		destroyEntities();

        _delSkeletonEntity();

		destroySceneNode();

		delAllEffect();

		delCurrentSkill();
	}
	//-----------------------------------------------------------------------
	void reset(void)
	{

	}
	//-----------------------------------------------------------------------
	bool LogicModel::addAttribute( const String &name, const String &value, const String &hint )
	{
		AttribMap::iterator i = mAttribMap.find(name);

		if ( i != mAttribMap.end() )
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Attribute with name '" + name + "' already exists! " +
				"LogicModel::addAttribute " + mName );

			return false;
		}

		std::pair<AttribMap::iterator, bool> inserted = 
			mAttribMap.insert( AttribMap::value_type( name, AttribValue(value, hint) ) );

		if (false == inserted.second)
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Attribute with name '" + name + "' inserted failed! " +
				"LogicModel::addAttribute " + mName );

			return false;
		}

        // 处理其他类型的属性
        _dealWithExtraAttributes(name, value);

		return true;
	}
	//-----------------------------------------------------------------------
	bool LogicModel::changeAttribute( const String &name, const String &value )
	{
		AttribMap::iterator i = mAttribMap.find(name);

		if ( i == mAttribMap.end() )
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Attribute with name '" + name + "' doesn't exists! " +
				"LogicModel::changeAttribute " + mName );

			return false;
		}

		AttribValue& tempValue = i->second;

		// 如果当前属性值就是要设置的值，直接返回
		if (tempValue.mAttribValue == value)
			return true;

		// 判断hint
		if ( tempValue.mHint == MESH_HINT )
		{
			if ( changeEntity(name, value) )
			{
				tempValue.mAttribValue = value;
				return true;
			}
		}
		else if ( tempValue.mHint == SLMT_CREATE_HINT )
		{
			if ( changeSlotAttrib(name, value) )
			{
				tempValue.mAttribValue = value;
				return true;
			}
		}	
		else if ( tempValue.mHint == MATERIAL_HINT )
		{
			if ( changeMaterial(name, value) )
			{
				tempValue.mAttribValue = value;
				return true;
			}
		}
		else
		{
            // 处理其他类型的属性
            _dealWithExtraAttributes(name, value);
		}

		return false;
	}
    //-----------------------------------------------------------------------
    bool LogicModel::getAttribute( const String& name, String& value)
    {
        AttribMap::iterator i = mAttribMap.find(name);

        if ( i == mAttribMap.end() )
        {
            Ogre::LogManager::getSingleton().logMessage( "Logic Model Attribute with name '" + name + "' doesn't exists! " +
                "LogicModel::changeAttribute " + mName );

            return false;
        }

        value = i->second.mAttribValue;

        return true;
    }
    //-----------------------------------------------------------------------
    Variant LogicModel::getAttribute( const String& attributeName )
    {
        AttribMap::iterator i = mAttribMap.find(attributeName);

        if ( i == mAttribMap.end() )
        {
            Ogre::LogManager::getSingleton().logMessage( "Logic Model Attribute with name '" + attributeName + "' doesn't exists! " +
                "LogicModel::changeAttribute " + mName );

            return Variant();
        }

        AttribValue& tempValue = i->second;

        // 判断hint
        if ( tempValue.mHint == SLMT_CREATE_HINT )
        {
            for ( LocatorMap::iterator itLocator = mLocatorMap.begin();
                itLocator != mLocatorMap.end(); ++itLocator )
            {
                LocatorValue& tempLocatorValue = itLocator->second;

                SlotMap::iterator itSlot = tempLocatorValue.mSlotMap.find(attributeName);

                if ( itSlot != tempLocatorValue.mSlotMap.end() )
                {
                    SlotValue& tempSlotValue = itSlot->second;

                    return tempSlotValue.mModel;
                }			
            }
        }
        
        return Variant();
    }
    //-----------------------------------------------------------------------
    bool LogicModel::hasAttribute( const String& name )
    {
        AttribMap::iterator i = mAttribMap.find(name);

        return ( i != mAttribMap.end() );
    }
	//-----------------------------------------------------------------------
	bool LogicModel::addEntity( const String &name, const String &meshName, const String &matName )
	{
		EntityMap::iterator i = mEntityMap.find(name);

		if ( i != mEntityMap.end() )
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Entity with name '" + name + "' already exists! " +
				"LogicModel::addEntity " + mName );

			return false;
		}

		std::pair<EntityMap::iterator, bool> inserted = 
			mEntityMap.insert( EntityMap::value_type( name, EntityValue(meshName, matName) ) );

		if (false == inserted.second)
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Entity with name '" + name + "' inserted failed! " +
				"LogicModel::addEntity " + mName );

			return false;
		}

		return true;
	}
	//-----------------------------------------------------------------------
	bool LogicModel::changeEntity( const String &name, const String &value )
	{
		EntityMap::iterator i = mEntityMap.find(name);

		if ( i == mEntityMap.end() )
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Entity with name '" + name + "' dosen't exists! " +
				"LogicModel::changeEntity " + mName );

			return false;
		}

		EntityValue& tempValue = i->second;

        // 删除原有的transparentor
        if (tempValue.mTransparentor)
        {
            delete tempValue.mTransparentor;
            tempValue.mTransparentor = NULL;
        }

		// 删除原有的entity
		if (tempValue.mEntity)
		{
            // 删除clone过的material
            _clearEntityMaterials(tempValue);

			_destroyEntityImpl(tempValue.mEntity);
            tempValue.mEntity = NULL;
		}

        if (false == value.empty())
        {
            tempValue.mEntity = _createEntityImpl(value);

            if (tempValue.mEntityColourChanged)
                _setEntityColour(tempValue, tempValue.mEntityColour);

            if (mCurrentTransparency > 0.0f)
            {
                tempValue.mTransparentor = new EntityTransparentor(tempValue.mEntity);
                tempValue.mTransparentor->apply(mCurrentTransparency);
            }
        }

        // 如果在obj文件中没定义包围盒的大小，就根据entity创建包围盒
        if (NULL == mExternalBoundingBox)
            _updateBoundingBox();

    	return true;
	}
	//-----------------------------------------------------------------------
	bool LogicModel::addMaterial( const String &name, const String &entityEntry, const String &matName )
	{
		MaterialMap::iterator i = mMaterialMap.find(name);

		if ( i != mMaterialMap.end() )
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Material with name '" + name + "' already exists! " +
				"LogicModel::addMaterial " + mName );

			return false;
		}

		std::pair<MaterialMap::iterator, bool> inserted = 
			mMaterialMap.insert( MaterialMap::value_type( name, MaterialValue(entityEntry, matName) ) );

		if (false == inserted.second)
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Material with name '" + name + "' inserted failed! " +
				"LogicModel::addMaterial " + mName );

			return false;
		}

		return true;
	}
    //-----------------------------------------------------------------------
	bool LogicModel::changeMaterial( const String &name, const String &value )
	{
		MaterialMap::iterator i = mMaterialMap.find(name);

		if ( i == mMaterialMap.end() )
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Material with name '" + name + "' dosen't exists! " +
				"LogicModel::changeMaterial " + mName );

			return false;
		}

		MaterialValue& tempValue = i->second;

		if ( _setEntityMaterial(tempValue.mEntityEntry, value) )
		{
			tempValue.mMaterialName = value;

			return true;
		}
		else
			return false;
	}
	//-----------------------------------------------------------------------
	bool LogicModel::addLocator( const String &name, const String &boneName, 
		const Ogre::Vector3 &pos, const Ogre::Quaternion &orientation, bool transferable, bool translateFirst)
	{
		LocatorMap::iterator i = mLocatorMap.find(name);

		if ( i != mLocatorMap.end() )
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Locator with name '" + name + "' already exists! " +
				"LogicModel::addLocator " + mName );

			return false;
		}

		std::pair<LocatorMap::iterator, bool> inserted = 
			mLocatorMap.insert( LocatorMap::value_type( name, 
            LocatorValue(boneName, pos, orientation, transferable, translateFirst ) ) );

		if (false == inserted.second)
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Locator with name '" + name + "' inserted failed! " +
				"LogicModel::addLocator " + mName );

			return false;
		}

		return true;
	}
    //-----------------------------------------------------------------------
    bool LogicModel::isLocatorTransferable(const String& name)
    {
        LocatorMap::iterator i = mLocatorMap.find(name);

        if (i != mLocatorMap.end())
        {
            return i->second.mTransferable;
        }
        else
            return false;
    }
    //-----------------------------------------------------------------------
    bool LogicModel::isLocatorExist(const String& locator)
    {
        LocatorMap::iterator i = mLocatorMap.find(locator);

        if (i != mLocatorMap.end())
        {
            return true;
        }
        else
            return false;
    }
    //-----------------------------------------------------------------------
    bool LogicModel::getLocatorInfo(const String& locator, LocatorValue& locatorInfo)
    {
        LocatorMap::const_iterator i = mLocatorMap.find(locator);

        if (i != mLocatorMap.end())
        {
            locatorInfo = i->second;
            return true;
        }
        else
            return false;
    }
    //-----------------------------------------------------------------------
    bool LogicModel::setLocatorInfo(const String& locator, const LocatorValue& info)
    {
        LocatorMap::iterator i = mLocatorMap.find(locator);

        if (i != mLocatorMap.end())
        {
            i->second = info;
            return true;
        }
        else
            return false;
    }
	//-----------------------------------------------------------------------
	bool LogicModel::addSlot( const String &locatorName, const String &slotName, const String &modelName, const String &attrib )
	{
		LocatorMap::iterator i = mLocatorMap.find(locatorName);

		if ( i == mLocatorMap.end() )
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Locator with name '" + locatorName + "' doesn't exists! " +
				"LogicModel::addSlot " + mName );

			return false;
		}

		SlotMap &slotMap = i->second.mSlotMap;
		std::pair<SlotMap::iterator, bool> inserted = 
			slotMap.insert( SlotMap::value_type( slotName, SlotValue(modelName) ) );

		if (false == inserted.second)
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Slot with name '" + slotName + "' inserted failed! " +
				"LogicModel::addSlot " + mName );

			return false;
		}

		return true;
	}  
	//-----------------------------------------------------------------------
	bool LogicModel::changeSlotAttrib( const String &name, const String &value )
	{
		for ( LocatorMap::iterator itLocator = mLocatorMap.begin();
			itLocator != mLocatorMap.end(); ++itLocator )
		{
			LocatorValue& tempLocatorValue = itLocator->second;

			SlotMap::iterator itSlot = tempLocatorValue.mSlotMap.find(name);

			if ( itSlot != tempLocatorValue.mSlotMap.end() )
			{
				SlotValue& tempSlotValue = itSlot->second;

				// 如果value是""，就删除原有的物体
				if ( value.empty() )
				{
					if (tempSlotValue.mModel)
						LogicModelManager::getSingleton().destroyLogicModel(tempSlotValue.mModel);

					tempSlotValue.mModel = NULL;
					tempSlotValue.mModelName = value;
					
					return true;
				}
				else
				{
                    tempSlotValue.mModelName = value;

					return _createModelInSlot(tempSlotValue, tempLocatorValue);
				}				
			}			
		}

		return false;
	}
	//-----------------------------------------------------------------------
	bool LogicModel::addEffectInfo( const String& name, const String& effectName,
        const String& locatorName, bool translateAll, const Ogre::ColourValue& colour )
	{
		EffectMap::iterator i = mEffectMap.find(name);

		if ( i != mEffectMap.end() )
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Effect with name '" + name + "' already exists! " +
				"LogicModel::addEffectInfo " + mName );

			return false;
		}

		std::pair<EffectMap::iterator, bool> inserted = 
			mEffectMap.insert( EffectMap::value_type( name, EffectValue(effectName, locatorName, translateAll, colour) ) );

		if (false == inserted.second)
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Effect with name '" + name + "' inserted failed! " +
				"LogicModel::addEffectInfo " + mName );

			return false;
		}

		return true;
	}
    //-----------------------------------------------------------------------
    bool LogicModel::addShadowUncastable(const String& meshName)
    {
        ShadowUncastableMeshs::iterator i = mShadowUncastableMeshs.begin();

        // 查找是否有重名的
        while (i != mShadowUncastableMeshs.end())
        {
            if (*i == meshName)
                break;

            ++i;
        }

        if ( i != mShadowUncastableMeshs.end() )
        {
            Ogre::LogManager::getSingleton().logMessage( "Logic Model ShadowUncastable with name '" + 
                meshName + "' already exists!  " + "LogicModel::addShadowUncastable " + mName );

            return false;
        }

        mShadowUncastableMeshs.push_back(meshName);

        return true;
    }
    //-----------------------------------------------------------------------
    bool LogicModel::isShadowUncastable(const String& meshName)
    {
        ShadowUncastableMeshs::iterator i = mShadowUncastableMeshs.begin();

        // 查找是否有重名的
        while (i != mShadowUncastableMeshs.end())
        {
            if (*i == meshName)
                return true;

            ++i;
        }

        return false;
    }
	//-----------------------------------------------------------------------
	/*LogicModel& LogicModel::operator = (const LogicModel& rhs)
	{


		return *this;
	}*/
	//-----------------------------------------------------------------------
	void LogicModel::_createEntity(void)
	{
		// 遍历entity列表并创建出相应的entity
		for ( EntityMap::iterator i = mEntityMap.begin();
			i != mEntityMap.end(); ++i )
		{
			Ogre::Entity* workingEntity = NULL;

			EntityValue& tempValue = i->second;
			
			String meshName = tempValue.mMeshName;
			String materialName = tempValue.mMaterialName;

            // 如果xml文件中mesh文件名不为空
            if (false == meshName.empty())
            {
                tempValue.mEntity = _createEntityImpl(meshName);

                if (tempValue.mEntity && false == materialName.empty())
                {
                    _setEntityMaterial(i->first, materialName);

                    if (tempValue.mEntityColourChanged)
                        _setEntityColour(tempValue, tempValue.mEntityColour);

                    if (mCurrentTransparency > 0.0f)
                    {
                        tempValue.mTransparentor = new EntityTransparentor(tempValue.mEntity);
                        tempValue.mTransparentor->apply(mCurrentTransparency);
                    }
                }
            }			
		}

        // 如果在obj文件中没定义包围盒的大小，就根据entity创建包围盒
        if (NULL == mExternalBoundingBox)
            _updateBoundingBox();
	}
	//-----------------------------------------------------------------------
	void LogicModel::destroyEntities(void)
	{
		for ( EntityMap::iterator i = mEntityMap.begin();
			i != mEntityMap.end(); ++i )
		{
            if (i->second.mTransparentor)
            {
                delete i->second.mTransparentor;
                i->second.mTransparentor = NULL;
            }

            // 删除clone过的material
            _clearEntityMaterials(i->second);

			Ogre::Entity* workingEntity = i->second.mEntity;

			if (workingEntity)
			{
				String entityName = workingEntity->getName();

                Ogre::SceneNode* parentNode = workingEntity->getParentSceneNode();

                if (parentNode)
                {
                    parentNode->detachObject(entityName);
                }

				mSystem->getSceneManager()->destroyEntity(entityName);
			}
		}
		mEntityMap.clear();
	}
	//-----------------------------------------------------------------------
	void LogicModel::setUserAny(const Ogre::Any& anything)
	{
		mUserAny = anything;

		for ( EntityMap::iterator i = mEntityMap.begin();
			i != mEntityMap.end(); ++i )
		{
			if (i->second.mEntity)
				i->second.mEntity->setUserAny(anything);
		}

		// locator上的model也要设置
		for ( LocatorMap::iterator itLocator = mLocatorMap.begin();
			itLocator != mLocatorMap.end(); ++itLocator )
		{
			LocatorValue& tempValue = itLocator->second;

			for ( SlotMap::iterator itSlot = tempValue.mSlotMap.begin();
				itSlot != tempValue.mSlotMap.end(); ++itSlot )
			{
				SlotValue& tempSlot = itSlot->second;

				if (tempSlot.mModel)
					tempSlot.mModel->setUserAny(anything);
			}
		}
	}
	//-----------------------------------------------------------------------
	const Ogre::Any& LogicModel::getUserAny(void) const
	{
		return mUserAny;
	}
	//-----------------------------------------------------------------------
	void LogicModel::setUserObject(Ogre::UserDefinedObject* obj)
	{
		setUserAny(Ogre::Any(obj));
	}
	//-----------------------------------------------------------------------
	Ogre::UserDefinedObject* LogicModel::getUserObject(void) const
	{
		return mUserAny.isEmpty() ? 0 : Ogre::any_cast<Ogre::UserDefinedObject*>(mUserAny);
	}
	//-----------------------------------------------------------------------
	void LogicModel::setParentSceneNode( Ogre::Node* parentNode )
	{
		if(mModelMainNode && mModelMainNode->getParentSceneNode())
			mModelMainNode->getParentSceneNode()->removeChild(mModelMainNode->getName());

		if(parentNode)
		{
			parentNode->addChild(mModelMainNode);

		}

        mParentNode = parentNode;
	}
	//-----------------------------------------------------------------------
	void LogicModel::destroySceneNode(void)
	{
		if (mModelMainNode)
		{
			if (mModelMainNode->getParentSceneNode())
			{
				mModelMainNode->getParentSceneNode()->removeChild(mModelMainNode->getName());
			}
            mModelMainNode->removeAndDestroyAllChildren();
			mModelMainNode->getCreator()->destroySceneNode(mModelMainNode->getName());
            mModelMainNode = NULL;
		}
	}
	//-----------------------------------------------------------------------
	const Ogre::AxisAlignedBox& LogicModel::getBoundingBox(void)
	{
        if (mExternalBoundingBox)
            return *mExternalBoundingBox;
        else
        {
            return _getFullBoundingBox();
        }
	}
	//-----------------------------------------------------------------------
	void LogicModel::_destroyEntityImpl(Ogre::Entity* entity)
	{
		assert (entity);

		String entityName = entity->getName();

  //      if (entityName == mSkeletonEntity->getName())
  //      {
  //          mSkeletonEntity = NULL;
  //          mSkeleton = NULL;
  //      }

		entity->getParentSceneNode()->detachObject(entityName);

		mSystem->getSceneManager()->destroyEntity(entityName);		
	}
	//-----------------------------------------------------------------------
	Ogre::Entity* LogicModel::_createEntityImpl( const String& meshName )
	{
		Ogre::Entity* resultEntity = NULL;

        loadMesh(
            meshName, Ogre::StringUtil::BLANK,
            Ogre::StringUtil::BLANK, DEFAULT_RESOURCE_GROUP_NAME);

		String entityName = mModelMainNode->getName() + "_Entity" + "_" + meshName +
			Ogre::StringConverter::toString(mCreatedEntityCount++);

        static bool suppressSkeletalAnimation = true;
		resultEntity = mSystem->getSceneManager()
            ->createEntity(entityName, meshName, suppressSkeletalAnimation);

		resultEntity->setUserAny(getUserAny());

        static bool initialised = false;
        static bool changeRenderQueue = false;
        if (!initialised)
        {
            initialised = true;
            const char* s = getenv("Fairy_Hack");
            changeRenderQueue = s && (strstr(s, "X-Ray") != 0);
        }
        if (changeRenderQueue)
        {
            resultEntity->setRenderQueueGroup(Ogre::RENDER_QUEUE_6);
        }
      
        // 设置entity为当前的visible flag
        resultEntity->setVisibilityFlags(mVisibleFlag);
        // 手动设置visible（由于octree manager的bug，所以要手动调用）
        resultEntity->setVisible(mVisible);

        // 根据需要设置normalize normal
        resultEntity->setNormaliseNormals(
            !mExternalScaleFactor.positionEquals(Ogre::Vector3::UNIT_SCALE));

		mModelMainNode->attachObject(resultEntity);

		if (!resultEntity->getMesh()->getSkeleton().isNull()) 
		{
            // 如果master enttiy为空（说明当前处理的是第一个有骨骼的entity），创建master entity
            if (NULL == mSkeletonEntity)
            {
                const Ogre::MeshPtr& originMesh = resultEntity->getMesh();
                assert (false == originMesh.isNull());
                const Ogre::SkeletonPtr& originSke = originMesh->getSkeleton();
                assert (false == originSke.isNull());

                _createSkeletonEntity(originSke);
            }

            assert (mSkeletonEntity);

            // 如果在共享骨骼时发生异常，记录下相关的信息
            try
            {
                resultEntity->shareSkeletonInstanceWith(mSkeletonEntity);
            }
            catch (const Ogre::Exception& e)
            {
                Ogre::StringUtil::StrStreamType errorString;

                errorString << "LogicModel::_createEntityImpl --> shareSkeletonInstanceWith\n";
                errorString << "new created entity : " << resultEntity->getMesh()->getName() << std::endl;
                errorString << "skeleton entity : " << mSkeletonEntity->getMesh()->getName() << std::endl;

                Ogre::LogManager::getSingleton().logMessage(errorString.str());

                if (mSystem->getThrowExceptionIfResourceIncorrect())
                {
                    resultEntity->destroy();
                    OGRE_EXCEPT(Ogre::Exception::ERR_RT_ASSERTION_FAILED,
                        "Resource was incorrectness due incaution producer. "
                        "Full error description: " + e.getFullDescription(),
                        "LogicModel::_createEntityImpl");
                }
            }
		}

        // share skeleton之后才创建hardware skin material，这时候能保证entity中的mSkeletonInstance是有值的
        _convertEntityToCharacterMaterial(resultEntity);
        // bool succeed = convertEntityToHardwareSkinning(resultEntity);

		if(mSkeletonEntity && !mSkeleton)
        {
			mSkeleton = mSkeletonEntity->getSkeleton();
            // 改变混合模式
            mSkeleton->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);
        }

        // 如果当前model被设置为可以投影，就要进行设置
        resultEntity->setCastShadows(mShadowCastable && false == isShadowUncastable(meshName));

		return resultEntity;
	}
	//-----------------------------------------------------------------------
	void LogicModel::_createMainSceneNode(void)
	{
		mModelMainNode = mSystem->getSceneManager()->getRootSceneNode()->createChildSceneNode();

        mParentNode = mModelMainNode->getParent();

        // 设置缩放因子
        mModelMainNode->setScale(mExternalScaleFactor);
	}
	//-----------------------------------------------------------------------
	void LogicModel::initModel(void)
	{
		_createMainSceneNode();

        // 创建external bounding box
        if (false == mExternalBoundingBoxValue.empty())
        {
            _createBoundingBoxFromExternal();
        }

		_createEntity();

		_setEntityMaterials();

		_createLocatorNode();

		_createEffects();

        // 播放初始动作
        if (false == mDefaultAnimationName.empty())
        {
            createSkill(mDefaultAnimationName, true, true);
        }
	}
	//-----------------------------------------------------------------------
	void LogicModel::_createLocatorNode(void)
	{
		for ( LocatorMap::iterator i = mLocatorMap.begin();
			i != mLocatorMap.end(); ++i )
		{
			LocatorValue& tempValue = i->second;

            // 这里先不创建locator所需的scene node，等到需要时才创建
		//	assert (NULL == tempValue.mLocatorNode);
		//	tempValue.mLocatorNode = 
        //        _createLocatorSceneNode(tempValue.mOffsetPos, tempValue.mOffsetOrientation);

            // 创建初始的model
            for ( SlotMap::iterator itSlot = tempValue.mSlotMap.begin();
                itSlot != tempValue.mSlotMap.end(); ++itSlot )
            {
                SlotValue& tempSlot = itSlot->second;

                if (false == tempSlot.mModelName.empty())
                    _createModelInSlot(tempSlot, tempValue);
            }
		}
	}
	//-----------------------------------------------------------------------
	void LogicModel::execute(Real delta)
	{	
        // 不可见的并且不是ui中的obj，不用更新
        if(!mVisible && mVisibleFlag != OVF_GUI_ELEMENTS)
            return;

		if (mMainNodeNeedUpdate)
		{
			// 如果位置或旋转发生了变化，就需要及时更新main node
			mModelMainNode->_update(true, false);

			// 根据当前地形来旋转物体
			if ( (mMovementType != MT_NORMAL) && mGetHeightInWorld )
				_rotate();

			mMainNodeNeedUpdate = false;
		}

		// 更新插入的动作列表
        _updatePlayingAnimations(delta);

        // 更新主动作
		_animationBlend(delta);

        // 设置透明度
        if (mNeedUpdateTransparency)
            updateTransparency(delta);

		if(mCurrentAnimationState)
		{
			Real totalen = 0.0f;
            Real oldTime = 0.0f;
            Real currTime = 0.0f;

			totalen = mCurrentAnimationState->getLength();

            // 如果这个动作的长度不为0，就正常地addTime，并计算出正确的oldTime和currTime
            if ( false == Ogre::Math::RealEqual(0.0f, totalen) )
            {
                oldTime = mCurrentAnimationState->getTimePosition() / totalen;

#ifdef EDITOR
                // 如果当前不是编辑状态，动作循环播放
                if ( false == mAnimEditing )
                {
                    mCurrentAnimationState->addTime(delta * mGlobalAnimationRate);
                }
                else
                {
                    mCurrentAnimationState->setTimePosition( mAnimTimePos * totalen );
                }
#else
                mCurrentAnimationState->addTime(delta * mGlobalAnimationRate);

#endif
                // Make sure the skeletal animation of the entities up-to-date, so all bones,
                // tag-points transform will up-to-date on demand
                static bool old = false;
                if (old)
                    mSkeleton->setAnimationState(*mSkeletonEntity->getAllAnimationStates());
                else
                    mSkeletonEntity->_updateSkeletalAnimation();

                currTime = mCurrentAnimationState->getTimePosition() / totalen;
            }

			bool animationConitnue = _handleAnimationFinish(oldTime, currTime);

			// 更新bone locator的位置
			_updateLocatorPos();
            
			if (animationConitnue)
			{
				if ( mCurrentSkill )
				{					
					_handleSkillHitTime(oldTime, currTime);
					_handleSkillBreakTime(oldTime, currTime);	
                    _handleSkillShakeTime(oldTime, currTime);

					if (mCurrentSkill && mVisible)
					{
						_createAnimationEffect(oldTime, currTime);
						_updateAttachedAnimationEffect(oldTime, currTime);

						_createAnimationRibbon(oldTime, currTime);
						_updateAnimationRibbon(delta);  

						_updateAnimationSound(oldTime, currTime);

                        _updateAnimationLight(oldTime, currTime);
					}
				}
			}
		}

        _updateFabricColliders();

        // 更新挂在model上的effect
        _updateAllEffects();

        _updateModelProjectors();

		_executeLocatorModel(delta);

        // 更新通过attachModel上的model
        _executeAttachedModel(delta);
	}
	//-----------------------------------------------------------------------
	void LogicModel::setVisibleFlag(uint32 flags)
	{
        if (mVisibleFlag != flags)
        {
            mVisibleFlag = flags;

            // 设置自身的entity
            for ( EntityMap::iterator itEntity = mEntityMap.begin();
                itEntity != mEntityMap.end(); ++itEntity )
            {
                Ogre::Entity* pEntity = itEntity->second.mEntity;
                if (pEntity)
                    pEntity->setVisibilityFlags(flags);
            }

            // 通过addEffect添加上去的effect也要设置visible flag（如武器特效）
            CreatedEffectList::iterator i = mCreatedEffectList.begin();

            while ( i != mCreatedEffectList.end() )
            {
                WX::Effect* effect = (*i).mEffect;

                if (effect)
                {
                    effect->setVisibleFlag(flags);
                }

                ++i;
            }		

            // 设置fabric的visible flag
            FabricInfoMap::iterator fabricIterator = mFabricInfoMap.begin();

            while ( fabricIterator != mFabricInfoMap.end() )
            {
                fabricIterator->second.mFabric->setVisibilityFlags(flags);

                ++fabricIterator;
            }

            // locator上的model也要设置
            for ( LocatorMap::iterator itLocator = mLocatorMap.begin();
                itLocator != mLocatorMap.end(); ++itLocator )
            {
                LocatorValue& tempValue = itLocator->second;

                for ( SlotMap::iterator itSlot = tempValue.mSlotMap.begin();
                    itSlot != tempValue.mSlotMap.end(); ++itSlot )
                {
                    SlotValue& tempSlot = itSlot->second;

                    if (tempSlot.mModel)
                        tempSlot.mModel->setVisibleFlag(flags);
                }
            }
        }		
	}
	//-----------------------------------------------------------------------
	bool LogicModel::_changeAnimation(const String& animationName, Real delay)
	{
		if (mFadeOutAnimationState)
		{
			mFadeOutAnimationState->setWeight(0);
			mFadeOutAnimationState->setEnabled(false);
		}

		mFadeOutAnimationState = mCurrentAnimationState;
		mFadeOutDelay = delay;

		if(animationName.empty())
		{
			mCurrentAnimationState = NULL;
			mCurrentAnimationName = "";
		}
		else
		{
			try{

                // 有可能因为资源的原因，缺少所需的skeleton文件，所以这个entity为空
                if (mSkeletonEntity)
                {
				    mCurrentAnimationState = _getAnimationState(animationName);

                    mSkeleton->getAnimation(animationName)->setInterpolationMode( 
                    LogicModelManager::getSingleton().getAnimationInterpolationMode() );
                }
                else
                {
                    Ogre::LogManager::getSingleton().logMessage( "Logic Model : " + mName + 
                        " Skeleton Entity is NULL, Maybe the skeleton file is lost!" +
                        "LogicModel::_changeAnimation");
                }

			}
			catch (const Ogre::Exception& e)
			{
                Ogre::LogManager::getSingleton().logMessage("Model : " + mName + '\n' + "_getAnimationState Failed!");
				mCurrentAnimationState = NULL;

                if (mSystem->getThrowExceptionIfResourceIncorrect())//添加捕捉完成（动作名不存在）
                {
                    OGRE_EXCEPT(Ogre::Exception::ERR_RT_ASSERTION_FAILED,
                        "Resource was incorrectness due incaution producer. "
                        "Full error description: " + e.getFullDescription(),
                        "LogicModel::_changeAnimation");
                }
			}
			if(mCurrentAnimationState)
			{
				if (mFadeOutAnimationState == mCurrentAnimationState)
					mFadeOutAnimationState = NULL;

				mCurrentAnimationState->setEnabled(true);
				mCurrentAnimationState->setTimePosition(0.0f);
				mCurrentAnimationState->setWeight(mFadeOutAnimationState ? 0 : 1);
				mCurrentAnimationName = animationName;

                // 暂时注释掉这个警告，因为现在我们的死亡动作都是只有一帧
                //float animLength = mCurrentAnimationState->getLength();

                //if ( animLength <= 0.0f )
                //{
                //    Ogre::LogManager::getSingleton().logMessage( mName + " The length of animation '"
                //        + mCurrentAnimationName + "' <= zero '" + 
                //        Ogre::StringConverter::toString(animLength) + "' " +
                //        "LogicModel::_changeAnimation");
                //}

				return true;
			}
			else
			{
				mCurrentAnimationName = "";
				return false;
			}
		}
		return true;
	}
	//-----------------------------------------------------------------------
	void LogicModel::_updateLocatorPos(void)
	{
		for ( LocatorMap::iterator i = mLocatorMap.begin();
			i != mLocatorMap.end(); ++i )
		{
			LocatorValue& tempValue = i->second;

            // 只更新创建出来的（已经使用的）locator node
            if (tempValue.mLocatorNode)
            {
			    if (false == tempValue.mBoneName.empty())
			    {
				    Ogre::Bone* bone = NULL;
				    try
				    {
					    bone = mSkeleton->getBone(tempValue.mBoneName);
				    }
				    catch (const Ogre::Exception& e)
				    {
					    Ogre::LogManager::getSingleton().logMessage("LogicModel::_updateLocatorPos " + mName + e.getDescription());

                        if (mSystem->getThrowExceptionIfResourceIncorrect())
                        {
                            OGRE_EXCEPT(Ogre::Exception::ERR_RT_ASSERTION_FAILED,
                                "Resource was incorrectness due incaution producer. "
                                "Full error description: " + e.getFullDescription(),
                                "LogicModel::_updateLocatorPos");
                        }

                        continue;
				    }

				    assert (bone);

				    Ogre::SceneNode* locatorNode = tempValue.mLocatorNode;
				    assert (locatorNode);

				    locatorNode->setPosition( bone->getWorldPosition() );
				    locatorNode->setOrientation( bone->getWorldOrientation() );

                    if (tempValue.mTranslateFirst)
                    {
                        locatorNode->translate( tempValue.mOffsetPos, Ogre::Node::TS_LOCAL );
                        locatorNode->rotate( tempValue.mOffsetOrientation, Ogre::Node::TS_LOCAL );
                    }
                    else
                    {
                        locatorNode->rotate( tempValue.mOffsetOrientation, Ogre::Node::TS_LOCAL );
                        locatorNode->translate( tempValue.mOffsetPos, Ogre::Node::TS_LOCAL );
                    }

                    // 这这里并不更新locatornode的update
             //       locatorNode->_update(true,false);
			    }
            }
		}

		// 全部locator node都设置了positon之后，再调用它们共同的父节点来update
        if (mLocatorMap.size())
            mModelMainNode->_update(true,false);
	}
	//-----------------------------------------------------------------------
	void LogicModel::setAnimationLoop(bool loop)
	{
		mAnimationLoop = loop;

		if (mCurrentAnimationState)
		{
			mCurrentAnimationState->setLoop(loop);
		}
	}
	//-----------------------------------------------------------------------
	void LogicModel::_animationBlend(Real delta)
	{
		if (mFadeOutAnimationState && mCurrentAnimationState)
		{
			if (mFadeOutDelay <= delta)
			{
				mFadeOutAnimationState->setWeight(0);
				mFadeOutAnimationState->setEnabled(false);
				mCurrentAnimationState->setWeight(1);
				mFadeOutAnimationState = NULL;
			}
			else
			{
				Real weight = mCurrentAnimationState->getWeight();
				weight = weight + delta * (1 - weight) / mFadeOutDelay;

				mCurrentAnimationState->setWeight(weight);
				mFadeOutAnimationState->setWeight(1 - weight);

				mFadeOutAnimationState->addTime(delta * mGlobalAnimationRate);
			}

			mFadeOutDelay -= delta;
		}
	}
	//-----------------------------------------------------------------------
	void LogicModel::_executeLocatorModel(Real delta)
	{
		for ( LocatorMap::iterator itLocator = mLocatorMap.begin();
			itLocator != mLocatorMap.end(); ++itLocator )
		{
			LocatorValue& tempValue = itLocator->second;

			for ( SlotMap::iterator itSlot = tempValue.mSlotMap.begin();
				itSlot != tempValue.mSlotMap.end(); ++itSlot )
			{
				SlotValue& tempSlot = itSlot->second;

				if (tempSlot.mModel)
					tempSlot.mModel->execute(delta);
			}
		}
	}
	//-----------------------------------------------------------------------
	LogicModel::EffectHandle LogicModel::addEffect( const String& effectName, const String& locatorName,
        LogicModel::GetTransformInfoType transformType, const Ogre::ColourValue& colour )
	{	
		// 找到这个locator的scene node
		LocatorMap::iterator i = mLocatorMap.find(locatorName);

		if ( i == mLocatorMap.end() )
		{
		//	Ogre::LogManager::getSingleton().logMessage( "Logic Model Locator with name '" + locatorName + "' doesn't exists! " +
		//		"LogicModel::addEffect " + mName );

            OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
                "Logic Model Locator with name '" + locatorName + "' doesn't exists! " +
                "LogicModel::addEffect " + mName + " " + effectName,
                "LogicModel::addEffect");

			return 0;
		}
		else
		{
			Effect *effect = EffectManager::getSingleton().createEffect(effectName);

			if (effect)
			{
				effect->createSceneNode();

                // 在创建的时候先设置为一个较远的地方，因为有可能剔除了，不会执行
                // 这个model的execute，effect的位置的信息不会被更新，这样作会避免在
                // 场景中看到一个没被更新位置的effect
                TransformInfo info;
                info.mPosition = Ogre::Vector3(0,-10000,0);

                effect->setTransformInfo(info);

                // 设置effect为当前的visible flag
                effect->setVisibleFlag(mVisibleFlag);
                effect->setVisible(mVisible);

				// 只有特别指定了颜色，才需要设置到effect中
                if (colour != Ogre::ColourValue::White)
                    effect->setColour(colour);

                mCreatedEffectList.push_back( CreatedEffectInfo(effect, locatorName, transformType, ++mEffectHandleCount) );

				return mEffectHandleCount;
			}
			else
				return 0;
		}		
	}
	//-----------------------------------------------------------------------
	LogicModel::CreatedEffectList::iterator LogicModel::delEffect( EffectHandle handle )
	{
        assert (handle > 0);

        CreatedEffectList::iterator i = mCreatedEffectList.begin();

        // 定位effect
        while ( i != mCreatedEffectList.end() )
        {
            if ( (*i).mEffectHandle == handle )
            {
                break;
            }

            ++i;
        }

		if ( i != mCreatedEffectList.end() )
		{
			EffectManager::getSingleton().removeEffect(((*i).mEffect),false);

            // 去除对已remove的effect的引用
            (*i).mEffect = NULL;

            // erase，并返回下个iterator
			return mCreatedEffectList.erase(i);
		}
		else
        {
            // 所找的effect不存在
            OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
                "the handle " + Ogre::StringConverter::toString(handle) + " is wrong!",
                "LogicModel::delEffect");
        }
	}
    //-----------------------------------------------------------------------
    LogicModel::CreatedEffectList::iterator 
        LogicModel::delEffect( LogicModel::CreatedEffectList::iterator effectIterator )
    {
        assert ( effectIterator != mCreatedEffectList.end() );

        EffectManager::getSingleton().removeEffect(((*effectIterator).mEffect),false);

        // 去除对已remove的effect的引用
        (*effectIterator).mEffect = NULL;

        // erase，并返回下个iterator
        return mCreatedEffectList.erase(effectIterator);
    }
	//-----------------------------------------------------------------------
	void LogicModel::delAllEffect(void)
	{
		for ( CreatedEffectList::iterator i = mCreatedEffectList.begin();
			i != mCreatedEffectList.end(); ++i )
		{		
			Effect* effect = (*i).mEffect;
			if (effect)
			{
				EffectManager::getSingleton().removeEffect(effect,false);

                // 去除对已remove的effect的引用
                (*i).mEffect = NULL;
			}
		}
		mCreatedEffectList.clear();
	}
    //-----------------------------------------------------------------------
    Effect* LogicModel::getEffect(LogicModel::EffectHandle handle)
    {
        assert (handle > 0);

        CreatedEffectList::iterator i = mCreatedEffectList.begin();

        // 定位effect
        while ( i != mCreatedEffectList.end() )
        {
            if ( (*i).mEffectHandle == handle )
            {
                break;
            }

            ++i;
        }

        if ( i != mCreatedEffectList.end() )
        {
            return (*i).mEffect;
        }
        else
        {
            // 所找的effect不存在
            /*OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
                "the handle " + Ogre::StringConverter::toString(handle) + " is wrong!",
                "LogicModel::getEffect");*/
            return NULL;
        }
    }
	//-----------------------------------------------------------------------
	bool LogicModel::getLocatorWorldTransform(const String& name, TransformInfo& info, const Ogre::Vector3& offsetPos,
		const Ogre::Quaternion& offsetRotation, LogicModel::GetTransformInfoType type )
	{
		Ogre::Matrix4 mtx;
		Ogre::Matrix3 mat(Ogre::Matrix3::IDENTITY);

		LocatorMap::iterator it = mLocatorMap.find(name);

		if (it == mLocatorMap.end())
			return false;
		else
		{
			LocatorValue& tempValue = it->second;

			Ogre::SceneNode* locatorNode = _getLocatorSceneNode(tempValue);

			switch (type)
			{
			case GTIT_ALL:

                info.mRotation = locatorNode->getWorldOrientation() * offsetRotation;
                info.mPosition = locatorNode->getWorldOrientation() * ( locatorNode->_getDerivedScale() * offsetPos )
                    + locatorNode->getWorldPosition();

				break;

			case GTIT_POSITION:

                info.mPosition = locatorNode->getWorldOrientation() * ( locatorNode->_getDerivedScale() * offsetPos )
                    + locatorNode->getWorldPosition();

				break;

			case GTIT_ORIENTATION:

                info.mRotation = locatorNode->getWorldOrientation() * offsetRotation;

				break;

			default:
				return false;
			}

			return true;
		}		
	}
	//-----------------------------------------------------------------------
	bool LogicModel::getBoneWorldTransform(const String& name, TransformInfo& info, const Ogre::Vector3& offsetPos,
		const Ogre::Quaternion& offsetRotation, LogicModel::GetTransformInfoType type )
	{
		assert (mSkeleton);

		Ogre::Bone* bone = NULL;

		try
		{
			bone = mSkeleton->getBone(name);
		}
		catch (const Ogre::Exception& e)
		{
			Ogre::LogManager::getSingleton().logMessage("LogicModel::getBoneWorldTransform " + mName + e.getDescription());

            /*if (mSystem->getThrowExceptionIfResourceIncorrect())//绑定点没找到（骨骼里也没这个名字）
            {
                OGRE_EXCEPT(Ogre::Exception::ERR_RT_ASSERTION_FAILED,
                    "Resource was incorrectness due incaution producer. "
                    "Full error description: " + e.getFullDescription(),
                    "LogicModel::getBoneWorldTransform");
            }*/

			return false;
		}

		Ogre::Matrix3 mat(Ogre::Matrix3::IDENTITY);

		assert (bone);
		
		switch (type)
		{
		case GTIT_ALL:

			info.mRotation = mModelMainNode->_getDerivedOrientation() * bone->_getDerivedOrientation() * offsetRotation;

			info.mRotation.ToRotationMatrix(mat);

			info.mPosition = mModelMainNode->_getFullTransform() * bone->_getDerivedPosition() + mat * offsetPos;

			break;

		case GTIT_POSITION:

			info.mPosition = mModelMainNode->_getFullTransform() * bone->_getDerivedPosition() + offsetPos;

			break;

		case GTIT_ORIENTATION:

			info.mRotation = mModelMainNode->_getDerivedOrientation() * bone->_getDerivedOrientation() * offsetRotation;

			break;

		default:
			return false;
		}	

		return true;		
	}
    //-----------------------------------------------------------------------
    bool LogicModel::getLocatorWorldTransform(const String& name, Ogre::Matrix4& mtx)
    {
        LocatorMap::iterator it = mLocatorMap.find(name);

        if (it == mLocatorMap.end())
            return false;
        else
        {
            LocatorValue& tempValue = it->second;

            Ogre::SceneNode* locatorNode = _getLocatorSceneNode(tempValue);

            mtx = locatorNode->_getFullTransform();

            return true;
        }
    }
	//-----------------------------------------------------------------------
	void LogicModel::createSkill(const String& skillName, bool loop, bool anim, Real delayTime)
	{
		delCurrentSkill();
		try
		{
			if ( false == anim )
			{
				mCurrentSkill = EffectManager::getSingleton().createSkill(skillName);

				if (NULL == mCurrentSkill)
				{
					_changeAnimation(skillName, delayTime);
				}
				else
				{
					_changeAnimation(mCurrentSkill->getAnimationName(), delayTime);
				}
			}
			else
			{
				_changeAnimation(skillName, delayTime);
			}												
		}
		catch ( Ogre::Exception& e)
		{
			if (e.getNumber() ==  Ogre::Exception::ERR_RT_ASSERTION_FAILED)
			{
				 Ogre::LogManager::getSingleton().logMessage("Animation : " + skillName + '\n' + "_getAnimationState Failed!");
			}

		}
		setAnimationLoop(loop);
	}
	//-----------------------------------------------------------------------
	void LogicModel::delCurrentSkill(void)
	{
		if (mCurrentSkill)
		{
			// todo
			_muteCurrentSkillSound();
			EffectManager::getSingleton().removeSkill(mCurrentSkill);
			mCurrentSkill = NULL;
		}
	}
	//-----------------------------------------------------------------------
	void LogicModel::_muteCurrentSkillSound(void)
	{
		if (mCurrentSkill)
		{
			if (mAnimationLoop && mOnStopSound)
			{
				for ( ushort i=0; i<mCurrentSkill->getNumAnimationSounds(); ++i )
				{
					AnimationSound *sound = mCurrentSkill->getAnimationSound(i);
					assert (sound);
					if (sound->mSoundHandle != -1)
					{
						mOnStopSound(sound->mSoundHandle);
					}
				}
			}
		}
	}
	//-----------------------------------------------------------------------
	bool LogicModel::_handleAnimationFinish(Real oldTime, Real currTime)
	{
		if (currTime <= oldTime && false == mAnimationLoop)
		{
			if ( mOnAnimationFinish )
			{
				mOnAnimationFinish(mCurrentAnimationName.c_str(), mCallbackFuncInfo);

				return false;
			}
		}

		return true;
	}
	//-----------------------------------------------------------------------
	void LogicModel::_handleSkillHitTime(Real oldTime, Real currTime)
	{
		if (mCurrentSkill)
		{
            unsigned short hitTimeCount = mCurrentSkill->getNumHitTimes();
			for (ushort i=0; i<hitTimeCount; ++i)
			{
				Real hitTime = mCurrentSkill->getHitTime(i);

				if (hitTime >= 0.0f && hitTime <= 1.0f)
				{
					if((currTime >= oldTime && hitTime >= oldTime && hitTime < currTime) || 
						(currTime < oldTime && (hitTime >= oldTime || hitTime < currTime)))
					{
						if (mOnSkillHitTime)
							mOnSkillHitTime(mCurrentAnimationName.c_str(), mCallbackFuncInfo, i, hitTimeCount);

						if ( mCurrentSkill == NULL )
							break;
					}
				}
			}
		}		
	}
	//-----------------------------------------------------------------------
	void LogicModel::_handleSkillBreakTime(Real oldTime, Real currTime)
	{
		if (mCurrentSkill)
		{
            unsigned short breakTimeCount = mCurrentSkill->getNumBreakTimes();
			for (ushort i=0; i<breakTimeCount; ++i)
			{
				Real breakTime = mCurrentSkill->getBreakTime(i);

				if (breakTime >= 0.0f && breakTime <= 1.0f)
				{
					if((currTime >= oldTime && breakTime >= oldTime && breakTime < currTime) || 
						(currTime < oldTime && (breakTime >= oldTime || breakTime < currTime)))
					{
						if (mOnSkillBreakTime)
							mOnSkillBreakTime(mCurrentAnimationName.c_str(), mCallbackFuncInfo, i, breakTimeCount);

						if ( mCurrentSkill == NULL )
							break;
					}
				}
			}
		}
	}
    //-----------------------------------------------------------------------
    void LogicModel::_handleSkillShakeTime(Real oldTime, Real currTime)
    {
        if (mCurrentSkill)
        {
            for (ushort i=0; i<mCurrentSkill->getNumShakeTimes(); ++i)
            {
                Real shakeTime = mCurrentSkill->getShakeTime(i);

                if (shakeTime >= 0.0f && shakeTime <= 1.0f)
                {
                    if((currTime >= oldTime && shakeTime >= oldTime && shakeTime < currTime) || 
                        (currTime < oldTime && (shakeTime >= oldTime || shakeTime < currTime)))
                    {
                        if (mOnSkillShakeTime)
                            mOnSkillShakeTime(mCurrentAnimationName.c_str(), mCallbackFuncInfo);
                    }
                }
            }
        }
    }
	//-----------------------------------------------------------------------
	void LogicModel::_createAnimationEffect(Real oldTime, Real currTime)
	{
		for ( ushort i = 0; i < mCurrentSkill->getNumAnimationEffectInfos(); ++i )
		{
			AnimationEffectInfo *effectInfo = mCurrentSkill->getAnimationEffectInfo(i);
			assert (effectInfo);

			Real time = effectInfo->getAttachTime();

			if((currTime >= oldTime && time >= oldTime && time < currTime) || 
				(currTime < oldTime && (time >= oldTime || time < currTime)))
			{
				Effect *effect = effectInfo->getEffect();

                // 获取脚本中的特效名称
                const String& effectTemplateName = effectInfo->getEffectTemplateName();

                String realEffectTemplate;

                // 如果是模板特效名称
                if ( effectTemplateName.find("<") != String::npos )
                {
                    if ( false == mEnableWalkingEffect ||
                        false == _getTemplateEffectName( realEffectTemplate, effectTemplateName, mTerrainHeight ) )
                        continue;
                }
                else
                {
                    realEffectTemplate = effectTemplateName;

                    mTerrainHeight = Ogre::Math::POS_INFINITY;
                }

                //////////////////////////////////////////////////////////////////////////
                String attachPoint = effectInfo->getAttachPoint();

                TransformInfo info;

                // 先从绑定点找，如果没有再从骨头名称找
                if ( getLocatorWorldTransform( attachPoint, info, effectInfo->getOffsetPos(), 
                    effectInfo->getOffsetRotation() ) )
                {
                }
                else if ( getBoneWorldTransform( attachPoint, info, effectInfo->getOffsetPos(), 
                    effectInfo->getOffsetRotation() ) )
                {
                }

                if (mTerrainHeight < Ogre::Math::POS_INFINITY && info.mPosition.y >= mTerrainHeight)
                    return;
                //////////////////////////////////////////////////////////////////////////

#ifdef EDITOR
                if (effect)
                {
                    // 先把旧的删除，在创建新的
                    EffectManager::getSingleton().removeEffect(effect, false, false);

                    effect = EffectManager::getSingleton().createEffect(realEffectTemplate);
                    effect->createSceneNode();
                    effectInfo->setEffect(effect);
                }
                else
                {
                    effect = EffectManager::getSingleton().createEffect(realEffectTemplate);
                    effect->createSceneNode();
                    effectInfo->setEffect(effect);
                }

#else

                if (effect)
                {
                    // 如果动作是循环的(可以走到这里，这个动作肯定是loop的)，而且当前skill需要在每次进行动作时都创建一个新特效
                    if (mCurrentSkill->getRepeatEffect())
                    {
                        // 先把旧的删除，在创建新的
                        EffectManager::getSingleton().removeEffect(effect, false, false);

                        effect = EffectManager::getSingleton().createEffect(realEffectTemplate);
                        effect->createSceneNode();
                        effectInfo->setEffect(effect);
                    }
                }
                else
                {
                    effect = EffectManager::getSingleton().createEffect(realEffectTemplate);
                    effect->createSceneNode();
                    effectInfo->setEffect(effect);
                }
#endif

				if ( false == effectInfo->getAttach() )
				{
                    // 根据需要调整高度数据
                    if (mTerrainHeight < Ogre::Math::POS_INFINITY)
                    {
                        info.mPosition.y = mTerrainHeight;
                    }

					// 给effect传入点
					effect->setTransformInfo(info);
				}					
			}
		}
	}
	//-----------------------------------------------------------------------
	void LogicModel::_updateAttachedAnimationEffect(Real oldTime, Real currTime)
	{
		for ( ushort i = 0; i < mCurrentSkill->getNumAnimationEffectInfos(); ++i )
		{
			AnimationEffectInfo *effectInfo = mCurrentSkill->getAnimationEffectInfo(i);
			assert (effectInfo);

			Effect *effect = effectInfo->getEffect();

			if (effect && effectInfo->getAttach())
			{
                // 获取脚本中的特效名称
                const String& effectTemplateName = effectInfo->getEffectTemplateName();

                String realEffectTemplate;

                // 如果是模板特效名称
                if ( effectTemplateName.find("<") != String::npos )
                {
                    if ( false == mEnableWalkingEffect ||
                        false == _getTemplateEffectName( realEffectTemplate, effectTemplateName, mTerrainHeight ) )
                        continue;
                }
                else
                {
                    realEffectTemplate = effectTemplateName;

                    mTerrainHeight = Ogre::Math::POS_INFINITY;
                }

				String attachPoint = effectInfo->getAttachPoint();

				TransformInfo info;

				// 先从绑定点找，如果没有再从骨头名称找
				if ( getLocatorWorldTransform( attachPoint, info, effectInfo->getOffsetPos(), 
					effectInfo->getOffsetRotation() ) )
				{
				}
				else if ( getBoneWorldTransform( attachPoint, info, effectInfo->getOffsetPos(), 
					effectInfo->getOffsetRotation() ) )
				{
				}
    
                // 根据需要调整高度数据
                if (mTerrainHeight < Ogre::Math::POS_INFINITY)
                {
                    // 如果当前脚下点的位置低于地形信息高度，说明人在水中，就把水的特效位置设为地形信息高度，也就是水面高度
                    if (info.mPosition.y < mTerrainHeight)
                        info.mPosition.y = mTerrainHeight;
                    else
                    {
                        // 否则，人的高度高于地形信息高度，说明人在水面以上（可能是在水面上的桥上），可以直接把特效删除
                        EffectManager::getSingleton().removeEffect(effect, false, true);

                        // 设置特效信息里的特效引用为NULL，不然有可能会在以后还调用这个effect，而这个effect已经在addToFreeList时
                        // 可能被delete了，会引发VC的R6025错误
                        effectInfo->setEffect(NULL);

                        continue;
                    }
                }

				// 给effect传入点
				effect->setTransformInfo(info);
			}
		}
	}
	//-----------------------------------------------------------------------
	void LogicModel::_createAnimationRibbon(Real oldTime, Real currTime)
	{
		for ( ushort i = 0; i < mCurrentSkill->getNumAnimationRibbons(); ++i )
		{
			AnimationRibbon *ribbon = mCurrentSkill->getAnimationRibbon(i);
			assert (ribbon);

			Real time = ribbon->getAttachTime();

			if((currTime >= oldTime && time >= oldTime && time < currTime) || 
				(currTime < oldTime && (time >= oldTime || time < currTime)))
			{
				ribbon->createRenderInstance();
			}
		}
	}
	//-----------------------------------------------------------------------
	void LogicModel::_updateAnimationRibbon(Real delta)
	{
		for ( ushort i = 0; i < mCurrentSkill->getNumAnimationRibbons(); ++i )
		{
			AnimationRibbon *ribbon = mCurrentSkill->getAnimationRibbon(i);
			assert (ribbon);

			String attachPoint = ribbon->getAttachPoint();

			TransformInfo info;

			// 先从绑定点找，如果没有再从骨头名称找
			if ( getLocatorWorldTransform( attachPoint, info, ribbon->getOffsetPos(), 
				ribbon->getOffsetRotation() ) )
			{
			}
			else if ( getBoneWorldTransform( attachPoint, info, ribbon->getOffsetPos(), 
				ribbon->getOffsetRotation() ) )
			{
			}

			// 给effect传入点
			ribbon->updateRenderInstance(delta, info);
		}
	}
	//-----------------------------------------------------------------------
	void LogicModel::_updateAnimationSound(Real oldTime, Real currTime)
	{
		if (mOnPlaySound)
		{
			// sound
			for ( ushort i = 0; i < mCurrentSkill->getNumAnimationSounds(); ++i )
			{					
				AnimationSound *sound = mCurrentSkill->getAnimationSound(i);
				assert (sound);

				if (false == sound->mPlayed)
				{
					Real time = sound->getAttachTime();

					if((currTime >= oldTime && time >= oldTime && time < currTime) || 
						(currTime < oldTime && (time >= oldTime || time < currTime)))
					{
						Ogre::Matrix4 mtx;

						mtx = mModelMainNode->_getFullTransform();

						Real pos[3] = { mtx.getTrans().x, mtx.getTrans().y, mtx.getTrans().z };

						sound->mSoundHandle = mOnPlaySound(sound->getSoundName().c_str(), pos, mAnimationLoop);

						sound->mPlayed = true;
					}
				}	
			}
		}
	}
    //-----------------------------------------------------------------------
    void LogicModel::_updateAnimationLight(Real oldTime, Real currTime)
    {
        MainSceneLight* light = EffectManager::getSingleton().getMainSceneLight();

        assert (light);

        unsigned short lightInfoNum = mCurrentSkill->getNumAnimationSceneLightInfos();

        // 如果当前发招者不在摄像机内，就不改变灯光
        if (lightInfoNum > 0 && false == isCameraVisible())
        {
            return;
        }

        for ( ushort i = 0; i < lightInfoNum; ++i )
        {
            AnimationSceneLightInfo *lightInfo = mCurrentSkill->getAnimationSceneLightInfo(i);
            assert (lightInfo);

            Real time = lightInfo->getAttachTime();

            if((currTime >= oldTime && time >= oldTime && time < currTime) || 
                (currTime < oldTime && (time >= oldTime || time < currTime)))
            {
                LightModifyPhase modifyPhase = light->getLightModifyPhase();

                if (modifyPhase == LMP_IDLE)
                {
                    light->setFadeTime( lightInfo->getFadeInTime(), lightInfo->getFadeOutTime() );

                    light->setLastTime(lightInfo->getLastTime());

                    light->setColourValue(lightInfo->getDestColour());
                }
                
                if (modifyPhase == LMP_LAST)
                    light->addLastTime(lightInfo->getLastTime());
            }
        }
    }
	//-----------------------------------------------------------------------
	bool LogicModel::attachModel(const String& locatorName, LogicModel* model)
	{
		assert (model);

		LocatorMap::iterator itLocator = mLocatorMap.find(locatorName);

		if (itLocator == mLocatorMap.end() )
		{
			Ogre::LogManager::getSingleton().logMessage( "Logic Model Locator with name '" + locatorName + "' doesn't exists! " +
				"LogicModel::attachModel " + mName );

			return false;
		}

		AttachedModelOriginSceneNodes::iterator it = mAttachedModelOriginSceneNodes.find(model);

		if ( it != mAttachedModelOriginSceneNodes.end() )
			it->second = mParentNode;
		else
		{
			std::pair<AttachedModelOriginSceneNodes::iterator, bool> inserted = 
				mAttachedModelOriginSceneNodes.insert( AttachedModelOriginSceneNodes::value_type
				( model, mParentNode ) );

			assert (inserted.second);
		}		

        Ogre::SceneNode* locatorNode = _getLocatorSceneNode(itLocator->second);

		model->setParentSceneNode(locatorNode);

        // 被挂接的model要重置位置，这样才能保证被挂接到locator的位置上
        model->resetPosition();

        // 如果在obj文件中没定义包围盒的大小，就根据entity创建包围盒
        if (NULL == mExternalBoundingBox)
            _updateBoundingBox();
		
        // 告诉被挂接者是谁attach它的
        model->notifyAttach(this);

		return true;
	}
	//-----------------------------------------------------------------------
	bool LogicModel::detachModel( LogicModel* model )
	{
		assert (model);

		AttachedModelOriginSceneNodes::iterator it = mAttachedModelOriginSceneNodes.find(model);

		if ( it == mAttachedModelOriginSceneNodes.end() )
			return false;

		model->setParentSceneNode(it->second);

		mAttachedModelOriginSceneNodes.erase(it);

        // detach，所以挂接者为NULL
        model->notifyAttach(NULL);

		return true;
	}
	//-----------------------------------------------------------------------
	ushort LogicModel::getNumBones(void)
	{
		if (mSkeleton)
			return mSkeleton->getNumBones();
		else
			return 0;
	}
	//-----------------------------------------------------------------------
	const String& LogicModel::getBoneName( ushort index )
	{
		assert (mSkeleton);
		Ogre::Bone *bone = mSkeleton->getBone(index);
		assert (bone);
		return bone->getName();
	}
	//-----------------------------------------------------------------------
	Real LogicModel::getCurrentAnimationLength(void)
	{
		if(mCurrentAnimationState)
		{
			return mCurrentAnimationState->getLength();
		}
		else
			return 0.0f;
	}
	//-----------------------------------------------------------------------
	ushort LogicModel::getSkeletonAnimationCount(void)
	{
		if (mSkeleton)
			return mSkeleton->getNumAnimations();
		else
			return 0;
	}
	//-----------------------------------------------------------------------
	Ogre::Animation* LogicModel::getSkeletonAnimation( ushort index )
	{
		if (mSkeleton)
			return mSkeleton->getAnimation(index);
		else
			return NULL;
	}
	//-----------------------------------------------------------------------
	void LogicModel::getLocatorNames( Ogre::StringVector& names )
	{
		for ( LocatorMap::iterator i = mLocatorMap.begin();
			i != mLocatorMap.end(); ++i )
		{
			names.push_back(i->first);
		}
	}
	//-----------------------------------------------------------------------
	bool LogicModel::_setEntityMaterial(const String& entityEntry, const String& matName)
	{
        // 如果传入一个空字符串，也return true，因为客户端在卸载装备时会给material传入""
        if (matName.empty())
            return true;
        else
        {
            EntityMap::iterator itEntity = mEntityMap.find(entityEntry);

            if ( itEntity == mEntityMap.end() )
            {
                Ogre::LogManager::getSingleton().logMessage( "Logic Model Entity with name '" + entityEntry + "' dosen't exists! " +
                    "LogicModel::_setEntityMaterial " + mName );

                return false;
            }

            EntityValue& tempEntityValue = itEntity->second;

            Ogre::Entity* entity = tempEntityValue.mEntity;
            assert (entity);

            // 多材质利用";"来分隔开
            if (matName.find(";") != String::npos)
            {
                Ogre::StringVector matNames = Ogre::StringUtil::split(matName, ";");

                assert (matName.size() > 1);

                for( uint i=0; i<entity->getNumSubEntities(); ++i )
                {
                    Ogre::SubEntity* subEntity = entity->getSubEntity(i);
                    assert (subEntity);

                    // 如果有配对的material，就使用，没有的话使用第一个material
                    Ogre::String subMatName;

                    if (i < matNames.size())
                    {
                        subMatName = matNames[i];
                    }
                    else
                    {
                        subMatName = matNames[0];
                    }

                    // 先检测是否有这个材质
                    const Ogre::MaterialPtr subMat = Ogre::MaterialManager::getSingleton().getByName(subMatName);

                    if ( false == subMat.isNull() )
                        subEntity->setMaterialName( subMatName );
                }
            }
            else
            {
                // 先检测是否有这个材质
                const Ogre::MaterialPtr entityMat = Ogre::MaterialManager::getSingleton().getByName(matName);

                if ( false == entityMat.isNull() )
                    entity->setMaterialName(matName);
            }

            // 设置材质之后也要再次调用，因为entity中的材质已经变化了
            _convertEntityToCharacterMaterial(entity);

            if (tempEntityValue.mEntityColourChanged)
                _setEntityColour(tempEntityValue, tempEntityValue.mEntityColour);

            if (mCurrentTransparency > 0.0f)
            {
                if (tempEntityValue.mTransparentor)
                    delete tempEntityValue.mTransparentor;

                tempEntityValue.mTransparentor = new EntityTransparentor(tempEntityValue.mEntity);
                tempEntityValue.mTransparentor->apply(mCurrentTransparency);
            }


            return true;
        }		
	}
	//-----------------------------------------------------------------------
	void LogicModel::_setEntityMaterials(void)
	{
		for ( MaterialMap::iterator i = mMaterialMap.begin();
			i != mMaterialMap.end(); ++i )
		{
			MaterialValue& tempValue = i->second;			

			_setEntityMaterial(tempValue.mEntityEntry, tempValue.mMaterialName);
		}
	}
	//-----------------------------------------------------------------------
	void LogicModel::_createEffects()
	{
		for ( EffectMap::iterator i = mEffectMap.begin();
			i != mEffectMap.end(); ++i )
		{
			EffectValue& tempValue = i->second;

            if (tempValue.mTranslateAll)
			    addEffect(tempValue.mEffectName, tempValue.mLocator, GTIT_ALL, tempValue.mEffectColour);
            else
                addEffect(tempValue.mEffectName, tempValue.mLocator, GTIT_POSITION, tempValue.mEffectColour);
		}
	}
	//-----------------------------------------------------------------------
	void LogicModel::_updateAllEffects(void)
	{
		CreatedEffectList::iterator i = mCreatedEffectList.begin();

		while ( i != mCreatedEffectList.end() )
		{
			WX::Effect* effect = (*i).mEffect;

			if (effect)
			{
				if ( false == effect->isAlive() )
				{
					i = delEffect(i);

					continue;
				}		
				else
				{
					TransformInfo info;

                    const String& locatorName = (*i).mLocator;

                    // 判断是否出于被attach的状态下，如果是，并且locator是可转移的，就要获取
                    // attachParent的locator位置了，如上马后，脚下光圈点的特效就要转移到马的
                    // 脚下光圈点（在表现上是这样，并不是真的把effect挂接到马的脚下光圈点上）

                    if ( mAttachParent // 自身被一个LogicModel所attach
                        && isLocatorTransferable(locatorName) // 该locator是可以转移的
                        && mAttachParent->isLocatorTransferable(locatorName) // attachParent上有这个locator，并且也是可转移的 
                        )
                    {
                        // 获取attachParent上locator的位置
                        mAttachParent->getLocatorWorldTransform( locatorName, info, Ogre::Vector3::ZERO,
                        Ogre::Quaternion::IDENTITY, (*i).mTransformType );
                    }
                    else
                    {
                        // 获取自身的locator位置
                        getLocatorWorldTransform( locatorName, info, Ogre::Vector3::ZERO,
						    Ogre::Quaternion::IDENTITY, (*i).mTransformType );	
                    }

					// 给effect传入点
					effect->setTransformInfo(info);
				}			
			}

			++i;
		}		
	}
	//-----------------------------------------------------------------------
	void LogicModel::setPosition(Ogre::Vector3& pos)
	{
		mModelPosition = pos;
		mModelMainNode->setPosition(mModelPosition);
		mMainNodeNeedUpdate = true;
	}
	//-----------------------------------------------------------------------
	void LogicModel::setOrientation(Ogre::Quaternion& orientation)
	{
		mModelOrientation = orientation;
		mModelMainNode->setOrientation(mModelOrientation);
		mMainNodeNeedUpdate = true;
	}
	//-----------------------------------------------------------------------
	void LogicModel::_updateBoundingBox(void)
	{
        if (!mFullBoundingBox)
            mFullBoundingBox = new Ogre::AxisAlignedBox;

        mFullBoundingBox->setNull();

        // 遍历当前所有的entity
        for ( EntityMap::iterator i = mEntityMap.begin();
            i != mEntityMap.end(); ++i )
        {
            Ogre::Entity* workingEntity = i->second.mEntity;

            if (workingEntity)
            {
                mFullBoundingBox->merge( workingEntity->getBoundingBox() );
            }
        }

        // 遍历所有的attach model
        AttachedModelOriginSceneNodes::const_iterator it = 
            mAttachedModelOriginSceneNodes.begin();

        while (it != mAttachedModelOriginSceneNodes.end())
        {
            if (it->first)
                mFullBoundingBox->merge( it->first->_getFullBoundingBox() );

            ++it;
        }
	}
	//-----------------------------------------------------------------------
	void LogicModel::calXAxisRotation(float deltaX, float deltaZ, float deltaHeight)
	{
		float deltaLength = Ogre::Math::Sqrt( deltaZ * deltaZ + deltaX * deltaX );
		float angle = deltaHeight / deltaLength;
		Ogre::Radian radian = Ogre::Math::ATan( angle );

		Ogre::Vector3 axis = mModelMainNode->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_X;
		mFrontBackOrientation.FromAngleAxis( radian, axis );
	}
	//-----------------------------------------------------------------------
	void LogicModel::calZAxisRotation(float deltaX, float deltaZ, float deltaHeight)
	{
		float deltaLength = Ogre::Math::Sqrt( deltaZ * deltaZ + deltaX * deltaX );
		float angle = deltaHeight / deltaLength;
		Ogre::Radian radian = Ogre::Math::ATan( angle );

		Ogre::Vector3 axis = mModelMainNode->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
		mLeftRightOrientation.FromAngleAxis( radian, axis );
	}
	//-----------------------------------------------------------------------
	void LogicModel::_rotate(void)
	{
		if (mModelMainNode)
		{
#define MAX_DELTA_HEIGHT 550.0f

			if (mMovementType == MT_ROTATE_X_AXIS ||
				mMovementType == MT_ROTATE_X_Z_AXIS)
			{
				TransformInfo frontPointInfo;

				bool succeedFront = getLocatorWorldTransform( "FrontPoint", frontPointInfo, Ogre::Vector3::ZERO,
					Ogre::Quaternion::IDENTITY, GTIT_POSITION );

				TransformInfo backPointInfo;

				bool succeedBack = getLocatorWorldTransform( "BackPoint", backPointInfo, Ogre::Vector3::ZERO,
					Ogre::Quaternion::IDENTITY, GTIT_POSITION );

				if (succeedFront && succeedBack)
				{
					// 获取四个点的地形高度
					float lfy = 0.0f;
                    _getWorldHeight(frontPointInfo.mPosition.x, frontPointInfo.mPosition.z, lfy);
					float lby = 0.0f;
                    _getWorldHeight(backPointInfo.mPosition.x, backPointInfo.mPosition.z, lby);

					float deltaHeight = lfy-lby;

                    // 如果前后点差距太大，就直接跳过
                    if (Ogre::Math::Abs(deltaHeight) > MAX_DELTA_HEIGHT)
                    {
                        Ogre::LogManager::getSingleton().logMessage("_ratate() Larged than MAX_DELTA_HEIGHT : "
                            + Ogre::StringConverter::toString(deltaHeight));
                        return;
                    }

					float dz = frontPointInfo.mPosition.z - backPointInfo.mPosition.z;
					float dx = frontPointInfo.mPosition.x - backPointInfo.mPosition.x;
					calXAxisRotation(dx, dz, deltaHeight);
				}	
			}

			if (mMovementType == MT_ROTATE_X_Z_AXIS)
			{
				TransformInfo leftPointInfo;

				bool succeedLeft = getLocatorWorldTransform( "LeftPoint", leftPointInfo, Ogre::Vector3::ZERO,
					Ogre::Quaternion::IDENTITY, GTIT_POSITION );

				TransformInfo rightPointInfo;

				bool succeedRight = getLocatorWorldTransform( "RightPoint", rightPointInfo, Ogre::Vector3::ZERO,
					Ogre::Quaternion::IDENTITY, GTIT_POSITION );

				if (succeedLeft && succeedRight)
				{
					// 获取四个点的地形高度
					float lfy = 0.0f;
                    _getWorldHeight(leftPointInfo.mPosition.x, leftPointInfo.mPosition.z, lfy);
					float lby = 0.0f;
                    _getWorldHeight(rightPointInfo.mPosition.x, rightPointInfo.mPosition.z, lby);

					float deltaHeight = lfy-lby;

                    // 如果左右点差距太大，就直接跳过
                    if (Ogre::Math::Abs(deltaHeight) > MAX_DELTA_HEIGHT)
                        return;

					float dz = leftPointInfo.mPosition.z - rightPointInfo.mPosition.z;
					float dx = leftPointInfo.mPosition.x - rightPointInfo.mPosition.x;
					calZAxisRotation(dx, dz, deltaHeight);
				}	
			}			

		//	Ogre::Quaternion initOri = mModelMainNode->getInitialOrientation();
			mModelMainNode->setOrientation(mLeftRightOrientation * mFrontBackOrientation * mModelOrientation);
		}	
	}
	//-----------------------------------------------------------------------
	void LogicModel::_setMovementType( const String& value )
	{
		if ( value == "Normal" )
			mMovementType = MT_NORMAL;
		else if ( value == "RotateX" )
			mMovementType = MT_ROTATE_X_AXIS;
		else if ( value == "RotateXZ" )
			mMovementType = MT_ROTATE_X_Z_AXIS;
		else
		{
			Ogre::LogManager::getSingleton().logMessage( "Wrong Movement Type '" + value + "' ! " +
				"LogicModel::_setMovementType " + mName );
		}
	}
    //-----------------------------------------------------------------------
    void LogicModel::resetPosition(void)
    {
        mModelPosition = Ogre::Vector3::ZERO;
        mModelOrientation = Ogre::Quaternion::IDENTITY;

        mModelMainNode->setPosition(mModelPosition);
        mModelMainNode->setOrientation(mModelOrientation);
    }
    //-----------------------------------------------------------------------
    void LogicModel::setVisible(bool visible)
    { 
        if (mModelMainNode)
        {
            mModelMainNode->setVisible(visible);

            Ogre::Node* parentNode = mModelMainNode->getParent();

            // 当前要转成可见
            if (visible)
            {
                // 之前removechild了，也就是说这时mVisible为false
                if (parentNode == NULL)
                {
                    mParentNode->addChild(mModelMainNode);
                }
                else
                {
                    if (mParentNode != parentNode)
                    {
                        Ogre::LogManager::getSingleton().logMessage("setVisible error 1");
                    }
                }
            }
            else
            {
                if (parentNode != NULL)
                {
                    if (mParentNode != parentNode)
                    {
                        Ogre::LogManager::getSingleton().logMessage("setVisible error 2");
                    }

                    parentNode->removeChild(mModelMainNode);
                }
                else
                {
                    if (mVisible != visible)
                    {
                        Ogre::LogManager::getSingleton().logMessage("setVisible error 3");
                    }
                }
            }
        }

        mVisible = visible;

        // 通过addEffect添加的特效也要隐藏
        CreatedEffectList::iterator i = mCreatedEffectList.begin();

        while ( i != mCreatedEffectList.end() )
        {
            WX::Effect* effect = (*i).mEffect;

            if (effect)
            {
                effect->setVisible(visible);
            }

            ++i;
        }

        // locator上的model也要设置
        for ( LocatorMap::iterator itLocator = mLocatorMap.begin();
            itLocator != mLocatorMap.end(); ++itLocator )
        {
            LocatorValue& tempValue = itLocator->second;

            for ( SlotMap::iterator itSlot = tempValue.mSlotMap.begin();
                itSlot != tempValue.mSlotMap.end(); ++itSlot )
            {
                SlotValue& tempSlot = itSlot->second;

                if (tempSlot.mModel)
                    tempSlot.mModel->setVisible(visible);
            }
        }

        // 设置projector
        for ( ModelProjectors::iterator i = mModelProjectors.begin();
            i != mModelProjectors.end(); ++i )
        {
            (*i)->mProjectorEffect->setVisible(visible);
        }

        // 如果把人物隐藏了，他当前所执行的skill也要删除，不然会在场景里留下skill的特效
        if (!visible)
            delCurrentSkill();
    }
    //-----------------------------------------------------------------------
    void LogicModel::addProjector(ModelProjectorType type, const String& projectorEffectName,
        bool show, float projectorSize)
    {
        assert (false == projectorEffectName.empty());

        ModelProjector* modelProjector = NULL;

        // 先搜索
        for ( ModelProjectors::iterator i = mModelProjectors.begin();
            i != mModelProjectors.end(); ++i )
        {
            if ((*i)->mType == type)
            {
                modelProjector = *i;
                break;
            }
        }

        // 如果当前身上已有这种类型的projector
        if (modelProjector)
        {
            // 如果特效名称不一样，就更换特效
            if (projectorEffectName != modelProjector->mProjectorName)
            {
                EffectManager::getSingleton().removeEffect(modelProjector->mProjectorEffect, false);

                modelProjector->mProjectorEffect = EffectManager::getSingleton().createEffect(projectorEffectName);

                /** projector现在不直接挂在mMainNode上，而是创建自己的一个节点，然后在execute中调用
                    _updateModelProjectors来更新这些节点的位置，这样是为了不受main node的影响，如在
                    LogicModel::setVisible中对main node进行setVisible，不会影响到这个projector节点
                */
                modelProjector->mProjectorNode = modelProjector->mProjectorEffect->createSceneNode();
          //      modelProjector->mProjectorEffect->getBasicNode()->setInheritOrientation(false);

                for (unsigned short i=0; i<modelProjector->mProjectorEffect->getNumElements(); ++i)
                {
                    EffectElement *element = modelProjector->mProjectorEffect->getElement(i);

                    if (element->getType() == "Projector")
                        static_cast<ProjectorElement*>
                        (element)->setProjectionSize(projectorSize);
                }


                modelProjector->mProjectorName = projectorEffectName;

                assert (modelProjector->mProjectorEffect);
            }
        }
        else
        {
            modelProjector = new ModelProjector(projectorEffectName, type);
            modelProjector->mProjectorEffect = EffectManager::getSingleton().createEffect(projectorEffectName);
            modelProjector->mProjectorNode = modelProjector->mProjectorEffect->createSceneNode();

    //        modelProjector->mProjectorEffect->createSceneNode(mModelMainNode);
    //        modelProjector->mProjectorEffect->getBasicNode()->setInheritOrientation(false);

            for (unsigned short i=0; i<modelProjector->mProjectorEffect->getNumElements(); ++i)
            {
                EffectElement *element = modelProjector->mProjectorEffect->getElement(i);

                if (element->getType() == "Projector")
                    static_cast<ProjectorElement*>
                    (element)->setProjectionSize(projectorSize);
            }

            assert (modelProjector->mProjectorEffect);

            mModelProjectors.push_back(modelProjector);
        }

        assert (modelProjector);

        modelProjector->mProjectorEffect->setVisible(show && mVisible);
    }
    //-----------------------------------------------------------------------
    bool LogicModel::removeProjector(ModelProjectorType type, const String& projectorEffectName)
    {
        ModelProjector* modelProjector = NULL;

        for ( ModelProjectors::iterator i = mModelProjectors.begin();
            i != mModelProjectors.end(); ++i )
        {
            if ((*i)->mType == type && (*i)->mProjectorName == projectorEffectName)
            {
                EffectManager::getSingleton().removeEffect((*i)->mProjectorEffect, false);

                delete *i;
                *i = NULL;

                mModelProjectors.erase(i);

                return true;
            }
        }

        return false;
    }
    //-----------------------------------------------------------------------
    void LogicModel::_delAllProjector(void)
    {
        for ( ModelProjectors::iterator i = mModelProjectors.begin();
            i != mModelProjectors.end(); ++i )
        {
            EffectManager::getSingleton().removeEffect((*i)->mProjectorEffect, false);

            delete *i;
            *i = NULL;
        }

        mModelProjectors.clear();
    }
    //-----------------------------------------------------------------------
    void LogicModel::_updateModelProjectors(void)
    {
        // 更新当前挂在人物身上的projector的位置
        for ( ModelProjectors::iterator i = mModelProjectors.begin();
            i != mModelProjectors.end(); ++i )
        {
            Ogre::SceneNode* projectorNode = (*i)->mProjectorNode;
            if (projectorNode)
                // 把这个model当前的世界坐标设置到projector上
                projectorNode->setPosition(mModelMainNode->_getDerivedPosition());
        }
    }
    //-----------------------------------------------------------------------
    void LogicModel::setTransparent(Real time, Real transparency)
    {
        // 如果时间为0，就马上设置透明度
        if ( Ogre::Math::RealEqual(time,0.0f) )
        {
            mCurrentTransparency = mDestTransparency = transparency;
            _updateAllEntitiesTransparency();
        }
        else
        {
            // Clamp transparency to range [0, 1]
            if (transparency < 0)
                transparency = 0;
            else if (transparency > 1)
                transparency = 1;

            if (false == Ogre::Math::RealEqual(mDestTransparency,transparency))
            {              
                mDestTransparency = transparency;
                mTransparencyTime = time;
                mNeedUpdateTransparency = true;
                mDeltaTransparency = mDestTransparency - mCurrentTransparency;

                // updateTransparency();
            }
        }
        // locator上的model也要设置
        for ( LocatorMap::iterator itLocator = mLocatorMap.begin();
            itLocator != mLocatorMap.end(); ++itLocator )
        {
            LocatorValue& tempValue = itLocator->second;

            for ( SlotMap::iterator itSlot = tempValue.mSlotMap.begin();
                itSlot != tempValue.mSlotMap.end(); ++itSlot )
            {
                SlotValue& tempSlot = itSlot->second;

                if (tempSlot.mModel)
                    tempSlot.mModel->setTransparent(time, transparency);
            }
        }
    }
    //-----------------------------------------------------------------------
    void LogicModel::updateTransparency(Real deltaTime)
    {
        assert (mTransparencyTime > 0.0f);

        Real transparencyInc = deltaTime / mTransparencyTime * mDeltaTransparency;

        mCurrentTransparency += transparencyInc;        

        if ( (mDeltaTransparency >= 0.0f) && (mCurrentTransparency >= mDestTransparency) ||
            (mDeltaTransparency < 0.0f) && (mCurrentTransparency < mDestTransparency) )
        {
            mNeedUpdateTransparency = false;
            mCurrentTransparency = mDestTransparency;
        }

        _updateAllEntitiesTransparency();
    }
    //-----------------------------------------------------------------------
    bool LogicModel::_createModelInSlot(SlotValue& slotValue, LocatorValue& locatorValue)
    {
        if (slotValue.mModelName.empty())
            return false;

        LogicModel* tempModel = LogicModelManager::getSingleton().createLogicModel(slotValue.mModelName);

        if (tempModel)
        {
            // 删除原来挂的model
            if (slotValue.mModel)
                LogicModelManager::getSingleton().destroyLogicModel(slotValue.mModel);

            slotValue.mModel = tempModel;

            Ogre::SceneNode* locatorNode = _getLocatorSceneNode(locatorValue);

            tempModel->setParentSceneNode( locatorNode );
            tempModel->setChildModel(true);

            tempModel->setTransparencyInfo(mCurrentTransparency, mDestTransparency,
                Ogre::Math::Abs(mDestTransparency - mCurrentTransparency) * mTransparencyTime);

            // 设置为挂接上的物体的visible flag
            tempModel->setVisibleFlag(mVisibleFlag);
            tempModel->setVisible(mVisible);

            tempModel->setScaleFactor(mExternalScaleFactor);

            return true;
        }

        return false;
    }
    //-----------------------------------------------------------------------
    void LogicModel::_dealWithExtraAttributes(const String& name, const String& value)
    {
        // 处理其他类型的属性
        if (name == MOVEMENT_TYPE_ATTRIBUTE)
        {
            // 设置移动类型
            _setMovementType(value);
        }
        else if (name == DEFAULT_ANIM_NAME)
        {
            _setDefaultAnimationName(value);
        }
        else if (name == BOUNDING_BOX)
        {
            mExternalBoundingBoxValue = value;
           // _createBoundingBoxFromExternal(value);
        }
        else if (name == SCALE_FACTOR)
        {
            _setScaleFactor(value);
        }
    }
    //-----------------------------------------------------------------------
    void LogicModel::setHairColour(const Ogre::ColourValue& colour)
    {
        EntityMap::iterator i = mEntityMap.find(HAIR_MESH_NAME);

        if (i != mEntityMap.end())
        {
            // 获取头发entity
            Ogre::Entity* hairEntity = i->second.mEntity;

            if (hairEntity)
            {
                // 设置颜色变量
                mHairColour = colour;

                _setEntityColour(i->second, colour);
            }
        }
    }
    //-----------------------------------------------------------------------
    Ogre::Entity* LogicModel::_getEntityByName(const String& name)
    {
        EntityMap::iterator i = mEntityMap.find(name);

        if ( i != mEntityMap.end() )
        {
            return i->second.mEntity;
        }
        else
            return NULL;
    }
    //-----------------------------------------------------------------------
    void LogicModel::setShadowCastable(bool castable)
    {
        if (mShadowCastable != castable)
        {
            mShadowCastable = castable;

            // 对当前所有的entity都进行设置
            for ( EntityMap::iterator i = mEntityMap.begin();
                i != mEntityMap.end(); ++i )
            {                
                const String& meshName = i->second.mMeshName;

                // 如果当前这个mesh是规定为不可投影的，就跳到下一个
                if ( isShadowUncastable(meshName) )
                    continue;

                Ogre::Entity* workingEntity = i->second.mEntity;
                if (workingEntity)
                {
                    workingEntity->setCastShadows(mShadowCastable);
                }
            }
        }        
    }
    //-----------------------------------------------------------------------
    void LogicModel::_createSkeletonEntity(const Ogre::SkeletonPtr& skeleton)
    {
        mSkeletonEntity = LogicModelManager::getSingleton()._createSkeletalEntity(skeleton);

        // 就算skeleton entity也需要设置外部数据，因为它也会参与查询
     //   mSkeletonEntity->setUserAny(getUserAny());
        mSkeletonEntity->setQueryFlags(0);

        mModelMainNode->attachObject(mSkeletonEntity);
    }
    //-----------------------------------------------------------------------
    void LogicModel::_delSkeletonEntity(void)
    {
        if (mSkeletonEntity)
        {
            // Detach from parent scene node
            Ogre::SceneNode* sn = mSkeletonEntity->getParentSceneNode();
            if (sn && sn == mSkeletonEntity->getParentNode())
            {
                sn->detachObject(mSkeletonEntity);
            }

            // The manager will take care with skeletal entity caching
            LogicModelManager::getSingleton()._destroySkeletalEntity(mSkeletonEntity);
            mSkeletonEntity = 0;
        }
    }
    //-----------------------------------------------------------------------
    void LogicModel::setSelected(bool selected)
    {
        if (mSelected != selected)
        {
            mSelected = selected;

            if (mSelected)
                setBodyColour(Ogre::ColourValue::White);
            else
                resetBodyColour();
        }        
    }
    //-----------------------------------------------------------------------
    void LogicModel::setBodyColour( const Ogre::ColourValue& colour )
    {
        for ( EntityMap::iterator i = mEntityMap.begin();
            i != mEntityMap.end(); ++i )
        {  
            // 不改变头发的颜色
            if (i->first == HAIR_MESH_NAME)
                continue;

            EntityValue& tempValue = i->second;

            _setEntityColour(tempValue, colour);
        }
    }
    //-----------------------------------------------------------------------
    void LogicModel::_setEntityColour( EntityValue& entityValue, const Ogre::ColourValue& entityColour )
    {
        entityValue.mEntityColour = entityColour;
        entityValue.mEntityColourChanged = true;

        Ogre::Entity* workingEntity = entityValue.mEntity;

        if (workingEntity)
        {
            for (unsigned int subEntityI = 0;
                subEntityI < workingEntity->getNumSubEntities(); ++subEntityI)
            {
                Ogre::SubEntity* subEntity = workingEntity->getSubEntity(subEntityI);

                assert (subEntity);

                if (entityValue.mClonedMaterials.size() < subEntityI + 1)
                {
                    const Ogre::MaterialPtr& originMat = subEntity->getMaterial();

                    // 保留当前的颜色值
                    entityValue.mEntityOriginColour = _getColourFromMaterial(originMat);

                    Ogre::String newName;

                    // Avoid name collision
                    do {
                        newName = originMat->getName() + Ogre::StringConverter::toString(Ogre::Math::UnitRandom());
                    } while (Ogre::MaterialManager::getSingleton().resourceExists(newName));

                    Ogre::MaterialPtr newMat = originMat->clone(newName);

                    newMat->setSelfIllumination(entityValue.mEntityColour);

                    // 对subEntity设置材质
                    subEntity->setMaterialName(newName);

                    // 再clone一个originMat来作为mOrigin，这样，entity中的原材质就永远不会被改动
                    do {
                        newName = originMat->getName() + Ogre::StringConverter::toString(Ogre::Math::UnitRandom());
                    } while (Ogre::MaterialManager::getSingleton().resourceExists(newName));

                    Ogre::MaterialPtr oldMat = originMat->clone(newName);

                    // 送入临时材质数组，用于清理工作
                    entityValue.mClonedMaterials.push_back(newMat);
                    entityValue.mOriginMaterialNames.push_back(oldMat->getName());
                }
                else
                {
                    Ogre::MaterialPtr originMat = subEntity->getMaterial();

                    originMat->setSelfIllumination(entityValue.mEntityColour);
                }                    
            }

            // 更新透明度中的origin material
            EntityTransparentor* transparentor = entityValue.mTransparentor;
         
            if (transparentor)
            {
                // 更新origin material
                for (unsigned int subEntityIndex = 0; subEntityIndex < workingEntity->getNumSubEntities();
                    ++subEntityIndex)
                {
                    Ogre::SubEntity* pSubEntity = workingEntity->getSubEntity(subEntityIndex);

                    assert (pSubEntity);

                    ModelMaterialInstance* materialInstance = transparentor->mMaterialInstances[transparentor->mSubEntityMaterialIndices[subEntityIndex]];

                    if (materialInstance)
                    {
                //        const Ogre::String& subEntityMatName = pSubEntity->getMaterialName();

                //        const Ogre::String& transparentorMatName = materialInstance->mOrigin->getName();

                        materialInstance->mDerived->setSelfIllumination(entityValue.mEntityColour);
                        materialInstance->mOrigin->setSelfIllumination(entityValue.mEntityColour);

                        /*if (subEntityMatName != transparentorMatName)
                        {
                            const Ogre::MaterialPtr& subEntityMat = pSubEntity->getMaterial();
                            materialInstance->mOrigin = subEntityMat;
                        }*/
                    }
                }
            }
        }

    }
    //-----------------------------------------------------------------------
    void LogicModel::_clearEntityMaterials( EntityValue& entityValue )
    {
        // 删除clone过的material
        for (size_t cloneMatI = 0; cloneMatI < entityValue.mClonedMaterials.size();
            ++cloneMatI)
        {
            std::vector< Ogre::MaterialPtr >& materials = entityValue.mClonedMaterials;
            Ogre::MaterialPtr& mat = materials[cloneMatI];

            if (false == mat.isNull())
            {
                Ogre::MaterialManager::getSingleton().remove(mat->getHandle());
                mat.setNull();
            }
        }
        entityValue.mClonedMaterials.clear();

        for (size_t cloneMatI = 0; cloneMatI < entityValue.mOriginMaterialNames.size();
            ++cloneMatI)
        {
            Ogre::MaterialManager::getSingleton().remove(entityValue.mOriginMaterialNames[cloneMatI]);
        }
        entityValue.mOriginMaterialNames.clear();
    }
    //-----------------------------------------------------------------------
    void LogicModel::resetBodyColour(void)
    {
        for ( EntityMap::iterator i = mEntityMap.begin();
            i != mEntityMap.end(); ++i )
        {  
            // 不改变头发的颜色
            if (i->first == HAIR_MESH_NAME)
                continue;

            EntityValue& tempValue = i->second;

            tempValue.mEntityColour = Ogre::ColourValue::Black;
            tempValue.mEntityColourChanged = false;

            _setEntityColour(tempValue, tempValue.mEntityOriginColour);
        //    _resetEntityMaterial(tempValue);
        }
    }
    //-----------------------------------------------------------------------
    void LogicModel::_resetEntityMaterial( EntityValue& entityValue )
    {
        Ogre::Entity* workingEntity = entityValue.mEntity;

        if (workingEntity)
        {
            for (unsigned int subEntityI = 0;
                subEntityI < workingEntity->getNumSubEntities(); ++subEntityI)
            {
                Ogre::SubEntity* subEntity = workingEntity->getSubEntity(subEntityI);

                assert (subEntity);

                subEntity->setMaterialName( entityValue.mOriginMaterialNames[subEntityI] );                
            }
        }

        _clearEntityMaterials(entityValue);
    }
    //-----------------------------------------------------------------------
    void LogicModel::_updateAllEntitiesTransparency(void)
    {
        // 设置自身的entity
        for ( EntityMap::iterator itEntity = mEntityMap.begin();
            itEntity != mEntityMap.end(); ++itEntity )
        {
            Ogre::Entity* pEntity = itEntity->second.mEntity;
            EntityTransparentor* transparentor = itEntity->second.mTransparentor;

            if (pEntity)
            {
                if (transparentor)
                {
                    transparentor->apply(mCurrentTransparency);

         //           if (mCurrentTransparency <= 0)
         //           {
         //               delete itEntity->second.mTransparentor;
         //               itEntity->second.mTransparentor = 0;
         //           }
                }
            //    else if (mDestTransparency > 0)
                else
                {
                    itEntity->second.mTransparentor = new EntityTransparentor(pEntity);

                    itEntity->second.mTransparentor->apply(mCurrentTransparency);
                }
            }           
        }

        // 更新当前所有的布料的透明度
        FabricInfoMap::iterator fabricIterator = mFabricInfoMap.begin();

        while ( fabricIterator != mFabricInfoMap.end() )
        {
            Ogre::Fabric* fabric = fabricIterator->second.mFabric;
            FabricTransparentor* transparentor = fabricIterator->second.mTransparentor;

            if (fabric)
            {
                if (transparentor)
                {
                    transparentor->apply(mCurrentTransparency);
                }
                else
                {
                    fabricIterator->second.mTransparentor = new FabricTransparentor(fabric);
                    fabricIterator->second.mTransparentor->apply(mCurrentTransparency);
                }
            }  

            ++fabricIterator;
        }
    }
    //-----------------------------------------------------------------------
    void LogicModel::setTransparencyInfo(Real currTransparency, Real destTransparency, Real transparentTime)
    {
        if ( mCurrentTransparency != currTransparency ||
            mDestTransparency != destTransparency )
        {
            mCurrentTransparency = currTransparency;
            mDestTransparency = destTransparency;

            if (Ogre::Math::RealEqual(transparentTime, 0.0f))
                _updateAllEntitiesTransparency();
            else
            {
                mTransparencyTime = transparentTime;
                // 这里需要把这个flag设置为true，不然在execute中不update transparency
                mNeedUpdateTransparency = true;

                mDeltaTransparency = mDestTransparency - mCurrentTransparency;
            }
        }
    }
    //-----------------------------------------------------------------------
    void LogicModel::_createBoundingBoxFromExternal(void)
    {
        Ogre::StringVector values = Ogre::StringUtil::split(mExternalBoundingBoxValue, " ");

        if (values.size() != 3)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                "the bounding box parameter count must be 3!",
                "LogicModel::_createBoundingBoxFromExternal"); 

            return;
        }

        Real x = Ogre::StringConverter::parseReal(values[0]);
        Real y = Ogre::StringConverter::parseReal(values[1]);
        Real z = Ogre::StringConverter::parseReal(values[2]);

        Ogre::Vector3 minPoint;
        minPoint.x = -x / 2;
        minPoint.z = -z / 2;
        minPoint.y = 0.0f;

        Ogre::Vector3 maxPoint;
        maxPoint.x = x / 2;
        maxPoint.z = z / 2;
        maxPoint.y = y;

        mExternalBoundingBox = new Ogre::AxisAlignedBox(minPoint, maxPoint);
    }
    //-----------------------------------------------------------------------
    bool LogicModel::rayIntersect(const Ogre::Ray& ray)
    {
        if (mExternalBoundingBox)
        {
            // 用当前scene node来transform当前指定的boundingbox（因为当前的boundingbox都是局部坐标系的）
            Ogre::AxisAlignedBox intersectedBox(mExternalBoundingBox->getMinimum(), mExternalBoundingBox->getMaximum());

            intersectedBox.transformAffine(mModelMainNode->_getFullTransform());

            // 把转换后的boundingbox和ray进行检测
            return Ogre::Math::intersects(ray, intersectedBox).first;
        }
        else if (mFullBoundingBox)
        {
            // 用当前scene node来transform当前指定的boundingbox（因为当前的boundingbox都是局部坐标系的）
            Ogre::AxisAlignedBox intersectedBox(mFullBoundingBox->getMinimum(), mFullBoundingBox->getMaximum());

            intersectedBox.transformAffine(mModelMainNode->_getFullTransform());

            // 把转换后的boundingbox和ray进行检测
            return Ogre::Math::intersects(ray, intersectedBox).first;
        }        
        else
            return false;
    }
    //-----------------------------------------------------------------------
    void LogicModel::_setScaleFactor(const String& value)
    {
        Ogre::StringVector values = Ogre::StringUtil::split(value, " ");

        if (values.size() != 3)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                "the scale factor parameter count must be 3!",
                "LogicModel::_setScaleFactor"); 

            return;
        }

        setScaleFactor(Ogre::StringConverter::parseVector3(value));
    }
    //-----------------------------------------------------------------------
    bool LogicModel::_getTemplateEffectName(String& effectName, const String& templateName, float& height)
    {
        WX::TerrainTypeInfos* typeInfos = mSystem->getTerrainTypeInfos();
        if (!typeInfos)
            return false;

        Ogre::Vector3 modelPos = mModelMainNode->_getDerivedPosition();

        typeInfos->getEffectInfo(modelPos.x, modelPos.z, templateName, effectName, height);

        if (effectName.empty())
            return false;
        else
            return true;
    }
    //-----------------------------------------------------------------------
    Ogre::AnimationState* LogicModel::_getAnimationState(const String& animName)
    {
        // 先判断当前skeleton有没有这个animation
        // 这里不用判断mSkeletonEntity->getAllAnimationStates()不为空，因为能进到
        // 这个函数，说明mSkeletonEntity不为空，所以肯定是带骨骼的
        if (!mSkeletonEntity->getAllAnimationStates()->hasAnimationState(animName))
        {
            if ( false == mSkeletonEntity->getSkeleton()->hasAnimation(animName) )        
            {
                // 如果没有，就从animation skeleton link中获取要读取的skeleton的名称
                mSkeletonEntity->getSkeleton()->addLinkedSkeletonAnimationSource(
                    LogicModelManager::getSingleton().getSkeletonFromAnimationName(mName, animName) );
            }
            mSkeletonEntity->refreshAvailableAnimationState();
        }

        return mSkeletonEntity->getAnimationState(animName);
    }
    //---------------------------------------------------------------------
    void LogicModel::_getWorldHeight(float x, float z, float& y)
    {
        try
        {
            mGetHeightInWorld(x,z,y);
        }
        catch (...)
        {
            Ogre::StringUtil::StrStreamType errorString;

            errorString << "Get height in world callback exception!\n";
            errorString << "Model Name : " << mName << '\n';
            errorString << "x : " << Ogre::StringConverter::toString(x) << '\n';
            errorString << "z : " << Ogre::StringConverter::toString(z) << '\n';
            errorString << "Function Name : " << "LogicModel::_getWorldHeight";

            Ogre::LogManager::getSingleton().logMessage(errorString.str());

            throw;
        }
    }
    //---------------------------------------------------------------------
    void LogicModel::_executeAttachedModel(Real delta)
    {
        AttachedModelOriginSceneNodes::const_iterator it = 
            mAttachedModelOriginSceneNodes.begin();

        while (it != mAttachedModelOriginSceneNodes.end())
        {
            if (it->first)
                it->first->execute(delta);

            ++it;
        }
    }
    //---------------------------------------------------------------------
    void LogicModel::addFabric(
        const String& fabricName,
        int width, int height,
        Real mass, Real naturalLength,
        Real structuralStiffness,
        Real shearStiffness,
        Real flexionStiffness,
        Real deformationRate,
        const Ogre::Vector3& windVelocity,
        const String& materialName,
        const String& headLocatorName,
        const String& tailLocatorName)
    {
        // 先搜索有没有同名的fabric存在
        FabricInfoMap::iterator fabricIterator = mFabricInfoMap.find(fabricName);

        // 如果有，先删除之前创建的fabric
        if (fabricIterator != mFabricInfoMap.end())
        {
            removeFabric(fabricName);
        }

        // 先查找要挂接的locator是否存在
        LocatorValue headLocatorValue, tailLocatorValue;

        if ( getLocatorInfo(headLocatorName, headLocatorValue) &&
             getLocatorInfo(tailLocatorName, tailLocatorValue) )
        {
            Ogre::Fabric* fabric = static_cast<Ogre::Fabric*>(
                mSystem->getSceneManager()->createMovableObject(mModelMainNode->getName() + "_" + fabricName,
                ScorpioFabricFactory_TypeName) );

            // 设置为当前的visible flag
            fabric->setVisibilityFlags(mVisibleFlag);

            mModelMainNode->attachObject(fabric);

            Ogre::SceneNode* headLocatorNode = _getLocatorSceneNode(headLocatorValue);
            Ogre::SceneNode* tailLocatorNode = _getLocatorSceneNode(tailLocatorValue);

            setLocatorInfo(headLocatorName, headLocatorValue);
            setLocatorInfo(tailLocatorName, tailLocatorValue);

            Ogre::Vector3 headLocatorpos = headLocatorNode->_getDerivedPosition();
            //Ogre::Vector3 tailLocatorpos = tailLocatorValue.mLocatorNode->_getDerivedPosition();

            //Ogre::Vector3 xAxis = (tailLocatorpos - headLocatorpos).normalisedCopy();

            //Ogre::Vector3 zAxis = xAxis.crossProduct(Ogre::Vector3::UNIT_Y);

            //Ogre::Quaternion originOri(xAxis, Ogre::Vector3::UNIT_Y, zAxis);

            fabric->setPhysicsToDisplayScale(100);

            // Setup maximise movement tracking for numric stable
            fabric->setMaxTrackingDistance(100);
            fabric->setMaxTrackingAngle(Ogre::Degree(30));

            fabric->createRectanglePiece(
                width, height,
                mass, naturalLength,
                structuralStiffness, shearStiffness, flexionStiffness,
                deformationRate,
                true, false, headLocatorpos / 100/*, originOri*/);

            fabric->trackParticle(0, headLocatorNode);
            fabric->trackParticle(width - 1, tailLocatorNode);

            fabric->setMaterialName(materialName);
            fabric->setTimeSteps(0.05f, 0.02f, 0.05f);
            fabric->setWindVelocity(windVelocity);

            fabric->fastForward(1, 0.03);

            // 如果当前还未创建出碰撞器的集合，就创建，并创建在obj文件中定义的所有collider
            if (NULL == mColliderSet)
                _initFabricColliders();

            if (mColliderSet)
                fabric->setColliderSet(mColliderSet);

            FabricInfo fabricInfo(fabric);

            // 如果当前处于透明状态,就设置布料的透明度
            if (mCurrentTransparency > 0.0f)
            {
                fabricInfo.mTransparentor = new FabricTransparentor(fabricInfo.mFabric);
                fabricInfo.mTransparentor->apply(mCurrentTransparency);
            }

            mFabricInfoMap.insert( FabricInfoMap::value_type(fabricName, fabricInfo) );
        }
    }
    //---------------------------------------------------------------------
    void LogicModel::removeFabric(const String& fabricName)
    {
        FabricInfoMap::iterator fabricIterator = mFabricInfoMap.find(fabricName);

        // 如果有，先删除之前创建的fabric
        if (fabricIterator != mFabricInfoMap.end())
        {
            mModelMainNode->detachObject(fabricIterator->second.mFabric);

            mSystem->getSceneManager()->destroyMovableObject(fabricIterator->second.mFabric);

            delete fabricIterator->second.mTransparentor;

            mFabricInfoMap.erase(fabricIterator);
        }
    }
    //---------------------------------------------------------------------
    void LogicModel::removeAllFabrics(void)
    {
        FabricInfoMap::iterator fabricIterator = mFabricInfoMap.begin();

        while ( fabricIterator != mFabricInfoMap.end() )
        {
            mModelMainNode->detachObject(fabricIterator->second.mFabric);

            mSystem->getSceneManager()->destroyMovableObject(fabricIterator->second.mFabric);

            delete fabricIterator->second.mTransparentor;

            ++fabricIterator;
        }

        mFabricInfoMap.clear();
    }
    //---------------------------------------------------------------------
    void LogicModel::_destroyColliders(void)
    {
        if (mColliderSet)
        {
            FabricColliderMap::iterator i = mFabricColliderMap.begin();

            while ( i != mFabricColliderMap.end() )
            {
                FabricCollider& collider = i->second;

                if (collider.mCollider)
                {
                    LogicModelManager::getSingleton().getCollisionManager()->destroyCollider(collider.mCollider); 
                }  

                ++ i;
            }

            LogicModelManager::getSingleton().getCollisionManager()->destroyColliderSet(mColliderSet);
        }
    }
    //---------------------------------------------------------------------
    bool LogicModel::addFabricCollider( const String& colliderName, const String& locatorName,
        FabricColliderType colliderType, const String& colliderInfo )
    {
        FabricColliderMap::iterator i = mFabricColliderMap.find(colliderName);

        if ( i != mFabricColliderMap.end() )
        {
            Ogre::LogManager::getSingleton().logMessage( "Logic Model Fabric Collider with name '"
                + colliderName + "' already exists! " +
                "LogicModel::addFabricCollider " + mName );

            return false;
        }

        std::pair<FabricColliderMap::iterator, bool> inserted = 
            mFabricColliderMap.insert( FabricColliderMap::value_type( 
            colliderName, FabricCollider(locatorName, colliderType, colliderInfo) ) );

        if (false == inserted.second)
        {
            Ogre::LogManager::getSingleton().logMessage( "Logic Model Fabric Collider with name '"
                + colliderName + "' inserted failed! " +
                "LogicModel::addFabricCollider " + mName );

            return false;
        }

        return true;
    }
    //---------------------------------------------------------------------
    void LogicModel::removeFabricCollider( const String& colliderName )
    {
        if (mColliderSet)
        {
            if (colliderName == "TerrainCollider")
                mColliderSet->removeCollider( LogicModelManager::getSingleton().getTerrainCollider() );
            else
            {
                FabricColliderMap::const_iterator i = mFabricColliderMap.find(colliderName);

                if ( i != mFabricColliderMap.end() )
                {
                    const FabricCollider& collider = i->second;
                    
                    if (collider.mCollider)
                    	mColliderSet->removeCollider(collider.mCollider);
                }
            }
        }
    }
    //---------------------------------------------------------------------
    void LogicModel::_initFabricColliders(void)
    {
        Ogre::CollisionManager* cm = LogicModelManager::getSingleton().getCollisionManager();

        if (cm)
        {
            mColliderSet = cm->createColliderSet();

            FabricColliderMap::iterator i = mFabricColliderMap.begin();

            while ( i != mFabricColliderMap.end() )
            {
                FabricCollider& collider = i->second;

                ++ i;

                // 获取locator指针
                LocatorValue locatorValue;
                if ( getLocatorInfo(collider.mLocatorName, locatorValue) )
                {
                    collider.mLocatorNode = _getLocatorSceneNode(locatorValue);

					// 把改变后的locator又重新设置回去
                    setLocatorInfo(collider.mLocatorName, locatorValue);
                }
                else
                    continue;

                switch (collider.mColliderType)
                {
                case FCT_BOX:
                    {
                        collider.mCollider = cm->createBoxCollider( Ogre::StringConverter::parseVector3(collider.mColliderInfo) );
                        break;
                    }
                case FCT_SPHERE:
                    {
                        collider.mCollider = cm->createSphereCollider( Ogre::StringConverter::parseReal(collider.mColliderInfo) );
                        break;
                    }
                case FCT_PLANE:
                    {
                        collider.mCollider = cm->createPlaneCollider();
                        break;
                    }
                default:
                    {
                        continue;
                        break;
                    }
                }

                mColliderSet->addCollider(collider.mCollider);
            }

            // 添加地形collider
            mColliderSet->addCollider( LogicModelManager::getSingleton().getTerrainCollider() );
        }
    }
    //---------------------------------------------------------------------
    void LogicModel::_updateFabricColliders(void)
    {
        FabricColliderMap::iterator i = mFabricColliderMap.begin();

        while ( i != mFabricColliderMap.end() )
        {
            FabricCollider& collider = i->second;

            if (collider.mCollider && collider.mLocatorNode)
            {
                collider.mCollider->setPosition( collider.mLocatorNode->_getDerivedPosition() / 100 );

                // 如果是平面类型，就设置collider的rotation
                if (collider.mColliderType == FCT_PLANE)
                {
                    // 设置plane的方向为locator的反方向
                    const Ogre::Quaternion& ori = collider.mLocatorNode->_getDerivedOrientation();

					// 再加上一个倾斜的角度,0.25大概是13度
                    Ogre::Quaternion angle(Ogre::Quaternion::IDENTITY);

                    angle.FromAngleAxis(Ogre::Radian(0.25), Ogre::Vector3::NEGATIVE_UNIT_X);

                    collider.mCollider->setRotation( ori * angle );
                }
            }  

            ++ i;
        }
    }
    //---------------------------------------------------------------------
    void LogicModel::enableAnimationState(bool enable)
    {
        if (mCurrentAnimationState && mCurrentAnimationState->getEnabled() != enable)
            mCurrentAnimationState->setEnabled(enable);

        if (mFadeOutAnimationState && mFadeOutAnimationState->getEnabled() != enable)
            mFadeOutAnimationState->setEnabled(enable);
    }
    //---------------------------------------------------------------------
    Ogre::SceneNode* LogicModel::_createLocatorSceneNode(const Ogre::Vector3& offsetPos, 
        const Ogre::Quaternion& offsetOri)
    {
        static uint count = 0;
        Ogre::StringUtil::StrStreamType str;
        str << "Locator_" << std::setw(5) << std::setfill('0') << count++ << std::ends;
        Ogre::SceneNode* locatorNode = mModelMainNode->createChildSceneNode(str.str());
        locatorNode->translate( offsetPos );
        locatorNode->setOrientation( offsetOri );

        return locatorNode;
    }
    //---------------------------------------------------------------------
    Ogre::SceneNode* LogicModel::_getLocatorSceneNode(LocatorValue& locatorValue)
    {
        if ( !(locatorValue.mLocatorNode) )
        {
            locatorValue.mLocatorNode =
                _createLocatorSceneNode(locatorValue.mOffsetPos, locatorValue.mOffsetOrientation);
        }

        return locatorValue.mLocatorNode;
    }
    //---------------------------------------------------------------------
    bool LogicModel::isCameraVisible(void)
    {
        bool visible = true;

        if ( mFullBoundingBox && false == isChildModel() && false == isAttached() && 
            getVisibleFlag() == OVF_DEFAULT )
        {
#if 0
            // 使用内部计算的bounding box
            Ogre::AxisAlignedBox intersectedBox(mFullBoundingBox->getMinimum(), mFullBoundingBox->getMaximum());

            intersectedBox.transformAffine(mModelMainNode->_getFullTransform());

            visible = mSystem->getCamera()->isVisible(intersectedBox);

#else

            // 使用scene node在_update中计算的bounding box
            visible = mSystem->getCamera()->isVisible( mModelMainNode->_getWorldAABB());
#endif
        }

        return visible;
    }
    //---------------------------------------------------------------------
    Ogre::StringVector LogicModel::getOriginMeshNames(void)
    {
        Ogre::StringVector nameVector;

        EntityMap::iterator i = mEntityMap.begin();

        while ( i != mEntityMap.end() )
        {
            const EntityValue& entity = i->second;

            if (false == entity.mMeshName.empty())
                nameVector.push_back(entity.mMeshName);

            ++i;
        }

        return nameVector;
    }
    //---------------------------------------------------------------------
    void LogicModel::playAnimation(const String& animName, Real startTime, Real weight)
    {
        // 检查列表中是否有这个动作，如果有的话，把旧的删除掉
        PlayingAnimationList::iterator animIterator = mPlayingAnimationList.begin();

        while (animIterator != mPlayingAnimationList.end())
        {
            Ogre::AnimationState* state = *animIterator;

            assert (state);

            if (state->getAnimationName() == animName)
            {
                state->setEnabled(false);
                mPlayingAnimationList.erase(animIterator);
                break;
            }

            ++animIterator;
        }

        Ogre::AnimationState* state = NULL;

        try
        {
            // 有可能因为资源的原因，缺少所需的skeleton文件，所以这个entity为空
            if (mSkeletonEntity)
            {
                state = _getAnimationState(animName);
                mSkeleton->getAnimation( animName )->setInterpolationMode( 
                    LogicModelManager::getSingleton().getAnimationInterpolationMode() );
            }
            else
            {
                Ogre::LogManager::getSingleton().logMessage( "Logic Model : " + mName + 
                    " Skeleton Entity is NULL, Maybe the skeleton file is lost!" +
                    "LogicModel::playAnimation");
                return;
            }

        }
        catch (const Ogre::Exception& e)
        {
            Ogre::LogManager::getSingleton().logMessage(e.getFullDescription() + "LogicModel::playAnimation!");

            if (mSystem->getThrowExceptionIfResourceIncorrect())//目前无调用函数
            {
                OGRE_EXCEPT(Ogre::Exception::ERR_RT_ASSERTION_FAILED,
                    "Resource was incorrectness due incaution producer. "
                    "Full error description: " + e.getFullDescription(),
                    "LogicModel::playAnimation");
            }

            return;
        }

        if(state)
        {         
            state->setEnabled(true);
            state->setLoop(false);
            state->setTimePosition(startTime);
            state->setWeight(weight);

            mPlayingAnimationList.push_back(state);
        }
    }
    //---------------------------------------------------------------------
    void LogicModel::_updatePlayingAnimations(Real deltaTime)
    {
        PlayingAnimationList::iterator animIterator = mPlayingAnimationList.begin();

        while (animIterator != mPlayingAnimationList.end())
        {
            Ogre::AnimationState* state = *animIterator;

            assert (state);

            // 如果这个动作已经播放完毕，就把它从列表中删除
            if (state->getTimePosition() >= state->getLength())
            {
                state->setEnabled(false);
                animIterator = mPlayingAnimationList.erase(animIterator);
            }
            else
            {
                // 否则，继续执行这个动作
                state->addTime(deltaTime * mGlobalAnimationRate);
                ++animIterator;
            }
        }
    }
    //---------------------------------------------------------------------
    bool LogicModel::_convertEntityToCharacterMaterial(Ogre::Entity* entity)
    {
        //    return false;

        // 利用一个static变量,来保证_checkForHardwareSkinningMaterialSupport只会被调一次
        static bool materialSupport = _checkForHardwareSkinningMaterialSupport();

        if (!materialSupport)
            return false;

        // 检测该entity是否带骨骼
        if (entity->isHardwareAnimationEnabled())
            return false;

        if (!entity->hasSkeleton() || entity->hasVertexAnimation())
            return false;

        // 目前只有一种program（hlsl）
        const String postfix = "_hlsl";

        // 对entity中的每个sub entity都进行检测，只有每个sub entity都符合要求才能转换
        std::vector<Ogre::MaterialPtr> derivedMaterials;
        for (unsigned short i = 0; i < entity->getNumSubEntities(); ++i)
        {
            Ogre::SubEntity* se = entity->getSubEntity(i);
            Ogre::SubMesh* sm = se->getSubMesh();

            Ogre::MaterialPtr origin = Ogre::MaterialManager::getSingleton().getByName(se->getMaterialName());
            if (origin.isNull())
                return false;

            if (origin->getNumTechniques() != 1)
                return false;

            Ogre::Technique* technique = origin->getTechnique(0);
            if (technique->getNumPasses() != 1)
                return false;

            Ogre::Pass* pass = technique->getPass(0);
            if (pass->hasVertexProgram())
            {
                if (!pass->getVertexProgram()->isSkeletalAnimationIncluded())
                    return false;
                if (!pass->getVertexProgram()->isSupported())
                    return false;

                derivedMaterials.push_back(origin);
                continue;
            }

            // 只有材质中specular是有数值的，才使用specular
            bool useSpecular = true;

            if (pass->getSpecular() == Ogre::ColourValue::Black)
                useSpecular = false;

            if (pass->getNumTextureUnitStates() != 1)
                return false;

            const Ogre::VertexData* vertexData;
            const Ogre::Mesh::IndexMap* indexMap;
            if (sm->useSharedVertices)
            {
                vertexData = sm->parent->sharedVertexData;
                indexMap = &sm->parent->sharedBlendIndexToBoneIndexMap;
            }
            else
            {
                vertexData = sm->vertexData;
                indexMap = &sm->blendIndexToBoneIndexMap;
            }

            const Ogre::VertexElement* blendWeights =
                vertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_BLEND_WEIGHTS);
            if (!blendWeights)
                return false;

            size_t numWeightsPerVertex = Ogre::VertexElement::getTypeCount(blendWeights->getType());
            size_t numWorldMatrices = indexMap->size();

            // 默认使用的是vs 1.1版的shader
            String vsVersion = "_VS_1_1";

            // 如果某个sub entity的骨头个数超过了25
            if (numWorldMatrices > 25)
            {
                // 而且当前机器支持vs 2.0，那就使用vs 2.0的shader
                if ( Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("vs_2_0") && numWorldMatrices < 75 )
                    vsVersion = "_VS_2_0";
                else
                    return false;
            }
            
            String prefix = "Character_HardwareSkinning";

            static const String weightNames[] = { "_OneWeight", "_TwoWeight", "_ThreeWeight", "_FourWeight" };
            const String& weightName = weightNames[numWeightsPerVertex-1];
            prefix += weightName + vsVersion;

            Ogre::MaterialPtr derived = _deriveHardwareSkinningMaterial(origin, prefix, postfix, useSpecular);
            derivedMaterials.push_back(derived);
        }

        // Change the materials
        for (unsigned short i = 0; i < entity->getNumSubEntities(); ++i)
        {
            Ogre::SubEntity* se = entity->getSubEntity(i);
            se->setMaterialName(derivedMaterials[i]->getName());
        }

        return true;
    }
    //---------------------------------------------------------------------
    Ogre::MaterialPtr LogicModel::_deriveHardwareSkinningMaterial(const Ogre::MaterialPtr& origin,
        const String& prefix, const String& postfix, bool useSpecular)
    {
        String derivedMaterialName = origin->getName() +
            " : " + prefix + "_";

        Ogre::MaterialPtr derived = Ogre::MaterialManager::getSingleton().getByName(derivedMaterialName);

        if (derived.isNull())
        {
            derived = origin->clone(derivedMaterialName);

            Ogre::Technique* newTechnique = derived->createTechnique();
            Ogre::Technique* technique = derived->getTechnique(0);
            *newTechnique = *technique;

            String baseName = prefix;

            Ogre::Pass* pass = technique->getPass(0);
            
            if (useSpecular)
            {
                pass->setVertexProgram(baseName + "_PixelSpecular" + postfix);

                if ( Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("ps_2_0") )
                    pass->setFragmentProgram("Character_HardwareSkinning_PixelSpecular_PS_2_0_hlsl");
            }
            else
            {
                pass->setVertexProgram(baseName + postfix);

                if ( Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("ps_1_1") )
                    pass->setFragmentProgram("Character_HardwareSkinning_PS_1_1_hlsl");
            }

            pass->setShadowCasterVertexProgram(baseName + "_AmbientOnly" + postfix);
            pass->getShadowCasterVertexProgramParameters()->setNamedAutoConstant("ambient", Ogre::GpuProgramParameters::ACT_AMBIENT_LIGHT_COLOUR);
            pass->setShadowReceiverVertexProgram(baseName + "_Unlit" + postfix);
        }

        LogicModelManager::getSingleton().addToMaterialPool(derived->getHandle());

        return derived;
    }
    //---------------------------------------------------------------------
    bool LogicModel::_checkForHardwareSkinningMaterialSupport(void)
    {
        // 随便挑一个program来检测.program是否被正确读入
        Ogre::GpuProgramPtr vp =
            Ogre::GpuProgramManager::getSingleton().getByName("Character_HardwareSkinning_OneWeight_VS_1_1_PixelSpecular_hlsl");
        if ( vp.isNull() )
            return false;
        else
        {
            vp->load();

            if ( false == vp->isSupported() )
                return false;
        }

        return true;
    }
    //---------------------------------------------------------------------
    float LogicModel::getAnimationLength(const String& animName) const
    {
        String realAnimName;

        Skill *skillTemplate = WX::EffectManager::getSingleton().getSkill(animName);

        if (skillTemplate)
        {
            realAnimName = skillTemplate->getAnimationName();
        }
        else
        {
            realAnimName = animName;
        }

        if (mSkeletonEntity)
        {
            Ogre::Animation* animation = NULL;

            if (mSkeletonEntity->getSkeleton()->hasAnimation(realAnimName))
            {
                animation = mSkeletonEntity->getSkeleton()->getAnimation(realAnimName);

                return animation->getLength();
            }
            else
            {
                try
                {
                    Ogre::SkeletonPtr skeleton = Ogre::SkeletonManager::getSingleton().load( 
                        LogicModelManager::getSingleton().getSkeletonFromAnimationName(mName, realAnimName), 
                        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );

                    animation = skeleton->getAnimation(realAnimName);

                    return animation->getLength();
                }
                catch (const Ogre::Exception& e)
                {
                    Ogre::LogManager::getSingleton().logMessage(e.getFullDescription());

                    if (mSystem->getThrowExceptionIfResourceIncorrect())//目前无调用函数
                    {
                        OGRE_EXCEPT(Ogre::Exception::ERR_RT_ASSERTION_FAILED,
                            "Resource was incorrectness due incaution producer. "
                            "Full error description: " + e.getFullDescription(),
                            "LogicModel::getAnimationLength");
                    }

                    return -1.0f;
                }
            }
        }
        else
            return -1.0f;
    }
    //---------------------------------------------------------------------
    void LogicModel::setCurrentAnimTime(Real range)
    {
        assert (range >= 0.0f && range <= 1.0f);

        if (mCurrentAnimationState)
        {
            mCurrentAnimationState->setTimePosition( mCurrentAnimationState->getLength() * range );
        }
    }
    //---------------------------------------------------------------------
    void LogicModel::setScaleFactor(const Ogre::Vector3& scaleFactor)
    {
        if (!mExternalScaleFactor.positionEquals(scaleFactor))
        {
            mExternalScaleFactor = scaleFactor;

            // child model是不需要把缩放因子设置到节点中的，因为缩放因子会传递下去
            if (false == isChildModel() && mModelMainNode)
                mModelMainNode->setScale(mExternalScaleFactor);

            setEntityNormalizeNormal(!mExternalScaleFactor.positionEquals(Ogre::Vector3::UNIT_SCALE));
        }
    }
    //---------------------------------------------------------------------
    void LogicModel::setEntityNormalizeNormal(bool normal)
    {
        // 设置自身的entity
        for ( EntityMap::iterator i = mEntityMap.begin();
            i != mEntityMap.end(); ++i )
        {
            Ogre::Entity* workingEntity = i->second.mEntity;

            if (workingEntity)
            {
                workingEntity->setNormaliseNormals(normal);
            }
        }

        // 设置locator上的model
        for ( LocatorMap::iterator itLocator = mLocatorMap.begin();
            itLocator != mLocatorMap.end(); ++itLocator )
        {
            LocatorValue& tempValue = itLocator->second;

            for ( SlotMap::iterator itSlot = tempValue.mSlotMap.begin();
                itSlot != tempValue.mSlotMap.end(); ++itSlot )
            {
                SlotValue& tempSlot = itSlot->second;

                if (tempSlot.mModel)
                    tempSlot.mModel->setEntityNormalizeNormal(normal);
            }
        }
    }


}
