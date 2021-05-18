#include "RenderSystem.h"
#include "Global.h"
#include "TDException.h"
#include "TDUtil.h"
#include "AxTrace.h"
#include <OgreException.h>
#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreCamera.h>
#include <OgreVector2.h>
#include <OgreOverlayManager.h>
#include <OgreRenderTarget.h>
#include <OgreOverlayElement.h>

#include "Core/WXSystem.h"
#include "Core/TerrainData.h"
#include "Core/FrameStatsListener.h"
#include "Core/WXObjectProxy.h"
#include "Camera/Camera_CharSel.h"
#include "Camera/Camera_Scene.h"
#include "./Object/RFObjDummy.h"
#include "./Object/RFObj_Static.h"
#include "./Object/RFObj_Model.h"
#include "./Object/RFObj_Actor.h"
#include "DebugPanel.h"
#include "HelpPanel.h"
#include "OgreLogmanager.h"
#include "OgreCommon.h"
#include "Core/WXSceneInfo.h"
#include "Core/Terrain.h"
#include "Core/WXResourceSolverHelper.h"
#include "PostFilter/WXPostFilters.h"
#include "PostFilter/WXPostFilterManager.h"
#include "ResourceLoadListener.h"
#include "Core/WXExpatParser.h"
#include "TDVariable.h"
#include "TerrainGrid.h"
#include "TDKernel.h"
#include <OgreParticleSystemManager.h>
#include <OgreParticleSystem.h>
#include "TDProfile.h"
#include "TDSound.h"
#include "ModelSystem/WXLogicModel.h"
#include "RFFakeObjectManager.h"
#include "Core/WXSceneInfo.h"
#include "Core/WXEnviromentObject.h"
#include <OgreMeshManager.h>
#include <OgreSkeletonManager.h>
#include <OgreMaterialManager.h>
#include <OgreSceneManager.h>
#include <OgreStringConverter.h>
#include "TDWorldSystem.h"
#include "TDEventDefine.h"
#include "Core/WXLogicModelObject.h"
#include "TDTimeSystem.h"
#include "ResourceProvider.h"
#include "ModelSystem/WXLogicModelManager.h"

const CHAR CRenderSystem::sz32BitTexturesVar[]				= "View_ColorQuality"; //"yszl";//颜色质量，比特数(16/32位)
const CHAR CRenderSystem::szDefaultTextureFilteringVar[]	= "View_TextureFiltering";//"cy";		//采样(高/中/低)
const CHAR CRenderSystem::szLightmapQualityVar[]			= "View_LightmapQuality";//"dxyy";	//地形阴影(高/低/无)
const CHAR CRenderSystem::szAnimationQualityVar[]			= "View_ObjectMove";//"wtdh";	//静态物体动画(开/关)
const CHAR CRenderSystem::szPostFilterEnabledVar[]			= "View_FullScreenLight";//"qpfg";	//全屏泛光效果(开/关)
const CHAR CRenderSystem::szFSAAVar[]						= "View_Fanhunjiao";//"fhj";	//反混角(1/2/4)
const CHAR CRenderSystem::szShadowTechniqueVar[]			= "View_HumanLightmap";//"rwyy";	//人物阴影(开/关)
const CHAR CRenderSystem::szvsyncVar[]						= "View_Aplomb";//"cztb";	//垂直同步(开/关)

CRenderSystem* CRenderSystem::s_pMe = NULL;
WX_IMPLEMENT_DYNAMIC(CRenderSystem, GETCLASS(tNode));

CRenderSystem::CRenderSystem(VOID)
	: m_pRenderSystem(NULL)
	, m_pCamera_Current(NULL)
{
	s_pMe = this;

	m_pTerrainGrid = NULL;

	m_bDeathMode = FALSE;
	m_nDeachModePercent = 0;

	m_pCamera_Current = 0;
	m_pCamera_CharView = 0;
	m_pCamera_Scene = 0;
	m_pTerrainGrid = 0;
	m_pRaySceneQuery = 0;
	m_pPostFilterManager = 0;
	m_pEnviromentObject = 0;
	m_pDebugPanel = 0;
	m_pFakeObjectManager = 0;

	//创建Fiary
	m_pRenderSystem = new WX::System;
}

CRenderSystem::~CRenderSystem(VOID)
{
}

VOID CRenderSystem::Release(VOID)
{
	if( m_pFakeObjectManager )
	{
		m_pFakeObjectManager->Release();
		delete m_pFakeObjectManager;
		m_pFakeObjectManager = NULL;
	}
	if(m_pCamera_CharView) 
	{
		delete m_pCamera_CharView;
		m_pCamera_CharView = NULL;
	}
	if(m_pCamera_Scene) 
	{
		delete m_pCamera_Scene;
		m_pCamera_Scene = NULL;
	}

	if(m_pDebugPanel)
	{
		delete m_pDebugPanel;
		m_pDebugPanel = NULL;
	}

	if(m_pRaySceneQuery)
	{
		m_pRenderSystem->getSceneManager()->destroyQuery(m_pRaySceneQuery);
		m_pRaySceneQuery = NULL;
	}

	if(m_pTerrainGrid)
	{
		delete m_pTerrainGrid;
		m_pTerrainGrid = NULL;
	}

	if(m_pRenderSystem)
	{
		delete m_pRenderSystem;	m_pRenderSystem = NULL;
	}
}

HWND CRenderSystem::GetWindow(VOID) const
{ 
	return g_hMainWnd; 
}

