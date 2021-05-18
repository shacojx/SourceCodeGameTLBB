/****************************************\
*										*
*			    图形输出系统			*
*										*
\****************************************/
#pragma once

#include "TDNode.h"
#include "TDGfxSystem.h"
#include "TDMath.h"
#include "TDEventSystem.h"
#include "..\WxRender\RenderSystem.h"

struct EVENT;

class CCamera;
class CDebugPanel;
class CProcessBar;
class CHelpPanel;
class CFakeObjectManager;
namespace WX 
{ 
	class System;
	class TerrainGrid;
	class PostFilterManager;
	class EnviromentObject;
};

namespace Ogre
{
	class RaySceneQuery;
};

class CRenderSystem : public tNode
{
public:
	//	坐标转换相关
	enum AXIS_TYPE 
	{
		AX_GAME,		//游戏世界坐标，以米为单位
		AX_PLAN,		//游戏世界坐标，以米为单位,不过不包括Y坐标
		AX_GFX,			//渲染系统中定点坐标，一般以厘米为单位
		AX_SCREEN,		//屏幕上的像素坐标，例如鼠标位置,整数
	};

	//	摄像机相关函数
	enum CAMERA_TYPE
	{
		CHAR_VIEW,
		SCENE
	};
	//----------------------------------------------------------------
	//	系统相关函数
	//----------------------------------------------------------------
public:
	/// 初始化渲染层
	virtual VOID		Initial(VOID*);
	/// 销毁渲染层
	virtual VOID		Release(VOID);
	//逻辑轮循函数
	virtual VOID		Tick(VOID);
	/// 渲染场景
	virtual VOID		RenderFrame(VOID);
	/// 渲染Loading画面
	virtual VOID		RenderLodingFrame(LPCTSTR szLoding);
	/// 响应WM_PAINT消息
	virtual VOID		OnPaint(VOID);
	/// 窗口大小发生改变
	virtual VOID		OnSizeChange(UINT message, WPARAM wParam, LPARAM lParam);
	/// 取得渲染窗口
	virtual HWND		GetWindow(VOID) const;
	//将当前屏幕截图到文件中
	virtual BOOL		PrintScreen(LPTSTR szReturnBuf, INT nBufSize);

	//变量截获函数，用于控制各种渲染效果的开关
	static VOID	WINAPI	_OnVariableChangedEvent(const EVENT* pEvent, UINT dwOwnerData);

	//----------------------------------------------------------------
	//	坐标转换相关
	//----------------------------------------------------------------
public:

	// 将一种坐标转化为另一种坐标,转化成成功返回TRUE
	// 当源坐标非法的时候，返回FALSE
	virtual BOOL		Axis_Trans(	AXIS_TYPE typeSource, const fVector3& fvSource,
									AXIS_TYPE typeTar,			fVector3& fvTarget);

	//检查坐标是否合法
	virtual BOOL		Axis_CheckValid(AXIS_TYPE typeSource, const fVector3& fvAxis);

	//----------------------------------------------------------------
	//	摄像机相关函数
	//----------------------------------------------------------------
public:
	//设置当前摄像机
	virtual VOID		Camera_SetCurrent(CAMERA_TYPE theType);

	/// 移动相机所看的位置到某一点,一般是玩家所在的位置(世界三维坐标系)
	virtual VOID		Camera_SetLookAt(const fVector3& fvPos);
	
	/// 缩放相机视口 [0.0f, 1.0f]  0-最近距离 1-最远距离
	virtual VOID		Camera_SetZoom(FLOAT fZoom);
	virtual VOID		Camera_SetZoom(BOOL bUp,INT multiple);
	virtual FLOAT		Camera_GetZoom(VOID) const;

	/// 设置相机方向 [0, 2л)， 以z轴指向的方向为0度，逆时针为正方向
	virtual VOID		Camera_Direction(FLOAT fDir);
	virtual VOID		Camera_AddDirection(FLOAT fMultiple);
	virtual FLOAT		Camera_GetDirection(VOID) const;

	// 取得屏幕射线
	virtual VOID		Camera_GetWindowToViewportRay(INT nX, INT nY, fRay& fRay);

	// 震荡一次
	virtual VOID		Camera_VibrateOneTime();

	//-----------------------------------------------------------------------
	//
	// 人物选择, 创建, 界面用到的摄像机接口.
	//

	// 设置眼睛的位置
	virtual VOID		Camera_SetEyePos(const fVector3& fvPos);
	// 设置观察点的位置
	virtual VOID		Camera_SetLookAtPos(const fVector3& fvPos);

	//----------------------------------------------------------------
	//调试信息
	//----------------------------------------------------------------
public:
	//--- for debug
	/// 加入滚动Deubg信息
	virtual VOID		PushDebugString(LPCTSTR szMsg, ...);
	/// 设置Debug状态信息
	virtual VOID		SetDebugString(LPCTSTR szName, LPCTSTR szValue);
	/// 设置选中物体信息
	virtual	VOID		SetHitObjText(LPCTSTR szHitObjText);
	//--- for debug

