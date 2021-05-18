#include "StdAfx.h"
#include "..\World\WorldManager.h"
#include "..\Procedure\GameProcedure.h"
#include "..\Procedure\GamePro_Enter.h"
#include "..\Procedure\GamePro_Login.h"
#include "..\Procedure\GamePro_ChangeScene.h"
#include "..\Network\NetManager.h"
#include "CGEnterScene.h"
#include "TDException.h"
#include "CGAskChangeScene.h"
#include "..\Object\ObjectManager.h"
#include "..\Object\Character\Obj_PlayerMySelf.h"
#include "..\Object\tripperobj\GMTripperObj_Resource.h"
#include "..\Object\tripperobj\GMTripperObj_transport.h"
#include "..\DBC\GMDataBase.h"
#include "..\Global.h"
#include "TDUtil.h"
#include "..\Event\GMEventSystem.h"
#include "TDVariable.h"
#include "..\interface\gmgameinterface.h"
#include "..\DataPool\GMDP_CharacterData.h"
#include "..\DataPool\gmuidatapool.h"
#include "..\Action\GMActionSystem.h"
#include "..\Sound\GMSoundSystem.h"
#include "..\WxRender\RenderSystem.h"
//#include "TDDataBase.h"
//#include "TDDBC_Struct.h"

WX_IMPLEMENT_DYNAMIC(CWorldManager, GETCLASS(tWorldSystem));
CWorldManager* CWorldManager::s_pMe = NULL;
CActivePosManager* CActivePosManager::s_pMe = NULL;
CWorldManager::CWorldManager()
{
	s_pMe = this;
	m_pActiveScene = NULL;
	m_pActiveManager = NULL;
	m_Station = WS_NOT_ENTER;
	m_idNextScene = INVALID_ID;
	m_nSearchRange = 1;
}

CWorldManager::~CWorldManager()
{
	if(m_pActiveScene) delete m_pActiveScene;
	m_pActiveScene = NULL;
	if( m_pActiveManager )
	{
		m_pActiveManager->Release();
		delete m_pActiveManager;
		m_pActiveManager = 0;
	}
}

VOID CWorldManager::Initial(VOID*)
{
	//注册事件处理
	CEventSystem::GetMe()->RegisterEventHandle("ON_SCENE_TRANS",	_OnSceneTransEvent);
	CEventSystem::GetMe()->RegisterEventHandle("ON_SERVER_TRANS",	_OnSceneTransEvent);

	//挂接变量系统
	CEventSystem::GetMe()->RegisterEventHandle("VARIABLE_CHANGED",	_OnVariableChangedEvent);
	m_pActiveManager = new CActivePosManager;
	if( m_pActiveManager )
		m_pActiveManager->Initial();

}

//当前场景的ServerID
INT CWorldManager::GetActiveSceneID(void) const
{
	if(m_pActiveScene) return m_pActiveScene->GetSceneDefine()->nServerID;
	else return 0;
}

