#ifndef _SKELETONMESH_H_
#define _SKELETONMESH_H_

#include "ModelTypes.h"

//ogre 头文件引用
#include <OgrePrerequisites.h>
#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <OgreAxisAlignedBox.h>
#include <OgreAny.h>

// fairy effect header
#include "EffectSystem/WXEffect.h"
#include "EffectSystem/WXEffectManager.h"

namespace WX	{
	class System;
};

// 属性影响标识
//Attribute hint define
#define AH_MESH							(0x01)
#define AH_ANIMATION					(0x02)
#define AH_LOCATOR						(0x04)
#define AH_ALL							(AH_MESH|AH_ANIMATION|AH_LOCATOR)

//类型预定义


typedef			Ogre::String				DString;
typedef			std::vector<DString>		DStringList;
typedef			unsigned long				dword;

typedef unsigned int EffectHandle;

struct FairyEffectInfo
{
	FairyEffectInfo() :	mEffect(NULL), mLocator("")
	{
	}

	WX::Effect *mEffect;
	Ogre::String mLocator;
};

typedef std::map<EffectHandle, FairyEffectInfo> FairyEffectMap;

//工具函数声明开始
void				  MeshLog(bool bShowDialog, const char* format, ...);
typedef bool          (*_OnAnimationEnd)(const char* pAnimationName, unsigned long info);

typedef bool          (*_OnSkillHitTime)(const char* pAnimationName, unsigned long info);
typedef bool          (*_OnSkillBreakTime)(const char* pAnimationName, unsigned long info);
typedef bool          (*_OnShakeTime)(const char* pAnimationName, unsigned long info);

// 声音函数指针
typedef int (__stdcall* OnPlaySound)(const char* szSoundFile, float* fvPos, bool isLoop);
typedef void (__stdcall* OnStopSound)(int handle);

enum ObjEntityIndex
{
	Face,
	Hair,
	Cap,
	MainBody,
	Foot,
	Arm,

	EntityCount,
};

enum GetTransformInfoType
{
	GTIT_POSITION,
	GTIT_ORIENTATION,
	GTIT_ALL,
};

// object类型，主要决定了object的行走方式
enum ObjectMovementType
{
	OT_NORMAL, // 普通类型，不会随地形修正object的方位
	OT_ROTATE_X_AXIS,	// 只能前后摇摆
	OT_ROTATE_X_Z_AXIS,	// 可以前后左右摇摆
};

class DObject_NT;

// 挂接信息
struct AttachInfo
{
	AttachInfo(DString &point, DObject_NT *object) :
	mAttachPoint(point), mAttachObject(object)
	{
	}

	AttachInfo() :
	mAttachPoint(""), mAttachObject(NULL)
	{
	}

	// 所挂接的点的名称
	DString mAttachPoint;
	// 所挂接的物体
	DObject_NT *mAttachObject;
};

typedef std::list<AttachInfo> AttachInfos;

// 被挂接上的物体原来的scene node
typedef std::map<DObject_NT *, Ogre::SceneNode *> AttachedObjectOriginSceneNodes;

struct ObjEntity
{
	ObjEntity() : matName(""), pEntity(NULL)	{	}
	DString				matName;
	Ogre::Entity*		pEntity;
};

class SkeletonMeshUtil;

class MODEL_API	DObject_NT
{
protected:	

	FairyEffectMap mFairyEffectMap;

	friend	class SkeletonMeshUtil;

	WX::Skill *mCurrentSkill;

public:
	//构造函数
	DObject_NT( WX::System *system );

	
	virtual ~DObject_NT();
	
	void							Init();

	void							ReSet();

	// 设置场景物体的可见标志
	void							setVisibleFlag(unsigned int nVisibleFlag);

    void                            setUserAny(const Ogre::Any& anything);
    const Ogre::Any&                getUserAny(void) const;
    void                            setUserObject(Ogre::UserDefinedObject* obj);
    Ogre::UserDefinedObject*        getUserObject(void) const;

	//属性处理方法
	void							SetGroupName(DString StrGroupName) {	m_GroupName = StrGroupName;	}

	void							SetObjFileName(DString StrObjFileName);
	DString							GetObjFileName(){return m_ObjFile;};

	void							SetObjName(DString StrObjName){m_ObjFile = StrObjName;};
	DString							GetObjName(){return m_ObjName;};

	//场景节点方法
	void							SetObjectNode(Ogre::SceneNode* pNode){ m_pObjNode = pNode;};
	Ogre::SceneNode*							GetObjectNode(){return m_pObjNode;};


