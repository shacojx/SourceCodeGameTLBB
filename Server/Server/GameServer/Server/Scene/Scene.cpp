#include "stdafx.h"

#include "Scene.h"
#include "Log.h"
#include "Config.h"
#include "PetPool.h"
#include "ItemBoxPool.h"
#include "PlatformPool.h"
#include "GCLeaveScene.h"
#include "Obj_ItemBox.h"
#include "MonsterManager.h"
#include "PetManager.h"
#include "MonsterPool.h"
#include "PlayerPool.h"
#include "SpecialPool.h"
#include "RecyclePlayerManager.h"
#include "GWMail.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"
#include "TimeManager.h"
#include "ItemBoxManager.h"
#include "FileDef.h"
#include "SceneManager.h"




BOOL Scene::IsCanEnter( )
{
__ENTER_FUNCTION

	INT iCount = m_pScenePlayerManager->GetCount( ) ;
	if( iCount > FD_SETSIZE)
	{
		return FALSE ;
	}
	if( GetSceneStatus()!=SCENE_STATUS_RUNNING )
	{//不因该尝试进入未开启的场景
		Assert(FALSE) ;
		return FALSE ;
	}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Scene::IsFull( )
{
__ENTER_FUNCTION

	INT iCount = m_pScenePlayerManager->GetCount( ) ;
	if( iCount >= FD_SETSIZE )
	{
		return TRUE ;
	}

	return FALSE ;

__LEAVE_FUNCTION

	return TRUE ;
}

//判断用户有无进入场景的权限, 0:不能进，1:可以进入，-1:启动备份场景
INT Scene::CheckEnter( PlayerID_t PlayerID )
{
__ENTER_FUNCTION

	if( GetSceneType()==SCENE_TYPE_COPY )//如果此场景是副本场景
	{
		GamePlayer* pGamePlayer = g_pPlayerPool->GetPlayer(PlayerID) ;
		INT nSceneStatus = GetSceneStatus() ;
		TeamID_t nHumanTeamID = pGamePlayer->GetHuman()->GetDB()->GetDBTeamID() ;

		if( nSceneStatus!=SCENE_STATUS_RUNNING )
		{//或者副本场景未被分配使用
			g_pLog->FastSaveLog( LOG_FILE_1, "CheckEnter No Scene S=%d HT=%d ST=%d HG=%X SG=%X", 
				nSceneStatus,nHumanTeamID,m_CopyData.m_TeamID,pGamePlayer->m_HumanGUID,m_CopyData.m_TeamLeader ) ;
			return -1 ;
		}

		if( m_CopyData.IsHaveMember( pGamePlayer->m_HumanGUID) )
		{//能够进入副本
			return 1 ;
		}

		g_pLog->FastSaveLog( LOG_FILE_1, "CheckEnter Not Valid S=%d HT=%d ST=%d HG=%X SG=%X", 
			nSceneStatus,nHumanTeamID,m_CopyData.m_TeamID,pGamePlayer->m_HumanGUID,m_CopyData.m_TeamLeader ) ;
		return -1 ;
		//if(	nHumanTeamID!=m_CopyData.m_TeamID )
		//{//当前副本场景已经不是此用户的所进入过的场景了
		//	g_pLog->FastSaveLog( LOG_FILE_1, "CheckEnter S=%d HT=%d ST=%d HG=%X SG=%X", 
		//		nSceneStatus,nHumanTeamID,m_CopyData.m_TeamID,pGamePlayer->m_HumanGUID,m_CopyData.m_TeamLeader ) ;
		//	return -1 ;
		//}
	}

	if( GetSceneType()==SCENE_TYPE_CIT )//如果此场景是副本场景
	{
		if( m_SceneInitData.m_CityData.m_Guid.isNull()
			|| GetSceneStatus()!=SCENE_STATUS_RUNNING
			)
		{//场景还未被分配
			return -1;

		}
	}

	return 1 ;

__LEAVE_FUNCTION

	return 0 ;
}

BOOL Scene::ObjectEnterScene( Obj *pObj )
{
__ENTER_FUNCTION

	if ( pObj == NULL )
	{
		Assert( pObj != NULL && "Scene::ObjectEnterScene" );
		return FALSE;
	}

	ENUM_OBJ_ID_TYPE eIDType;
	if ( pObj->GetObjType() == Obj::OBJ_TYPE_HUMAN )
	{
		eIDType = OBJ_ID_TYPE_HUMAN;
	}
	else
	{
		eIDType = OBJ_ID_TYPE_NORMAL;
	}

	BOOL bResult;
	bResult = m_pObjManager->AddObj( pObj, eIDType );
	if ( !bResult )
	{
		Assert( FALSE && "Scene::ObjectEnterScene m_pObjManager->AddObj failed" );
		return FALSE;
	}

	BOOL bSingleManagerAddSucceeded = FALSE;
	Obj::ObjType eType = pObj->GetObjType();
	switch ( eType )
	{
	case Obj::OBJ_TYPE_HUMAN:
		bSingleManagerAddSucceeded = m_pHumanManager->Add( pObj );
		break;
	case Obj::OBJ_TYPE_MONSTER:
		bSingleManagerAddSucceeded = m_pMonsterManager->Add( pObj );
		break;
	case Obj::OBJ_TYPE_PET:
		bSingleManagerAddSucceeded = m_pPetManager->Add( pObj );
		break;
	case Obj::OBJ_TYPE_ITEM_BOX:
		bSingleManagerAddSucceeded = m_pItemBoxManager->Add( (Obj_ItemBox*)pObj);
		break;
	case Obj::OBJ_TYPE_PLATFORM:
		bSingleManagerAddSucceeded = m_pPlatformManager->Add( pObj );
		break;
	case Obj::OBJ_TYPE_SPECIAL:
		bSingleManagerAddSucceeded = m_pSpecialManager->Add( pObj );
		break;
	default:
		Assert( FALSE && "Scene::ObjectEnterScene Unknow obj type" );
		bSingleManagerAddSucceeded = FALSE;
		break;
	}

	if ( !bSingleManagerAddSucceeded )
	{
		Assert( FALSE && "Scene::ObjectEnterScene add obj to single manager failed" );
		m_pObjManager->RemoveObj( pObj->GetID() );
		return FALSE;
	}

	pObj->setScene( this );
	pObj->OnEnterScene();

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

VOID Scene::ObjectLeaveScene( Obj *pObj )
{
__ENTER_FUNCTION

	if ( pObj == NULL )
	{
		Assert( pObj != NULL && "Scene::ObjectLeaveScene" );
		return ;
	}

	//清空角色的场景指针信息
	pObj->OnLeaveScene();
	pObj->setScene( NULL );

	Obj::ObjType eObjType = pObj->GetObjType();
	switch ( eObjType )
	{
	case Obj::OBJ_TYPE_HUMAN:
		{
			//广播玩家离开消息
			GCLeaveScene Msg ;
			Msg.setLeaveCode(0) ;
			Msg.setObjID( pObj->GetID() ) ;

			ZoneID_t zid = CalcZoneID( pObj->getWorldPos() ) ;

			BroadCast( &Msg, zid ) ;

			m_pHumanManager->Remove( pObj );
		}
		break;
	case Obj::OBJ_TYPE_MONSTER:
		m_pMonsterManager->Remove( pObj );
		break;
	case Obj::OBJ_TYPE_PET:
		m_pPetManager->Remove( pObj );
		break;
	case Obj::OBJ_TYPE_ITEM_BOX:
		m_pItemBoxManager->Remove( pObj->GetID() );
		break;
	case Obj::OBJ_TYPE_PLATFORM:
		m_pPlatformManager->Remove( pObj );
		break;
	case Obj::OBJ_TYPE_SPECIAL:
		m_pSpecialManager->Remove( pObj );
		break;
	default:
		Assert( FALSE && "Scene::ObjectLeaveScene Unknow obj type" );
		break;
	}

	//注销怪物Obj的ID信息，此操作后OBJ的ID会被设置成INVALID_ID
	m_pObjManager->RemoveObj( pObj->GetID() );

__LEAVE_FUNCTION
}

VOID Scene::GetRectInRadius( VRECT* pRect, INT nRadius, ZoneID_t zid )
{
__ENTER_FUNCTION

	Assert( zid!=INVALID_ID ) ;
	INT nW = zid%m_ZoneInfo.m_wZoneW;
	INT nH = zid/m_ZoneInfo.m_wZoneW;

	0>nRadius?nRadius=0:NULL;
	
	pRect->nStartx = nW-nRadius;
	if(pRect->nStartx < 0)
		pRect->nStartx = 0;

	pRect->nStartz = nH-nRadius;
	if(pRect->nStartz < 0)
		pRect->nStartz = 0;

	pRect->nEndx = nW+nRadius;
	if(pRect->nEndx >= m_ZoneInfo.m_wZoneW)
		pRect->nEndx = m_ZoneInfo.m_wZoneW-1;

	pRect->nEndz = nH+nRadius;
	if(pRect->nEndz >= m_ZoneInfo.m_wZoneH)
		pRect->nEndz = m_ZoneInfo.m_wZoneH-1;	

	/*  这个算法有问题？
	nRadius -= 1;

	pRect->nStartx = nW-nRadius;
	if(pRect->nStartx < 0)
		pRect->nStartx = 0;

	pRect->nStartz = nH-nRadius;
	if(pRect->nStartz < 0)
		pRect->nStartz = 0;

	pRect->nEndx = nW+nRadius + 1;
	if(pRect->nEndx > m_ZoneInfo.m_wZoneW)
		pRect->nEndx = m_ZoneInfo.m_wZoneW-1;

	pRect->nEndz = nH+nRadius + 1;
	if(pRect->nEndz > m_ZoneInfo.m_wZoneH)
		pRect->nEndz = m_ZoneInfo.m_wZoneH-1;	
	*/
__LEAVE_FUNCTION
}

ZoneID_t Scene::CalcZoneID( const WORLD_POS* pos )
{
__ENTER_FUNCTION

	if ( pos == NULL || pos->m_fX < 0.f || pos->m_fZ < 0.f )
	{
		Assert( (pos != NULL && pos->m_fX >= 0.f && pos->m_fZ >= 0.f) && "Scene::CalcZoneID" );
		return INVALID_ID;
	}

	UINT uX = (UINT)(pos->m_fX/g_Config.m_ConfigInfo.m_ZoneSize) ;
	UINT uZ = (UINT)(pos->m_fZ/g_Config.m_ConfigInfo.m_ZoneSize) ;

	return (ZoneID_t)(uX+uZ*GetZoneInfo()->m_wZoneW) ;

__LEAVE_FUNCTION

	return INVALID_ID ;
}

BOOL Scene::ObjZone_Register( Obj *pObj, ZoneID_t idZone )
{
__ENTER_FUNCTION

	if( pObj == NULL )
	{
		Assert( pObj == NULL && "Scene::ObjZone_Register" );
		return FALSE;
	}

	if( idZone < 0 || idZone >= m_ZoneInfo.m_wZoneSize )
	{
		Assert( idZone >= 0 && idZone < m_ZoneInfo.m_wZoneSize && "Scene::ObjZone_Register" );
		return FALSE;
	}

	// 刷新Obj消息到客户端
	Packet *pPacket = pObj->CreateNewObjPacket();
	if ( pPacket != NULL )
	{
		HUMANLIST listHuman;
		ScanHuman( idZone, MAX_REFESH_OBJ_ZONE_RADIUS, &(listHuman) );

		// 可见性过滤
		BYTE yNewCount, i;
		yNewCount = 0;
		for ( i = 0; i < listHuman.m_Count; i++ )
		{
			if ( pObj->IsCanViewMe( listHuman.m_aHuman[i] ) )
			{
				listHuman.m_aHuman[yNewCount++] = listHuman.m_aHuman[i];
			}
		}
		listHuman.m_Count = yNewCount;

		if ( listHuman.m_Count > 0 )
		{
			SendPacket( pPacket, &listHuman );

			//SendPacket_CrateAttr( pObj, &listHuman );
		}

		pObj->DestroyNewObjPacket( pPacket );
	}
	else
	{
		Assert( pPacket == NULL && "Scene::ObjZone_Register CreateNewObjPacket return NULL" );
		return FALSE;
	}

	// 新进入区域，把新进入的区域中的所有角色发往新进来的客户端
	if ( pObj->GetObjType() == Obj::OBJ_TYPE_HUMAN  )
	{
		OBJLIST listObj;
		ScanObj( idZone, MAX_REFESH_OBJ_ZONE_RADIUS, &(listObj) );

		Obj_Human *pHuman = ((Obj_Human*)(pObj));
		Obj *pFindObj;
		BYTE i;
		for ( i = 0; i < listObj.m_Count; i++ )
		{
			pFindObj = listObj.m_aObj[i];
			if ( pFindObj->IsCanViewMe( pHuman ) )
			{
				Packet *pPacket = pFindObj->CreateNewObjPacket();
				if ( pPacket != NULL )
				{
					pHuman->GetPlayer()->SendPacket( pPacket );

					pFindObj->DestroyNewObjPacket( pPacket );
				}
			}
		}
	}

	m_pZone[idZone].OnObjectEnter( pObj );
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL Scene::ObjZone_Unregister( Obj *pObj, ZoneID_t idZone )
{
__ENTER_FUNCTION

	if( pObj == NULL )
	{
		Assert( pObj == NULL && "Scene::ObjZone_Unregister" );
		return FALSE;
	}

	if( idZone < 0 || idZone >= m_ZoneInfo.m_wZoneSize )
	{
		Assert( idZone >= 0 && idZone < m_ZoneInfo.m_wZoneSize && "Scene::ObjZone_Unregister" );
		return FALSE;
	}

	m_pZone[idZone].OnObjectLeave( pObj );

	// 刷新Obj消息到客户端
	Packet *pPacket = pObj->CreateDeleteObjPacket();
	if ( pPacket != NULL )
	{
		HUMANLIST listHuman;
		ScanHuman( idZone, MAX_REFESH_OBJ_ZONE_RADIUS, &(listHuman) );

		// 可见性过滤
		BYTE yNewCount, i;
		yNewCount = 0;
		for ( i = 0; i < listHuman.m_Count; i++ )
		{
			if ( pObj->IsCanViewMe( listHuman.m_aHuman[i] ) )
			{
				listHuman.m_aHuman[yNewCount++] = listHuman.m_aHuman[i];
			}
		}
		listHuman.m_Count = yNewCount;

		if ( listHuman.m_Count > 0 )
		{
			SendPacket( pPacket, &listHuman );
		}

		pObj->DestroyDeleteObjPacket( pPacket );
	}
	else
	{
		Assert( pPacket == NULL && "Scene::ObjZone_Unregister CreateDeleteObjPacket return NULL" );
		return FALSE;
	}

	// 把原来区域中的所有角色的删除消息发往的客户端
	if ( pObj->GetObjType() == Obj::OBJ_TYPE_HUMAN  )
	{
		OBJLIST listObj;
		ScanObj( idZone, MAX_REFESH_OBJ_ZONE_RADIUS, &(listObj) );

		Obj_Human *pHuman = ((Obj_Human*)(pObj));
		Obj *pFindObj;
		BYTE i;
		for ( i = 0; i < listObj.m_Count; i++ )
		{
			pFindObj = listObj.m_aObj[i];
			if ( pFindObj->IsCanViewMe( pHuman ) )
			{
				Packet *pPacket = pFindObj->CreateDeleteObjPacket();
				if ( pPacket != NULL )
				{
					pHuman->GetPlayer()->SendPacket( pPacket );

					pFindObj->DestroyNewObjPacket( pPacket );
				}
			}
		}
	}
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL Scene::ObjZone_Changed( Obj *pObj, ZoneID_t idNew, ZoneID_t idOld )
{
__ENTER_FUNCTION
	if( pObj == NULL )
	{
		Assert( pObj == NULL && "Scene::ObjZone_Changed" );
		return FALSE;
	}

	if( (idNew < 0 || idNew >= m_ZoneInfo.m_wZoneSize) && (idOld < 0 || idOld >= m_ZoneInfo.m_wZoneSize) )
	{
		Assert( (idNew < 0 || idNew >= m_ZoneInfo.m_wZoneSize) && (idOld < 0 || idOld >= m_ZoneInfo.m_wZoneSize) && "Scene::ObjZone_Changed" );
		return FALSE;
	}

	if( idOld < 0 || idOld >= m_ZoneInfo.m_wZoneSize )
	{
		Assert( idOld >= 0 && idOld < m_ZoneInfo.m_wZoneSize && "Scene::ObjZone_Changed" );
	}
	else
	{
		m_pZone[idOld].OnObjectLeave( pObj );

		// 刷新Obj删除消息到客户端
		Packet *pPacket = pObj->CreateDeleteObjPacket();
		if ( pPacket != NULL )
		{
			HUMANLIST listHuman;
			if ( idNew != INVALID_ID )
			{
				ScanHuman_Sub( idOld, idNew, MAX_REFESH_OBJ_ZONE_RADIUS, &(listHuman) );
			}
			else
			{
				ScanHuman( idOld, MAX_REFESH_OBJ_ZONE_RADIUS, &(listHuman) );
			}

			// 可见性过滤
			BYTE yNewCount, i;
			yNewCount = 0;
			for ( i = 0; i < listHuman.m_Count; i++ )
			{
				if ( pObj->IsCanViewMe( listHuman.m_aHuman[i] ) )
				{
					listHuman.m_aHuman[yNewCount++] = listHuman.m_aHuman[i];
				}
			}
			listHuman.m_Count = yNewCount;

			if ( listHuman.m_Count > 0 )
			{
				SendPacket( pPacket, &listHuman );
			}

			pObj->DestroyDeleteObjPacket( pPacket );
		}
		else
		{
			Assert( pPacket == NULL && "Scene::ObjZone_Changed 刷新Obj删除消息到客户端 CreateNewObjPacket return NULL" );
			return FALSE;
		}

		// 如果是玩家,把超出视口的角色删除消息发到客户端
		if ( pObj->GetObjType() == Obj::OBJ_TYPE_HUMAN )
		{
			OBJLIST listObj;
			if ( idNew != INVALID_ID )
			{
				ScanObj_Sub( idOld, idNew, MAX_REFESH_OBJ_ZONE_RADIUS, &(listObj) );
			}
			else
			{
				ScanObj( idOld, MAX_REFESH_OBJ_ZONE_RADIUS, &(listObj) );
			}

			Obj_Human *pHuman = ((Obj_Human*)(pObj));
			Obj *pFindObj;
			BYTE i;
			for ( i = 0; i < listObj.m_Count; i++ )
			{
				pFindObj = listObj.m_aObj[i];
				if ( pFindObj->IsCanViewMe( pHuman ) )
				{
					Packet *pPacket = pFindObj->CreateDeleteObjPacket();
					if ( pPacket != NULL )
					{
						pHuman->GetPlayer()->SendPacket( pPacket );

						pFindObj->DestroyDeleteObjPacket( pPacket );
					}
					else
					{
						Assert( pPacket == NULL && "Scene::ObjZone_Changed 把超出视口的角色删除消息发到客户端 CreateDeleteObjPacket return NULL" );
						return FALSE;
					}
				}
			}
		}
	}

	if( idNew < 0 || idNew >= m_ZoneInfo.m_wZoneSize )
	{
		Assert( idOld >= 0 && idOld < m_ZoneInfo.m_wZoneSize && "Scene::ObjZone_Changed" );
	}
	else
	{
		// 刷新Obj创建消息到客户端
		Packet *pPacket = pObj->CreateNewObjPacket();
		if ( pPacket != NULL )
		{
			HUMANLIST listHuman;
			if ( idOld != INVALID_ID )
			{
				ScanHuman_Sub( idNew, idOld, MAX_REFESH_OBJ_ZONE_RADIUS, &(listHuman) );
			}
			else
			{
				ScanHuman( idNew, MAX_REFESH_OBJ_ZONE_RADIUS, &(listHuman) );
			}

			// 可见性过滤
			BYTE yNewCount, i;
			yNewCount = 0;
			for ( i = 0; i < listHuman.m_Count; i++ )
			{
				if ( pObj->IsCanViewMe( listHuman.m_aHuman[i] ) )
				{
					listHuman.m_aHuman[yNewCount++] = listHuman.m_aHuman[i];
				}
			}
			listHuman.m_Count = yNewCount;

			if ( listHuman.m_Count > 0 )
			{
				SendPacket( pPacket, &listHuman );
				// 当发现玩家时才开始泡泡说话
				if (pObj->GetObjType() == Obj::OBJ_TYPE_MONSTER)
				{
					((Obj_Monster*)pObj)->ProcessPaoPao();
				}
			}

			pObj->DestroyNewObjPacket( pPacket );
		}
		else
		{
			Assert( pPacket == NULL && "Scene::ObjZone_Changed 刷新Obj创建消息到客户端 CreateNewObjPacket return NULL" );
			return FALSE;
		}

		// 如果是玩家
		if ( pObj->GetObjType() == Obj::OBJ_TYPE_HUMAN )
		{
			// 新进入区域，把新进入的区域中的所有角色发往新进来的客户端
			OBJLIST listObj;
			if ( idOld != INVALID_ID )
			{
				ScanObj_Sub( idNew, idOld, MAX_REFESH_OBJ_ZONE_RADIUS, &(listObj) );
			}
			else
			{
				ScanObj( idNew, MAX_REFESH_OBJ_ZONE_RADIUS, &(listObj) );
			}

			Obj_Human *pHuman = ((Obj_Human*)(pObj));
			Obj *pFindObj;
			BYTE i;
			for ( i = 0; i < listObj.m_Count; i++ )
			{
				pFindObj = listObj.m_aObj[i];
				if ( pFindObj->IsCanViewMe( pHuman ) )
				{
					Packet *pPacket = pFindObj->CreateNewObjPacket();
					if ( pPacket != NULL )
					{
						pHuman->GetPlayer()->SendPacket( pPacket );

						pFindObj->DestroyNewObjPacket( pPacket );
					}
					else
					{
						Assert( pPacket == NULL && "Scene::ObjZone_Changed 把新进入的区域中的所有角色发往新进来的客户端 CreateDeleteObjPacket return NULL" );
						return FALSE;
					}
				}
			}
		}
		m_pZone[idNew].OnObjectEnter( pObj );
	}
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL Scene::RegisterArea( const Area* pArea)
{
__ENTER_FUNCTION

	Assert(pArea);
	FLOAT ox, oz, maxX, maxZ;

	ox = pArea->m_rcArea.m_fLeft; // Area 左上角点 x 值
	oz = pArea->m_rcArea.m_fTop; // Area 左上角点 z 值
	maxX = pArea->m_rcArea.m_fRight; // 右边的 x 值
	maxZ = pArea->m_rcArea.m_fBottom; // 下边的 z 值

	Assert( m_pMap->CX() >= maxX && "Oops... area 跑到地图外面去鸟~~" );
	Assert( m_pMap->CZ() >= maxZ && "Oops... area 跑到地图外面去鸟~~" );
	INT ZoneSize = g_Config.m_ConfigInfo.m_ZoneSize;
	UINT uX = (UINT)(ox / ZoneSize);
	UINT uZ = (UINT)(oz / ZoneSize);

	FLOAT xPos = (FLOAT)(uX * ZoneSize);
	FLOAT zPos = (FLOAT)(uZ * ZoneSize);
	WORD ZoneWidth = GetZoneInfo()->m_wZoneW;
	ZoneID_t zoneid;


	for( INT i = 0; zPos < maxZ; zPos += ZoneSize, ++i )
	{
		for( INT j = 0; xPos < maxX; xPos += ZoneSize, ++j )
		{
			zoneid = (ZoneID_t)((uX + j) + (uZ + i) * ZoneWidth);
			GetZone( zoneid )->AddArea( pArea );
		}
		xPos = (FLOAT)(uX * ZoneSize) ;
	}

__LEAVE_FUNCTION

	return TRUE;
}

const Area* Scene::CheckArea( Obj_Human* pHuman )
{
__ENTER_FUNCTION

	Assert(pHuman);

	Zone* pZone = GetZone(pHuman->getZoneID());
	Assert(pZone);

	if( pZone->GetAreaCount() < 1 )
	{
		return NULL;
	}

	return pZone->GetCurrentArea( pHuman->getWorldPos() );

__LEAVE_FUNCTION

	return NULL ;
}

ObjID_t Scene::CreateTempMonster(const _OBJ_MONSTER_INIT* pMonsterInit )
{
__ENTER_FUNCTION

	Assert(pMonsterInit);

	Obj_Monster* pMonster = (Obj_Monster*)NewObject( Obj::OBJ_TYPE_MONSTER );
	if ( pMonster == NULL )
		return INVALID_ID;


	pMonster->Init( pMonsterInit ) ;

	pMonster->SetActiveFlag( TRUE );

	return pMonster->GetID() ;
	//return MonsterEnterScene(pMonster,&Pos);
__LEAVE_FUNCTION
	
	return INVALID_ID ;
}

BOOL Scene::DeleteTempMonster(Obj_Monster* pMonster)
{
__ENTER_FUNCTION

	if( pMonster==NULL )
	{
		Assert(FALSE);
		return FALSE ;
	}

	pMonster->SetActiveFlag( FALSE );

__LEAVE_FUNCTION
	return TRUE;
}

Obj *Scene::NewObject( Obj::ObjType eType )
{
__ENTER_FUNCTION

	Obj *pObj;
	switch ( eType )
	{
	case Obj::OBJ_TYPE_HUMAN:
		pObj = NULL;
		break;
	case Obj::OBJ_TYPE_MONSTER:
		pObj = g_pMonsterPool->NewObj( );
		break;
	case Obj::OBJ_TYPE_PET:
		pObj = g_pPetPool->NewObj( );
		break;
	case Obj::OBJ_TYPE_ITEM_BOX:
		pObj = g_pItemBoxPool->NewItemBox( );
		break;
	case Obj::OBJ_TYPE_PLATFORM:
		pObj = g_pPlatformPool->NewObj( );
		break;
	case Obj::OBJ_TYPE_SPECIAL:
		pObj = g_pSpecialPool->NewObj( );
		break;
	default:
		Assert( FALSE && "Scene::NewObject invalid obj type" );
		pObj = NULL;
		break;
	}
	if ( pObj != NULL )
	{
		BOOL bResult = ObjectEnterScene( pObj );
		if ( !bResult )
		{
			DeleteObject( pObj );
			return NULL;
		}
	}
	return pObj;

__LEAVE_FUNCTION

	return NULL ;
}

VOID Scene::DeleteObject( Obj *pObj )
{
__ENTER_FUNCTION

	if ( pObj == NULL )
	{
		Assert( pObj == NULL && "Scene::DeleteObject" );
		return ;
	}

	ObjectLeaveScene( pObj );

	Obj::ObjType eType = pObj->GetObjType();
	switch ( eType )
	{
	case Obj::OBJ_TYPE_HUMAN:
		break;
	case Obj::OBJ_TYPE_MONSTER:
		g_pMonsterPool->DeleteObj( (Obj_Monster*)pObj );
		break;
	case Obj::OBJ_TYPE_PET:
		g_pPetPool->DeleteObj( (Obj_Pet*)pObj );
		break;
	case Obj::OBJ_TYPE_ITEM_BOX:
		g_pItemBoxPool->DeleteObj( (Obj_ItemBox*)pObj );
		break;
	case Obj::OBJ_TYPE_PLATFORM:
		g_pPlatformPool->DeleteObj( (Obj_Platform*)pObj );
		break;
	case Obj::OBJ_TYPE_SPECIAL:
		g_pSpecialPool->DeleteObj( (Obj_Special*)pObj );
		break;
	default:
		pObj = NULL;
		Assert( FALSE && "Scene::DeleteObject invalid obj type" );
		break;
	}

__LEAVE_FUNCTION
}

VOID Scene::BeginSceneTimer( UINT uTerm, UINT uNowTime )
{
	m_SceneTimer.BeginTimer( uTerm, uNowTime ) ;
}

VOID Scene::EndSceneTimer( )
{
	m_SceneTimer.CleanUp() ;
}

// 发送普通邮件
VOID Scene::SendNormalMail(const Obj_Human* pHuman, const CHAR* szReceiverName, const CHAR* szContent)
{
__ENTER_FUNCTION

	Assert( pHuman );
	Assert( szReceiverName );
	Assert( szContent );

	MAIL mail;

	mail.m_GUID = pHuman->GetGUID();
	mail.m_uFlag = MAIL_TYPE_NORMAL2;
	mail.m_SourSize = (BYTE)strlen( pHuman->GetName() );
	strncpy( mail.m_szSourName, pHuman->GetName(), MAX_CHARACTER_NAME-1 );
	mail.m_nPortrait = pHuman->GetPortraitID();
	mail.m_DestSize = (BYTE)strlen( szReceiverName );
	strncpy( mail.m_szDestName, szReceiverName, MAX_CHARACTER_NAME-1 );
	mail.m_ContexSize = (BYTE)strlen( szContent );
	strncpy( mail.m_szContex, szContent, MAX_MAIL_CONTEX-1 );

	mail.m_uCreateTime = g_pTimeManager->GetANSITime();
	mail.m_Camp = INVALID_CAMP;

	GWMail* pMsg = (GWMail*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_MAIL));
	pMsg->SetMail( &mail );

	g_pServerManager->SendPacket( pMsg, INVALID_ID );

__LEAVE_FUNCTION
}

// 发送系统邮件
VOID Scene::SendSystemMail(const CHAR* szReceiverName, const CHAR* szContent)
{
	__ENTER_FUNCTION

	Assert( szReceiverName );
	Assert( szContent );

	MAIL mail;

	mail.m_uFlag	= MAIL_TYPE_SYSTEM;
	strncpy( mail.m_szDestName, szReceiverName, sizeof(mail.m_szDestName));
	mail.m_szDestName[sizeof(mail.m_szDestName)-1] = '\0';
	mail.m_DestSize = (BYTE)strlen( mail.m_szDestName );
	strncpy( mail.m_szContex, szContent, sizeof(mail.m_szContex) );
	mail.m_szContex[sizeof(mail.m_szContex)-1] = '\0';
	mail.m_ContexSize = (BYTE)strlen( mail.m_szContex );

	mail.m_uCreateTime = g_pTimeManager->GetANSITime();
	mail.m_Camp = INVALID_CAMP;

	GWMail* pMsg = (GWMail*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_MAIL));
	pMsg->SetMail( &mail );

	g_pServerManager->SendPacket( pMsg, INVALID_ID );

	__LEAVE_FUNCTION
}

// 发送可执行邮件
VOID Scene::SendScriptMail(const CHAR* szReceiverName, UINT uParam0, UINT uParam1, UINT uParam2, UINT uParam3 )
{
__ENTER_FUNCTION

	Assert( szReceiverName );

	MAIL mail;

	mail.m_uFlag = MAIL_TYPE_SCRIPT;
	mail.m_DestSize = (BYTE)strlen( szReceiverName );
	strncpy( mail.m_szDestName, szReceiverName, MAX_CHARACTER_NAME-1 );
	mail.m_uParam0 = uParam0;
	mail.m_uParam1 = uParam1;
	mail.m_uParam2 = uParam2;
	mail.m_uParam3 = uParam3;
	mail.m_Camp = INVALID_CAMP;

	GWMail* pMsg = (GWMail*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_MAIL));
	pMsg->SetMail( &mail );

	g_pServerManager->SendPacket( pMsg, INVALID_ID );

__LEAVE_FUNCTION
}

BOOL Scene::ExecuteMonsterIni( const CHAR* szFileName )
{
__ENTER_FUNCTION

	CHAR szTemp[_MAX_PATH] ;
	memset( szTemp, 0, _MAX_PATH ) ;
	GET_SCENE_FULL_PATH( szTemp, szFileName ) ;

	return GetMonsterManager()->LoadMonster(szTemp) ;

__LEAVE_FUNCTION

	return FALSE ;
}
Obj* Scene::GetSpecificObjByID(ObjID_t nID)const
{
	__ENTER_FUNCTION
	if(NULL!=m_pObjManager)
	{
		return m_pObjManager->GetObj(nID);
	}
	return NULL;
	__LEAVE_FUNCTION
	return NULL;
}

//这个函数没完成
CHAR* Scene::FormatMissionString(const CHAR* strIn, CHAR* strOut, const Obj_Human* pHuman, const _MISSION_LIST_t* pMis, const _MISSION_DATA_t* pData, const _MISSION_REWARD_t* pReward, BOOL& bConvert)
{
	__ENTER_FUNCTION
	//return strIn;









	CHAR szTemp[2048] = {0};
	CHAR szNum[32];
	CHAR* pFind;
	INT nLen;
	bConvert = FALSE;

	if( strIn == NULL || pHuman == NULL ) return NULL;
	if( pData == NULL || pReward == NULL ) return NULL;

	nLen = (INT)strlen( strIn );
	if( nLen > 1024 ) return NULL;

	strcpy( szTemp, strIn );

	//format player name
	pFind = strstr( szTemp, "$ply_name" );
	if( pFind )
	{
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		strcat( strOut, pHuman->GetName() );
		strcat( strOut, pFind + 9 );
		strcpy( szTemp, strOut );
		bConvert = TRUE;
	}

	//format npc name
	pFind = strstr( szTemp, "$npc_name" );
	if( pFind )
	{
		sprintf( szNum, "[%d,%d]", pMis->nDestX, pMis->nDestZ );
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		strcat( strOut, g_Config.m_SceneInfo.m_pScene[pMis->nDestScene].m_szName );
		strcat( strOut, "的" );
		strcat( strOut, GetMonsterManager()->GetMonster(pMis->nSubmitorID)->GetName() );
		strcat( strOut, szNum );
		strcat( strOut, pFind + 9 );
		strcpy( szTemp, strOut );
		bConvert = TRUE;
	}

	//monster1
	pFind = strstr( szTemp, "$mon1_name" );
	if( pFind )
	{
		//sprintf( szNum, "[%d,%d]", pMis->nDestX, pMis->nDestZ );
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		strcat( strOut, g_Config.m_SceneInfo.m_pScene[pMis->nDestScene].m_szName );
		strcat( strOut, "的" );
		const MONSTER_EXT_ATTR* pExtAttr = g_MonsterAttrExTbl.GetExtAttr( pData->nMonster1ID );
		strcat( strOut, pExtAttr->m_Name );
		//strcat( strOut, szNum );
		strcat ( strOut, pFind + 10 );
		strcpy ( szTemp, strOut );
		bConvert = TRUE;
	}
	pFind = strstr( szTemp, "$mon1_num" );
	if( pFind )
	{
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		tsnprintf( szNum, 30, "%d", pData->nMonster1Num );
		strcat( strOut, szNum );
		strcat ( strOut, pFind + 9 );
		strcpy ( szTemp, strOut );
		bConvert = TRUE;
	}

	//monster2
	pFind = strstr( szTemp, "$mon2_name" );
	if( pFind )
	{
		//sprintf( szNum, "[%d,%d]", pMis->nDestX, pMis->nDestZ );
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		strcat( strOut, g_Config.m_SceneInfo.m_pScene[pMis->nDestScene].m_szName );
		strcat( strOut, "的" );
		const MONSTER_EXT_ATTR* pExtAttr = g_MonsterAttrExTbl.GetExtAttr( pData->nMonster2ID );
		strcat( strOut, pExtAttr->m_Name );
		//strcat( strOut, szNum );
		strcat ( strOut, pFind + 10 );
		strcpy ( szTemp, strOut );
		bConvert = TRUE;
	}
	pFind = strstr( szTemp, "$mon2_num" );
	if( pFind )
	{
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		tsnprintf( szNum, 30, "%d", pData->nMonster2Num );
		strcat( strOut, szNum );
		strcat ( strOut, pFind + 9 );
		strcpy ( szTemp, strOut );
		bConvert = TRUE;
	}

	//monster3
	pFind = strstr( szTemp, "$mon3_name" );
	if( pFind )
	{
		//sprintf( szNum, "[%d,%d]", pMis->nDestX, pMis->nDestZ );
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		strcat( strOut, g_Config.m_SceneInfo.m_pScene[pMis->nDestScene].m_szName );
		strcat( strOut, "的" );
		const MONSTER_EXT_ATTR* pExtAttr = g_MonsterAttrExTbl.GetExtAttr( pData->nMonster3ID );
		strcat( strOut, pExtAttr->m_Name );
		//strcat( strOut, szNum );
		strcat ( strOut, pFind + 10 );
		strcpy ( szTemp, strOut );
		bConvert = TRUE;
	}
	pFind = strstr( szTemp, "$mon3_num" );
	if( pFind )
	{
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		tsnprintf( szNum, 30, "%d", pData->nMonster3Num );
		strcat( strOut, szNum );
		strcat ( strOut, pFind + 9 );
		strcpy ( szTemp, strOut );
		bConvert = TRUE;
	}

	//monster4
	pFind = strstr( szTemp, "$mon4_name" );
	if( pFind )
	{
		//sprintf( szNum, "[%d,%d]", pMis->nDestX, pMis->nDestZ );
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		strcat( strOut, g_Config.m_SceneInfo.m_pScene[pMis->nDestScene].m_szName );
		strcat( strOut, "的" );
		const MONSTER_EXT_ATTR* pExtAttr = g_MonsterAttrExTbl.GetExtAttr( pData->nMonster4ID );
		strcat( strOut, pExtAttr->m_Name );
		//strcat( strOut, szNum );
		strcat ( strOut, pFind + 10 );
		strcpy ( szTemp, strOut );
		bConvert = TRUE;
	}
	pFind = strstr( szTemp, "$mon4_num" );
	if( pFind )
	{
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		tsnprintf( szNum, 30, "%d", pData->nMonster4Num );
		strcat( strOut, szNum );
		strcat ( strOut, pFind + 9 );
		strcpy ( szTemp, strOut );
		bConvert = TRUE;
	}

	//item1
	pFind = strstr( szTemp, "$itm1_name_req" );
	if( pFind )
	{
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		strcat ( strOut, "@itemid_" );
		tsnprintf( szNum, 30, "%d", pData->nItem1ID );
		strcat( strOut, szNum );
		strcat( strOut, pFind + 14 );
		strcpy( szTemp, strOut );
		bConvert = TRUE;
	}
	pFind = strstr( szTemp, "$itm1_num_req" );
	if( pFind )
	{
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		tsnprintf( szNum, 30, "%d", pData->nItem1Num );
		strcat( strOut, szNum );
		strcat( strOut, pFind + 13 );
		strcpy( szTemp, strOut );
		bConvert = TRUE;
	}

	//item2
	pFind = strstr( szTemp, "$itm2_name_req" );
	if( pFind )
	{
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		strcat ( strOut, "@itemid_" );
		tsnprintf( szNum, 30, "%d", pData->nItem2ID );
		strcat( strOut, szNum );
		strcat( strOut, pFind + 14 );
		strcpy( szTemp, strOut );
		bConvert = TRUE;
	}
	pFind = strstr( szTemp, "$itm2_num_req" );
	if( pFind )
	{
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		tsnprintf( szNum, 30, "%d", pData->nItem2Num );
		strcat( strOut, szNum );
		strcat( strOut, pFind + 13 );
		strcpy( szTemp, strOut );
		bConvert = TRUE;
	}

	//item3
	pFind = strstr( szTemp, "$itm3_name_req" );
	if( pFind )
	{
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		strcat ( strOut, "@itemid_" );
		tsnprintf( szNum, 30, "%d", pData->nItem3ID );
		strcat( strOut, szNum );
		strcat( strOut, pFind + 14 );
		strcpy( szTemp, strOut );
		bConvert = TRUE;
	}
	pFind = strstr( szTemp, "$itm3_num_req" );
	if( pFind )
	{
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		tsnprintf( szNum, 30, "%d", pData->nItem3Num );
		strcat( strOut, szNum );
		strcat( strOut, pFind + 13 );
		strcpy( szTemp, strOut );
		bConvert = TRUE;
	}

	//item4
	pFind = strstr( szTemp, "$itm4_name_req" );
	if( pFind )
	{
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		strcat ( strOut, "@itemid_" );
		tsnprintf( szNum, 30, "%d", pData->nItem4ID );
		strcat( strOut, szNum );
		strcat( strOut, pFind + 14 );
		strcpy( szTemp, strOut );
		bConvert = TRUE;
	}
	pFind = strstr( szTemp, "$itm4_num_req" );
	if( pFind )
	{
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		tsnprintf( szNum, 30, "%d", pData->nItem4Num );
		strcat( strOut, szNum );
		strcat( strOut, pFind + 13 );
		strcpy( szTemp, strOut );
		bConvert = TRUE;
	}

	//item5
	pFind = strstr( szTemp, "$itm5_name_req" );
	if( pFind )
	{
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		strcat ( strOut, "@itemid_" );
		tsnprintf( szNum, 30, "%d", pData->nItem5ID );
		strcat( strOut, szNum );
		strcat( strOut, pFind + 14 );
		strcpy( szTemp, strOut );
		bConvert = TRUE;
	}
	pFind = strstr( szTemp, "$itm5_num_req" );
	if( pFind )
	{
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		tsnprintf( szNum, 30, "%d", pData->nItem5Num );
		strcat( strOut, szNum );
		strcat( strOut, pFind + 13 );
		strcpy( szTemp, strOut );
		bConvert = TRUE;
	}

	//item6
	pFind = strstr( szTemp, "$itm6_name_req" );
	if( pFind )
	{
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		strcat ( strOut, "@itemid_" );
		tsnprintf( szNum, 30, "%d", pData->nItem6ID );
		strcat( strOut, szNum );
		strcat( strOut, pFind + 14 );
		strcpy( szTemp, strOut );
		bConvert = TRUE;
	}
	pFind = strstr( szTemp, "$itm6_num_req" );
	if( pFind )
	{
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		tsnprintf( szNum, 30, "%d", pData->nItem6Num );
		strcat( strOut, szNum );
		strcat( strOut, pFind + 13 );
		strcpy( szTemp, strOut );
		bConvert = TRUE;
	}

	//item7
	pFind = strstr( szTemp, "$itm7_name_req" );
	if( pFind )
	{
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		strcat ( strOut, "@itemid_" );
		tsnprintf( szNum, 30, "%d", pData->nItem7ID );
		strcat( strOut, szNum );
		strcat( strOut, pFind + 14 );
		strcpy( szTemp, strOut );
		bConvert = TRUE;
	}
	pFind = strstr( szTemp, "$itm7_num_req" );
	if( pFind )
	{
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		tsnprintf( szNum, 30, "%d", pData->nItem7Num );
		strcat( strOut, szNum );
		strcat( strOut, pFind + 13 );
		strcpy( szTemp, strOut );
		bConvert = TRUE;
	}


	//item1
	pFind = strstr( szTemp, "$itm1_name_rew" );
	if( pFind )
	{
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		strcat ( strOut, "@itemid_" );
		tsnprintf( szNum, 30, "%d", pData->nItem1ID );
		strcat( strOut, szNum );
		strcat( strOut, pFind + 14 );
		strcpy( szTemp, strOut );
		bConvert = TRUE;
	}
	pFind = strstr( szTemp, "$itm1_num_rew" );
	if( pFind )
	{
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		tsnprintf( szNum, 30, "%d", pData->nItem1Num );
		strcat( strOut, szNum );
		strcat( strOut, pFind + 13 );
		strcpy( szTemp, strOut );
		bConvert = TRUE;
	}

	//item2
	pFind = strstr( szTemp, "$itm2_name_rew" );
	if( pFind )
	{
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		strcat ( strOut, "@itemid_" );
		tsnprintf( szNum, 30, "%d", pData->nItem2ID );
		strcat( strOut, szNum );
		strcat( strOut, pFind + 14 );
		strcpy( szTemp, strOut );
		bConvert = TRUE;
	}
	pFind = strstr( szTemp, "$itm2_num_rew" );
	if( pFind )
	{
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		tsnprintf( szNum, 30, "%d", pData->nItem2Num );
		strcat( strOut, szNum );
		strcat( strOut, pFind + 13 );
		strcpy( szTemp, strOut );
		bConvert = TRUE;
	}

	//item3
	pFind = strstr( szTemp, "$itm3_name_rew" );
	if( pFind )
	{
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		strcat ( strOut, "@itemid_" );
		tsnprintf( szNum, 30, "%d", pData->nItem3ID );
		strcat( strOut, szNum );
		strcat( strOut, pFind + 14 );
		strcpy( szTemp, strOut );
		bConvert = TRUE;
	}
	pFind = strstr( szTemp, "$itm3_num_rew" );
	if( pFind )
	{
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		tsnprintf( szNum, 30, "%d", pData->nItem3Num );
		strcat( strOut, szNum );
		strcat( strOut, pFind + 13 );
		strcpy( szTemp, strOut );
		bConvert = TRUE;
	}

	//item4
	pFind = strstr( szTemp, "$itm4_name_rew" );
	if( pFind )
	{
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		strcat ( strOut, "@itemid_" );
		tsnprintf( szNum, 30, "%d", pData->nItem4ID );
		strcat( strOut, szNum );
		strcat( strOut, pFind + 14 );
		strcpy( szTemp, strOut );
		bConvert = TRUE;
	}
	pFind = strstr( szTemp, "$itm4_num_rew" );
	if( pFind )
	{
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		tsnprintf( szNum, 30, "%d", pData->nItem4Num );
		strcat( strOut, szNum );
		strcat( strOut, pFind + 13 );
		strcpy( szTemp, strOut );
		bConvert = TRUE;
	}

	//item5
	pFind = strstr( szTemp, "$itm5_name_rew" );
	if( pFind )
	{
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		strcat ( strOut, "@itemid_" );
		tsnprintf( szNum, 30, "%d", pData->nItem5ID );
		strcat( strOut, szNum );
		strcat( strOut, pFind + 14 );
		strcpy( szTemp, strOut );
		bConvert = TRUE;
	}
	pFind = strstr( szTemp, "$itm5_num_rew" );
	if( pFind )
	{
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		tsnprintf( szNum, 30, "%d", pData->nItem5Num );
		strcat( strOut, szNum );
		strcat( strOut, pFind + 13 );
		strcpy( szTemp, strOut );
		bConvert = TRUE;
	}

	//item6
	pFind = strstr( szTemp, "$itm6_name_rew" );
	if( pFind )
	{
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		strcat ( strOut, "@itemid_" );
		tsnprintf( szNum, 30, "%d", pData->nItem6ID );
		strcat( strOut, szNum );
		strcat( strOut, pFind + 14 );
		strcpy( szTemp, strOut );
		bConvert = TRUE;
	}
	pFind = strstr( szTemp, "$itm6_num_rew" );
	if( pFind )
	{
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		tsnprintf( szNum, 30, "%d", pData->nItem6Num );
		strcat( strOut, szNum );
		strcat( strOut, pFind + 13 );
		strcpy( szTemp, strOut );
		bConvert = TRUE;
	}
	pFind = strstr( szTemp, "$next_npc" );
	if( pFind )
	{
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		MissionID_t RealMissionID = ( pData->nMissionClass << 5 ) + pData->nMissionSubClass;
		if ( pHuman->IsHaveMission(RealMissionID) )
		{
			UINT uMisIndex = pHuman->GetMissionIndexByID( RealMissionID );
			Assert(UINT_MAX != uMisIndex);
			UINT uSceneID = pHuman->GetMissionParam(uMisIndex, 7)/10000;
			Scene *pScene = g_pSceneManager->GetScene( uSceneID );
			CHAR *szNextSceneName = g_pSceneManager->GetSceneInfo( uSceneID )->m_szName;
			UINT uNpcID = pHuman->GetMissionParam(uMisIndex, 7)%10000;
			const CHAR *szNextNpcName = pScene->GetMonsterManager()->GetMonster(uNpcID)->GetName();
			memcpy( strOut, szTemp, pFind - szTemp );
			strOut[pFind - szTemp] = 0;
			strcat( strOut, "#G" );
			strcat( strOut, szNextSceneName );
			strcat( strOut, "的#Y" );
			strcat( strOut, szNextNpcName );
			strcat( strOut, "#W" );
			strcat( strOut, pFind + 9 );
			strcpy( szTemp, strOut );
		}
		else
		{
			strcat ( strOut, "#R********" );
			strcat( strOut, pFind + 9 );
			strcpy( szTemp, strOut );
		}
		bConvert = TRUE;
	}
	pFind = strstr( szTemp, "$run_mis" );
	if( pFind )
	{
		memcpy( strOut, szTemp, pFind - szTemp );
		strOut[pFind - szTemp] = 0;
		MissionID_t RealMissionID = ( pData->nMissionClass << 5 ) + pData->nMissionSubClass;
		if ( pHuman->IsHaveMission(RealMissionID) )
		{
			UINT uMisIndex = pHuman->GetMissionIndexByID( RealMissionID );
			Assert(UINT_MAX != uMisIndex);
			strcat( strOut, "#Y当前第#R" );
			UINT uNonceTach = pHuman->GetMissionParam(uMisIndex, 4);
			tsnprintf( szNum, 30, "%d", uNonceTach );
			strcat( strOut, szNum );
			strcat( strOut, "#Y/" );
			UINT uMaxTach = pHuman->GetMissionParam(uMisIndex, 3);
			tsnprintf( szNum, 30, "%d", uMaxTach );
			strcat( strOut, szNum );
			strcat( strOut, "环#W" );
			strcat( strOut, pFind + 8 );
			strcpy( szTemp, strOut );
		}
		else
		{
			strcat ( strOut, "#R********" );
			strcat( strOut, pFind + 8 );
			strcpy( szTemp, strOut );
		}
		bConvert = TRUE;
	}

	strcpy( strOut, szTemp );

	return strOut;

	__LEAVE_FUNCTION
	return NULL;
}



























