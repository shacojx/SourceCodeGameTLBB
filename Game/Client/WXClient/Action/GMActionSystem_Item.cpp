#include "StdAfx.h"
#include "GMActionSystem_Item.h"
#include "..\Object\Item\Obj_Item.h"
#include "..\Object\Item\Obj_Item_Equip.h"
#include "..\DataPool\GMDataPool.h"
#include "..\DBC\GMDataBase.h"
#include "..\Procedure\GameProcedure.h"
#include "..\Interface\GMGameInterface.h"
#include "..\NetWork\NetManager.h"
#include "CGBankAddItem.h"
#include "CGBankRemoveItem.h"
#include "CGBankSwapItem.h"
#include "CGPlayerShopApply.h"
#include "ExchangeMsgDefine.h"
#include "CGExchangeSynchItemII.h"
#include "TDUISystem.h"
#include "..\Event\GMEventSystem.h"
#include "CGUnEquip.h"									// 使用卸下装备消息
#include "CGPackage_SwapItem.h"
#include "..\object\item\Obj_Item_Equip.h"
#include "..\object\item\Obj_Item_Gem.h"
#include "..\object\item\Obj_Item_Medicine.h"

#include "..\Object\ObjectManager.h"
#include "..\Object\Object.h"
#include "..\Object\Character\Obj_PlayerMySelf.h"
#include "..\DataPool\GMDP_CharacterData.h"

#include "CGStallRemoveItem.h"

#include "CGDiscardEquip.h"
#include "CGDiscardItem.h"

#include "..\Sound\GMSoundSystem.h"

#include "..\DBC\GMDataBase.h"
#include "GameStruct_Skill.h"

#include "..\Input\GMInputSystem.h"
#include "CGItemSynch.h"
#include "CGAskItemInfo.h"
#include "CGUseItem.h"
#include "CGShopBuy.h"


#include "GMActionSystem_MouseCmd.h"

CActionItem_Item::CActionItem_Item(INT nID)
	:CActionItem(nID)
{
}

CActionItem_Item::~CActionItem_Item()
{
}

VOID CActionItem_Item::Update_Item(tObject_Item* pItem)
{
	TDAssert(pItem);

	//引用
	m_idItemImpl = pItem->GetID();
	//名称
	m_strName = pItem->GetName();
	//图标
	if ( pItem->GetIconName() != NULL )
		m_strIconName = pItem->GetIconName();
	//通知UI
	UpdateToRefrence();
}

tObject_Item* CActionItem_Item::GetItemImpl(VOID)
{
	return CObject_Item::FindItem(m_idItemImpl);
}

LPCTSTR CActionItem_Item::GetType_String(VOID)
{
	if(!GetItemImpl()) return "";

	switch( GetItemImpl()->GetTypeOwner() )
	{
	case tObject_Item::IO_ITEMBOX:
		return NAMETYPE_LOOTITEM;
	case tObject_Item::IO_BOOTH:
		return NAMETYPE_BOOTITEM;
	case tObject_Item::IO_BOOTH_CALLBACK:
		return NAMETYPE_CALLBACK;
	case tObject_Item::IO_MYSELF_BANK:
		return NAMETYPE_BANKITEM;
	case tObject_Item::IO_MYSELF_EQUIP:
		return NAMETYPE_EQUIP;
	case tObject_Item::IO_MYEXBOX:
		return NAMETYPE_EXCHANGESELF;
	case tObject_Item::IO_OTHEREXBOX:
		return NAMETYPE_EXCHANGEOTHER;
	case tObject_Item::IO_MISSIONBOX:
		return NAMETYPE_MISSIONREFER;
	case tObject_Item::IO_MYSTALLBOX:
		return NAMETYPE_STALLSELF;
	case tObject_Item::IO_OTSTALLBOX:
		return NAMETYPE_STALLOTHER;
	case tObject_Item::IO_QUESTVIRTUALITEM:
		return NAMETYPE_VIRTUALITEM;
	case tObject_Item::IO_PLAYEROTHER_EQUIP:
		return NAMETYPE_OTHEREQUIP;
	case tObject_Item::IO_PS_OTHERBOX:
		return NAMETYPE_PS_OTHER;
	case tObject_Item::IO_GEM_SEPARATE:
		return NAMETYPE_SPLITGEM;
	case tObject_Item::IO_PS_SELFBOX:
		return NAMETYPE_PS_SELF;
	case tObject_Item::IO_CITY_RESEARCH:
		return NAMETYPE_CITY_RESEARCH;
	case tObject_Item::IO_CITY_SHOP:
		return NAMETYPE_CITY_SHOP;
	case tObject_Item::IO_TRANSFER_ITEM:
		return NAMETYPE_TRANSFER_ITEM;
	case tObject_Item::IO_QUESTUI_DEMAND:
		return NAMETYPE_QUESTUI_DEMAND;
	case tObject_Item::IO_QUESTUI_REWARD:
		return NAMETYPE_QUESTUI_REWARD;
	default:
		return NAMETYPE_PACKAGEITEM;
	}
}

INT	CActionItem_Item::GetDefineID(VOID)
{
	tObject_Item* pItem = GetItemImpl();
	if(!pItem) return -1;

	return pItem->GetIdTable();
}

INT CActionItem_Item::GetNum(VOID)
{
	tObject_Item* pItem = GetItemImpl();

	if(pItem) 
		return pItem->GetNumber();
	else
	{
		//AxTrace(0, 1, "InvalidItem:%d", m_idItemImpl);
		return -1;
	}
}

LPCTSTR CActionItem_Item::GetDesc(VOID)
{
	tObject_Item* pItem = GetItemImpl();
	if(!pItem) return NULL;

	LPCTSTR szExtraInfo = pItem->GetExtraDesc();
	return szExtraInfo;//

}

INT CActionItem_Item::GetPosIndex(VOID)
{
	tObject_Item* pItem = GetItemImpl();
	if(!pItem) return -1;

	return pItem->GetPosIndex();
}

VOID CActionItem_Item::DoSubAction(VOID)
{
	CObject_Item* pItem = (CObject_Item*)GetItemImpl();
	if(!pItem) return;

	//被包里的物品
	if(	pItem->GetTypeOwner() == tObject_Item::IO_MYSELF_PACKET)
	{
		//按住Shift
		if( CInputSystem::GetMe()->IsKeyDown(KC_LSHIFT) || CInputSystem::GetMe()->IsKeyDown(KC_RSHIFT) )
		{
			//判定这个物品是否具备拆分的条件,数量多于一个可以拆分
			if(pItem->GetNumber() > 1)
			{
				//先将可能存在的已经锁定的物品解除锁定
				INT nCon = CDataPool::GetMe()->Split_GetItemConta();
				INT nPos = CDataPool::GetMe()->Split_GetItemPos();
				switch( nCon ) 
				{
				case tObject_Item::IO_MYSELF_PACKET:				// 来自玩家的背包
					{
						tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nPos);

						if(pItem)
						{
							pItem->SetLock(FALSE);
						}
					}
					break;
				case tObject_Item::IO_MYSELF_BANK:					// 玩家的银行
					{
						tObject_Item* pItem = CDataPool::GetMe()->UserBank_GetItem(nPos);

						if(pItem)
						{
							pItem->SetLock(FALSE);
						}
					}
					break;
				default:
					break;
				}

				//记录拆分物品的位置到数据池
				CDataPool::GetMe()->Split_SetItemConta(pItem->GetTypeOwner());
				CDataPool::GetMe()->Split_SetItemPos(this->GetPosIndex());
				CDataPool::GetMe()->Split_SetSum(pItem->GetNumber());

				//打开拆分对话框
				CEventSystem::GetMe()->PushEvent(GE_SHOW_SPLIT_ITEM);		

				// 锁定这个物品
				pItem->SetLock(TRUE);

				//通知背包锁定状态
				CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
			}
			else
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"此物品不可拆分");
			}
			return;
		}

		//商品界面是开的
		if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Shop"))
		{
			//在修理状态
			if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Repair::GetMe())
			{
				//判以是不是装备,只有装备可以修理
				if(pItem->GetItemClass() == ICLASS_EQUIP)
				{
					if( pItem->GetItemDur() < pItem->GetItemMaxDur() )
					{
						CGameProcedure::s_pGameInterface->Booth_Repair(0, pItem->GetPosIndex());
					}
					else
					{
						//操作失败音效
						CSoundSystemFMod::_PlayUISoundFunc(47);
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "此物品不需要修理");
					}
				}
				else
				{
					//操作失败音效
					CSoundSystemFMod::_PlayUISoundFunc(47);
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"此物品不需要修理");
				}

				return;
			}
		}

		
		switch(pItem->GetItemClass())
		{
		case ICLASS_EQUIP:
			{
				if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Identify::GetMe())
				{
					// 使用鉴定卷轴2006-3-30
					UseIdentify();
					return; 
				}
				break;
			}
		case ICLASS_COMITEM:
			{
				int iType		= pItem->GetItemTableType();
				int iQuanlity	= pItem->GetItemTableQuality();

				if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Identify::GetMe())
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "物品不需要鉴定");
					return; 
				}

				if(5 == iQuanlity)
				{
					if((iType >= COMITEM_WPIDENT)&&(iType <= COMITEM_NCIDENT))				
					{
						PrepareUseIdentify();
					}
					else
					{
						if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Identify::GetMe())
						{
							// 使用鉴定卷轴2006-3-30
							CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "只能鉴定武器和装备");
							return; 
						}
					}
				}
				break;
			}
		default:
			{
				if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Identify::GetMe())
				{
					// 使用鉴定卷轴2006-3-30
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "只能鉴定武器和装备");
					return; 
				}
				break;
			}
		
		}
	}
	
}