BOOL CWorldManager::EnterScene(INT nSceneID, INT nCityLevel)
{
	//必须在进入场景流程中
	if(CGameProcedure::GetActiveProcedure() != (CGameProcedure*)CGameProcedure::s_pProcEnter) 
	{
		//TDThrow("Must enter scene at ENTER procedure");
		return FALSE;
	}
	
	DBC_DEFINEHANDLE(s_pSceneDBC, DBC_SCENE_DEFINE);

	const _DBC_SCENE_DEFINE* pSceneDef = 0;

	//如果是玩家城市
	bool bUserCity = (nCityLevel>=0);
	if(bUserCity)
	{
		//查找城市场景
		for(int i=0; i<(int)s_pSceneDBC->GetRecordsNum(); i++)
		{
			const _DBC_SCENE_DEFINE* pTempSceneDef = (const _DBC_SCENE_DEFINE*)s_pSceneDBC->Search_LineNum_EQU(i);

			//Id和等级相同
			if(pTempSceneDef->nServerID==nSceneID && pTempSceneDef->nCityLevel==nCityLevel)
			{
				pSceneDef = pTempSceneDef;
				break;
			}
		}
	}
	else
	{
		//查找场景定义
		pSceneDef = (const _DBC_SCENE_DEFINE*)s_pSceneDBC->Search_First_Column_Equ(
							_DBC_SCENE_DEFINE::SCENE_SERVER_ID_COLUMN, nSceneID);
	}

	if(!pSceneDef)
	{
		//非法的场景ID
		TDThrow(_T("(CWorldManager::EnterScene) Invalid scene ID(%d,%d)"), nSceneID, nCityLevel);
		return FALSE;
	}

	//首先离开现有的场景
	if(GetActiveScene()) 
	{
		//相同的场景
		if(GetActiveSceneID() == pSceneDef->nLocalID) return FALSE;

		//删除当前场景
		m_pActiveScene->LeaveScene();
		delete m_pActiveScene;	m_pActiveScene = NULL;
	}

	//创建新的场景
	m_pActiveScene = new CScene(pSceneDef, bUserCity);

	//加载新的场景， 加载静态物体定义
	m_pActiveScene->Initial();

	//进入场景
	m_pActiveScene->EnterScene();

	//产生进入场景的事件
	CGameProcedure::s_pEventSystem->PushEvent(GE_SCENE_TRANSED, m_pActiveScene->GetSceneDefine()->szSceneMap);

	m_Station = WS_ENTER;

	// 读取场景传送点
	const tDataBase* pdbcbase = CGameProcedure::s_pDataBaseSystem->GetDataBase( DBC_SCENE_POS_DEFINE );
	int nNum = pdbcbase->GetRecordsNum();

	m_ScenePoslistObj.clear();
	m_FlashlistObj.clear();
	m_ActivelistObj.clear();

	MAP_POS_DEFINE data;
	for( int i = 0; i < nNum; i ++ )
	{
		const _DBC_SCENE_POS_DEFINE* pLine = (const _DBC_SCENE_POS_DEFINE*)((tDataBase*)pdbcbase)->Search_LineNum_EQU(i);
		if( pLine->nSceneID == pSceneDef->nLocalID )
		{
			data.pos.x = float( pLine->nXPos );
			data.pos.z = float( pLine->nZPos );
			_snprintf( data.name, 128, "%s", pLine->szAimName );
			m_ScenePoslistObj.push_back( data );
		}
	}

	/*m_pActiveManager->AddActivePos( true, "木婉清", 160,160, GetActiveSceneID() );
	m_pActiveManager->AddFlashPos( false,"", 160,160, GetActiveSceneID() );
	m_pActiveManager->AddFlashPos( false,"", 130,160, GetActiveSceneID() );
	m_pActiveManager->AddFlashPos( false,"", 190,160, GetActiveSceneID() );
	m_pActiveManager->AddFlashPos( false,"", 100,160, GetActiveSceneID() );*/
	return TRUE;
}

BOOL CWorldManager::UpdateUserCity(int nNewLevel)
{
	//是否已经进入一个场景
	if(!m_pActiveScene) return FALSE;
	//不是玩家城市
	if(!(m_pActiveScene->IsUserCity())) return FALSE;

	//更新场景数据
	if(!(m_pActiveScene->UpdateUserCity(nNewLevel))) return FALSE;
	const _DBC_SCENE_DEFINE* pSceneDefine = m_pActiveScene->GetSceneDefine();

	//产生场景更新的事件
	CGameProcedure::s_pEventSystem->PushEvent(GE_ON_SCENE_UPDATED, pSceneDefine->szMiniMap);

	// 读取场景传送点
	const tDataBase* pdbcbase = CGameProcedure::s_pDataBaseSystem->GetDataBase( DBC_SCENE_POS_DEFINE );
	int nNum = pdbcbase->GetRecordsNum();

	m_ScenePoslistObj.clear();
	m_FlashlistObj.clear();
	m_ActivelistObj.clear();

	MAP_POS_DEFINE data;
	for( int i = 0; i < nNum; i ++ )
	{
		const _DBC_SCENE_POS_DEFINE* pLine = (const _DBC_SCENE_POS_DEFINE*)((tDataBase*)pdbcbase)->Search_LineNum_EQU(i);
		if( pLine->nSceneID == pSceneDefine->nLocalID )
		{
			data.pos.x = float( pLine->nXPos );
			data.pos.z = float( pLine->nZPos );
			_snprintf( data.name, 128, "%s", pLine->szAimName );
			m_ScenePoslistObj.push_back( data );
		}
	}

	

	return TRUE;
}

