/********************************************************************
filename:   WXLogicModel.h

purpose:    LogicModel类是用于替换原来DObject的，其主要作用是渲染层与
			客户端的一个接合层，利用渲染层，实现客户端对场景中
			动态物体的一些需求（如换装，挂特效等），尽量是客户端可以很
			方便地实现各种效果，而不需调用渲染层的代码。
*********************************************************************/

#ifndef _FAIRYLOGICMODEL_H_
#define _FAIRYLOGICMODEL_H_

#include "Core/WXPrerequisites.h"
#include "Core/WXVariant.h"
#include "Core/TerrainData.h"

#include "WXLogicModelManager.h"

#include "EffectSystem/WXEffect.h"

#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <OgreAny.h>
#include <OgreStringVector.h>
#include <OgreMaterial.h>
#include <OgreRay.h>
#include <OgreIteratorWrappers.h>

// 动作运行过程中的回调函数
typedef bool          (*OnAnimationFinish)(const char* pAnimationName, unsigned long info);

typedef bool          (*OnSkillHitTime)(const char* pAnimationName, unsigned long info,
                                        unsigned short index, unsigned short totalCount);

typedef bool          (*OnSkillBreakTime)(const char* pAnimationName, unsigned long info,
                                          unsigned short index, unsigned short totalCount);

typedef bool          (*OnSkillShakeTime)(const char* pAnimationName, unsigned long info);
typedef bool          (*OnGetEffectName)(const char* pAnimationName, Ogre::String& effectName);


// 获取游戏世界中某一x，z坐标的高度值
typedef bool         (__stdcall*GetHeightInWorld)(float x, float z, float& y);

// 声音函数指针
typedef int (__stdcall* OnPlaySound)(const char* szSoundFile, float* fvPos, bool isLoop);
typedef void (__stdcall* OnStopSound)(int handle);

namespace Ogre	{

	class Entity;
	class Any;
	class UserDefinedObject;
	class SkeletonInstance;
	class Node;
	class AnimationState;
	class Animation;
    class AxisAlignedBox;

    /// 布料所需的类声明
    class Fabric;
    class StdCollider;
    class ColliderSet;
}

namespace WX	{

    const Ogre::ColourValue& _getColourFromMaterial(const Ogre::MaterialPtr& mat);

    enum ModelProjectorType
    {
        MPT_SELECTED,   // 被选中时显示的的projector
        MPT_SHADOW,     // 地下的简单阴影
    };

	class System;
	class Effect;
	class Skill;
   // class LogicModelManager;

	class LogicModel
	{
        // 定义LogicModelManager为友元类，直接存储私有成员
        friend class LogicModelManager;

	public:

		/// 设置属性时的属性类型
		static String MESH_HINT;
		static String MATERIAL_HINT;
		static String SLMT_CREATE_HINT;

		/// 特殊属性名称
		static String MOVEMENT_TYPE_ATTRIBUTE;
        static String DEFAULT_ANIM_NAME;
        static String BOUNDING_BOX;
        static String SCALE_FACTOR;

        static String HAIR_MESH_NAME;

		/// addEffect时返回的特效句柄
		typedef uint EffectHandle;

		/// 获取locator信息时的标志
		enum GetTransformInfoType
		{
			GTIT_POSITION,	// 只获取位置信息
			GTIT_ORIENTATION,	// 只获取旋转信息
			GTIT_ALL,	// 获取位置信息和旋转信息
		};

		// object类型，主要决定了object的行走方式
		enum MovementType
		{
			MT_NORMAL, // 普通类型，不会随地形修正object的方位
			MT_ROTATE_X_AXIS,	// 只能前后摇摆
			MT_ROTATE_X_Z_AXIS,	// 可以前后左右摇摆
		};

		/// 已创建的effect的信息，用于updateAllEffects
		struct CreatedEffectInfo
		{
			CreatedEffectInfo() :
			mEffect(NULL),
			mLocator(""),
            mTransformType(GTIT_POSITION),
            mEffectHandle(0)
			{
			}

			CreatedEffectInfo(Effect* effect, const String& locatorName, 
                GetTransformInfoType transformType, EffectHandle handle) :
			mEffect(effect),
			mLocator(locatorName),
            mTransformType(transformType),
            mEffectHandle(handle)
			{
			}

			Effect* mEffect;
			String mLocator;
            GetTransformInfoType mTransformType;
            EffectHandle mEffectHandle;
		};

        typedef std::list<CreatedEffectInfo> CreatedEffectList;

        /// slot属性
        struct SlotValue
        {
            SlotValue() : mModelName(""), mModel(NULL)
            {
            }

            SlotValue( const String& modelName ) :
            mModelName(modelName),
                mModel(NULL)
            {
            }

            ~SlotValue(void)
            {
                if (mModel)
                    LogicModelManager::getSingleton().destroyLogicModel(mModel);
            }

            String mModelName;
            LogicModel* mModel;
        };

