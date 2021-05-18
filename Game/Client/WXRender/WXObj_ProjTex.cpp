#include "StdAfx.h"
#include "WXObj_ProjTex.h"
#include "RenderSystem.h"
#include "TDException.h"
#include "EffectSystem/WXEffect.h"
#include "EffectSystem/WXEffectManager.h"

CWXObj_ProjTex::CWXObj_ProjTex(tEntityNode::PROJTEX_TYPE type, Ogre::SceneNode* pParentNode, FLOAT fAddHeight)
{
    TDAssert(pParentNode);
    mProjectorEffect = NULL;

    Ogre::String effectName;
	switch(type)
	{
	case tEntityNode::REACHABLE:
        effectName = "reachable_projector";
		break;

	case tEntityNode::UNREACHABLE:
        effectName = "unreachable_projector";
		break;

	case tEntityNode::AURA_RUNE:
        effectName = "aura_rune_projector";
		break;

	default:
		TDThrow("Invalid ProjTextType!");
		break;
	}

    mProjectorEffect = WX::EffectManager::getSingleton().createEffect(effectName);
    mProjectorEffect->createSceneNode(pParentNode);

	m_theType = type;
	
	//ÉèÖÃ¸ß¶È
	SetAddHeight(fAddHeight);
}

CWXObj_ProjTex::~CWXObj_ProjTex()
{
    WX::EffectManager::getSingleton().removeEffect(mProjectorEffect);
    mProjectorEffect = NULL;
}

VOID CWXObj_ProjTex::SetAddHeight(FLOAT fAddHeight)
{
	m_fAddHeight = fAddHeight;
}

VOID CWXObj_ProjTex::Show(BOOL bShow)
{
    if (mProjectorEffect)
        mProjectorEffect->setVisible(bShow==TRUE);
}