VOID CWorldManager::Tick(VOID)
{
	if(m_pActiveScene)
		m_pActiveScene->Tick();
}

VOID CWorldManager::Release(VOID)
{
	//首先离开现有的场景
	if(m_pActiveScene) 
	{
		m_pActiveScene->LeaveScene();
		delete m_pActiveScene;	m_pActiveScene = NULL;
	}
	m_pActiveScene = NULL;
}

///游戏过程中切换场景
VOID CWorldManager::ChangeScene(INT idTargetScene, const fVector2& fvPos, INT nDirection, BYTE bFlag )
{
	//必须在主流程中
	if(CGameProcedure::GetActiveProcedure() != (CGameProcedure*)CGameProcedure::s_pProcMain) 
	{
		TDThrow("Must change scene at MAIN procedure");
		return;
	}

	//发送请求消息
	CGAskChangeScene msg;
	msg.SetSourSceneID(GetActiveSceneID());
	msg.SetDestSceneID(idTargetScene);
	CNetManager::GetMe()->SendPacket(&msg);
	//马上发送
	CNetManager::GetMe()->ProcessOutput();

	//目标场景名
	DBC_DEFINEHANDLE(s_pSceneDBC, DBC_SCENE_DEFINE);
	const _DBC_SCENE_DEFINE* pSceneDef = 
		(const _DBC_SCENE_DEFINE*)s_pSceneDBC->Search_First_Column_Equ(
		_DBC_SCENE_DEFINE::SCENE_SERVER_ID_COLUMN, idTargetScene);

	//通知UI,关闭相应窗口
	CGameProcedure::s_pEventSystem->PushEvent(GE_PLAYER_LEAVE_WORLD, 
		pSceneDef ? pSceneDef->szName : "");

	//---
	//保存状态
	m_idNextScene = idTargetScene;
	m_fvNextPos = fvPos;
	m_fNextDir = (FLOAT)((nDirection%36)*2.0f*TDU_PI/36.0f);
	INT idDefaultSkill = CActionSystem::GetMe()->GetDefaultAction() ? CActionSystem::GetMe()->GetDefaultAction()->GetDefineID() : 0;

	CGameProcedure::s_pVariableSystem->SetAs_Int("DefaultSkill",idDefaultSkill);
//	CActionSystem::GetMe()->SaveAction();


	m_Station = bFlag ? WS_RELIVE : WS_ASK_CHANGE_SCANE;
}

VOID CWorldManager::_OnSceneTransEvent(const EVENT* pEvent, UINT dwOwnerData)
{
	TDAssert(pEvent);

	switch(pEvent->pEventDef->idEvent)
	{
	case GE_ON_SCENE_TRANS:
		//转入进入场景流程
		{
			// 在同一个服务器中切换场景.
			CGameProcedure::s_pProcEnter->SetStatus(CGamePro_Enter::ENTERSCENE_READY);
			CGameProcedure::s_pProcEnter->SetEnterType(ENTER_TYPE_FROM_OTHER);
			
			CGameProcedure::SetActiveProc(CGameProcedure::s_pProcEnter);
		}
		break;

	case GE_ON_SERVER_TRANS:
		{
			CGameProcedure::s_pProcChangeScene->SetStatus(CGamePro_ChangeScene::CHANGESCENE_DISCONNECT);
			
			// 切换服务器.
			CGameProcedure::SetActiveProc(CGameProcedure::s_pProcChangeScene);
		}
		break;
	}

	CGameProcedure::s_pProcLogIn->LoginForChangeScene();
}