VOID CActionItem_Item::DoAction(VOID)
{
	CObject_Item* pItem = (CObject_Item*)GetItemImpl();
	if(!pItem) return;

	//先检测“Shift”是不是按下状态，如果是，那么先处理拆分的操作
	if( CInputSystem::GetMe()->IsKeyDown(KC_LSHIFT) || CInputSystem::GetMe()->IsKeyDown(KC_RSHIFT) )
	{
		//修改：目前银行是是DoAction实现拆分，背包使用DoSubAction进行拆分
		if( pItem->GetTypeOwner() == tObject_Item::IO_MYSELF_BANK )
		{
			//判定这个物品是否具备拆分的条件,数量多于一个可以拆分
			if(pItem->GetNumber() > 1)
			{
				//先将可能存在的已经锁定的物品解除锁定
				INT nCon = CDataPool::GetMe()->Split_GetItemConta();
				INT nPos = CDataPool::GetMe()->Split_GetItemPos();
				switch( nCon ) 
				{
				case tObject_Item::IO_MYSELF_PACKET:				// 来自玩家的背包
					{
						tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nPos);

						if(pItem)
						{
							pItem->SetLock(FALSE);
						}
					}
					break;
				case tObject_Item::IO_MYSELF_BANK:					// 玩家的银行
					{
						tObject_Item* pItem = CDataPool::GetMe()->UserBank_GetItem(nPos);

						if(pItem)
						{
							pItem->SetLock(FALSE);
						}
					}
					break;
				default:
					break;
				}

				//记录拆分物品的位置到数据池
				CDataPool::GetMe()->Split_SetItemConta(pItem->GetTypeOwner());
				CDataPool::GetMe()->Split_SetItemPos(this->GetPosIndex());
				CDataPool::GetMe()->Split_SetSum(pItem->GetNumber());

				//打开拆分对话框
				CEventSystem::GetMe()->PushEvent(GE_SHOW_SPLIT_ITEM);

				// 锁定这个物品
				pItem->SetLock(TRUE);

				//通知背包锁定状态
				CEventSystem::GetMe()->PushEvent(GE_UPDATE_BANK);
			}
			else
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"此物品不可拆分");
			}
		}
		return;
	}

	STRING strTemp = "";

	//根据物品所在的归属
	switch(pItem->GetTypeOwner())
	{
		//玩家的摆摊界面（卖）
	case tObject_Item::IO_MYSTALLBOX:			//自己的摊位盒
		{
			//直接通知界面，自己选中的编号
			CEventSystem::GetMe()->PushEvent(GE_STALL_SALE_SELECT,this->GetPosIndex());
		}
		break;
		//玩家的摆摊界面（买）
	case tObject_Item::IO_OTSTALLBOX:			//对方的摊位盒
		{
			//直接通知界面，自己选中的编号
			CEventSystem::GetMe()->PushEvent(GE_STALL_BUY_SELECT,this->GetPosIndex());
		}
		break;
		//玩家交任务的对话框中
	case tObject_Item::IO_MISSIONBOX:
		{
			//解除锁定
			if(pItem)
			{
				INT nBagIndex = CDataPool::GetMe()->UserBag_GetItemIndexByGUID(pItem->GetGUID());

				tObject_Item* pBagItem = CDataPool::GetMe()->UserBag_GetItem(nBagIndex);
				pBagItem->SetLock(FALSE);
			}

			//在任务BOX中的Item点击一次就让它消失
			CDataPool::GetMe()->MissionBox_SetItem(this->GetPosIndex(), NULL);

			CActionSystem::GetMe()->UserMission_Update();
			//发送更新消息
			CEventSystem::GetMe()->PushEvent(GE_UPDATE_REPLY_MISSION);
			//更新背包
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
		}
		break;
		//玩家的银行租赁箱子中
	case tObject_Item::IO_MYSELF_BANK:
		{
			CGBankRemoveItem msg;
			msg.SetToType(CGBankRemoveItem::BAG_POS);
			msg.SetIndexFrom(this->GetPosIndex());
			msg.SetIndexTo(255);

			CNetManager::GetMe()->SendPacket(&msg);
		}
		break;
		//掉落箱中的物品
	case tObject_Item::IO_ITEMBOX:
		{
			//捡起
			if(pItem->GetItemClass() == ICLASS_TASKITEM)
			{
				INT idTable = pItem->GetIdTable();
				INT nCount = CDataPool::GetMe()->UserBag_CountItemByIDTable(idTable);

				DBC_DEFINEHANDLE(s_pItem_Medic, DBC_ITEM_MEDIC);
				//搜索纪录
				const _DBC_ITEM_MEDIC* pMedic = (const _DBC_ITEM_MEDIC*)s_pItem_Medic->Search_Index_EQU(idTable);
				if( pMedic->nMaxHold != INVALID_ID && nCount >= pMedic->nMaxHold )
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "不能拾取这样物品。");				
					break;
				}
			}
			CGameProcedure::s_pGameInterface->ItemBox_PickItem(pItem);
			break;
		}
		break;
		//玩家自己身上的包中
	case tObject_Item::IO_MYSELF_PACKET:
		{
			DoAction_Packet();
		}
		break;

	case tObject_Item::IO_BOOTH:
		//商人的货架,发送交易请求
		{
			//	变成修理指针后，不能再购买商品
			if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Repair::GetMe())
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"不能修理此物品");
				break;
			}

			if( CDataPool::GetMe()->Booth_GetShopType() == 0 )//虚拟货币
			{
				if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Money() >= pItem->GetItemPrice())
				{
					CGameProcedure::s_pGameInterface->Booth_BuyItem( pItem );
				}
				else
				{
					if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Money() >= pItem->GetItemPrice())
					{
						CGameProcedure::s_pGameInterface->Booth_BuyItem( pItem );
					}
					else
					{
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"金钱不够");
					}
				}
			}
			else if( CDataPool::GetMe()->Booth_GetShopType() == 1 )//元宝
			{
				if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_RMB() >= pItem->GetItemPrice())
				{
					CGameProcedure::s_pGameInterface->Booth_BuyItem( pItem );
				}
				else
				{
					if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_RMB() >= pItem->GetItemPrice())
					{
						CGameProcedure::s_pGameInterface->Booth_BuyItem( pItem );
					}
					else
					{
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"元宝不够");
					}
				}
			}
		}
		break;
	case tObject_Item::IO_BOOTH_CALLBACK:		//回购物品 
		{
			if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Repair::GetMe())
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"不能修理此物品");
				break;
			}

			if(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Money() >= 
				(INT)(CDataPool::GetMe()->Booth_GetSoldPrice(this->GetPosIndex())))
			{
				_ITEM_GUID Guid;
				tObject_Item::ITEM_GUID temp;
				temp.m_idUnion = 0;

				pItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
				Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
				Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
				Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;

				CGShopBuy msg;
				msg.SetUniqueID(CDataPool::GetMe()->Booth_GetShopUniqueId());
				msg.SetIndex(pItem->GetPosIndex()+200);
				msg.SetItemGUID(Guid);
				msg.SetSerialNum(CDataPool::GetMe()->Booth_GetSerialNum());

				CNetManager::GetMe()->SendPacket( &msg );
			}
			else
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"金钱不够");
			}
		}
		break;
	case tObject_Item::IO_MYEXBOX:
		// 交易界面的自己的物品筐里头
		{
			//发送拖动物品的消息
			CGExchangeSynchItemII msg;
			msg.SetOpt(EXCHANGE_MSG::OPT_REMOVEITEM);
			//msg.SetFromType(EXCHANGE_MSG::);
			msg.SetFromIndex(this->GetPosIndex());
			msg.SetToType(EXCHANGE_MSG::POS_BAG);
			msg.SetToIndex(0);						//由服务器查找
			CNetManager::GetMe()->SendPacket(&msg);
		}
		break;

	case tObject_Item::IO_MYSELF_EQUIP :
		// 如果action item在装备栏上.
		{
			if(!CDataPool::GetMe()->Booth_IsClose())
			{
				//在修理状态
				if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Repair::GetMe())
				{
					//判以下是不是装备,只有装备可以修理
					if(pItem->GetItemClass() == ICLASS_EQUIP)
					{
						if( pItem->GetItemDur() < pItem->GetItemMaxDur() )
						{
							CGameProcedure::s_pGameInterface->Booth_Repair(0, pItem->GetPosIndex(), FALSE);
						}
						else
						{
							CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"此物品不需要修理");
						}
					}
				}
			}
			else
			{
				CGUnEquip msg;
				CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)pItem;
				if(pItemEquip->GetItemClass() != ICLASS_EQUIP)
				{
					return;
				}

				// 发送卸下装备消息.
				msg.setEquipPoint((BYTE)(pItemEquip->GetItemType()));
				msg.setBagIndex(-1);

				CNetManager::GetMe()->SendPacket( &msg );
			}
		}
		break;
	case tObject_Item::IO_PS_SELFBOX:
		{
			//选中的物品需要在进行换页操作后清除
			//玩家商店（自己的）
			// 记录当前选中的物品
			INT nConIndex = CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
			CDataPool::GetMe()->PlayerShop_SetMySelectConTa(nConIndex);
			CDataPool::GetMe()->PlayerShop_SetMySelectPos(this->GetPosIndex()%20);

			CEventSystem::GetMe()->PushEvent(GE_PS_SELF_SELECT);
		}
		break;
	case tObject_Item::IO_PS_OTHERBOX:
		{
			//玩家商店（别人的）
			CDataPool::GetMe()->PlayerShop_SetOtSelectPos(this->GetPosIndex()%20);
						
			CEventSystem::GetMe()->PushEvent(GE_PS_OTHER_SELECT);
		}
		break;
	default:
		break;

	}
}

BOOL CActionItem_Item::IsValidate()
{
	tObject_Item* pItem = GetItemImpl();
	if(!pItem) return FALSE;

	if( pItem->GetTypeOwner() == tObject_Item::IO_BOOTH 
		||pItem->GetTypeOwner() == tObject_Item::IO_BOOTH_CALLBACK
		||pItem->GetTypeOwner() == tObject_Item::IO_CITY_SHOP)
	{
		return (pItem->GetMax() == 0)? FALSE : TRUE;
	}

	return TRUE;
}

VOID CActionItem_Item::Update(tActionReference* pRef)
{
	tObject_Item* pItem = GetItemImpl();
	if(!pItem) return;

	if( pItem->GetTypeOwner() == tObject_Item::IO_BOOTH 
		||pItem->GetTypeOwner() == tObject_Item::IO_BOOTH_CALLBACK
		||pItem->GetTypeOwner() == tObject_Item::IO_CITY_SHOP)
	{
		//刷新最大数量
		INT nMaxNumber = pItem->GetMax();
		if(nMaxNumber > 1)
		{
			pRef->Enable();
			CHAR szTemp[32];
			_snprintf(szTemp, 32, "%d", nMaxNumber);
			pRef->SetCornerChar(tActionReference::ANP_TOPLEFT, szTemp);
		}
		else if(nMaxNumber == 0)
		{
			pRef->Disable();
			pRef->SetCornerChar(tActionReference::ANP_TOPLEFT, "");
		}
		else
		{
			pRef->Enable();
			pRef->SetCornerChar(tActionReference::ANP_TOPLEFT, "");
		}
	}
}

