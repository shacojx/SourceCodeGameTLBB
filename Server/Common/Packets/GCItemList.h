// GCItemList.h
// 将物品列表发送给客户端
// 
//////////////////////////////////////////////////////

#ifndef __GCITEMLIST_H__
#define __GCITEMLIST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "ItemSynchStructDefine.h"
#define MAX_MSG_ITEM			MAX_STALL_NUM_PER_SHOP*STALL_BOX_SIZE
#define MAX_EXTRA_LENGTH		200


namespace Packets
{
	class GCItemList : public Packet
	{
	public:

		enum
		{
			OPT_NONE  = 0,
			OPT_ADD_ONE_ITEM,
			OPT_ADD_ITEM_LIST,
		};

		enum
		{
			TYPE_ITEM  = 0,
			TYPE_PET,
			TYPE_SERIALS,
		};

	public:
		struct _MSG_ITEM
		{
			BYTE			nType;							//物品类型 物品 0, 宠物 1, 序列号 2
			BYTE			nIndex;							//界面中的位置
			_ITEM			item_data;						//物品数据
			BYTE			ExtraLength;					//附加信息长度
			CHAR			ExtraInfo[MAX_EXTRA_LENGTH];	//附加信息内容

			_MSG_ITEM()
			{
				nType		=	0;
				nIndex		=	0;
				ExtraLength	=	0;
				memset(ExtraInfo, 0, MAX_EXTRA_LENGTH);
			}

			UINT	GetSize() const
			{
				UINT SelfLength = 0;
				if(nType == TYPE_ITEM)
				{
					SelfLength += sizeof(BYTE);//nType
					SelfLength += sizeof(BYTE);//nIndex
					SelfLength += sizeof(_ITEM);//item_data
					SelfLength += sizeof(BYTE);//ExtraLength
					if( ExtraLength > 0 )
					{
						SelfLength += ExtraLength;//ExtraInfo
					}
				}
				else if(nType == TYPE_PET)
				{

				}
				else if(nType == TYPE_SERIALS)
				{
					SelfLength += sizeof(BYTE);//nType
					SelfLength += sizeof(BYTE);//nIndex
					SelfLength += sizeof(BYTE);//ExtraLength
					if( ExtraLength > 0 )
					{
						SelfLength += ExtraLength;//ExtraInfo
					}
				}
				return SelfLength;
			}
		};
	public:
		GCItemList( )
		{
			m_Opt			=	0;
			m_ItemNum		=	0;
			m_ExtraLength	=	0;
			memset(m_ExtraInfo, 0, MAX_EXTRA_LENGTH);
		}
		virtual ~GCItemList( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_ITEMLIST; }
		virtual UINT			GetPacketSize()const 
		{
			UINT	nLength = 0;
			nLength += sizeof(BYTE);//m_Opt
			switch(m_Opt)
			{
			case OPT_ADD_ONE_ITEM:
				{
					nLength += m_ItemList[0].GetSize();
				}
				break;

			case OPT_ADD_ITEM_LIST:
				{
					nLength += sizeof(BYTE);//m_ItemNum
					for(UINT i = 0; i<m_ItemNum; i++)
					{
						nLength += m_ItemList[i].GetSize();					
					}
				}
				break;

			default:
				break;
			}

			//ExtraInfo
			nLength += sizeof(BYTE);//m_ExtraLength
			if(m_ExtraLength>0)
			{
				nLength += m_ExtraLength;//m_ExtraInfo
			}

			return	nLength;
		}

	public:
		BYTE					GetOpt(VOID) const {return m_Opt;};
		VOID					SetOpt(BYTE optID) {m_Opt = optID;};

		BYTE					GetItemNum(VOID) const {return m_ItemNum;};
		VOID					SetItemNum(BYTE Num) {m_ItemNum = Num;};

		_MSG_ITEM*				GetItemList(VOID) {return m_ItemList;};
		VOID					SetItemList(_MSG_ITEM* ItemList) 
		{
			if(m_Opt == OPT_ADD_ONE_ITEM)
			{
				memcpy(m_ItemList, ItemList, sizeof(_MSG_ITEM));
			}
			else if(m_Opt == OPT_ADD_ITEM_LIST)
			{
				memcpy(m_ItemList, ItemList, m_ItemNum*sizeof(_MSG_ITEM));
			}
		}

		BYTE					GetExtraLength(VOID) const {return m_ExtraLength;};
		VOID					SetExtraLength(BYTE ExtraLength) {m_ExtraLength = ExtraLength;};

		CHAR*					GetExtraInfo(VOID) {return m_ExtraInfo;};
		VOID					SetExtraInfo(CHAR* ExtraInfo) 
		{
			memcpy(m_ExtraInfo, ExtraInfo, m_ExtraLength);
		}

	private:
		BYTE		m_Opt;
		BYTE		m_ItemNum;
		_MSG_ITEM	m_ItemList[MAX_MSG_ITEM];
		BYTE		m_ExtraLength;
		CHAR		m_ExtraInfo[MAX_EXTRA_LENGTH];
	};

	class GCItemListFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCItemList() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_ITEMLIST; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE) 
													+ sizeof(BYTE)
													+ sizeof(GCItemList::_MSG_ITEM)*MAX_MSG_ITEM
													+ sizeof(BYTE)
													+ sizeof(CHAR)*MAX_EXTRA_LENGTH;
											};
	};

	class GCItemListHandler 
	{
	public:
		static UINT Execute( GCItemList* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
