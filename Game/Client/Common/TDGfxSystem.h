/****************************************\
*										*
*			  渲染器接口				*
*										*
\****************************************/
#pragma once
#include "TDNode.h"
#include "TDMath.h"


namespace WX
{
	struct TransformInfo;
	typedef std::vector<TransformInfo> TransformInfos;
}

typedef bool (*FUNC_ONANIMATIONEND)(LPCTSTR, DWORD);
//
// 渲染层物体
//

class tEntityNode
{
public:
	enum EntityType
	{
		ETYPE_DUMMY,	//空物体,只有SceneNode，不会渲染,可以附加特效或者投影纹理
		ETYPE_MESH,		//静态物体(.mesh)
		ETYPE_MODEL,	//动态模型(.model)
		ETYPE_ACTOR,	//角色(DObject)
						//...
	};
public:
	//-------------------------------------------------------
	//得到类型
	virtual EntityType	GetType(VOID) const		= 0;
	//从渲染层销毁节点
	virtual VOID		Destroy(VOID)			= 0;
	
	//设置附加数据
	virtual VOID	SetData(UINT dwData)		= 0;
	virtual UINT	GetData(VOID) const			= 0;

	//-------------------------------------------------------
	//设置位置 (游戏世界坐标)
	virtual VOID	SetPosition(const fVector3& vPos)		= 0;
	//设置旋转角度
	virtual VOID	SetOrientation(const fVector3& vRotate)	= 0;
	//直接设置属性
	virtual VOID	SetWXObjectProperty(const char* szPropertyName, const char* szPropertyValue) = 0;

	virtual VOID	SetLeftWeaponEffect(LPCTSTR szEffectName, UINT color) = 0;
	virtual VOID	SetRightWeaponEffect(LPCTSTR szEffectName, UINT color) = 0;

	virtual VOID	SetRightWeaponMat(LPCTSTR szWeaponMat, LPCTSTR szMatName) = 0;
	virtual VOID	SetLeftWeaponMat(LPCTSTR szWeaponMat, LPCTSTR szMatName) = 0;
	//设置是否可见
	virtual VOID	SetVisible(BOOL bVisible)				= 0;

	//设置是否能被鼠标选中
	virtual VOID	SetRayQuery(BOOL bQuery)				= 0;
	virtual BOOL	GetRayQuery(VOID) const					= 0;


	//设置鼠标选中优先级，数字越大优先级越高
	enum RAYQUERY_LEVEL
	{
		RL_UNKNOWN		= 0,

		RL_ITEMBOX				= 1,		//掉落包		1
		RL_PLAYER_DEADBODY		= 2,		//玩家尸体		2
		RL_CREATURE				= 3,		//NPC&Monster	3
		RL_TRIPPEROBJ			= 4,		//资源			4
		RL_PLAYEROTHER			= 5,		//玩家			5
		RL_PLAYERMYSLEF			= 6,		//自己			6
		RL_PET					= 7,		//宠物			7
		RL_CREATURE_DEADBODY	= 8,		//怪物尸体		8
		RL_PLATFORM				= 9,		//操作平台		9
	};
	virtual VOID			RayQuery_SetLevel(RAYQUERY_LEVEL nLevel)	= 0;
	virtual RAYQUERY_LEVEL	RayQuery_GetLevel(VOID) const				= 0;

	//-------------------------------------------------------
	//附加一个特效
	virtual VOID	Attach_Effect(LPCTSTR szEffectName)		= 0;
	virtual VOID	Detach_Effect(void)						= 0;

	// 新版附加一个特效
	virtual UINT			AddEffect( LPCSTR effectName, LPCSTR locatorName )	= 0;
	virtual VOID			DelEffect( UINT effect )							= 0;
	virtual VOID			DelAllEffect( VOID )								= 0;

	//附加一个投射纹理
	enum PROJTEX_TYPE
	{
		SELECT_RING,		//选择环
		SHADOW_BLOB,		//阴影点
		REACHABLE,			//可以到达的目的点
		UNREACHABLE,		//无法到达的目的点
		AURA_RUNE,			//技能作用范围环
							//...
	};
	virtual VOID	Attach_ProjTexture(PROJTEX_TYPE type, BOOL bShow, FLOAT Ring_Range=100.0f, FLOAT fHeight=1.7f) = 0;
	virtual void	Attach_Object(tEntityNode *pObject, LPCTSTR szAttachLocator)	{	}
	virtual void	Detach_Object(tEntityNode *pObject)	{	}

	//-------------------------------------------------------
	//静态物体类
	//设置MeshFile
	virtual VOID	StaObj_SetMeshFile(LPCTSTR szMeshFile)	= 0;