        typedef std::map<String, SlotValue> SlotMap;

        /// locator信息
        struct	LocatorValue
        {
            LocatorValue() : 
            mBoneName(""),
                mOffsetPos(Ogre::Vector3::ZERO),
                mOffsetOrientation(Ogre::Quaternion::IDENTITY),
                mLocatorNode(NULL),
                mTransferable(false),
                mTranslateFirst(false)
            {
                mSlotMap.clear();
            }

            LocatorValue(const String& boneName, const Ogre::Vector3& pos, 
                const Ogre::Quaternion& orientation, bool transferable = false,
                bool translateFirst = false) :
            mBoneName(boneName),
                mOffsetPos(pos),
                mOffsetOrientation(orientation),
                mLocatorNode(NULL),
                mTransferable(transferable),
                mTranslateFirst(translateFirst)
            {
                mSlotMap.clear();
            }

            LocatorValue& operator = (const LocatorValue& rhs)
            {
                this->mBoneName = rhs.mBoneName;
                this->mOffsetPos = rhs.mOffsetPos;
                this->mOffsetOrientation= rhs.mOffsetOrientation;
                this->mLocatorNode = rhs.mLocatorNode;
                this->mTransferable = rhs.mTransferable;
                this->mTranslateFirst = rhs.mTranslateFirst;

                return *this;
            }

            String	mBoneName;
            Ogre::Vector3		mOffsetPos;
            Ogre::Quaternion	mOffsetOrientation;
            Ogre::SceneNode*	mLocatorNode;
            SlotMap			mSlotMap;
            bool mTransferable;
            bool mTranslateFirst;   // 在update locator pos时是否先进行translate
        };

        typedef std::map<String, LocatorValue> LocatorMap;

        /// 布料碰撞体类型
        enum FabricColliderType
        {
            FCT_BOX,
            FCT_SPHERE,
            FCT_PLANE,
        };

        /// 布料碰撞体所需的信息
        struct FabricCollider 
        {
            FabricCollider(const String& locator, FabricColliderType type, const String& colliderInfo) :
            mLocatorName(locator), mColliderType(type), mColliderInfo(colliderInfo),
            mLocatorNode(NULL), mCollider(NULL)
            {
            }

            String mLocatorName;    // 碰撞体所需的挂接点的名称
            FabricColliderType mColliderType;   // 碰撞体类型
            String mColliderInfo;   // 碰撞体的额外信息，根据不同碰撞体而定，如box类型的需要三个浮点数来定义长宽高。
            Ogre::SceneNode* mLocatorNode;  // 保留locator的指针
            Ogre::StdCollider* mCollider;   // 创建出来的碰撞体的指针
        };

        /// 保存布料碰撞体信息的map
        typedef std::map<String, FabricCollider> FabricColliderMap;

        /// 角色的属性值
        struct AttribValue
        {
            AttribValue() : mAttribValue(""), mHint(0)
            {
            }

            AttribValue(const String& value, const String& hint) :
            mAttribValue(value),
                mHint(hint)
            {
            }

            String mAttribValue;
            String mHint;
        };
        /// 属性map，key是属性名，value是属性值信息
        typedef std::map<String, AttribValue> AttribMap;

	protected:

        /// 透明所需用的材质实例
        struct ModelMaterialInstance
        {
            ModelMaterialInstance(const Ogre::MaterialPtr& origin);
            ~ModelMaterialInstance();

            Ogre::MaterialPtr mOrigin;
            Ogre::MaterialPtr mDerived;
        };

        /// 透明器
        class EntityTransparentor
        {
        protected:
            Ogre::Entity* mEntity;

            typedef std::vector<ModelMaterialInstance*> MaterialInstanceList;
            typedef std::vector<size_t> IndexList;
            size_t addMaterial(const Ogre::MaterialPtr& material);

            bool applyTransparency(const Ogre::MaterialPtr& origin, const Ogre::MaterialPtr& derived, Real transparency);

        public:
            EntityTransparentor(void);

            EntityTransparentor(Ogre::Entity* entity);

            ~EntityTransparentor();

            void clear(void);

            void init(Ogre::Entity* entity);

            void apply(Real transparency);

        public:
            MaterialInstanceList mMaterialInstances;
            IndexList mSubEntityMaterialIndices;
        };

		/** typedef	*/
		
		/// 普通角色所带的entity的列表
		struct EntityValue
		{
			EntityValue() : mMeshName(""), mMaterialName(""), mEntity(NULL), mTransparentor(NULL),
                mEntityColour(Ogre::ColourValue::Black), mEntityOriginColour(Ogre::ColourValue::Black),
                mEntityColourChanged(false)
			{
			}

			EntityValue(const String& meshName, const String& matName) :
			mMeshName(meshName),
				mMaterialName(matName),
				mEntity(NULL),
                mTransparentor(NULL),
                mEntityColour(Ogre::ColourValue::Black),
                mEntityOriginColour(Ogre::ColourValue::Black),
                mEntityColourChanged(false)
			{
			}

