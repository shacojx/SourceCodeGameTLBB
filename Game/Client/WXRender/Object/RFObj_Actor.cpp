#include "StdAfx.h"

#include "RFObj_Actor.h"

#include "../Global.h"
#include "../RenderSystem.h"
#include "../WXObj_ProjTex.h"

#include <OgreVector3.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreLogmanager.h>

#include "Core/WXSystem.h"
#include "Core/WXObjectFactoryManager.h"
#include "Core/WXLogicModelObject.h"
#include "Core/WXVariant.h"
#include "Core/WXFlags.h"

#include "ModelSystem/WXLogicModel.h"
#include "EffectSystem/WXEffectBasic.h"

#include "TDException.h"
#include "TDDBC_Struct.h"
#include "TDDataBase.h"
#include "TDException.h"
#include "TDKernel.h"
#include "TDWorldSystem.h"
#include "AxTrace.h"
#define		FOBJ_ACTOR_FILE			_T("logic model name")			//人物模型资源名

CRFObj_Actor::CRFObj_Actor()
	: CWXObj_Object("LogicModel")
{
    mModel = NULL;
	m_bVisible = TRUE;
	m_rightWeaponModel = NULL;
	m_leftWeaponModel = NULL;
}

CRFObj_Actor::~CRFObj_Actor()
{
}

VOID CRFObj_Actor::Destroy(VOID)
{
    CWXObj_Object::Destroy();
}

VOID CRFObj_Actor::Actor_SetFile(LPCTSTR szActorFile)
{
	TDAssert(szActorFile&& m_pWXObject);

	m_pWXObject->setProperty(FOBJ_ACTOR_FILE, WX::String(szActorFile));

//	AxTrace(0, 0, "%s", szActorFile);
	//检测资源Load情况
	mModel = ((WX::LogicModelObject*)m_pWXObject.get())->getLogicModel();

	TDAssert(mModel);
}

VOID CRFObj_Actor::SetVisible(BOOL bVisible)
{
	//确认资源已经Load
	if(!mModel) return;

    mModel->setVisible(bVisible==TRUE);
	
	m_bVisible = bVisible;
}

VOID CRFObj_Actor::Actor_SetObjectProperty(LPCTSTR szName, LPCTSTR szValue)
{
	TDAssert(m_pWXObject);

	//确认资源已经Load
	if(!mModel) return;

	try
	{
		mModel->changeAttribute(szName, szValue);
	}
	catch(...)
	{
		int a=0;
	}
}

VOID CRFObj_Actor::Actor_GetObjectProperty(LPCTSTR szName, STRING& szValue)
{
	TDAssert(m_pWXObject);

	//确认资源已经Load
	if(!mModel) return;

    mModel->getAttribute(szName, szValue);
}

VOID CRFObj_Actor::Actor_GetLocator(LPCTSTR szName, fVector3& fvPosition)
{
	TDAssert(m_pWXObject);

	//确认资源已经Load
	if(!mModel) return;

	Ogre::Matrix4 mx;
	bool bResult = mModel->getLocatorWorldTransform(szName, mx);
	if(bResult)
	{
		fVector3 fvGfx(mx[0][3], mx[1][3], mx[2][3]);

		CRenderSystem::GetMe()->Axis_Trans(CRenderSystem::AX_GFX, fvGfx, CRenderSystem::AX_GAME, fvPosition);
	}
}

inline float SMOOTH(float f1, float f2, float fper, float max_f)
{
	if(abs(f1-f2) > max_f)
	{
		return (int)(f2+0.5f);
	}

	return (int)((float)f1+(f2-f1)*(float)fper+0.5f);
}

