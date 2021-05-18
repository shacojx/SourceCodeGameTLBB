// GCExchangeSynchII.h
// 
// 告诉客户端买卖是否成功
// 
//////////////////////////////////////////////////////

#ifndef __GCEXCHANGESYNCHII_H__
#define __GCEXCHANGESYNCHII_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"
#include "ExchangeMsgDefine.h"

namespace Packets
{
	class GCExchangeSynchII : public Packet
	{
	public:
		GCExchangeSynchII( )
		{
			m_IsMyself = 0;		
			m_Opt = 0;	
			m_FromType = 0;
			m_ToType = 0;
			m_FromIndex = 0;
			m_ToIndex = 0;
			m_Money = 0;
		}
		virtual ~GCExchangeSynchII( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_EXCHANGESYNCHII; }
		virtual UINT			GetPacketSize()const 
		{
			UINT length = 0;
			if(m_IsMyself)
			{//给自己盒子里加东西
				switch (m_Opt)
				{
				case EXCHANGE_MSG::OPT_ADDITEM:
					{
						return sizeof(BYTE)/*m_IsMyself*/
							+sizeof(BYTE)/*m_Opt*/
							+sizeof(BYTE)/*m_FromType*/
							+sizeof(BYTE)/*m_FromIndex*/
							+sizeof(BYTE);/*m_ToIndex*/
					}
					break;
				case EXCHANGE_MSG::OPT_REMOVEITEM:
					{
						return sizeof(BYTE)/*m_IsMyself*/
							+sizeof(BYTE)/*m_Opt*/
							+sizeof(BYTE)/*m_ToType*/
							+sizeof(BYTE)/*m_ToIndex*/
							+sizeof(BYTE);/*m_FromIndex*/					
					}
					break;
				case EXCHANGE_MSG::OPT_MONEY:
					return sizeof(BYTE)/*m_IsMyself*/
						+sizeof(BYTE)/*m_Opt*/
						+sizeof(UINT);/*m_Money*/					
					break;
				case EXCHANGE_MSG::OPT_REMOVEMONEY:
					return sizeof(BYTE)/*m_IsMyself*/
						+sizeof(BYTE)/*m_Opt*/
						+sizeof(UINT);/*m_Money*/					
					break;
				case EXCHANGE_MSG::OPT_ADDPET:
					return sizeof(BYTE)/*m_IsMyself*/
						+sizeof(BYTE)/*m_Opt*/
						+sizeof(BYTE)/*m_ToIndex*/
						+sizeof(PET_GUID_t);/*m_PetGuid*/
					break;
				case EXCHANGE_MSG::OPT_REMOVEPET:
					return sizeof(BYTE)/*m_IsMyself*/
						+sizeof(BYTE)/*m_Opt*/
						+sizeof(PET_GUID_t);/*m_PetGuid*/
					break;
				default:
					return	length;
					break;
				}
			}
			else
			{//给对方盒子里加东西
				switch (m_Opt)
				{
				case EXCHANGE_MSG::OPT_ADDITEM:
					{
						length += sizeof(BYTE)/*m_IsMyself*/+sizeof(BYTE)/*m_Opt*/;
						length += sizeof(BYTE)/*m_ToIndex*/;
						if(m_ExItem.isBlueEquip)
						{
							length += sizeof(BYTE)/*isBlueEquip*/+sizeof(BYTE)/*byNumber*/+sizeof(_ITEM)/*item_data*/;
						}
						else
						{
							length += sizeof(BYTE)/*isBlueEquip*/+sizeof(BYTE)/*byNumber*/+sizeof(UINT)/*item_guid*/;
						}
						return	length; 
					}
					break;
				case EXCHANGE_MSG::OPT_REMOVEITEM:
					return sizeof(BYTE)/*m_IsMyself*/
						+sizeof(BYTE)/*m_Opt*/
						+sizeof(BYTE);/*m_FromIndex*/					
					break;
				case EXCHANGE_MSG::OPT_MONEY:
					return sizeof(BYTE)/*m_IsMyself*/
						+sizeof(BYTE)/*m_Opt*/
						+sizeof(UINT);/*m_Money*/					
					break;
				case EXCHANGE_MSG::OPT_REMOVEMONEY:
					return sizeof(BYTE)/*m_IsMyself*/
						+sizeof(BYTE)/*m_Opt*/
						+sizeof(UINT);/*m_Money*/					
					break;
				case EXCHANGE_MSG::OPT_ADDPET:
					return sizeof(BYTE)/*m_IsMyself*/
						+sizeof(BYTE);/*m_Opt*/
					break;
				case EXCHANGE_MSG::OPT_REMOVEPET:
					return sizeof(BYTE)/*m_IsMyself*/
						+sizeof(BYTE)/*m_Opt*/
						+sizeof(PET_GUID_t);/*m_PetGuid*/
					break;
				default:
					return	length;
					break;
				}

			}
			
		}

	public:
		BYTE					GetIsMyself(VOID) const {return m_IsMyself;};
		VOID					SetIsMyself(BYTE IsMyself) {m_IsMyself = IsMyself;};

		BYTE					GetOpt(VOID) const {return m_Opt;};
		VOID					SetOpt(BYTE Opt) {m_Opt = Opt;};

		BYTE					GetFromType(VOID) const {return m_FromType;};
		VOID					SetFromType(BYTE FromType) {m_FromType = FromType;};

		BYTE					GetToType(VOID) const {return m_ToType;};
		VOID					SetToType(BYTE ToType) {m_ToType = ToType;};

		BYTE					GetFromIndex(VOID) const {return m_FromIndex;};
		VOID					SetFromIndex(BYTE FromIndex) {m_FromIndex = FromIndex;};

		BYTE					GetToIndex(VOID) const {return m_ToIndex;};
		VOID					SetToIndex(BYTE ToIndex) {m_ToIndex = ToIndex;};

		UINT					GetMoney(VOID) const {return m_Money;};
		VOID					SetMoney(UINT Money) {m_Money = Money;};

		EXCHANGE_MSG::_EXCHANGE_ITEM*		GetItem(VOID) {return &m_ExItem;};
		VOID								SetItem(EXCHANGE_MSG::_EXCHANGE_ITEM* pItem)
		{
			memcpy(&m_ExItem, pItem, sizeof(EXCHANGE_MSG::_EXCHANGE_ITEM));
		}

		PET_GUID_t				GetPetGuid(VOID) const {return m_PetGuid;};
		VOID					SetPetGuid(PET_GUID_t PetGuid) {m_PetGuid = PetGuid;};

	private:
		BYTE							m_IsMyself;		
		BYTE							m_Opt;	
		BYTE							m_FromType;
		BYTE							m_ToType;
		BYTE							m_FromIndex;
		BYTE							m_ToIndex;
		UINT							m_Money;
		EXCHANGE_MSG::_EXCHANGE_ITEM	m_ExItem;
		PET_GUID_t						m_PetGuid;
	};

	class GCExchangeSynchIIFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCExchangeSynchII() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_EXCHANGESYNCHII; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE)*6 + sizeof(UINT) + sizeof(BYTE)*2 + sizeof(UINT) + sizeof(_ITEM) + sizeof(PET_GUID_t);};
	};

	class GCExchangeSynchIIHandler 
	{
	public:
		static UINT Execute( GCExchangeSynchII* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