			String mMeshName;
			String mMaterialName;
			Ogre::Entity* mEntity;
            EntityTransparentor* mTransparentor;
            /// 保存clone过的材质
            std::vector< Ogre::MaterialPtr > mClonedMaterials;
            /// 保存clone之前用的材质
            std::vector< String > mOriginMaterialNames;

            /// 这个entity所设置的颜色值
            Ogre::ColourValue mEntityColour;
            /// entity设置颜色之前的颜色值
            Ogre::ColourValue mEntityOriginColour;
            bool mEntityColourChanged;
		};

		typedef std::map<String, EntityValue> EntityMap;

		/// material属性
		struct MaterialValue
		{
			MaterialValue() : mEntityEntry(""), mMaterialName("")
			{
			}

			MaterialValue(const String& entityEntry, const String& matName) :
			mEntityEntry(entityEntry),
			mMaterialName(matName)
			{
			}

			String mEntityEntry;
			String mMaterialName;
		};

		typedef std::map<String, MaterialValue> MaterialMap;

		/// 记录obj文件中Effects项的信息
		struct EffectValue
		{
            EffectValue() :	mEffectName(""), mLocator(""), mTranslateAll(false), mEffectColour(Ogre::ColourValue::White)
			{
			}

            EffectValue(const String& effectName, const String& locatorName, bool translateAll, const Ogre::ColourValue& colour) :
			mEffectName(effectName),
				mLocator(locatorName),
                mTranslateAll(translateAll),
                mEffectColour(colour)
			{
			}

			String mEffectName;
			String mLocator;
            bool mTranslateAll;
            Ogre::ColourValue mEffectColour;
		};
		typedef std::map<String, EffectValue> EffectMap;			

		/// 被挂接上的物体原来的node
		typedef std::map<LogicModel*, Ogre::Node*> AttachedModelOriginSceneNodes;

        /// 挂在model上的projector
        struct ModelProjector
        {
            ModelProjector() : mProjectorName(""), mType(MPT_SELECTED), mProjectorEffect(NULL), mProjectorNode(NULL)
            {
            }

            ModelProjector(const String& projectorName, ModelProjectorType type) :
            mProjectorName(projectorName),
            mType(type),
            mProjectorEffect(NULL),
            mProjectorNode(NULL)
            {
            }

            String mProjectorName;  // 所用特效名称
            ModelProjectorType mType;   // 投影类型
            Effect* mProjectorEffect;   // 特效指针
            Ogre::SceneNode* mProjectorNode;
        };
        typedef std::list<ModelProjector*> ModelProjectors;        

        /// 不要投阴影的mesh名称
        typedef std::list<String> ShadowUncastableMeshs;
        
		/// 保存插入动作的列表
        typedef std::list<Ogre::AnimationState*> PlayingAnimationList;
        PlayingAnimationList mPlayingAnimationList;

        /// 布料专用透明器
        class FabricTransparentor
        {
        protected:
            Ogre::Fabric* mFabric;
            ModelMaterialInstance* mMaterialInstance;

            bool applyTransparency(const Ogre::MaterialPtr& origin, const Ogre::MaterialPtr& derived, Real transparency);

        public:
            FabricTransparentor(Ogre::Fabric* fabric);
            ~FabricTransparentor();

            void clear(void);
            void init(void);
            void apply(Real transparency);
        };

        struct FabricInfo
        {
            FabricInfo(Ogre::Fabric* fabric)
                : mFabric(fabric)
                , mTransparentor(NULL)
            {
            }

            Ogre::Fabric* mFabric;
            FabricTransparentor* mTransparentor;
        };

        typedef std::map<String, FabricInfo> FabricInfoMap;

	public:
		LogicModel( const String& name, System *system );
		~LogicModel();

		/// Model会进行回收，在回收前要先reset
		void reset(void);

		/// obj文件中属性的设置
		bool addAttribute( const String &name, const String &value, const String &hint );
		bool changeAttribute( const String &name, const String &value );

        Variant getAttribute( const String& attributeName );

        const AttribMap& getAttribMap(void)
        {
            return mAttribMap;
        }
        /** 获取当前属性值
        @param value 当前属性值
        @return 
            如果没有要找的属性，就返回false
        */
        bool getAttribute( const String& name, String& value);

        /** 检查是否有指定的属性
        */
        bool hasAttribute( const String& name );

		bool addEntity( const String &name, const String &meshName, const String &matName );

        /** 改变entity
        @param name 参数名称
        @param value mesh名称，如果为""，表示删除原来的entity，不创建新的entity
        */
		bool changeEntity( const String &name, const String &value );

		bool addMaterial( const String &name, const String &entityEntry, const String &matName );
		bool changeMaterial( const String &name, const String &value );

		bool addLocator( const String &name, const String &boneName, 
			const Ogre::Vector3 &pos, const Ogre::Quaternion &orientation, bool transferable = false, bool translateFirst = false);

