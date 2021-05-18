#include "stdafx.h"

#include "Scene.h"
#include "PlayerPool.h"
#include "Obj_Human.h"
#include "Obj_ItemBox.h"
#include "Obj_Monster.h"
#include "ChatPipe.h"
#include "SkillLogic.h"
#include "ScanOperator.h"



using namespace Combat_Module::Skill_Module;

BOOL Scene::BroadCast_Scene( Packet* pPacket )
{
__ENTER_FUNCTION

		if (m_pScenePlayerManager)
		{
			uint nPlayerCount = m_pScenePlayerManager->GetPlayerNumber() ;
			for( uint i=0; i<nPlayerCount; i++ )
			{
				PlayerID_t* PlayerIDs = m_pScenePlayerManager->GetPlayers() ;
				if( PlayerIDs[i] == INVALID_ID )
					continue ;

				Player* pPlayer = g_pPlayerPool->GetPlayer(PlayerIDs[i]) ;
				if( pPlayer )
					pPlayer->SendPacket( pPacket ) ;
			}
		}

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Scene::BroadCast(	Packet* pPacket, Obj_Character* pOwnCharacter, BOOL bSendMe )
{
__ENTER_FUNCTION

	if( pOwnCharacter==NULL )
		return FALSE ;

	if ( pOwnCharacter->getZoneID() == INVALID_ID )
	{
		Assert( pOwnCharacter->getZoneID() != INVALID_ID && "Scene::BroadCast(	Packet* pPacket, Obj_Character* pOwnCharacter, BOOL bSendMe )" );
		return FALSE;
	}

	VRECT rc ;
	GetRectInRadius( &rc, MAX_RADIUS, pOwnCharacter->getZoneID() ) ;

	for(INT h = rc.nStartz; h <= rc.nEndz; h++)
	{
		for(INT w = rc.nStartx; w <= rc.nEndx; w++)
		{
			ZoneID_t zid = w+h*m_ZoneInfo.m_wZoneW ;
			ObjList* pList = m_pZone[zid].GetHumanList();//&(m_pZone[zid].m_HumanList) ;

			_OBJ_LIST_NODE* pPoint = pList->m_Head.m_pNext;
			while(pPoint != &pList->m_Tail)
			{
				Obj* pObj = pPoint->m_pNode;
				pPoint = pPoint->m_pNext ;

				if( pObj==NULL )
				{
					Assert( FALSE ) ;
					return FALSE ;
				}

				Obj_Human* pHuman = (Obj_Human*)pObj ;
				if( (!bSendMe) && (pHuman->GetID()==pOwnCharacter->GetID()) )
				{
					continue ;
				}

				pHuman->GetPlayer()->SendPacket( pPacket ) ;
			};
		}
	}


	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Scene::BroadCast(	Packet* pPacket, ZoneID_t  zoneId)
{
	__ENTER_FUNCTION

	if ( zoneId == INVALID_ID )
	{
		Assert( zoneId != INVALID_ID && "Scene::BroadCast(	Packet* pPacket, ZoneID_t  zoneId)" );
		return FALSE;
	}
	

	VRECT rc ;
	GetRectInRadius( &rc, MAX_RADIUS, zoneId ) ;

	for(INT h = rc.nStartz; h <= rc.nEndz; h++)
	{
		for(INT w = rc.nStartx; w <= rc.nEndx; w++)
		{
			ZoneID_t zid = w+h*m_ZoneInfo.m_wZoneW ;
			ObjList* pList = m_pZone[zid].GetHumanList();//&(m_pZone[zid].m_HumanList) ;

			_OBJ_LIST_NODE* pPoint = pList->m_Head.m_pNext;
			while(pPoint != &pList->m_Tail)
			{
				Obj* pObj = pPoint->m_pNode;
				pPoint = pPoint->m_pNext ;

				if( pObj==NULL )
				{
					Assert( FALSE ) ;
					return FALSE ;
				}

				Obj_Human* pHuman = (Obj_Human*)pObj ;
				
				pHuman->GetPlayer()->SendPacket( pPacket ) ;
			};
		}
	}


	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Scene::BroadCast_Chat(Packet* pPacket, ZoneID_t	zoneID )
{
	__ENTER_FUNCTION

	if( zoneID == INVALID_ID )
	{
		uint nPlayerCount = m_pScenePlayerManager->GetPlayerNumber() ;
		for( uint i=0; i<nPlayerCount; i++ )
		{
			PlayerID_t* PlayerIDs = m_pScenePlayerManager->GetPlayers() ;
			if( PlayerIDs[i] == INVALID_ID )
				continue ;

			Player* pPlayer = g_pPlayerPool->GetPlayer(PlayerIDs[i]) ;
			if( pPlayer )
			{
				pPlayer->SendPacket( pPacket ) ;
				GetChatPipe()->m_nValidCount -- ;
			}
		}
	}
	else
	{
		VRECT rc ;
		GetRectInRadius( &rc, MAX_RADIUS, zoneID ) ;

		for(INT h = rc.nStartz; h <= rc.nEndz; h++)
		{
			for(INT w = rc.nStartx; w <= rc.nEndx; w++)
			{
				ZoneID_t zid = w+h*m_ZoneInfo.m_wZoneW ;
				ObjList* pList = m_pZone[zid].GetHumanList();//&(m_pZone[zid].m_HumanList) ;

				_OBJ_LIST_NODE* pPoint = pList->m_Head.m_pNext;
				while(pPoint != &pList->m_Tail)
				{
					Obj* pObj = pPoint->m_pNode;
					pPoint = pPoint->m_pNext ;

					if( pObj==NULL )
					{
						Assert( FALSE ) ;
						return FALSE ;
					}

					Obj_Human* pHuman = (Obj_Human*)pObj ;
					
					pHuman->GetPlayer()->SendPacket( pPacket ) ;
					GetChatPipe()->m_nValidCount -- ;
				};
			}
		}
	}

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Scene::ScanHuman( ZoneID_t idZone, UINT uZoneRadius, HUMANLIST *plistOutHuman )
{
__ENTER_FUNCTION

	if ( idZone == INVALID_ID )
	{
		Assert( idZone != INVALID_ID && "Scene::ScanHuman( ZoneID_t idZone, UINT uZoneRadius, HUMANLIST *plistOutHuman )" );
		return FALSE;
	}

	VRECT rc ;
	GetRectInRadius( &rc,uZoneRadius, idZone );

	plistOutHuman->m_Count = 0;

	INT h, w;
	for ( h = rc.nStartz; h <= rc.nEndz; h++ )
	{
		for ( w = rc.nStartx; w <= rc.nEndx; w++ )
		{
			ZoneID_t zid = w+h*m_ZoneInfo.m_wZoneW ;
			ObjList* pList = m_pZone[zid].GetHumanList();

			_OBJ_LIST_NODE* pPoint = pList->m_Head.m_pNext;
			while(pPoint != &pList->m_Tail)
			{
				Obj_Human* pHuman = (Obj_Human*)(pPoint->m_pNode);
				pPoint = pPoint->m_pNext ;

				plistOutHuman->m_aHuman[plistOutHuman->m_Count++] = pHuman;

				if ( plistOutHuman->m_Count >= MAX_HUMAN_LIST_SIZE )
					return TRUE;
			}
		}
	}
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL Scene::ScanHuman_Sub( ZoneID_t idZoneA, ZoneID_t idZoneB, UINT uZoneRadius, HUMANLIST *plistOutHuman )
{
__ENTER_FUNCTION

	if ( idZoneA == INVALID_ID || idZoneB == INVALID_ID )
	{
		Assert( idZoneA != INVALID_ID && idZoneB != INVALID_ID && "Scene::ScanHuman_Sub( ZoneID_t idZoneA, ZoneID_t idZoneB, UINT uZoneRadius, HUMANLIST *plistOutHuman )" );
		return FALSE;
	}

	VRECT rcA, rcB;
	GetRectInRadius( &rcA,uZoneRadius, idZoneA );
	GetRectInRadius( &rcB,uZoneRadius, idZoneB );

	plistOutHuman->m_Count = 0;

	INT h, w;
	for ( h = rcA.nStartz; h <= rcA.nEndz; h++ )
	{
		for ( w = rcA.nStartx; w <= rcA.nEndx; w++ )
		{
			if ( !rcB.IsContinue( w, h ) )
			{
				ZoneID_t zid = w+h*m_ZoneInfo.m_wZoneW ;
				ObjList* pList = m_pZone[zid].GetHumanList();

				_OBJ_LIST_NODE* pPoint = pList->m_Head.m_pNext;
				while(pPoint != &pList->m_Tail)
				{
					Obj_Human* pHuman = (Obj_Human*)(pPoint->m_pNode);
					pPoint = pPoint->m_pNext ;

					plistOutHuman->m_aHuman[plistOutHuman->m_Count++] = pHuman;

					if ( plistOutHuman->m_Count >= MAX_HUMAN_LIST_SIZE )
						return TRUE;
				}
			}
		}
	}
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL Scene::ScanObj( ZoneID_t idZone, UINT uZoneRadius, OBJLIST *plistOutObj )
{
__ENTER_FUNCTION

	if ( idZone == INVALID_ID )
	{
		Assert( idZone != INVALID_ID && "Scene::ScanObj( ZoneID_t idZone, UINT uZoneRadius, OBJLIST *plistOutObj )" );
		return FALSE;
	}

	VRECT rc ;
	GetRectInRadius( &rc,uZoneRadius, idZone );

	plistOutObj->m_Count = 0;

	INT h, w;
	for ( h = rc.nStartz; h <= rc.nEndz; h++ )
	{
		for ( w = rc.nStartx; w <= rc.nEndx; w++ )
		{
			ZoneID_t zid = w+h*m_ZoneInfo.m_wZoneW ;
			ObjList* pList = m_pZone[zid].GetObjList();

			_OBJ_LIST_NODE* pPoint = pList->m_Head.m_pNext;
			while(pPoint != &pList->m_Tail)
			{
				Obj* pObj = (Obj*)(pPoint->m_pNode);
				pPoint = pPoint->m_pNext ;

				plistOutObj->m_aObj[plistOutObj->m_Count++] = pObj;

				if ( plistOutObj->m_Count >= plistOutObj->MAX_OBJ_LIST_SIZE )
					return TRUE;
			}
		}
	}
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL Scene::ScanObj_Sub( ZoneID_t idZoneA, ZoneID_t idZoneB, UINT uZoneRadius, OBJLIST *plistOutObj )
{
__ENTER_FUNCTION

	if ( idZoneA == INVALID_ID || idZoneB == INVALID_ID )
	{
		Assert( idZoneA != INVALID_ID && idZoneB != INVALID_ID && "Scene::ScanObj_Sub( ZoneID_t idZoneA, ZoneID_t idZoneB, UINT uZoneRadius, OBJLIST *plistOutObj )" );
		return FALSE;
	}

	VRECT rcA, rcB;
	GetRectInRadius( &rcA,uZoneRadius, idZoneA );
	GetRectInRadius( &rcB,uZoneRadius, idZoneB );

	plistOutObj->m_Count = 0;

	INT h, w;
	for ( h = rcA.nStartz; h <= rcA.nEndz; h++ )
	{
		for ( w = rcA.nStartx; w <= rcA.nEndx; w++ )
		{
			if ( !rcB.IsContinue( w, h ) )
			{
				ZoneID_t zid = w+h*m_ZoneInfo.m_wZoneW ;
				ObjList* pList = m_pZone[zid].GetObjList();

				_OBJ_LIST_NODE* pPoint = pList->m_Head.m_pNext;
				while(pPoint != &pList->m_Tail)
				{
					Obj* pObj = (Obj*)(pPoint->m_pNode);
					pPoint = pPoint->m_pNext ;

					plistOutObj->m_aObj[plistOutObj->m_Count++] = pObj;

					if ( plistOutObj->m_Count >= plistOutObj->MAX_OBJ_LIST_SIZE )
						return TRUE;
				}
			}
		}
	}
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL Scene::SendPacket( Packet *pPacket, HUMANLIST *plistHuman )
{
__ENTER_FUNCTION

	UINT i;
	for ( i = 0; i < plistHuman->m_Count; i++ )
	{
		Obj_Human* pHuman = plistHuman->m_aHuman[i];
		if( pHuman == NULL )
		{
			Assert( pHuman != NULL && "Scene::SendPacket" );
			continue;
		}
		pHuman->GetPlayer()->SendPacket( pPacket );
	}
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL Scene::SendPacket_CrateAttr( Obj *pObj, HUMANLIST *plistHuman )
{
__ENTER_FUNCTION

	if ( IsCharacterObj( pObj->GetObjType() ) )
	{
		Obj_Character *pCharacter = (Obj_Character*)pObj;
		UINT i;
		for ( i = 0; i < plistHuman->m_Count; i++ )
		{
			Obj_Human* pHuman = plistHuman->m_aHuman[i];
			if( pHuman == NULL )
			{
				Assert( pHuman != NULL && "Scene::SendPacket_CrateAttr" );
				continue;
			}
			pCharacter->AskMsg_BaseAttrib( pHuman );
			//pHuman->GetPlayer()->SendPacket( pPacket );
		}
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

//
//BOOL Scene::ScanEnv( Obj_Human* pHuman, ZoneID_t zid )
//{
//__ENTER_FUNCTION
//
//	VRECT rc ;
//
//	ZoneID_t CurZoneID ;
//	if( zid==INVALID_ID )
//	{
//		CurZoneID = pHuman->getZoneID() ;
//	}
//	else
//	{
//		CurZoneID = zid ;
//	}
//
//	GetRectInRadius( &rc, MAX_RADIUS, CurZoneID ) ;
//
//	for(INT h = rc.nStartz; h <= rc.nEndz; h++)
//	{
//		for(INT w = rc.nStartx; w <= rc.nEndx; w++)
//		{
//			ZoneID_t zid = w+h*m_ZoneInfo.m_wZoneW ;
//			ObjList* pList = m_pZone[zid].GetObjList();//&(m_pZone[zid].m_ObjList) ;
//
//			_OBJ_LIST_NODE* pPoint = pList->m_Head.m_pNext;
//			while(pPoint != &pList->m_Tail)
//			{
//				Obj* pObj = pPoint->m_pNode;
//				pPoint = pPoint->m_pNext ;
//
//				if( pObj==NULL )
//				{
//					Assert( FALSE ) ;
//					return FALSE ;
//				}
//
//				if( pHuman->GetID()==pObj->GetID() )
//				{
//					continue ;
//				}
//
//				pHuman->SendPacket_NewViewObj( pObj ) ;
//			};
//		}
//	}
//
//
//	return TRUE ;
//
//__LEAVE_FUNCTION
//
//	return FALSE ;
//}
//
//BOOL Scene::ScanEnv_Border( Obj_Human* pHuman, INT right, INT down )
//{
//__ENTER_FUNCTION
//
//	VRECT rc ;
//	GetRectInRadius( &rc, MAX_RADIUS, pHuman->getZoneID() ) ;
//
//	INT x = pHuman->getZoneID()%m_ZoneInfo.m_wZoneW;
//	INT z = pHuman->getZoneID()/m_ZoneInfo.m_wZoneW;
//
//	for(INT h = rc.nStartz; h <= rc.nEndz; h++)
//	{
//		for(INT w = rc.nStartx; w <= rc.nEndx; w++)
//		{
//			BOOL bWide = FALSE ;
//
//			if( right>0 )
//			{
//				if( w==rc.nEndx )
//					bWide = TRUE ;
//			}
//			else if( right<0 )
//			{
//				if( w==rc.nStartx )
//					bWide = TRUE ;					
//			}
//
//			if( !bWide )
//			{
//				if( down>0 )
//				{
//					if( h==rc.nEndz )
//						bWide = TRUE ;						
//				}
//				else if( down<0 )
//				{
//					if( h==rc.nStartz )
//						bWide = TRUE ;						
//				}
//			}
//			if( !bWide )
//				continue ;
//			if( w==x && z==h )
//				continue ;
//
//			ZoneID_t zid = w+h*m_ZoneInfo.m_wZoneW ;
//			ObjList* pList = m_pZone[zid].GetObjList();//&(m_pZone[zid].m_ObjList) ;
//
//			_OBJ_LIST_NODE* pPoint = pList->m_Head.m_pNext;
//			while(pPoint != &pList->m_Tail)
//			{
//				Obj* pObj = pPoint->m_pNode;
//				pPoint = pPoint->m_pNext ;
//
//				if( pObj==NULL )
//				{
//					Assert( FALSE ) ;
//					return FALSE ;
//				}
//
//				if( pHuman->GetID()==pObj->GetID() )
//				{
//					continue ;
//				}
//
//				pHuman->SendPacket_NewViewObj( pObj ) ;
//			};
//		}
//	}
//
//
//	return TRUE ;
//
//__LEAVE_FUNCTION
//
//	return FALSE ;
//}

BOOL Scene::ScanHuman( WORLD_POS* pPos, FLOAT fRange, HUMANLIST* pHumanList )
{
__ENTER_FUNCTION

	ZoneID_t ZID = CalcZoneID( pPos ) ;
	if( ZID==INVALID_ID || ZID>= GetZoneInfo()->m_wZoneSize )
	{
		Assert(FALSE) ;
		return FALSE ;
	}

	VRECT rc ;
	GetRectInRadius( &rc, MAX_RADIUS, ZID ) ;

	pHumanList->m_Count = 0 ;
	
	for(INT h = rc.nStartz; h <= rc.nEndz; h++)
	{
		for(INT w = rc.nStartx; w <= rc.nEndx; w++)
		{
			ZoneID_t zid = w+h*m_ZoneInfo.m_wZoneW ;
			ObjList* pList = m_pZone[zid].GetHumanList();//&(m_pZone[zid].m_HumanList) ;

			_OBJ_LIST_NODE* pPoint = pList->m_Head.m_pNext;
			while(pPoint != &pList->m_Tail)
			{
				Obj* pObj = pPoint->m_pNode;
				pPoint = pPoint->m_pNext ;

				if( pObj==NULL )
				{
					Assert( FALSE ) ;
					return FALSE ;
				}

				Obj_Human* pHuman = (Obj_Human*)pObj ;
				if( pHumanList->m_Count >= MAX_HUMAN_LIST_SIZE )
					return TRUE ;

				FLOAT fTDist = MySqrt(pPos, pHuman->getWorldPos()) ;
				if( fTDist > fRange )
					continue ;

				pHumanList->m_aHuman[pHumanList->m_Count] = pHuman ;
				pHumanList->m_Count ++ ;
			}//end while

		}//end for w

	}//end for h


	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}



BOOL Scene::ScanHuman( ZoneID_t idZone, INT nRadius_ZoneCount, HUMANLIST *pOutHumanList )
{
__ENTER_FUNCTION

	pOutHumanList->m_Count = 0;

	VRECT rc ;

	GetRectInRadius( &rc,nRadius_ZoneCount, idZone ) ;

	for(INT h = rc.nStartz; h <= rc.nEndz; h++)
	{
		for(INT w = rc.nStartx; w <= rc.nEndx; w++)
		{
			ZoneID_t zid = w+h*m_ZoneInfo.m_wZoneW ;
			ObjList* pList = m_pZone[zid].GetHumanList();

			_OBJ_LIST_NODE* pPoint = pList->m_Head.m_pNext;
			while(pPoint != &pList->m_Tail)
			{
				Obj_Human* pHuman = (Obj_Human*)(pPoint->m_pNode);
				pPoint = pPoint->m_pNext ;

				if( pHuman==NULL )
				{
					Assert( FALSE && "Scene::ScanHuman pHuman==NULL" ) ;
					return FALSE ;
				}

				pOutHumanList->m_aHuman[pOutHumanList->m_Count++] = pHuman;
				if ( pOutHumanList->m_Count == MAX_HUMAN_LIST_SIZE )
					return TRUE;
			};
		}
	}

	return TRUE ;

__LEAVE_FUNCTION
	return FALSE ;
}

BOOL Scene::Scan( ScanOperator* pScan )
{
__ENTER_FUNCTION

	if( pScan==NULL )
	{
		Assert( FALSE ) ;
		return FALSE ;
	}

	if( !pScan->On_BeforeScan( ) )
	{
		return TRUE ;
	}

	VRECT rc ;
	INT Count = 0;
	GetRectInRadius( &rc, pScan->m_nZoneRadius, pScan->m_ZoneID ) ;

	BOOL bNeedRet = FALSE ;

	for(INT h = rc.nStartz; h <= rc.nEndz; h++)
	{
		if( bNeedRet )
			break ;

		for(INT w = rc.nStartx; w <= rc.nEndx; w++)
		{
			if( bNeedRet )
				break ;

			ZoneID_t zid = w+h*m_ZoneInfo.m_wZoneW ;
			if( !pScan->On_IsNeedScan( zid ) )
			{
				continue ;
			}

			ObjList* pList = NULL ;
			if( pScan->m_bScanHuman )
			{
				pList = m_pZone[zid].GetHumanList();
			}
			else
			{
				pList = m_pZone[zid].GetObjList();
			}

			_OBJ_LIST_NODE* pPoint = pList->m_Head.m_pNext;
			while(pPoint != &pList->m_Tail)
			{
				Obj* pObj = (Obj*)(pPoint->m_pNode);
				pPoint = pPoint->m_pNext ;

				UINT uRet = pScan->On_FindObj( pObj ) ;
				if( uRet == SCANRETURN_CONTINUE )
				{
					continue ;
				}
				else if( uRet == SCANRETURN_BREAK )
				{
					break ;
				}
				else if( uRet == SCANRETURN_RETURN )
				{
					bNeedRet = TRUE ;
					break ;
				}
				else
				{
					Assert(FALSE) ;
				}
			};

		}
	}
	
	pScan->On_AfterScan( ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

