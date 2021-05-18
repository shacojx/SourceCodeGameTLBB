#pragma once

#include "TDGfxSystem.h"
#include "FairyObject.h"

namespace Fairy
{
	class TerrainProjector;
};

class CFairyObj_ProjTex;
//从Fairy中直接引申的渲染实体
class CFairyObject : public tEntityNode
{
public:
	Fairy::ObjectPtr	GetFairyObj(VOID) { return m_pFairyObj; }
	//得到类型
	virtual LPCTSTR GetType(VOID) const	 
	{ 
		if( m_pFairyObj) return m_pFairyObj->getType().c_str();
		else return "";
	}
	//从渲染层销毁节点
	virtual VOID	Destroy(VOID);

	//设置MeshFile
	virtual VOID	SetMeshFile(LPCTSTR szMeshFile)			;
	//设置人物模型资源名
	virtual VOID	SetActorFile(LPCTSTR szActorFile)		;
	//设置位置 (游戏世界坐标)
	virtual VOID	SetPosition(const fVector3& vPos)		;
	//设置旋转角度
	virtual VOID	SetOrientation(const fVector3& vRotate)	;
	//设置是否可见
	virtual VOID	SetVisible(BOOL bVisible)				;
	
	//设置投射纹理
	virtual VOID	SetProjTexture(PROJTEX_TYPE type, BOOL bShow, FLOAT fHeight=1.7f);
	
	//设置是否能被鼠标选中
	virtual VOID	SetRayQuery(BOOL bQuery) { m_bRayQuery = bQuery; }
	virtual	BOOL	GetRayQuery(VOID) const	{ return m_bRayQuery; }

	//设置人物模型的属性
	virtual VOID	SetActorObjectProperty(LPCTSTR szName, LPCTSTR szValue);
	virtual VOID	GetActorObjectProperty(LPCTSTR szName, STRING& szValue);
	//取得任务模型上的某关键点位置
	virtual VOID	GetActorLocator(LPCTSTR szName, fVector3& fvPosition);

	//取得"头顶状态点"在屏幕上的位置,如果返回FALSE，表示在屏幕之外,或者没有该点
	virtual BOOL	GetInfoBoardPos(iVector2& ivPos);

	//设置人物动画结束通知
	virtual VOID	SetAnimationEndEvent(FUNC_ONANIMATIONEND pFunc, DWORD dwParam);

	//------------------------------------------------------------
	//动作/招式
public:
	//进入某招式
	// bAnim		- TRUE(调用动作)/FALSE(调用招式)
	// szSkillName	- 动作名或者招式名
	// bLoop		- 是否循环
	virtual VOID	Skill_Enter(BOOL bAnim, LPCTSTR szSkillName, BOOL bLoop);
	//设置缺省动作
	virtual VOID	Skill_SetDefaultAnim(LPCTSTR szAnimName);

public:
	CFairyObject(LPCTSTR szType);
	virtual ~CFairyObject(VOID);

protected:
	//引入FairyObject
	Fairy::ObjectPtr					m_pFairyObj;
	//附属投射阴影
	std::vector< CFairyObj_ProjTex* >	m_vProjTex;
	//是否能被鼠标选中
	BOOL								m_bRayQuery;
	//显示隐藏
	BOOL								m_bVisible;


	//选择环
	Fairy::TerrainProjector*	m_pUnitSelect;
	//选择环挂接点
	Ogre::SceneNode*			m_pUnitSelectNode;
	//阴影斑点
	Fairy::TerrainProjector*	m_pShadowBlob;
	//阴影斑点挂接点
	Ogre::SceneNode*			m_pShadowBlobNode;
	//类型
	STRING						m_strType;
};
