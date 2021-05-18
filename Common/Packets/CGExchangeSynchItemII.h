// CGExchangeSynchItemII.h
// 
// 
// 
//////////////////////////////////////////////////////

#ifndef __CGEXCHANGESYNCHITEMII_H__
#define __CGEXCHANGESYNCHITEMII_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"
#include "ExchangeMsgDefine.h"

namespace Packets
{
	class CGExchangeSynchItemII : public Packet
	{
	public:
		CGExchangeSynchItemII( )
		{
			m_Opt = 0;
			m_FromType = 0;
			m_ToType = 0;
			m_FromIndex = 0;
			m_ToIndex = 0;
		};
		virtual ~CGExchangeSynchItemII( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_EXCHANGESYNCHITEMII; }
		virtual UINT			GetPacketSize()const 
		{
			if(m_Opt == EXCHANGE_MSG::OPT_ADDITEM)
			{
				return sizeof(BYTE)*3;
			}
			else if(m_Opt == EXCHANGE_MSG::OPT_ADDPET)
			{
				return sizeof(BYTE) + sizeof(PET_GUID_t);
			}
			else if(m_Opt == EXCHANGE_MSG::OPT_REMOVEPET)
			{
				return sizeof(BYTE) + sizeof(PET_GUID_t);
			}
			else
			{
				return sizeof(BYTE)*4;
			}
		}

	public:
		BYTE					GetOpt(VOID) const {return m_Opt;};
		VOID					SetOpt(BYTE optID) {m_Opt = optID;};

		BYTE					GetFromType(VOID) const {return m_FromType;};
		VOID					SetFromType(BYTE FromType) {m_FromType = FromType;};

		BYTE					GetToType(VOID) const {return m_ToType;};
		VOID					SetToType(BYTE ToType) {m_ToType = ToType;};

		BYTE					GetFromIndex(VOID) const {return m_FromIndex;};
		VOID					SetFromIndex(BYTE FromIndex) {m_FromIndex = FromIndex;};

		BYTE					GetToIndex(VOID) const {return m_ToIndex;};
		VOID					SetToIndex(BYTE ToIndex) {m_ToIndex = ToIndex;};

		PET_GUID_t				GetPetGuid(VOID) const {return m_PetGuid;};
		VOID					SetPetGuid(PET_GUID_t PetGuid) {m_PetGuid = PetGuid;};

	private:
		BYTE					m_Opt;
		BYTE					m_FromType;
		BYTE					m_ToType;
		BYTE					m_FromIndex;
		BYTE					m_ToIndex;
		PET_GUID_t				m_PetGuid;
	};

	class CGExchangeSynchItemIIFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGExchangeSynchItemII() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_EXCHANGESYNCHITEMII; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE)*5 + sizeof(PET_GUID_t);};
	};

	class CGExchangeSynchItemIIHandler 
	{
	public:
		static UINT Execute( CGExchangeSynchItemII* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
