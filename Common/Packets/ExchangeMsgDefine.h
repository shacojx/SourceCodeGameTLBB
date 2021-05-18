#ifndef __EXCHANGEMSG_H__
#define __EXCHANGEMSG_H__
#include "Type.h"

namespace EXCHANGE_MSG
{
	enum
	{
		POS_ERR	= 0,
		POS_BAG,				//背包
		POS_EQUIP,				//装备
		POS_PET,				//宠物
	};
	enum
	{
		OPT_ERROR = 0,
		OPT_ADDITEM,
		OPT_REMOVEITEM,
		OPT_MONEY,
		OPT_REMOVEMONEY,
		OPT_ADDPET,
		OPT_REMOVEPET,
	};
	enum
	{
		ERR_ERR = 0,
		ERR_SELF_IN_EXCHANGE,
		ERR_TARGET_IN_EXCHANGE,
		ERR_DROP,
		ERR_ALREADY_LOCKED,
		ERR_ILLEGAL,
		ERR_NOT_ENOUGHT_ROOM_SELF,
		ERR_NOT_ENOUGHT_ROOM_OTHER,
		ERR_NOT_ENOUGHT_EXROOM,
		ERR_NOT_ENOUGHT_MONEY_SELF,
		ERR_NOT_ENOUGHT_MONEY_OTHER,
		ERR_TOO_FAR,
		ERR_REFUSE_TRADE,
		ERR_PET_LEVEL_TOO_HIGH,
	};
	struct _EXCHANGE_ITEM
	{
		BYTE			isBlueEquip;
		BYTE			byNumber;
		UINT			item_guid;
		_ITEM			item_data;
		_EXCHANGE_ITEM()
		{
			isBlueEquip = 0;
			byNumber = 0;
			item_guid = 0;
		}
	};
}
#endif