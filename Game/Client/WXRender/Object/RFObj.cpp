#include "StdAfx.h"
#include "RFObj.h"
#include "TDException.h"
#include "../RenderSystem.h"
#include "Core/WXSystem.h"
#include "Core/WXObjectFactoryManager.h"
#include <OgreVector3.h>

#define		FOBJ_POSITION			_T("position")				//位置
#define		FOBJ_ORIENTATION		_T("orientation")			//旋转

CWXObj_Object::CWXObj_Object(LPCTSTR szType)
{
	WX::System* pRenderSystem = CRenderSystem::GetMe()->GetRenderSystem();
	//在中创建实体
	m_pWXObject = pRenderSystem->getObjectFactoryManager()->createInstance(WX::String(szType));
	//在ogre中创建渲染对象
	m_pWXObject->createRenderInstance( pRenderSystem );
	//将this指针保留在WXData中
	m_pWXObject->setData((WX::ulong)this);	
	//可被鼠标选中
	m_bRayQuery = TRUE;
}

CWXObj_Object::~CWXObj_Object()
{
}

VOID CWXObj_Object::Destroy()
{
	//销毁WXObj
	m_pWXObject.reset();

	CWXObject::Destroy();
}

VOID CWXObj_Object::SetPosition(const fVector3& vPos)
{
	TDAssert(m_pWXObject);

	//坐标转化
	fVector3 fvGfxPos;
	CRenderSystem::GetMe()->Axis_Trans(CRenderSystem::AX_GAME, vPos, CRenderSystem::AX_GFX, fvGfxPos);

	m_pWXObject->setProperty(FOBJ_POSITION, Ogre::Vector3(fvGfxPos.x, fvGfxPos.y, fvGfxPos.z));
}

VOID CWXObj_Object::SetOrientation(const fVector3& vRotate)
{
	TDAssert(m_pWXObject);

	Ogre::Quaternion qu(Ogre::Radian(vRotate.y), Ogre::Vector3::UNIT_Y);
	m_pWXObject->setProperty(FOBJ_ORIENTATION, qu);
}

VOID CWXObj_Object::SetWXObjectProperty(const char* szPropertyName, const char* szPropertyValue)
{
	TDAssert(m_pWXObject);

	m_pWXObject->setPropertyAsString(szPropertyName, szPropertyValue);
}