BOOL CRFObj_Actor::Actor_GetInfoBoardPos(fVector2& fvPos, const fVector3 *pvObjPos,const float fObligeHeight)
{
	//确认资源已经Load
	if(!mModel) 
		return FALSE;

	Ogre::Vector3 vPos;
	if ( pvObjPos != NULL )
	{
		//坐标转化
		fVector3 fvGfxPos;
		CRenderSystem::GetMe()->Axis_Trans(CRenderSystem::AX_GAME, *pvObjPos, CRenderSystem::AX_GFX, fvGfxPos);

		vPos.x = fvGfxPos.x;
		vPos.y = fvGfxPos.y;
		vPos.z = fvGfxPos.z;
	}
	else
	{
		vPos =  mModel->getSceneNode()->_getDerivedPosition();
	}
	
	//非主角位置
	FLOAT s_fAddOffset = 0.1 * CRenderSystem::GetMe()->GetSacle().y;
	FLOAT s_fAddHeight = abs(mModel->getBoundingBox().getMaximum().y - mModel->getBoundingBox().getMinimum().y );
	
//	if(fObligeHeight > 0)
//		vPos.y += (fObligeHeight + s_fAddOffset);


	//使用数据表中指定高度
	if(fObligeHeight > 0)
		vPos.y += fObligeHeight*CRenderSystem::GetMe()->GetSacle().y;
	else
	//使用Boundbox高度
		vPos.y += abs(mModel->getBoundingBox().getMaximum().y - mModel->getBoundingBox().getMinimum().y );

	//增加0.1m
	vPos.y += 0.1 * CRenderSystem::GetMe()->GetSacle().y;
	
	fVector3 fvGfx(vPos.x, vPos.y, vPos.z);
	fVector3 fvScreen;
	BOOL bVisible = CRenderSystem::GetMe()->Axis_Trans(CRenderSystem::AX_GFX, fvGfx, CRenderSystem::AX_SCREEN, fvScreen);
	
	if(!bVisible) 
		return FALSE;

	//平滑处理
	fvPos.x = SMOOTH(fvPos.x, fvScreen.x, 0.8f, 3.f);
	fvPos.y = SMOOTH(fvPos.y, fvScreen.y, 0.8f, 3.f);

	return TRUE;	
}

VOID CRFObj_Actor::Actor_EnterSkill(BOOL bAnim, LPCTSTR szSkillName, BOOL bLoop, FLOAT fFuseParam)
{
	//确认资源已经Load
	if(!mModel) return;

	mModel->createSkill(szSkillName, bLoop==TRUE, bAnim==TRUE, fFuseParam);
}

// 切换动画的播放速度
// fRate		- 缩放比率
VOID CRFObj_Actor::Actor_ChangeActionRate(FLOAT fRate)
{
	if(!mModel) return;

	mModel->setGlobalAnimationRate(fRate);
}

//设置缺省动作
VOID CRFObj_Actor::Actor_SetDefaultAnim(LPCTSTR szAnimName)
{
	//确认资源已经Load
	if(!mModel) return;

	//mModel->SetDefaultAnimation(szAnimName);
}

VOID CRFObj_Actor::Actor_SetTransparent(FLOAT fTransparency, FLOAT fTime)
{
	if(!mModel) return;

	mModel->setTransparent(fTime, fTransparency);
}

VOID CRFObj_Actor::Actor_SetEffectExtraTransformInfos(UINT uEffect, fVector3& fvPosition)
{
	if(!mModel) return;

	WX::Effect *pEffect = mModel->getEffect(uEffect);
	if(pEffect != NULL)
	{
		WX::TransformInfos tempTransformInfos;
		fVector3 fvGfxPos;
		CRenderSystem::GetMe()->Axis_Trans(CRenderSystem::AX_GAME, fvPosition, CRenderSystem::AX_GFX, fvGfxPos);
		WX::TransformInfo tempTransformInfo(Ogre::Vector3(fvGfxPos.x, fvGfxPos.y, fvGfxPos.z));
		tempTransformInfos.push_back(tempTransformInfo);
		pEffect->setExtraTransformInfos(tempTransformInfos);
	}
}

VOID CRFObj_Actor::Actor_SetHairColor(UINT HairColor)
{
	//确认资源已经Load
	if(!mModel) return;

	Ogre::ColourValue color;
	color.r = (HairColor >> 24)/ 255.0;
	color.g = ((HairColor >> 16) & 0xFF)/255.0;
	color.b = ((HairColor >> 8) & 0xFF)/255.0;
	color.a = (HairColor & 0xFF)/255.0;

	mModel->setHairColour(color);
}

//设置角色动画结束通知
void CRFObj_Actor::SetAnimationEndEvent(FUNC_ONANIMATIONEND pFunc, DWORD dwParam)
{
	//确认资源已经Load
	if(!mModel) return;

	mModel->SetAnimationFinishListener( pFunc, dwParam );
}