        /// 判断所传入的locator是否为transferable
        bool isLocatorTransferable(const String& name);

        bool isLocatorExist(const String& locator);

        /// 获取locator信息
        bool getLocatorInfo(const String& locator, LocatorValue& locatorInfo);
        bool setLocatorInfo(const String& locator, const LocatorValue& info);

		bool addSlot( const String &locatorName, const String &slotName, const String &modelName, const String &attrib );
		bool changeSlotAttrib( const String &name, const String &value );
		//LogicModel& operator = (const LogicModel& rhs);

        /** 添加特效信息（从obj文件中获取该信息）
        */
		bool addEffectInfo( const String& name, const String& effectName,
            const String& locatorName, bool translateAll = false, const Ogre::ColourValue& colour = Ogre::ColourValue::White );

        /** 添加不可以透射阴影的mesh名称
        */
        bool addShadowUncastable(const String& meshName);

        /// 判断该mesh是否规定为不可投影
        bool isShadowUncastable(const String& meshName);

		void setUserAny(const Ogre::Any& anything);
		const Ogre::Any& getUserAny(void) const;

		void setUserObject(Ogre::UserDefinedObject* obj);
		Ogre::UserDefinedObject* getUserObject(void) const;

		void setParentSceneNode( Ogre::Node* parentNode );		
		Ogre::SceneNode* getSceneNode(void)
		{
			return mModelMainNode;
		}

		void destroySceneNode(void);

		void destroyEntities(void);

		const Ogre::AxisAlignedBox&     getBoundingBox(void);

		void initModel(void);

		/// 每帧中进行必要的更新，如动作，特效，skill的更新
		void execute(Real delta);

		/// 设置全局的动作速率，1.0表示1倍速，2.0表示2倍速
		void setGlobalAnimationRate(Real rate)
		{
			mGlobalAnimationRate = rate;
		}
		Real getGlobalAnimationRate(void)
		{
			return mGlobalAnimationRate;
		}

		/// 设置这个model上所有entity的visible flag
		void setVisibleFlag(uint32 flags);
        uint32 getVisibleFlag(void)
        {
            return mVisibleFlag;
        }

		void setAnimationLoop(bool loop);
		bool getAnimationLoop(void)
		{
			return mAnimationLoop;
		}

        /** 向LogicModel上挂一个effect
        @param effectName 特效名称
        @param locatorName 挂接点名称
        @param transformType 特效的运动类型，默认为GTIT_POSITION，表示
               只更新model的位置信息
        @param colour 创建特效时所指定的特效颜色值
        */
		EffectHandle addEffect( const String& effectName, const String& locatorName, 
            GetTransformInfoType transformType = GTIT_POSITION, const Ogre::ColourValue& colour = Ogre::ColourValue::White );

        /** 删除effect
        @param handle 特效handle
        @remarks 如果人物身上没挂接所指定的effect，抛异常
        */
        CreatedEffectList::iterator delEffect( EffectHandle handle );

        /** 删除effect
        @param effectIterator 指向要删除的特效的iterator
        */
        CreatedEffectList::iterator delEffect( CreatedEffectList::iterator effectIterator );

        /// 删除所有特效
		void delAllEffect(void);

        /** 获取指定特效
        @param handle 代表特效的handle
        @remarks 如果当前人物身上没带这个effect，就返回NULL
        */
        Effect* getEffect(EffectHandle handle);

		bool getLocatorWorldTransform(const String& name, WX::TransformInfo& info, const Ogre::Vector3& offsetPos = Ogre::Vector3::ZERO,
			const Ogre::Quaternion& offsetRotation = Ogre::Quaternion::IDENTITY, GetTransformInfoType type = GTIT_ALL );
		bool getBoneWorldTransform(const String& name, WX::TransformInfo& info, const Ogre::Vector3& offsetPos = Ogre::Vector3::ZERO,
			const Ogre::Quaternion& offsetRotation = Ogre::Quaternion::IDENTITY, GetTransformInfoType type = GTIT_ALL );

        bool getLocatorWorldTransform(const String& name, Ogre::Matrix4& mtx);

		void createSkill(const String& skillName, bool loop = true, bool anim = false, Ogre::Real delayTime = 0.0f);
		void delCurrentSkill(void);

		Skill* getCurrentSkill(void)
		{	
			return mCurrentSkill;
		}

		void setName(const String& name)
		{
			mName = name;
		}
		const String& getName(void)
		{
			return mName;
		}		

		bool attachModel(const String& locatorName, LogicModel* model);
		bool detachModel( LogicModel* model );
        /// 设置attach自己的LogicModel
        void notifyAttach(LogicModel* parentModel)
        {
            mAttachParent = parentModel;
        }

        /// 判断当前是否挂接在别的model上
        bool isAttached(void)
        {
            return (mAttachParent != NULL);
        }

		/// 获取当前挂接model的父model
        LogicModel* getAttachModel(void)
        {
            return mAttachParent;
        }

