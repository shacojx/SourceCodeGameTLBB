
#pragma once

#include "Type.h"

struct PLAYERSHOP_APPLY_INFO
{
	FLOAT							m_ComFactor;					//当前商业指数
	UINT							m_Cost;							//开店金额
	BYTE							m_Type;							//可开的类型
	PLAYERSHOP_APPLY_INFO()
	{
		memset(this, 0, sizeof(PLAYERSHOP_APPLY_INFO));
	}
};

struct PLAYERSHOP_INFO
{
	STRING							m_szShopName;					//商店名
	STRING							m_szShopDesc;					//商店描述
	STRING							m_szOwnerName;					//摊主名
	BYTE							m_StallNumOpened;				//开放的柜台数
	BYTE							m_StallNumOnSale;				//开张的柜台数
	BYTE							m_Type;							//店铺的类型
	_PLAYERSHOP_GUID				m_ID;							//店铺的唯一ID
	UINT							m_OwnerGuid;					//摊主GUID
	STRING							m_szFoundedTime;				//建立时间
	BYTE							m_Serial;						//商店序列号
	BYTE							m_bIsFavor;						//是否在收藏家内

	PLAYERSHOP_INFO()
	{
		m_szShopName	=	"";				//商店名
		m_szShopDesc	=	"";				//商店描述
		m_szOwnerName	=	"";				//摊主名
		m_StallNumOpened=	0;				//开放的柜台数
		m_StallNumOnSale=	0;				//开张的柜台数
		m_Type			=	0;				//店铺的类型
		m_ID.Reset();						//店铺的唯一ID
		m_OwnerGuid		=	0;				//摊主GUID
		m_szFoundedTime	=	"";				//建立时间
		m_Serial		=	0;
		m_bIsFavor		=	0;				//是否在收藏家内
	}
};
//------------
// 玩家商店合伙人
//------------

struct PlayerShopPartners_t
{
	GUID_t		m_Guid;
	STRING		m_szName;
	VOID		CleanUp()
	{
		m_Guid = INVALID_GUID;
		m_szName = "";
	}
};