// 拖动结束
VOID CActionItem_Item::NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName)
{
	//拖动到表示销毁的地方
	if( bDestory )
	{
		DestoryItem(szSourceName);
		return;
	}
	//拖动到空白地方
	if(!szTargetName || szTargetName[0]=='\0' || GetItemImpl()==0)
	{
		return;
	}

	CHAR cSourceName = szSourceName[0];
	CHAR cTargetType = szTargetName[0];
	INT nOldTargetId = -1;
	INT nIndex =-1;
	//如果是快捷栏，不能往其他地方拖，除了自己本身。
	if(  cSourceName == 'M' && 
		 cTargetType != 'M' )
		return;

	switch(cTargetType)
	{
	case 'S':		//摆摊界面
		{
			INT nIndex = szTargetName[1]-'0';
			nIndex = nIndex*10 + szTargetName[2]-'0';

			//检查是不是由背包界面拖过来的(只能接受玩家背包内的物品)
			if(this->GetItemImpl()->GetTypeOwner() == tObject_Item::IO_MYSELF_PACKET)
			{
				if(this->GetPosIndex() >= 40)
				{
					//任务物品
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "不能上架任务物品");
					break;
				}
				// 判定物品是否已经绑定
				tObject_Item* pItem = this->GetItemImpl();
				if(pItem)
				{
					if(pItem->GetItemBindInfo() == 1)		//==1 是绑定
					{
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "此物品已经绑定");
						break;
					}
				}

				// 将拖动的物品设为当前的物品
				CDataPool::GetMe()->MyStallBox_SetCurSelectItem(this->GetItemImpl());
				
				// 弹出输入出售价格的对话框
				CEventSystem::GetMe()->PushEvent(GE_TOGLE_INPUT_MONEY, "price");
			}
		}
		break;
	case 'Q':		//任务提交界面(MissionReply)
		{
			INT nIndex = szTargetName[1]-'0';

			//根据物品的来源决定发送什么消息（暂时只支持背包）
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			switch( nTypeOwner ) 
			{
			case tObject_Item::IO_MYSELF_PACKET:			//来自玩家的背包
				{
					INT nGUID = this->GetItemImpl()->GetIdTable();
					//克隆物品
					tObject_Item* pNewItem  = CObject_Item::NewItem( this->GetItemImpl()->GetIdTable());
					((CObject_Item*)pNewItem)->Clone((CObject_Item*)this->GetItemImpl());

					//在这里保存直接通知界面添加按钮，不给服务器发消息，
					CDataPool::GetMe()->MissionBox_SetItem(GetPosIndex(), pNewItem);

					// 锁定背包中对应的这个物品
					tObject_Item* pMissionItem = this->GetItemImpl();
					pMissionItem->SetLock(TRUE);

					CActionSystem::GetMe()->UserMission_Update();
					//发送更新消息
					CEventSystem::GetMe()->PushEvent(GE_UPDATE_REPLY_MISSION);
					CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
				}
				break;
			default:
				break;
			}				
		}
		break;
	case 'B':		//银行
		{
			INT nIndex = szTargetName[1]-'0';
			nIndex = nIndex*10 + szTargetName[2]-'0';

			//根据物品的来源决定发送什么消息
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			//任务物品不能放入银行
			if(this->GetItemImpl()->GetItemClass() == ICLASS_TASKITEM)
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "任务物品不能放入");
				return;
			}
			switch( nTypeOwner ) 
			{
			case tObject_Item::IO_MYSELF_PACKET:			//来自玩家的背包
				{
					CGBankAddItem msg;
					msg.SetFromType(CGBankAddItem::BAG_POS);
					msg.SetIndexFrom(this->GetPosIndex());
					msg.SetIndexTo(nIndex + this->GetCurBankRentBegin());
					CNetManager::GetMe()->SendPacket(&msg);
				}
				break;
			case tObject_Item::IO_MYSELF_BANK:				//来自银行内部
				{
					CGBankSwapItem msg;
					msg.SetFromType(CGBankSwapItem::BANK_POS);
					msg.SetIndexFrom(this->GetPosIndex());
					msg.SetIndexTo(nIndex + this->GetCurBankRentBegin());
					msg.SetToType(CGBankSwapItem::BANK_POS);
					if( msg.GetIndexFrom() == msg.GetIndexTo() )
						break;
					CNetManager::GetMe()->SendPacket(&msg);
				}
				break;
			default:
				break;
			}

		}
		break;

	case 'P':		//背包
		{
			INT nTargetIndex = atoi(szTargetName+1)-1;
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			
			switch(nTypeOwner)
			{
			//Bank -> Package
			case tObject_Item::IO_MYSELF_BANK:
				{
					CGBankRemoveItem msg;
					msg.SetToType(CGBankRemoveItem::BAG_POS);
					msg.SetIndexFrom(this->GetPosIndex());
					msg.SetIndexTo(nTargetIndex);

					CNetManager::GetMe()->SendPacket(&msg);
				}
				break;
			//Package -> Package
			case tObject_Item::IO_MYSELF_PACKET:
				{
					//同一格
					if(this->GetPosIndex() == nTargetIndex) break;

					//不同格
					CGPackage_SwapItem msg;
					msg.SetPackageIndex1(GetPosIndex());
					msg.SetPackageIndex2(nTargetIndex);

					CNetManager::GetMe()->SendPacket(&msg);
				}
				break;
			//SelfEquip -> package
			case tObject_Item::IO_MYSELF_EQUIP:
				{ 
					//发送卸下装备的消息
					CGUnEquip msg;
					msg.setEquipPoint(this->GetPosIndex());
					msg.setBagIndex((BYTE)nTargetIndex);

					CNetManager::GetMe()->SendPacket(&msg);
					break;
				}
			//playerShop -> package
			case tObject_Item::IO_PS_SELFBOX:
				{
					_ITEM_GUID Guid;
					tObject_Item::ITEM_GUID temp;
					temp.m_idUnion = 0;

					tObject_Item* pSelectItem = this->GetItemImpl();
					if(NULL == pSelectItem)
					{
						return;
					}
					
					//需要先判定这个物品是否满足移动的条件
					//查询这个物品是不是处于上架的状态
					INT nIndex = pSelectItem->GetPosIndex();
					INT nConIndex = nIndex/20;
					INT nPosition = nIndex%20;
					if(CDataPool::GetMe()->PlayerShop_GetItemOnSale(TRUE, nConIndex, nPosition))
					{
						//已经上架，不能取回
						return;
					}

					pSelectItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
					Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
					Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
					Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;

					CGItemSynch msg;
					msg.SetOpt(CGItemSynch::OPT_MOVE_ITEM_MANU);
					msg.SetFromType(CGItemSynch::POS_PLAYERSHOP);
					msg.SetToIndex(nTargetIndex);
					msg.SetToType(CGItemSynch::POS_BAG);
					msg.SetItemGUID(Guid);

					INT nPage = CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
					CGManuMoveItemFromPlayerShopToBag_t ExtraDataInfo;
					ExtraDataInfo.m_ShopGuid	= CDataPool::GetMe()->PlayerShop_GetShopID(TRUE);
					ExtraDataInfo.m_nStallIndex = (BYTE)nPage;
					ExtraDataInfo.m_uSerial		= CDataPool::GetMe()->PlayerShop_GetItemSerial(TRUE,nPage,this->GetPosIndex()%20);
					msg.SetExtraInfoLength(ExtraDataInfo.GetSize());
					msg.SetExtraInfoData((BYTE*)&ExtraDataInfo);
				
					CNetManager::GetMe()->SendPacket(&msg);
				}
				break;
			default:
				break;
			}
		}
		break;
	case 'E':		//交易界面,只能拖动到自己的（右边的）格子里头（1~6）
		{
			INT nIndex = szTargetName[1]-'0';

			//根据物品的来源决定发送什么消息
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			switch(nTypeOwner)
			{
			case tObject_Item::IO_MYSELF_PACKET:			//来自玩家的背包
				{
					// 判定物品是否已经绑定
					tObject_Item* pItem = this->GetItemImpl();
					if(pItem)
					{
						if(pItem->GetItemBindInfo() == 1)		//==1 是绑定
						{
							CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "此物品已经绑定");
							break;
						}
					}

					//验证是否可以交易
					if(!this->GetItemImpl()->Rule(tObject_Item::RULE_EXCHANGE))
					{
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, this->GetItemImpl()->RuleFailMessage(tObject_Item::RULE_EXCHANGE).c_str());
						break;
					}

					//发送拖动物品的消息
					CGExchangeSynchItemII msg;
					msg.SetOpt(EXCHANGE_MSG::OPT_ADDITEM);	
					msg.SetFromType(EXCHANGE_MSG::POS_BAG);
					msg.SetFromIndex(this->GetPosIndex());
					msg.SetToIndex(0);						//由服务器查找
					
					//检测交易栏是不是有空位
					if( !CDataPool::GetMe()->MyExBox_IsEmpty() )
					{
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"交易栏已经满了");
						break;
					}

					CNetManager::GetMe()->SendPacket(&msg);
				}
				break;
			default:
				break;
			}
			
		}
		break;
	case 'R':		//银行的界面上的租赁箱格子
		{
			INT nIndex = szTargetName[1]-'0';

			//目标箱子如果已经满了，就给个提示，不在发这个消息
			if(FALSE == CDataPool::GetMe()->UserBank_IsEmpty(nIndex))
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"目标箱子已经满了");
				break;
			}

			//根据物品的来源决定发送什么消息
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			//任务物品不能放入银行
			if(this->GetItemImpl()->GetItemClass() == ICLASS_TASKITEM)
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "任务物品不能存入银行");
				break;
			}

			switch( nTypeOwner ) 
			{
			case tObject_Item::IO_MYSELF_PACKET:			//来自玩家的背包
				{
					CGBankAddItem msg;
					msg.SetFromType(CGBankAddItem::BAG_POS);
					msg.SetIndexFrom(this->GetPosIndex());
					msg.SetIndexTo(256 - nIndex);
					
					CNetManager::GetMe()->SendPacket(&msg);
				}
				break;
			case tObject_Item::IO_MYSELF_BANK:				//玩家银行
				{
					CGBankSwapItem msg;
					msg.SetFromType(CGBankSwapItem::BANK_POS);
					msg.SetIndexFrom(this->GetPosIndex());
					msg.SetIndexTo(256 - nIndex);
					msg.SetToType(CGBankSwapItem::BANK_POS);

					//如果来自同一个租赁箱子，那么就不发消息了
					if( CActionSystem::GetMe()->GetCurBankRentBoxIndex() == nIndex )
					{
						break;
					}
					CNetManager::GetMe()->SendPacket(&msg);
				}
				break;
			default:
				break;
			}

		}
		break;
	case 'G':		//宝石合成/镶嵌界面
		{
			INT nIndex = szTargetName[1]-'0';

			//根据物品的来源决定发送什么消息
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			switch( nTypeOwner ) 
			{
			case tObject_Item::IO_MYSELF_PACKET:			//来自玩家的背包
				{
//						CDataPool::GetMe()->EnchasePage_SetItem(nIndex,this->GetItemImpl());
					if(GetItemImpl()->GetItemClass() == ICLASS_EQUIP)
					{
						if(((CObject_Item_Equip*)GetItemImpl())->GetEquipQuantity() == CObject_Item_Equip::BLUE_EQUIP && 
							((CObject_Item_Equip*)GetItemImpl())->GetEquipAttrib() == CObject_Item_Equip::EQUIP_ATTRIB_UNIDENTIFY)
						{
							STRING strTemp = "此物品需要鉴定";
							ADDNEWDEBUGMSG(strTemp);
							break;
						}
					}
					CEventSystem::GetMe()->PushEvent(GE_UPDATE_COMPOSE_GEM, nIndex, this->GetItemImpl()->GetPosIndex());
					/*
						设置FromIndex为灰色...
					*/
				}
				break;
			default:
				break;
			}

		}
		break;
	case 'T':		//宠物技能学习/还童/延长寿命/宠物驯养/宠物征友
		{
			INT nIndex = szTargetName[1]-'0';
			//根据物品的来源决定发送什么消息
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			switch( nTypeOwner ) 
			{
			case tObject_Item::IO_MYSELF_PACKET:			//来自玩家的背包
				{
					tObject_Item *pObj = this->GetItemImpl();
					if(pObj && !(pObj->IsLocked()))
					{
                        if(pObj->GetItemClass() == ICLASS_COMITEM)
						{
							CObject_Item_Medicine* pMedicine_Item = (CObject_Item_Medicine*)pObj;

							if(pMedicine_Item->GetItemTargetType() == ITEM_TARGET_TYPE_SELF_PET)
							{
								INT itemId = pMedicine_Item->GetIdTable();
								BOOL isIdOk = FALSE;
								switch(szTargetName[2]) {
								case 'S':
									//仅限技能书
									isIdOk = (itemId > ITEM_PET_SKILL_STUDY_BEGIN && itemId < ITEM_PET_SKILL_STUDY_END)?TRUE:FALSE;
									break;
								case 'R':
									//仅限还童丹
									isIdOk = (itemId >= ITEM_PET_RETURN_BABAY_BEGIN && itemId <= ITEM_PET_RETURN_BABAY_END)?TRUE:FALSE;
									break;
								case 'A':
									//仅限延长寿命道具
									isIdOk = (itemId > ITEM_PET_ADD_LIFE_BEGIN && itemId < ITEM_PET_ADD_LIFE_END)?TRUE:FALSE;
									break;
								default:
									break;
								}

								if(isIdOk)
								{
									//lock item
									pMedicine_Item->SetLock(TRUE);
									CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

									//notify petskillstudy ui
									CEventSystem::GetMe()->PushEvent(GE_UPDATE_PETSKILLSTUDY, nIndex, this->GetItemImpl()->GetPosIndex());
								}
								else
								{
									STRING strTemp = "这个物品不能用在这里";
									ADDNEWDEBUGMSG(strTemp);
								}
							}
							else
							{
								STRING strTemp = "这个物品不是给自己宠物使用的哟";
								ADDNEWDEBUGMSG(strTemp);
							}
						}
						else
						{
							STRING strTemp = "这个物品不能用在这里";
							ADDNEWDEBUGMSG(strTemp);
						}
					}
				}
				break;
			default:
				break;
			}
		}
		break;
	case 'Z':		//人物装备界面
		{
			INT nIndex = szTargetName[1]-'0';

			//根据来源，确定如何处理
			//来自其他地方，直接给予提示不能装备
			INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
			switch( nTypeOwner )
			{
			case tObject_Item::IO_MYSELF_PACKET:			//来自背包，就发送装备消息
				{
					CObject_Item* pItem = (CObject_Item*)GetItemImpl();
					if(!pItem) return;
					//判定类型
					if(pItem->GetItemClass() == ICLASS_EQUIP)
					{
						int iNeedLevel   = 0;
						int iPlayerLevel = 0;

						tObject_Item* pItem = GetItemImpl();
						if(pItem)
						{
							//验证是否可以使用
							if(!pItem->Rule(tObject_Item::RULE_USE))
							{
								CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, pItem->RuleFailMessage(tObject_Item::RULE_USE).c_str());
								break;
							}

							iNeedLevel = pItem->GetNeedLevel();
						}

						CCharacterData* pCharData = CObjectManager::GetMe()->GetMySelf()->GetCharacterData();
						if(pCharData)
						{
							iPlayerLevel = pCharData->Get_Level();
						}
						
						if(iPlayerLevel >= iNeedLevel)
						{
							CGameProcedure::s_pGameInterface->PacketItem_UserEquip( pItem->GetPosIndex() );
						}
						else
						{
							CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "等级不够");
						}
					}
					else
					{
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"此物品无法装备");
					}
				}
				break;
			case tObject_Item::IO_MYSELF_EQUIP:				//来自自己装备装备
				{
				}
				break;
			case tObject_Item::IO_MYSELF_BANK:				//来自银行，也发送装备消息
				{
				}
				break;
			case tObject_Item::IO_MYEXBOX:					//来自交易对话框
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"物品正在交易");
				}
				break;
			case tObject_Item::IO_BOOTH_CALLBACK:
			case tObject_Item::IO_CITY_SHOP:
			case tObject_Item::IO_BOOTH:					//来自商人货架
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"请先购买此物品");
				}
				break;
			case tObject_Item::IO_MYSTALLBOX:				//来自自己的摊位
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"此物品正在摆摊销售中");
				}
				break;
			default:
				break;
			}
		}
		break;
	case 'A':		//玩家商店
		{
			INT nIndex = szTargetName[1]-'0';
			nIndex = nIndex*10 + szTargetName[2]-'0';

			//检测这个格子是不是已经有物品，并且这个物品是不是已经上架
			INT nConIndex = CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
			tObject_Item* pShopItem = CDataPool::GetMe()->PlayerShop_GetItem(TRUE, nConIndex, nIndex);
			if(pShopItem)
			{
				if(CDataPool::GetMe()->PlayerShop_GetItemOnSale(TRUE, nConIndex, nIndex))
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "目标位置的物品已经上架");
					return;
				}
			}

			//检查是不是由背包界面拖过来的(只能接受玩家背包内的物品)
			if(this->GetItemImpl()->GetTypeOwner() == tObject_Item::IO_MYSELF_PACKET)
			{
				// 判定物品是否已经绑定
				tObject_Item* pItem = this->GetItemImpl();
				if(pItem)
				{
					if(pItem->GetItemBindInfo() == 1)		//==1 是绑定
					{
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "此物品已经绑定");
						break;
					}
					//需要先判定是不是能够存入银行的物品
					if( GetItemImpl() && GetItemImpl()->GetItemClass() == ICLASS_TASKITEM )
					//if(pItem->GetPosIndex() >= TASK_CONTAINER_OFFSET )
					{
						//任务物品不能放入银行
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"任务物品不能放入");
						break;
					}

				}
				//执行将物品转移到商店去的操作
				_ITEM_GUID Guid;
				tObject_Item::ITEM_GUID temp;
				temp.m_idUnion = 0;

				tObject_Item* pSelectItem = this->GetItemImpl();
				if(NULL == pSelectItem)
				{
					return;
				}

				pSelectItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
				Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
				Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
				Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;

				CGItemSynch msg;
				msg.SetOpt(CGItemSynch::OPT_MOVE_ITEM_MANU);
				msg.SetFromType(CGItemSynch::POS_BAG);
				msg.SetToIndex(nIndex);
				msg.SetToType(CGItemSynch::POS_PLAYERSHOP);
				msg.SetItemGUID(Guid);


				INT nPage = CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
				CGManuMoveItemFromBagToPlayerShop_t ExtraInfoData;
				ExtraInfoData.m_ShopGuid		= CDataPool::GetMe()->PlayerShop_GetShopID(TRUE);
				ExtraInfoData.m_nStallIndex		= (BYTE)nPage;
				ExtraInfoData.m_uSerial			= CDataPool::GetMe()->PlayerShop_GetItemSerial(TRUE,nPage,nIndex);
				msg.SetExtraInfoLength(ExtraInfoData.GetSize());
				msg.SetExtraInfoData((BYTE*)&ExtraInfoData);
			
				CNetManager::GetMe()->SendPacket(&msg);
			}
			//在背包内调整位置
			else if(this->GetItemImpl()->GetTypeOwner() == tObject_Item::IO_PS_SELFBOX)
			{
				//如果放到自己格子，那么不需要发送这个消息
				if(this->GetPosIndex() == nIndex)
				{
					return;
				}
				//执行将物品转移到商店去的操作
				_ITEM_GUID Guid;
				tObject_Item::ITEM_GUID temp;
				temp.m_idUnion = 0;

				tObject_Item* pSelectItem = this->GetItemImpl();
				if(NULL == pSelectItem)
				{
					return;
				}

				INT nIndex1 = pSelectItem->GetPosIndex();
				INT nConIndex = nIndex1/20;
				INT nPosition = nIndex1%20;

				if( 1 == CDataPool::GetMe()->PlayerShop_GetItemOnSale(TRUE, nConIndex, nPosition) )
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "此物品已经上架");
					return;
				}

				pSelectItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
				Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
				Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
				Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;

				CGItemSynch msg;
				msg.SetOpt(CGItemSynch::OPT_MOVE_ITEM_MANU);
				msg.SetFromType(CGItemSynch::POS_PLAYERSHOP);
				msg.SetToIndex(nIndex);
				msg.SetToType(CGItemSynch::POS_PLAYERSHOP);
				msg.SetItemGUID(Guid);

				INT nPage = CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
				CGManuMoveItemFromPlayerShopToPlayerShop_t ExtraInfoData;
				ExtraInfoData.m_ShopGuid		= CDataPool::GetMe()->PlayerShop_GetShopID(TRUE);
				ExtraInfoData.m_nStallIndex		= (BYTE)nPage;
				ExtraInfoData.m_uSerialSource	= CDataPool::GetMe()->PlayerShop_GetItemSerial(TRUE,nPage,this->GetPosIndex()%20);
				ExtraInfoData.m_uSerialDest		= CDataPool::GetMe()->PlayerShop_GetItemSerial(TRUE,nPage,nIndex);
				msg.SetExtraInfoLength(ExtraInfoData.GetSize());
				msg.SetExtraInfoData((BYTE*)&ExtraInfoData);
			
				CNetManager::GetMe()->SendPacket(&msg);
			}
		}
		break;
	case 'M':		//主菜单
		{
			//查询来源，只能来源于背包
			if(this->GetItemImpl()->GetTypeOwner() == tObject_Item::IO_MYSELF_PACKET)
			{
				if( this->GetItemImpl()->Rule(tObject_Item::RULE_QUICK) )
				{
					INT nIndex = szTargetName[1]-'0';
					nIndex = szTargetName[2]-'0' + nIndex*10 ;
					//查询目标位置原来的内容
					nOldTargetId = CActionSystem::GetMe()->MainMenuBar_Get(nIndex);
					CActionSystem::GetMe()->MainMenuBar_Set(nIndex, GetID() );
				}
				else
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"此物品不能置入快捷栏。");
				}
			}
		}
		break;

	case 'Y':		//对象头像
		{
			CObject_Item* pItem = (CObject_Item*)GetItemImpl();
			//根据装备类型
			switch(pItem->GetItemClass())
			{
			case ICLASS_COMITEM:	//药瓶
			case ICLASS_TASKITEM:	//任务物品类
			case ICLASS_STOREMAP:	//藏宝图
				{		//摆摊时不可以随便使用物品
					if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
					{
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你正在摆摊……");
						break;
					}

					//验证是否可以使用
					if(!pItem->Rule(tObject_Item::RULE_USE))
					{
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, pItem->RuleFailMessage(tObject_Item::RULE_USE).c_str());
						break;
					}

					//检查冷却是否结束
					if(!CoolDownIsOver()) 
					{
						CActionSystem::GetMe()->SetSetoutAction(this);
						CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, GetOResultText( OR_COOL_DOWNING ) );
						break;
					}

					CObject *pTargetObj = (CObject*)(CObjectManager::GetMe()->GetMainTarget());
					CGameProcedure::s_pGameInterface->PacketItem_UserItem(
						this, pTargetObj? pTargetObj->GetServerID() : -1, fVector2(-1, -1));
				}
				break;
			}

		}
		break;
	case 'L':		//自己头像
		{
			CObject_Item* pItem = (CObject_Item*)GetItemImpl();
			//根据装备类型
			switch(pItem->GetItemClass())
			{
			case ICLASS_COMITEM:	//药瓶
			case ICLASS_TASKITEM:	//任务物品类
			case ICLASS_STOREMAP:	//藏宝图
				{		//摆摊时不可以随便使用物品
					if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
					{
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你正在摆摊……");
						break;
					}

					//验证是否可以使用
					if(!pItem->Rule(tObject_Item::RULE_USE))
					{
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, pItem->RuleFailMessage(tObject_Item::RULE_USE).c_str());
						break;
					}

					//检查冷却是否结束
					if(!CoolDownIsOver()) 
					{
						CActionSystem::GetMe()->SetSetoutAction(this);
						CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, GetOResultText( OR_COOL_DOWNING ) );
						break;
					}

					//对主目标使用物品
					CObject* pMainTarget = (CObject*)CObjectManager::GetMe()->GetMainTarget();
					CGameProcedure::s_pGameInterface->PacketItem_UserItem(this, 
						CObjectManager::GetMe()->GetMySelf()->GetServerID(), fVector2(-1, -1));
				}
				break;
			}

		}
		break;
	default:
		break;
	}

	switch(cSourceName)
	{
	case 'M':
		{
			INT nIndex = szSourceName[1]-'0';
			nIndex = szSourceName[2]-'0' + nIndex*10 ;
			CActionSystem::GetMe()->MainMenuBar_Set(nIndex, nOldTargetId);
			//AxTrace(0,0,"nIndex",)
		}
		break;
	default:
		break;
	}