BOOL CRenderSystem::Axis_CheckValid(AXIS_TYPE typeSource, const fVector3& fvAxis)
{
	register WX::TerrainData* pTerrainData = m_pRenderSystem->getTerrainData();
	//尚未加载
	if(pTerrainData && (pTerrainData->getXSize()==0 || pTerrainData->getZSize()==0))
	{
		pTerrainData = 0;
	}

	switch(typeSource)
	{
		//-----------------------------------------------
	case AX_GAME:
	case AX_PLAN:
		{
			if(pTerrainData)
				return pTerrainData->isValidGrid(std::make_pair((INT)fvAxis.x, (INT)fvAxis.z));
			else
				return TRUE;
		}
		break;

		//-----------------------------------------------
	case AX_GFX:
		{
			if(pTerrainData)
				return pTerrainData->isValidCoord(fvAxis.x, fvAxis.z);
			else
				return TRUE;
		}
		break;

		//-----------------------------------------------
	case AX_SCREEN:
		{
			RECT rect;
			GetClientRect(g_hMainWnd, &rect);

			POINT pt;
			pt.x = (INT)fvAxis.x;
			pt.y = (INT)fvAxis.y;

			return (PtInRect(&rect, pt)==TRUE);
		}
		break;

	default:
		return FALSE;
	}

	return FALSE;
}

BOOL CRenderSystem::Axis_Trans(	AXIS_TYPE typeSource, const fVector3& fvSource,
								AXIS_TYPE typeTar,			fVector3& fvTarget)
{
	register WX::TerrainData*	pTerrainData = m_pRenderSystem->getTerrainData();
	//尚未加载
	if(pTerrainData && (pTerrainData->getXSize()==0 || pTerrainData->getZSize()==0))
	{
		pTerrainData = 0;
	}

	fVector3 fvScale = GetSacle();

	//检查是否是合法的坐标
	fvTarget = fvSource;
	if(!Axis_CheckValid(typeSource, fvSource)) return FALSE;
	if(typeSource == typeTar) return TRUE;

	switch(typeSource)
	{
		//-----------------------------------------------
	case AX_GAME:
		{
			if(AX_PLAN==typeTar) return TRUE;
			else if(AX_GFX == typeTar)
			{
				if(pTerrainData)
				{
					fvTarget.x = pTerrainData->mPosition.x + (FLOAT)fvSource.x * pTerrainData->mScale.x;
					fvTarget.y = pTerrainData->mPosition.y + (FLOAT)fvSource.y * pTerrainData->mScale.y;
					fvTarget.z = pTerrainData->mPosition.z + (FLOAT)fvSource.z * pTerrainData->mScale.z;
				}
				else
				{
					fvTarget.x = (FLOAT)fvSource.x * fvScale.x;
					fvTarget.y = (FLOAT)fvSource.y * fvScale.y;
					fvTarget.z = (FLOAT)fvSource.z * fvScale.z;
				}
				return TRUE;
			}
			else if(AX_SCREEN==typeTar)
			{
				TDThrow("(AX_GAME->AX_SCREEN) Not support yet!");
			}
		}
		break;
		//-----------------------------------------------
	case AX_PLAN:
		{
			if(!pTerrainData) return FALSE;

			FLOAT fGfxX = pTerrainData->mPosition.x + (FLOAT)fvSource.x * pTerrainData->mScale.x;
			FLOAT fGfxZ = pTerrainData->mPosition.z + (FLOAT)fvSource.z * pTerrainData->mScale.z;

			if(AX_GAME==typeTar)
			{
				//取得相地形高度作为Y坐标(游戏坐标)
				fvTarget.y = (pTerrainData->getHeightAtGrid(fvSource.x, fvSource.z)-pTerrainData->mPosition.y)/ pTerrainData->mScale.y;
				return TRUE;
			}
			else if(AX_GFX == typeTar)
			{
				//取得相地形高度作为Y坐标
				fvTarget.x = fGfxX;
				fvTarget.z = fGfxZ;
				fvTarget.y = pTerrainData->getHeightAtGrid(fvSource.x, fvSource.z);
				return TRUE;
			}
			else if(AX_SCREEN==typeTar)
			{
				TDThrow("(AX_PLAN->AX_SCREEN) Not support yet!");
			}
		}
		break;

		//-----------------------------------------------
	case AX_GFX:
		{
			if(AX_GAME==typeTar || AX_PLAN==typeTar)
			{
				if(pTerrainData)
				{
					fvTarget.x = (fvSource.x-pTerrainData->mPosition.x)/pTerrainData->mScale.x;
					fvTarget.y = (fvSource.y-pTerrainData->mPosition.y)/pTerrainData->mScale.y;
					fvTarget.z = (fvSource.z-pTerrainData->mPosition.z)/pTerrainData->mScale.z;
				}
				else
				{
					fvTarget.x = (fvSource.x)/fvScale.x;
					fvTarget.y = (fvSource.y)/fvScale.y;
					fvTarget.z = (fvSource.z)/fvScale.z;
				}
				return TRUE;
			}
			else if(AX_SCREEN==typeTar)
			{
				//TDThrow("(AX_GFX->AX_SCREEN) Not support yet!");
				if(!pTerrainData) return FALSE;
				
				//得到Ogre相机
				Ogre::Camera* pOgreCamera = m_pCamera_Current->GetOgreCamera();
				if(!pOgreCamera) return FALSE;
				if(!(pOgreCamera->isVisible(Ogre::Vector3(fvSource.x, fvSource.y, fvSource.z)))) return FALSE;

				// x:[-1w, 1w]	z:[-1h, 1h]
				Ogre::Vector3 vRet = pOgreCamera->getProjectionMatrix()* pOgreCamera->getViewMatrix() * Ogre::Vector3(fvSource.x, fvSource.y, fvSource.z);

				int nWidth  = m_pRenderSystem->getViewport()->getActualWidth();
				int nHeight = m_pRenderSystem->getViewport()->getActualHeight();

				fvTarget.x = (( vRet.x + 1.0f)*nWidth/2.0f);
				fvTarget.y = ((-vRet.y + 1.0f)*nHeight/2.0f);
					
				return TRUE;
			}
		}
		break;

		//-----------------------------------------------
	case AX_SCREEN:
		{
			if(!pTerrainData) return FALSE;

			Ogre::Vector3 vRenderPos;
			BOOL bRet = m_pRenderSystem->getTerrainIntersects(WX::Point((INT)fvSource.x, fvSource.y), vRenderPos);
			if(!bRet) return FALSE;

			if(AX_GAME==typeTar || AX_PLAN==typeTar)
			{
				return Axis_Trans(AX_GFX, fVector3(vRenderPos.x, vRenderPos.y, vRenderPos.z), AX_GAME, fvTarget);
			}
			else if(AX_GFX==typeTar)
			{
				fvTarget = fVector3(vRenderPos.x, vRenderPos.y, vRenderPos.z);
				return TRUE;
			}
		}
		break;
	}
	return FALSE;
}

