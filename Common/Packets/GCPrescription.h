// GCPrescription.h

#ifndef _GC_PRESCRIPTION_H_
#define _GC_PRESCRIPTION_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCPrescription:public Packet
	{

	public:
		GCPrescription(){};
		virtual				~GCPrescription(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_PRESCRIPTION; }
		virtual UINT			GetPacketSize() const { return sizeof(UINT) + sizeof(BOOL); }

	public:
		VOID			setPrescription(UINT idx) { m_Prescription = idx; }
		UINT			getPrescription() { return m_Prescription; }

		VOID			setLearnOrAbandon(BOOL bFlag) { m_LearnOrAbandon = bFlag; }
		BOOL			getLearnOrAbandon() { return m_LearnOrAbandon; }

	private:
		UINT			m_Prescription;
		BOOL			m_LearnOrAbandon;
	};

	class GCPrescriptionFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCPrescription(); }
		PacketID_t	GetPacketID() const { return PACKET_GC_PRESCRIPTION; }
		UINT		GetPacketMaxSize() const {return sizeof(UINT) + sizeof(BOOL); }
	};

	class GCPrescriptionHandler
	{
	public:
		static UINT Execute( GCPrescription* pPacket, Player* pPlayer );
	};

}

using namespace Packets;

#endif // _GC_PRESCRIPTION_H_
