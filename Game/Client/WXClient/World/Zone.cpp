#include "StdAfx.h"
#include "Zone.h"
#include "..\Global.h"
#include "..\Object\Obj_Map.h"
#include "TDException.h"

CZone::CZone()
{

}

CZone::~CZone()
{

}

VOID CZone::Initial(VOID)
{
	//清空数据
	m_setRegisterObject.clear();
	m_listRegion.clear();
	return;
}

VOID CZone::RegisterObject(CObject_Map* pObject)
{
	TDAssert(pObject);

	//判断是否确实是CObject_Map继承类
	if(!g_theKernel.IsKindOf(pObject->GetClass(), GETCLASS(CObject_Map))) return;

	//注册到Set中，如果已经有该id的物体注册，则直接替换
	INT id = pObject->GetID();
	m_setRegisterObject.insert( id );

	pObject->m_pZone = this;
}

VOID CZone::UnRegisterObject(CObject_Map* pObject)
{
	TDAssert(pObject);

	//从Set中搜索该物体，如果找到则删除
	INT id = pObject->GetID();
	std::set< INT >::iterator it = m_setRegisterObject.find( id );
	if(it != m_setRegisterObject.end())
	{
		m_setRegisterObject.erase(it);
	}

	pObject->m_pZone = NULL;
}

VOID CZone::AddRegion(INT id)
{
	std::list<INT>::iterator it;
	it = find(m_listRegion.begin(),m_listRegion.end(),id);
	if (it == m_listRegion.end())
	{
		m_listRegion.push_back(id);		
	}

}