fVector3 CRenderSystem::GetSacle(VOID) const
{
	register WX::TerrainData* pTerrainData = m_pRenderSystem->getTerrainData();
	if(pTerrainData && pTerrainData->getXSize()>0 && pTerrainData->getZSize()>0)
		return fVector3(pTerrainData->mScale.x, pTerrainData->mScale.y, pTerrainData->mScale.z);
	else
		return fVector3(100.0f, 100.0f, 100.0f);
}

//初始化
VOID CRenderSystem::Initial(VOID* pParam)
{
#if (_MSC_VER >= 1400)
//	std::locale langLocale("");
//	std::locale::global(langLocale);
	setlocale( LC_CTYPE, "" );
#endif
	//---------------------------------------------------------
	//取得渲染窗口句柄
	TDAssert(pParam);
	HWND hMainWnd = *((HWND*)pParam);
	g_hMainWnd = hMainWnd;

	g_pVaribleSys = (tVariableSystem*)g_pKernel->GetNode("bin\\var");
	TDAssert(g_pVaribleSys);

	g_pEventSys = (tEventSystem*)g_pKernel->GetNode("bin\\event");
	TDAssert(g_pEventSys);

	//---------------------------------------------------------
	//初始化RenderSystem
	m_pRenderSystem->init("WXResources.cfg", "", "", "WXRender.log");

	//---------------------------------------------------------
	//加载OGRE插件
	g_pDebuger = (tDebuger*)g_pKernel->GetNode("bin\\debuger");

	g_pSoundSystem = (tSoundSystem*)g_pKernel->GetNode("bin\\snd");
	TDAssert(g_pSoundSystem);

	g_pWorldSystem = (tWorldSystem*)g_pKernel->GetNode("bin\\worldman");
	TDAssert(g_pWorldSystem);

	g_pTimeSystem = (tTimeSystem*)g_pKernel->GetNode("bin\\time");
	TDAssert(g_pTimeSystem);

	g_pResourceProvider = (CResourceProvider*)g_pKernel->GetNode("bin\\resprovider");
	TDAssert(g_pResourceProvider);

	//STRING str = g_pVaribleSys->GetAs_String("Gfx_API");
	Ogre::Root* pTheRoot = &(Ogre::Root::getSingleton());

	//选择渲染系统
	//if(stricmp(g_pVaribleSys->GetAs_String("Gfx_API").c_str(), "Direct3D") == 0)
	//{
	//	pTheRoot->loadPlugin("RenderSystem_Direct3D9.dll");
	//	pTheRoot->setRenderSystem(pTheRoot->getRenderSystemByName("Direct3D9 Rendering Subsystem"));
	//}
	//else if(stricmp(g_pVaribleSys->GetAs_String("Gfx_API").c_str(), "OpenGL") == 0)
	//{
	//	TDThrow("Don't Support GL RenderSystem!");
	//	//pTheRoot->loadPlugin("RenderSystem_GL.dll");
	//	//pTheRoot->setRenderSystem(pTheRoot->getRenderSystemByName("OpenGL Rendering Subsystem"));
	//}
	//else 
	//	TDThrow("UnKnown RenderSystem!");


	//使用D3D渲染
	pTheRoot->loadRenderPlugins_D3D9();
	//加载其他插件
	pTheRoot->loadInnerPlugins();
	//pTheRoot->loadPlugin("Plugin_ParticleFX.dll");
	//pTheRoot->loadPlugin("Plugin_ParticleFX2.dll");
	//pTheRoot->loadPlugin("Plugin_OctreeSceneManager.dll");
	//pTheRoot->loadPlugin("Plugin_CgProgramManager.dll");

	//分析资源
	ScriptParsingListener listener;
	try 
	{
		//传入渲染窗口句柄
		WX::System::NameValuePairList ViewSetting;
		INT nFSAA = g_pVaribleSys->GetAs_Int(szFSAAVar);
		INT nVsync = g_pVaribleSys->GetAs_Int(szvsyncVar);

		switch(nFSAA)
		{
		case 0:
			ViewSetting["FSAA"]  = Ogre::StringConverter::toString(0);
			break;
		case 1:
			ViewSetting["FSAA"]  = Ogre::StringConverter::toString(2);
			break;
		case 2:
			ViewSetting["FSAA"]  = Ogre::StringConverter::toString(4);
			break;
		default:
			ViewSetting["FSAA"]  = Ogre::StringConverter::toString(0);
			break;
		}

		if(nVsync == 1)
			ViewSetting["vsync"] = "true";
		else
			ViewSetting["vsync"] = "false";

		m_pRenderSystem->setup((WX::ulong)(UINT_PTR)g_hMainWnd, &ViewSetting);

		//关闭OGRE图标
		m_pRenderSystem->getFrameStatsListener()->showLogoPanel(FALSE);
		
		//状态板
		m_pRenderSystem->getFrameStatsListener()->showStatPanel(FALSE);

		//创建射线交集
		m_pRaySceneQuery = m_pRenderSystem->getSceneManager()->createRayQuery(Ogre::Ray());
        m_pRaySceneQuery->setSortByDistance(TRUE);

		//---------------------------------------------------------
		//设置DebugPanel
		m_pDebugPanel = new CDebugPanel(
								Ogre::OverlayManager::getSingleton().getByName("WX/DebugOverlay"),
								Ogre::OverlayManager::getSingleton().getOverlayElement("WX/StatPanel"), 
								Ogre::OverlayManager::getSingleton().getOverlayElement("WX/HitObjPanel"), 
								Ogre::OverlayManager::getSingleton().getOverlayElement("WX/HitObjText"), 
								m_pRenderSystem->getRenderWindow());

		//---------------------------------------------------------
		//创建摄像机
		{
			//设置摄像机
			//取得环境变量
		
			BOOL bHave;
			FLOAT fCamera_MinDistance = g_pVaribleSys->GetAs_Float("Camera_MinDistance", &bHave);
			if(bHave) CCamera_Scene::MIN_DISTANCE = fCamera_MinDistance;

			FLOAT fCamera_MaxDistance = g_pVaribleSys->GetAs_Float("Camera_MaxDistance", &bHave);
			if(bHave) CCamera_Scene::MAX_DISTANCE = fCamera_MaxDistance;

			FLOAT fCamera_Pitch = g_pVaribleSys->GetAs_Float("Camera_Pitch", &bHave);
			if(bHave) CCamera_Scene::STATIC_PITCH = -fCamera_Pitch*TDU_PI/180.0f;
		}

		m_pCamera_CharView = new CCamera_CharSel(m_pRenderSystem->getCamera());
		m_pCamera_Scene = new CCamera_Scene(m_pRenderSystem->getCamera());
		m_pCamera_Scene->SetDistance(CCamera_Scene::MAX_DISTANCE);

		m_pCamera_Current = m_pCamera_CharView;

		//--------------------------------------------
		//初始化PostFilter系统
		m_pPostFilterManager = m_pRenderSystem->getPostFilterManager();
		//注册所有全局渲染器
		WX::registerAllPostFilterFactories(m_pPostFilterManager);
		m_pPostFilterManager->createPostFilter("Floodlighting", m_pRenderSystem->getViewport());
//		m_pPostFilterManager->createPostFilter("Bloom", m_pRenderSystem->getViewport());
//		m_pPostFilterManager->createPostFilter("HeatVision", m_pRenderSystem->getViewport());
		m_pPostFilterManager->createPostFilter("Fade", m_pRenderSystem->getViewport());

		//--------------------------------------------
		//设置特效系统的声音播放函数
		WX::LogicModel::SetPlaySoundFuncton((OnPlaySound)(g_pSoundSystem->GetPlaySoundFunc()));
		WX::LogicModel::SetStopSoundFunction((OnStopSound)(g_pSoundSystem->GetStopSoundFunc()));
		WX::LogicModel::SetGetHeightInWorldListener((GetHeightInWorld)(g_pWorldSystem->GetTerrainHeightFunc()));

		// 初始化 fake obeject manager
		m_pFakeObjectManager = new CFakeObjectManager;
		m_pFakeObjectManager->Initial();

//		WX::Effect::setDefaultIterativeInterval(0.01f);
		Ogre::ParticleSystem::setDefaultIterationInterval(0.033f);

		//系统设置接口挂接变量控制系统
		g_pEventSys->RegisterEventHandle("VARIABLE_CHANGED",_OnVariableChangedEvent);

		//恢复上次颜色设置
		g_pEventSys->PushEvent(
            GE_VARIABLE_CHANGED, 
			sz32BitTexturesVar, 
			g_pVaribleSys->GetAs_String(sz32BitTexturesVar).c_str());

	} 
	catch(const Ogre::Exception& e)
	{
        BOOL rethrow = TRUE;

        if (e.getNumber() == Ogre::Exception::ERR_DUPLICATE_ITEM)
        {
            if (e.getSource() == "ResourceManager::add")
            {
                if (Ogre::StringUtil::endsWith(listener._scriptName, ".material", TRUE))
                {
                    char materialName[256];
                    if (sscanf(e.getDescription().c_str(), "Resource with the name %255s already exists.", materialName) == 1)
                    {
                        Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(materialName);
                        if (!material.isNull())
                        {
                            char message[1024];

                            Ogre::StringVectorPtr currentNames = WX::findResourceFilenames(listener._groupName, listener._scriptName);
                            const Ogre::String& currentName = currentNames.isNull() || currentNames->empty() ? listener._scriptName : currentNames->front();
                            if (material->getOrigin().empty())
                            {
								_snprintf(message, 1024,
										("Predefined material \"%s\" duplicated in file \"%s\", the application will exit."),
                                        material->getName().c_str(), currentName.c_str());
                            }
                            else
                            {
                                Ogre::StringVectorPtr originNames = WX::findResourceFilenames(material->getGroup(), material->getOrigin());
                                const Ogre::String& originName = originNames.isNull() || originNames->empty() ? material->getOrigin() : originNames->front();
                                if (currentName == originName)
                                {
									_snprintf(message, 1024,
												("The material \"%s\" occuring in the file \"%s\" more than once, the application will exit."),
			                                    material->getName().c_str(), currentName.c_str());
                                }
                                else
                                {
									_snprintf(message, 1024,
											("Duplicate material \"%s\" in files \"%s\" and \"%s\", the application will exit."),
                                            material->getName().c_str(), currentName.c_str(), originName.c_str());
                                }
                            }

							TDThrow("Ogre::%s", message);
                        }
                    }
                }
            }
		}
		
		throw;
	}
}

