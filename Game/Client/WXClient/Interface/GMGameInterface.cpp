#include "StdAfx.h"
#include "GMGameInterface.h"
#include "..\Procedure\GameProcedure.h"
#include "..\world\worldmanager.h"
#include "..\network\netmanager.h"
#include "..\object\Object.h"
#include "..\object\Objectmanager.h"
#include "..\object\character\Obj_playermyself.h"
#include "..\global.h"
#include "TDinputsystem.h"
#include "TDUtil.h"
#include "..\Procedure\GamePro_main.h"
#include "TDworldsystem.h"
#include "TDobjectsystem.h"
#include "tdsound.h"
#include "packets\cgpickboxitem.h"
#include "packets\cguseequip.h"
#include "..\object\ObjectCommand.h"
#include "..\object\ObjectCommanddef.h"
#include "..\DataPool\GMUIDataPool.h"
#include "..\DataPool\GMDataPool.h"
#include "..\Object\Character\AI\GMAICommandDef.h"
#include "..\Object\Character\AI\GMAI_Base.h"
#include "CGCommand.h"
#include "..\World\Path.h"
#include "..\Object\ProjTex\Obj_ProjTex.h"
#include "..\Object\Item\Obj_Item_Equip.h"
#include "CGUseGem.h"
#include "CGUseItem.h"
#include "CGDiscardItem.h"
#include "CGDiscardEquip.h"
#include "CGUnEquip.h"
#include "CGReqResetAttr.h"
#include "CGCommand.h"
#include "CGReqLevelUp.h"
#include "..\Action\GMActionSystem.h"
#include "..\DataPool\DetailAttrib_Player.h"
#include "cgchat.h"
#include "..\DBC\GMDataBase.h"
#include "..\World\WorldManager.h"
#include "..\Event\GMEventSystem.h"
#include "..\DataPool\GMDP_CharacterData.h"
#include "..\Input\GMInputSystem.h"
#include "CampAndStand.h"
#include "DB_Struct.h"

//商店
#include "CGShopBuy.h"
#include "CGShopClose.h"
#include "CGShopSell.h"
#include "CGShopRepair.h"
//称号
#include "CGCharUpdateCurTitle.h"
#include "GMInterface_Script_SystemSetup.h"
//stall
#include "CGStallOpen.h"
#include "CGBBSApply.h"

#include "..\Action\GMActionSystem_Item.h"
#include "..\Object\Item\Obj_Item_Medicine.h"
#include "..\Action\GMActionSystem_MouseCmd.h"
#include "..\WxRender\RenderSystem.h"

CGameInterface* CGameInterface::s_pMe = NULL;
WX_IMPLEMENT_DYNAMIC(CGameInterface, GETCLASS(tGameInterfaceBase));
CGameInterface::CGameInterface()
{
	s_pMe = this;
}

CGameInterface::~CGameInterface()
{
	s_pMe = 0;
}

VOID CGameInterface::Initial(VOID*)
{
	Make_Count = -1;
	Make_Prescription = -1;
	//系统设置接口挂接变量控制系统
	CEventSystem::GetMe()->RegisterEventHandle("VARIABLE_CHANGED",	SCRIPT_SANDBOX::SystemSetup::_OnVariableChangedEvent);
}

VOID CGameInterface::Tick(VOID)
{
}

VOID CGameInterface::Release(VOID)
{
}

VOID CGameInterface::Object_SelectAsMainTarget(INT idObj, int iType)
{
	CObjectManager::GetMe()->SetMainTarget(idObj, (tObjectSystem::DESTROY_MAIN_TARGET_TYPE)iType);
}

