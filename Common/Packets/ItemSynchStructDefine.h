#ifndef __ITEMSYNCHSTRUCTDEFINE_H__
#define __ITEMSYNCHSTRUCTDEFINE_H__

#include "Type.h"
namespace ITEM_SYNCH_STRUCT
{
	//CG
	
	//从背包中移动物品到玩家商店，自动寻找位置
	struct CGAutoMoveItemFromBagToPlayerShop_t
	{
		_PLAYERSHOP_GUID	m_ShopGuid;							//商店GUID
		BYTE				m_nStallIndex;						//柜台索引
		BYTE				m_nShopSerial;						//商店序列号
		CGAutoMoveItemFromBagToPlayerShop_t()
		{
			m_nStallIndex = 0;
			m_nShopSerial = 0;
		}
		UINT GetSize()
		{
			return	sizeof(CGAutoMoveItemFromBagToPlayerShop_t);
		}
	};

	//从背包中移动物品到玩家商店，自动寻找位置
	struct CGAutoMovePetFromBagToPlayerShop_t
	{
		_PLAYERSHOP_GUID	m_ShopGuid;							//商店GUID
		BYTE				m_nStallIndex;						//柜台索引
		BYTE				m_nShopSerial;						//商店序列号
		CGAutoMovePetFromBagToPlayerShop_t()
		{
			m_nStallIndex = 0;
			m_nShopSerial = 0;
		}
		UINT GetSize()
		{
			return	sizeof(CGAutoMovePetFromBagToPlayerShop_t);
		}
	};

	//从玩家商店移动物品到背包，自动寻找位置
	struct CGAutoMoveItemFromPlayerShopToBag_t
	{
		_PLAYERSHOP_GUID	m_ShopGuid;							//商店GUID
		BYTE				m_nStallIndex;						//柜台索引
		UINT				m_uSerial;							//物品序列号
		BYTE				m_nShopSerial;						//商店序列号
		CGAutoMoveItemFromPlayerShopToBag_t()
		{
			m_nStallIndex	=	0;
			m_uSerial		=	0;
			m_nShopSerial	=	0;
		}
		UINT GetSize()
		{
			return	sizeof(CGAutoMoveItemFromPlayerShopToBag_t);
		}
	};

	//从玩家商店移动物品到背包，自动寻找位置
	struct CGAutoMovePetFromPlayerShopToBag_t
	{
		_PLAYERSHOP_GUID	m_ShopGuid;							//商店GUID
		BYTE				m_nStallIndex;						//柜台索引
		UINT				m_uSerial;							//物品序列号
		BYTE				m_nShopSerial;						//商店序列号
		CGAutoMovePetFromPlayerShopToBag_t()
		{
			m_nStallIndex	=	0;
			m_uSerial		=	0;
			m_nShopSerial	=	0;
		}
		UINT GetSize()
		{
			return	sizeof(CGAutoMovePetFromPlayerShopToBag_t);
		}
	};

	//从背包移动物品到玩家商店，指定位置
	struct CGManuMoveItemFromBagToPlayerShop_t
	{
		_PLAYERSHOP_GUID	m_ShopGuid;							//商店GUID
		BYTE				m_nStallIndex;						//柜台索引
		UINT				m_uSerial;							//物品序列号
		BYTE				m_nShopSerial;						//商店序列号
		CGManuMoveItemFromBagToPlayerShop_t()
		{
			m_nStallIndex	=	0;
			m_uSerial		=	0;
			m_nShopSerial	=	0;
		}
		UINT GetSize()
		{
			return	sizeof(CGManuMoveItemFromBagToPlayerShop_t);
		}
	};

	//从玩家商店移动物品到背包，指定位置
	struct CGManuMoveItemFromPlayerShopToBag_t
	{
		_PLAYERSHOP_GUID	m_ShopGuid;							//商店GUID
		BYTE				m_nStallIndex;						//柜台索引
		UINT				m_uSerial;							//物品序列号
		BYTE				m_nShopSerial;						//商店序列号
		CGManuMoveItemFromPlayerShopToBag_t()
		{
			m_nStallIndex	=	0;
			m_uSerial		=	0;
			m_nShopSerial	=	0;
		}
		UINT GetSize()
		{
			return	sizeof(CGManuMoveItemFromPlayerShopToBag_t);
		}
	};
    