VOID CRenderSystem::Tick(VOID)
{
	const INT nStep = 5;
	CHAR szParam[32];
	if(m_bDeathMode && m_nDeachModePercent < 100)
	{
		m_nDeachModePercent += nStep;

		_snprintf(szParam, 32, "%f", (100.0f-m_nDeachModePercent)/100.0f );
		m_pPostFilterManager->setPostFilterParameter("Fade", "ColourAmount", szParam);

		_snprintf(szParam, 32, "%f", (FLOAT)m_nDeachModePercent/100.0f );
		m_pPostFilterManager->setPostFilterParameter("Fade", "GrayAmount", szParam);
	}
	else if(!m_bDeathMode && m_nDeachModePercent>0)
	{
		m_nDeachModePercent-=nStep;

		_snprintf(szParam, 32, "%f", (100.0f-m_nDeachModePercent)/100.0f );
		m_pPostFilterManager->setPostFilterParameter("Fade", "ColourAmount", szParam);

		_snprintf(szParam, 32, "%f", (FLOAT)m_nDeachModePercent/100.0f );
		m_pPostFilterManager->setPostFilterParameter("Fade", "GrayAmount", szParam);

		if(m_nDeachModePercent <= 0)
		{
			m_pPostFilterManager->setPostFilterEnabled("Fade", false);
		}
	}
}