bool WINAPI CWorldManager::_GetTerrainHeight(float fX, float fZ, float& fY)
{
	//非法的状态
	if(WS_ENTER != GetMe()->m_Station || !(GetMe()->GetActiveScene())) return false;
	
	//坐标转换 gfx->game
	fVector3 fvGame;
	CGameProcedure::s_pGfxSystem->Axis_Trans(
		CRenderSystem::AX_GFX, fVector3(fX, 0, fZ), CRenderSystem::AX_GAME, fvGame);

	//返回
	if(GetMe()->m_pActiveScene->GetMapHeight(fVector2(fvGame.x, fvGame.z), fY) == TRUE)
    {
        fVector3 fvGfx;

        //坐标转换 game->gfx
        fvGame.y = fY;
	    CGameProcedure::s_pGfxSystem->Axis_Trans(
	    	CRenderSystem::AX_GAME, fvGame, CRenderSystem::AX_GFX, fvGfx);

        fY = fvGfx.y;
        return true;
    }

    fY = 0.0f;
    return false;
}

bool WINAPI CWorldManager::GetTerrainHeight_GamePos(float fX, float fZ, float& fY)
{
	//非法的状态
	if(WS_ENTER != GetMe()->m_Station || !(GetMe()->GetActiveScene())) return false;

	//返回
	if(GetMe()->m_pActiveScene->GetMapHeight(fVector2(fX, fZ), fY) == TRUE)
    {
        return true;
    }

    fY = 0.0f;
    return false;
}

