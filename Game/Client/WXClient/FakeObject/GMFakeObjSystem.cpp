#include "StdAfx.h"
#include "GMFakeObjSystem.h"
#include "..\object\ObjectManager.h"
#include "tdgfxsystem.h"//rendersystem.h"
#include "..\object\object.h"
#include "..\procedure\gameprocedure.h"
#include "TDException.h"
#include "TDUISystem.h"
#include "..\WxRender\RenderSystem.h"

WX_IMPLEMENT_DYNAMIC(CFakeObjSystem, GETCLASS(tFakeObjSystem));
CFakeObjSystem* CFakeObjSystem::s_pMe = NULL;
CFakeObjSystem::CFakeObjSystem()
{
	s_pMe = this;
	m_pRotateObj = NULL;
	m_fRotateSpeed = 0.0f;
}

CFakeObjSystem::~CFakeObjSystem()
{
}

tObject* CFakeObjSystem::NewFakeObject(LPCTSTR szClassName, LPCTSTR szObjName, LPCTSTR szCameraName)
{ 
	// 如果已经有这个名字的物体了,就不创建了
	std::map< STRING, FakeObject >::iterator it = m_listFakeMap.find( szObjName );
	if(it != m_listFakeMap.end())
	{
		TDThrow("Fake Object Multi define(%s)", szObjName);
	}

	tObject* pNewNode = CObjectManager::GetMe()->NewObject( szClassName );
	((CObject*)pNewNode)->SetFakeObjectFlag(TRUE);

	FakeObject newFakeObj;
	newFakeObj.bVisible = false;
	newFakeObj.nObjectID= pNewNode->GetID();
	newFakeObj.strName	= szObjName;
	newFakeObj.strCameraName = szCameraName;

	m_listFakeMap.insert(std::make_pair(szObjName, newFakeObj));

	SObjectInit init;
	init.m_fvPos.x = init.m_fvPos.y = init.m_fvPos.z = 100;
	pNewNode->Initial(&init);

	return pNewNode; 
}

VOID CFakeObjSystem::DestroyObject( LPCTSTR szObjName )
{
	std::map< STRING, FakeObject >::iterator it = m_listFakeMap.find(szObjName);
	if(it!=m_listFakeMap.end())
	{
		FakeObject& obj = it->second;

		//找到显示该模型的UI窗口
		std::map< STRING, FakeObject* >::iterator itWin = 
			m_listFakeUI.find(obj.strWindowName);
		if(itWin != m_listFakeUI.end())
		{
			//断开和渲染层、UI层的连接，并释放相应资源
			OnUIUnHook(itWin->first.c_str());
		}

		//销毁逻辑层数据
		CObject* pObj = (CObject*)CObjectManager::GetMe()->FindObject(obj.nObjectID);
		if(pObj)
		{
			pObj->Release();
			CObjectManager::GetMe()->DestroyObject(pObj);
		}

		//从列表中删除
		m_listFakeMap.erase(it);
	}
}

VOID CFakeObjSystem::OnUIHook(LPCTSTR szWindowName, LPCTSTR szObjName)
{
	//察看是否该窗口已经有显示
	std::map< STRING, FakeObject* >::iterator itWin;
	itWin = m_listFakeUI.find(szWindowName);
	if(itWin != m_listFakeUI.end())
	{
		FakeObject* obj = itWin->second;

		//一样的模型
		if(itWin->second->strName == szObjName) return;

		//首先隐藏该窗口上已有的模型
		OnUIUnHook(szWindowName);
	}

	//找到先前创建的FakeObj
	std::map< STRING, FakeObject >::iterator it = m_listFakeMap.find(szObjName);
	if(it == m_listFakeMap.end()) return;
	
	FakeObject& obj = it->second;
	//目前不支持一个模型被多个窗口显示
	TDAssert(obj.strWindowName.empty());
	//确认渲染层物体已经创建
	CObject* pObjPoint = (CObject*)CObjectManager::GetMe()->FindObject(obj.nObjectID);
	TDAssert(pObjPoint && pObjPoint->GetRenderInterface());

	//通知渲染层，创建渲染纹理
	CGameProcedure::s_pGfxSystem->FakeObj_Create(szObjName, pObjPoint->GetRenderInterface(), obj.strCameraName.c_str());
			
	//通知UI创建相应的Image
	if(CGameProcedure::s_pUISystem)
		CGameProcedure::s_pUISystem->FakeObj_Show(szWindowName, szObjName);

	//加入列表
	obj.strWindowName = szWindowName;
	m_listFakeUI.insert(std::make_pair(szWindowName, &obj));
}