VOID CRenderSystem::Camera_SetCurrent(CAMERA_TYPE theType)
{
	switch(theType)
	{
	case CHAR_VIEW:
		m_pCamera_Current = m_pCamera_CharView;
		break;

	case SCENE:
		m_pCamera_Current = m_pCamera_Scene;
		break;
	}

	m_pCamera_Current->Update();
}

/// 渲染Loading场景
VOID CRenderSystem::RenderLodingFrame(LPCTSTR szLoding)
{
	RenderFrame();
}

/// 渲染OGRE场景
VOID CRenderSystem::RenderFrame(VOID)
{
	if(!Ogre::Root::getSingletonPtr())
	{
		TDThrow("Ogre::Root = 0");
	}

	const UINT MAX_RENDER_INTER_TIME = 2000;

	static UINT s_nLastRenderTime = 0;
	UINT s_nTimeNow = g_pTimeSystem->GetTimeNow();

	if(g_pTimeSystem->CalSubTime(s_nLastRenderTime, s_nTimeNow) > MAX_RENDER_INTER_TIME)
	{
		Ogre::Root::getSingleton().clearEventTimes();
		s_nLastRenderTime = s_nTimeNow;
		return;
	}
	s_nLastRenderTime = s_nTimeNow;
	
	Ogre::Root::getSingleton().renderOneFrame();
}

/// 响应WM_PAINT消息
VOID CRenderSystem::OnPaint(VOID)
{
	RenderFrame();
}

/// 窗口大小发生改变
VOID CRenderSystem::OnSizeChange(UINT message, WPARAM wParam, LPARAM lParam)
{
	//用户点击最大化或者最小化
	if(WM_SIZE == message)
	{
		m_pRenderSystem->resizeWindow(LOWORD(lParam), HIWORD(lParam));
	}
	//用户拖动窗口边缘结束
	else if(WM_EXITSIZEMOVE == message)
	{
		RECT rect;
		GetClientRect(g_hMainWnd, &rect);

		m_pRenderSystem->resizeWindow(rect.right-rect.left, rect.bottom-rect.top);
	}
}

VOID CRenderSystem::Camera_SetLookAt(const fVector3& fvPos)
{
	TDAssert(m_pCamera_Current);

	((CCamera_Scene*)m_pCamera_Current)->SetLookAt(fvPos);
}

/// 缩放相机视口 [0.0f, 1.0f]  0-最近距离 1-最远距离
VOID CRenderSystem::Camera_SetZoom(FLOAT fZoom)
{
	if(fZoom < 0.0f || fZoom > 1.0f) return;
	FLOAT fDistance = CCamera_Scene::MIN_DISTANCE + (CCamera_Scene::MAX_DISTANCE-CCamera_Scene::MIN_DISTANCE)*fZoom;
	m_pCamera_Current->SetDistance(fDistance);
}

VOID CRenderSystem::Camera_SetZoom(BOOL bUp,INT multiple)
{
	const FLOAT ZOOMSPEED = 0.05f;

	FLOAT fZoom = (m_pCamera_Current->GetDistance()-CCamera_Scene::MIN_DISTANCE)/(CCamera_Scene::MAX_DISTANCE-CCamera_Scene::MIN_DISTANCE);
	if (bUp)
	{
		fZoom-=(ZOOMSPEED*multiple);		
		if (fZoom<0.0f) fZoom=0.0f;
	}
	else
	{		
		fZoom+=(ZOOMSPEED*multiple);
		if (fZoom>1.0f) fZoom=1.0f;
	}
	Camera_SetZoom(fZoom);
}

VOID CRenderSystem::Camera_Direction(FLOAT fDir)
{
	m_pCamera_Current->SetDirection(fDir);		
}

VOID CRenderSystem::Camera_AddDirection(FLOAT fMultiple)
{
	FLOAT fDir = m_pCamera_Current->GetDirection() + fMultiple;
	Camera_Direction(fDir);
}

VOID CRenderSystem::Camera_GetWindowToViewportRay(INT nX, INT nY, fRay& fRay)
{
	Ogre::Ray ray = m_pRenderSystem->getWindowToViewportRay(nX, nY);

	fVector3 fGFX = fVector3(ray.getOrigin().x, ray.getOrigin().y, ray.getOrigin().z);
	Axis_Trans(AX_GFX, fGFX, AX_GAME, fRay.mOrigin);

	fRay.mDirection = fVector3(ray.getDirection().x, ray.getDirection().y, ray.getDirection().z);
}

// 震荡一次
VOID CRenderSystem::Camera_VibrateOneTime()
{
	TDAssert(m_pCamera_Current);

	((CCamera_Scene*)m_pCamera_Current)->AddOneVib(10, 8, 2);
}


// 设置眼睛的位置, ogre 坐标
VOID	CRenderSystem::Camera_SetEyePos(const fVector3& fvPos)
{
	
	if(m_pRenderSystem)
	{
		Ogre::Camera* pOgreCamera = m_pRenderSystem->getCamera();

		pOgreCamera->setPosition(Ogre::Vector3(fvPos.x, fvPos.y, fvPos.z));
		
	}
}

// 设置观察点的位置 ogre 坐标.
VOID	CRenderSystem::Camera_SetLookAtPos(const fVector3& fvPos)
{
	if(m_pRenderSystem)
	{
		Ogre::Camera* pOgreCamera = m_pRenderSystem->getCamera();
		pOgreCamera->lookAt(Ogre::Vector3(fvPos.x, fvPos.y, fvPos.z));
	}
}