	//从玩家商店移动物品到玩家商店，指定位置
	struct CGManuMoveItemFromPlayerShopToPlayerShop_t
	{
		_PLAYERSHOP_GUID	m_ShopGuid;							//商店GUID
		BYTE				m_nStallIndex;						//柜台索引
		UINT				m_uSerialSource;					//源位置序列号
		UINT				m_uSerialDest;						//目的位置序列号
		BYTE				m_nShopSerial;						//商店序列号
		CGManuMoveItemFromPlayerShopToPlayerShop_t()
		{
			m_nStallIndex		=	0;
			m_uSerialSource		=	0;
			m_uSerialDest		=	0;
			m_nShopSerial		=	0;
		}
		UINT GetSize()
		{
			return	sizeof(CGManuMoveItemFromPlayerShopToPlayerShop_t);
		}
	};

	//GC
	//从背包移动物品到玩家商店
	struct GCMoveItemFromBagToPlayerShop_t
	{
		_PLAYERSHOP_GUID	m_ShopGuid;							//商店GUID
		BYTE				m_nStallIndex;						//柜台索引
		UINT				m_uSerial;							//物品序列号
		BYTE				m_nShopSerial;						//商店序列号
		GCMoveItemFromBagToPlayerShop_t()
		{
			m_nStallIndex		=	0;
			m_uSerial			=	0;
			m_nShopSerial		=	0;
		}
		UINT GetSize()
		{
			return	sizeof(GCMoveItemFromBagToPlayerShop_t);
		}
	};

	//从背包移动物品到玩家商店
	struct GCMovePetFromBagToPlayerShop_t
	{
		_PLAYERSHOP_GUID	m_ShopGuid;							//商店GUID
		BYTE				m_nStallIndex;						//柜台索引
		UINT				m_uSerial;							//物品序列号
		BYTE				m_nShopSerial;						//商店序列号
		GCMovePetFromBagToPlayerShop_t()
		{
			m_nStallIndex		=	0;
			m_uSerial			=	0;
			m_nShopSerial		=	0;
		}
		UINT GetSize()
		{
			return	sizeof(GCMovePetFromBagToPlayerShop_t);
		}
	};

	//从玩家商店移动物品到背包，指定位置
	struct GCMoveItemFromPlayerShopToBag_t
	{
		_PLAYERSHOP_GUID	m_ShopGuid;							//商店GUID
		BYTE				m_nStallIndex;						//柜台索引
		BYTE				m_Flag;								//标志
		UINT				m_uSerial;							//物品序列号
		BYTE				m_nShopSerial;						//商店序列号
		GCMoveItemFromPlayerShopToBag_t()
		{
			m_nStallIndex		=	0;
			m_Flag				=	0;
			m_uSerial			=	0;
			m_nShopSerial		=	0;
		}
		UINT GetSize()
		{
			return	sizeof(GCMoveItemFromPlayerShopToBag_t);
		}
	};

	//从玩家商店移动物品到背包，指定位置
	struct GCMovePetFromPlayerShopToBag_t
	{
		_PLAYERSHOP_GUID	m_ShopGuid;							//商店GUID
		BYTE				m_nStallIndex;						//柜台索引
		BYTE				m_Flag;								//标志
		UINT				m_uSerial;							//物品序列号
		BYTE				m_nShopSerial;						//商店序列号
		GCMovePetFromPlayerShopToBag_t()
		{
			m_nStallIndex		=	0;
			m_Flag				=	0;
			m_uSerial			=	0;
			m_nShopSerial		=	0;
		}
		UINT GetSize()
		{
			return	sizeof(GCMovePetFromPlayerShopToBag_t);
		}
	};

	//从玩家商店移动物品到玩家商店
	struct GCMoveItemFromPlayerShopToPlayerShop_t
	{
		_PLAYERSHOP_GUID	m_ShopGuid;							//商店GUID
		BYTE				m_nStallIndex;						//柜台索引
		UINT				m_uSerialSource;					//物品序列号
		UINT				m_uSerialDest;						//物品序列号
		BYTE				m_nShopSerial;						//商店序列号

		GCMoveItemFromPlayerShopToPlayerShop_t()
		{
			m_nStallIndex		=	0;
			m_uSerialSource		=	0;
			m_uSerialDest		=	0;
			m_nShopSerial		=	0;
		}
		UINT GetSize()
		{
			return	sizeof(GCMoveItemFromPlayerShopToPlayerShop_t);
		}
	};