		bool isChildModel(void)
		{
			return mChildModel;
		}

		void setChildModel(bool child)
		{
			mChildModel = child;
		}

		void setPosition(Ogre::Vector3& pos);

		Ogre::Vector3& getPosition(void)
		{
			return mModelPosition;
		}

		void setOrientation(Ogre::Quaternion& orientation);

		Ogre::Quaternion& getOrientation(void)
		{
			return mModelOrientation;
		}

        bool isMainNodeNeedUpdate(void) const
        {
            return mMainNodeNeedUpdate;
        }

		// 计算根据地形所旋转的角度
		void calZAxisRotation(float deltaX, float deltaZ, float deltaHeight);
		void calXAxisRotation(float deltaX, float deltaZ, float deltaHeight);

        const String& getCurrentAnimName(void) const
        {
            return mCurrentAnimationName;
        }

        /// 重置位置
        void resetPosition(void);

        /// 设置model是否显示，包括挂接在locator上的物体以及attachModel挂上去的物体
        void setVisible(bool visible);
        
        /** 往model上添加projector
        @remarks 客户端调用，用于挂接projector，包括地下的阴影，选中环等
        @param type projector类型
        @param projectorEffectName 特效名称
        @param show 是否在挂上时就显示
        @param projectorSize 初始的透射范围
        */
        void addProjector(ModelProjectorType type, const String& projectorEffectName,
            bool show = true, float projectorSize = 100.0f);

        /** 删除挂在model上的projector
        @param type projector类型
        @param projectorEffectName 特效名称
        @return 如果删除成功,就return true,不然,就return false
        */
        bool removeProjector(ModelProjectorType type, const String& projectorEffectName);

        /** 设置整个model的透明度
        @remarks 这个方法可以改变model的透明度（包括它身上挂的物体）
        @param time 从当前透明度过渡到transparent参数所定的透明度的过渡时间
        @param transparency 所要的透明度（0表示不透明，1表示全透明，也就是不可见）
        */
        void setTransparent(Real time, Real transparency);

        /// 更新model所带的entity的透明度
        void updateTransparency(Real deltaTime);

        /** 设置透明度信息
        @remarks 设置该model的透明度信息，如果transparentTime为0，就马上设置到entity中，否则，就需要在execute中进行过渡
        @param currTranparency 当前时刻的透明度
        @param destTranparency 要达到的透明度
        @param transparentTime 当前所剩下的透明过渡时间
        */
        void setTransparencyInfo(Real currTransparency, Real destTransparency, Real transparentTime);

        void setHairColour(const Ogre::ColourValue& colour);

        Ogre::ColourValue& getHairColour(void)
        {
            return mHairColour;
        }

        /// 设置是否可以投影
        void setShadowCastable(bool castable);

        /** 设置人物是否选中
        @param selected 如果选中，默认的情况是给物体的自发光加上0.5
        */
        void setSelected(bool selected);

        /// 当前人物是否处于选中状态
        bool isSelected(void)
        {
            return mSelected;
        }

        /** 设置身体颜色（头发除外）
        @param colour 要设置的颜色值
        @param method 颜色设置方式
        */
        void setBodyColour( const Ogre::ColourValue& colour );

        /// 重置身体颜色到原始材质
        void resetBodyColour(void);

        /** 把当前的bounding box和ray进行相交性检测
        @param ray 进行检测的射线
        @return 如果这条ray和当前的bounding box相交，就返回true，否则，返回false
        */
        bool rayIntersect(const Ogre::Ray& ray);

        /// 当前的bounding box是否是外部定义的
        bool isUseExternalBoundingBox(void)
        {
            return NULL != mExternalBoundingBox;
        }

        /** 添加一个布料
        @param fabricName 布料的名称，必须在该model内是唯一的，如果之前已经创建过
                          同名的布料，会把之前的布料删除
        @param headLocatorName 第一个儿挂接点的名称
        @param tailLocatorName 最后一个挂接点的名称
        @remarks
        */
        void addFabric(
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
            const String& tailLocatorName);

        /// 删除指定名称的布料
        void removeFabric(const String& fabricName);

        /// 删除当前所挂接的所有布料
        void removeAllFabrics(void);

        /** 添加一个布料碰撞器
        @param colliderName 碰撞器的名称，model中唯一，在obj文件中定义
        @param locatorName 碰撞器所挂接的locator的名称
        @param colliderType 碰撞器的类型
        @param colliderInfo 创建碰撞器所需的额外信息，如box的长宽高
        */
        bool addFabricCollider( const String& colliderName, const String& locatorName,
                                FabricColliderType colliderType, const String& colliderInfo = "" ); 

        /** 把布料碰撞器从自身的collider set中remove掉
        @param colliderName 碰撞器的名称，如果为“TerrainCollider”，就remove掉地形collider，
        否则，指定的是在obj文件中FabricCollider定义的名称 
        */
        void removeFabricCollider( const String& colliderName );