VOID CRenderSystem::Scene_PreLoad(const STRING& theScenePreload)
{
	char* lpAddress = NULL;
	DWORD dwSize = g_pResourceProvider->loadResource(theScenePreload.c_str(), lpAddress, "General");
	if(dwSize > 0)
	{
		register const char* pMemory = lpAddress;
		const char* pDeadEnd = lpAddress + dwSize + 1;

		char szLine[1024] = {0};
		do
		{
			//读取一行
			pMemory = TDU_GetLineFromMemory(szLine, 1024, pMemory, pDeadEnd);
			if(!pMemory) break;

			WX::LogicModelManager::getSingleton().loadModelResource(szLine);
		}while(true);
		g_pResourceProvider->unloadResource(lpAddress, dwSize);
	}
}

VOID CRenderSystem::Scene_Load(INT nID, const STRING& theSceneDefine)
{
	WX::ExpatParser parserExpat(NULL);
	
	if(m_pTerrainGrid)
	{
		m_pTerrainGrid->destroyRenderable();
	}

	//卸载现有资源
	m_pRenderSystem->resetScene();

	//加载场景文件
	m_pRenderSystem->loadSceneFromResource(&parserExpat, theSceneDefine.c_str(), "General");

	//释放未使用资源
	{
		Ogre::ResourceGroupManager& resManager = Ogre::ResourceGroupManager::getSingleton();

		const Ogre::StringVector& strGroupVec = resManager.getResourceGroups();

		for(int i=0; i<(int)strGroupVec.size(); i++)
		{
			if( strGroupVec[i] != Ogre::ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME &&
				strGroupVec[i] != Ogre::ResourceGroupManager::BOOTSTRAP_RESOURCE_GROUP_NAME)
			{
				resManager.unloadUnreferencedResourcesInGroup(strGroupVec[i]);
			}
		}
	}
	
	//设置LightMap
	m_pRenderSystem->getTerrain()->setLightmapQuality(WX::Terrain::LMQ_LOW);
	m_pRenderSystem->getTerrain()->buildGeometry(m_pRenderSystem->getBaseSceneNode());

	//取得环境特效
	WX::SceneInfo::ObjectsByTypeRange objEnviroment = 
		m_pRenderSystem->getSceneInfo()->findObjectsByType("Enviroment");

	if (objEnviroment.first != objEnviroment.second)
	{
		m_pEnviromentObject = static_cast< WX::EnviromentObject * >(objEnviroment.first->get());
		TDAssert(m_pEnviromentObject);
	}
	else
	{
		m_pEnviromentObject = NULL;
	}
}

VOID CRenderSystem::Scene_SetEnviromentInfo(const fVector3& fvPos)
{
	if(m_pEnviromentObject)
	{
		//坐标转换
		fVector3 fvPosGfx;
		if(!Axis_Trans(AX_GAME, fvPos, AX_GFX, fvPosGfx)) return;

		//设置新的位置
		m_pEnviromentObject->setWeatherParticleSystemPosition(Ogre::Vector3(fvPosGfx.x, fvPosGfx.y+300, fvPosGfx.z));
	}
}

//VOID CRenderSystem::Debug_Scene_SetFloodlighting(FLOAT fBlur, FLOAT fShine)
//{
//	Scene_SetPostFilter("Floodlighting");
//
//	CHAR szTemp[32];
//	
//	_snprintf(szTemp, 32, "%.2f", fBlur);
//	m_pPostFilterManager->setPostFilterParameter("BlurAmount", szTemp);
//
//	_snprintf(szTemp, 32, "%.2f", fShine);
//	m_pPostFilterManager->setPostFilterParameter("ShineAmount", szTemp);
//}

//设置地形阴影
VOID CRenderSystem::Scene_SetLightmapQuality(INT nQuality)
{
	switch(nQuality)
	{
	case 0:
		m_pRenderSystem->getTerrain()->setLightmapQuality(WX::Terrain::LMQ_NONE);
		m_pRenderSystem->getTerrain()->buildGeometry(m_pRenderSystem->getBaseSceneNode());
		break;

	case 1:
		m_pRenderSystem->getTerrain()->setLightmapQuality(WX::Terrain::LMQ_LOW);
		m_pRenderSystem->getTerrain()->buildGeometry(m_pRenderSystem->getBaseSceneNode());
		break;

	case 2:
		m_pRenderSystem->getTerrain()->setLightmapQuality(WX::Terrain::LMQ_HIGH);
		m_pRenderSystem->getTerrain()->buildGeometry(m_pRenderSystem->getBaseSceneNode());
		break;
	
	default:
		m_pRenderSystem->getTerrain()->setLightmapQuality(WX::Terrain::LMQ_HIGH);
		m_pRenderSystem->getTerrain()->buildGeometry(m_pRenderSystem->getBaseSceneNode());
		break;
	}
}

VOID CRenderSystem::Scene_ShowGridSwitch(INT nZoneSize)
{
	if(m_pTerrainGrid == NULL)
	{
		m_pTerrainGrid = new WX::TerrainGrid(m_pRenderSystem->getSceneManager(), nZoneSize);
	}

	//显示网格
	m_pTerrainGrid->setActive(!(m_pTerrainGrid->isActive()));
}

//显示设置全局渲染效果
VOID CRenderSystem::Scene_SetPostFilter(LPCTSTR szFilterName)
{
	//设置全局渲染特效
	m_pPostFilterManager->setPostFilterEnabled(szFilterName, true);

	//取得环境变量
	g_pVaribleSys->SetVariable("Gfx_PostFilter", szFilterName);
}

//进入/退出死亡全局渲染效果
VOID CRenderSystem::Scene_SetPostFilter_Death(BOOL bEnter)
{
	if(bEnter == m_bDeathMode) return;

	if(bEnter)
	{
		m_pPostFilterManager->setPostFilterEnabled("Fade", true);
		m_nDeachModePercent = 0;

		m_pPostFilterManager->setPostFilterParameter("Fade", "ColourAmount", "1.0");
		m_pPostFilterManager->setPostFilterParameter("Fade", "GrayAmount", "0.0");
	}
	else
	{
		m_nDeachModePercent = 100;
	}

	m_bDeathMode = bEnter;
}