VOID WINAPI CWorldManager::_OnVariableChangedEvent(const EVENT* pEvent, UINT dwOwnerData)
{
	if(!GetMe()->GetActiveScene()) return;

	if(pEvent && pEvent->pEventDef && pEvent->pEventDef->idEvent == GE_VARIABLE_CHANGED)
	{
		const STRING& strVariable = pEvent->vArg[0];
		const STRING& strValue = pEvent->vArg[1];

		BOOL bEnable = (strValue=="1");

		if(strVariable == CSoundSystemFMod::szBKSoundVar) 
		{
			((CScene*)GetMe()->GetActiveScene())->ControlBkgSound(bEnable);
		}
		else if(strVariable == CSoundSystemFMod::sz3DSoundVar)
		{
			((CScene*)GetMe()->GetActiveScene())->ControlEnvSound(bEnable);
		}
		else if(strVariable == CSoundSystemFMod::szBKVolumeVar)  // 背景音效的音量
		{
			((CScene*)GetMe()->GetActiveScene())->ControlBkgSoundVol();
		}
		else if(strVariable == CSoundSystemFMod::sz3DVolumeVar)  
		{
			((CScene*)GetMe()->GetActiveScene())->ControlEnvSoundVol();
		}

	}
}
///////////////////////////////////////////////////////////////////////
bool	CWorldManager::CheckIsFriend( INT id )
{
	for( int i = 0; i < CUIDataPool::GetMe()->GetTeamOrGroup()->GetTeamMemberCount(); i ++ )
	{
		const TeamMemberInfo* pInfo = CUIDataPool::GetMe()->GetTeamOrGroup()->GetMemberByIndex( i );
		if( pInfo )
		{
			if( pInfo->m_SceneID != m_pActiveScene->GetSceneDefine()->nServerID )
				continue;
			if( pInfo->m_OjbID == id )
				return true;

		}
	}
	return false;
}
void    CWorldManager::AddFriend( CObject_PlayerOther* pObj )
{
	std::list< MAP_POS_DEFINE >::iterator  it;
	for( it = m_FriendlistObj.begin(); it != m_FriendlistObj.end(); it ++ )
	{
		if( it->nServerID == pObj->GetServerID() ) // 如果是队友
		{
			it->pos.x = pObj->GetPosition().x;
			it->pos.z = pObj->GetPosition().z;
		}
	}
}
void	CWorldManager::UpdateMinimapData()
{
	if( m_bAnimyNeedFresh )
	{
		m_AnimylistObj.clear();
		m_ExpNPClistObj.clear();
		m_FriendlistObj.clear();
		m_PlayerlistObj.clear();
		m_ExpObjlistObj.clear();
		//m_ActivelistObj.clear();
		m_PetlistObj.clear();

		MAP_POS_DEFINE data;

		m_pActiveManager->update();
		m_pActiveManager->GetActiveList( GetActiveSceneID(), &m_ActivelistObj );
		m_pActiveManager->GetFlashList(  GetActiveSceneID(), &m_FlashlistObj );
		// 更新队友列表
		for( int i = 0; i < CUIDataPool::GetMe()->GetTeamOrGroup()->GetTeamMemberCount(); i ++ )
		{
			const TeamMemberInfo* pInfo = CUIDataPool::GetMe()->GetTeamOrGroup()->GetMemberByIndex( i );
			if( pInfo )
			{
				/*if( pInfo->m_SceneID != m_pActiveScene->GetSceneDefine()->nID )
					continue;*/
				if( pInfo->m_OjbID == CObjectManager::GetMe()->GetMySelf()->GetServerID() )
					continue; 
				data.pos.x = pInfo->m_WorldPos.m_fX;
				data.pos.z = pInfo->m_WorldPos.m_fZ;
				_snprintf( data.name, 128, "%s", pInfo->m_szNick );
				data.nServerID = pInfo->m_OjbID;
				data.dwSceneID = pInfo->m_SceneID;
				m_FriendlistObj.push_back( data );
			}
		}

		CObject_PlayerMySelf* pCharObj = CObjectManager::GetMe()->GetMySelf();
		const _DBC_SCENE_DEFINE* pSceneDef = GetActiveScene()->GetSceneDefine();
		CScene* pActiveScene = (CScene*)GetActiveScene();
		
		INT nZoneX, nZoneZ;
		int nTmp = 0;
		nZoneX = pActiveScene->GetZoneX( pCharObj->GetPosition().x );
		nZoneZ = pActiveScene->GetZoneZ( pCharObj->GetPosition().z );
		for( INT nX = nZoneX - m_nSearchRange; nX <= nZoneX + m_nSearchRange; nX ++ )
		{
			if( nX < 0 )
				continue;
			if( nX >= pActiveScene->GetSizeX() ) 
				break;
			for( INT nZ = nZoneZ - m_nSearchRange; nZ <= nZoneZ + m_nSearchRange; nZ ++ )
			{
				if( nZ < 0 )
					continue;
				if( nZ >= pActiveScene->GetSizeZ() )
					break;
				// 走到这里说明是合法的zone
				CZone* pZone = pActiveScene->GetZone( nX, nZ );
				std::set<INT>::iterator it;
				for ( it = pZone->GetRigisterObject()->begin(); it != pZone->GetRigisterObject()->end(); it++ )
				{
					tObject* pCurObj = CObjectManager::GetMe()->FindObject( ( *it ) );
					if(!pCurObj) continue;
					if( pCurObj == CObjectManager::GetMe()->GetMySelf() ) // 如果是自己跳出
						continue;

		
					
					nTmp ++;
					// 如果是角色
					if( g_theKernel.IsKindOf( pCurObj->GetClass(), GETCLASS(CObject_Character) ) ) // 如果是角色
					{
						ENUM_RELATION sCamp = CGameInterface::GetMe()->GetCampType( 
							pCharObj, 
							(CObject*)pCurObj );
						// 如果已经死了，就不加如列表了。
						if( ((CObject_Character*)( pCurObj ))->CharacterLogic_Get() == CObject_Character::CHARACTER_LOGIC_DEAD )
							continue;
						if( (( CObject* )pCurObj)->GetFakeObjectFlag() == 1 )
							continue;
						// 判断是不是宠物
						std::list< MAP_POS_DEFINE >::iterator  it;
						bool bNeedReturn = false;
						for( it = m_FlashlistObj.begin(); it != m_FlashlistObj.end(); it ++ )
						{
							MAP_POS_DEFINE data = (*it);
							if( data.nServerID == -1 )
							{
								// 如果名字一样，就不加了
								if( strcmp( data.name , ((CObject_Character*)( pCurObj ))->GetCharacterData()->Get_Name() ) == 0 )
								{
									bNeedReturn = true;
									break;
								}
							}
						}
						for( it = m_ActivelistObj.begin(); it != m_ActivelistObj.end(); it ++ )
						{
							MAP_POS_DEFINE data = (*it);
							if( data.nServerID == -1 )
							{
								// 如果名字一样，就不加了
								if( strcmp( data.name , ((CObject_Character*)( pCurObj ))->GetCharacterData()->Get_Name() ) == 0 )
								{
									bNeedReturn = true;
									break;
								}
							}
						}
						if( bNeedReturn )
							continue;
						if( g_theKernel.IsKindOf( pCurObj->GetClass(), GETCLASS(CObject_PlayerOther) ) ) // 如果是玩家
						{
							if( sCamp == RELATION_FRIEND ) // 如果是同一阵营的
							{
								// 如果是队友,就替掉本来已经存在的位置
								if( CheckIsFriend( pCurObj->GetServerID() ) )
								{
									AddFriend( (CObject_PlayerOther*)pCurObj );
									continue;
								}
								data.pos.x = (( CObject*)pCurObj)->GetPosition().x;
								data.pos.z = (( CObject*)pCurObj)->GetPosition().z;
								_snprintf( data.name, 128, "%s", ((CObject_Character*)( pCurObj ))->GetCharacterData()->Get_Name() );
								m_PlayerlistObj.push_back( data );	
							}
							else if( sCamp == RELATION_ENEMY ) // 敌人
							{
								data.pos.x = (( CObject*)pCurObj)->GetPosition().x;
								data.pos.z = (( CObject*)pCurObj)->GetPosition().z;
								_snprintf( data.name, 128, "%s", ((CObject_Character*)( pCurObj ))->GetCharacterData()->Get_Name() );
								m_AnimylistObj.push_back( data );	
							}
							//else if( sCamp == CAMP_TYPE_TEAM )
							//{
							//	OutputDebugString( "发先对有\n");
							//}
						}
						else if( g_theKernel.IsKindOf( pCurObj->GetClass(), GETCLASS(CObject_PlayerNPC) ) ) // 如果是npc
						{
							if( ((CObject_PlayerNPC*)pCurObj)->IsDisplayBoard() == FALSE)
							{
								continue;
							}
							// 如果是宠物
							if( ((CObject_PlayerNPC*)pCurObj)->GetNpcType() == NPC_TYPE_PET )
							{
								if( ((CObject_PlayerNPC*)pCurObj)->GetOwnerID() == INVALID_ID ) // 入伙还没有主人
								{
									data.pos.x = (( CObject*)pCurObj)->GetPosition().x;
									data.pos.z = (( CObject*)pCurObj)->GetPosition().z;
									_snprintf( data.name, 128, "%s", ((CObject_Character*)( pCurObj ))->GetCharacterData()->Get_Name() );
									m_PetlistObj.push_back( data );	
								}
								continue;
							}								
							if( sCamp == RELATION_FRIEND ) // 如果是同阵营的，是npc,否则是敌人
							{
								data.pos.x = (( CObject*)pCurObj)->GetPosition().x;
								data.pos.z = (( CObject*)pCurObj)->GetPosition().z;
								_snprintf( data.name, 128, "%s", ((CObject_Character*)( pCurObj ))->GetCharacterData()->Get_Name() );
								m_ExpNPClistObj.push_back( data );	
								continue;
							}
							else if( sCamp == RELATION_ENEMY )
							{
								data.pos.x = (( CObject*)pCurObj)->GetPosition().x;
								data.pos.z = (( CObject*)pCurObj)->GetPosition().z;
								_snprintf( data.name, 128, "%s", ((CObject_Character*)( pCurObj ))->GetCharacterData()->Get_Name() );
								m_AnimylistObj.push_back( data );	
							}
						}

					} 
					else if( g_theKernel.IsKindOf( pCurObj->GetClass(), GETCLASS(CTripperObject_Resource) ) )
					{
						data.pos.x = (( CObject*)pCurObj)->GetPosition().x;
						data.pos.z = (( CObject*)pCurObj)->GetPosition().z;
						_snprintf( data.name, 128, "%s", ((CTripperObject_Resource*)pCurObj )->GetLifeAbility()->szName );
						m_ExpObjlistObj.push_back( data );	
					}
					else if( g_theKernel.IsKindOf( pCurObj->GetClass(), GETCLASS(CTripperObject_Transport ) ) )
					{
						data.pos.x = (( CObject*)pCurObj)->GetPosition().x;
						data.pos.z = (( CObject*)pCurObj)->GetPosition().z;
						_snprintf( data.name, 128, "传送点" );
					}
					
				}

			}
		}

		m_bAnimyNeedFresh = false;
	}

}
void	CWorldManager::MinimapNeedFresh()
{
		
	m_bAnimyNeedFresh = true;
	return;
}
std::list< MAP_POS_DEFINE >* CWorldManager::GetObjectListByClass( INT nType )
{
	switch( nType )
	{
	case 0: //Animy
		return &m_AnimylistObj;
	case 1://ExpNPC
		return &m_ExpNPClistObj;
	case 2:// Teamate
		return &m_FriendlistObj;
	case 3://OtherPlayer
		return &m_PlayerlistObj;
	case 4://ExpObj
		return &m_ExpObjlistObj;
	case 5: // 方向
		return &m_ActivelistObj;
	case 6: // 场景跳转点
		return &m_ScenePoslistObj;
	case 7:
		return &m_FlashlistObj;
	case 8: //  宠物点
		return &m_PetlistObj;
	default:
		break;
	}
	return NULL;
}
fVector3 CWorldManager::GetMyselfPos()
{
	CObject_PlayerMySelf* pCharObj = CObjectManager::GetMe()->GetMySelf();
	return pCharObj->GetPosition();
}
int CWorldManager::GetMyselfDir()
{
	CObject_PlayerMySelf* pCharObj = CObjectManager::GetMe()->GetMySelf();
	return INT( pCharObj->GetFaceDir() * 180.f / TDU_PI );
}

