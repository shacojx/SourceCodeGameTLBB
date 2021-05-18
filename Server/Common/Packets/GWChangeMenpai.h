
#ifndef _GW_CHANGE_MENPAI_H_
#define _GW_CHANGE_MENPAI_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GWChangeMenpai:public Packet
	{
	public:
		GWChangeMenpai(){};
		virtual					~GWChangeMenpai(){};

		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_GW_CHANGEMENPAI; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(GUID_t) + sizeof(INT);
		}

	public:

		VOID			SetGUID(GUID_t guid) { m_GUID = guid; }
		GUID_t			GetGUID() { return m_GUID; }

		VOID			SetMenpai(INT nMenpai) { m_nMenpai = nMenpai; }
		INT				GetMenpai() { return m_nMenpai; }


	private:
		GUID_t			m_GUID;
		INT				m_nMenpai;

	};


	class GWChangeMenpaiFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GWChangeMenpai(); }
		PacketID_t	GetPacketID() const { return PACKET_GW_CHANGEMENPAI; }
		UINT		GetPacketMaxSize() const { return sizeof(GUID_t) + sizeof(INT); }
	};


	class GWChangeMenpaiHandler
	{
	public:
		static UINT Execute( GWChangeMenpai* pPacket, Player* pPlayer );
	};

}

#endif // _GW_CHANGE_MENPAI_H_