VOID CGameInterface::Object_ShowContexMenu(INT idObj,BOOL showMyself)
{
	CObject* pObject = (CObject*)CObjectManager::GetMe()->FindServerObject(idObj);
	if(!pObject) return;

	CHAR szObjId[32]; _snprintf(szObjId, 32, "%d", pObject->GetID());

	//得到鼠标位置
	POINT ptMouse = CInputSystem::GetMe()->MouseGetPos();

	CHAR szXPos[32]; _snprintf(szXPos, 32, "%d", ptMouse.x);
	CHAR szYPos[32]; _snprintf(szYPos, 32, "%d", ptMouse.y);
	
	std::vector< STRING > vParam;
	//根据不同物体产生不同右键事件
	if(g_theKernel.IsKindOf(pObject->GetClass(), GETCLASS(CObject_PlayerMySelf)))
	{
		if(showMyself)
		{
			// 如果自己已经在队伍中了
			if( CUIDataPool::GetMe()->IsInTeam())
			{
				vParam.push_back("player_in_team");
				vParam.push_back(szObjId);
				vParam.push_back(szXPos);
				vParam.push_back(szYPos);
				CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);
				return;
			}

			// 自己没有组队
			vParam.push_back("player");
			vParam.push_back(szObjId);
			vParam.push_back(szXPos);
			vParam.push_back(szYPos);
			CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);
		}
	}
	else if(g_theKernel.IsKindOf(pObject->GetClass(), GETCLASS(CObject_PlayerOther)))
	{
		CObject_PlayerOther* pOtherObject = (CObject_PlayerOther*)pObject;

		// 自己有队伍, 点击的其他玩家也有队伍
		if((pOtherObject->GetCharacterData()->Get_HaveTeamFlag())&&( CUIDataPool::GetMe()->IsInTeam()))
		{
			vParam.push_back("other_team_member");
			vParam.push_back(szObjId);
			vParam.push_back(szXPos);
			vParam.push_back(szYPos);
			CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);
			return;

		}

		//点击其他玩家没有队伍
		if((!pOtherObject->GetCharacterData()->Get_HaveTeamFlag()))
		{
			vParam.push_back("other_not_team_member");
			vParam.push_back(szObjId);
			vParam.push_back(szXPos);
			vParam.push_back(szYPos);
			CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);
			return;

		}

		
		// 自己没队伍, 点击其他玩家有队伍
		if((pOtherObject->GetCharacterData()->Get_HaveTeamFlag())&&( !CUIDataPool::GetMe()->IsInTeam()))
		{
			vParam.push_back("other_team_member_me_not_teamer");
			vParam.push_back(szObjId);
			vParam.push_back(szXPos);
			vParam.push_back(szYPos);
			CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);
			return;

		}


		vParam.push_back("other_player");
		vParam.push_back(szObjId);
		vParam.push_back(szXPos);
		vParam.push_back(szYPos);
		CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);
	}
	else if(g_theKernel.IsKindOf(pObject->GetClass(), GETCLASS(CObject_PlayerNPC)))
	{
		PET_GUID_t pg = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentPetGUID();

		//自己的宠物
		if(!(pg.IsNull()) && CDataPool::GetMe()->Pet_GetPet(pg) && CDataPool::GetMe()->Pet_GetPet(pg)->m_idServer == pObject->GetServerID())
		{
			vParam.push_back("my_pet");
			vParam.push_back(szObjId);
			vParam.push_back(szXPos);
			vParam.push_back(szYPos);
			CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);
			return;
		}

		CObject_PlayerNPC* pNpcObject = (CObject_PlayerNPC*)pObject;
		//其他宠物
		if(pNpcObject->GetNpcType() == NPC_TYPE_PET)
		{
			if(INVALID_ID != pNpcObject->GetCharacterData()->Get_OwnerID())
			{
				//有归属的宠物才显示菜单
				vParam.push_back("other_pet");
				vParam.push_back(szObjId);
				vParam.push_back(szXPos);
				vParam.push_back(szYPos);
				CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);
			}
			return;
		}

		vParam.push_back("npc");
		vParam.push_back(szObjId);
		vParam.push_back(szXPos);
		vParam.push_back(szYPos);
		CEventSystem::GetMe()->PushEvent(GE_SHOW_CONTEXMENU, vParam);
	}
	else return;

}