	bool							isChildObject(){return m_isChildObject;};
	void							SetIsChildObject(bool isChild){m_isChildObject = isChild;};

	void							SetAttachedVisibleEntity(Ogre::Entity* pEntity){m_pAttachedVisibleEntity = pEntity;};
	Ogre::Entity*							GetAttachedVisibleEntity(){return m_pAttachedVisibleEntity;};


	void							SetGlobalAnimationRate(float fRate);

	void                            CreateSkill(const DString &skillName, bool loop = true,
										bool anim = false, Ogre::Real delayTime = 0.0f);

	void                            DelCurrentSkill(void);

	WX::Skill *					GetCurrentSkill(void)
	{	
		return mCurrentSkill;
	}

	float                           GetCurrentAnimationLength(void);

    virtual bool					Execute(Ogre::Real delta);

    bool							ChangeAction(const DString& actioname, Ogre::Real delay = 0);

	bool							ChangeAttributes(const DStringList& attribnames, const DStringList& attribvalues);

	bool							SetParentNode(Ogre::SceneNode* parentnode);

	EffectHandle					AddEffect(const Ogre::String &effectName, const Ogre::String &locatorName);
	std::pair<bool, FairyEffectMap::iterator>		DelEffect( EffectHandle handle );
	void                            DelAllEffect(void);

	bool							SetAnimationEndEventListener(_OnAnimationEnd func, unsigned long info);
	bool                            SetSkillHitTimeListener(_OnSkillHitTime func, unsigned long info);
	bool                            SetSkillBreakTimeListener(_OnSkillBreakTime func, unsigned long info);
	bool                            SetShakeTimeListener(_OnShakeTime func, unsigned long info);
	//public edit functions
	bool							UpdateObjAttribute(dword hints);//update object attribute and clear the attribute's dirty state
	long 							GetObjAttributeCount();
	bool 							GetObjAttribute(const DString& attribname, DString& attribvalue, dword& hints);
	bool 							GetObjAttribute(long idx, DString& attribname, DString& attribvalue, dword& hints);
	bool 							AddObjAttribute(const DString& name, const DString& value, const DString& hints);
	bool 							SetObjAttribute(const DString& name, const DString& value, const DString& hints);
	bool 							DelObjAttribute(const DString& name);
	DString							Hint2String(dword hints);

	long 							GetObjEntityCount();
	bool 							GetObjEntity(long idx, DString& name, DString& objfile, DString& material, DString& condition);
	bool 							SetObjEntity(long idx, const DString& material, const DString& condition);
	bool 							SetObjEntity(const DString& name, const DString& material, const DString& condition);

	bool 							GetObjAllAnimName(DStringList& anl);
	DStringList&					GetObjAllAnimName();

	long 							GetObjLocatorCount();
	bool 							GetObjLocatorName(long idx, DString& locname);
	long 							GetObjLocatorSlotCount(long idx);
	bool 							GetObjLocatorSlot(long idx, long slotidx, DString& slotname, DString& objfile, DStringList& attribs);
	DString&						GetObjLocatorSlotName(long idx,long slotidx);
	DString&						GetObjLocatorSlotObjectFile(long idx,long slotidx);
	DStringList&					GetObjLocatorSlotAttribs(long idx,long slotidx);

	bool                            IsLocatorExist(const DString &locName);

	bool 							AddObjLocatorSlot(long idx, const DString& slotname, const DString& objfile, const DStringList& attribs);
	bool 							SetObjLocatorSlot(long idx, const DString& slotname, const DString& objfile, const DStringList& attribs);
	bool 							DelObjLocatorSlot(long idx, const DString& slotname);

	bool 							GetObjLocatorWorldTransform(const DString& name, Ogre::Matrix4& mtx);

	bool 							GetLocatorWorldTransform(const DString& name, Ogre::Matrix4& mtx);

	static void						SetSceneMgr(Ogre::SceneManager* pSceneMgr);
	static Ogre::SceneManager*		GetSceneMgr();
	
	Ogre::SceneNode*				getSceneNode(void) const {	return m_pObjNode;}

    const Ogre::AxisAlignedBox&     getBoundingBox(void) const;

	/// 设置默认动作（不循环动作做完后进行的动作，该动作为循环播放）
	void SetDefaultAnimation(const DString& animName)
	{
		m_DefaultAnimName = animName;
	}

	void SetAnimationLoop(bool loop);
	
	bool GetAnimationLoop(void);

	static void SetPlaySoundFuncton(OnPlaySound func)
	{
		m_fOnPlaySound = func;
	}