void CRFObj_Actor::SetAnimationCanBreakEvent(FUNC_ONANIMATIONEND pFunc, DWORD dwParam)
{
	//确认资源已经Load
	if(!mModel) return;

	//mModel->SetSkillBreakTimeListener( pFunc, dwParam );
}

void CRFObj_Actor::SetAnimationHitEvent(FUNC_ONANIMATIONEND pFunc, DWORD dwParam)
{
	//确认资源已经Load
	if(!mModel) return;

	//mModel->SetSkillHitTimeListener( pFunc, dwParam );
}

void CRFObj_Actor::SetAnimationShakeEvent(FUNC_ONANIMATIONEND pFunc, DWORD dwParam)
{
	//确认资源已经Load
	if(!mModel) return;

	mModel->SetSkillShakeTimeListener(pFunc, dwParam );
}


//特效相关
UINT CRFObj_Actor::AddEffect( LPCSTR effectName, LPCSTR locatorName )
{
	if(!mModel)
		return NULL;
	return mModel->addEffect( Ogre::String(effectName), Ogre::String(locatorName) );
}

VOID CRFObj_Actor::DelEffect( UINT effect )
{
	if(!mModel)
		return ;

	mModel->delEffect( effect );
}

VOID CRFObj_Actor::DelAllEffect( VOID )
{
	if(!mModel)
		return ;
	mModel->delAllEffect( );
}

VOID CRFObj_Actor::Attach_ProjTexture(PROJTEX_TYPE type, BOOL bShow, FLOAT Ring_Range, FLOAT fHeight )
{
    if (!mModel) return;
	bool show = bShow==TRUE && Ring_Range >=0.0f;
	
    switch (type)
    {
    case SELECT_RING:
        {
			if (show)
			{
				mModel->addProjector(WX::MPT_SELECTED, "selected_projector", show, Ring_Range);
			}else
			{
				mModel->removeProjector(WX::MPT_SELECTED, "selected_projector");
			}
            break;
        }
    case SHADOW_BLOB:
        {
            mModel->addProjector(WX::MPT_SHADOW, "shadow_projector", show, Ring_Range);
            break;
        }
    }
}

void CRFObj_Actor::Attach_Object(tEntityNode *pObject, LPCTSTR szAttachLocator)
{
	if ( mModel != NULL && pObject!= NULL && pObject->GetType() == ETYPE_ACTOR && szAttachLocator != NULL )
	{
		CRFObj_Actor *pRFObject = (CRFObj_Actor*)pObject;
		WX::LogicModel* pActorImpl = pRFObject->GetActorImpl();
		if ( pActorImpl != NULL )
		{

			mModel->attachModel( szAttachLocator, pActorImpl );
		}
	}
}

void CRFObj_Actor::Detach_Object(tEntityNode *pObject)
{
	if ( mModel != NULL && pObject!= NULL && pObject->GetType() == ETYPE_ACTOR )
	{
		CRFObj_Actor *pRFObject = (CRFObj_Actor*)pObject;
		WX::LogicModel* pActorImpl = pRFObject->GetActorImpl();
		if ( pActorImpl != NULL )
		{

			mModel->detachModel( pActorImpl );
		}
	}
}

//设置VisibleFlag
VOID CRFObj_Actor::Actor_SetUIVisibleFlag(VOID)
{
	//确认资源已经Load
	if(!mModel) return;

	mModel->setVisibleFlag((unsigned int)WX::OVF_GUI_ELEMENTS);
	mModel->changeAttribute("MovementType", "Normal");
}

VOID CRFObj_Actor::SetPosition(const fVector3& vPos)
{
	TDAssert(m_pWXObject);
	//确认资源已经Load
	if(!mModel) return;

	//坐标转化
	fVector3 fvGfxPos;
	CRenderSystem::GetMe()->Axis_Trans(CRenderSystem::AX_GAME, vPos, CRenderSystem::AX_GFX, fvGfxPos);

    mModel->setPosition(Ogre::Vector3(fvGfxPos.x, fvGfxPos.y, fvGfxPos.z));
}

