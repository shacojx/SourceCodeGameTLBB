#include "stdafx.h"
// CGGemCompoundHandler.cpp

#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "HumanItemLogic.h"
#include "Log.h"

#include "OResultDef.h"
#include "ScriptDef.h"

#include "CGGemCompound.h"
#include "GCOperateResult.h"

uint CGGemCompoundHandler::Execute( CGGemCompound* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer;
	Assert( pGamePlayer );

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );

	Scene* pScene = pHuman->getScene();
	if( pScene==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_ERROR;
	}

	// 检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

	// 玩家处于不可行动状态
	if( !pHuman->IsAlive() )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGGemCompoundHandler: Name=%s is not alive.", pHuman->GetName() );
		return PACKET_EXE_CONTINUE;
	}

	ORESULT res = OR_OK;

	INT nBagIndex1 = pPacket->GetGemBagIndex1();
	INT nBagIndex2 = pPacket->GetGemBagIndex2();
	if( (nBagIndex1 < 0) || (nBagIndex1 >= MAX_BAG_SIZE)
	 || (nBagIndex2 < 0) || (nBagIndex2 >= MAX_BAG_SIZE)
	 )
	{
		Assert(FALSE);
		res = OR_ERROR;
	}
	else
	{
		Item* pGem1Item = HumanItemLogic::GetItem(pHuman, nBagIndex1);
		Item* pGem2Item = HumanItemLogic::GetItem(pHuman, nBagIndex2);
		if( pGem1Item == NULL || pGem2Item == NULL )
		{
			Assert( (pGem1Item != NULL) && (pGem2Item != NULL) );
			res = OR_ERROR;
		}
		else if( (pGem1Item->GetItemClass() != ICLASS_GEM) || (pGem2Item->GetItemClass() != ICLASS_GEM) )
		{
			res = OR_STUFF_LACK;
		}
	}

	if( res == OR_OK )
	{ // 通过初步验证
		LuaInterface* pLuaInterface = pHuman->getScene()->GetLuaInterface();
		if( pLuaInterface == NULL )
		{
			Assert( pLuaInterface );
			return PACKET_EXE_ERROR;
		}

		res = (ORESULT)pLuaInterface->ExeScript_DDDD( GEM_COMPOUND_SCRIPT, DEF_GEM_COMPOUND,
			(INT)pScene->SceneID(), (INT)pHuman->GetID(), nBagIndex1, nBagIndex2);
		if( res != OR_OK )
		{
			// 发送失败消息
			pHuman->SendOperateResultMsg(res);
		}
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "CGGemCompoundHandler BagPosition1=%d BagPosition2=%d \
									   Result=%d", nBagIndex1, nBagIndex2, res );

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