	static void SetStopSoundFunction(OnStopSound func)
	{
		m_fOnStopSound = func;
	}

	static OnPlaySound GetPlaySoundFuncton(void)
	{
		return m_fOnPlaySound;
	}

	static OnStopSound GetStopSoundFunction(void)
	{
		return m_fOnStopSound;
	}

	const DString & getCurrentActionName(void)
	{
		return m_CurrActionName;
	}

	// 在指定的位置挂接其他DObject，注意，这个物体在被挂接之前必须要先创建出来
	void AttachObjects( AttachInfos &infos );
	// 把指定的DObject从当前DObject上接挂，并把其挂到之前的父节点上
	void DetachObjects( AttachInfos &infos );

	// 计算根据地形所旋转的角度
	void CalZAxisRotation(float deltaX, float deltaZ, float deltaHeight);
	void CalXAxisRotation(float deltaX, float deltaZ, float deltaHeight);

	ObjectMovementType GetObjectMovementType(void)
	{
		return mObjectMovementType;
	}

	bool							_GetLocatorWorldTransform(const DString& name, WX::TransformInfo &info, const Ogre::Vector3 &offsetPos = Ogre::Vector3::ZERO,
		const Ogre::Quaternion &offsetRotation = Ogre::Quaternion::IDENTITY, GetTransformInfoType type = GTIT_ALL );
	bool 							_GetBoneWorldTransform(const DString& name, WX::TransformInfo &info, const Ogre::Vector3 &offsetPos = Ogre::Vector3::ZERO,
		const Ogre::Quaternion &offsetRotation = Ogre::Quaternion::IDENTITY, GetTransformInfoType type = GTIT_ALL );


protected://type define
	//Attributes
	
	
	struct	_AttribValue
	{
		DString				Value;
		dword				Hints;
		bool				IsChanged;
	};
	typedef		std::map<DString, _AttribValue>		_AttribMap;


	//EntityList
	struct	_EntityInfo
	{
		DString				Name;
		DString				File;
		DString				Material;
		DString				Condition;
		Ogre::Entity*		pEntity;
	};
	typedef		std::vector<_EntityInfo>		_EntityList;
	
	//Locators
	struct	_Slot
	{
		DString				Name;
		DString				ObjFile;
		DStringList			InitAttribs;
		DObject_NT*			pObject;
	};
	typedef std::vector<_Slot> _SlotList;
	
	
	struct	_Locator
	{
        _Locator()
            : OffRot(Ogre::Quaternion::IDENTITY)
        {
        }

		DString				Name;
		DString				BoneName;
		Ogre::Vector3		OffPos;
		Ogre::Quaternion	OffRot;
		Ogre::SceneNode*	pLocatorNode;
		_SlotList			SlotList;
	};
	typedef		std::vector<_Locator>	_LocatorList;	

protected://attributes
	static Ogre::SceneManager*		s_pSceneMgr;

    Ogre::Any                       mUserAny;

	DString							m_GroupName;
	DString							m_ObjFile;
	DString							m_ObjName;

	Ogre::SceneNode*				m_pObjNode;
	Ogre::Entity*					m_pObjFirstSkeletonEntity;

	Ogre::SkeletonInstance*			m_pObjSkeleton;
	Ogre::SkeletonInstance*			m_pObjParentSkeleton;

	Ogre::AnimationState*			m_pAnimationState;
    Ogre::AnimationState*			m_fadeOutAnimationState;
    Ogre::Real		                m_fadeOutDelay;
	
	DString							m_CurrActionName;

	_AttribMap						m_AttribMap;
	_EntityList						m_EntityList;
	_LocatorList					m_LocatorList;

	//动作结束事件回调函数
	_OnAnimationEnd					m_fOnAnimationEnd;

	_OnSkillBreakTime               m_fOnSkillBreakTime;
	_OnSkillHitTime                 m_fOnSkillHitTime;
	_OnShakeTime                    m_fOnShakeTime;

	unsigned long					m_ReserveInfo;

	//是否子Object
	bool							m_isChildObject;

	Ogre::Entity*					m_pAttachedVisibleEntity;

    //完整BoundingBox
    mutable Ogre::AxisAlignedBox    m_FullBoundingBox;
    
    //是否完成BoundingBox
    mutable bool                    m_isPrepareBoundBox;

	//全局的Animation play rate
	float							m_fGlobalAnimationRate;

	/// 默认动作的名称
	DString                       m_DefaultAnimName;

	bool                           m_bAnimationLoop;