//	CEventSystem::GetMe()->PushEvent(GE_UPDATE_SUPERTOOLTIP);
	CEventSystem::GetMe()->PushEvent(GE_SUPERTOOLTIP, "0");
}
INT CActionItem_Item::GetCurBankRentBegin(VOID)
{
	INT nRentIndex = CActionSystem::GetMe()->GetCurBankRentBoxIndex();
	INT nBeginIndex = -1;
	switch(nRentIndex) 
	{
	case 1:
		{
			nBeginIndex = RENTBOX1_START_INDEX;
		}
		break;
	case 2:
		{
			nBeginIndex = RENTBOX2_START_INDEX;
		}
		break;
	case 3:
		{
			nBeginIndex = RENTBOX3_START_INDEX;
		}
		break;
	case 4:
		{
			nBeginIndex = RENTBOX4_START_INDEX;
		}
		break;
	case 5:
		{
			nBeginIndex = RENTBOX5_START_INDEX;
		}
		break;
	default:
		return -1;
	}

	return nBeginIndex;
}


//查询逻辑属性
STRING CActionItem_Item::GetAttributeValue(LPCTSTR szAttributeName)
{
	tObject_Item* pItem = GetItemImpl();

	if(!pItem) return "";
	else return pItem->GetAttributeValue(szAttributeName);
}