	static const CHAR CRenderSystem::sz32BitTexturesVar[];	//颜色质量，比特数(16/32位)
	static const CHAR CRenderSystem::szDefaultTextureFilteringVar[];		//采样(高/中/低)
	static const CHAR CRenderSystem::szLightmapQualityVar[];	//地形阴影(高/低/无)
	static const CHAR CRenderSystem::szAnimationQualityVar[];	//静态物体动画(开/关)
	static const CHAR CRenderSystem::szPostFilterEnabledVar[];	//全屏泛光效果(开/关)
	static const CHAR CRenderSystem::szFSAAVar[];	//反混角(1/2/4)
	static const CHAR CRenderSystem::szShadowTechniqueVar[];	//人物阴影(开/关)
	static const CHAR CRenderSystem::szvsyncVar[];	//垂直同步(开/关)

	//----------------------------------------------------------------
	//场景相关
	//----------------------------------------------------------------
public:
	//预加载场景
	virtual VOID			Scene_PreLoad(const STRING& theScenePreload);
	//加载场景
	virtual VOID			Scene_Load(INT nID, const STRING& theSceneDefine);
	//显示地形网格
	virtual VOID			Scene_ShowGridSwitch(INT nZoneSize);
	//显示设置全局渲染效果
	virtual VOID			Scene_SetPostFilter(LPCTSTR szFilterName);
	//进入/退出死亡全局渲染效果
	virtual VOID			Scene_SetPostFilter_Death(BOOL bEnter);
	//设置Lightmap
	virtual VOID			Scene_SetLightmapQuality(INT nQuality);
	//设置环境特效(位置)
	virtual VOID			Scene_SetEnviromentInfo(const fVector3& fvPos);
	//设置材质32位/16位转化
	virtual VOID			Scene_Enable32BitTextures(BOOL bSetting=TRUE);
	//设置采样（高是各向异性3、中是三线性2、低是双线性1(远景清晰度)）
	virtual VOID			Scene_SetTextureFiltering(INT nOptions);
	//设置静态物体动画
	virtual VOID			Scene_SetAnimationQuality(INT nOptions);
	//设置全屏泛光效果
	virtual VOID			Scene_SetPostFilterEnabled(BOOL bSetting);
	//设置人物实时阴影
	virtual VOID			Scene_SetShadowTechnique(BOOL bSetting);


	//在某点增加一个粒子
	virtual VOID			Debug_AddParticle(LPCTSTR szParticleName, const fVector3& fvPos);
	//
	virtual VOID			Debug_SetFakeCamera(int nType, float fValue);
	//显示/隐藏DebugPanel
	virtual VOID			Debug_ToggleShowDebugPanel(VOID);

	//----------------------------------------------------------------
	// 创建可渲染物体相关
public:
	/// 创建一个可渲染实体  ETYPE_*
	virtual tEntityNode*	New_EntityObject(tEntityNode::EntityType type);
	//删除一个渲染体
			VOID			Destroy_EntityObject(tEntityNode* pNode);
	///寻找和鼠标接触的实体
	// nX, nY - 鼠标位置
	virtual tEntityNode*	Find_HitWXObject(INT nX, INT nY);

	//----------------------------------------------------------------
	//UI显示模型相关
public:
	//创建一个Fake渲染对象
	virtual VOID			FakeObj_Create(LPCTSTR  szName, tEntityNode* pNode, LPCTSTR szCameraName);
	//销毁一个Fake渲染对象
	virtual VOID			FakeObj_Destroy(LPCTSTR szObjName);
	//隐藏/显示Fake渲染对象
	virtual VOID			FakeObj_Show(LPCTSTR szObjName, BOOL bVisible, float fAspectRatio);

	//-----------------------------------------------------------
	//变量
protected:
	//	引用 渲染系统
	WX::System* m_pRenderSystem;
	//将ogre摄像机作进一步包装,使其更好控制
	CCamera* m_pCamera_Current;
	//人物选择流程摄像机
	CCamera* m_pCamera_CharView;
	//第三人称人物视角摄像机，
	CCamera* m_pCamera_Scene;
	//地形网格
	WX::TerrainGrid* m_pTerrainGrid;
	//鼠标射线和场景交集
	Ogre::RaySceneQuery	*m_pRaySceneQuery;
	//PostFilter系统(全局渲染器)
	WX::PostFilterManager* m_pPostFilterManager;
	//环境特效
	WX::EnviromentObject*  m_pEnviromentObject;

	//死亡特效
	BOOL m_bDeathMode;
	INT	m_nDeachModePercent; //[0,100]
	//调试板
	CDebugPanel			*m_pDebugPanel;

	// FakeManager
	CFakeObjectManager*  m_pFakeObjectManager;
	//-----------------------------------------------------------
	//渲染层内部调用
	//-----------------------------------------------------------
public:
	//得到渲染系统
	WX::System*		GetRenderSystem(VOID) { return m_pRenderSystem; }
	//游戏坐标与渲染坐标之间的放缩比例
	fVector3			GetSacle(VOID) const;

	// 重新设置旧的显示设置， 全屏显示等 2006－4－10；
	// 登录界面之后。 进入游戏中， 调用这个函数。
	// void ResetOldShowSetting();

protected:

public:
	CRenderSystem(VOID);
	virtual ~CRenderSystem(VOID);
	static CRenderSystem*		GetMe(VOID) { return s_pMe; }

protected:
	static CRenderSystem*	s_pMe;

	WX_DECLARE_DYNAMIC(CRenderSystem);
};

