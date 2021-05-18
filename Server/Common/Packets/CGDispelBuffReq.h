#ifndef CGDISPELBUFFREQ_H
#define CGDISPELBUFFREQ_H

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGDispelBuffReq:	public Packet
	{
	public:
		CGDispelBuffReq(){};
		virtual		~CGDispelBuffReq(){};

		//公用接口
		virtual BOOL		Read( SocketInputStream& iStream ) ;
		virtual BOOL		Write( SocketOutputStream& oStream )const ;
		virtual UINT		Execute( Player* pPlayer ) ;

		virtual PacketID_t	GetPacketID()const { return PACKET_CG_DISPEL_BUFF; }
		virtual UINT		GetPacketSize()const 
							{
								return 	sizeof(m_nSkillID)
									+	sizeof(m_nImpactID)
									+	sizeof(m_nSN);
							}
		SkillID_t			GetSkillID(VOID) const {return m_nSkillID;};
		VOID 				SetSkillID(SkillID_t nID) {m_nSkillID = nID;};

		ImpactID_t			GetImpactID(VOID) const {return m_nImpactID;}
		VOID				SetImpactID(ImpactID_t nID) {m_nImpactID=nID;};

		INT					GetSN(VOID) const {return m_nSN;};
		VOID				SetSN(INT nSN) {m_nSN=nSN;};
	private:
		SkillID_t			m_nSkillID;
		ImpactID_t			m_nImpactID;
		INT					m_nSN;
	};

	class CGDispelBuffReqFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGDispelBuffReq() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_CG_DISPEL_BUFF; }
		UINT		GetPacketMaxSize()const		
					{ 
						return	sizeof(SkillID_t)
							+	sizeof(ImpactID_t)
							+	sizeof(INT);
					}			
	};

	class CGDispelBuffReqHandler
	{
	public:
		static UINT Execute(CGDispelBuffReq* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif //CGDISPELBUFFREQ_H