VOID CGameInterface::Player_UseSkill(INT idSkill)
{
	//判断是否为合法的瞬发技能
	CActionItem* pAction = (CActionItem*)CActionSystem::GetMe()->GetAction(idSkill);
	if(!pAction) 
		return;

	//取得技能数据
	const SCLIENT_SKILL *pSkill = (const SCLIENT_SKILL *)pAction->GetImpl();
	if(!pSkill) return;

	CAI_Base * pMySelfAI = CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();

	//发送消息
	SCommand_AI cmdTemp;
	cmdTemp.m_wID			= AIC_USE_SKILL;
	cmdTemp.m_adwParam[0]	= pSkill->m_pDefine->m_nID;
	cmdTemp.m_adwParam[1]	= pSkill->m_nLevel;
	cmdTemp.m_adwParam[2]	= INVALID_ID;
	cmdTemp.m_afParam[3]	= -1.f;
	cmdTemp.m_afParam[4]	= -1.f;
	cmdTemp.m_afParam[5]	= -1.f;
	cmdTemp.m_auParam[6]	= INVALID_GUID;
	pMySelfAI->PushCommand( &cmdTemp );

	//发送事件
	CGameProcedure::s_pEventSystem->PushEvent(GE_ON_SKILL_ACTIVE, idSkill);
}

VOID CGameInterface::Player_UseSkill(INT idSkill, INT idObj)
{
	//判断是否为合法的目标技能
	CActionItem* pAction = (CActionItem*)CActionSystem::GetMe()->GetAction(idSkill);
	if(!pAction) return;

	//取得技能数据
	const SCLIENT_SKILL *pSkill = (const SCLIENT_SKILL *)pAction->GetImpl();
	if(!pSkill) return;

	//检测目标是否合法
	
//	if(!(pSkill->IsValidTarget(idObj))) return;
	

	CAI_Base * pMySelfAI = CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();

	//发送消息
	SCommand_AI cmdTemp;
	cmdTemp.m_wID			= AIC_USE_SKILL;
	cmdTemp.m_adwParam[0]	= pSkill->m_pDefine->m_nID;
	cmdTemp.m_adwParam[1]	= pSkill->m_nLevel;
	cmdTemp.m_adwParam[2]	= idObj;
	cmdTemp.m_afParam[3]	= -1.f;
	cmdTemp.m_afParam[4]	= -1.f;
	cmdTemp.m_afParam[5]	= -1.f;
	cmdTemp.m_auParam[6]	= INVALID_GUID;
	pMySelfAI->PushCommand( &cmdTemp );

	//发送事件
	CGameProcedure::s_pEventSystem->PushEvent(GE_ON_SKILL_ACTIVE, idSkill);
}

VOID CGameInterface::Player_UseSkill(INT idSkill, const fVector2& fvPos)
{
	//判断是否为合法的范围技能
	CActionItem* pAction = (CActionItem*)CActionSystem::GetMe()->GetAction(idSkill);
	if(!pAction) return;

	CAI_Base * pMySelfAI = CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();

	const SCLIENT_SKILL *pSkill = (const SCLIENT_SKILL *)pAction->GetImpl();
	if(!pSkill) return;

	//发送消息
	SCommand_AI cmdTemp;
	cmdTemp.m_wID			= AIC_USE_SKILL;
	cmdTemp.m_adwParam[0]	= pSkill->m_pDefine->m_nID;
	cmdTemp.m_adwParam[1]	= pSkill->m_nLevel;
	cmdTemp.m_adwParam[2]	= INVALID_ID;
	cmdTemp.m_afParam[3]	= fvPos.x;
	cmdTemp.m_afParam[4]	= fvPos.y;
	cmdTemp.m_afParam[5]	= -1.f;
	cmdTemp.m_auParam[6]	= INVALID_GUID;
	pMySelfAI->PushCommand( &cmdTemp );

	//发送事件
	CGameProcedure::s_pEventSystem->PushEvent(GE_ON_SKILL_ACTIVE, idSkill);
}

