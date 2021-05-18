/****************************************\
*										*
*			  DummyObject				*
*										*
\****************************************/

#pragma once
#include "RFObject.h"

namespace WX
{
	class Effect;
	class EffectElement;
};

class CRFObj_Dummy : public CWXObject
{
public:
	//得到类型
	virtual EntityType	GetType(VOID) const	 { return ETYPE_DUMMY; }
	//从渲染层销毁节点
	virtual VOID		Destroy(VOID);

	//设置位置 (游戏世界坐标)
	virtual VOID	SetPosition(const fVector3& vPos);
	//设置旋转角度
	virtual VOID	SetOrientation(const fVector3& vRotate);
	//设置是否可见
	virtual VOID	SetVisible(BOOL bVisible);
	//附加一个特效
	virtual VOID	Attach_Effect(LPCTSTR szEffectName);
	virtual VOID	Detach_Effect(void);
	//附加一个投射纹理
	virtual VOID	Attach_ProjTexture(PROJTEX_TYPE type, BOOL bShow, FLOAT Ring_Range=100.0f, FLOAT fHeight=1.7f );

	virtual VOID	SetLeftWeaponEffect(LPCTSTR szEffectName, UINT color) {};
	virtual VOID	SetRightWeaponEffect(LPCTSTR szEffectName, UINT color) {};

	virtual VOID	SetRightWeaponMat(LPCTSTR szWeaponMat, LPCTSTR szMatName) {};
	virtual VOID	SetLeftWeaponMat(LPCTSTR szWeaponMat, LPCTSTR szMatName) {};
protected:
	//场景节点
	Ogre::SceneNode*	m_pSceneNode;
	//附属投射阴影
	std::vector< CWXObj_ProjTex* >	m_vProjTex;
	//附属的特效
	WX::Effect*						m_pEffect;

public:
	CRFObj_Dummy();
	virtual ~CRFObj_Dummy();
};