const CHAR* CWorldManager::GetSceneName( INT nSceneID )
{
	//查找场景定义
	DBC_DEFINEHANDLE(s_pSceneDBC, DBC_SCENE_DEFINE);
	const _DBC_SCENE_DEFINE* pSceneDef = (const _DBC_SCENE_DEFINE*)s_pSceneDBC->Search_Index_EQU(nSceneID);
	if(!pSceneDef)
	{
		return NULL;
	}

	return pSceneDef->szName;
}
CActivePosManager::CActivePosManager()
{
	s_pMe = this;
	m_listActiveObj.clear();
	m_listFlashObj.clear();
	m_nDistance = 25;
}

CActivePosManager::~CActivePosManager()
{
}

void   CActivePosManager::Initial()
{
}
void   CActivePosManager::Release()
{
}
void  CActivePosManager::AddFlashPos( bool bOnNPC,const char* pPosName, float fX, float fY, int nSceneID )
{
	MAP_POS_DEFINE data;
	data.dwSceneID = nSceneID;
	_snprintf( data.name, 128, "%s", pPosName );
	if( bOnNPC )
		data.nServerID = -1;
	else
		data.nServerID = -2;
	data.pos.x = fX;
	data.pos.z = fY;
	m_listFlashObj.push_back( data );
	char szInfo[ 128 ];
	if( bOnNPC )
		_snprintf( szInfo, 128, "添加FlashPos -->  %s,附着方式npc", pPosName );
	else
		_snprintf( szInfo, 128, "添加FlashPos -->  %s,附着方式位置", pPosName );
	AxTrace( 0,0, szInfo );
	CEventSystem::GetMe()->PushEvent(GE_UPDATE_MAP);
}
void  CActivePosManager::AddActivePos( bool bOnNPC,const char* pPosName, float fX, float fY, int nSceneID )
{
	std::list< MAP_POS_DEFINE >::iterator  it;
	char szInfo[ 128 ];
	for( it = m_listActiveObj.begin(); it != m_listActiveObj.end(); it ++ )
	{
		MAP_POS_DEFINE data = (*it);
		if( data.dwSceneID == nSceneID )
		{
			_snprintf( szInfo, 128, "添加ActivePos -->  %s", data.name );
			AxTrace( 0,0, szInfo );
			m_listActiveObj.erase( it );
			break;
		}
			
	}
	MAP_POS_DEFINE data;
	data.dwSceneID = nSceneID;
	_snprintf( data.name, 128, "%s", pPosName );
	if( bOnNPC )
		data.nServerID = -1;
	else
		data.nServerID = -2;
	data.pos.x = fX;
	data.pos.z = fY;
	m_listActiveObj.push_back( data );
	
	_snprintf( szInfo, 128, "添加ActivePos -->  %s", pPosName );
	AxTrace( 0,0, szInfo );
	CEventSystem::GetMe()->PushEvent(GE_UPDATE_MAP);
}
void   CActivePosManager::update()
{
	update( &m_listFlashObj );
	update( &m_listActiveObj);
}
void   CActivePosManager::update( POS_LIST* pList )
{
	std::list< MAP_POS_DEFINE >::iterator  it;
	fVector3 fMyselfPos = CObjectManager::GetMe()->GetMySelf()->GetPosition();
	fVector3 fAimPos( 0,0,0 );
	for( it = pList->begin(); it != pList->end(); it ++ )
	{
		MAP_POS_DEFINE data = (*it);
		if( data.dwSceneID == CWorldManager::GetMe()->GetActiveSceneID() )
		{
			if( data.nServerID == -1 ) // 如果是绑定在npc身上的
			{
				CObject_Character* pObj = CObjectManager::GetMe()->FindCharacterByName( data.name );
				if( pObj )
				{
					fAimPos = pObj->GetPosition();
					(*it).pos = fAimPos;
				}
			}
			else
			{
				fAimPos = data.pos;
			}
			float fX = fAimPos.x - fMyselfPos.x;
			float fY = fAimPos.z - fMyselfPos.z;
			if( ( fX * fX + fY * fY ) < m_nDistance ) // 这个点消失
			{
				std::list< MAP_POS_DEFINE >::iterator it2 = it ++;
				pList->erase( it2 );
			}
		}
	}
}
void CActivePosManager::GetFlashList( int nSceneID, POS_LIST* pReturnList )
{
	pReturnList->clear();
	std::list< MAP_POS_DEFINE >::iterator  it;
	for( it = m_listFlashObj.begin(); it != m_listFlashObj.end(); it ++ )
	{
		MAP_POS_DEFINE data = (*it);
		if( data.dwSceneID == nSceneID )
			pReturnList->push_back( data );
	}
}
void CActivePosManager::GetActiveList( int nSceneID, POS_LIST* pReturnList )
{
	pReturnList->clear();
	std::list< MAP_POS_DEFINE >::iterator  it;
	for( it = m_listActiveObj.begin(); it != m_listActiveObj.end(); it ++ )
	{
		MAP_POS_DEFINE data = (*it);
		if( data.dwSceneID == nSceneID )
			pReturnList->push_back( data );
	}
}