	//-------------------------------------------------------
	//角色类
	//设置角色模型资源名
	virtual VOID	Actor_SetFile(LPCTSTR szActorFile)	= 0;
	//设置角色模型的属性
	virtual VOID	Actor_SetObjectProperty(LPCTSTR szName, LPCTSTR szValue) = 0;
	//得到角色模型的属性
	virtual VOID	Actor_GetObjectProperty(LPCTSTR szName, STRING& szValue) = 0;
	//取得任务模型上的某关键点位置
	virtual VOID	Actor_GetLocator(LPCTSTR szName, fVector3& fvPosition) = 0;
	//取得"头顶状态点"在屏幕上的位置,如果返回FALSE，表示在屏幕之外,或者没有该点
	virtual BOOL	Actor_GetInfoBoardPos(fVector2& ivPos, const fVector3 *pvObjPos = NULL,const float fObligeHeight = -1) = 0;
	//进入招式
	// bAnim		- TRUE(调用动作)/FALSE(调用招式)
	// szSkillName	- 动作名或者招式名
	// bLoop		- 是否循环
	// fFuseParam	- 骨骼动画融合参数 
	virtual VOID	Actor_EnterSkill(BOOL bAnim, LPCTSTR szSkillName, BOOL bLoop, FLOAT fFuseParam=0.3) = 0;
	virtual VOID	Actor_SetHairColor(UINT HairColor) = 0;
	// 切换动画的播放速度
	// fRate		- 缩放比率
	virtual VOID	Actor_ChangeActionRate(FLOAT fRate) = 0;
	//设置缺省动作
	virtual VOID	Actor_SetDefaultAnim(LPCTSTR szAnimName) = 0;
	//设置UI VisibleFlag
	virtual VOID	Actor_SetUIVisibleFlag(VOID) = 0;
	//设置透明度
	virtual VOID	Actor_SetTransparent(FLOAT fTransparency, FLOAT fTime) = 0;
	// 设置连线特效的目标点
	virtual VOID	Actor_SetEffectExtraTransformInfos(UINT uEffect, fVector3& fvPosition) = 0;
	//设置是否出于鼠标Hover状态
	virtual VOID	Actor_SetMouseHover(BOOL bHover) = 0;

	//设置角色动画结束通知
	virtual void	SetAnimationEndEvent(FUNC_ONANIMATIONEND pFunc, DWORD dwParam)	{	}
	//设置角色动画可以结束通知
	virtual void	SetAnimationCanBreakEvent(FUNC_ONANIMATIONEND pFunc, DWORD dwParam)	{	}
	//设置角色动画打击点通知
	virtual void	SetAnimationHitEvent(FUNC_ONANIMATIONEND pFunc, DWORD dwParam)	{	}
	//设置角色动画打击点通知
	virtual void	SetAnimationShakeEvent(FUNC_ONANIMATIONEND pFunc, DWORD dwParam)	{	}
};


