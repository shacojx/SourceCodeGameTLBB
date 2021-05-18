#include "StdAfx.h"
#include "RFFakeObjectManager.h"

#include <OgreException.h>
#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreCamera.h>
#include <OgreVector2.h>
#include <OgreOverlayManager.h>
#include <OgreRenderTarget.h>
#include <OgreRenderSystem.h>
#include <OgreOverlayElement.h>
#include <OgreCommon.h>
#include <OgreRenderTargetListener.h>
#include <OgreColourValue.h>
#include <OgreHardwarePixelBuffer.h>

#include "Core/WXFlags.h"
#include "Core/WXSystem.h"
#include "RenderSystem.h"
#include "AxTrace.h"
#include "TDException.h"
#include "..\WxRender\RenderSystem.h"

//=====================================================================
//Ogre ViewPort渲染侦听
class CFakeRenderTargetListener : public Ogre::RenderTargetListener
{
public:
	virtual void preViewportUpdate(const Ogre::RenderTargetViewportEvent& evt)
	{
		Ogre::SceneManager* pSceneManager = CRenderSystem::GetMe()->GetRenderSystem()->getSceneManager();
		CFakeObjectManager::FakeObject* pObj = CFakeObjectManager::GetMe()->GetObjectAsViewPort(evt.source);
		if(!pObj) return;

		//----------------------------------------------------------
		//设置VisibleFlag
		m_nRenderMask = pSceneManager->getVisibilityMask();
		pSceneManager->setVisibilityMask(WX::OVF_GUI_ELEMENTS);
		CFakeObjectManager::GetMe()->SetActiveObject(pObj);

		//----------------------------------------------------------
		//设置环境光
		m_colAmbient = pSceneManager->getAmbientLight();
		pSceneManager->setAmbientLight(Ogre::ColourValue(1.0f, 1.0f, 1.0f, 1.0f));
		m_pLightModel->setPosition(pObj->pCamera->getPosition());
	}

    virtual void postViewportUpdate(const Ogre::RenderTargetViewportEvent& evt)
	{
		Ogre::SceneManager* pSceneManager = CRenderSystem::GetMe()->GetRenderSystem()->getSceneManager();

		CFakeObjectManager::FakeObject* pObj = CFakeObjectManager::GetMe()->GetObjectAsViewPort(evt.source);
		if(!pObj) return;

		//----------------------------------------------------------
		//恢复VisibleFlag
		pSceneManager->setVisibilityMask(m_nRenderMask);
		CFakeObjectManager::GetMe()->SetActiveObject(0);

		//----------------------------------------------------------
		//恢复环境光
		pSceneManager->setAmbientLight(m_colAmbient);

#if 0
		{
			static int index = 0;
			static std::string strLast;
			if(index == 10)
			{
				CHAR szFileName[MAX_PATH];
				_snprintf(szFileName, MAX_PATH, "c:\\%s.tga", pObj->strName.c_str());
				pObj->pRenderTexture->writeContentsToFile(szFileName);
			}
			index++;//

			if(strLast != pObj->strName)
			{
				strLast = pObj->strName;
				index=0;
			}
			
		}
#endif		
	}

	//创建专用灯光
	void createSpecialLight(void)
	{
		m_pLightModel = 
			CRenderSystem::GetMe()->GetRenderSystem()->getSceneManager()->createLight("Light_ForFakeSystem");
		TDAssert(m_pLightModel);

		//设置VisibleFlag
		m_pLightModel->setVisibilityFlags(WX::OVF_GUI_ELEMENTS);
	}

	//销毁灯光
	void destroySpecialLight(void)
	{
		CRenderSystem::GetMe()->GetRenderSystem()->getSceneManager()->destroyLight(m_pLightModel);
	}

	CFakeRenderTargetListener()
		:m_pLightModel(0)
	{
	}
protected:
	//用于临时储存场景渲染参数

	//Visible Flag
	unsigned int			m_nRenderMask;
	//环境光参数
	Ogre::ColourValue		m_colAmbient;
	//用于照亮模型的灯光
	Ogre::Light*			m_pLightModel;
};
CFakeRenderTargetListener	g_theListener;


//=====================================================================
CFakeObjectManager*	CFakeObjectManager::s_pMe = NULL;
CFakeObjectManager::CFakeObjectManager()
{
	s_pMe = this;
	m_fvPosition = Ogre::Vector3(0.f, 0.f, 0.f);
}

CFakeObjectManager::~CFakeObjectManager()
{
}

// 初始化
VOID  CFakeObjectManager::Initial(VOID)
{
	//创建专用灯光
	g_theListener.createSpecialLight();
}