	//从背包与玩家商店交换物品
	struct GCExchangeItemFromBagToPlayerShop_t
	{
		_PLAYERSHOP_GUID	m_ShopGuid;							//商店GUID
		BYTE				m_nStallIndex;						//柜台索引
		UINT				m_uSerial;							//物品序列号
		BYTE				m_nShopSerial;						//商店序列号
		GCExchangeItemFromBagToPlayerShop_t()
		{
			m_nStallIndex		=	0;
			m_uSerial			=	0;
			m_nShopSerial		=	0;
		}
		UINT GetSize()
		{
			return	sizeof(GCExchangeItemFromBagToPlayerShop_t);
		}
	};

	//从玩家商店交换物品到背包
	struct GCExchangeItemFromPlayerShopToBag_t
	{
		_PLAYERSHOP_GUID	m_ShopGuid;							//商店GUID
		BYTE				m_nStallIndex;						//柜台索引
		UINT				m_uSerial;							//物品序列号
		BYTE				m_nShopSerial;						//商店序列号
		GCExchangeItemFromPlayerShopToBag_t()
		{
			m_nStallIndex		=	0;
			m_uSerial			=	0;
			m_nShopSerial		=	0;
		}
		UINT GetSize()
		{
			return	sizeof(GCExchangeItemFromPlayerShopToBag_t);
		}
	};

	//从玩家商店交换物品到玩家商店
	struct GCExchangeItemFromPlayerShopToPlayerShop_t
	{
		_PLAYERSHOP_GUID	m_ShopGuid;							//商店GUID
		BYTE				m_nStallIndex;						//柜台索引
		UINT				m_uSerialSource;					//物品序列号
		UINT				m_uSerialDest;						//物品序列号
		BYTE				m_nShopSerial;						//商店序列号

		GCExchangeItemFromPlayerShopToPlayerShop_t()
		{
			m_nStallIndex		=	0;
			m_uSerialSource		=	0;
			m_uSerialDest		=	0;
			m_nShopSerial		=	0;
		}
		UINT GetSize()
		{
			return	sizeof(GCExchangeItemFromPlayerShopToPlayerShop_t);
		}
	};

	//ItemList
	struct GCPlayerShopItemListForSelf_t
	{
		enum
		{
			FOR_BUYER = 1,
			FOR_MANAGER,
		};
		BYTE					m_bFlag;								//标志
		_PLAYERSHOP_GUID		m_ShopGuid;								//商店GUID
		CHAR					m_ShopName[MAX_SHOP_NAME_SIZE];			//商店名
		CHAR					m_ShopDesc[MAX_SHOP_DESC_SIZE];			//商店描述
		GUID_t					m_OwnerGuid;							//店主GUID
		CHAR					m_OwnerName[MAX_CHARACTER_NAME];		//店主名
		UINT					m_uBaseMoney;							//本金
		UINT					m_uProfitMoney;							//赢利金
		BYTE					m_aStallIsOpen[MAX_STALL_NUM_PER_SHOP];	//此柜台是否开张
		BYTE					m_bIsItemList;
		BYTE					m_bUIFlag;								//UI需要的一个标志
		BYTE					m_bIsSaleOut;							//是否是盘出状态
		UINT					m_uSaleOutPrice;						//盘出价
		BYTE					m_Serial;								//商店序列号
		BYTE					m_ExRecListNum;							//交易记录数量
		BYTE					m_MaRecListNum;							//管理记录数量
		BYTE					m_bStallNum;							//摊位数量
		BYTE					m_bType;								//商店类型
		FLOAT					m_ComFactor;							//商业指数
	
		GCPlayerShopItemListForSelf_t()
		{
			m_bFlag					=	0;
			m_uBaseMoney			=	0;
			m_uProfitMoney			=	0;
			m_OwnerGuid				=	0;
			m_bIsItemList			=	0;
			m_bUIFlag				=	0;
			m_bIsSaleOut			=	0;
			m_uSaleOutPrice			=	0;
			m_Serial				=	0;
			m_ExRecListNum			=	0;
			m_MaRecListNum			=	0;
			m_bStallNum				=	0;
			m_bType					=	0;
			m_ComFactor				=	0;
			memset(m_ShopName, 0, MAX_SHOP_NAME_SIZE);
			memset(m_ShopDesc, 0, MAX_SHOP_DESC_SIZE);
			memset(m_OwnerName, 0, MAX_CHARACTER_NAME);
			memset(m_aStallIsOpen, 0, MAX_STALL_NUM_PER_SHOP);
		}
		static UINT GetSize()
		{
			return	sizeof(GCPlayerShopItemListForSelf_t);
		}
	};

