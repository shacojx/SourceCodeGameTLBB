#include "StdAfx.h"
#include "EffectObj.h"
#include "RenderSystem.h"
#include "OgreLogmanager.h"
#include "TDDBC_Struct.h"
#include "TDDataBase.h"
#include "Global.h"
#include "TDException.h"
#include "TDKernel.h"
#include "../EffectSystem/FairyEffectManager.h"
#include "../EffectSystem/FairyEffect.h"
#include <OgreSceneNode.h>

CEffectObject::CEffectObject(VOID)
{
}

CEffectObject::~CEffectObject()
{
	std::list< std::pair< Fairy::Effect*, Ogre::SceneNode* > >::iterator it;
	for(it = m_listEffectImpl.begin(); it!=m_listEffectImpl.end(); it++)
	{
		if(it->second)
		{
			Fairy::EffectManager::getSingleton().removeEffect(it->first);
		}
	}

	m_listEffectImpl.clear();
}

HANDLE CEffectObject::AddEffect(INT idEffect, BOOL bLoop, LPCTSTR /*szLocatorName*/)
{
	//创建特效实例
	Fairy::Effect* pEffectIml = Fairy::EffectManager::getSingleton().createEffect("g01_jingangquan_01");
	if(!pEffectIml) return NULL;

	//创建场景节点
	Ogre::SceneNode* pSceneNode = pEffectIml->createSceneNode();

	//保存
	m_listEffectImpl.push_back(std::make_pair(pEffectIml, pSceneNode));

	return pEffectIml;

#if 0
	//-------------------------------------------------------------
	//得到数据库系统模块
	static const tDataBaseSystem* s_pDataBase = NULL;
	if(!s_pDataBase) s_pDataBase = (tDataBaseSystem*)g_pKernel->GetNode("bin\\dbc");
	TDAssert(s_pDataBase);

	//-------------------------------------------------------------
	//从DBC_EFFECT_DEFINE查找具体特效
	static const tDataBase* s_pDBCEffect = NULL;
	if(!s_pDBCEffect) s_pDBCEffect = s_pDataBase->GetDataBase(DBC_EFFECT_DEFINE);
	TDAssert(s_pDBCEffect);

	//查找具体特效
	const _DBC_EFFECT_DEFINE* pEffect = (const _DBC_EFFECT_DEFINE*)s_pDBCEffect->Search_Index_EQU(idEffect);
	//没有找到
	if(!pEffect) return NULL;
	
	//生成Effect对象
	Ogre::Effect* pEffectIml = Ogre::EffectManager::getSingleton().createEffect(pEffect->pEffectType, pEffect->pParam1);
	if(!pEffectIml) return NULL;

	//保存
	m_listEffectImpl.push_back(pEffectIml);

	return (HANDLE)pEffectIml;
#endif
}

VOID CEffectObject::DelEffect(HANDLE hEffect)
{
	std::list< std::pair< Fairy::Effect*, Ogre::SceneNode* > >::iterator it;

	for(it=m_listEffectImpl.begin(); it!=m_listEffectImpl.end(); it++)
	{
		//从链表中删除
		if(it->first == (Fairy::Effect*)hEffect)
		{
			Fairy::EffectManager::getSingleton().removeEffect(it->first);
			m_listEffectImpl.erase(it);
			return;
		}
	}
}

VOID CEffectObject::Destroy(VOID)
{
	CRenderSystem::GetMe()->Destroy_FairyObject(this);
	return;
	//之后不能调用任何代码
}

//设置位置 (游戏世界坐标)
VOID CEffectObject::SetPosition(const fVector3& vPos)
{
	//坐标转换
	fVector3 fvGfx;
	CRenderSystem::GetMe()->Axis_Trans(CRenderSystem::AX_GAME, vPos, CRenderSystem::AX_GFX, fvGfx);

	std::list< std::pair< Fairy::Effect*, Ogre::SceneNode* > >::iterator it;
	for(it=m_listEffectImpl.begin(); it!=m_listEffectImpl.end(); it++)
	{
		it->second->setPosition(Ogre::Vector3(fvGfx.x, fvGfx.y, fvGfx.z));
	}

#if 0
	Ogre::Matrix4 mxPosition = Ogre::Matrix4::IDENTITY;
	mxPosition.makeTrans(fvGfx.x, fvGfx.y, fvGfx.z);

	Ogre::MatrixList vMatrix;
	vMatrix.push_back(mxPosition);

	std::list< Ogre::Effect* >::iterator it;
	for(it=m_listEffectImpl.begin(); it!=m_listEffectImpl.end(); it++)
	{
		(*it)->execute(0, vMatrix);
	}
#endif
}

