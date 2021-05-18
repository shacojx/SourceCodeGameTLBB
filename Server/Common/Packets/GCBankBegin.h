// GCBankBegin.h
// 
// 告诉客户端买卖是否成功
// 
//////////////////////////////////////////////////////

#ifndef __GCBANKBEGIN_H__
#define __GCBANKBEGIN_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCBankBegin : public Packet
	{
	public:
		GCBankBegin( )
		{
			m_BankID = 0;
			m_NPCID = 0;
		}
		virtual ~GCBankBegin( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_BANKBEGIN; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE)+ sizeof(ObjID_t);}

	public:
		BYTE					GetBankID(VOID) const {return m_BankID;};
		VOID					SetBankID(BYTE BankID) {m_BankID = BankID;};

		ObjID_t					GetNPCObjID(VOID) const {return m_NPCID;};
		VOID					SetNPCObjID(ObjID_t NPCID) {m_NPCID = NPCID;};


	private:
		BYTE					m_BankID;		//成功与否
		ObjID_t					m_NPCID;
	};

	class GCBankBeginFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCBankBegin() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_BANKBEGIN; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE)+ sizeof(ObjID_t);};
	};

	class GCBankBeginHandler 
	{
	public:
		static UINT Execute( GCBankBegin* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