    struct GCPlayerShopItemListEachItemForSelf_t
	{
		BYTE	m_nStallIndex;
		UINT	m_ItemPrice;
		BYTE	m_ItemSerial;
		BYTE	m_IsMine;
		BYTE	m_IsOnSale;

		GCPlayerShopItemListEachItemForSelf_t()
		{
			m_nStallIndex = 0;
			m_ItemPrice = 0;
			m_ItemSerial = 0;
			m_IsMine = 0;
			m_IsOnSale = 0;
		}
		UINT GetSize()
		{
			return	sizeof(GCPlayerShopItemListEachItemForSelf_t);

		}
	};

	struct GCPlayerShopItemListEachSerialForSelf_t
	{
		BYTE	m_nStallIndex;
		BYTE	m_ItemSerial;

		GCPlayerShopItemListEachSerialForSelf_t()
		{
			m_nStallIndex	= 0;
			m_ItemSerial	= 0;
		}
		UINT GetSize()
		{
			return	sizeof(GCPlayerShopItemListEachSerialForSelf_t);

		}
	};

	struct GCPlayerShopPetListForSelf_t
	{
		BYTE					m_bUIFlag;						//UI需要的一个标志
	
		GCPlayerShopPetListForSelf_t()
		{
			m_bUIFlag		= 0;
		}
		static UINT GetSize()
		{
			return	sizeof(GCPlayerShopPetListForSelf_t);
		}
	};

	struct GCPlayerShopItemListForOther_t
	{
		BYTE					m_bFlag;						//标志1.非本人接收 2.本人接收
		_PLAYERSHOP_GUID		m_ShopGuid;						//商店GUID
		CHAR					m_ShopName[MAX_SHOP_NAME_SIZE];	//商店名
		CHAR					m_ShopDesc[MAX_SHOP_DESC_SIZE];	//描述
		GUID_t					m_OwnerGuid;					//店主GUID
		CHAR					m_OwnerName[MAX_CHARACTER_NAME];//店主名
		BYTE					m_aStallIsOpen[MAX_STALL_NUM_PER_SHOP];	//此柜台是否开张
		BYTE					m_bIsItemList;
		BYTE					m_bUIFlag;						//UI需要的一个标志
		BYTE					m_bIsSaleOut;					//此店是否已经盘出
		UINT					m_uBaseMoney;					//本金
		UINT					m_uProfitMoney;					//赢利金
		UINT					m_uSaleOutPrice;				//盘出价格
		BYTE					m_Serial;						//商店序列号
		BYTE					m_bStallNum;					//拥有的柜台数

		GCPlayerShopItemListForOther_t()
		{
			m_bFlag				= 0;
			m_OwnerGuid			= 0;
			m_bIsItemList		= 0;
			m_bUIFlag			= 0;
			m_bIsSaleOut		= 0;
			m_uBaseMoney		= 0;
			m_uProfitMoney		= 0;
			m_uSaleOutPrice		= 0;
			m_Serial			= 0;
			m_bStallNum		= 0;
			memset(m_ShopName, 0, MAX_SHOP_NAME_SIZE);
			memset(m_ShopDesc, 0, MAX_SHOP_DESC_SIZE);
			memset(m_OwnerName, 0, MAX_CHARACTER_NAME);
			memset(m_aStallIsOpen, 0, MAX_STALL_NUM_PER_SHOP);
		}
		static UINT GetSize()
		{
			return	sizeof(GCPlayerShopItemListForOther_t);
		}
	};

	struct GCPlayerShopItemListEachItemForOther_t
	{
		BYTE				m_nStallIndex;				//柜台索引
		UINT				m_uPrice;					//物品价格
		BYTE				m_uSerial;					//物品序列号
		BYTE				m_bIsMine;					//是否是自己

		GCPlayerShopItemListEachItemForOther_t()
		{
			m_nStallIndex	=	0;				//柜台索引
			m_uPrice		=	0;				//物品价格
			m_uSerial		=	0;				//物品序列号
			m_bIsMine		=	0;				//是否是自己
		}
		static UINT GetSize()
		{
			return	sizeof(GCPlayerShopItemListEachItemForOther_t);
		}
	};

	struct GCPlayerShopPetListForOther_t
	{
		BYTE					m_bUIFlag;				//UI  需要的一个标志
	
		GCPlayerShopPetListForOther_t()
		{
			m_bUIFlag		= 0;
		}
		static UINT GetSize()
		{
			return	sizeof(GCPlayerShopPetListForOther_t);
		}
	};




}

#endif
