#ifndef _CG_REQRESETATTRRESULT_H_
#define _CG_REQRESETATTRRESULT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"




namespace Packets
{
	class GCReqResetAttrResult:	public Packet
	{
	public:
		GCReqResetAttrResult(){};
		virtual		~GCReqResetAttrResult(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_RESETATTRRESULT ; }
		virtual UINT			GetPacketSize()const { return sizeof(RATTR_RESULT)+sizeof(UINT); }


		VOID					SetAttrPoint(UINT Point){m_RemainAttrPoint = Point;}
		UINT					GetAttrPoint(){return m_RemainAttrPoint;}

		VOID					SetResult(RATTR_RESULT	ret){m_Result	=	ret;}
		RATTR_RESULT			GetResult(){return m_Result;}	

	private:
		UINT					m_RemainAttrPoint;
		RATTR_RESULT			m_Result;	
	
	};

	class GCReqResetAttrResultFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCReqResetAttrResult() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_GC_RESETATTRRESULT ; }
		UINT		GetPacketMaxSize()const		{ return sizeof(RATTR_RESULT)+sizeof(UINT); }			
	};

	class GCReqResetAttrResultHandler
	{
	public:
		static UINT Execute(GCReqResetAttrResult* pPacket,Player* pPlayer);
	};

}

using namespace Packets;


#endif