// 把装备从身上卸下
void CActionItem_Item::UnUseEquip()
{
	CObject_Item* pItem = (CObject_Item*)GetItemImpl();
	if(!pItem) 
	{
		return;
	}

	CGUnEquip msg;
	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)pItem;
	if(pItemEquip->GetItemClass() != ICLASS_EQUIP)
	{
		return;
	}

	// 发送卸下装备消息.
	msg.setEquipPoint((BYTE)(pItemEquip->GetItemType()));
	CNetManager::GetMe()->SendPacket( &msg );
}

// 得到物品的等级
INT	CActionItem_Item::GetItemLevel()
{
	tObject_Item* pItem = GetItemImpl();
	
	if(pItem)
	{
		return pItem->GetNeedLevel();
	}
	else
	{
		return -1;
	}

}

// 得到物品的耐久
INT	CActionItem_Item::GetItemDur()
{
	tObject_Item* pItem = GetItemImpl();
	
	if(pItem)
	{
		return pItem->GetItemDur();
	}
	else
	{
		return -1;
	}

}



// 得到物品的最大耐久
INT	CActionItem_Item::GetItemMaxDur()
{
	tObject_Item* pItem = GetItemImpl();
	
	if(pItem)
	{
		return pItem->GetItemMaxDur();
	}
	else
	{
		return -1;
	}
}

// 得到物品的修理次数
INT	CActionItem_Item::GetItemRepairCount()
{
	tObject_Item* pItem = GetItemImpl();
	
	if(pItem)
	{
		return pItem->GetItemRepairCount();
	}
	else
	{
		return -1;
	}
}

// 得到物品的绑定信息 
INT	CActionItem_Item::GetItemBindInfo()
{
	tObject_Item* pItem = GetItemImpl();
	if(pItem)
	{
		return pItem->GetItemBindInfo();
	}
	else
	{
		return -1;
	}
}

// 得到物品卖给npc的价格
INT	CActionItem_Item::GetItemPrice()
{
	tObject_Item* pItem = GetItemImpl();
	if(pItem)
	{
		return pItem->GetItemPrice();
	}
	else
	{
		return -1;
	}
	
}

// 得到物品的制作人
LPCTSTR	CActionItem_Item::GetManufacturer()
{
	tObject_Item* pItem = GetItemImpl();
	if(pItem)
	{
		return pItem->GetManufacturer();
	}
	else
	{
		return _T("");
	}
	
}


// 得到白色属性
LPCTSTR	CActionItem_Item::GetBaseWhiteAttrInfo()
{
	tObject_Item* pItem = GetItemImpl();
	if(pItem)
	{
		return pItem->GetBaseWhiteAttrInfo();
	}
	else
	{
		return _T("");
	}

	
}

// 得到绿色属性
LPCTSTR	CActionItem_Item::GetGreenAttrInfo()
{
	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	if(pItemEquip)
	{
		return pItemEquip->GetGreenAttrInfo();
	}
	else
	{
		return _T("");
	}

	
}


// 得到扩展蓝色属性
LPCTSTR	CActionItem_Item::GetExtBlueAttrInfo()
{

	tObject_Item* pItem = GetItemImpl();
	if(pItem)
	{
		return pItem->GetExtBlueAttrInfo();
	}
	else
	{
		return _T("");
	}
}

// 得到装备的属性(白, 绿, 蓝)
INT CActionItem_Item::GetEquipQuantity()
{
	tObject_Item* pItem = GetItemImpl();
	if(pItem)
	{
		CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)pItem;
		return pItemEquip->GetEquipQuantity();
	}
	else
	{
		return 0;
	}
}


// 得到action所引用的item 的类型(如是装备, 还是物品)
INT	CActionItem_Item::GetItemType()
{

	STRING strTypeName = GetType_String();

	if(("equip" == strTypeName)			|| 
	    ("packageitem" == strTypeName)	||
		("bankitem" == strTypeName)		||
		("boothitem" == strTypeName)	||
		("lootitem" == strTypeName)		||
		("virtualitem" == strTypeName)  ||
		("ex_self" == strTypeName)		||
		("ex_other" == strTypeName)		||
		("st_self" == strTypeName)		||
		("st_other" == strTypeName)		||
		("ps_self" == strTypeName)		||
		("ps_other" == strTypeName)		||
		("mission" == strTypeName)	)	

	{
		CObject_Item* pItem = (CObject_Item*)GetItemImpl();
		if(pItem)
		{
			return pItem->GetItemClass();
		}
		else
		{
			return -1;
		}
	}

	return -1;
	
}

// 得到装备上的宝石个数
INT CActionItem_Item::GetGemInEquipCount()
{
	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	
	if(pItemEquip)
	{
		// 得到已经镶嵌宝石的个数
		//INT iCount = pItemEquip->GetGemCount();

		// 得到可以镶嵌宝石的个数
		INT iCount = pItemEquip->GetGemMaxCount();
		return iCount;
	}
	else
	{
		return 0;
	}
	
}

// 得到宝石的图标
VOID CActionItem_Item::GetGemIcon(INT iGemIndex, STRING& strIconName)
{
	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	
	if(pItemEquip)
	{
		
		pItemEquip->GetGemIcon(iGemIndex, strIconName);
	}
	else
	{
		strIconName = _T("");
	}

	return;
}