VOID CGameInterface::Player_UseSkill(INT idSkill, FLOAT fDir)
{
	//判断是否为合法的方向技能
	CActionItem* pAction = (CActionItem*)CActionSystem::GetMe()->GetAction(idSkill);
	if(!pAction) return;

	CAI_Base * pMySelfAI = CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();

	const SCLIENT_SKILL *pSkill = (const SCLIENT_SKILL *)pAction->GetImpl();
	if(!pSkill) return;

	//发送消息
	SCommand_AI cmdTemp;
	cmdTemp.m_wID			= AIC_USE_SKILL;
	cmdTemp.m_adwParam[0]	= pSkill->m_pDefine->m_nID;
	cmdTemp.m_adwParam[1]	= pSkill->m_nLevel;
	cmdTemp.m_adwParam[2]	= INVALID_ID;
	cmdTemp.m_afParam[3]	= -1.0f;
	cmdTemp.m_afParam[4]	= -1.0f;
	cmdTemp.m_afParam[5]	= fDir;
	cmdTemp.m_afParam[6]	= -1.f;
	cmdTemp.m_auParam[7]	= INVALID_GUID;
	pMySelfAI->PushCommand( &cmdTemp );

	//发送事件
	CGameProcedure::s_pEventSystem->PushEvent(GE_ON_SKILL_ACTIVE, idSkill);
}

//使用技能(对某个队友)
VOID CGameInterface::Player_UseSkill( INT idSkill, const GUID_t& guid)
{
	//判断是否为合法的目标技能
	CActionItem* pAction = (CActionItem*)CActionSystem::GetMe()->GetAction(idSkill);
	if(!pAction) return;

	//取得技能数据
	const SCLIENT_SKILL *pSkill = (const SCLIENT_SKILL *)pAction->GetImpl();
	if(!pSkill) return;

	CAI_Base * pMySelfAI = CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();

	//发送消息
	SCommand_AI cmdTemp;
	cmdTemp.m_wID			= AIC_USE_SKILL;
	cmdTemp.m_adwParam[0]	= pSkill->m_pDefine->m_nID;
	cmdTemp.m_adwParam[1]	= pSkill->m_nLevel;
	cmdTemp.m_adwParam[2]	= -1;
	cmdTemp.m_afParam[3]	= -1.f;
	cmdTemp.m_afParam[4]	= -1.f;
	cmdTemp.m_afParam[5]	= -1.f;
	cmdTemp.m_auParam[6]	= guid;
	pMySelfAI->PushCommand( &cmdTemp );

	//发送事件
	CGameProcedure::s_pEventSystem->PushEvent(GE_ON_SKILL_ACTIVE, idSkill);
}

VOID CGameInterface::Player_UseLifeAbility(INT idPrescription,INT nMakeCount)
//VOID CGameInterface::Player_UseLifeAbility(INT idPrescription)
{
	if( nMakeCount < 1 )
		return;
	Make_Count = nMakeCount;
	Make_Prescription = idPrescription;

	Player_UseLifeAbility(idPrescription);
	return;
}

VOID CGameInterface::Player_UseLifeAbility(INT idPrescription)
{
	if( (--Make_Count) < 0 )
		return;
	if( idPrescription != Make_Prescription)
		return;
	//判断是否满足使用技能之条件
	CAI_Base * pMySelfAI = CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();

	//发送消息
	SCommand_AI cmdTemp;
	cmdTemp.m_wID			= AIC_COMPOSE_ITEM;
	cmdTemp.m_adwParam[0]	= idPrescription;
	pMySelfAI->PushCommand( &cmdTemp );
}

VOID CGameInterface::Player_MoveTo( const fVector2&  fvAimPos )
{
	static const INT	DRAG_MOVE_TIME_INTERVAL	= 0;
	static unsigned long  ulTime = 0;

	// 当点选地面移动时必须及时响应，修复了原来在使用技能的时候，
	// 移动被忽略的相像
	//if( ulTime +  DRAG_MOVE_TIME_INTERVAL < ::timeGetTime() )
	{
		//显示指示环
		CGameProcedure::s_pProcMain->SetMouseTargetProjTex(fvAimPos);

		//移动指令
		CAI_Base *pMySelfAI;
		pMySelfAI		= CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();

		pMySelfAI->PushCommand_MoveTo(fvAimPos.x, fvAimPos.y);

		//记录时间
		ulTime = ::timeGetTime();
	}
}
VOID CGameInterface::Player_AUTOHit( const int AUTOHit )
{
		//挂机指令
		CAI_Base *pMySelfAI;
		pMySelfAI		= CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();
		pMySelfAI->PushCommand_AutoHit(AUTOHit);
}
VOID CGameInterface::Player_DestroyEquip(const tObject_Item* pEquip)
{
	if(!pEquip || pEquip->GetItemClass() != ICLASS_EQUIP) return;

	CGDiscardEquip msg;
	msg.SetEquipPoint((BYTE)((CObject_Item_Equip*)pEquip)->GetItemType());

	CNetManager::GetMe()->SendPacket( &msg );
}

