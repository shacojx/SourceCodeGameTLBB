#pragma once
#include "TDGfxSystem.h"

namespace Fairy
{
	class Effect;
};
namespace Ogre
{
	class SceneNode;
};

//特效渲染实体
class CEffectObject : public tEntityNode
{
public:
	//得到类型
	virtual LPCTSTR GetType(VOID) const	 { return EOBJ_TYPE_EFFECT; }
	//从渲染层销毁节点
	virtual VOID	Destroy(VOID);

	//-------------------------------------------------------
	//设置位置 (游戏世界坐标)
	virtual VOID	SetPosition(const fVector3& vPos);
	//设置旋转角度
	virtual VOID	SetOrientation(const fVector3& vRotate)	{	}
	//设置是否可见
	virtual VOID	SetVisible(BOOL bVisible)				{	}
	//取得"头顶状态点"在屏幕上的位置,如果返回FALSE，表示在屏幕之外,或者没有该点
	virtual BOOL	GetInfoBoardPos(iVector2& ivPos)		{ return FALSE; }
	//设置是否能被鼠标选中
	virtual VOID	SetRayQuery(BOOL bQuery){}
	virtual	BOOL	GetRayQuery(VOID) const{ return FALSE; }

	//-------------------------------------------------------
	//增加一个特效
	/*
	|		如果是角色类对象，则挂接到szLocatorName指定的挂接点上, 
	|	如果不是角色类对象，则直接生成一个特效, 返回生成的特效对象
	|		
	|		idEffect		- 特效ID(DBC_EFFECT_DEFINE)
	|		bLoop			- 是否循环播放
	|		szLocatorName	- 如果是绑定在角色上的特效，指定绑定点
	|
	*/
	virtual HANDLE	AddEffect(INT idEffect, BOOL bLoop=TRUE, LPCTSTR szLocatorName = NULL);
	//卸载特效
	virtual VOID	DelEffect(HANDLE hEffect);

public:
	CEffectObject();
	~CEffectObject();

private:
	//Effect Impl
	std::list< std::pair< Fairy::Effect*, Ogre::SceneNode* > >	m_listEffectImpl;
};