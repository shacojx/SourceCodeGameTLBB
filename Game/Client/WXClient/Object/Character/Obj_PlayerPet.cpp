#include "StdAfx.h"
#include "Obj_PlayerOther.h"
#include "..\..\Global.h"
#include "..\..\Network\NetManager.h"
#include "CGAttack.h"
#include "CGCharAskEquipment.h"
#include "..\ObjectManager.h"
#include "..\Character\Obj_PlayerMySelf.h"
#include "CGOtherEquip.h"
#include "..\..\NetWork\NetManager.h"
#include "..\..\Procedure\GameProcedure.h"
#include "TDTimeSystem.h"
#include "TDSound.h"
#include "..\ObjectDef.h"
#include "..\..\DBC\GMDataBase.h"
#include "TDDBC_Struct.h"
#include "TDException.h"
#include "..\ObjectCommandDef.h"
#include "..\..\GameCommand.h"
#include "..\Item\Obj_Item_Equip.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Event\GMEventSystem.h"


TD_IMPLEMENT_DYNAMIC(CObject_PlayerPet, GETCLASS(CObject_PlayerNPC));
CObject_PlayerPet::CObject_PlayerPet()
{
	m_fDebugRate = 1.4f;
}

CObject_PlayerPet::~CObject_PlayerPet()
{
}

VOID CObject_PlayerPet::Initial( VOID *pParam )
{
	CObject_PlayerNPC::Initial( pParam );
}

VOID CObject_PlayerPet::CreateRenderInterface(VOID)
{
	m_pRenderInterface = CGameProcedure::s_pGfxSystem->New_FairyObject("Actor");
	m_pRenderInterface->SetData(GetID());

	//----------------------------------------------------------------------
	INT nCharExterior;
	DBC_DEFINEHANDLE(s_pCreatureDBC, DBC_CREATURE_ATT);
	//查询主角族数据表
	DBC_DEFINEHANDLE(s_pCharacterRace, DBC_CHARACTER_RACE);
	//取得主角种族(性别)
	if(m_nExterior)
		m_pCharRace = (const _DBC_CHAR_RACE*)s_pCharacterRace->Search_Index_EQU(m_nExterior);
	else
		m_pCharRace = (const _DBC_CHAR_RACE*)s_pCharacterRace->Search_Index_EQU(GetCharacterData()->Get_RaceID());

	if(!m_pCharRace) return;

	nCharExterior = m_pCharRace->nObjFile;

	const _DBC_CREATURE_ATT* pCharExterior = (const _DBC_CREATURE_ATT*)s_pCreatureDBC->Search_Index_EQU(nCharExterior);
	if(!pCharExterior) return;

	//----------------------------------------------------------------------
	//设置ActorFile
	m_pRenderInterface->SetActorFile( pCharExterior->szModeleFile );
	SetPosition(GetPosition());
	
	//----------------------------------------------------------------------
	//打开阴影
	m_pRenderInterface->SetProjTexture(tEntityNode::SHADOW_BLOB, true);

	//----------------------------------------------------------------------
	//设置缺省的外形
	GetCharacterData()->Set_FaceMesh(m_pCharRace->nDefHeadGeo);
	GetCharacterData()->Set_HairMesh(m_pCharRace->nDefHairGeo);
	GetCharacterData()->Set_FaceMat(m_pCharRace->nDefHeadMat);
	GetCharacterData()->Set_HairMat(m_pCharRace->nDefHairMat);

	//设置缺省装备
	EquipItem_BodyPart(HEQUIP_ARMOR, m_pCharRace->nDefBody);		//衣服
	EquipItem_BodyPart(HEQUIP_BOOT, m_pCharRace->nDefFoot);		//鞋

	//重新刷新装备
	SendEuipRequest();
}

//发出请求更详细信息的请求
VOID CObject_PlayerPet::SendEuipRequest(VOID)
{
	CGCharAskEquipment msg;
	msg.setObjID((ObjID_t)GetServerID());

	CNetManager::GetMe()->SendPacket(&msg);
}


VOID CObject_PlayerPet::Tick(VOID)
{
	CObject_PlayerNPC::Tick();
}

VOID CObject_PlayerPet::Debug_ChangeSpeed(FLOAT fStep)
{
	FLOAT fSpeed = GetCharacterData()->Get_MoveSpeed() + fStep;

	if(fSpeed < 0.0f) fSpeed = 0.0f;
	if(fSpeed > 10.0f) fSpeed = 10.0f;

	GetCharacterData()->Set_MoveSpeed(fSpeed);
}

