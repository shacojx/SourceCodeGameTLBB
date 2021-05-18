#include "stdafx.h"
#include "Ability.h"
#include "AbilityGatherFish.h"
#include "Scene.h"
#include "Log.h"
#include "Obj_ItemBox.h"
#include "ItemManager.h"
#include "GCBoxItemList.h"
#include "GameTable.h"
#include "Obj_Human.h"
#include "AI_Human.h"
#include "GCAbilityAction.h"
#include "GCAbilityResult.h"


#define		MIN_FISH_TIME	10
#define		MAX_FISH_TIME	70


AbilityGatherFish::~AbilityGatherFish()
{
}

ORESULT		AbilityGatherFish::CanUseAbility(Obj_Human* pHuman)
{
__ENTER_FUNCTION

	ORESULT res = AbilityGather::CanUseAbility(pHuman);
	
	if( res != OR_OK )
	{
		return res;
	}
	return OR_OK;
__LEAVE_FUNCTION
	return OR_ERROR ;
}


ORESULT		AbilityGatherFish::OnProcOver(Obj_Human* pHuman)
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
		
			//自动开始
			//判断是否能打开
			if(pScene->GetGrowPointManager()->CallScriptOpenBoxFunc(pGET->m_ScriptID,pHuman->GetID(),ItemBoxObjID,pScene->SceneID()))
			{
				return OnProcFailure(pHuman);
			}

			//进入打开过程
		
			ORESULT res = pHuman->GetHumanAI()->PushCommand_UseAbility();
			if( res == OR_OK )
			{
				//GCAbilityAction Msg;
				//Msg.setObjID( pHuman->GetID() );
				//Msg.setAction( pAbilityOpera->m_AbilityID );
				//Msg.setTargetID( pAbilityOpera->m_Obj );
				//Msg.setBeginOrEnd( GCAbilityAction::ABILITY_BEGIN );
				//pScene->BroadCast( &Msg, pHuman, FALSE );
			}

			GCAbilityResult Msg;
			Msg.SetAbilityID( pAbilityOpera->m_AbilityID );
			Msg.SetPrescriptionID( INVALID_ID );
			Msg.setResult( res );
			pHuman->GetPlayer()->SendPacket(&Msg);
			g_pLog->FastSaveLog( LOG_FILE_1, "AutoStart Ability AbilityID=%d PrescriptionID=-1 PlatformGUID=%d Result=%d",
				pAbilityOpera->m_AbilityID, pAbilityOpera->m_Obj, res ) ;

			return OnProcSuccess(pHuman);

		}

	}
	else
	{
		return OnProcSuccess(pHuman);
	}

__LEAVE_FUNCTION

	return OR_ERROR;
}