VOID CGameInterface::Player_UnEquip(const tObject_Item* pEquip)
{
	if(!pEquip || pEquip->GetItemClass() != ICLASS_EQUIP) return;

	CGUnEquip msg;
	msg.setEquipPoint((BYTE)((CObject_Item_Equip*)pEquip)->GetItemType());

	CNetManager::GetMe()->SendPacket( &msg );
}

//请求升级
VOID CGameInterface::Player_AskLevelUp(VOID)
{
	CGReqLevelUp msg;

	CNetManager::GetMe()->SendPacket(&msg);
}

//请求洗一级属性点
VOID CGameInterface::Player_AskCleanBat1Point(VOID)
{
	CGReqResetAttr msg;

	CNetManager::GetMe()->SendPacket(&msg);
}

//谈话
VOID CGameInterface::Player_Speak(INT idObj)
{
	CAI_Base *pMySelfAI;
	pMySelfAI		= CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();

	SCommand_AI cmdSpeak;
	cmdSpeak.m_wID = AIC_DEFAULT_EVENT;
	cmdSpeak.m_adwParam[0] = idObj;
	pMySelfAI->PushCommand(&cmdSpeak);
}

//-----------------------------------------------------------
// 从货架上头购买一个物品
VOID CGameInterface::Booth_BuyItem( tObject_Item* pItem )
{
	CGShopBuy msg;
	// 先判断是否够钱买

	msg.SetUniqueID(CDataPool::GetMe()->Booth_GetShopUniqueId());
	msg.SetIndex(pItem->GetPosIndex());

	CNetManager::GetMe()->SendPacket( &msg );
}

// 卖出一个物品
VOID CGameInterface::Booth_SellItem( tObject_Item* pItem )
{
	//验证是否可以卖出
	if(!pItem->Rule(tObject_Item::RULE_SALE))
	{
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, pItem->RuleFailMessage(tObject_Item::RULE_SALE).c_str());
		return;
	}

	//判断当前的NPC是不是收购这类物品
	if( !CDataPool::GetMe()->Booth_IsCanBuy(pItem) )
	{
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "我不收购这个物品");
		return;
	}

	CGShopSell msg;
	// 先判断是否够钱买，
	msg.SetUniqueID(CDataPool::GetMe()->Booth_GetShopUniqueId());
	msg.SetBagIndex( pItem->GetPosIndex() );
	CNetManager::GetMe()->SendPacket( &msg );
}


//完成交易
VOID CGameInterface::Booth_Close()
{
	//本次交易结束
	CGShopClose msg;
	CNetManager::GetMe()->SendPacket( &msg );

	//清空原有商品列表,用这个标示商店界面已经关上了
	CDataPool::GetMe()->Booth_Close();
	if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Repair::GetMe())
	{
		CActionSystem::GetMe()->SetDefaultAction(CGameInterface::GetMe()->Skill_GetActive());
	}

	return;
}

//修理
VOID CGameInterface::Booth_Repair(BOOL bAll, INT bagindex, BOOL isInBag)
{	
	if(isInBag)
	{
		CGShopRepair msg;
		msg.RepairAll(bAll);
		if(bAll)
		{
			CNetManager::GetMe()->SendPacket( &msg );
		}
		else
		{
			msg.SetBagIndex(bagindex);
			CNetManager::GetMe()->SendPacket( &msg );
		}
	}
	else
	{
		CGShopRepair msg;
		msg.RepairAll(FALSE);
		msg.SetBagIndex(bagindex);
		msg.SetOpt(CGShopRepair::FromEquip);
		CNetManager::GetMe()->SendPacket( &msg );
	}

}