VOID CFakeObjSystem::OnUIUnHook(LPCTSTR szWindowName)
{
	std::map< STRING, FakeObject* >::iterator itWin;
	itWin = m_listFakeUI.find(szWindowName);
	if(itWin == m_listFakeUI.end()) return;

	FakeObject& obj = *(itWin->second);

	//通知UI
	if(CGameProcedure::s_pUISystem)
		CGameProcedure::s_pUISystem->FakeObj_Close(szWindowName, obj.strName.c_str());

	//删除渲染层中的渲染纹理
	CGameProcedure::s_pGfxSystem->FakeObj_Destroy(obj.strName.c_str());

	//从列表中删除
	obj.strWindowName.clear();
	m_listFakeUI.erase(itWin);
}

VOID CFakeObjSystem::OnUIShown(LPCTSTR szWindowName, float fAspectRatio)
{
	std::map< STRING, FakeObject* >::iterator it = m_listFakeUI.find(szWindowName);
	if(it == m_listFakeUI.end()) return;

	//通知渲染层，开始渲染该模型
	CGameProcedure::s_pGfxSystem->FakeObj_Show(it->second->strName.c_str(), TRUE, fAspectRatio);
}

VOID CFakeObjSystem::OnUIHiden(LPCTSTR szWindowName)
{
	std::map< STRING, FakeObject* >::iterator it = m_listFakeUI.find(szWindowName);
	if(it == m_listFakeUI.end()) return;

	//通知渲染层，停止渲染该模型
	CGameProcedure::s_pGfxSystem->FakeObj_Show(it->second->strName.c_str(), FALSE, 0.0f);
}

VOID CFakeObjSystem::RotateObj_Begin(LPCTSTR szObjName, FLOAT fAngle)
{
	std::map< STRING, FakeObject >::iterator it = m_listFakeMap.find(szObjName);
	if(it == m_listFakeMap.end()) return;

	//开始旋转模型
	m_pRotateObj = &(it->second);
	m_fRotateSpeed = fAngle;
}

VOID CFakeObjSystem::Obj_ChangeAction(LPCTSTR szObjName, INT SkillID,INT SkillLoop)
{
	std::map< STRING, FakeObject >::iterator it = m_listFakeMap.find(szObjName);
	if(it == m_listFakeMap.end()) return;

	//开始旋转模型
	m_pActionObj = &(it->second);
	CObject* pObj = (CObject*)CObjectManager::GetMe()->FindObject(m_pActionObj->nObjectID);
	((CObject_Character*)pObj)->ChangeAction(SkillID,1.5f,SkillLoop);
}

VOID CFakeObjSystem::RotateObj_End(LPCTSTR szObjName)
{
//	std::map< STRING, FakeObject >::iterator it = m_listFakeMap.find(szObjName);
//	if(it == m_listFakeMap.end()) return;

//	CObject* pObj = (CObject*)(it->second.pObject);

	//停止旋转模型
	m_pRotateObj = NULL;
}

VOID CFakeObjSystem::Initial(VOID*)
{
}

VOID CFakeObjSystem::Tick(VOID)
{
	if(m_pRotateObj)
	{
		CObject* pObj = (CObject*)CObjectManager::GetMe()->FindObject(m_pRotateObj->nObjectID);
		if(!pObj) return;

		FLOAT fAngleNow = pObj->GetFaceDir();
		pObj->SetFaceDir(fAngleNow+m_fRotateSpeed);
	}
}


VOID CFakeObjSystem::Release(VOID)
{
//	m_listFakeMap.clear();
}
