#pragma once
#include "TDGfxSystem.h"

namespace Ogre
{
	class SceneNode;
}
class CFairyObj_ProjTex;

class CDummyObject : public tEntityNode
{
public:
	//得到类型
	virtual LPCTSTR GetType(VOID) const	 { return EOBJ_TYPE_DUMMY; }
	//从渲染层销毁节点
	virtual VOID	Destroy(VOID);

	//-------------------------------------------------------
	//设置位置 (游戏世界坐标)
	virtual VOID	SetPosition(const fVector3& vPos);
	//设置旋转角度
	virtual VOID	SetOrientation(const fVector3& vRotate);
	//设置是否可见
	virtual VOID	SetVisible(BOOL bVisible);
	//无法被选中
	virtual	BOOL	GetRayQuery(VOID) const{ return FALSE; }

	//附加一个特效
	virtual VOID	Attach_Effect(LPCTSTR szEffectName);
	//附加一个投射纹理
	virtual VOID	Attach_ProjTexture(PROJTEX_TYPE type, BOOL bShow, FLOAT fHeight=1.7f);

protected:
	Ogre::SceneNode*	m_pSceneNode;

	//附属投射阴影
	std::vector< CFairyObj_ProjTex* >	m_vProjTex;

public:
	CDummyObject();
	virtual ~CDummyObject();
};