/************************************************************************/
/*
创建日期:	2006年3月2日
创建时间:	11:54
文件名称:	PetDetailExtraStructDefine.h

文件功能：	宠物消息的附加信息都定义在这里
*/
/************************************************************************/

#ifndef __PETDETAILEXTRASTRUCTDEFINE_H__
#define __PETDETAILEXTRASTRUCTDEFINE_H__

#include "Type.h"
namespace PET_EXTRA_STRUCT
{
	struct GCExchangePetView_t
	{
		BYTE				m_bFlag;							//标志此消息的用途

		GCExchangePetView_t()
		{
			m_bFlag			=	0;							//标志此消息的用途
		}
		static UINT GetSize()
		{
			return	sizeof(GCExchangePetView_t);
		}
	};


	struct GCPlayerShopPetView_t
	{
		BYTE				m_bFlag;							//标志此消息的用途
		INT					m_nPrice;							//宠物价格
		INT					m_nSerial;							//宠物序列号
		BYTE				m_bIsMine;							//是否是自己察看
		BYTE				m_nStallIndex;						//所在柜台号
		BYTE				m_bIsOnSale;						//是否在架上

		GCPlayerShopPetView_t()
		{
			m_bFlag			=	0;							//标志此消息的用途
			m_nPrice		=	0;							//宠物价格
			m_nSerial		=	0;							//宠物序列号
			m_bIsMine		=	0;							//是否是自己察看
			m_nStallIndex	=	0;							//所在柜台号
			m_bIsOnSale		=	0;							//是否在架上

		}
		static UINT GetSize()
		{
			return	sizeof(GCPlayerShopPetView_t);
		}
	};

	struct GCStallPetView_t
	{
		BYTE				m_bFlag;							//标志此消息的用途
		INT					m_nPrice;							//宠物价格
		INT					m_nSerial;							//宠物序列号

		GCStallPetView_t()
		{
			m_bFlag			=	0;							//标志此消息的用途
			m_nPrice		=	0;							//宠物价格
			m_nSerial		=	0;							//宠物序列号
		}
		static UINT GetSize()
		{
			return	sizeof(GCStallPetView_t);
		}
	};
}

#endif