//
//图形输出系统
//
//class tGfxSystem : public tNode
//{
//public:
//	//----------------------------------------------------------------
//	//	系统相关函数
//
//	/// 初始化渲染层
//	virtual VOID		Initial(VOID*)		= 0;
//	/// 销毁渲染层
//	virtual VOID		Release(VOID)		= 0;
//	/// 渲染场景
//	virtual VOID		RenderFrame(VOID)	= 0;
//	/// 渲染Loading画面
//	virtual VOID		RenderLodingFrame(LPCTSTR szLoding) = 0;
//	/// 响应WM_PAINT消息
//	virtual VOID		OnPaint(VOID)		= 0;
//	/// 窗口大小发生改变
//	virtual VOID		OnSizeChange(UINT message, WPARAM wParam, LPARAM lParam)	= 0;
//	/// 取得渲染窗口
//	virtual HWND		GetWindow(VOID)	const				= 0;
//	//将当前屏幕截图到文件中
//	virtual BOOL		PrintScreen(LPTSTR szReturnBuf, INT nBufSize) = 0;
//
//	//--- for debug
//	/// 加入Deubg信息
//	virtual VOID		PushDebugString(LPCTSTR szMsg, ...)	= 0;
//	/// 设置Debug状态信息
//	virtual VOID		SetDebugString(LPCTSTR szName, LPCTSTR szValue) = 0;
//	/// 设置选中物体信息
//	virtual	VOID		SetHitObjText(LPCTSTR szHitObjText)	= 0;
//	//--- for debug
//
//public:
//	//----------------------------------------------------------------
//	//	坐标转换相关
//	enum AXIS_TYPE 
//	{
//		AX_GAME,		//游戏世界坐标，以米为单位
//		AX_PLAN,		//游戏世界坐标，以米为单位,不过不包括Y坐标
//		AX_GFX,			//渲染系统中定点坐标，一般以厘米为单位
//		AX_SCREEN,		//屏幕上的像素坐标，例如鼠标位置,整数
//	};
//
//	// 将一种坐标转化为另一种坐标,转化成成功返回TRUE
//	// 当源坐标非法的时候，返回FALSE
//	virtual BOOL		Axis_Trans(	AXIS_TYPE typeSource, const fVector3& fvSource,
//									AXIS_TYPE typeTar,			fVector3& fvTarget) = 0;
//
//public:
//	//----------------------------------------------------------------
//	//	摄像机相关函数
//	enum CAMERA_TYPE
//	{
//		CHAR_VIEW,
//		SCENE
//	};
//	//设置当前摄像机
//	virtual VOID		Camera_SetCurrent(CAMERA_TYPE theType)	= 0;
//
//	/// 移动相机所看的位置到某一点,一般是玩家所在的位置(世界二维坐标系)
//	virtual VOID		Camera_SetLookAt(const fVector3& fvPos) = 0;
//	
//	/// 缩放相机视口 [0.0f, 1.0f]  0-最近距离 1-最远距离
//	virtual VOID		Camera_SetZoom(FLOAT fZoom)				= 0;
//	virtual VOID		Camera_SetZoom(BOOL bUp,INT multiple)	= 0;
//	virtual FLOAT		Camera_GetZoom(VOID) const				= 0;
//
//	/// 设置相机方向 [0, 2л)， 以z轴指向的方向为0度，逆时针为正方向
//	virtual VOID		Camera_Direction(FLOAT fDir)			= 0;
//	virtual VOID		Camera_AddDirection(FLOAT fMultiple)	= 0;
//	virtual FLOAT		Camera_GetDirection(VOID) const			= 0;
//
//	// 取得屏幕射线
//	virtual VOID		Camera_GetWindowToViewportRay(INT nX, INT nY, fRay& fRay) = 0;
//
//	// 震荡一次
//	virtual VOID		Camera_VibrateOneTime() = 0;
//
//	//-----------------------------------------------------------------------
//	//
//	// 人物选择, 创建, 界面用到的摄像机接口.
//	//
//
//	// 设置眼睛的位置
//	virtual VOID		Camera_SetEyePos(const fVector3& fvPos) = 0;
//	// 设置观察点的位置
//	virtual VOID		Camera_SetLookAtPos(const fVector3& fvPos) = 0;
//
//
//public:
//	//----------------------------------------------------------------
//	//预加载场景
//	virtual VOID			Scene_PreLoad(const STRING& theScenePreload) = 0;
//	//加载场景
//	virtual VOID			Scene_Load(INT nID, const STRING& theSceneDefine)	= 0;
//	//显示地形网格
//	virtual VOID			Scene_ShowGridSwitch(INT nZoneSize)			= 0;
//	//显示设置全局渲染效果
//	virtual VOID			Scene_SetPostFilter(LPCTSTR szFilterName)	= 0;
//	//设置LightMap
//	virtual VOID			Scene_SetLightmapQuality(INT nQuality)		= 0;
//	//进入/退出死亡全局渲染效果
//	virtual VOID			Scene_SetPostFilter_Death(BOOL bEnter)		= 0;
//	//设置环境特效(位置)
//	virtual VOID			Scene_SetEnviromentInfo(const fVector3& fvPos)	= 0;
//	//设置材质32位/16位转化
//	virtual VOID			Scene_Enable32BitTextures(BOOL bSetting=TRUE)	= 0;
//	//设置采样（高是各向异性3、中是三线性2、低是双线性1(远景清晰度)）
//	virtual VOID			Scene_SetTextureFiltering(INT nOptions)			= 0;
//	//设置静态物体动画
//	virtual VOID			Scene_SetAnimationQuality(INT nOptions)			= 0;
//	//设置全屏泛光效果
//	virtual VOID			Scene_SetPostFilterEnabled(BOOL bSetting)		= 0;
//	//设置人物实时阴影
//	virtual VOID			Scene_SetShadowTechnique(BOOL bSetting)			= 0;
//
//	//--- for debug
//	//在某点增加一个粒子
//	virtual VOID			Debug_AddParticle(LPCTSTR szParticleName, const fVector3& fvPos) = 0;
//	//
//	virtual VOID			Debug_SetFakeCamera(int nType, float fValue) = 0;
//	//显示/隐藏DebugPanel
//	virtual VOID			Debug_ToggleShowDebugPanel(VOID) = 0;
//	//--- for debug
//
//	//----------------------------------------------------------------
//	//	创建可渲染物体相关
//public:
//	/// 创建一个可渲染实体  ETYPE_*
//	virtual tEntityNode*	New_EntityObject(tEntityNode::EntityType type)	= 0;
//	///寻找和鼠标接触的实体
//	virtual tEntityNode*	Find_HitWXObject(INT nX, INT nY)				= 0;
//
//	//----------------------------------------------------------------
//	//UI显示模型相关
//public:
//	//创建一个Fake渲染对象
//	virtual VOID			FakeObj_Create(LPCTSTR  szName, tEntityNode* pNode, LPCTSTR szCameraName) = 0;
//	//销毁一个Fake渲染对象
//	virtual VOID			FakeObj_Destroy(LPCTSTR szObjName) = 0;
//	//隐藏/显示Fake渲染对象
//	virtual VOID			FakeObj_Show(LPCTSTR szObjName, BOOL bVisible, float fAspectRatio) = 0;
//
//	WX_DECLARE_DYNAMIC(tGfxSystem);
//};