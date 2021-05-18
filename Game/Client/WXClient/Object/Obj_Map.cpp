#include "StdAfx.h"
#include "Obj_Map.h"
#include "../World/WorldManager.h"
#include "../Global.h"

WX_IMPLEMENT_DYNAMIC(CObject_Map, GETCLASS(CObject_Phy));
CObject_Map::CObject_Map()
	: m_ivGridPos(-1, -1)
	, m_pZone(NULL)
{
}


CObject_Map::~CObject_Map()
{
}

VOID CObject_Map::SetPosition(const fVector3& fvPosition) 
{ 
	//找到该物体所处的网格
	CScene* pActiveScene = (CScene*)CWorldManager::GetMe()->GetActiveScene();
	if(pActiveScene)
	{
		//当前有激活场景
		m_ivGridPos.x = (INT)pActiveScene->GetZoneX(fvPosition.x);
		m_ivGridPos.y = (INT)pActiveScene->GetZoneZ(fvPosition.z);

		//是否是UI模型
		if(!GetFakeObjectFlag()) 
		{
			//如果已经有注册过的网格，首先反注册
			if(m_pZone)
				m_pZone->UnRegisterObject(this);

			//注册到新网格上
			CZone* pZone = pActiveScene->GetZone(m_ivGridPos.x, m_ivGridPos.y);
			if(!pZone || pZone == m_pZone) return;
			pZone->RegisterObject(this);
		}
	}

	CObject::SetPosition( fvPosition );
}

