/****************************************\
*										*
*	   CRFObj_Actor(DObject)		*
*										*
\****************************************/

#pragma once
#include "RFObj.h"

namespace WX
{
    class LogicModel;
};

class DObject_NT;
class CWXObj_ProjTex;
class CRFObj_Actor : public CWXObj_Object
{
public:
	//得到类型
	virtual EntityType	GetType(VOID) const	{ return ETYPE_ACTOR; }

	//从渲染层销毁节点
	virtual VOID		Destroy(VOID);
	//设置是否可见
	virtual VOID		SetVisible(BOOL bVisible);
	//附加一个投射纹理
	virtual VOID		Attach_ProjTexture(PROJTEX_TYPE type, BOOL bShow, FLOAT Ring_Range=100.0f, FLOAT fHeight=1.7f );
	virtual void		Attach_Object(tEntityNode *pObject, LPCTSTR szAttachLocator);
	virtual void		Detach_Object(tEntityNode *pObject);

	//设置角色模型资源名
	virtual VOID	Actor_SetFile(LPCTSTR szActorFile);
	//设置角色模型的属性
	virtual VOID	Actor_SetObjectProperty(LPCTSTR szName, LPCTSTR szValue);
	//得到角色模型的属性
	virtual VOID	Actor_GetObjectProperty(LPCTSTR szName, STRING& szValue);
	//取得任务模型上的某关键点位置
	virtual VOID	Actor_GetLocator(LPCTSTR szName, fVector3& fvPosition);
	//取得"头顶状态点"在屏幕上的位置,如果返回FALSE，表示在屏幕之外,或者没有该点
	virtual BOOL	Actor_GetInfoBoardPos(fVector2& ivPos, const fVector3 *pvObjPos = NULL,const float fObligeHeight = -1);
	//进入招式
	// bAnim		- TRUE(调用动作)/FALSE(调用招式)
	// szSkillName	- 动作名或者招式名
	// bLoop		- 是否循环
	// fFuseParam	- 骨骼动画融合参数 
	virtual VOID	Actor_EnterSkill(BOOL bAnim, LPCTSTR szSkillName, BOOL bLoop, FLOAT fFuseParam);
	virtual VOID	Actor_SetHairColor(UINT HairColor);
	// 切换动画的播放速度
	// fRate		- 缩放比率
	virtual VOID	Actor_ChangeActionRate(FLOAT fRate);
	//设置VisibleFlag
	virtual VOID	Actor_SetUIVisibleFlag(VOID);
	//设置缺省动作
	virtual VOID	Actor_SetDefaultAnim(LPCTSTR szAnimName);
	//设置透明度
	virtual VOID	Actor_SetTransparent(FLOAT fTransparency, FLOAT fTime);
	// 设置连线特效的目标点
	virtual VOID	Actor_SetEffectExtraTransformInfos(UINT uEffect, fVector3& fvPosition);
	//设置是否出于鼠标Hover状态
	virtual VOID	Actor_SetMouseHover(BOOL bHover);
	//设置角色动画结束通知
	virtual void	SetAnimationEndEvent(FUNC_ONANIMATIONEND pFunc, DWORD dwParam);
	//设置角色动画可以结束通知
	virtual void	SetAnimationCanBreakEvent(FUNC_ONANIMATIONEND pFunc, DWORD dwParam);
	//设置角色动画打击点通知
	virtual void	SetAnimationHitEvent(FUNC_ONANIMATIONEND pFunc, DWORD dwParam);
	//设置角色动画打击点通知
	virtual void	SetAnimationShakeEvent(FUNC_ONANIMATIONEND pFunc, DWORD dwParam);
	//设置位置 (游戏世界坐标)
	virtual VOID	SetPosition(const fVector3& vPos);

    virtual VOID    SetOrientation(const fVector3& vRotate);
	
	//
	virtual VOID	SetLeftWeaponEffect(LPCTSTR szEffectName, UINT color);
	virtual VOID	SetRightWeaponEffect(LPCTSTR szEffectName, UINT color);

	virtual VOID	SetRightWeaponMat(LPCTSTR szWeaponMat, LPCTSTR szMatName);
	virtual VOID	SetLeftWeaponMat(LPCTSTR szWeaponMat, LPCTSTR szMatName);
	//特效相关
	virtual UINT	AddEffect( LPCSTR effectName, LPCSTR locatorName );
	virtual VOID	DelEffect( UINT effect );
	virtual VOID	DelAllEffect( VOID );

	WX::LogicModel*	GetActorImpl( VOID )
	{
		return mModel;
	}

protected:
    WX::LogicModel* mModel;
	
	WX::LogicModel*					m_leftWeaponModel;
	UINT								m_leftWeaponEffect;

	WX::LogicModel*					m_rightWeaponModel;
	UINT								m_rightWeaponEffect;

	BOOL			m_bVisible;

public:
	CRFObj_Actor();
	virtual ~CRFObj_Actor();
};
