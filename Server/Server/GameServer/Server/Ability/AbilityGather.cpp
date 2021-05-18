#include "stdafx.h"
#include "AbilityGather.h"
#include "Scene.h"
#include "Log.h"
#include "Obj_ItemBox.h"
#include "ItemManager.h"
#include "GCBoxItemList.h"
#include "GameTable.h"
#include "GCAbilitySucc.h"

using namespace Packets;


AbilityGather::AbilityGather(Ability& a):Ability(a)
{
	
}


ORESULT	AbilityGather::CanUseAbility(Obj_Human* pHuman)
{
__ENTER_FUNCTION

	Assert( pHuman );

	ORESULT res;
	res = Ability::CanUseAbility(pHuman);
	if( res != OR_OK )
	{
		return res;
	}
	AbilityOpera* pAbilityOpera;
	pAbilityOpera = pHuman->GetAbilityOpera();
	Assert( pAbilityOpera );

	ObjID_t		ItemBoxObjID = pAbilityOpera->m_Obj;

	if( ItemBoxObjID == INVALID_ID )
	{
		Assert(FALSE) ;
	}
	
	Scene* pScene = pHuman->getScene();
	Assert(pScene);

	Obj* pObj = pScene->GetObjManager()->GetObj( ItemBoxObjID ) ;

	if( pObj==NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "AbilityGatherMine: not find obj=%d", ItemBoxObjID ) ;
		return OR_INVALID_TARGET ;
	}

	Obj_ItemBox*	pItemBox = (Obj_ItemBox*)pObj;

	if( pItemBox->GetObjType()!= Obj::OBJ_TYPE_ITEM_BOX)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "AbilityGatherMine: Obj_ItemBox obj	=	%d\r\n  is Not class of ITEM_CLASS_BOX", ItemBoxObjID ) ;
		return OR_INVALID_TARGET ;
	}

	if(!pItemBox->CanPickBox(pHuman->GetID(),pHuman->GetID()))
		return OR_FAILURE;



	INT ItemBoxType = pItemBox->GetType();

	_GROW_POINT_INFO*	pGET = g_GrowPointInfoTbl.Get(ItemBoxType);
	Assert(pGET);

	if(pGET->m_ScriptID>0)
	{
		return (OPERATE_RESULT)(pScene->GetGrowPointManager()->CallScriptOpenBoxFunc(pGET->m_ScriptID,pHuman->GetID(),ItemBoxObjID,pScene->SceneID()));
		
	}


	return OR_OK;

__LEAVE_FUNCTION

	return OR_ERROR ;
}

ORESULT		AbilityGather::OnProcOver(Obj_Human* pHuman)
{
__ENTER_FUNCTION
	
	AbilityOpera* pAbilityOpera;

	pAbilityOpera = pHuman->GetAbilityOpera();
	Assert( pAbilityOpera );

	ObjID_t		ItemBoxObjID = pAbilityOpera->m_Obj;

	if( ItemBoxObjID == INVALID_ID )
	{
		Assert(FALSE) ;
	}

	Scene* pScene = pHuman->getScene();
	Assert(pScene);

	Obj* pObj = pScene->GetObjManager()->GetObj( ItemBoxObjID ) ;

	if( pObj==NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "AbilityGather: not find obj=%d", ItemBoxObjID ) ;
		return OR_INVALID_TARGET ;
	}

	Obj_ItemBox*	pItemBox = (Obj_ItemBox*)pObj;

	if( pItemBox->GetObjType()!= Obj::OBJ_TYPE_ITEM_BOX)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "AbilityGather: Obj_ItemBox obj	=	%d\r\n  is Not class of ITEM_CLASS_BOX", ItemBoxObjID ) ;
		return OR_INVALID_TARGET ;
	}

	INT ItemBoxType = pItemBox->GetType();

	_GROW_POINT_INFO*	pGET = g_GrowPointInfoTbl.Get(ItemBoxType);
	Assert(pGET);

	if(pGET->m_ScriptID>0)
	{
		if(pScene->GetGrowPointManager()->CallScriptProcOverFunc(pGET->m_ScriptID,pHuman->GetID(),ItemBoxObjID,pScene->SceneID()))
		{
			return OnProcFailure(pHuman);
		}
		else
		{
			return OnProcSuccess(pHuman);
			
		}
	}
	else
	{
		return OnProcSuccess(pHuman);
	}
	
__LEAVE_FUNCTION

	return OR_ERROR ;
}

ORESULT	AbilityGather::OnProcSuccess(Obj_Human* pHuman)
{
__ENTER_FUNCTION

	Assert(pHuman);
	AbilityOpera* pOpera	=	pHuman->GetAbilityOpera();
	Assert(pOpera);
	
	ObjID_t		TargetID	=	pOpera->m_Obj;
	AbilityID_t	AbilityID	=	pOpera->m_AbilityID;

	Assert(TargetID!=INVALID_ID);

	Obj* pObj		=   pHuman->getScene()->GetObjManager()->GetObj(TargetID);

	if(pObj->GetObjType()==Obj::OBJ_TYPE_ITEM_BOX)
	{
		Obj_ItemBox* pItemBox = static_cast<Obj_ItemBox*>(pObj);
		Assert(pItemBox);

		if(pItemBox->GetItemCount()>0)
		{
			if(pItemBox->GetOpenFlag()!=TRUE)
			{
				pItemBox->SetOpenFlag(TRUE);
			}
			
			GCBoxItemList	Msg;
			int j = 0;
			for(int i =0;i<MAX_BOXITEM_NUMBER;i++)
			{
				_ITEM* pItem = pItemBox->GetItem(i);
				Assert(pItem);

				if(pItem->IsNullType())
					continue;

				Msg.setItemData(pItem,j);
				j++;
			}
			Msg.setItemNum(pItemBox->GetItemCount());
			Msg.setItemBoxId(pItemBox->GetID());
			Msg.setItemBoxType(pItemBox->GetType());
			pHuman->GetPlayer()->SendPacket(&Msg);
		}
		else
		{
			Scene* pScene = pHuman->getScene();
			Assert(pScene);
			INT ItemBoxType = pItemBox->GetType();
			_GROW_POINT_INFO*	pGET = g_GrowPointInfoTbl.Get(ItemBoxType);
			Assert(pGET);
			if(pGET->m_ScriptID>0)
			{
				if(pScene->GetGrowPointManager()->CallScriptRecycleFunc(pGET->m_ScriptID,pHuman->GetID(),TargetID,pScene->SceneID()))
				{
					pItemBox->Recycle();
				}
			}

		}

		GCAbilitySucc SuccMsg;
		SuccMsg.SetAbilityID( AbilityID );
		SuccMsg.SetPrescriptionID( INVALID_ID );
		pHuman->GetPlayer()->SendPacket( &SuccMsg );
	
	}
	else
	{
		return OR_INVALID_TARGET;
	}
	return OR_OK;
__LEAVE_FUNCTION
	return OR_ERROR;
}

VOID	AbilityGather::DefaultProcSuccess(Obj_Human* pHuman)
{

}


VOID	AbilityGather::ScriptProcSuccess(Obj_Human* pHuman)
{

}
