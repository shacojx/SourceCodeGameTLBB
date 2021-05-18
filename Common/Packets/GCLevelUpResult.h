#ifndef	_GC_LEVELUP_RESULT_H_
#define _GC_LEVELUP_RESULT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"




namespace Packets
{
		
		class GCLevelUpResult:		public Packet
		{
		public:
			GCLevelUpResult()	{};
			~GCLevelUpResult()	{};


			//公用接口
			virtual BOOL			Read( SocketInputStream& iStream ) ;
			virtual BOOL			Write( SocketOutputStream& oStream )const ;
			virtual UINT			Execute( Player* pPlayer ) ;

			virtual PacketID_t		GetPacketID()const { return PACKET_GC_LEVELUPRESULT ; }
			virtual UINT			GetPacketSize()const { return sizeof(LEVELUP_RESULT)+sizeof(UINT); }

			VOID					SetLevelUpResult(LEVELUP_RESULT	ret){m_Result	=	ret;}
			LEVELUP_RESULT			GetLevelUpResult(){return m_Result;}

			VOID					SetRemainExp(INT exp){m_nRemainExp	=	exp;}
			UINT					GetRemainExp()	{return m_nRemainExp;}


		private:

			LEVELUP_RESULT			m_Result;

			UINT						m_nRemainExp;
		};
		
		class GCLevelUpResultFactory:	public PacketFactory
		{
		public:
			Packet*		CreatePacket() { return new GCLevelUpResult() ; }
			PacketID_t	GetPacketID()const			{ return PACKET_GC_LEVELUPRESULT ; }
			UINT		GetPacketMaxSize()const		{ return sizeof(LEVELUP_RESULT)+sizeof(INT); }	
		};

		class GCLevelUpResultHandler
		{
		public:
			static UINT Execute(GCLevelUpResult* pPacket,Player* pPlayer);
		};

}

using namespace Packets;


#endif