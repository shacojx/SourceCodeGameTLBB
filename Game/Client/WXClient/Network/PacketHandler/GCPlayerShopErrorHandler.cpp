#include "StdAfx.h"

#include "GCPlayerShopError.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Action\GMActionSystem.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Object.h"

#include "TDException.h"
UINT GCPlayerShopErrorHandler::Execute( GCPlayerShopError* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
		{
			switch(pPacket->GetID())
			{
				//不用强行取消本次交易的错误
			case PLAYERSHOP_MSG::ERR_NOT_ENOUGH_LEVEL:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你的等级太低，到30级再来找我吧");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_NAME_ALREADY_EXISTED:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"对不起，已经有同名店铺存在，请重新取名。");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_NOT_ENOUGH_MONEY_TO_NEW:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你的金钱不够，筹些资金再来找我吧。");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_NOT_ENOUGH_SHOP_IN_POOL:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"超过系统店铺上限，不能再开店了");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_NOT_ENOUGH_ROOM_IN_STALL:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"此柜台已满");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_ALREADY_LOCK:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"此物品已锁");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_NEED_NEW_COPY:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"数据已改变，请重新打开商店");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_NOT_ENOUGH_ROOM:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"背包已满");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_NOT_ENOUGH_MONEY:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"金钱不够");
					return PACKET_EXE_CONTINUE ;
					break;
				}

			case PLAYERSHOP_MSG::ERR_ALREADY_HAVE_ENOUGH_SHOP:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"无法开更多店铺");
					return PACKET_EXE_CONTINUE ;
				}
				break;

			case PLAYERSHOP_MSG::ERR_SHOP_SUCCESS_SELL:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"购买商店成功");
					return PACKET_EXE_CONTINUE ;
				}
				break;

			case PLAYERSHOP_MSG::ERR_SHOP_SUCCESS_CHANGE_DESC:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"修改商店描述成功");
					return PACKET_EXE_CONTINUE ;
				}
				break;

			case PLAYERSHOP_MSG::ERR_SHOP_SUCCESS_CHANGE_NAME:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"修改商店名成功");
					return PACKET_EXE_CONTINUE ;
				}
				break;

			case PLAYERSHOP_MSG::ERR_SHOP_STILL_STALL_OPEN:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你的店铺还有柜台在营业");
					return PACKET_EXE_CONTINUE ;
				}
				break;

			case PLAYERSHOP_MSG::ERR_SHOP_SALE_OUT_SERIAL_IS_CHANGED:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"店主已更改内容,请重新打开");
					return PACKET_EXE_CONTINUE ;
				}
				break;


			case PLAYERSHOP_MSG::ERR_SHOP_BASE_MONEY_TOO_LOW:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"本金不足");
					return PACKET_EXE_CONTINUE ;
				}
				break;


			case PLAYERSHOP_MSG::ERR_SHOP_NOT_ENOUTH_MONEY_TO_SALE_OUT:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"金钱不足");
					return PACKET_EXE_CONTINUE ;
				}
				break;


			case PLAYERSHOP_MSG::ERR_SHOP_NOT_ENOUTH_MONEY_TO_BUY_BACK:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "金钱不足");
					return PACKET_EXE_CONTINUE ;
				}
				break;

			case PLAYERSHOP_MSG::ERR_SHOP_NOT_ENOUTH_MONEY_TO_CHANGE_NAME:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "金钱不足");
					return PACKET_EXE_CONTINUE ;
				}
				break;

			case PLAYERSHOP_MSG::ERR_SHOP_PARTNER_LIST_FULL:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"合伙人列表已经满了");
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case PLAYERSHOP_MSG::ERR_SHOP_PARTNER_ALREADY_IN_LIST:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"此人已经是合伙人了");
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case PLAYERSHOP_MSG::ERR_SHOP_PARTNER_NOT_FIND_IN_WORLD:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"人物不存在");
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case PLAYERSHOP_MSG::ERR_SHOP_PARTNER_INVALID:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"非法命令");
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case PLAYERSHOP_MSG::ERR_SHOP_PARTNER_LIST_EMPTY:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"已经没有合伙人了");
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case PLAYERSHOP_MSG::ERR_SHOP_PARTNER_NOT_FIND_IN_LIST:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"无法删除");
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case PLAYERSHOP_MSG::ERR_SHOP_ADD_TO_FAVORITE:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"无法添加");
					return PACKET_EXE_CONTINUE ;
				}
				break;
			case PLAYERSHOP_MSG::ERR_SHOP_DEL_FROM_FAVORITE:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"无法删除");
					return PACKET_EXE_CONTINUE ;
				}
				break;

				//必须取消本次交易的错误
			case PLAYERSHOP_MSG::ERR_ILLEGAL:
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"非法消息");
				}
			default:
				{

				}
				break;
			}

			//清空池中的商店数据
			CDataPool::GetMe()->PlayerShop_CleanUp(TRUE);
			CDataPool::GetMe()->PlayerShop_CleanUp(FALSE);

		}
		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
