#ifndef _GC_MANUALATTR_RESULT_H_
#define _GC_MANUALATTR_RESULT_H_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


		class GCManualAttrResult:	public	Packet
		{
			public:
				GCManualAttrResult()	{};
				~GCManualAttrResult()	{};

				//公用接口
				virtual BOOL			Read( SocketInputStream& iStream ) ;
				virtual BOOL			Write( SocketOutputStream& oStream )const ;
				virtual UINT			Execute( Player* pPlayer ) ;

				virtual PacketID_t		GetPacketID()const { return PACKET_GC_MANUALATTRRESULT ; }
				virtual UINT			GetPacketSize()const { return sizeof(ATTR_RESUlT); }

				ATTR_RESUlT				GetResult(){return	m_Result;}
				VOID					SetResult(ATTR_RESUlT ret){m_Result	= ret;}
				
				//CHAR_ATTR_LEVEL1		GetAttrType(){return m_AttrType;}
				//VOID					SetAttrType(CHAR_ATTR_LEVEL1 type){m_AttrType = type;}
			private:

				ATTR_RESUlT				m_Result;

				//CHAR_ATTR_LEVEL1		m_AttrType;
		};	

		class GCManualAttrResultFactory:	public PacketFactory
		{
		public:
			Packet*		CreatePacket() { return new GCManualAttrResult() ; }
			PacketID_t	GetPacketID()const			{ return PACKET_GC_MANUALATTRRESULT ; }
			UINT		GetPacketMaxSize()const		{ return sizeof(ATTR_RESUlT); }			
		};

		class GCManualAttrResultHandler
		{
		public:
			static UINT Execute(GCManualAttrResult* pPacket,Player* pPlayer);
		};

}

using namespace Packets;

#endif