// 得到装备上的宝石描述
VOID CActionItem_Item::GetGemInEquipDesc(STRING& strDesc)
{
	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	STRING strInfo = _T("");
	strDesc = _T("");
	
	if(pItemEquip)
	{
		int iCount = pItemEquip->GetGemCount();
		for(int i = 0; i < iCount; i++)
		{
			strInfo = _T("");//每颗宝石重新计算附加属性
			pItemEquip->GetGemExtAttr(i, strInfo);
			strDesc += strInfo;
		}
	}
	else
	{
		strDesc = _T("");
	}

	return;
}

// 得到装备的最大修理次数
INT CActionItem_Item::GetItemMaxRepair()
{
	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	if(pItemEquip)
	{
		pItemEquip->GetMaxRepair();
	}
	
	return -1;
}

// 得到宝石等级
INT CActionItem_Item::GetGemLevel()
{
	CObject_Item_Gem* pItem = (CObject_Item_Gem*)GetItemImpl();
	if(pItem)
	{
		return pItem->GetGemLevel();
	}

	return -1;
}

// 得到宝石属性信息
LPCTSTR	CActionItem_Item::GetGemArribInfo()
{
	static TCHAR szInfo[32] = {0};

	CObject_Item_Gem* pItem = (CObject_Item_Gem*)GetItemImpl();
	if(pItem)
	{
		return pItem->GetGemArribInfo();
	}

	return szInfo;
}

// 得到原料或药品的质量
INT CActionItem_Item::GetItemQuality()
{
	CObject_Item_Medicine* pItem = (CObject_Item_Medicine*)GetItemImpl();

	if(pItem)
	{
		return pItem->GetQuantity();
	}

	return -1;
}

// 得到消耗品在表中的类型
INT CActionItem_Item::GetMedicineItemTableType()
{
	CObject_Item_Medicine* pItem = (CObject_Item_Medicine*)GetItemImpl();

	if(pItem)
	{
		return pItem->GetMedicineItemTableType();
	}

	return -1;
}

//得到冷却状组ID
INT	CActionItem_Item::GetCoolDownID(VOID)
{
	CObject_Item_Medicine* pItem = (CObject_Item_Medicine*)GetItemImpl();
	if(pItem && pItem->GetItemClass() == ICLASS_COMITEM)
	{
		//只有背包里的物品会走CoolDown
		if(pItem->GetTypeOwner() != tObject_Item::IO_MYSELF_PACKET)
			return -1;

		INT nSkillTableID = pItem->GetItemSkillID();

		if(nSkillTableID != -1)
		{
			//查表
			DBC_DEFINEHANDLE(s_pSkillTable, DBC_SKILL_DATA);
			//搜索纪录
			const _DBC_SKILL_DATA* pSkill = (const _DBC_SKILL_DATA*)s_pSkillTable->Search_Index_EQU((UINT)nSkillTableID);
			if(!pSkill) return NULL;

			return pSkill->m_nCooldownID;
		}
	}
	return -1;
}

//检查冷却是否结束
BOOL CActionItem_Item::CoolDownIsOver(VOID)
{
	INT nCoolDownID = GetCoolDownID();

	//对于没有冷却的Action
	if(INVALID_ID == nCoolDownID) return TRUE;
	//取得冷却组数据
	const CDataPool::COOLDOWN_GROUP* pCoolDownGroup =
		CDataPool::GetMe()->CoolDownGroup_Get(nCoolDownID);

	if( pCoolDownGroup && pCoolDownGroup->nTime <= 0 && CDataPool::GetMe()->CommonCoolDown_Get() <= 0 ) 
		return TRUE;
	else 
		return FALSE;
}

//DoAction_Packet
VOID CActionItem_Item::DoAction_Packet(VOID)
{
	CObject_Item* pItem = (CObject_Item*)this->GetItemImpl();
	if(!pItem) return;

	BOOL bLog = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;

	if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Identify::GetMe())
	{
		// 取消鉴定2006－4－15
		CancelIdentify();
		return;
	}

	//如果玩家商店开着，
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("PS_ShopMag"))
	{
		
		if(!(CDataPool::GetMe()->PlayerShop_GetShopType(TRUE) == 1))
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "物品不能放入宠物店");
			return;
		}

		if(pItem->GetItemBindInfo() == 1)		//==1 是绑定
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "此物品已经绑定");
			return;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
		}

		//需要先判定是不是能够存入银行的物品
		if( GetItemImpl() && GetItemImpl()->GetItemClass() == ICLASS_TASKITEM )
		//if(pItem->GetPosIndex() >= TASK_CONTAINER_OFFSET )
		{
			//任务物品不能放入银行
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"任务物品不能放入");
			return;
		}

		//执行将物品转移到商店去的操作
		_ITEM_GUID Guid;
		tObject_Item::ITEM_GUID temp;
		temp.m_idUnion = 0;

		tObject_Item* pSelectItem = this->GetItemImpl();
		if(NULL == pSelectItem)
		{
			return;
		}
		pSelectItem->GetGUID(temp.m_idOrg.m_idWorld, temp.m_idOrg.m_idServer, temp.m_idOrg.m_uSerial);
		Guid.m_World	= (BYTE)temp.m_idOrg.m_idWorld;
		Guid.m_Server	= (BYTE)temp.m_idOrg.m_idServer;	
		Guid.m_Serial	= (INT)temp.m_idOrg.m_uSerial;

		CGItemSynch msg;
		msg.SetOpt(CGItemSynch::OPT_MOVE_ITEM_AUTO);
		msg.SetFromType(CGItemSynch::POS_BAG);
		msg.SetToType(CGItemSynch::POS_PLAYERSHOP);
		msg.SetItemGUID(Guid);

		CGAutoMoveItemFromBagToPlayerShop_t ExtraDataInfo;
		ExtraDataInfo.m_ShopGuid		= CDataPool::GetMe()->PlayerShop_GetShopID(TRUE);
		ExtraDataInfo.m_nStallIndex		= (BYTE)CDataPool::GetMe()->PlayerShop_GetMySelectConTa();
		ExtraDataInfo.m_nShopSerial		= (BYTE)CDataPool::GetMe()->PlayerShop_GetShopSerial(TRUE);
		msg.SetExtraInfoLength(ExtraDataInfo.GetSize());
		msg.SetExtraInfoData((BYTE*)&ExtraDataInfo);
	
		if(pItem->GetByProtect())
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"物品已加锁");
		}
		else
		{
			CNetManager::GetMe()->SendPacket(&msg);
		}


		return;
	}
	//如果摆摊界面开着，点击物品代表飞到摆摊界面中(中间需要经过一个确认步骤)
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("StallSale"))
	{
		// 判定物品是否已经绑定
		if(pItem->GetItemBindInfo() == 1)		//==1 是绑定
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "此物品已经绑定");
			return;
		}
		//判断物品是不是任务物品
		if( GetItemImpl() && GetItemImpl()->GetItemClass() == ICLASS_TASKITEM )
		//if( this->GetPosIndex() >= TASK_CONTAINER_OFFSET )
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "任务物品不能上架销售");
			return;
		}
		//查询摊位上是不是还有空位
		if(CDataPool::GetMe()->MyStallBox_IsHaveEmpty(1) == FALSE)
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "摊位上已经没有空格");
			return;
		}

		//先保存选中的物品
		CDataPool::GetMe()->MyStallBox_SetCurSelectItem(this->GetItemImpl());
		// 同时记录Item所在的容器
		CDataPool::GetMe()->MyStallBox_SetSelectItemInPos(tObject_Item::IO_MYSELF_PACKET);
		//打开一个输入卖价的对话框
		CEventSystem::GetMe()->PushEvent(GE_TOGLE_INPUT_MONEY,"price");

		return;
	}
	//如果提交任务物品的界面开着,点击代表物品飞到提交栏中
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("MissionReply"))
	{
		//需要在MissionBOX中寻找空的位置
		INT nIndex = -1;
		for(int i=0;i<3;i++)
		{
			if( CDataPool::GetMe()->MissionBox_GetItem(i) == NULL )
			{
				nIndex = i;
				break;
			}
		}
		if(nIndex == -1)
		{
			//没有空格可以放入物品了，给个提示就退出
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"任务物品已经放满了");
			return;
		}

		//克隆物品
		tObject_Item* pNewItem  = CObject_Item::NewItem( this->GetItemImpl()->GetIdTable());
		((CObject_Item*)pNewItem)->Clone((CObject_Item*)this->GetItemImpl());

		//在这里保存直接通知界面添加按钮，不给服务器发消息，
		CDataPool::GetMe()->MissionBox_SetItem(nIndex, pNewItem);

		// 锁定背包中对应的这个物品
		pItem->SetLock(TRUE);

		CActionSystem::GetMe()->UserMission_Update();
		// 发送更新消息
		CEventSystem::GetMe()->PushEvent(GE_UPDATE_REPLY_MISSION);
		// 背包更新消息
		CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

		return;
	}
	//如果银行界面时打开的，点击代表存银行
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Bank"))
	{
		//需要先判定是不是能够存入银行的物品
		if( GetItemImpl() && GetItemImpl()->GetItemClass() == ICLASS_TASKITEM )
		//if(pItem->GetPosIndex() >= TASK_CONTAINER_OFFSET )
		{
			//任务物品不能放入银行
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"任务物品不能存入银行");
			return;
		}	
		
		//需要检测当前银行打开的是第几个租赁栏
		INT nRentIndex = CActionSystem::GetMe()->GetCurBankRentBoxIndex();

		CGBankAddItem msg;
		msg.SetFromType(CGBankAddItem::BAG_POS);
		msg.SetIndexFrom(this->GetPosIndex());

		if(nRentIndex == 1)
			msg.SetIndexTo(CGBankAddItem::AUTO_POS_BOX1);
		else if(nRentIndex == 2)
			msg.SetIndexTo(CGBankAddItem::AUTO_POS_BOX2);
		else if(nRentIndex == 3)
			msg.SetIndexTo(CGBankAddItem::AUTO_POS_BOX3);
		else if(nRentIndex == 4)
			msg.SetIndexTo(CGBankAddItem::AUTO_POS_BOX4);
		else if(nRentIndex == 5)
			msg.SetIndexTo(CGBankAddItem::AUTO_POS_BOX5);

		//检测这个租赁箱是不是有空位
		if( !CDataPool::GetMe()->UserBank_IsEmpty( nRentIndex ) )
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"目标箱子已经满了");
			return;
		}

		CNetManager::GetMe()->SendPacket(&msg);

		return;
	}
	// 如果交易界面开着，点击代表将此物品放入交易栏
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Exchange"))
	{
		// 判定物品是否已经绑定
		if(pItem->GetItemBindInfo() == 1)		//==1 是绑定
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "此物品已经绑定");
			return;
		}
		//判定是不是能够存入银行的物品
		if( pItem->GetItemClass() == ICLASS_TASKITEM )
		//if(pItem->GetPosIndex() >= TASK_CONTAINER_OFFSET )
		{
			//任务物品不能交易
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"任务物品不能进行交易");
			return;
		}
		//验证是否可以交易
		if(!pItem->Rule(tObject_Item::RULE_EXCHANGE))
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, pItem->RuleFailMessage(tObject_Item::RULE_EXCHANGE).c_str());
			return;
		}
		
		//发送拖动物品到交易界面的消息
		// 需要先检测是否交易界面已满
		CGExchangeSynchItemII msg;
		msg.SetOpt(EXCHANGE_MSG::OPT_ADDITEM);	
		msg.SetFromType(EXCHANGE_MSG::POS_BAG);
		msg.SetFromIndex(this->GetPosIndex());
		msg.SetToIndex(0);						//由服务器查找

		if(pItem->GetByProtect())
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"物品已加锁");
		}
		else
		{
			CNetManager::GetMe()->SendPacket(&msg);
		}

		return;
	}

	// 镶嵌界面
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Enchase"))
	{
		if( GetItemImpl()->GetByProtect() )
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"物品已加锁");
			return;
		}

		if(GetItemImpl()->GetItemClass() == ICLASS_EQUIP)
		{
			if(((CObject_Item_Equip*)GetItemImpl())->GetEquipQuantity() == CObject_Item_Equip::BLUE_EQUIP && 
				((CObject_Item_Equip*)GetItemImpl())->GetEquipAttrib() == CObject_Item_Equip::EQUIP_ATTRIB_UNIDENTIFY)
			{
				STRING strTemp = "此物品需要鉴定";
				ADDNEWDEBUGMSG(strTemp);
				return;
			}
			CEventSystem::GetMe()->PushEvent(GE_UPDATE_COMPOSE_GEM, 1, this->GetItemImpl()->GetPosIndex());
		}
		if(GetItemImpl()->GetItemClass() == ICLASS_GEM)
		{
			CEventSystem::GetMe()->PushEvent(GE_UPDATE_COMPOSE_GEM, 2, this->GetItemImpl()->GetPosIndex());
		}
		

		return;
	}

	//如果商品界面是开的,点击代表卖出
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Shop"))
	{
		//是否在修理状态
		if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Repair::GetMe())
		{
			//取消修理
			CGameProcedure::s_pGameInterface->Skill_CancelAction();
		}
		//执行销售
		else
		{
			//任务物品不可以卖
			if( pItem->GetItemClass() == ICLASS_TASKITEM )
			//if(pItem->GetPosIndex() < TASK_CONTAINER_OFFSET )
			{
				//任务物品不能卖的信息提示
				if(bLog)
				{
					STRING strTemp = "这件物品不能出售";//NOCOLORMSGFUNC("stall_sold_failure");
					ADDNEWDEBUGMSG(strTemp);
				}
			}
			else
			{
				CGameProcedure::s_pGameInterface->Booth_SellItem( pItem );
			}
		}

		return;
	}
	else
	{
		//根据装备类型
		switch(pItem->GetItemClass())
		{
		case ICLASS_EQUIP:
			//装备
			{
				//使用
				CGameProcedure::s_pGameInterface->PacketItem_UserEquip( pItem->GetPosIndex() );
			}
			break;

		case ICLASS_COMITEM:	//药瓶
		case ICLASS_TASKITEM:	//任务物品类
		case ICLASS_STOREMAP:	//藏宝图
			{
				int iType = pItem->GetItemTableType();

				//摆摊时不可以随便使用物品
				if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你正在摆摊……");
					break;
				}

				//验证是否可以使用
				if(!pItem->Rule(tObject_Item::RULE_USE))
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, pItem->RuleFailMessage(tObject_Item::RULE_USE).c_str());
					break;
				}

				//检查冷却是否结束
				if(!CoolDownIsOver()) 
				{
					CActionSystem::GetMe()->SetSetoutAction(this);
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, GetOResultText( OR_COOL_DOWNING ) );
					break;
				}
				CObject* pMainTarget = (CObject*)CObjectManager::GetMe()->GetMainTarget();

				CGameProcedure::s_pGameInterface->PacketItem_UserItem(this, 
					pMainTarget ? pMainTarget->GetServerID() : -1, fVector2(-1, -1));
			}
			break;

		case ICLASS_GEM:
			//宝石
			{
			}
			break;
		}
	}
}