VOID CRFObj_Actor::SetOrientation(const fVector3& vRotate)
{
	TDAssert(m_pWXObject);

	Ogre::Quaternion qu(Ogre::Radian(vRotate.y), Ogre::Vector3::UNIT_Y);
	mModel->setOrientation(qu);
}

VOID CRFObj_Actor::Actor_SetMouseHover(BOOL bHover)
{
	TDAssert(m_pWXObject);

	mModel->setSelected(bHover==TRUE);
}

VOID CRFObj_Actor::SetLeftWeaponEffect(LPCTSTR szEffectName, UINT color)
{
	Ogre::ColourValue colour;

	colour.a = ((FLOAT)((color & 0xFF)))/255.0f;
	colour.r = (((FLOAT)(color >> 24))/255.0f) * colour.a;
	colour.g = (((FLOAT)(( color >> 16 ) & 0xFF))/255.0f) *colour.a;
	colour.b = (((FLOAT)((color >> 8) & 0xFF))/255.0f) * colour.a;
	

	if(!m_leftWeaponModel)
	{
//		mDObject->changeAttribute("RightWeaponObj", "大刀13.obj");
		m_leftWeaponModel = WX::VariantCast<WX::LogicModel*>(mModel->getAttribute("LeftWeaponObj"));
	}

    if (m_leftWeaponModel)
    {
		m_leftWeaponModel = WX::VariantCast<WX::LogicModel*>(mModel->getAttribute("LeftWeaponObj"));
      
		if(color == 0)
		{
			m_leftWeaponModel->delAllEffect();
		}
		else
		{
			m_leftWeaponModel->delAllEffect();
			m_leftWeaponEffect = m_leftWeaponModel->addEffect(szEffectName,"武器特效点",WX::LogicModel::GTIT_ALL);
			WX::Effect* effect = m_leftWeaponModel->getEffect(m_leftWeaponEffect);

			if (effect)
			{
				effect->setColour(colour);
			}
		}
	}
}

VOID CRFObj_Actor::SetRightWeaponEffect(LPCTSTR szEffectName, UINT color)
{
	Ogre::ColourValue colour;

	colour.a = ((FLOAT)((color & 0xFF)))/255.0f;
	colour.r = (((FLOAT)(color >> 24))/255.0f)*colour.a;
	colour.g = (((FLOAT)(( color >> 16 ) & 0xFF))/255.0f)*colour.a;
	colour.b = (((FLOAT)((color >> 8) & 0xFF))/255.0f)*colour.a;
	

	if(!m_rightWeaponModel)
	{
//		mDObject->changeAttribute("RightWeaponObj", "大刀13.obj");
		m_rightWeaponModel = WX::VariantCast<WX::LogicModel*>(mModel->getAttribute("RightWeaponObj"));
	}

    if (m_rightWeaponModel)
    {
		m_rightWeaponModel = WX::VariantCast<WX::LogicModel*>(mModel->getAttribute("RightWeaponObj"));
        

		if(color == 0)
		{
			m_rightWeaponModel->delAllEffect();
		}
		else
		{
			m_rightWeaponModel->delAllEffect();
			m_rightWeaponEffect = m_rightWeaponModel->addEffect(szEffectName,"武器特效点",WX::LogicModel::GTIT_ALL);
			WX::Effect* effect = m_rightWeaponModel->getEffect(m_rightWeaponEffect);

			if (effect)
			{
				effect->setColour(colour);
			}
		}
	}
}

VOID CRFObj_Actor::SetRightWeaponMat(LPCTSTR szWeaponMat, LPCTSTR szMatName)
{
	//if(!m_rightWeaponModel)
		m_rightWeaponModel = WX::VariantCast<WX::LogicModel*>(mModel->getAttribute("RightWeaponObj"));

	if (m_rightWeaponModel)
	{
		m_rightWeaponModel->changeAttribute(szWeaponMat, szMatName);
	}
}

VOID CRFObj_Actor::SetLeftWeaponMat(LPCTSTR szWeaponMat, LPCTSTR szMatName)
{
	//if(!m_leftWeaponModel)
		m_leftWeaponModel = WX::VariantCast<WX::LogicModel*>(mModel->getAttribute("LeftWeaponObj"));

	if (m_leftWeaponModel)
	{
		m_leftWeaponModel->changeAttribute(szWeaponMat, szMatName);
	}
}