void  CFakeObjectManager::Release()
{
	FakeObjectMap::iterator it = m_mapObject.begin();

	for(it=m_mapObject.begin(); it!=m_mapObject.end(); it++)
	{
		FakeObject& obj = it->second;
		//销毁对象
		_DestroyFakeNode(obj);
	}
	m_mapObject.clear();

	//销毁灯光
	g_theListener.destroySpecialLight();
}

// 把一个物体放到指定得摄影机位置
void CFakeObjectManager::CreateObject(LPCTSTR szObjName, tEntityNode* pNode, LPCTSTR szCameraName)
{
	//物体不存在，创建新的渲染环境
	FakeObjectMap::iterator it = _GetFakeNode(szObjName,pNode,szCameraName);

	if(it == m_mapObject.end()) return;

	it->second.pEntityNode = pNode;
	//将需要渲染的实体放入渲染环境中

	//坐标转化(GFX->Game)
	Ogre::Vector3& fGfxPos = m_fvPosition;
	fVector3 fvGamePos;
	CRenderSystem::GetMe()->Axis_Trans(CRenderSystem::AX_GFX, 
		fVector3(fGfxPos.x, fGfxPos.y, fGfxPos.z),
		CRenderSystem::AX_GAME, fvGamePos);

	pNode->SetPosition(fvGamePos);
	//初始隐藏
	pNode->SetVisible(FALSE);
}

void CFakeObjectManager::SetActiveObject(FakeObject* pObj)
{
	FakeObjectMap::iterator it;
	for(it=m_mapObject.begin(); it!=m_mapObject.end(); it++)
	{
		FakeObject& theObj = it->second;

		theObj.pEntityNode->SetVisible((&theObj == pObj));
	}
}

VOID CFakeObjectManager::Debug_SetFakeCamera(int nType, float fValue)
{
	FakeObjectMap::iterator it = m_mapObject.find("Player_Head");
	if(it == m_mapObject.end()) return;

	FakeObject& obj = it->second;

	Ogre::Camera* pCam = it->second.pCamera;

#if 1
	switch(nType)
	{
	case 2:
		{

			obj.fCameraHeight += fValue;
			_UpdateCamera(obj);

			AxTrace(0, 0, "Height:%f", obj.fCameraHeight);
		}
		break;

	case 3:
		{
			obj.fCameraDistance += fValue;
			_UpdateCamera(obj);

			AxTrace(0, 0, "Distance:%f", obj.fCameraDistance);
		}
		break;
	case 4:
		{
			obj.fCameraPitch += fValue;
			_UpdateCamera(obj);

			AxTrace(0, 0, "Pitch:%f", obj.fCameraPitch);
		}
		break;
	default:
		return;
	}
#endif
}

void CFakeObjectManager::SetVisible(LPCTSTR szObjName, BOOL bVisible, float fAspectRatio)
{
	FakeObjectMap::iterator it = m_mapObject.find(szObjName);
	if(it == m_mapObject.end()) return;
	FakeObject& obj = it->second;

	if(bVisible)
	{
		//设置摄像机AspectRatio
		obj.pCamera->setAspectRatio(fAspectRatio);
	}

	//设置是否自动刷新
	if(obj.pRenderTexture)
		obj.pRenderTexture->setAutoUpdated(bVisible==TRUE);
}