	// 声音的函数指针
	static OnPlaySound                   m_fOnPlaySound;
	static OnStopSound                   m_fOnStopSound;

	ObjEntity mObjEntities[EntityCount];

	unsigned int mEntityNameCount;
	unsigned int mEffectHandleCount;

	// 该dobject的类型，不同的类型在更新时会有不同的行为
	ObjectMovementType mObjectMovementType;

	AttachedObjectOriginSceneNodes mAttachedObjectOriginSceneNodes;
	
	// 保存系统指针，用于取得TerrainData的指针
	WX::System *mSystem;	

	// 前后着地点计算出的Orientation
	Ogre::Quaternion mFrontBackOrientation;
	// 左右着地点计算出的Orientation
	Ogre::Quaternion mLeftRightOrientation;

protected://functions
	bool 							_EvalCondition(const DString& condition);
	bool 							_EvalAction(const DString& action);
	
	//属性相关的操作函数
	bool 							_AddAttrib(const DString& name, const DString& value, const DString& hints);
	bool 							_SetAttrib(const DString& name, const DString& value, const DString& hints);
	bool 							_GetAttrib(const DString& name, DString& value, dword& hints, bool& ischanged);
	bool 							_DelAttrib(const DString& name);
	bool 							_SetAttribValue(const DString& name, const DString& value, dword* hints = NULL);
	long 							_GetAttribCount();
	bool 							_GetAttrib(long idx, DString& name, DString& value, dword &hints, bool& ischanged);
	bool 							_ClearAttribState();
	static DString					_Hint2Str(dword hints);
	static dword					_Str2Hint(const DString& hints);

	//实体相关的操作函数
	bool 							_AddEntity(const DString& name, const DString& entfile, const DString& mat, const DString& condition);
	bool 							_LoadEntity();
	Ogre::Entity*					_FindVisibleEntity();
	bool							_LoadChildEntity();

	bool 							_SetEntity(const DString& name, const DString& mat, const DString& condition);
	bool 							_UpdateEntitys();
	void							_ReAttachChildObjects();
		

	bool 							_DestroyEntitys();		////只在删除对象的时候调用
	long 							_GetEntityCount();
	bool 							_GetEntity(long idx, DString& name, DString& entfile, DString& mat, DString& condition);
	bool 							_SetEntity(long idx, const DString& mat, const DString& condition);

	void							_SetEntityMat(Ogre::Entity* pEntity,const DString& mat);

	//绑定点信息相关的操作函数
	bool 							_AddLocator(const DString& name, const DString& bonename, const Ogre::Vector3& offpos, const Ogre::Quaternion& offrot);
	bool							_AddChildLocator(const DString& name, const DString& bonename, const Ogre::Vector3& offpos, const Ogre::Quaternion& offrot,Ogre::SkeletonInstance* pParentSkeleton,Ogre::Bone* ParentBone);
	bool 							_GetLocatorWorldTransform(const DString& name, Ogre::Matrix4& mtx);
	bool 							_AddSlot(const DString& locatorname, const DString& slotname, const DString& slotobj, const DStringList& initattribs);
	bool 							_SetSlot(const DString& locatorname, const DString& slotname, const DString& slotobj, const DStringList& initattribs);
	bool 							_DelSlot(const DString& locatorname, const DString& slotname);
	bool 							_UpdateLocators();		//当对象属性改变时更新locator list

	bool							_UpdateLocatorPos();

	bool							_AttachEntitysToBone(Ogre::Bone* pBone);
	
	bool 							_DestroyLocators();	//只在删除对象的时候调用
	long 							_GetLocatorCount();
	bool 							_GetLocatorName(long idx, DString& locname);
	long 							_GetSlotCount(long idx);
	bool 							_GetSlot(long idx, long slotidx, DString& slotname, DString& objfile, DStringList& attribs);
	bool 							_AddSlot(long idx, const DString& slotname, const DString& slotobj, const DStringList& initattribs);
	bool 							_SetSlot(long idx, const DString& slotname, const DString& slotobj, const DStringList& initattribs);
	bool 							_DelSlot(long idx, const DString& slotname);

	Ogre::Entity * _LoadEntity( const DString& meshName );
	void _SetEntity( const DString& name, const DString& value );

	void _DeleteEntities(void);

	void _MuteCurrentSkillSound(void);

	// 设置该Object的行走类型
	void _SetObjectMovementType( const DString& typeStr );

	// 根据需要旋转物体
	void _Rotate(void);

	// 更新挂接上去的fairy effect
	void _updateFairyEffects(void);
};

#endif