        void enableAnimationState(bool enable = true);

        void setEnableWalkingEffect(bool enable)
        {
            mEnableWalkingEffect = enable;
        }
        bool getEnableWalkingEffect(void)
        {
            return mEnableWalkingEffect;
        }

        const Ogre::AxisAlignedBox& _getFullBoundingBox(void)
        {
            if (!mFullBoundingBox)
                mFullBoundingBox = new Ogre::AxisAlignedBox;

            return *mFullBoundingBox;
        }

        /// 当前摄像机是否能看见该model
        bool isCameraVisible(void);
        
        /// 获取entitymap中的mesh名称
        Ogre::StringVector getOriginMeshNames(void);

        /** 马上播放一个动作
        @param animName 动作名称
        @param startTime 动作开始播放的时间
        @param weight 该动作的权重
        @remarks 如果该动作不存在，会在log中写入出错信息
        */
        void playAnimation(const String& animName, Real startTime = 0.0f, Real weight = 1.0f);

        /// 获取指定动作的length
        float getAnimationLength(const String& animName) const;

        /// 设置该model的active状态,如果不是处于active状态,就不刷新
        void setActive(bool active = true)
        {
            mIsActive = active;
        }
        bool isActive(void)
        {
            return mIsActive;
        }

        /** 设置当前所播放的动作从指定的时间开始播放
        @param range 参数范围是0到1，0表示播放动作的第一帧，1表示动作的最后一帧
        */
        void setCurrentAnimTime(Real range);

        /// 设置当前模型的缩放系数
        void setScaleFactor(const Ogre::Vector3& scaleFactor);

        /// 更新当前所有的entity的normalize normal状态
        void setEntityNormalizeNormal(bool normal);

		//////////////////////////////////////////////////////////////////////////
		void SetAnimationFinishListener(OnAnimationFinish func, ulong info)
		{
			mOnAnimationFinish = func;
            mCallbackFuncInfo = info;
		}
		OnAnimationFinish GetAnimationFinishListener(void)
		{
			return mOnAnimationFinish;
		}

		void SetSkillHitTimeListener(OnSkillHitTime func, ulong info)
		{
			mOnSkillHitTime = func;
            mCallbackFuncInfo = info;
		}
		OnSkillHitTime GetSkillHitTimeListener(void)
		{
			return mOnSkillHitTime;
		}

		void SetSkillBreakTimeListener(OnSkillBreakTime func, ulong info)
		{
			mOnSkillBreakTime = func;
            mCallbackFuncInfo = info;
		}
		OnSkillBreakTime GetSkillBreakTimeListener(void)
		{
			return mOnSkillBreakTime;
		}

        void SetSkillShakeTimeListener(OnSkillShakeTime func, ulong info)
        {
            mOnSkillShakeTime = func;
            mCallbackFuncInfo = info;
        }
        OnSkillShakeTime GetSkillShakeTimeListener(void)
        {
            return mOnSkillShakeTime;
        }

        void SetGetEffectNameListener(OnGetEffectName func, ulong info)
        {
            mOnGetEffectName = func;
            mCallbackFuncInfo = info;
        }
        OnGetEffectName GetGetEffectNameListener(void)
        {
            return mOnGetEffectName;
        }

        //////////////////////////////////////////////////////////////////////////
		static void SetGetHeightInWorldListener(GetHeightInWorld func)
		{
			mGetHeightInWorld = func;
		}
		static GetHeightInWorld GetGetHeightInWorldListener(void)
		{
			return mGetHeightInWorld;
		}

		static void SetPlaySoundFuncton(OnPlaySound func)
		{
			mOnPlaySound = func;
		}
		static OnPlaySound GetPlaySoundFuncton(void)
		{
			return mOnPlaySound;
		}

		static void SetStopSoundFunction(OnStopSound func)
		{
			mOnStopSound = func;
		}
		static OnStopSound GetStopSoundFunction(void)
		{
			return mOnStopSound;
		}

		/////////////////// 编辑功能 ////////////////////////
		ushort getNumBones(void);
		const String& getBoneName( ushort index );

		Real getCurrentAnimationLength(void);

		ushort getSkeletonAnimationCount(void);

		Ogre::Animation* getSkeletonAnimation( ushort index );

		void getLocatorNames( Ogre::StringVector& names );

		bool getAnimEditing(void)
		{
			return mAnimEditing;
		}
		void setAnimEditing(bool edit = true)
		{	
			mAnimEditing = edit;
		}

		void setAnimTimePos(Real sliderValue)
		{
			mAnimTimePos = sliderValue;
		}
		Real getAnimTimePos(void)	
		{
			return mAnimTimePos;
		}

		Real getCurrentTimePos(void);	

        typedef Ogre::MapIterator<LocatorMap> LocatorMapIterator;
        LocatorMapIterator getLocatorMapIterator(void)
        {
            return LocatorMapIterator(
                mLocatorMap.begin(), mLocatorMap.end());
        }

