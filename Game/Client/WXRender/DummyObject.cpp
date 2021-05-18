#include "StdAfx.h"
#include "DummyObject.h"
#include "RenderSystem.h"
#include "Global.h"
#include "TDException.h"
#include "FairySystem.h"
#include <OgreSceneManager.h>
#include "FairyObj_ProjTex.h"

CDummyObject::CDummyObject()
{
	m_pSceneNode = CRenderSystem::GetMe()->GetFairySystem()->getSceneManager()->createSceneNode();

	CRenderSystem::GetMe()->GetFairySystem()->getBaseSceneNode()->addChild(m_pSceneNode);
}

CDummyObject::~CDummyObject()
{
	for(INT i=0; i<(INT)m_vProjTex.size(); i++)
	{
		delete m_vProjTex[i];
		m_vProjTex[i] = NULL;
	}

	m_vProjTex.clear();
}

VOID CDummyObject::Destroy(VOID)
{
	CRenderSystem::GetMe()->Destroy_FairyObject(this);
	return;
	//之后不能调用任何代码
}

//设置位置 (游戏世界坐标)
VOID CDummyObject::SetPosition(const fVector3& vPos)
{
	//坐标转换
	fVector3 fvGfx;
	CRenderSystem::GetMe()->Axis_Trans(CRenderSystem::AX_PLAN, 
		fVector3(vPos.x, 0.0f, vPos.z), 
		CRenderSystem::AX_GFX, fvGfx);

	m_pSceneNode->setPosition(fvGfx.x, fvGfx.y, fvGfx.z);
}

VOID CDummyObject::SetOrientation(const fVector3& vRotate)
{

}

//设置是否可见
VOID CDummyObject::SetVisible(BOOL bVisible)
{
	if(bVisible)
	{
		if(!(m_pSceneNode->getParent()))
		{
			CRenderSystem::GetMe()->GetFairySystem()->getBaseSceneNode()->addChild(m_pSceneNode);
		}
	}
	else
	{
		CRenderSystem::GetMe()->GetFairySystem()->getBaseSceneNode()->removeChild(m_pSceneNode);
	}
}

//设置投射纹理
VOID CDummyObject::Attach_ProjTexture(PROJTEX_TYPE type, BOOL bShow, FLOAT fHeight)
{
	//定位现有的ProjTex
	CFairyObj_ProjTex* pShadowProj = NULL;
	for(INT i=0; i<(INT)m_vProjTex.size(); i++)
	{
		if(m_vProjTex[i]->GetType() == type)
		{
			pShadowProj = m_vProjTex[i];
			break;
		}
	}

	//高度转换 GAME->GFX
	//取得放缩系数
	fVector3& fvScale = CRenderSystem::GetMe()->GetSacle();
	fHeight = fHeight*fvScale.y;

	if(bShow)
	{
		//已经存在
		if(pShadowProj)
		{
			pShadowProj->Show(bShow);
			pShadowProj->SetAddHeight(fHeight);
			return;
		}

		//创建
		pShadowProj = new CFairyObj_ProjTex(type, m_pSceneNode, fHeight);

		m_vProjTex.push_back(pShadowProj);
	}
	else
	{
		//已经存在
		if(pShadowProj)
		{
			pShadowProj->Show(bShow);
			pShadowProj->SetAddHeight(fHeight);
			return;
		}
	}
}