//销毁物品的操作
VOID CActionItem_Item::DestoryItem(LPCSTR szSourceName)
{
	CHAR cSourceType = szSourceName[0];
	switch(cSourceType)
	{
	case 'M':		//主菜单
		{
			INT nIndex = szSourceName[1]-'0';
			nIndex = szSourceName[2]-'0' + nIndex*10 ;
			CActionSystem::GetMe()->MainMenuBar_Set(nIndex, -1 );
			return;
		}
		break;
	default:
		break;
	}
	INT nTypeOwner = this->GetItemImpl()->GetTypeOwner();
	if( nTypeOwner == tObject_Item::IO_MYSELF_PACKET || 
		//nTypeOwner == tObject_Item::IO_MYSELF_EQUIP  ||
		nTypeOwner == tObject_Item::IO_MYSELF_BANK)
	{
		// 任务物品不能销毁，给一个提示
		if(nTypeOwner == tObject_Item::IO_MYSELF_PACKET && GetItemImpl() && GetItemImpl()->GetItemClass() == ICLASS_TASKITEM )
		{
			// 需要先询问是否销毁这个装备
			//CEventSystem::GetMe()->PushEvent(GE_OPEN_CANNT_DISCARD_ITEM,this->GetItemImpl()->GetName());
			// 给出屏幕提示

			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"任务物品不能丢弃");
		}
		else
		{
			// 先取消可能存在的锁定
			INT nTypeOwner	=	CDataPool::GetMe()->DisCard_GetItemConta();
			INT	nItemPos	=	CDataPool::GetMe()->DisCard_GetItemPos();

			switch( nTypeOwner ) 
			{
			case tObject_Item::IO_MYSELF_PACKET:				// 来自玩家的背包
				{
					tObject_Item* pItem = CDataPool::GetMe()->UserBag_GetItem(nItemPos);

					if(pItem)
					{
						pItem->SetLock(FALSE);
					}
				}
				break;
			case tObject_Item::IO_MYSELF_EQUIP:					// 来自玩家身上的装备
				{
					tObject_Item* pItem = CDataPool::GetMe()->UserEquip_GetItem((HUMAN_EQUIP)nItemPos);

					if(pItem)
					{
						pItem->SetLock(FALSE);
					}
				}
				break;
			case tObject_Item::IO_MYSELF_BANK:					// 玩家的银行
				{
					tObject_Item* pItem = CDataPool::GetMe()->UserBank_GetItem(nItemPos);

					if(pItem)
					{
						pItem->SetLock(FALSE);
					}
				}
				break;
			default:
				break;
			}
					
			// 需要先询问是否销毁这个装备
			CEventSystem::GetMe()->PushEvent(GE_OPEN_DISCARD_ITEM_FRAME,this->GetItemImpl()->GetName());

			// 锁定这个物品
			tObject_Item *pObj = this->GetItemImpl();
			pObj->SetLock(TRUE);

			//通知背包锁定状态
			CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

			CDataPool::GetMe()->DisCard_SetItemConta(this->GetItemImpl()->GetTypeOwner());
			CDataPool::GetMe()->DisCard_SetItemPos(this->GetPosIndex());
		}
	}
}

// 得到配方需要的技能名称。
LPCTSTR CActionItem_Item::GetPrescrNeedSkillName()
{
	CObject_Item_Medicine* pItem = (CObject_Item_Medicine*)GetItemImpl();
	if(pItem)
	{
		INT nIndex = pItem->GetBaseDefine()->nScriptID - 700000;


		// 生活技能
		DBC_DEFINEHANDLE(s_ItemCompound, DBC_LIFEABILITY_ITEMCOMPOSE);
		_DBC_LIFEABILITY_ITEMCOMPOSE* pLifeInfo = (_DBC_LIFEABILITY_ITEMCOMPOSE*)s_ItemCompound->Search_Index_EQU(nIndex);

		//配方表
		DBC_DEFINEHANDLE(s_pAbility, DBC_LIFEABILITY_DEFINE);
		_DBC_LIFEABILITY_DEFINE* pData = (_DBC_LIFEABILITY_DEFINE*)s_pAbility->Search_Index_EQU(pLifeInfo->nLifeAbility);

		if(pData)
		{
			return pData->szName;
		}
	}

	return _T("");
			
}

// 得到配方需要的技能等级。
int CActionItem_Item::GetPrescrNeedSkillLevel()
{

	CObject_Item_Medicine* pItem = (CObject_Item_Medicine*)GetItemImpl();
	if(pItem)
	{
		INT nIndex = pItem->GetBaseDefine()->nScriptID - 700000;

//		int iNeedSkillLevel = pItem->m_theBaseDef->nLevelRequire;
		DBC_DEFINEHANDLE(s_ItemCompound, DBC_LIFEABILITY_ITEMCOMPOSE);
		const _DBC_LIFEABILITY_ITEMCOMPOSE* pLifeInfo = (_DBC_LIFEABILITY_ITEMCOMPOSE*)s_ItemCompound->Search_Index_EQU(nIndex);

		return pLifeInfo->nLifeAbility_Level;
	//	return iNeedSkillLevel;
	}

	return 0;
}
bool CActionItem_Item::IsPrescrStudy()
{
	//CObject_Item_Medicine* pItem = (CObject_Item_Medicine*)GetItemImpl();
	//if(pItem)
	//{
	//	const SCLIENT_PRESCR* pPrescr = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Prescr(pItem->GetBaseDefine()->nRecipeID);
	//	if(pPrescr) 
	//		return true;
	//}
	return false;
}
// 得到配方需要技能的当前等级。
int CActionItem_Item::GetPrescrNeedSkillCurLevel()
{
	CObject_Item_Medicine* pItem = (CObject_Item_Medicine*)GetItemImpl();
	if(pItem)
	{
		INT nIndex = pItem->GetBaseDefine()->nScriptID - 700000;

		const SCLIENT_PRESCR* pPrescr = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Prescr(nIndex);
		if(pPrescr)
		{
			return INVALID_ID;
		}
		else
		{
			DBC_DEFINEHANDLE(s_ItemCompound, DBC_LIFEABILITY_ITEMCOMPOSE);
			const _DBC_LIFEABILITY_ITEMCOMPOSE* pLifeInfo = (_DBC_LIFEABILITY_ITEMCOMPOSE*)s_ItemCompound->Search_Index_EQU(nIndex);
			
			const SCLIENT_LIFEABILITY* pLifeAbility = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_LifeAbility(pLifeInfo->nLifeAbility);

			if(!pLifeAbility)
				return 0;

			return pLifeAbility->m_nLevel;
			

		}
	}

	return 0;
}