//鼠标进入修理状态
VOID CGameInterface::Booth_EnterRepair(VOID)
{
	CActionSystem::GetMe()->SetDefaultAction(CActionItem_MouseCmd_Repair::GetMe());
}

//恢复到缺省技能操作，用于退出修理，鉴定，等特殊鼠标命令状态
VOID CGameInterface::Skill_CancelAction(VOID)
{
	//修理
	if(	CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Repair::GetMe())
	{
		CActionSystem::GetMe()->SetDefaultAction(Skill_GetActive());
	}
	else if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Identify::GetMe())  //鉴定
	{
		CDataPool::GetMe()->Identify_UnLock();
		CActionSystem::GetMe()->SetDefaultAction(Skill_GetActive());
	}
}

//-------------------------------------------------
//更新称号
VOID CGameInterface::Title_UpdateCurTitle(BYTE titleType, SHORT titleData)
{
	CGCharUpdateCurTitle Msg;
	Msg.setTitleType(titleType);
	Msg.setTitleData(titleData);

	CNetManager::GetMe()->SendPacket( &Msg );
}



//=====================================================================
VOID CGameInterface::TripperObj_Active( UINT dwId )
{
	CAI_Base *pMySelfAI;
	pMySelfAI		= CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();
	
	SCommand_AI cmdTemp;
	cmdTemp.m_wID			= AIC_TRIPPER_ACTIVE;
	cmdTemp.m_adwParam[0]	= dwId;
	pMySelfAI->PushCommand( &cmdTemp );
}

VOID CGameInterface::ItemBox_PickItem( tObject_Item* pItem )
{
	_ITEM_GUID  sDataGuid;
	
	CGPickBoxItem msg;
	msg.setItemBoxId( CDataPool::GetMe()->ItemBox_GetID() );

	WORD wServer, wWorld;
	GUID_t dwSerial;
	pItem->GetGUID(wWorld, wServer, dwSerial);

	sDataGuid.m_Serial	= dwSerial;
	sDataGuid.m_Server	= (BYTE)wServer;
	sDataGuid.m_World	= (BYTE)wWorld;
	msg.setItemId( sDataGuid );

	CNetManager::GetMe()->SendPacket( &msg );
}

VOID CGameInterface::PacketItem_UserItem(tActionItem* pActionItem, int targetServerID, fVector2& fvPos)
{
	//空物品
	if(!pActionItem || pActionItem->GetType() != AOT_ITEM) return;
	CObject_Item* pItem = (CObject_Item*)(((CActionItem_Item*)pActionItem)->GetItemImpl());
	if(!pItem) return;
	//必须是能够使用的物品
	if(pItem->GetItemClass()!=ICLASS_COMITEM && pItem->GetItemClass()!=ICLASS_TASKITEM) return;
	//特殊物品不能在背包中直接使用，例如，宠物技能书
	STRING strTemp;
	if(!CObject_Item::CheckUseInPackage(pItem, strTemp))
	{
		if(!strTemp.empty()) CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
		return;
	}
	//组队跟随中...
	if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_TeamFollowFlag()) return;

	//检查目前选中的目标
	CObject* pObj = (CObject*)CObjectManager::GetMe()->FindServerObject(targetServerID);
	
	//检查物品是否能够直接使用
	int objID;
	PET_GUID_t petID;
	bool bCanuseDir = ((CObject_Item_Medicine*)pItem)->IsValidTarget(pObj, fvPos, objID, petID);

	if(bCanuseDir)
	{
		WORLD_POS posTarget(fvPos.x, fvPos.y);

		//能够直接使用
		CGUseItem msg;
		msg.SetBagIndex( pItem->GetPosIndex() );
		msg.SetTargetObjID(objID);
		msg.SetTargetPetGUID(petID);
		msg.SetTargetPos(&posTarget);

		CNetManager::GetMe()->SendPacket( &msg );
		return;
	}

	//如果已经选中目标，说明目标不合适,如果是用在自己宠物上的物品，说明宠物没有释放
	if(pObj || ((CObject_Item_Medicine*)pItem)->IsTargetOne())
	{
		CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE, "无效目标");
		return;
	}

	//需要选中目标,在鼠标上挂上物品
	CActionSystem::GetMe()->SetDefaultAction(pActionItem);
}

