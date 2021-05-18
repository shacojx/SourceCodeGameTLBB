#include "stdafx.h"
/*
客户端请求打开界面摊位
*/

#include "CGStallOpen.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCStallOpen.h"
#include "ItemContainer.h"
#include "GCDetailAttrib_Pet.h"
#include "GameTable.h"

uint CGStallOpenHandler::Execute( CGStallOpen* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}
	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;
	ObjID_t ObjId = pPacket->GetObjID();

	//临时数组
	GCStallOpen::_STALL_ITEM	StallItem[STALL_BOX_SIZE+STALL_PET_BOX_SIZE];

	if(ObjId == pHuman->GetID())
	{//自己打开自己，肯定打开
		//摊主container
		if(pHuman->m_StallBox.GetStallStatus() != ServerStallBox::STALL_OPEN)
		{
			GCStallError	Msg;
			Msg.SetID(STALL_MSG::ERR_ILLEGAL);
			pGamePlayer->SendPacket(&Msg);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallOpenHandler::ObjID=%d, ERR_ILLEGAL: != ServerStallBox::STALL_OPEN"
				,pHuman->GetID()) ;
			return PACKET_EXE_CONTINUE;
		}

		ItemContainer*	pStallContainer		= pHuman->m_StallBox.GetContainer(); 
		ItemContainer*	pStallPetContainer	= pHuman->m_StallBox.GetPetContainer(); 
		
		//循环写入
		UINT	k = 0;	
		for(INT i = 0; i<pStallContainer->GetContainerSize(); i++)
		{
			if( pHuman->m_StallBox.GetSerialByIndex(i) != 0 )
			{//这个格子里的东西更改过，需要通知客户端它的序列号
				StallItem[k].nIndex		= i;
				StallItem[k].nPrice		= pHuman->m_StallBox.GetPriceByIndex(i);
				StallItem[k].nSerial	= pHuman->m_StallBox.GetSerialByIndex(i);
				if(pStallContainer->GetItem(i)->IsEmpty() == FALSE)
				{
					pStallContainer->GetItem(i)->SaveValueTo(&(StallItem[k].item));
				}
				k++;
			}
		}

		for(INT i = 0; i<pStallPetContainer->GetContainerSize(); i++)
		{
			if( pHuman->m_StallBox.GetPetSerialByIndex(i) != 0 )
			{//有东西
				StallItem[k].bIsPet		=	TRUE;
				StallItem[k].nIndex		=	i;
				StallItem[k].nPrice		=	pHuman->m_StallBox.GetPetPriceByIndex(i);
				StallItem[k].nSerial	=	pHuman->m_StallBox.GetPetSerialByIndex(i);
				if(pStallPetContainer->GetItem(i)->IsEmpty() == FALSE)
				{
					StallItem[k].PetGuid	=	pStallPetContainer->GetItem(i)->GetPetGUID();
				}
				k++;
			}
		}

		pHuman->StallNameChanged();

		//消息填充
		GCStallOpen Msg;
		Msg.SetFirstPage( pHuman->m_StallBox.GetFirstPage());
		Msg.SetMerchadiseNum(k);
		Msg.SetMerchadiseList(StallItem);
		Msg.SetShopName(pHuman->m_StallBox.GetStallName(), (UINT)strlen(pHuman->m_StallBox.GetStallName()));
		Msg.SetObjID(ObjId);
		Msg.SetGUID(pHuman->GetGUID());

		//发还本人Q
		pGamePlayer->SendPacket(&Msg);

	}
	else
	{//别人
		//判断距离是否可以打开
		Obj_Character *pTarget = (Obj_Character*)(pScene->GetObjManager()->GetObj( ObjId ));
		if(pTarget->GetObjType() != Obj::OBJ_TYPE_HUMAN)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallOpenHandler::ObjName=%s, ERR_OWNER_INVALID: ObjId = %d"
				,pHuman->GetName(), ObjId) ;
			return PACKET_EXE_CONTINUE;
		}
		Obj_Human* pTargetHuman = (Obj_Human*)pTarget;
		if( pTargetHuman == NULL )
		{
			GCStallError Msg;
			Msg.SetID(STALL_MSG::ERR_OWNER_INVALID);
			pHuman->GetPlayer()->SendPacket(&Msg);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallOpenHandler::ObjName=%s, ERR_OWNER_INVALID: ObjId = %d"
				,pHuman->GetName(), ObjId) ;
			return PACKET_EXE_CONTINUE;
		}
		// 不同阵营，不让查看
		if( pHuman->IsEnemy( pTargetHuman ) )
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGStallOpenHandler: %s cann't ask %s's detailattr ", pHuman->GetName(), pTargetHuman->GetName() ) ;
			return	PACKET_EXE_CONTINUE;
		}
		
		if(pTargetHuman->m_StallBox.GetStallStatus() != ServerStallBox::STALL_OPEN)
		{
			GCStallError	Msg;
			Msg.SetID(STALL_MSG::ERR_CLOSE);
			pGamePlayer->SendPacket(&Msg);
			g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGStallOpenHandler::ObjName=%s, ERR_CLOSE"
				,pHuman->GetName()) ;
			return PACKET_EXE_CONTINUE;
		}

		//摊主container
		ItemContainer*	pStallContainer = pTargetHuman->m_StallBox.GetContainer(); 

		//循环写入
		UINT	k = 0;	
		for(INT i = 0; i<pStallContainer->GetContainerSize(); i++)
		{
			if( pStallContainer->GetItem(i)->IsEmpty() == FALSE )
			{//有东西
				StallItem[k].nIndex		= i;
				StallItem[k].nPrice		= pTargetHuman->m_StallBox.GetPriceByIndex(i);
				StallItem[k].nSerial	= pTargetHuman->m_StallBox.GetSerialByIndex(i);
				pStallContainer->GetItem(i)->SaveValueTo(&(StallItem[k].item));
				k++;
			}
		}

		//消息填充
		GCStallOpen Msg;
		Msg.SetFirstPage( pTargetHuman->m_StallBox.GetFirstPage());
		Msg.SetMerchadiseNum(k);
		Msg.SetMerchadiseList(StallItem);
		Msg.SetShopName(pTargetHuman->m_StallBox.GetStallName(), MAX_STALL_NAME);
		Msg.SetObjID(ObjId);
		Msg.SetGUID(pTargetHuman->GetGUID());

		//发还本人
		pGamePlayer->SendPacket(&Msg);

		//宠物列表
		ItemContainer*	pStallPetContainer = pTargetHuman->m_StallBox.GetPetContainer(); 

		//循环写入
		for(INT i = 0; i<pStallPetContainer->GetContainerSize(); i++)
		{
			if( pStallPetContainer->GetItem(i)->IsEmpty() == FALSE )
			{//有东西
				INT	nPrice	= pTargetHuman->m_StallBox.GetPetPriceByIndex(i);
				INT	nSerial	= pTargetHuman->m_StallBox.GetPetSerialByIndex(i);

				Item* pIt = pStallPetContainer->GetItem(i);
				GCDetailAttrib_Pet PetMsgDetail;

				//组装GCDetailAttrib_Pet结构
				Obj_Human::CalculatePetDetailAttrib(PetMsgDetail, pIt);
				PetMsgDetail.SetTradeIndex( i );
				
				PET_EXTRA_STRUCT::GCStallPetView_t	ExtraPetInfo;
				ExtraPetInfo.m_bFlag	= TYPE_STALL;
				ExtraPetInfo.m_nPrice	= nPrice;
				ExtraPetInfo.m_nSerial	= nSerial;

				PetMsgDetail.SetExtraInfoLength(ExtraPetInfo.GetSize());
				PetMsgDetail.SetExtraInfoData((BYTE*)&ExtraPetInfo);
				pGamePlayer->SendPacket( &PetMsgDetail );		
			}
		}
	}
	g_pLog->FastSaveLog( LOG_FILE_1, "CGStallOpenHandler::ObjID=%d"
		,pHuman->GetID()) ;
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
