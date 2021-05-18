#include "stdafx.h"
// CGShopRepairHandler.cpp

/*  旧的修理费用公式
修理费用计算：
V：物品的标准价格，查询道具表
L：最大耐久
A暂定A=6，建议开放
x：当前耐久，0<=x<=L
y：修理费用
公式：
当0<=x<1/3L时：y= -7*V^2*x*A/（4*L）+V^2
当1/3L<=x<2/3L时：y= -V^2*x*A/L+3*V^2/4
当2/3L<=x<=L时：y= -V^2*x*A/（4*L）+V^2/4
*/


/*  新的修理费用公式
修理费用计算：
V：物品的标准价格，查询道具表
L：最大耐久
x：当前耐久，0<=x<=L
y：修理费用
公式：
y=a*(1-x/L)*V，a为参数暂定a=1，建议开放
*/

#define REPAIR_SPEND 1

#include "GCShopRepair.h"
#include "GCShopSell.h"
#include "GCShopBuy.h"
#include "CGShopRepair.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "Log.h"
#include "ShopManager.h"
#include "HumanItemLogic.h"

uint CGShopRepairHandler::Execute( CGShopRepair* pPacket, Player* pPlayer )
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

	BYTE	Opt         =	pPacket->GetOpt();
	BOOL	RepairAll	=	pPacket->IsRepairAll();		//修理全部
	BYTE	BagIndex	=	pPacket->GetBagIndex();		//包中的位置
	UINT	UniqueID	=	pPacket->GetUniqueID();

	SceneID_t	SceneID =	UniqueID>>16;
	ObjID_t		NpcObjID=	UniqueID&0x00ff;

	if(SceneID != pScene->SceneID())
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGShopRepairHandler Illegal scene ObjName = %s", pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ; 
	}

	//距离判定
	Obj* pNpcObj = (Obj*) (pScene->GetObjManager()->GetObj(NpcObjID));
	if(pNpcObj == NULL)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGShopRepairHandler Illegal Obj ObjName = %s", pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ; 
	}

	FLOAT fDist = MySqrt(pNpcObj->getWorldPos(), pHuman->getWorldPos());

	if(fDist>MAX_NPC_DISTANCE)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGShopRepairHandler Out Of Range ObjName = %s", pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ; 
	}

	Item*	pCurItem = NULL;
	INT				MoneySpent = 0;
	INT				MoneyLast = 0;
	INT				MoneyHave  = pHuman->GetMoney();
	INT				RepairedIndex = 0;
	GCShopRepair	Msg;
	GCShopRepair::REPAIRED_ITEM	ItemList[MAX_REPAIRED_NUMBER];

	_SHOP*		pShop	=	pHuman->Shop( )	;
	if(!pShop)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR:CGShopRepairHandler Npc Shop Lose");
		return PACKET_EXE_CONTINUE;
	}

	if(Opt == CGShopRepair::FromBag)
	{
		if(RepairAll)
		{
			INT nBeginPoint	= 0;
			INT nEndPoint	= 0;
			switch( pShop->m_nRepairType )
			{
			case SHOP_All:
				{
					nBeginPoint = HEQUIP_WEAPON;
					nEndPoint	= HEQUIP_RIDER;
				}
				break;
			case SHOP_WEAPON:		//防具
				{
					nBeginPoint = HEQUIP_WEAPON;
					nEndPoint	= HEQUIP_WEAPON;
				}
				break;
			case SHOP_DEFENCE:		//饰物
				{
					nBeginPoint = HEQUIP_CAP;
					nEndPoint	= HEQUIP_BOOT;
				}
				break;
			case SHOP_ADORN:		//武器
				{
					nBeginPoint = HEQUIP_SASH;
					nEndPoint	= HEQUIP_RIDER;
				}
				break;
			default:
				{
					return PACKET_EXE_CONTINUE;
				}
				break;
			}


			for(INT i =nBeginPoint; i<=nEndPoint; i++)
			{
				pCurItem	= HumanItemLogic::GetEquip(pHuman,(HUMAN_EQUIP)i);
				//pCurItem = pHuman->GetEquip((HUMAN_EQUIP)i);
				if(!pCurItem->IsEmpty())
				{//有东西
					//判定是否狗修理等级
					if(pCurItem->GetRequireLevel() > pShop->m_nRepairLevel)
					{
						continue;
					}

					FLOAT fCur = (FLOAT)pCurItem->GetDurPoints();
					FLOAT fMax = (FLOAT)pCurItem->GetMaxDurPoint();
					FLOAT V	   = (FLOAT)pCurItem->GetPrice();

					if(fCur != fMax)
					{
						//if(fCur<fMax/3)
						//	MoneySpent += (INT)(7*V*V*fCur*6/(4*fMax)+V*V);
						//else if(fCur>=fMax/3&&fCur<fMax*2/3)
						//	MoneySpent += (INT)(V*V*fCur*6/fMax+V*V*3/4);
						//else if(fCur>=fMax*2/3&&fCur<fMax)
						//	MoneySpent += (INT)(V*V*fCur*6/(4*fMax)+V*V/4);
						MoneySpent += (INT) ( ((REPAIR_SPEND * (1- fCur/fMax) * V) * pShop->m_nRepairSpend) + 0.5);

						//临时的修理费计算方法
//						FLOAT scale = (FLOAT)(fCur/fMax);
//						scale = 1-scale;
//						MoneySpent += static_cast<INT>(pCurItem->GetPrice()*scale);
						if(MoneyHave<MoneySpent)
							break;
						MoneyLast	=	MoneySpent;
						HumanItemLogic::SetEquipDur(pHuman,(HUMAN_EQUIP)i, (INT)fMax);

						ItemList[RepairedIndex].IsIn		=	GCShopRepair::EQUIP;					
						ItemList[RepairedIndex++].BagIndex	=	(BYTE)i;
					}
				}
			}

			if(MoneyHave<MoneySpent)
			{//break出来的
				if(RepairedIndex)
				{
					pHuman->SetMoney(MoneyHave - MoneyLast);
					
					MONEY_LOG_PARAM	MoneyLogParam;
					MoneyLogParam.CharGUID	=	pHuman->GetGUID();
					MoneyLogParam.OPType	=	MONEY_SHOP_REPAIR;
					MoneyLogParam.Count		=	MoneyLast;
					MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
					MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
					MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
					SaveMoneyLog(&MoneyLogParam);


					Msg.SetAll(0);
					Msg.SetReListNum(RepairedIndex);
					Msg.SetReList(ItemList);
					pHuman->GetPlayer()->SendPacket(&Msg);
				}

				GCShopBuy Msg ;
				Msg.SetBuyOk((BYTE)GCShopBuy::BUY_MONEY_FAIL);
				pHuman->GetPlayer()->SendPacket( &Msg ) ;
				g_pLog->FastSaveLog( LOG_FILE_1, "CGShopRepairHandler BUY_MONEY_FAIL") ;
				return PACKET_EXE_CONTINUE ;
			}
			else if(MoneySpent != 0)
			{//全修完了
				pHuman->SetMoney(MoneyHave - MoneySpent);
				MONEY_LOG_PARAM	MoneyLogParam;
				MoneyLogParam.CharGUID	=	pHuman->GetGUID();
				MoneyLogParam.OPType	=	MONEY_SHOP_REPAIR;
				MoneyLogParam.Count		=	MoneySpent;
				MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
				MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
				MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
				SaveMoneyLog(&MoneyLogParam);

				Msg.SetAll(1);
				Msg.SetReListNum(0);
				pHuman->GetPlayer()->SendPacket(&Msg);
			}
		}
		else
		{//修理单个,一定应该在背包中
			pCurItem =	HumanItemLogic::GetItem(pHuman,BagIndex);
			if(!pCurItem->IsEmpty())
			{
				if(pCurItem->GetRequireLevel()>pShop->m_nRepairLevel)
				{
					g_pLog->FastSaveLog( LOG_FILE_1, "CGShopRepairHandler Npc have no the Repair Levle") ;
					return PACKET_EXE_CONTINUE;
				}
				BOOL bCanRepair = TRUE;
				switch( pShop->m_nRepairType )
				{
				case SHOP_All:
					{
					}
					break;
				case SHOP_DEFENCE:		//防具
					{
						if( pCurItem->GetEquipPoint() != HEQUIP_CAP &&
							pCurItem->GetEquipPoint() != HEQUIP_ARMOR &&
							pCurItem->GetEquipPoint() != HEQUIP_CUFF &&
							pCurItem->GetEquipPoint() != HEQUIP_BOOT )
						{
							bCanRepair = FALSE;
						}
					}
					break;
				case SHOP_ADORN:		//饰物
					{
						if( pCurItem->GetEquipPoint() != HEQUIP_SASH &&
							pCurItem->GetEquipPoint() != HEQUIP_RING &&
							pCurItem->GetEquipPoint() != HEQUIP_NECKLACE &&
							pCurItem->GetEquipPoint() != HEQUIP_RIDER)
						{
							bCanRepair = FALSE;
						}
					}
					break;
				case SHOP_WEAPON:		//武器
					{
						if( pCurItem->GetEquipPoint() != HEQUIP_WEAPON )
						{
							bCanRepair = FALSE;
						}
					}
					break;
				default:
					{
						bCanRepair = FALSE;
					}
					break;
				}

				if(bCanRepair == FALSE)
				{
					g_pLog->FastSaveLog( LOG_FILE_1, "CGShopRepairHandler Npc have no the Repair type") ;
					return PACKET_EXE_CONTINUE;
				}


				FLOAT fCur = (FLOAT)pCurItem->GetDurPoints();
				FLOAT fMax = (FLOAT)pCurItem->GetMaxDurPoint();
				FLOAT V	   = (FLOAT)pCurItem->GetPrice();
				if(fCur != fMax)
				{
					//if(fCur<fMax/3)
					//	MoneySpent += (INT)(7*V*V*fCur*6/(4*fMax)+V*V);
					//else if(fCur>=fMax/3&&fCur<fMax*2/3)
					//	MoneySpent += (INT)(V*V*fCur*6/fMax+V*V*3/4);
					//else if(fCur>=fMax*2/3&&fCur<fMax)
					//	MoneySpent += (INT)(V*V*fCur*6/(4*fMax)+V*V/4);
					MoneySpent += (INT) ( ((REPAIR_SPEND * (1- fCur/fMax) * V) * pShop->m_nRepairSpend) + 0.99);


					if(MoneyHave>=MoneySpent)
					{//可以修
						pHuman->SetMoney(MoneyHave - MoneySpent);
						
						MONEY_LOG_PARAM	MoneyLogParam;
						MoneyLogParam.CharGUID	=	pHuman->GetGUID();
						MoneyLogParam.OPType	=	MONEY_SHOP_REPAIR;
						MoneyLogParam.Count		=	MoneySpent;
						MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
						MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
						MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
						SaveMoneyLog(&MoneyLogParam);

						HumanItemLogic::SetBagItemDur(pHuman,(UINT)BagIndex,(INT)fMax);
						ItemList[0].IsIn		=	GCShopRepair::BAG;					
						ItemList[0].BagIndex	=	(BYTE)BagIndex;
						Msg.SetAll(0);
						Msg.SetReListNum(1);
						Msg.SetReList(ItemList);
						pHuman->GetPlayer()->SendPacket(&Msg);
					}
					else
					{//没钱还捣乱
						GCShopBuy Msg ;
						Msg.SetBuyOk((BYTE)GCShopBuy::BUY_MONEY_FAIL);
						pHuman->GetPlayer()->SendPacket( &Msg ) ;
						g_pLog->FastSaveLog( LOG_FILE_1, "CGShopRepairHandler BUY_MONEY_FAIL") ;
						return PACKET_EXE_CONTINUE ;
					}
				}
			}
			else
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGShopRepairHandler BagIndex = %d", BagIndex) ;
				return PACKET_EXE_CONTINUE ;
			}
		}
	}
	else if(Opt == CGShopRepair::FromEquip)
	{
		pCurItem = HumanItemLogic::GetEquip(pHuman,(HUMAN_EQUIP)BagIndex);

		if(!pCurItem->IsEmpty())
		{//有东西
			FLOAT fCur = (FLOAT)pCurItem->GetDurPoints();
			FLOAT fMax = (FLOAT)pCurItem->GetMaxDurPoint();
			FLOAT V	   = (FLOAT)pCurItem->GetPrice();

			{
				if(pCurItem->GetRequireLevel()>pShop->m_nRepairLevel)
				{
					g_pLog->FastSaveLog( LOG_FILE_1, "CGShopRepairHandler Npc have no the Repair Levle") ;
					return PACKET_EXE_CONTINUE;
				}
				BOOL bCanRepair = TRUE;
				switch( pShop->m_nRepairType )
				{
				case SHOP_All:
					{
					}
					break;
				case SHOP_DEFENCE:		//防具
					{
						if( pCurItem->GetEquipPoint() != HEQUIP_CAP &&
							pCurItem->GetEquipPoint() != HEQUIP_ARMOR &&
							pCurItem->GetEquipPoint() != HEQUIP_CUFF &&
							pCurItem->GetEquipPoint() != HEQUIP_BOOT )
						{
							bCanRepair = FALSE;
						}
					}
					break;
				case SHOP_ADORN:		//饰物
					{
						if( pCurItem->GetEquipPoint() != HEQUIP_SASH &&
							pCurItem->GetEquipPoint() != HEQUIP_RING &&
							pCurItem->GetEquipPoint() != HEQUIP_NECKLACE &&
							pCurItem->GetEquipPoint() != HEQUIP_RIDER)
						{
							bCanRepair = FALSE;
						}
					}
					break;
				case SHOP_WEAPON:		//武器
					{
						if( pCurItem->GetEquipPoint() != HEQUIP_WEAPON )
						{
							bCanRepair = FALSE;
						}
					}
					break;
				default:
					{
						bCanRepair = FALSE;
					}
					break;
				}

				if(bCanRepair == FALSE)
				{
					g_pLog->FastSaveLog( LOG_FILE_1, "CGShopRepairHandler Npc have no the Repair type") ;
					return PACKET_EXE_CONTINUE;
				}
			}


			if(fCur != fMax)
			{
				//if(fCur<fMax/3)
				//	MoneySpent += (INT)(7*V*V*fCur*6/(4*fMax)+V*V);
				//else if(fCur>=fMax/3&&fCur<fMax*2/3)
				//	MoneySpent += (INT)(V*V*fCur*6/fMax+V*V*3/4);
				//else if(fCur>=fMax*2/3&&fCur<fMax)
				//	MoneySpent += (INT)(V*V*fCur*6/(4*fMax)+V*V/4);

				MoneySpent += (INT) ( ((REPAIR_SPEND * (1- fCur/fMax) * V) * pShop->m_nRepairSpend) + 0.5);

				if(MoneyHave>=MoneySpent)
				{//可以修
					pHuman->SetMoney(MoneyHave - MoneySpent);
					MONEY_LOG_PARAM	MoneyLogParam;
					MoneyLogParam.CharGUID	=	pHuman->GetGUID();
					MoneyLogParam.OPType	=	MONEY_SHOP_REPAIR;
					MoneyLogParam.Count		=	MoneySpent;
					MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
					MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
					MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
					SaveMoneyLog(&MoneyLogParam);

					HumanItemLogic::SetEquipDur(pHuman,(HUMAN_EQUIP)BagIndex,(INT)fMax);
					ItemList[0].IsIn		=	GCShopRepair::EQUIP;					
					ItemList[0].BagIndex	=	(BYTE)BagIndex;
					Msg.SetAll(0);
					Msg.SetReListNum(1);
					Msg.SetReList(ItemList);
					pHuman->GetPlayer()->SendPacket(&Msg);
				}
				else
				{//没钱还捣乱
					GCShopBuy Msg ;
					Msg.SetBuyOk((BYTE)GCShopBuy::BUY_MONEY_FAIL);
					pHuman->GetPlayer()->SendPacket( &Msg ) ;
					g_pLog->FastSaveLog( LOG_FILE_1, "CGShopRepairHandler BUY_MONEY_FAIL") ;
					return PACKET_EXE_CONTINUE ;
				}
			}
		}

	}


	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