	protected:

		void _createEntity(void);

		void _setEntityMaterials(void);

		Ogre::Entity* _createEntityImpl( const String& meshName );

		void _destroyEntityImpl(Ogre::Entity* entity);

		void _createMainSceneNode(void);

		void _createLocatorNode(void);

		bool _changeAnimation(const String& animationName, Real delay);

		void _updateLocatorPos(void);

		void _animationBlend(Real delta);

		void _executeLocatorModel(Real delta);

		void _muteCurrentSkillSound(void);

		bool _handleAnimationFinish(Real oldTime, Real currTime);

		void _handleSkillHitTime(Real oldTime, Real currTime);

		void _handleSkillBreakTime(Real oldTime, Real currTime);

        void _handleSkillShakeTime(Real oldTime, Real currTime);

		void _createAnimationEffect(Real oldTime, Real currTime);

		void _updateAttachedAnimationEffect(Real oldTime, Real currTime);

		void _createAnimationRibbon(Real oldTime, Real currTime);

		void _updateAnimationRibbon(Real delta);

		void _updateAnimationSound(Real oldTime, Real currTime);

        void _updateAnimationLight(Real oldTime, Real currTime);

		bool _setEntityMaterial(const String& entityEntry, const String& matName);

		void _createEffects(void);

		void _updateAllEffects(void);

        void _updateModelProjectors(void);

		void _updateBoundingBox(void);

		// 根据需要旋转物体
		void _rotate(void);

		/// 根据字符串来设置不同的移动类型
		void _setMovementType( const String& value );

        /// 删除挂在model上的所有projector effect
        void _delAllProjector(void);

        /// 实例化model时创建slot上的model
        bool _createModelInSlot(SlotValue& slotValue, LocatorValue& locatorValue);

        /// 设置默认动作名称
        void _setDefaultAnimationName(const String& name)
        {
            mDefaultAnimationName = name;
        }

        /// 处理其他属性（没有hint的属性）
        void _dealWithExtraAttributes(const String& name, const String& value);

        /** 获取指定名称的entity
        @param name 要获取的entity的名称（如HairMesh等）
        */
        Ogre::Entity* _getEntityByName(const String& name);

        /** 创建主entity
        @remarks 每个model都有一个主entity，作为skeleton entity，其他entity都从master
                 entity共享骨骼。
                 master entity只有在LogicModel析构时才会被删除，所以可以保存更换其他部分的
                 entity时，共享骨骼的关系是正确的。
        @param skeleton 用于创建skeleton entity的skeleton
        */
        void _createSkeletonEntity(const Ogre::SkeletonPtr& skeleton);

        /// 删除master entity
        void _delSkeletonEntity(void);

        /// 设置一个entity的颜色
        void _setEntityColour( EntityValue& entityValue, const Ogre::ColourValue& entityColour );
        /// 清除一个entity所clone的材质
        void _clearEntityMaterials( EntityValue& entityValue );

        void _resetEntityMaterial( EntityValue& entityValue );

        void _updateAllEntitiesTransparency(void);

        /// 利用外部定义的数据，创建出一个bounding box
        void _createBoundingBoxFromExternal(void);

        /// 根据外部定义的数据，设置缩放因子
        void _setScaleFactor(const String& value);

        /// 根据当前位置，获取所对应的模板特效的名称
        bool _getTemplateEffectName(String& effectName, const String& templateName, float& height);

        /** 根据动作名称，获取AnimationState
        @par 如果当前skeleton entity中没有这个动作，就读取这个动作所在的骨骼文件，
             并link到当前的skeleton entity
        */
		Ogre::AnimationState* _getAnimationState(const String& animName);

        /// 调用获取地形高度的回调函数
        void _getWorldHeight(float x, float z, float& y);

        /// 更新通过attachModel方法attach到自身的model
        void _executeAttachedModel(Real delta);

        /// 删除所创建的collider，包括colliderset
        void _destroyColliders(void);

        /// 在execute中更新collider的位置和方位
        void _updateFabricColliders(void);

        /// 实例化所有的collider
        void _initFabricColliders(void);

        /** 创建一个locator node
        @param offsetPos 初始偏移量
        @param offsetOri 初始偏移旋转量
        */
        Ogre::SceneNode* _createLocatorSceneNode(const Ogre::Vector3& offsetPos, const Ogre::Quaternion& offsetOri);

        /** 获取该locator所对应的scene node指针
        @remarks 如果当前这个locator的scene node还未创建出来，就会在这个函数中调用_createLocatorSceneNode进行创建
        */
        Ogre::SceneNode* _getLocatorSceneNode(LocatorValue& locatorValue);

        /** 更新插入的动作列表
        @remarks 插入的动作会跟主动作一起播放（只播放一次），如果一个动作播放完成了，就把这个动作
        从动作列表中删除
        */
        void _updatePlayingAnimations(Real deltaTime);