CFakeObjectManager::FakeObjectMap::iterator CFakeObjectManager::_GetFakeNode(LPCTSTR szNodeName, tEntityNode* pNode, LPCTSTR szCameraName)
{
	//缺省摄像机的位置
	static const float s_fHeight	= 0.3f;
	static const float s_fDistance = 1.2f;
	static const int s_nTexWidth = 256;
	static const int s_nTexHeight = 512;
	static const float s_fPitch = 0.21f;

	FakeObjectMap::iterator it = m_mapObject.find(szNodeName);
	if(it != m_mapObject.end()) return it;

	//不存在，创建
	FakeObject newNode;
	newNode.strName = szNodeName;

	//--------------------------------------------------
	//创建RenderTarget
	Ogre::TexturePtr ptrTex =
		Ogre::TextureManager::getSingleton().createManual(
			Ogre::String(szNodeName) + "_RenderTexture",
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			Ogre::TEX_TYPE_2D,
			s_nTexWidth, s_nTexHeight, 1, 0,
			Ogre::PF_R8G8B8A8,
			Ogre::TU_RENDERTARGET,
			0);

	newNode.pTexturePtr = ptrTex;
	newNode.pRenderTexture = ptrTex->getBuffer()->getRenderTarget();
	
	//缺省不刷新
	newNode.pRenderTexture ->setAutoUpdated(false);
	newNode.pRenderTexture ->addListener(&g_theListener);

	//--------------------------------------------------
	//放置摄像机
	Ogre::SceneManager* pScnManager = CRenderSystem::GetMe()->GetRenderSystem()->getSceneManager();
	newNode.pCamera = pScnManager->createCamera(Ogre::String(szNodeName) + "_Camera");

	//放缩系数
	fVector3 fvScale = CRenderSystem::GetMe()->GetSacle();

	newNode.fCameraHeight=s_fHeight;
	newNode.fCameraDistance=s_fDistance;
	newNode.fCameraPitch=s_fPitch;

	STRING szUserCameraValue;
	pNode->Actor_GetObjectProperty(szCameraName, szUserCameraValue);
	if(szUserCameraValue.size() > 2 && szUserCameraValue.find(';') != STRING::npos)
	{
		sscanf(szUserCameraValue.c_str(), "%f;%f", &(newNode.fCameraHeight), &(newNode.fCameraDistance));
	}

	//设置摄像机
	_UpdateCamera(newNode);

	newNode.pCamera->setNearClipDistance(10.f);
	newNode.pCamera->setAspectRatio((float)s_nTexWidth/s_nTexHeight);
	newNode.pCamera->setFOVy(Ogre::Degree(45.0f));	// 经验值
	newNode.pCamera->setProjectionType(Ogre::PT_PERSPECTIVE);	//透视投影 (平行投影 Ogre::PT_ORTHOGRAPHIC)

	//--------------------------------------------------
	//创建ViewPort
	newNode.pViewPort = newNode.pRenderTexture->addViewport(newNode.pCamera, 1);
	newNode.pViewPort->setClearEveryFrame(true);
	newNode.pViewPort->setBackgroundColour(Ogre::ColourValue(0,0,0,0));
	newNode.pViewPort->setOverlaysEnabled(false);
	newNode.pViewPort->setSkiesEnabled(false);

	//--------------------------------------------------
	//加入Map
	m_mapObject.insert(std::make_pair(newNode.strName, newNode));

	it = m_mapObject.find(newNode.strName);
	TDAssert(it != m_mapObject.end());

	//加入索引Map
	m_mapIndexOfViewPort.insert(std::make_pair(newNode.pViewPort, &(it->second)));
	return it;
}

//更新摄像机
void CFakeObjectManager::_UpdateCamera(FakeObject& node)
{
	//放缩系数
	fVector3 fvScale = CRenderSystem::GetMe()->GetSacle();

	//Lookat
	Ogre::Vector3 fvLookAtPos = m_fvPosition;
	fvLookAtPos += Ogre::Vector3(0.0f, node.fCameraHeight*fvScale.y, 0.0f);

	//EyePos
	Ogre::Vector3 fvEyePosPos = m_fvPosition;
	fvEyePosPos += Ogre::Vector3(0.0f, 
		(node.fCameraHeight+node.fCameraDistance*node.fCameraPitch)*fvScale.y, node.fCameraDistance*fvScale.z);

	node.pCamera->setPosition(fvEyePosPos);
	node.pCamera->lookAt(fvLookAtPos);
}

void CFakeObjectManager::_DestroyFakeNode(FakeObject& obj)
{
	//销毁摄像机
	CRenderSystem::GetMe()->GetRenderSystem()->getSceneManager()->destroyCamera(obj.pCamera);

	//销毁ViewPort
	obj.pRenderTexture->removeAllViewports();

	//销毁RenderTexture
	Ogre::TextureManager::getSingleton().remove(
		Ogre::String(obj.strName) + "_RenderTexture");
}

//销毁
void CFakeObjectManager::DestroyObject(LPCTSTR szObjName)
{
	FakeObjectMap::iterator it = m_mapObject.find(szObjName);
	if(it == m_mapObject.end()) return;

	//先从索引表中删除
	std::map< Ogre::Viewport*, FakeObject* >::iterator itIndex;
	itIndex = m_mapIndexOfViewPort.find(it->second.pViewPort);
	if(itIndex != m_mapIndexOfViewPort.end())
	{
		m_mapIndexOfViewPort.erase(itIndex);
	}

	//删除节点数据
	_DestroyFakeNode(it->second);

	m_mapObject.erase(it);
}

CFakeObjectManager::FakeObject*	CFakeObjectManager::GetObjectAsViewPort(Ogre::Viewport* pViewPort)
{
	std::map< Ogre::Viewport*, FakeObject* >::iterator it;
	it = m_mapIndexOfViewPort.find(pViewPort);

	if(it == m_mapIndexOfViewPort.end()) return NULL;

	return it->second;
}


