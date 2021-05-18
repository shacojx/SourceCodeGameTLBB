#ifndef _GC_REMOVEGEMRESULT_H_
#define _GC_REMOVEGEMRESULT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"




namespace Packets
{
	class GCRemoveGemResult:	public Packet
	{
	public:
		GCRemoveGemResult(){};
		virtual		~GCRemoveGemResult(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_REMOVEGEMRESULT ; }
		virtual UINT			GetPacketSize()const { return sizeof(REMOVEGEM_RESULT); }

		REMOVEGEM_RESULT		GetResult(){return m_Result;}
		VOID					SetResult(REMOVEGEM_RESULT	Ret){m_Result	= Ret;}
	private:

		REMOVEGEM_RESULT		m_Result;

	};

	class GCRemoveGemResultFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket()				{ return new GCRemoveGemResult() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_GC_REMOVEGEMRESULT ; }
		UINT		GetPacketMaxSize()const		{ return sizeof(REMOVEGEM_RESULT); }			
	};

	class GCRemoveGemResultHandler
	{
	public:
		static UINT Execute(GCRemoveGemResult* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif