#ifndef _GC_USEGEMRESULT_H_
#define _GC_USEGEMRESULT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"




namespace Packets
{
	class GCUseGemResult:	public Packet
	{
	public:
		GCUseGemResult(){};
		virtual		~GCUseGemResult(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_USEGEMRESULT ; }
		virtual UINT			GetPacketSize()const { return sizeof(USEITEM_RESULT); }

		USEGEM_RESULT			GetResult(){return m_Result;}
		VOID					SetResult(USEGEM_RESULT	Ret){m_Result	= Ret;}
	private:

		USEGEM_RESULT			m_Result;

	};

	class GCUseGemResultFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCUseGemResult() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_GC_USEGEMRESULT ; }
		UINT		GetPacketMaxSize()const		{ return sizeof(USEGEM_RESULT); }			
	};

	class GCUseGemResultHandler
	{
	public:
		static UINT Execute(GCUseGemResult* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif