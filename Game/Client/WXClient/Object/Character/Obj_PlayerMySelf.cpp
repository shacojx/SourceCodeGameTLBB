#include "StdAfx.h"

#include "..\ObjectCommandDef.h"
#include "..\..\NetWork\NetManager.h"

#include "TDUtil.h"
#include "TDException.h"
#include "..\..\GameCommand.h"
#include "..\ObjectManager.h"
#include "..\..\Procedure\GameProcedure.h"
#include "Obj_PlayerMySelf.h"
#include "AI\GMAI_MySelf.h"
#include "..\..\event\GMEventSystem.h"
#include "..\..\FakeObject\GMFakeObjSystem.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "TDSound.h"

#include "TDActionSystem.h"
#include "TDGameInterface.h"
#include "..\..\DataPool\GMDP_Struct_Skill.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "GameStruct_Skill.h"
#include "Obj_PlayerMySelf.h"
#include "..\..\Procedure\GameProcedure.h"

#include "..\WxRender\RenderSystem.h"

WX_IMPLEMENT_DYNAMIC(CObject_PlayerMySelf, GETCLASS(CObject_Character));
CObject_PlayerMySelf::CObject_PlayerMySelf( VOID )
{
	//初始化AI
	m_pTheAI = new CAI_MySelf(this);

	m_pAvatar = NULL;
	m_pHeadAvatar = NULL;
}

CObject_PlayerMySelf::~CObject_PlayerMySelf( VOID )
{
	delete m_pTheAI;
	m_pTheAI = NULL;
}

VOID CObject_PlayerMySelf::Initial(VOID* pParam)
{
	CObject_PlayerOther::Initial(pParam);

	//创建Avatar
	m_pAvatar  = (CObject_PlayerOther*)
			(CFakeObjSystem::GetMe()->NewFakeObject("CObject_PlayerOther", "Player", "Camera_Main"));
	m_pHeadAvatar  = (CObject_PlayerOther*)
			(CFakeObjSystem::GetMe()->NewFakeObject("CObject_PlayerOther", "Player_Head", "Camera_Head"));

	//设置初始方向
	SetFaceDir((FLOAT)TDU_PI);
}

VOID CObject_PlayerMySelf::Release(VOID)
{
	CFakeObjSystem::GetMe()->DestroyObject("Player");
	CFakeObjSystem::GetMe()->DestroyObject("Player_Head");
	m_pAvatar = NULL;
	m_pHeadAvatar = NULL;

	CObject_PlayerOther::Release();
}
//
//// 压入一条指令
//BOOL CObject_PlayerMySelf::PushCommand(const SCommand_Object *pCmd )
//{
//	switch( pCmd->m_wID )
//	{
//	case OC_MOVE_USE_PATH:
//	case OC_MOVE_USE_NODE_LIST:
//	case OC_SKILL_GATHER:
//	case OC_SKILL_GATHER_MODIFY:
//	case OC_SKILL_LEAD:
//	case OC_SKILL_LEAD_MODIFY:
//	case OC_SKILL_SEND:
//		{
//			DoCommand( pCmd );
//
//			CleanupCommandList();
//		}
//		return TRUE;
//	default:
//		return CObject_Character::PushCommand( pCmd );
//	}
//}

BOOL CObject_PlayerMySelf::IsInVisualFiled( CObject_Character* pObj )
{
	TDAssert(pObj);

	const iVector2& vThat = pObj->GetMapPosition();
	const iVector2& vThis = GetMapPosition();

	if(	abs(vThat.x - vThis.x) > 1 ||
		abs(vThat.y - vThis.y) > 1 )
		return FALSE;
	else
		return true;
}

// 左键指令的分析
BOOL CObject_PlayerMySelf::FillMouseCommand_Left( SCommand_Mouse *pOutCmd, tActionItem* pActionItem )
{
	if(pActionItem && pActionItem->GetType() == AOT_ITEM)
	{
		//鼠标上挂的是一个物品
		return CObject_PlayerNPC::FillMouseCommand_Left(pOutCmd, pActionItem);
	}
	else
	{
		pOutCmd->m_typeMouse = SCommand_Mouse::MCT_NULL;
	}
	return TRUE;
}

// 右键指令的分析
BOOL CObject_PlayerMySelf::FillMouseCommand_Right( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill )
{
	pOutCmd->m_typeMouse	= SCommand_Mouse::MCT_CONTEXMENU;
	pOutCmd->m_adwParam[0]	= GetServerID();
	return TRUE;
}

VOID CObject_PlayerMySelf::OnDataChanged_RaceID(VOID)
{
	//创建Avatar外形
	m_pAvatar->GetCharacterData()->Set_RaceID(GetCharacterData()->Get_RaceID());
	m_pAvatar->SetFaceDir(0.0f);

	m_pHeadAvatar->GetCharacterData()->Set_RaceID(GetCharacterData()->Get_RaceID());
	m_pHeadAvatar->SetFaceDir(0.0f);

	CObject_PlayerOther::OnDataChanged_RaceID();
}