        /** 把所传入的entity的材质转换成hardware skinning材质
        @remarks 如果entity中的原材质不符合以下要求，转换会失败，仍使用原材质：
                单个technique，单个pass，单个texture unit
                对于没有骨骼动画的entity，也会直接返回false
        */
        bool _convertEntityToCharacterMaterial(Ogre::Entity* entity);

        Ogre::MaterialPtr _deriveHardwareSkinningMaterial(const Ogre::MaterialPtr& origin,
            const String& prefix, const String& postfix, bool useSpecular);

        /// 判断当前系统是否支持硬件皮肤
        bool _checkForHardwareSkinningMaterialSupport(void);

	protected:

		/// 数据成员
		AttribMap mAttribMap;
		EntityMap mEntityMap;
		MaterialMap mMaterialMap;
		LocatorMap mLocatorMap;
		EffectMap mEffectMap;

        ShadowUncastableMeshs mShadowUncastableMeshs;

        FabricInfoMap mFabricInfoMap;

        FabricColliderMap mFabricColliderMap;

        /// 挂在model上的所有projector effect，有可能是选中标志，地下阴影等
        ModelProjectors mModelProjectors;

		System* mSystem;

		Ogre::Any mUserAny;

		Ogre::SceneNode* mModelMainNode;
        Ogre::Node* mParentNode;

		Ogre::Entity* mSkeletonEntity;

		ulong mCreatedEntityCount;

		Ogre::SkeletonInstance* mSkeleton;

		//完整BoundingBox
		Ogre::AxisAlignedBox* mFullBoundingBox;

		//是否完成BoundingBox
		bool                    mIsPrepareBoundBox;

		Ogre::AnimationState* mCurrentAnimationState;
		Ogre::AnimationState* mFadeOutAnimationState;

		Real mGlobalAnimationRate;

		Real mFadeOutDelay;

		String mCurrentAnimationName;

		bool mAnimationLoop;

        CreatedEffectList mCreatedEffectList;

		uint mEffectHandleCount;

		Skill* mCurrentSkill;

		Real mAnimTimePos;
		bool mAnimEditing;

		String mName;

		bool mChildModel;

		Ogre::Vector3 mModelPosition;
		Ogre::Quaternion mModelOrientation;

		AttachedModelOriginSceneNodes mAttachedModelOriginSceneNodes;

		/// 前后着地点计算出的Orientation
		Ogre::Quaternion mFrontBackOrientation;
		/// 左右着地点计算出的Orientation
		Ogre::Quaternion mLeftRightOrientation;

		bool mMainNodeNeedUpdate;

		/// 该dobject的类型，不同的类型在更新时会有不同的行为
		MovementType mMovementType;

        /// 当前model的透明度
        Real mCurrentTransparency;

        /// 目标透明度
        Real mDestTransparency;

        /// 透明度delta
        Real mDeltaTransparency;

        /// 透明度是否要求改变
        bool mNeedUpdateTransparency;

        /// 透明度过渡时间
        Real mTransparencyTime;

        /// 初始动作名称，初始动作是这个model一创建出来就开始循环播放的
        String mDefaultAnimationName;

        /// 头发的颜色
        Ogre::ColourValue mHairColour;
        
        /// 是否可以投影
        bool mShadowCastable;

        /** attach该LogicModel的LogicModel
        @remarks 当调用attachModel和detachModel时，要把this设成被attach的model的mAttachParent，
                 这样，自己是被哪个LogicModel所attach的
        */
        LogicModel* mAttachParent;

        /// 当前是否被选中
        bool mSelected;

        Ogre::AxisAlignedBox* mExternalBoundingBox;
        String mExternalBoundingBoxValue;

        /// 外部定义的缩放因子
        Ogre::Vector3 mExternalScaleFactor;

        /// 碰撞器集合
        Ogre::ColliderSet* mColliderSet;

        Real mTerrainHeight;

        uint32 mVisibleFlag;

        bool mEnableWalkingEffect;
        
		// 动作相关回调函数
		OnAnimationFinish					mOnAnimationFinish;
		OnSkillBreakTime               mOnSkillBreakTime;
		OnSkillHitTime                 mOnSkillHitTime;
        OnSkillShakeTime               mOnSkillShakeTime;
        OnGetEffectName                mOnGetEffectName;

        ulong mCallbackFuncInfo;

        /// 是否为active，只有active的model才会执行execute
        bool mIsActive;
        /// 是否设为可见
        bool mVisible;

		/// 获取游戏世界的高度值函数
		/** 由于在游戏世界中，某一坐标的地面高度值可能是地形的高度值，也可能
			是行走面的高度值，而行走面的高度值在底层无法获取，所以就通过回调
			函数，由高层根据不同情况给出高度值
 		*/
		static GetHeightInWorld          mGetHeightInWorld;

		// 声音的函数指针
		static OnPlaySound                   mOnPlaySound;
		static OnStopSound                   mOnStopSound;
	};
}




#endif