VOID CGameInterface::PacketItem_UserItem_ToTarget( INT nIndex, UINT nTargetID)
{
	if ( CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_TeamFollowFlag() )
	{
		return;
	}

	CGUseItem msg;
	msg.SetBagIndex( nIndex );
	msg.SetTargetObjID(nTargetID);

	CNetManager::GetMe()->SendPacket( &msg );
}

VOID CGameInterface::PacketItem_UserEquip( INT nIndex )
{
	tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem( nIndex );
	if(pItem == NULL ) return;

	//摆摊时不可以随便换装
	if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
	{
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你正在摆摊……");
		return;
	}

	int iNeedLevel   = 0;
	int iPlayerLevel = 0;
	int iNeedJob = -1;

	//验证是否可以使用
	if(!pItem->Rule(tObject_Item::RULE_USE))
	{
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, pItem->RuleFailMessage(tObject_Item::RULE_USE).c_str());
		return;
	}

	CCharacterData* pCharData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
	iNeedJob = pItem->GetNeedJob();
	if( pCharData )
	{
		if( iNeedJob != -1 && iNeedJob != pCharData->Get_MenPai() )
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "此物品无法装备");
			return;
		}
	}


	//判断等级
	iNeedLevel = pItem->GetNeedLevel();
	if(pCharData)
	{
		iPlayerLevel = pCharData->Get_Level();
	}

	if(iPlayerLevel < iNeedLevel)
	{
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "等级不够");
		return;
	}

	//是否鉴定
	if(((CObject_Item_Equip*)pItem)->GetEquipQuantity() == CObject_Item_Equip::BLUE_EQUIP &&
		((CObject_Item_Equip*)pItem)->GetEquipAttrib() == CObject_Item_Equip::EQUIP_ATTRIB_UNIDENTIFY)
	{
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "此物品需要鉴定");
		return;
	}

	//取得详细信息
	LPCTSTR szExtraInfo = ((tObject_Item*)pItem)->GetExtraDesc();

	//如果没有详细信息，从服务器上请求详细信息
	if(!szExtraInfo)
	{
		CDataPool::GetMe()->UserBag_AskExtraInfo( nIndex );
	}

	CGUseEquip msg;
	msg.setBagIndex( nIndex );
	CNetManager::GetMe()->SendPacket( &msg );
}

VOID CGameInterface::PacketItem_UserGem(INT nGemIndex, INT nEquipIndex)
{
	CGUseGem msg;
	msg.SetGemBagIndex(nGemIndex);
	msg.SetEquipBagIndex(nEquipIndex);
	CNetManager::GetMe()->SendPacket( &msg );
}

VOID CGameInterface::PacketItem_Destroy(INT nIndex)
{
	CGDiscardItem msg;
	msg.SetBagIndex(nIndex);

	CNetManager::GetMe()->SendPacket( &msg );
}

VOID CGameInterface::Scene_SetPostFilter(LPCTSTR szFilterName)
{
	CGameProcedure::s_pGfxSystem->Scene_SetPostFilter(szFilterName);
}

VOID CGameInterface::Scene_SetLightmapQuality(INT nQuality)
{
	CGameProcedure::s_pGfxSystem->Scene_SetLightmapQuality(nQuality);
}
VOID CGameInterface::SendChatCommand( INT nChannelID, INT nChatType, INT nTeamID, LPCTSTR szContex, LPCTSTR szTargetName )
{
	if( strlen( szContex ) == 0 )
		return;
	CGChat   msg;
	msg.SetChannelID( nChannelID );
	msg.SetContexSize( BYTE( strlen( szContex ) ) );
	msg.SetContex( (CHAR*)szContex );
	msg.SetTargetName( (CHAR*)szTargetName );
	msg.SetTeamID( nTeamID );
	msg.SetChatType( nChatType );
	CNetManager::GetMe()->SendPacket( &msg );
}

