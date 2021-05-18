#include "StdAfx.h"
#include "RFObject.h"
#include "../RenderSystem.h"
#include "Core/WXSystem.h"
#include "Core/WXSystem.h"
#include <OgreSceneManager.h>

CWXObject::CWXObject()
{
	m_nRayQueryLevel = RL_UNKNOWN;
	m_nData = 0;
}

CWXObject::~CWXObject()
{
}

VOID CWXObject::Destroy(VOID)
{
	//删除自身
	CRenderSystem::GetMe()->Destroy_EntityObject(this);
	return;
	//之后不能调用任何代码
	//...
}

