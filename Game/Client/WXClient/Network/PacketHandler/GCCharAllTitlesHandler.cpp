//GCCharAllTitlesHandler.cpp
#include "stdafx.h"
#include "GCCharAllTitles.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerOther.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\Item\Obj_Item.h"
#include "..\..\Action\GMActionSystem.h"
#include "..\..\Object\GMObjectLoadQueue.h"

uint GCCharAllTitlesHandler::Execute( GCCharAllTitles* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		CObject* pObj = (CObject*)(pObjectManager->FindServerObject( (INT)pPacket->getTargetID() ));
		if ( pObj == NULL )
			return PACKET_EXE_CONTINUE;

		////如果需要更新资源，尝试放入缓冲队列
		//if(pObjectManager->GetLoadQueue()->TryAddLoadTask(pObj->GetID(), pPacket))
		//	return PACKET_EXE_NOTREMOVE;

		CCharacterData* pCharacterData = ((CObject_Character*)pObj)->GetCharacterData();
		int	titleidNum = pPacket->GetTitleIdNum();
		TDAssert(titleidNum>=0 && titleidNum<=MAX_TITLE_ID_SIZE);
		int	titleStrNum = pPacket->GetTitleStrNum();
		TDAssert(titleStrNum>=0 && titleStrNum<=MAX_STRING_TITLE_SIZE);

		pCharacterData->Clear_AllTitles();	
		_TITLE_ titletmp;
		int num = 0;

		for(int i =0; i<titleidNum; i++)
		{
			titletmp.bFlag	=	_TITLE_::ID_TITLE;
			titletmp.bType	=	pPacket->GetTitleIdList()[i].m_titleid;
			titletmp.ID		=	pPacket->GetTitleIdList()[i].m_titleData;
			pCharacterData->Set_Title(num++, titletmp);
		}
		
		for(int i =0; i<titleStrNum; i++)
		{
			titletmp.bFlag	=	_TITLE_::STRING_TITLE;
			titletmp.bType	=	pPacket->GetTitleStrList()[i].m_titleid;
			memset(&titletmp.szTitleData, 0, MAX_CHARACTER_TITLE);
			memcpy(&titletmp.szTitleData, &(pPacket->GetTitleStrList()[i].m_szTitle), pPacket->GetTitleStrList()[i].m_strlength);
			pCharacterData->Set_Title(num++, titletmp);
		}
		
		pCharacterData->Set_TitleNum(num);

//		CUIDataPool::GetMe()->TitleList_Open();

//		CEventSystem::GetMe()->PushEvent(GE_OPEN_AGNAME);
	}

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}