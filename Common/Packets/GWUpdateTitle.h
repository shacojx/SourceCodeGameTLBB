
#ifndef _GW_UPDATE_TITLE_H_
#define _GW_UPDATE_TITLE_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GWUpdateTitle:public Packet
	{
	public:
		GWUpdateTitle(){ m_uTitleSize = 0; memset((void*)m_szTitle, 0, sizeof(m_szTitle)); }
		virtual					~GWUpdateTitle(){};

		virtual BOOL			Read( SocketInputStream& iStream );
		virtual BOOL			Write( SocketOutputStream& oStream ) const;
		virtual UINT			Execute( Player* pPlayer );

		virtual PacketID_t		GetPacketID() const { return PACKET_GW_UPDATETITLE; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(m_GUID) + sizeof(m_uTitleSize) + sizeof(CHAR) * m_uTitleSize;
		}

	public:

		VOID			SetGUID(GUID_t guid) { m_GUID = guid; }
		GUID_t			GetGUID() { return m_GUID; }

		UCHAR			GetTitleSize() { return m_uTitleSize; }

		const CHAR*		GetTitle( ) { return m_szTitle; }
		VOID			SetTitle( const CHAR* pTitle )
		{
			strncpy( m_szTitle, pTitle, MAX_CHARACTER_TITLE-1 );
			m_uTitleSize = (UCHAR)strlen(m_szTitle);
		}

	private:
		GUID_t			m_GUID;
		UCHAR			m_uTitleSize;						// ³ÆºÅÃû³Æ
		CHAR			m_szTitle[MAX_CHARACTER_TITLE];		// ³ÆºÅ

	};


	class GWUpdateTitleFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GWUpdateTitle(); }
		PacketID_t	GetPacketID() const { return PACKET_GW_UPDATETITLE; }
		UINT		GetPacketMaxSize() const { return sizeof(GUID_t)
													+ sizeof(UCHAR)
													+ sizeof(CHAR) * MAX_CHARACTER_TITLE; }
	};


	class GWUpdateTitleHandler
	{
	public:
		static UINT Execute( GWUpdateTitle* pPacket, Player* pPlayer );
	};

}

#endif // _GW_UPDATE_TITLE_H_