VOID CObject_PlayerPet::Debug_ChangeRate(FLOAT fStep)
{
	FLOAT fRate = m_fDebugRate + fStep;

	if(fRate < 0.0f) fRate = 0.0f;
	if(fRate > 3.0f) fRate = 3.0f;

	m_fDebugRate = fRate;

	CHAR szTemp[8];
	_snprintf(szTemp, 8, "%.2f", m_fDebugRate);
	if(m_pRenderInterface)
		m_pRenderInterface->SetActorObjectProperty("RunSpeed", szTemp);
}

VOID CObject_PlayerPet::OnDataChanged_Equip(HUMAN_EQUIP point)
{
	INT nID = GetCharacterData()->Get_Equip(point);

	//生成一个临时的Item对象
	CObject_Item_Equip *pTempEquip = (CObject_Item_Equip *)CObject_Item::NewItem(nID);

	//-----------------------------------------------------------
	//如果是装备根据装配点装备物品
	if(pTempEquip)
	{
		if(ICLASS_EQUIP == pTempEquip->GetItemClass())
		{
			switch(pTempEquip->GetItemType())
			{
			case HEQUIP_WEAPON:				//武器，挂接类装备
				{
					EquipItem_BodyLocator(pTempEquip->GetItemType(), pTempEquip->GetVisualID());
					SetWeaponType( pTempEquip->GetWeaponType() );
				}
				break;

			case HEQUIP_CAP:				//帽子, 身体, 手臂, 鞋, 装备类物品
			case HEQUIP_ARMOR:				
			case HEQUIP_CUFF:				
			case HEQUIP_BOOT:	
				{
					EquipItem_BodyPart(pTempEquip->GetItemType(), pTempEquip->GetVisualID());
				}
				break;

			case HEQUIP_SASH:				//腰带, 戒子, 项链
			case HEQUIP_RING:				
			case HEQUIP_NECKLACE:		
				break;

			default:
				break;
			}
		}
	}
	else
	{
		//卸载装备
		UnEquipItem(point);
	}

	CObject_Item::DestroyItem(pTempEquip);

	//Push事件
	std::string strReturn;
	if( IsSpecialObject(strReturn) )
	{
		std::vector<std::string> vectorStr;
		vectorStr.push_back( strReturn );
		CEventSystem::GetMe()->PushEvent( GE_UNIT_EQUIP, vectorStr );
	}
}

VOID CObject_PlayerPet::EquipItem_BodyPart(HUMAN_EQUIP nPart, INT nID)
{
	//-----------------------------------------------------------
	//角色装备类数据库
	DBC_DEFINEHANDLE(s_pEquipItem, DBC_ITEM_VISUAL_CHAR);
	const _DBC_ITEM_VISUAL_CHAR* pEquipVisual = (const _DBC_ITEM_VISUAL_CHAR*)s_pEquipItem->Search_Index_EQU(nID);
	if(!pEquipVisual) return;

	//-----------------------------------------------------------
	//渲染层设置
	if(!m_pRenderInterface) return;

	switch(nPart)
	{
		case HEQUIP_CAP:				//帽子
			{
				m_pRenderInterface->SetActorObjectProperty(szIDSTRING_CAP_MESH, pEquipVisual->pMeshFile);
				m_pRenderInterface->SetActorObjectProperty(szIDSTRING_CAP_MAT, pEquipVisual->pMatName);
			}
			break;
		case HEQUIP_ARMOR:				//身体
			{
				m_pRenderInterface->SetActorObjectProperty(szIDSTRING_MAINBODY_MESH, pEquipVisual->pMeshFile);
				m_pRenderInterface->SetActorObjectProperty(szIDSTRING_MAINBODY_MAT, pEquipVisual->pMatName);
			}
			break;
		case HEQUIP_CUFF:				//手臂
			{
				m_pRenderInterface->SetActorObjectProperty(szIDSTRING_ARM_MESH, pEquipVisual->pMeshFile);
				m_pRenderInterface->SetActorObjectProperty(szIDSTRING_ARM_MAT, pEquipVisual->pMatName);
			}
			break;
		case HEQUIP_BOOT:				//鞋
			{
				m_pRenderInterface->SetActorObjectProperty(szIDSTRING_FOOT_MESH, pEquipVisual->pMeshFile);
				m_pRenderInterface->SetActorObjectProperty(szIDSTRING_FOOT_MAT, pEquipVisual->pMatName);
			}
			break;
	}
}

VOID CObject_PlayerPet::EquipItem_BodyLocator(HUMAN_EQUIP nPart, INT nID)
{
	//-----------------------------------------------------------
	//角色挂接类数据库
	DBC_DEFINEHANDLE(s_pWeaponItem, DBC_ITEM_VISUAL_LOCATOR);
	const _DBC_ITEM_VISUAL_LOCATOR* pEquipVisual = (const _DBC_ITEM_VISUAL_LOCATOR*)s_pWeaponItem->Search_Index_EQU(nID);
	if(!pEquipVisual) return;

	//-----------------------------------------------------------
	//渲染层设置
	if(!m_pRenderInterface) return;
		
	//右手
	if(pEquipVisual->pObjFile_Right && pEquipVisual->pObjFile_Right[0] != '\0')
    	m_pRenderInterface->SetActorObjectProperty(szIDSTRING_CURRENT_RIGHTWEAPON, pEquipVisual->pObjFile_Right);
	else
    	m_pRenderInterface->SetActorObjectProperty(szIDSTRING_CURRENT_RIGHTWEAPON, "");

	//左手
	if(pEquipVisual->pObjFile_Left && pEquipVisual->pObjFile_Left[0] != '\0')
    	m_pRenderInterface->SetActorObjectProperty(szIDSTRING_CURRENT_LEFTWEAPON, pEquipVisual->pObjFile_Left);
	else
    	m_pRenderInterface->SetActorObjectProperty(szIDSTRING_CURRENT_LEFTWEAPON, "");
}

VOID CObject_PlayerPet::UnEquipItem(HUMAN_EQUIP nPart)
{
	if(!m_pRenderInterface) return;

	switch(nPart)
	{
	case HEQUIP_WEAPON:
		m_pRenderInterface->SetActorObjectProperty(szIDSTRING_CURRENT_LEFTWEAPON, "");
		m_pRenderInterface->SetActorObjectProperty(szIDSTRING_CURRENT_RIGHTWEAPON, "");
		SetWeaponType(WEAPON_TYPE_NONE);
		break;

	case HEQUIP_CAP:
		m_pRenderInterface->SetActorObjectProperty(szIDSTRING_CAP_MESH, "");
		m_pRenderInterface->SetActorObjectProperty(szIDSTRING_CAP_MAT, "");
		break;

	case HEQUIP_ARMOR:
		//设置缺省装备
		EquipItem_BodyPart(HEQUIP_ARMOR, m_pCharRace->nDefBody);		//衣服
		break;

	case HEQUIP_CUFF:
		//设置缺省装备
		EquipItem_BodyPart(HEQUIP_CUFF, m_pCharRace->nDefArm);		//手
		break;

	case HEQUIP_BOOT:
		//设置缺省装备
		EquipItem_BodyPart(HEQUIP_BOOT, m_pCharRace->nDefFoot);		//脚
		break;

	case HEQUIP_SASH:
	case HEQUIP_RING:
	case HEQUIP_NECKLACE:
		//Not Care...
		break;

	default:
		break;
	}
}

VOID CObject_PlayerPet::OnDataChanged_FaceMesh(VOID)
{
	INT nFaceMeshID = GetCharacterData()->Get_FaceMesh();

	//-----------------------------------------------------------
	//角色头部模型数据库
	DBC_DEFINEHANDLE(s_pCharHeadGeo, DBC_CHARACTER_HEAD_GEO);
	//查找相应记录
	const _DBC_CHAR_HEAD_GEO* pHeadGeo = (const _DBC_CHAR_HEAD_GEO* )s_pCharHeadGeo->Search_Index_EQU(nFaceMeshID);
	if(!pHeadGeo) return;

	//-----------------------------------------------------------
	//渲染层设置
	if(m_pRenderInterface)
		m_pRenderInterface->SetActorObjectProperty(szIDSTRING_FACE_MESH, pHeadGeo->pMeshFile);

	//-----------------------------------------------------------
	//Push事件
	std::string strReturn;
	if( IsSpecialObject(strReturn) )
	{
		std::vector<std::string> vectorStr;
		vectorStr.push_back( strReturn );
		CEventSystem::GetMe()->PushEvent( GE_UNIT_FACE_MESH, vectorStr );
	}
}

VOID CObject_PlayerPet::OnDataChanged_FaceMat(VOID)
{
	INT nFaceMat = GetCharacterData()->Get_FaceMat();

	//-----------------------------------------------------------
	//角色头部材质数据库
	DBC_DEFINEHANDLE(s_pCharHeadMat, DBC_CHARACTER_HEAD_MAT);
	//查找相应记录
	const _DBC_CHAR_HEAD_MAT* pHeadMat = (const _DBC_CHAR_HEAD_MAT*)s_pCharHeadMat->Search_Index_EQU(nFaceMat);
	if(!pHeadMat) return;

	//-----------------------------------------------------------
	//渲染层设置
	if(m_pRenderInterface)
		m_pRenderInterface->SetActorObjectProperty(szIDSTRING_FACE_MAT, pHeadMat->pMatName);

	//-----------------------------------------------------------
	//Push事件
	std::string strReturn;
	if( IsSpecialObject(strReturn) )
	{
		std::vector<std::string> vectorStr;
		vectorStr.push_back( strReturn );
		CEventSystem::GetMe()->PushEvent( GE_UNIT_FACE_MAT, vectorStr );
	}
}

VOID CObject_PlayerPet::OnDataChanged_MenPai(VOID)
{
	//-----------------------------------------------------------
	//Push事件
	std::string strReturn;
	if( IsSpecialObject(strReturn) )
	{
		std::vector<std::string> vectorStr;
		vectorStr.push_back( strReturn );
		CEventSystem::GetMe()->PushEvent( GE_UNIT_MENPAI, vectorStr );
	}
}

VOID CObject_PlayerPet::OnDataChanged_HairMesh(VOID)
{
	INT nHairMesh = GetCharacterData()->Get_HairMesh();

	//-----------------------------------------------------------
	//角色头发模型数据库
	DBC_DEFINEHANDLE(s_pCharHairMesh, DBC_CHARACTER_HAIR_GEO);
	//查找相应记录
	const _DBC_CHAR_HAIR_GEO* pHairMesh = (const _DBC_CHAR_HAIR_GEO*)s_pCharHairMesh->Search_Index_EQU(nHairMesh);
	if(!pHairMesh) return;

	//-----------------------------------------------------------
	//渲染层设置
	if(m_pRenderInterface)
		m_pRenderInterface->SetActorObjectProperty(szIDSTRING_HAIR_MESH, pHairMesh->pMeshFile);

	//-----------------------------------------------------------
	//Push事件
	std::string strReturn;
	if( IsSpecialObject(strReturn) )
	{
		std::vector<std::string> vectorStr;
		vectorStr.push_back( strReturn );
		CEventSystem::GetMe()->PushEvent( GE_UNIT_HAIR_MESH, vectorStr );
	}
}

VOID CObject_PlayerPet::OnDataChanged_HairMat(VOID)
{
	INT nHairMat = GetCharacterData()->Get_HairMat();

	//-----------------------------------------------------------
	//角色头发材质数据库
	DBC_DEFINEHANDLE(s_pCharHairMat, DBC_CHARACTER_HAIR_MAT);
	//查找相应记录
	const _DBC_CHAR_HAIR_MAT* pHairMat = (const _DBC_CHAR_HAIR_MAT*)s_pCharHairMat->Search_Index_EQU(nHairMat);
	if(!pHairMat) return;

	//-----------------------------------------------------------
	//渲染层设置
	if(m_pRenderInterface)
		m_pRenderInterface->SetActorObjectProperty(szIDSTRING_HAIR_MAT, pHairMat->pMatName);

	//-----------------------------------------------------------
	//Push事件
	std::string strReturn;
	if( IsSpecialObject(strReturn) )
	{
		std::vector<std::string> vectorStr;
		vectorStr.push_back( strReturn );
		CEventSystem::GetMe()->PushEvent( GE_UNIT_HAIR_MAT, vectorStr );
	}
}

VOID CObject_PlayerPet::OnDataChanged_EquipVer(VOID)
{

	//-----------------------------------------------------------
	//Push事件
	std::string strReturn;
	if( IsSpecialObject(strReturn) )
	{
		std::vector<std::string> vectorStr;
		vectorStr.push_back( strReturn );
		CEventSystem::GetMe()->PushEvent( GE_UNIT_EQUIP_VER, vectorStr );
	}
}

VOID CObject_PlayerPet::OnNewSoundHandle(VOID)
{
	//脚步声
	m_hWalkSound = CSoundSystem::GetMe()->CreateNewItem(19);
}

VOID CObject_PlayerPet::OnDeleteSoundHandle(VOID)
{
	if(m_hWalkSound)
	{
		CSoundSystem::GetMe()->DelItem(m_hWalkSound);
		m_hWalkSound = NULL;
	}
}