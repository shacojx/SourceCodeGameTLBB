#include "stdafx.h"
// CGShopSellHandler.cpp

#include "CGShopSell.h"
#include "GCShopSell.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "Log.h"
#include "ShopManager.h"
#include "GCShopSoldList.h"
#include "HumanItemLogic.h"

uint CGShopSellHandler::Execute( CGShopSell* pPacket, Player* pPlayer )
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

	BYTE	m_nBagIndex	=	pPacket->GetBagIndex();		//包中的位置
	UINT	UniqueID	=	pPacket->GetUniqueID();

	SceneID_t	SceneID =	UniqueID>>16;
	ObjID_t		NpcObjID=	UniqueID&0x00ff;

	if(SceneID != pScene->SceneID())
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGShopSellHandler Illegal scene ObjName = %s", pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ; 
	}

	//距离判定
	Obj* pNpcObj = (Obj*) (pScene->GetObjManager()->GetObj(NpcObjID));
	if(pNpcObj == NULL)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGShopSellHandler Illegal Obj ObjName = %s", pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ; 
	}

	FLOAT fDist = MySqrt(pNpcObj->getWorldPos(), pHuman->getWorldPos());

	if(fDist>MAX_NPC_DISTANCE)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGShopSellHandler Out Of Range ObjName = %s", pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE ; 
	}

	Item*		 pCurrentItem = HumanItemLogic::GetItem(pHuman,m_nBagIndex);

	if(!pCurrentItem||ConvertSerial2ItemType(pCurrentItem->GetItemTableIndex()).isNull())
	{
		//背包索引不存在
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR:CGShopSellHandler m_nBagIndex=%d PlayerName=s%",
			m_nBagIndex,  pHuman->GetName()) ;
		return PACKET_EXE_CONTINUE;
	}

	//判定这个商人是不是收购这个物品
	_SHOP*		pShop	=	pHuman->Shop( )	;

	//商店不存在
	if(!pShop)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "ERROR:CGShopSellHandler Npc Shop Lose");
		return PACKET_EXE_CONTINUE;
	}

	//查询收购等级限制
	if( pCurrentItem->GetItemClass() == ICLASS_EQUIP )
	{
		if( pCurrentItem->GetRequireLevel() > pShop->m_nBuyLevel )
		{
			return PACKET_EXE_CONTINUE;
		}
	}
	else
	{
		if( pCurrentItem->GetItemLevel() > pShop->m_nBuyLevel )
		{
			return PACKET_EXE_CONTINUE;
		}
	}

	BOOL bCanBuy = TRUE;
	if(pShop->m_nBuyType != SHOP_All)
	{
		switch(pShop->m_nBuyType)
		{
		case SHOP_All:
			{
			}
			break;
		case SHOP_DEFENCE:		//防具
			{
				if( ICLASS_EQUIP == pCurrentItem->GetItemClass())
				{
					if( pCurrentItem->GetEquipPoint() != HEQUIP_CAP &&
						pCurrentItem->GetEquipPoint() != HEQUIP_ARMOR &&
						pCurrentItem->GetEquipPoint() != HEQUIP_CUFF &&
						pCurrentItem->GetEquipPoint() != HEQUIP_BOOT )
					{
						bCanBuy = FALSE;
					}
				}
				else
				{
					bCanBuy = FALSE;
				}
			}
			break;
		case SHOP_ADORN:		//饰物
			{
				if( ICLASS_EQUIP == pCurrentItem->GetItemClass())
				{
					if( pCurrentItem->GetEquipPoint() != HEQUIP_SASH &&
						pCurrentItem->GetEquipPoint() != HEQUIP_RING &&
						pCurrentItem->GetEquipPoint() != HEQUIP_NECKLACE &&
						pCurrentItem->GetEquipPoint() != HEQUIP_RIDER)
					{
						bCanBuy = FALSE;
					}
				}
				else
				{
					bCanBuy = FALSE;
				}
			}
			break;
		case SHOP_WEAPON:		//武器
			{
				if( ICLASS_EQUIP == pCurrentItem->GetItemClass())
				{
					if( pCurrentItem->GetEquipPoint() != HEQUIP_WEAPON )
					{
						bCanBuy = FALSE;
					}
				}
				else
				{
					bCanBuy = FALSE;
				}
			}
			break;
		case SHOP_FOOD:			//食物
			{
				if( ICLASS_COMITEM != pCurrentItem->GetItemClass())
				{
					bCanBuy = FALSE;
				}
			}
			break;
		case SHOP_MATERIAL:		//材料
			{
				if( ICLASS_MATERIAL != pCurrentItem->GetItemClass())
				{
					bCanBuy = FALSE;
				}
			}
			break;
		case SHOP_COMITEM:		//药品
			{
				if( ICLASS_COMITEM != pCurrentItem->GetItemClass())
				{
					bCanBuy = FALSE;
				}
			}
			break;
		default:
			{
				bCanBuy = FALSE;
				break;
			}
		}
	}
	
	//是否返回失败消息？？？
	if(bCanBuy == FALSE)
	{
		return PACKET_EXE_CONTINUE;
	}

	INT   iPrice = ShopMgr::ConvertItemType2Money(ConvertSerial2ItemType(pCurrentItem->GetItemTableIndex()));
	iPrice *= pCurrentItem->GetLayedNum();
	//2006-4-21 
	//玩家向商店出售已经鉴定过的物品或者无需鉴定的物品按照基础价格*系数A(=1/3) 计算；
	//若出售未鉴定物品，价格应该更低，设此价格系数为B(=1/10)，这个有待于调整，请在config中开放调整A,B的接口。
	//耐久衰减系数D=当前耐久/最大耐久
	//修理失败衰减次数F=1/（修理失败次数+1）
	//因此最终的出售价格为V=基础价格B*A*D*F
	//再乘以商店得价格比例

	FLOAT fCur			= 1;
	FLOAT fMax			= 1;
	INT	  iFailTimes	= 0;

	if(pCurrentItem->GetItemClass() == ICLASS_EQUIP)
	{
		fCur		= (FLOAT)pCurrentItem->GetDurPoints();
		fMax		= (FLOAT)pCurrentItem->GetMaxDurPoint();
		iFailTimes	= pCurrentItem->GetFailTimes();
	}

	if(pCurrentItem->GetItemIdent())
	{
		iPrice = (INT)( ((FLOAT)iPrice/(FLOAT)3.0) * (fCur/fMax) * ((FLOAT)1/(FLOAT)(iFailTimes+1)) * (pShop->m_scale) );
	}
	else
	{
		iPrice = (INT)( ((FLOAT)iPrice/(FLOAT)10.0) * (fCur/fMax) * ((FLOAT)1/(FLOAT)(iFailTimes+1)) * (pShop->m_scale) );
	}
	
	//备份一下，一会儿放到购回列表里
	_ITEM tempitem;
	pCurrentItem->SaveValueTo(&tempitem);

	//备份到回购列表中
	pHuman->AddToSoldList(m_nBagIndex, iPrice);

	//给钱
	pHuman->SetMoney( pHuman->GetMoney() + iPrice);

	MONEY_LOG_PARAM	MoneyLogParam;
	MoneyLogParam.CharGUID	=	pHuman->GetGUID();
	MoneyLogParam.OPType	=	MONEY_SHOP_SELL;
	MoneyLogParam.Count		=	iPrice;
	MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
	MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
	MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
	SaveMoneyLog(&MoneyLogParam);
		
	//把这个回购商品显示到界面
	GCShopSoldList::_MERCHANDISE_ITEM	SoldItem;
	SoldItem.item_data = tempitem;
	GCShopSoldList	MsgSold;
	MsgSold.SetMerchadiseNum(1);
	MsgSold.SetMerchadiseList(&SoldItem);
	pHuman->GetPlayer()->SendPacket( &MsgSold );

	GCShopSell	MsgSell;
	pHuman->GetPlayer()->SendPacket( &MsgSell );
	
	g_pLog->FastSaveLog( LOG_FILE_1, "CGShopSellHandler m_nBagIndex=%d ",
		m_nBagIndex  ) ;

	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