VOID CGameInterface::GM_SendCommand( LPCSTR strCommand )
{
	if(!strCommand || strCommand[0] == 0) return;

	CGCommand msg;
	msg.SetCommand((CHAR*)strCommand);
	msg.SetCommandSize((BYTE)strlen(strCommand));

	CNetManager::GetMe()->SendPacket( &msg );
}

VOID CGameInterface::Skill_SetActive(tActionItem* pActiveSkill)
{
	//设置激活技能
	CGameProcedure::s_pProcMain->SetActiveSkill(pActiveSkill);

	//通知UI设置按钮的Check状态
	CActionSystem::GetMe()->SetDefaultAction(pActiveSkill);
}

tActionItem* CGameInterface::Skill_GetActive(VOID)
{
	return CGameProcedure::s_pProcMain ? CGameProcedure::s_pProcMain->GetActiveSkill() : 0;
}

ENUM_RELATION CGameInterface::GetCampType( CObject *pObj_A, CObject *pObj_B )
{
	if ( pObj_A == NULL || pObj_B == NULL )
	{
		return RELATION_ENEMY;
	}

	if ( pObj_A == pObj_B )
	{
		return RELATION_FRIEND;
	}

	ObjID_t idOwner_A, idOwner_B, idA, idB;
	idA			= pObj_A->GetServerID();
	idB			= pObj_B->GetServerID();
	idOwner_A	= pObj_A->GetOwnerID();
	idOwner_B	= pObj_B->GetOwnerID();
	if ( idOwner_A != INVALID_ID || idOwner_B != INVALID_ID )
	{
		if ( idOwner_A == idOwner_B
			|| idOwner_A == idB
			|| idOwner_B == idA )
		{
			return RELATION_FRIEND;
		}
	}

	//const _CAMP_DATA *pCamp_A, *pCamp_B;
	//BOOL bHuman_A, bHuman_B;
	//const CampAndStandDataMgr_T *pCampMgr;

	//pCamp_A		= pObj_A->GetCampData();
	//pCamp_B		= pObj_B->GetCampData();
	//bHuman_A	= g_theKernel.IsKindOf( pObj_A->GetClass(), GETCLASS(CObject_PlayerMySelf));
	//bHuman_B	= g_theKernel.IsKindOf( pObj_B->GetClass(), GETCLASS(CObject_PlayerMySelf));
	//pCampMgr	= CGameProcedure::s_pCampDataMgr;

	return CalcRelationType( pObj_A->GetCampData()->m_nCampID, pObj_B->GetCampData()->m_nCampID, CGameProcedure::s_pCampDataMgr );
}
//打开一个玩家的摊位
VOID CGameInterface::Stall_OpenStallSale(INT nObjId)
{
	CObject *pChar = (CObject*) CObjectManager::GetMe()->FindServerObject(nObjId);
	if(pChar)
	{
		//先判定距离是不是合法
		const fVector3 pMePos = CObjectManager::GetMe()->GetMySelf()->GetPosition();
		const fVector3 pOtherPos = (pChar)->GetPosition();

		FLOAT fDistance = 
			TDU_GetDist(fVector2(pMePos.x, pMePos.z),
						fVector2(pOtherPos.x, pOtherPos.z));

		AxTrace(0,0,"%f", fDistance);

		if( EXCHANGE_MAX_DISTANCE < fDistance )
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"距离太远，不能打开这个摊位");
			return ;
		}

		//发送打开摊位的消息
		CGStallOpen OpenStallMsg;
		OpenStallMsg.SetObjID(nObjId);
		CNetManager::GetMe()->SendPacket(&OpenStallMsg);

		//同时发送请求StallMessage的消息
		//CGBBSApply BbsMsg;
		//BbsMsg.SetObjID(nObjId);
		//CNetManager::GetMe()->SendPacket(&BbsMsg);
	}

}