VOID CObject_PlayerMySelf::OnDataChanged_HairMesh(VOID)
{
	m_pAvatar->GetCharacterData()->Set_HairMesh(GetCharacterData()->Get_HairMesh());
	m_pHeadAvatar->GetCharacterData()->Set_HairMesh(GetCharacterData()->Get_HairMesh());

	CObject_PlayerOther::OnDataChanged_HairMesh();
}

VOID CObject_PlayerMySelf::OnDataChanged_HairColor(VOID)
{
	m_pAvatar->GetCharacterData()->Set_HairColor(GetCharacterData()->Get_HairColor());
	m_pHeadAvatar->GetCharacterData()->Set_HairColor(GetCharacterData()->Get_HairColor());

	CObject_PlayerOther::OnDataChanged_HairColor();
}

VOID CObject_PlayerMySelf::OnDataChanged_FaceMesh(VOID)
{
	m_pAvatar->GetCharacterData()->Set_FaceMesh(GetCharacterData()->Get_FaceMesh());
	m_pHeadAvatar->GetCharacterData()->Set_FaceMesh(GetCharacterData()->Get_FaceMesh());

	CObject_PlayerOther::OnDataChanged_FaceMesh();
}

VOID CObject_PlayerMySelf::OnDataChanged_Equip(HUMAN_EQUIP point)
{
	m_pAvatar->GetCharacterData()->Set_EquipGem(point,GetCharacterData()->Get_EquipGem(point));
	m_pAvatar->GetCharacterData()->Set_Equip(point, GetCharacterData()->Get_Equip(point));
	m_pHeadAvatar->GetCharacterData()->Set_EquipGem(point,GetCharacterData()->Get_EquipGem(point));
	m_pHeadAvatar->GetCharacterData()->Set_Equip(point, GetCharacterData()->Get_Equip(point));

	CObject_PlayerOther::OnDataChanged_Equip(point);
}

VOID CObject_PlayerMySelf::OnDataChanged_Level(VOID)
{
	SCRIPT_SANDBOX::Talk::s_Talk.SetTalkRule();
}

VOID CObject_PlayerMySelf::SetPosition(const fVector3& fvPosition)
{
	CObject_PlayerOther::SetPosition( fvPosition );
	CGameProcedure::m_bNeedFreshMinimap = true;
}

VOID CObject_PlayerMySelf::OnDataChanged_FaceImage(VOID)
{
	//m_pAvatar->GetCharacterData()->Set_HairMesh(GetCharacterData()->Get_HairMesh());
	//CObject_PlayerOther::OnDataChanged_HairMesh();
}

VOID CObject_PlayerMySelf::OnDataChanged_Die(VOID)
{
	CObject_PlayerOther::OnDataChanged_Die();
	if(GetCharacterData()->IsDie())
	{
		//进入死亡效果
		CGameProcedure::s_pGfxSystem->Scene_SetPostFilter_Death(true);
	}
	else
	{
		CGameProcedure::s_pGfxSystem->Scene_SetPostFilter_Death(FALSE);
	}
}

VOID CObject_PlayerMySelf::Tick_UpdateInfoBoard(VOID)
{
	//隐藏中，不再计算
	if(!IsEnable(OSF_VISIABLE) || !GetRenderInterface()) return;
	if(!m_pInfoBoard) return;
/*
	if (CharacterLogic_Get() == CHARACTER_LOGIC_DEAD && g_theKernel.IsKindOf(GetClass(), GETCLASS(CObject_PlayerNPC)))
	{
		m_pInfoBoard->Show(FALSE);
		return ;
	}
*/
	//计算位置
	fVector2 fvPos = m_pInfoBoard ? m_pInfoBoard->GetPosition() : fVector2(0.f, 0.f);
	fVector3 fvCurPos = GetPosition();
	if ( m_pMountRenderInterface != NULL )
	{
		fvCurPos.y += m_fMountAddHeight;
	}
	BOOL bVisible;

	bVisible = GetRenderInterface()->Actor_GetInfoBoardPos(fvPos,&fvCurPos);

	if(!bVisible)
	{
		m_pInfoBoard->Show(FALSE);
		return;
	}

	// 设置离主角的距离
	fVector3 fMyselfPos = CObjectManager::GetMe()->GetMySelf()->GetPosition();
	FLOAT fDistance = TDU_GetDistSq( CObjectManager::GetMe()->GetMySelf()->GetPosition(), GetPosition() );

	m_fvInfoBoardPos = fvPos;
	float Pos_X =	m_pInfoBoard->GetPosition().x;

	if( (fvPos.x - Pos_X < 10) && (fvPos.x - Pos_X > -10) )
        m_pInfoBoard->SetPosition(Pos_X, fvPos.y);//为了不晃动
	else
		m_pInfoBoard->SetPosition(fvPos.x,fvPos.y);

	//AxTrace(0, 1, "Pos_X=%d", Pos_X);
	//AxTrace(0, 1, "ivPos.x=%d", ivPos.x);
	m_pInfoBoard->SetInfoDistance( fDistance );

	m_pInfoBoard->Show(TRUE);
}

VOID CObject_PlayerMySelf::ResetLogicCount(VOID)
{
	CObject_PlayerOther::ResetLogicCount();
	m_nSimulantLogicCount	= -1;
}