tEntityNode* CRenderSystem::New_EntityObject(tEntityNode::EntityType type)
{
	switch(type)
	{
	case tEntityNode::ETYPE_DUMMY:
		return new CRFObj_Dummy;

	case tEntityNode::ETYPE_MESH:
		return new CRFObj_Static;

	case tEntityNode::ETYPE_MODEL:
		return new CRFObj_Model;

	case tEntityNode::ETYPE_ACTOR:
		return new CRFObj_Actor;

	default:
		return NULL;
	}
}

VOID CRenderSystem::Destroy_EntityObject(tEntityNode* pNode)
{
	TDAssert(pNode);

	delete ((CWXObject*)pNode);

	//之后不能调用和该node相关的任何代码
	return;
}

tEntityNode* CRenderSystem::Find_HitWXObject(INT nX, INT nY)
{
	Ogre::Ray ray = m_pRenderSystem->getWindowToViewportRay(WX::Point(nX, nY));
	m_pRaySceneQuery->setRay(ray);

	//按照逻辑优先级的查询队列
	std::map< INT, tEntityNode* > mapRayQuery;

	const Ogre::RaySceneQueryResult& queryResult = m_pRaySceneQuery->execute();
	for (Ogre::RaySceneQueryResult::const_iterator it = queryResult.begin(); it != queryResult.end(); ++it)
	{
		Ogre::MovableObject* pMovable = it->movable;
		WX::ObjectPtr object = WX::getObjectFromMovable(pMovable);
		if (object&&object->getData())
		{
			tEntityNode* pEntityNode = (tEntityNode*)(object->getData());
			//不是游戏逻辑中动态生成的
			if(!pEntityNode) continue;
			//不再被关心
			if(!(pEntityNode->GetRayQuery())) continue;

			if(pEntityNode->GetType() == tEntityNode::ETYPE_ACTOR) // object->getType() == "LogicModel")
            {
                WX::LogicModelObject* logicModelObject = static_cast<WX::LogicModelObject*>(object.get());

                assert (logicModelObject);

                WX::LogicModel* model = logicModelObject->getLogicModel();

                assert (model);

				//不再自定义BoundingBox中
				if (model->isUseExternalBoundingBox() && !(model->rayIntersect(ray)))
                {        
                    continue;
                }
            }

			//如果已经有同级存在，按照摄像机远近排序
			if(mapRayQuery.find(pEntityNode->RayQuery_GetLevel()) != mapRayQuery.end()) continue;

			//放入查询队列
			mapRayQuery.insert(std::make_pair(pEntityNode->RayQuery_GetLevel(), pEntityNode));
		}
	}

	//返回优先级最高的逻辑对象
	if(!mapRayQuery.empty()) 
	{
		return mapRayQuery.begin()->second;
	}

	return NULL;
}

VOID CRenderSystem::PushDebugString(LPCTSTR szMsg, ...)
{
}

VOID CRenderSystem::SetDebugString(LPCTSTR szName, LPCTSTR szValue)
{
	if(!m_pDebugPanel) return;
	m_pDebugPanel->SetDebugString(szName, szValue);
}

inline FLOAT CRenderSystem::Camera_GetDirection(VOID) const 
{
	return m_pCamera_Current->GetDirection();
}

inline FLOAT CRenderSystem::Camera_GetZoom(VOID) const 
{
	return m_pCamera_Current->GetDistance();
}

VOID CRenderSystem::SetHitObjText(LPCTSTR szHitObjText)
{
	m_pDebugPanel->SetHitObjString(szHitObjText);
}

VOID CRenderSystem::Debug_AddParticle(LPCTSTR szParticleName, const fVector3& fvPos)
{
	//static int name = 0;
	//char szTemp[MAX_PATH];
	//_snprintf(szTemp, MAX_PATH, "__test_%d", name++);

	//Ogre::ParticleSystem *system = m_pRenderSystem->getSceneManager()->createParticleSystem(szTemp,	szParticleName);
	//assert (system);
	//Ogre::SceneNode *node = m_pRenderSystem->getBaseSceneNode()->createChildSceneNode();

	//node->attachObject(system);

	//fVector3 fvGfx;
	//Axis_Trans(AX_GAME, fvPos, AX_GFX, fvGfx);

	//node->setPosition(Ogre::Vector3(fvGfx.x, fvGfx.y, fvGfx.z));//
}


//创建一个Fake渲染对象
VOID CRenderSystem::FakeObj_Create(LPCTSTR szObjectName, tEntityNode* pNode, LPCTSTR szCameraName)
{
	m_pFakeObjectManager->CreateObject(szObjectName, pNode, szCameraName);
}

//销毁一个Fake渲染对象
VOID CRenderSystem::FakeObj_Destroy(LPCTSTR szObjName)
{
	m_pFakeObjectManager->DestroyObject(szObjName);
}

//隐藏/显示Fake渲染对象
VOID CRenderSystem::FakeObj_Show(LPCTSTR szObjName, BOOL bVisible, float fAspectRatio)
{
	m_pFakeObjectManager->SetVisible(szObjName, bVisible, fAspectRatio);
}

VOID CRenderSystem::Debug_SetFakeCamera(INT nType, FLOAT fValue)
{
	m_pFakeObjectManager->Debug_SetFakeCamera(nType, fValue);
}

VOID CRenderSystem::Debug_ToggleShowDebugPanel(VOID)
{
	m_pDebugPanel->ToggleShow();

	bool bShow = !(m_pRenderSystem->getFrameStatsListener()->isStatPanelShown());
	m_pRenderSystem->getFrameStatsListener()->showStatPanel(bShow);

}

