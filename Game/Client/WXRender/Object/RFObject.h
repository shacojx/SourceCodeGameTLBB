#pragma once
#include "..\WxRender\RenderSystem.h"

namespace Ogre
{
	class SceneNode;
}
class CWXObj_ProjTex;

class CWXObject : public tEntityNode
{
public:
	//从渲染层销毁节点
	virtual VOID	Destroy(VOID);

	//设置附加数据
	virtual VOID	SetData(UINT nData)		{ m_nData = nData; }
	virtual UINT	GetData(VOID) const		{ return m_nData; }

	//设置位置 (游戏世界坐标)
	virtual VOID	SetPosition(const fVector3& vPos) {}
	//设置旋转角度
	virtual VOID	SetOrientation(const fVector3& vRotate) {}
	//直接设置属性
	virtual VOID	SetWXObjectProperty(const char* szPropertyName, const char* szPropertyValue){}
	//设置是否可见
	virtual VOID	SetVisible(BOOL bVisible){}

	//设置是否能被鼠标选中
	virtual VOID	SetRayQuery(BOOL bQuery) { }
	virtual BOOL	GetRayQuery(VOID) const { return FALSE; }
	//鼠标选中优先级
	virtual VOID			RayQuery_SetLevel(RAYQUERY_LEVEL nLevel) { m_nRayQueryLevel = nLevel; }
	virtual RAYQUERY_LEVEL	RayQuery_GetLevel(VOID) const { return m_nRayQueryLevel; }

	//附加一个特效
	virtual VOID	Attach_Effect(LPCTSTR szEffectName) {}
	virtual VOID	Detach_Effect(void) {}

	virtual UINT	AddEffect( LPCSTR effectName, LPCSTR locatorName ){ return NULL; }
	virtual VOID	DelEffect( UINT effect ){}
	virtual VOID	DelAllEffect( VOID ){}

	virtual VOID	SetLeftWeaponEffect(LPCTSTR szEffectName, UINT color) {}
	virtual VOID	SetRightWeaponEffect(LPCTSTR szEffectName, UINT color) {}

	virtual VOID	SetRightWeaponMat(LPCTSTR szWeaponMat, LPCTSTR szMatName) {};
	virtual VOID	SetLeftWeaponMat(LPCTSTR szWeaponMat, LPCTSTR szMatName) {};
	//附加一个投射纹理
	virtual VOID	Attach_ProjTexture(PROJTEX_TYPE type, BOOL bShow, FLOAT Ring_Range=100.0f, FLOAT fHeight=1.7f) {}

	//设置MeshFile
	virtual VOID	StaObj_SetMeshFile(LPCTSTR szMeshFile)	{}
	//设置角色模型资源名
	virtual VOID	Actor_SetFile(LPCTSTR szActorFile)	{}
	//设置角色模型的属性
	virtual VOID	Actor_SetObjectProperty(LPCTSTR szName, LPCTSTR szValue) {}
	//得到角色模型的属性
	virtual VOID	Actor_GetObjectProperty(LPCTSTR szName, STRING& szValue) {}
	//取得任务模型上的某关键点位置
	virtual VOID	Actor_GetLocator(LPCTSTR szName, fVector3& fvPosition) {}
	//取得"头顶状态点"在屏幕上的位置,如果返回FALSE，表示在屏幕之外,或者没有该点
	virtual BOOL	Actor_GetInfoBoardPos(fVector2& ivPos, const fVector3 *pvObjPos = NULL,const float fObligeHeight = -1) {return FALSE;}
	//进入招式
	// bAnim		- TRUE(调用动作)/FALSE(调用招式)
	// szSkillName	- 动作名或者招式名
	// bLoop		- 是否循环
	// fFuseParam	- 骨骼动画融合参数 
	virtual VOID	Actor_EnterSkill(BOOL bAnim, LPCTSTR szSkillName, BOOL bLoop, FLOAT fFuseParam) {}
	virtual VOID	Actor_SetHairColor(UINT HairColor) {}
	// 切换动画的播放速度
	// fRate		- 缩放比率
	virtual VOID	Actor_ChangeActionRate(FLOAT fRate){}
	//设置缺省动作
	virtual VOID	Actor_SetDefaultAnim(LPCTSTR szAnimName) {}
	//设置VisibleFlag
	virtual VOID	Actor_SetUIVisibleFlag(VOID) {}
	//设置透明度
	virtual VOID	Actor_SetTransparent(FLOAT fTransparency, FLOAT fTime){}
	// 设置连线特效的目标点
	virtual VOID	Actor_SetEffectExtraTransformInfos(UINT uEffect, fVector3& fvPosition){}
	//设置是否出于鼠标Hover状态
	virtual VOID	Actor_SetMouseHover(BOOL bHover) {} 

public:
	CWXObject();
	virtual ~CWXObject();

protected:
	UINT				m_nData;
	//鼠标选中优先级,数越大越优先选中
	RAYQUERY_LEVEL		m_nRayQueryLevel;
};