// 发送请求详细数据消息
void CActionItem_Item::SendAskItemInfoMsg()
{
	if(GetItemImpl()->GetTypeOwner() == tObject_Item::IO_MYSELF_PACKET)
	{
		CObject_Item* pItem = (CObject_Item*)GetItemImpl();

		if((pItem)&&(pItem->IsItemHasDetailInfo()))
		{
			int iIndex = GetItemImpl()->GetPosIndex();
			CGAskItemInfo msg;
			msg.setID(iIndex);
			CNetManager::GetMe()->SendPacket(&msg);
		}
	}
}

// 得到装备的鉴定信息2006－3－29
INT CActionItem_Item::GetEquipIdentify()
{
	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	if(pItemEquip)
	{
		return pItemEquip->GetEquipAttrib();
	}

	return 0;
	
}

// 使用鉴定卷轴2006-3-30
VOID CActionItem_Item::UseIdentify()
{
	CObject_Item* pItem = (CObject_Item*)GetItemImpl();
	if(ICLASS_EQUIP != pItem->GetItemClass())
	{
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "只能鉴定武器");
		return;
	}

	CObject_Item_Equip* pItemEquip = (CObject_Item_Equip*)GetItemImpl();
	if(pItemEquip)
	{
		if(CObject_Item_Equip::BLUE_EQUIP == pItemEquip->GetEquipQuantity())
		{
			if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Identify::GetMe())
			{
				if(CObject_Item_Equip::EQUIP_ATTRIB_IDENTIFY == pItemEquip->GetEquipAttrib())
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "已经鉴定过了");
				}
				else
				{
					//if(pItemEquip->GetNeedLevel() > (CDataPool::GetMe()->m_iIdentifyLevel + 9))
					//{
					//	CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "不能鉴定比卷轴级别高的道具");
					//	return;
					//}//

					// 使用卷轴的背包索引位置
					//使3种鉴定卷轴通用 fujia 2008.1.2
					//CObject_Item* pItem = (CObject_Item*)CDataPool::GetMe()->UserBag_GetItem( CDataPool::GetMe()->m_iIdentifyInBagIndex);

					//int iIdentifyType = pItem->GetItemTableType();
					//switch(iIdentifyType)
					//{
					//case CObject_Item_Medicine::IDEN_EQUIP:
					//	{
					//		if(!pItemEquip->IsWeapon())
					//		{
					//			// 只能鉴定武器
					//			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "只能鉴定武器");
					//			return;
					//		}

					//		break;
					//	}
					//case CObject_Item_Medicine::IDEN_DEFINCE:
					//	{
					//		if(!pItemEquip->IsDefence())
					//		{
					//			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "只能鉴定防俱");
					//			return;
					//		}
					//		break;
					//	}
					//case CObject_Item_Medicine::IDEN_APPEND:
					//	{
					//		if(!pItemEquip->IsAppend())
					//		{
					//			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "只能鉴定饰品");
					//			return;
					//		}
					//		break;
					//	}

					//default:
					//	{
					//		break;
					//	}

					//}

					// 发送鉴定消息
					CGUseItem msg;
					msg.SetBagIndex( CDataPool::GetMe()->m_iIdentifyInBagIndex );
					msg.SetTargetItem(pItemEquip->GetPosIndex());
					CNetManager::GetMe()->SendPacket(&msg);
				}
			}
		}
		else
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "只能鉴定蓝色装备");
		}
	}
	
}



	// 取消鉴定状态2006－4－15
VOID CActionItem_Item::CancelIdentify()
{
	// 解锁action button
	CDataPool::GetMe()->Identify_UnLock();
}

// 准备使用鉴定卷轴2006-3-30
VOID CActionItem_Item::PrepareUseIdentify()
{
	if(CActionSystem::GetMe()->GetDefaultAction()==CActionItem_MouseCmd_Identify::GetMe())
	{
		//已经进入鉴定状态
		return;
	}

	// 使用卷轴的背包索引位置
	CObject_Item* pItem = (CObject_Item*)GetItemImpl();
	if(pItem)
	{
		if(ICLASS_COMITEM == pItem->GetItemClass())
		{
			int iType = pItem->GetItemTableType();
			if((iType >= COMITEM_WPIDENT)&&(iType <= COMITEM_NCIDENT))
			{
				CDataPool::GetMe()->m_iIdentifyInBagIndex = pItem->GetPosIndex();
				CDataPool::GetMe()->m_iIdentifyLevel	  = pItem->GetNeedLevel();

				GetItemImpl()->SetLock(TRUE);
				CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);

				// 记录锁定位置。
				CDataPool::GetMe()->Identify_Lock(pItem->GetPosIndex());
			}
		}
	}
}

	
// 得到类型描述
LPCTSTR	CActionItem_Item::GetTypeDesc()
{
	// 使用卷轴的背包索引位置
	CObject_Item* pItem = (CObject_Item*)GetItemImpl();
	if(pItem)
	{
		return pItem->GetItemTableTypeDesc();
	}

	return "";
}

// 是否需要在ToolTip上显示价格
INT CActionItem_Item::IsViewToolTopsPrice()
{
	//显示ToolTips上的价格现在有两种情况，两种情况都是在Shop开启的情况下的
	//1.当Shop界面开启，这时候背包中的物品显示
	//2.Shop上的回购物品显示
	if( CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Shop") )
	{
		if( NAMETYPE_PACKAGEITEM == GetType_String() )
		{
			// 修理价格
			if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Repair::GetMe())
			{
				//看这个Npc是不是能够修理这个物品
				if(CDataPool::GetMe()->Booth_IsCanRepair(this->GetItemImpl()))
				{
					return TOOLTOPS_VIEW_PRICE_REPAIR;
				}
			}

			// 还需要看看这个NPC是不是收购这个东西
			else if(CDataPool::GetMe()->Booth_IsCanBuy(this->GetItemImpl()))
			{
				return TOOLTOPS_VIEW_PRICE_SELL;
			}
		}
		if( NAMETYPE_EQUIP == GetType_String() )
		{
			// 修理价格
			if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Repair::GetMe())
			{
				return TOOLTOPS_VIEW_PRICE_REPAIR;
			}
		}
		if( NAMETYPE_CALLBACK == GetType_String() )
		{
			return TOOLTOPS_VIEW_PRICE_CALLBACK;
		}
	}

	//3.银票，不考虑商店界面是否打开，不考虑是否在修理中
	// 拿到supertooltip中直接做 [9/11/2006]
	//if(NAMETYPE_PACKAGEITEM == GetType_String()			//1.背包中
	//	&& GetDefineID() == TICKET_ITEM_INDEX			//2.银票的物品编号
	//	&& GetItemType() == ICLASS_MATERIAL				//3.原料类型
	//	&& !(((CObject_Item*)GetItemImpl())->IsItemHasDetailInfo()))		//4.扩展信息已经获取
	//{
	//	return TOOLTOPS_VIEW_LOOT_SILVER;
	//}

	return TOOLTOPS_VIEW_PRICE_NO;
}

// 获得在ToolTip上显示的价格
INT CActionItem_Item::GetViewToolTopsPrice()
{
	//显示ToolTips上的价格现在有两种情况，两种情况都是在Shop开启的情况下的
	//1.当Shop界面开启，这时候背包中的物品显示
	//2.Shop上的回购物品显示
	if( CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Shop") )
	{
		if( NAMETYPE_PACKAGEITEM == GetType_String() )
		{
			if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Repair::GetMe())
			{
				FLOAT fRepairSpend = CDataPool::GetMe()->GetMe()->Booth_GetRepairSpend();

				if( this->GetItemImpl() && this->GetItemImpl()->GetItemClass() == ICLASS_EQUIP )
				{
					return (INT)(((CObject_Item_Equip*)this->GetItemImpl())->GetRepairPrice() * fRepairSpend);
				}
				else
				{
					return 0;
				}
			}
			else if(CDataPool::GetMe()->Booth_IsCanBuy(this->GetItemImpl()))
			{
				return CDataPool::GetMe()->ItemBox_GetCurNpcBuyPrice(GetPosIndex());
			}
		}
		if( NAMETYPE_EQUIP == GetType_String() )
		{
			// 修理价格
			if(CActionSystem::GetMe()->GetDefaultAction() == CActionItem_MouseCmd_Repair::GetMe())
			{
				FLOAT fRepairSpend = CDataPool::GetMe()->GetMe()->Booth_GetRepairSpend();

				if( this->GetItemImpl() && this->GetItemImpl()->GetItemClass() == ICLASS_EQUIP )
				{
					return (INT)(((CObject_Item_Equip*)this->GetItemImpl())->GetRepairPrice() * fRepairSpend);
				}
				else
				{
					return 0;
				}
			}
		}
		if( NAMETYPE_CALLBACK == GetType_String() )
		{
			return CDataPool::GetMe()->Booth_GetSoldPrice(GetPosIndex());
		}
	}

	//3.银票，不考虑商店界面是否打开，不考虑是否在修理中
	// 拿到supertooltip中直接做 [9/11/2006]
	//if(NAMETYPE_PACKAGEITEM == GetType_String()			//1.背包中
	//	&& GetDefineID() == TICKET_ITEM_INDEX			//2.银票的物品编号
	//	&& GetItemType() == ICLASS_MATERIAL				//3.原料类型
	//	&& !(((CObject_Item*)GetItemImpl())->IsItemHasDetailInfo()))		//4.扩展信息已经获取
	//{
	//	if(((CObject_Item*)GetItemImpl())->GetItemExtraInfoItem())	//为了安全，还是检查一下吧。
	//	{
	//		INT nMoney = ((CObject_Item*)GetItemImpl())->GetItemExtraInfoItem()->GetItemParamValue( 0, IPV_INT );
	//		return (nMoney >= 0)?nMoney:0;
	//	}
	//}

	return 0;
}

// 得到物品档次等级描述 2006-5-18
INT CActionItem_Item::GetItemLevelDesc()
{
	// 使用卷轴的背包索引位置
	CObject_Item* pItem = (CObject_Item*)GetItemImpl();
	if(pItem)
	{
		if(ICLASS_COMITEM == pItem->GetItemClass())
		{
			return pItem->GetItemLevelDesc();
		}
	}

	return 0;

}