VOID WINAPI CRenderSystem::_OnVariableChangedEvent(const EVENT* pEvent, UINT dwOwnerData)
{
	if(pEvent && pEvent->pEventDef && pEvent->pEventDef->idEvent == GE_VARIABLE_CHANGED)
	{
		const STRING& strVariable = pEvent->vArg[0];
		const STRING& strValue = pEvent->vArg[1];

		BOOL bEnable = (strValue=="1");

		if(strVariable == szAnimationQualityVar)
		{
			INT nValue = strValue[0] - '0';

			GetMe()->Scene_SetAnimationQuality( nValue*50 - 50 );
			//低的模型 -50
			//中		0
			//高		50
		}
		else if(strVariable == szLightmapQualityVar)
		{
			INT nValue = strValue[0] - '0';

			GetMe()->Scene_SetLightmapQuality(nValue);
		}

		else if(strVariable == szPostFilterEnabledVar)
		{
			if(bEnable)
				GetMe()->Scene_SetPostFilterEnabled(TRUE);
			else
				GetMe()->Scene_SetPostFilterEnabled(FALSE);			
		}

		else if(strVariable == sz32BitTexturesVar)
		{
			INT nValue = strValue[0] - '0';

			if(nValue == 0)
			{	//16
				GetMe()->Scene_Enable32BitTextures(FALSE);
			}
			else if(nValue == 1)
			{
				//32
				GetMe()->Scene_Enable32BitTextures(TRUE);
			}
			else
			{
				//桌面
				int nDesktopBits = ::GetDeviceCaps(::GetDC(::GetDesktopWindow()), BITSPIXEL);
				if(16 == nDesktopBits)
				{
//					g_pVaribleSys->SetAs_Int(sz32BitTexturesVar, 0, FALSE);
					GetMe()->Scene_Enable32BitTextures(FALSE);
				}
				else
				{
//					g_pVaribleSys->SetAs_Int(sz32BitTexturesVar, 1, FALSE);
					GetMe()->Scene_Enable32BitTextures(TRUE);
				}
			}
		}
		else if(strVariable == szDefaultTextureFilteringVar)
		{
			INT nValue = strValue[0] - '0';

			GetMe()->Scene_SetTextureFiltering(nValue);

		}
	}
}

//设置材质32位/16位转化
VOID CRenderSystem::Scene_Enable32BitTextures(BOOL bSetting)
{
	#if (_MSC_VER >= 1400)
	//	std::locale langLocale("");
	//	std::locale::global(langLocale);
		setlocale( LC_CTYPE, "" );
	#endif

	//Ogre::TextureManager::getSingleton().enable32BitTextures(bSetting==TRUE);
}

//设置采样
VOID CRenderSystem::Scene_SetTextureFiltering(INT nOptions)
{
	switch(nOptions)
	{
	case 0:	
		Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_BILINEAR);
		Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(1);
		break;
	case 1:
		Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_TRILINEAR);
		Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(2);
		break;
	case 2:
		Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
		Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(8);
		break;
	default:
		break;
	}
}

//设置静态物体动画
VOID CRenderSystem::Scene_SetAnimationQuality(INT nOptions)
{
//	WX::System::setAnimationQuality( (bSetting == TRUE)?1:0);
	m_pRenderSystem->setAnimationQuality( nOptions );
}

//设置全屏泛光效果
VOID CRenderSystem::Scene_SetPostFilterEnabled(BOOL bSetting)
{
//	WX::PostFilterManager::setPostFilterEnabled("Floodlighting",(bSetting==TRUE));
	m_pRenderSystem->getPostFilterManager()->setPostFilterEnabled("Floodlighting",(bSetting==TRUE));
}

//设置人物实时阴影
VOID CRenderSystem::Scene_SetShadowTechnique(BOOL bSetting)
{
	//Ogre::SceneManager::setShadowTechnique
	if(bSetting == TRUE)
		m_pRenderSystem->getSceneManager()->setShadowTechnique(Ogre::SHADOWTYPE_NONE);
	else
		m_pRenderSystem->getSceneManager()->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);

}

//将当前屏幕截图到文件中
BOOL CRenderSystem::PrintScreen(LPTSTR szReturnBuf, INT nBufSize)
{
	const char* szScreenShortPath = "..\\ScreenShots";
	CHAR szFileName[MAX_PATH] = {0};
	try
	{
		//时间
		SYSTEMTIME timeNow;
		::GetLocalTime(&timeNow);

		_snprintf(szFileName, MAX_PATH, "%04d_%02d_%02d_%02d_%02d_%02d.jpg",
			timeNow.wYear, timeNow.wMonth, timeNow.wDay, 
			timeNow.wHour, timeNow.wMinute, timeNow.wSecond);

		//创建目录
		::CreateDirectory(szScreenShortPath, 0);

		//全路径
		char szPathFileName[MAX_PATH] = {0};
		_snprintf(szPathFileName, MAX_PATH, "%s\\%s", szScreenShortPath, szFileName);

		m_pRenderSystem->getRenderWindow()->writeContentsToFile(szPathFileName);
	}
	catch(...)
	{
		return FALSE;	
	}

	strncpy(szReturnBuf, szFileName, nBufSize);
	return TRUE;
}

//// 重新设置旧的显示设置， 全屏显示等 2006－4－10；
//// 登录界面之后。 进入游戏中， 调用这个函数。
//void CRenderSystem::ResetOldShowSetting()
//{
//	//-------------------------------------------
//	//其他需要重置的显示设置
//	{
//		const char* szNeedReset[] = 
//		{
//			"View_FullScreen",		//全屏模式
//			"View_ColorQuality",	//颜色质量
//			"View_Resoution",		//分辨率
//		};
//
//		for(int i=0; i<sizeof(szNeedReset)/sizeof(const char*); i++)
//		{
//			g_pEventSys->PushEvent(GE_VARIABLE_CHANGED, szNeedReset[i], 
//				g_pVaribleSys->GetAs_String(szNeedReset[i]).c_str());
//		}
//	}
//}
