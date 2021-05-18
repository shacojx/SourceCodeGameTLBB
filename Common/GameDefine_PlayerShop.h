#ifndef __GAMEDEFINE_PLAYERSHOP_H__
#define __GAMEDEFINE_PLAYERSHOP_H__

namespace PLAYER_SHOP
{
	enum STATUS_STALL
	{
		STALL_INVALID = 0,					//此柜台还没有开放给玩家使用
		STALL_CLOSE,						//此柜台已经打烊
		STALL_OPEN,							//此柜台已经开张
	};
}

#endif // __GAMEDEFINE_PLAYERSHOP_H__
