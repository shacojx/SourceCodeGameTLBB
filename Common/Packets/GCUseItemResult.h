#ifndef _GC_USEITEMRESULT_H_
#define _GC_USEITEMRESULT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"




namespace Packets
{
	class GCUseItemResult:	public Packet
	{
	public:
		GCUseItemResult(){};
		virtual		~GCUseItemResult(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_USEITEMRESULT ; }
		virtual UINT			GetPacketSize()const { return sizeof(USEITEM_RESULT); }
		
		USEITEM_RESULT			GetResult(){return m_Result;}
		VOID					SetResult(USEITEM_RESULT	Ret){m_Result	= Ret;}
	private:
		
		USEITEM_RESULT			m_Result;

	};

	class GCUseItemResultFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCUseItemResult() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_GC_USEITEMRESULT ; }
		UINT		GetPacketMaxSize()const		{ return sizeof(USEITEM_RESULT); }			
	};

	class GCUseItemResultHandler
	{
	public:
		static UINT Execute(GCUseItemResult* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif